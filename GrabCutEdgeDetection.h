#pragma once
#include <opencv2/opencv.hpp>
#include <tuple>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

// Function to apply multi-scale Canny edge detection
__declspec(dllexport) cv::Mat multi_scale_canny(const cv::Mat& image, const std::vector<double>& sigma_list);

// Function to compute the foreground and background probabilities
__declspec(dllexport) std::tuple<cv::Mat, double, double, double> compute_foreground_background_probability(
    const cv::Mat& image_rgb, const cv::Mat& edges_refined);

#ifdef __cplusplus
}
#endif
