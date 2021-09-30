/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EqTutorialAudioProcessorEditor::EqTutorialAudioProcessorEditor (EqTutorialAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
      lowCutFreqAttachment (audioProcessor.apvts, "LowCut Freq", lowCutFreqBox),
      lowCutQualityAttachment (audioProcessor.apvts, "LowCut Quality", lowCutQualityBox),
      highCutFreqAttachment (audioProcessor.apvts, "HighCut Freq", highCutFreqBox),
      highCutQualityAttachment (audioProcessor.apvts, "HighCut Quality", highCutQualityBox)
{
    setSize (300, 150);
    setWantsKeyboardFocus (true);
    
    lowCutFreqBox.setColour (juce::Slider::textBoxTextColourId, blue);
    lowCutQualityBox.setColour (juce::Slider::textBoxTextColourId, blue.darker(0.3));

    highCutFreqBox.setColour (juce::Slider::textBoxTextColourId, yellow);
    highCutQualityBox.setColour (juce::Slider::textBoxTextColourId, yellow.darker(0.3));
    
    addAndMakeVisible (lowCutFreqBox);
    addAndMakeVisible (lowCutQualityBox);
    addAndMakeVisible (highCutFreqBox);
    addAndMakeVisible (highCutQualityBox);
}

EqTutorialAudioProcessorEditor::~EqTutorialAudioProcessorEditor()
{
}

//==============================================================================
void EqTutorialAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (black);
}

void EqTutorialAudioProcessorEditor::resized()
{
    lowCutFreqBox.setBounds (35, getHeight() / 2 - 20, 80, 20);
    lowCutQualityBox.setBounds (35, getHeight() / 2 + 10, 80, 20);
    
    highCutFreqBox.setBounds (185, getHeight() / 2 - 20, 80, 20);
    highCutQualityBox.setBounds (185, getHeight() / 2 + 10, 80, 20);
}
