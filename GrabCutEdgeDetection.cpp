#include "GrabCutEdgeDetection.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <tuple>
#include <vector>

cv::Mat multi_scale_canny(const cv::Mat& image, const std::vector<double>& sigma_list) {
    cv::Mat edges_combined = cv::Mat::zeros(image.size(), CV_8U);

    for (double sigma : sigma_list) {
        cv::Mat blurred;
        int kernel_size = std::max(3, static_cast<int>(std::round(sigma * 2.0 + 1)));
        cv::GaussianBlur(image, blurred, cv::Size(kernel_size, kernel_size), sigma);
        cv::Mat edges;
        cv::Canny(blurred, edges, 50, 150);
        edges_combined = cv::max(edges_combined, edges);
    }

    cv::Mat edges_refined;
    cv::morphologyEx(edges_combined, edges_refined, cv::MORPH_CLOSE,
                     cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

    return edges_refined;
}

cv::Mat simple_grabcut_foreground(const cv::Mat& image) {
    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

    // Apply a simple threshold to create an initial binary mask
    cv::Mat mask;
    cv::threshold(gray_image, mask, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    mask.convertTo(mask, CV_64F);

    return mask;
}

cv::Mat upgraded_grabcut_foreground(const cv::Mat& image) {
    cv::Mat filtered_image;
    cv::bilateralFilter(image, filtered_image, 9, 75, 75);

    cv::Mat lab_image;
    cv::cvtColor(filtered_image, lab_image, cv::COLOR_BGR2Lab);

    std::vector<cv::Mat> lab_channels(3);
    cv::split(lab_image, lab_channels);

    cv::Mat mask;
    cv::threshold(lab_channels[1], mask, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    cv::Mat morph_mask;
    cv::morphologyEx(mask, morph_mask, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    cv::morphologyEx(morph_mask, morph_mask, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

    morph_mask.convertTo(morph_mask, CV_64F);

    return morph_mask;
}

std::tuple<cv::Mat, double, double, double> compute_foreground_background_probability(
    const cv::Mat& image_rgb, const cv::Mat& edges_refined) {

    cv::Mat fg_prob = simple_grabcut_foreground(image_rgb);
    fg_prob = (fg_prob > 0);
    fg_prob.convertTo(fg_prob, CV_64F);

    double minVal, max_prob;
    cv::minMaxLoc(fg_prob, &minVal, &max_prob);

    if (max_prob > 0) {
        fg_prob /= max_prob;
    }

    double foreground_score = cv::mean(fg_prob)[0];
    foreground_score = std::clamp(foreground_score, 0.0, 1.0);
    double background_score = 1.0 - foreground_score;

    cv::Mat edges_refined_64F;
    edges_refined.convertTo(edges_refined_64F, CV_64F);
    edges_refined_64F /= 255.0;

    double edge_weighted_fg = 0.0;
    if (cv::sum(edges_refined_64F)[0] > 0) {
        edge_weighted_fg = cv::sum(fg_prob.mul(edges_refined_64F))[0] /
                           std::max(1.0, cv::sum(edges_refined_64F)[0]);
    }

    return std::make_tuple(fg_prob, foreground_score, background_score, edge_weighted_fg);
}
