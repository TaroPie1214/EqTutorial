/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChainSettings getChainSettings (juce::AudioProcessorValueTreeState& apvts)
{
    ChainSettings settings;
    
    settings.lowCutFreq = apvts.getRawParameterValue ("LowCut Freq")->load();
    settings.lowCutQuality = apvts.getRawParameterValue ("LowCut Quality")->load();
    settings.highCutFreq = apvts.getRawParameterValue ("HighCut Freq")->load();
    settings.highCutQuality = apvts.getRawParameterValue ("HighCut Quality")->load();

    return settings;
}

EqTutorialAudioProcessor::EqTutorialAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

EqTutorialAudioProcessor::~EqTutorialAudioProcessor()
{
}

//==============================================================================
const juce::String EqTutorialAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EqTutorialAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EqTutorialAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EqTutorialAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EqTutorialAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EqTutorialAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EqTutorialAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EqTutorialAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String EqTutorialAudioProcessor::getProgramName (int index)
{
    return {};
}

void EqTutorialAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void EqTutorialAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    spec.sampleRate = sampleRate;
    
    leftChain.prepare(spec);
    rightChain.prepare(spec);
    
    updateFilters();
}

void EqTutorialAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EqTutorialAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void EqTutorialAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    updateFilters();
    
    juce::dsp::AudioBlock<float> block (buffer);
    
    auto leftBlock = block.getSingleChannelBlock (0);
    auto rightBlock = block.getSingleChannelBlock (1);
    
    juce::dsp::ProcessContextReplacing<float> leftContext (leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext (rightBlock);
    
    leftChain.process (leftContext);
    rightChain.process (rightContext);
}

//==============================================================================
bool EqTutorialAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* EqTutorialAudioProcessor::createEditor()
{
    return new EqTutorialAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void EqTutorialAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EqTutorialAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EqTutorialAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout EqTutorialAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add (std::make_unique<juce::AudioParameterFloat> ("LowCut Freq",
                                                             "LowCut Freq",
                                                             juce::NormalisableRange<float> (20.f, 20000.f, 1.f, 0.25f),
                                                             20.f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) { return (value < 1000.0f) ?
                                                                 juce::String (value, 0) + " Hz" :
                                                                 juce::String (value / 1000.0f, 1) + " kHz"; },
                                                             nullptr));
    
    layout.add (std::make_unique<juce::AudioParameterFloat> ("LowCut Quality",
                                                             "LowCut Quality",
                                                             juce::NormalisableRange<float> (0.1f, 10.f, 0.01f, 0.25f),
                                                             0.71f));
    
    layout.add (std::make_unique<juce::AudioParameterFloat> ("HighCut Freq",
                                                             "HighCut Freq",
                                                             juce::NormalisableRange<float> (20.f, 20000.f, 1.f, 0.25f),
                                                             20000.f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) { return (value < 1000.0f) ?
                                                                 juce::String (value, 0) + " Hz" :
                                                                 juce::String (value / 1000.0f, 1) + " kHz"; },
                                                             nullptr));
    
    layout.add (std::make_unique<juce::AudioParameterFloat> ("HighCut Quality",
                                                             "HighCut Quality",
                                                             juce::NormalisableRange<float> (0.1f, 10.f, 0.01f, 0.25f),
                                                             0.71f));
    
    return layout;
}

void EqTutorialAudioProcessor::updateLowCutFilter (const ChainSettings& chainSettings)
{
    auto lowCutCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass (getSampleRate(),
                                                                                 chainSettings.lowCutFreq,
                                                                                 chainSettings.lowCutQuality);
        
    *leftChain.get<ChainPositions::LowCut>().coefficients = *lowCutCoefficients;
    *rightChain.get<ChainPositions::LowCut>().coefficients = *lowCutCoefficients;
}

void EqTutorialAudioProcessor::updateHighCutFilter (const ChainSettings& chainSettings)
{
    auto highCutCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass (getSampleRate(),
                                                                                 chainSettings.highCutFreq,
                                                                                 chainSettings.highCutQuality);
    
    *leftChain.get<ChainPositions::HighCut>().coefficients = *highCutCoefficients;
    *rightChain.get<ChainPositions::HighCut>().coefficients = *highCutCoefficients;
}

void EqTutorialAudioProcessor::updateFilters()
{
    auto chainSettings = getChainSettings (apvts);
    
    updateLowCutFilter (chainSettings);
    updateHighCutFilter (chainSettings);
}
