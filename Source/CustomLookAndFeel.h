/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 30 Sep 2021 8:04:47pm
    Author:  香芋派Taro

  ==============================================================================
*/

#pragma once

class CustomLookAndFeel  : public juce::LookAndFeel_V4
{
public:
    struct EditableLabel : public juce::Label
    {
        EditableLabel() {};
        ~EditableLabel() {};
        
        juce::TextEditor* createEditorComponent() override
        {
            auto* ed = juce::Label::createEditorComponent();
            ed->setJustification (juce::Justification::centred);
            ed->setColour (juce::TextEditor::backgroundColourId, juce::Colour::fromFloatRGBA (0.078, 0.078, 0.078, 1.0));
            ed->setMouseCursor (juce::MouseCursor::NoCursor);
            ed->setInputRestrictions (5, "0123456789.");
            ed->setIndents (4, -1);
            return ed;
        }
        
        void editorShown (juce::TextEditor* editor) override
        {
            editor->clear();
        }
    };
    
    CustomLookAndFeel();
    ~CustomLookAndFeel() override;
    
    
    EditableLabel* createSliderTextBox (juce::Slider& slider) override;

    juce::CaretComponent* createCaretComponent (juce::Component* keyFocusOwner) override;
private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomLookAndFeel)
};
