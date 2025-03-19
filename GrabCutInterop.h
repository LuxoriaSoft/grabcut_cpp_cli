// GrabCutInterop.h
#pragma once

using namespace System;

namespace GrabCutInterop {

    public ref class GrabCutWrapper
    {
    public:
        GrabCutWrapper();
        ~GrabCutWrapper();

        // Method to initialize the GrabCut algorithm (in C++)
        void InitializeGrabCutAlgorithm(String^ imagePath);

        // Method to compute the foreground and background probabilities
        void ComputeForegroundBackgroundProbabilities();

        // Method to display the results
        void DisplayResults();

    private:
        luxoria::filter::algorithms::GrabCutAlgorithm* m_grabCutAlgorithm;
    };
}
