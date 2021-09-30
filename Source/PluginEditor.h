/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "NumberBox.h"

class EqTutorialAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    EqTutorialAudioProcessorEditor (EqTutorialAudioProcessor&);
    ~EqTutorialAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //void mouseDown (const juce::MouseEvent& event) override;

private:
    EqTutorialAudioProcessor& audioProcessor;
    
    NumberBox lowCutFreqBox;
    NumberBox lowCutQualityBox;
        
    NumberBox highCutFreqBox;
    NumberBox highCutQualityBox;
    
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    Attachment lowCutFreqAttachment,
               lowCutQualityAttachment,
               highCutFreqAttachment,
               highCutQualityAttachment;
    
    juce::Colour blue = juce::Colour::fromFloatRGBA (0.4, 0.8, 1.0, 1.0);
    juce::Colour yellow = juce::Colour::fromFloatRGBA (1.0, 0.7, 0.2, 1.0);
    juce::Colour black = juce::Colour::fromFloatRGBA (0.08, 0.08, 0.08, 1.0);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqTutorialAudioProcessorEditor)
};
