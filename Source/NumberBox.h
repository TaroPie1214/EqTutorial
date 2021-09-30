/*
  ==============================================================================

    NumberBox.h
    Created: 30 Sep 2021 7:47:21pm
    Author:  香芋派Taro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

class NumberBox  : public juce::Slider
{
public:
    NumberBox();
    ~NumberBox() override;

    void paint (juce::Graphics&) override;
    
    void mouseDown (const juce::MouseEvent& event) override;
    void mouseUp (const juce::MouseEvent& event) override;

    void focusGained (juce::Component::FocusChangeType) override;
    void focusLost (juce::Component::FocusChangeType) override;

    bool getLockedOnState ();
    void setLockedOnState (bool state);

private:
    bool isLockedOn = false;

    CustomLookAndFeel customLookAndFeel;
    
    juce::Colour grey = juce::Colour::fromFloatRGBA(0.42, 0.42, 0.42, 1.0);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NumberBox)
};
