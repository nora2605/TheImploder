#include "PluginProcessor.h"
#include "MainComponent.h"
#include <math.h>
#include <numbers>

TheImploderAudioProcessor::TheImploderAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    params(*this, nullptr, juce::Identifier("parameters"), {
        std::make_unique<juce::AudioParameterFloat> ("preamp",   "PreAmp", juce::NormalisableRange<float>(0.0f, 10.0f, 0.01f), 1.0f),
        std::make_unique<juce::AudioParameterFloat> ("mix",      "Mix", juce::NormalisableRange<float>(0.0f, 1.0f, 0.005f), 1.0f),
        std::make_unique<juce::AudioParameterFloat> ("threshold","Threshold", juce::NormalisableRange<float>(-60.0f, 20.0f, 0.01f), 10.0f),
        std::make_unique<juce::AudioParameterFloat> ("ratio",    "Ratio", juce::NormalisableRange<float>(1.0f, 20.0f, 0.01f), 2.0f),
        std::make_unique<juce::AudioParameterFloat> ("knee",     "Knee Width", juce::NormalisableRange<float>(0.0f, 24.0f, 0.01f), 0.0f),
        std::make_unique<juce::AudioParameterFloat> ("attack",   "Attack", juce::NormalisableRange<float>(0.01f, 500.0f, 0.01f), 100.0f),
        std::make_unique<juce::AudioParameterFloat> ("release",  "Release", juce::NormalisableRange<float>(0.01f, 2000.0f, 0.01f), 500.0f),
        std::make_unique<juce::AudioParameterInt> ("fuckup",   "Imploder Function", 1, 6, 1),
        std::make_unique<juce::AudioParameterFloat> ("period", "Imploder Period", juce::NormalisableRange<float>(0.05f, 10.0f, 0.01f), 1.0f)
    })
{

}

TheImploderAudioProcessor::~TheImploderAudioProcessor()
{
    
}

const juce::String TheImploderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TheImploderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TheImploderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TheImploderAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TheImploderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TheImploderAudioProcessor::getNumPrograms()
{
    return 1;
}

int TheImploderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TheImploderAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TheImploderAudioProcessor::getProgramName (int index)
{
    return {};
}

void TheImploderAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void TheImploderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (int c = 0; c < getNumOutputChannels(); c++) {
        compressors.add(Compressor());
    }
    preamp = params.getRawParameterValue("preamp");
    mix = params.getRawParameterValue("mix");
    thresh = params.getRawParameterValue("threshold");
    ratio = params.getRawParameterValue("ratio");
    knee = params.getRawParameterValue("knee");
    attack = params.getRawParameterValue("attack");
    release = params.getRawParameterValue("release");
    fuckup = params.getRawParameterValue("fuckup");
    period = params.getRawParameterValue("period");
}

void TheImploderAudioProcessor::releaseResources()
{
    
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TheImploderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TheImploderAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    float at = 1 - std::pow(std::numbers::e, (-2200.0f / (getSampleRate() * *attack)));
    float rt = 1 - std::pow(std::numbers::e, (-2200.0f / (getSampleRate() * *release)));

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        for (int c = 0; c < getTotalNumOutputChannels(); c++) {
            auto* data = buffer.getWritePointer(c);
            Compressor* cmp = &compressors.getReference(c);
            data[i] = (1 - *mix) * data[i] + *mix * cmp->compressSample(*preamp * data[i] * std::abs(implode(getSampleRate())), *thresh, *ratio, at, rt, *knee);
        }
    }
}

float TheImploderAudioProcessor::implode(float sps) {
    sps *= *period;
    int id = *fuckup;

    auto expb = [](float x) {
        return std::exp(-25.0f * (x - 1 / 2.0f) * (x - 1 / 2.0f));
    };

    if (implodeSample > sps) implodeSample -= sps;
    switch (id) {
    case 1: return 1.0f;
    case 2: return 10 * std::sin(implodeSample++ / sps * 2 * std::numbers::pi);
    case 3: return 10 * expb(implodeSample++ / sps);
    case 4: return std::tan(implodeSample++ / sps * 2 * std::numbers::pi);
    case 5: return pcf(40 * implodeSample++ / sps + 2);
    case 6: return (int)lgamma(10 * implodeSample++ / sps + 1) % 10;
    default: return 1.0f;
    }
}

float TheImploderAudioProcessor::pcf(float x) {
    return x/log(x);
}
bool TheImploderAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* TheImploderAudioProcessor::createEditor()
{
    return new MainComponent (*this, params);
}

void TheImploderAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto stateTree = params.copyState();
    std::unique_ptr<juce::XmlElement> xml(stateTree.createXml());
    copyXmlToBinary(*xml, destData);
}

void TheImploderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr && xmlState->hasTagName(params.state.getType())) {
        params.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TheImploderAudioProcessor();
}
