/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

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

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    juce::Slider sliderCutoff;
    juce::Label labelCutoff;
    
    juce::Slider sliderReson;
    juce::Label labelReson;
    
    juce::Slider sliderDrive;
    juce::Label labelDrive;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentCutoff;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentReson;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentDrive;

    LadderFilterBasicAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LadderFilterBasicAudioProcessorEditor)
};
