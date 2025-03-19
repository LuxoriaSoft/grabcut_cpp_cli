// GrabCutAlgorithm.hpp
#pragma once

#include <string>

namespace luxoria {
    namespace filter {
        namespace algorithms {

            class GrabCutAlgorithm {
            public:
                GrabCutAlgorithm();
                ~GrabCutAlgorithm();

                void Initialize(const std::string& imagePath);
                void ComputeForegroundBackgroundProbability();
                void DisplayResults();
            private:
                std::string m_imagePath;
            };

        }
    }
}
