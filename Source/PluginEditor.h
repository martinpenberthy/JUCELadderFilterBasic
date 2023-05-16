/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <string>

//==============================================================================
/**
*/
class LadderFilterBasicAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    LadderFilterBasicAudioProcessorEditor (LadderFilterBasicAudioProcessor&);
    ~LadderFilterBasicAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void modeMenuChanged();
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    juce::Slider sliderCutoff;
    juce::Label labelCutoff;
    
    juce::Slider sliderReson;
    juce::Label labelReson;
    
    juce::Slider sliderDrive;
    juce::Label labelDrive;
    
    juce::Label labelFilterType { {}, "Filter Type" };
    juce::Font textFont   { 12.0f };
    juce::ComboBox filterTypeMenu;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentCutoff;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentReson;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentDrive;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboAttachmentFilterType;
        
    LadderFilterBasicAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LadderFilterBasicAudioProcessorEditor)
    
};
