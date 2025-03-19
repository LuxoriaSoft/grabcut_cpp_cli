#ifndef GRABCUT_EDGE_DETECTION_H
#define GRABCUT_EDGE_DETECTION_H

#include <opencv2/opencv.hpp>

// Define the export/import attribute depending on the platform on Windows (x86, x64, ARM64)
#if defined(_WIN32) || defined(_WIN64) || defined(_ARM64)
    #define EXPORT_DECL __declspec(dllexport)
#else
    #define EXPORT_DECL __attribute__((visibility("default")))
#endif

// Struct to hold the result
struct Result {
    double foreground_score;  // Foreground probability score
    double background_score;  // Background probability score
    double edge_weighted_fg;  // Edge-weighted foreground score
};

// Exported function to compute the foreground and background probability scores
extern "C" EXPORT_DECL Result* compute_foreground_background_probability(const char* image_path);

#endif
