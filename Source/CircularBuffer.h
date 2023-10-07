/*
  ==============================================================================

    CircularBuffer.h
    Created: 27 Jun 2023 11:00:53pm
    Author:  noraj

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class CircularBuffer {
public:
    CircularBuffer();
    CircularBuffer(int bufferSize, int delayLength);
    float getData();
    void setData(float data);
    void nextSample();
private:
    juce::AudioSampleBuffer buffer;
    int writeIndex;
    int readIndex;
    int delayLength;
};