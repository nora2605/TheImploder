/*
  ==============================================================================

    Compressor.h
    Created: 28 Jun 2023 3:59:27pm
    Author:  noraj

  ==============================================================================
*/

#include "CircularBuffer.h"
#pragma once

class Compressor {
public:
    Compressor();
    float compressSample(float data, float thresh, float ratio, float attack, float release, float knee);
    float interpolatePoints(float* xPoints, float* yPoints, float detectedValue);

private:
    CircularBuffer buffer;
    float tav, rms, gain;
};