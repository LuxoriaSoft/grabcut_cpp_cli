#include "GrabCutEdgeDetection.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

// Helper function: Simple GrabCut (based on thresholding)
cv::Mat simple_grabcut_foreground(const cv::Mat& image) {
    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    
    // Apply a simple threshold to create an initial binary mask
    cv::Mat mask;
    cv::threshold(gray_image, mask, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    
    // Convert the mask to double for further processing
    mask.convertTo(mask, CV_64F);
    return mask;
}

// Helper function: Multi-scale Canny edge detection
cv::Mat multi_scale_canny(const cv::Mat& image, const std::vector<double>& sigma_list = {1.0, 2.0, 3.0}) {
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

// Helper function: Compute the foreground and background probability scores
extern "C" EXPORT_DECL Result* compute_foreground_background_probability(const char* image_path) {
    Result* result = new Result;

    // Load the image from the file path
    cv::Mat image_rgb = cv::imread(image_path, cv::IMREAD_COLOR);
    
    if (image_rgb.empty()) {
        std::cerr << "Error: Unable to load image from path: " << image_path << std::endl;
        return nullptr;
    }

    // Apply multi-scale Canny edge detection
    cv::Mat gray_image;
    cv::cvtColor(image_rgb, gray_image, cv::COLOR_BGR2GRAY);
    cv::Mat edges_refined = multi_scale_canny(gray_image);

    // Apply simple GrabCut method for foreground detection
    cv::Mat fg_prob = simple_grabcut_foreground(image_rgb);
    fg_prob = (fg_prob > 0); // Binary mask
    fg_prob.convertTo(fg_prob, CV_64F);

    // Compute foreground score (mean of the mask)
    double foreground_score = cv::mean(fg_prob)[0];
    foreground_score = std::clamp(foreground_score, 0.0, 1.0);
    result->foreground_score = foreground_score;

    // Compute background score (1 - foreground score)
    result->background_score = 1.0 - foreground_score;

    // Compute edge-weighted foreground score
    cv::Mat edges_refined_64F;
    edges_refined.convertTo(edges_refined_64F, CV_64F);
    edges_refined_64F /= 255.0;

    double edge_weighted_fg = 0.0;
    if (cv::sum(edges_refined_64F)[0] > 0) {
        edge_weighted_fg = cv::sum(fg_prob.mul(edges_refined_64F))[0] /
                           std::max(1.0, cv::sum(edges_refined_64F)[0]);
    }
    result->edge_weighted_fg = edge_weighted_fg;

    return result;
}
