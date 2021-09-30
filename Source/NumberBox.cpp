/*
  ==============================================================================

    NumberBox.cpp
    Created: 30 Sep 2021 7:47:21pm
    Author:  香芋派Taro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "NumberBox.h"

NumberBox::NumberBox()
{
    setSliderStyle (juce::Slider::LinearBarVertical);
    setLookAndFeel(&customLookAndFeel);
    setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    setColour (juce::Slider::trackColourId, juce::Colours::transparentWhite);
    setTextBoxIsEditable (false);
    setVelocityBasedMode (true);
    setVelocityModeParameters (0.5, 1, 0.09, false);
    setRange (0, 100, 0.01);
    setWantsKeyboardFocus (true);
    onValueChange = [&]()
    {
        if (getValue() < 10)
            setNumDecimalPlacesToDisplay(2);
        else if (getValue() >= 10 && getValue() < 100)
            setNumDecimalPlacesToDisplay(1);
        else
            setNumDecimalPlacesToDisplay(0);
    };
}

NumberBox::~NumberBox()
{
}

void NumberBox::paint (juce::Graphics& g)
{
    if (isLockedOn)
    {
        auto length = getHeight() > 15 ? 4 : 3;
        auto thick  = getHeight() > 15 ? 3.0 : 2.5;
        
        g.setColour (findColour (juce::Slider::textBoxTextColourId));
        //          fromX       fromY        toX                  toY
        g.drawLine (0,          0,           0,                   length,               thick);
        g.drawLine (0,          0,           length,              0,                    thick);
        g.drawLine (0,          getHeight(), 0,                   getHeight() - length, thick);
        g.drawLine (0,          getHeight(), length,              getHeight(),          thick);
        g.drawLine (getWidth(), getHeight(), getWidth() - length, getHeight(),          thick);
        g.drawLine (getWidth(), getHeight(), getWidth(),          getHeight() - length, thick);
        g.drawLine (getWidth(), 0,           getWidth() - length, 0,                    thick);
        g.drawLine (getWidth(), 0,           getWidth(),          length,               thick);
    }
}

void NumberBox::mouseDown (const juce::MouseEvent& event)
{
    juce::Slider::mouseDown (event);
    setMouseCursor (juce::MouseCursor::NoCursor);
}

void NumberBox::mouseUp (const juce::MouseEvent& event)
{
    juce::Slider::mouseUp (event);
    juce::Desktop::getInstance().getMainMouseSource().setScreenPosition(event.source.getLastMouseDownPosition());
    setMouseCursor (juce::MouseCursor::NormalCursor);
}
    
void NumberBox::focusGained (juce::Component::FocusChangeType)
{
    setLockedOnState (true);
}

void NumberBox::focusLost (juce::Component::FocusChangeType)
{
    setLockedOnState (false);
}

bool NumberBox::getLockedOnState ()
{
    return isLockedOn;
}

void NumberBox::setLockedOnState (bool state)
{
    isLockedOn = state;
    repaint();
}
