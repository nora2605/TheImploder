/*
  ==============================================================================

    CircularBuffer.cpp
    Created: 27 Jun 2023 11:00:53pm
    Author:  noraj

  ==============================================================================
*/

#include "CircularBuffer.h"

CircularBuffer::CircularBuffer() {
    buffer = juce::AudioSampleBuffer();
    writeIndex = readIndex = delayLength = 0;
}

CircularBuffer::CircularBuffer(int bufferSize, int delayLength) {
    buffer = juce::AudioSampleBuffer(1, bufferSize);
    buffer.clear();
    writeIndex = delayLength;
    readIndex = 0;
    this->delayLength = delayLength;
}

float CircularBuffer::getData() {
    return buffer.getSample(0, readIndex);
}

void CircularBuffer::setData(float data) {
    buffer.setSample(0, writeIndex, data);
}

void CircularBuffer::nextSample() {
    int bufferLength = buffer.getNumSamples();
    readIndex = (bufferLength + writeIndex - delayLength) % bufferLength;
    writeIndex = (writeIndex + 1) % bufferLength;
}