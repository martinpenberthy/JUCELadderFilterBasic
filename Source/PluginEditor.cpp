/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LadderFilterBasicAudioProcessorEditor::LadderFilterBasicAudioProcessorEditor (LadderFilterBasicAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 400);
    
    addAndMakeVisible(sliderCutoff); //CUTOFF
    addAndMakeVisible(sliderReson); //RESONANCE
    addAndMakeVisible(sliderDrive); //DRIVE
    addAndMakeVisible(labelFilterType);
    addAndMakeVisible(filterTypeMenu);
    
    for(int i = 0; i < 6; i++)
        filterTypeMenu.addItem(p.filterTypes[i], i+1);
    
    filterTypeMenu.onChange = [this] { modeMenuChanged(); };
    filterTypeMenu.setSelectedId (1);
    
    labelFilterType.attachToComponent(&filterTypeMenu, false);
    labelFilterType.setFont(textFont);
    labelFilterType.setColour(juce::Label::textColourId, juce::Colours::white);
    
    
    
    sliderCutoff.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sliderCutoff.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 38);
    sliderCutoff.setDoubleClickReturnValue(true, 2000.0f);
    sliderCutoff.setSkewFactorFromMidPoint(500.0f);
    labelCutoff.attachToComponent(&sliderCutoff, false);
    labelCutoff.setText("Cutoff(Hz)", juce::dontSendNotification);
    
    sliderReson.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sliderReson.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 38);
    sliderReson.setDoubleClickReturnValue(true,0.0f);
    labelReson.attachToComponent(&sliderReson, false);
    labelReson.setText("Res\n(0-.75)", juce::dontSendNotification);
    
    sliderDrive.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sliderDrive.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 38);
    sliderDrive.setDoubleClickReturnValue(true,0.0f);
    labelDrive.attachToComponent(&sliderDrive, false);
    labelDrive.setText("Drive\n1-10", juce::dontSendNotification);
    
    
    sliderAttachmentCutoff = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CUTOFF", sliderCutoff);
    
    sliderAttachmentReson = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RESONANCE", sliderReson);
    
    sliderAttachmentDrive = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DRIVE", sliderDrive);
    
    comboAttachmentFilterType = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "TYPE", filterTypeMenu);

}

LadderFilterBasicAudioProcessorEditor::~LadderFilterBasicAudioProcessorEditor()
{
}

//==============================================================================
void LadderFilterBasicAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void LadderFilterBasicAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    sliderCutoff.setBounds(getWidth()/2-150, getHeight()/2, 75, 200);
    sliderReson.setBounds(getWidth()/2-50, getHeight()/2, 75, 200);
    sliderDrive.setBounds(getWidth()/2+50, getHeight()/2, 75, 200);
    filterTypeMenu.setBounds(getWidth()/2 - 75, getHeight() - 350, 100, 25);
}

void LadderFilterBasicAudioProcessorEditor::modeMenuChanged()
{
    //{"LPF12", "HPF12", "BPF12", "LPF24", "HPF24", "BPF24"}
    switch (filterTypeMenu.getSelectedId())
    {
        case 1:
            audioProcessor.filterMode = juce::dsp::LadderFilterMode::LPF12;
            break;
        case 2:
            audioProcessor.filterMode = juce::dsp::LadderFilterMode::HPF12;
            break;
        case 3:
            audioProcessor.filterMode = juce::dsp::LadderFilterMode::BPF12;
            break;
        case 4:
            audioProcessor.filterMode = juce::dsp::LadderFilterMode::LPF24;
            break;
        case 5:
            audioProcessor.filterMode = juce::dsp::LadderFilterMode::HPF24;
            break;
        case 6:
            audioProcessor.filterMode = juce::dsp::LadderFilterMode::BPF24;
            break;
            
        default:
            audioProcessor.filterMode = juce::dsp::LadderFilterMode::LPF12;
            break;
    }

}
