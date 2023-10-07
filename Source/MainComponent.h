#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

class MainComponent  : public juce::AudioProcessorEditor, juce::ComboBox::Listener
{
public:
    MainComponent (TheImploderAudioProcessor& p, juce::AudioProcessorValueTreeState& vts);
    ~MainComponent() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

    void comboBoxChanged(juce::ComboBox* sender) override;

    static const char* theImploder_png;
    static const int theImploder_pngSize;


private:
    juce::AudioProcessorValueTreeState& valueTreeState;
    TheImploderAudioProcessor& audioProcessor;

    juce::Slider preampKnob;
    juce::Slider mixKnob;
    juce::Slider thresholdBar;
    juce::Slider ratioKnob;
    juce::Slider kneeKnob;
    juce::Slider attackKnob;
    juce::Slider releaseKnob;
    juce::Slider periodKnob;
    juce::ComboBox fuckupFunctionBox;
    std::unique_ptr<SliderAttachment> preampAtm;
    std::unique_ptr<SliderAttachment> mixAtm;
    std::unique_ptr<SliderAttachment> thresholdAtm;
    std::unique_ptr<SliderAttachment> ratioAtm;
    std::unique_ptr<SliderAttachment> kneeAtm;
    std::unique_ptr<SliderAttachment> attackAtm;
    std::unique_ptr<SliderAttachment> releaseAtm;
    std::unique_ptr<SliderAttachment> periodAtm;
    std::unique_ptr<ComboBoxAttachment> fuckupAtm;

    std::unique_ptr<juce::Label> thresholdLabel;
    std::unique_ptr<juce::Label> preampLabel;
    std::unique_ptr<juce::Label> mixLabel;
    std::unique_ptr<juce::Label> ratioLabel;
    std::unique_ptr<juce::Label> kneeLabel;
    std::unique_ptr<juce::Label> fuckupLabel;
    std::unique_ptr<juce::Label> attackLabel;
    std::unique_ptr<juce::Label> releaseLabel;
    std::unique_ptr<juce::Label> periodLabel;
    juce::Image cachedImage_theImploder_png_1;
    juce::Path internalPath;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
