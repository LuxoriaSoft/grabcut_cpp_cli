#ifndef GRABCUT_EDGE_DETECTION_H
#define GRABCUT_EDGE_DETECTION_H

#include <opencv2/opencv.hpp>

// Struct to hold the result
struct Result {
    double foreground_score;  // Foreground probability score
    double background_score;  // Background probability score
    double edge_weighted_fg;  // Edge-weighted foreground score
};

// Exported function to compute the foreground and background probability scores
extern "C" __declspec(dllexport) Result* compute_foreground_background_probability(const cv::Mat& image_rgb);

#endif
