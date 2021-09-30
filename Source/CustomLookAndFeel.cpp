#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
}
CustomLookAndFeel::~CustomLookAndFeel()
{
}

CustomLookAndFeel::EditableLabel* CustomLookAndFeel::createSliderTextBox (juce::Slider& slider)
{
    auto* l = new EditableLabel();
    l->setJustificationType (juce::Justification::centred);
    l->setColour (juce::Label::textColourId,               slider.findColour (juce::Slider::textBoxTextColourId));
    l->setColour (juce::Label::textWhenEditingColourId,    slider.findColour (juce::Slider::textBoxTextColourId));
    l->setColour (juce::Label::outlineWhenEditingColourId, slider.findColour (juce::Slider::textBoxOutlineColourId));

    // For Cube's NumberBox. Not relevant this time.
    if (slider.getHeight() > 15)
        l->setFont (18);

    return l;
}

juce::CaretComponent* CustomLookAndFeel::createCaretComponent (juce::Component* keyFocusOwner)
{
    auto caret = new juce::CaretComponent (keyFocusOwner);
    caret->setColour (juce::CaretComponent::caretColourId, juce::Colours::red);
    return caret;
}
