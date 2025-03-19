// GrabCutInterop.cpp
#include "GrabCutInterop.h"
#include "GrabCutAlgorithm.hpp"
#include <msclr/marshal_cppstd.h>  // To convert between managed and unmanaged types

namespace GrabCutInterop {

    GrabCutWrapper::GrabCutWrapper()
    {
        m_grabCutAlgorithm = new luxoria::filter::algorithms::GrabCutAlgorithm();
    }

    GrabCutWrapper::~GrabCutWrapper()
    {
        delete m_grabCutAlgorithm;
    }

    void GrabCutWrapper::InitializeGrabCutAlgorithm(String^ imagePath)
    {
        // Convert managed String^ to native std::string
        std::string nativeImagePath = msclr::interop::marshal_as<std::string>(imagePath);
        m_grabCutAlgorithm->Initialize(nativeImagePath);
    }

    void GrabCutWrapper::ComputeForegroundBackgroundProbabilities()
    {
        m_grabCutAlgorithm->ComputeForegroundBackgroundProbability();
    }

    void GrabCutWrapper::DisplayResults()
    {
        m_grabCutAlgorithm->DisplayResults();
    }

}
