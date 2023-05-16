/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LadderFilterBasicAudioProcessor::LadderFilterBasicAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, juce::Identifier("Parameters"), createParameters() )//Constructor for apvts
#endif
{
    apvts.state.addListener(this);
    
}

LadderFilterBasicAudioProcessor::~LadderFilterBasicAudioProcessor()
{
}

//==============================================================================
const juce::String LadderFilterBasicAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LadderFilterBasicAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LadderFilterBasicAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LadderFilterBasicAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LadderFilterBasicAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LadderFilterBasicAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LadderFilterBasicAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LadderFilterBasicAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LadderFilterBasicAudioProcessor::getProgramName (int index)
{
    return {};
}

void LadderFilterBasicAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LadderFilterBasicAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    cutoffFreq = apvts.getRawParameterValue("CUTOFF")->load();
    res = apvts.getRawParameterValue("RESONANCE")->load();
    drive = apvts.getRawParameterValue("DRIVE")->load();
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    Filter.prepare(spec);
    Filter.setEnabled(true);
    Filter.setMode(juce::dsp::LadderFilterMode::LPF24);
    Filter.setCutoffFrequencyHz(cutoffFreq);
    Filter.setResonance(res);
    Filter.setDrive(drive);
}

void LadderFilterBasicAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LadderFilterBasicAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void LadderFilterBasicAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block (buffer);
    
    //Check and set cutoff
    float cutoffFreqNew = apvts.getRawParameterValue("CUTOFF")->load();
    if(cutoffFreq != cutoffFreqNew)
        Filter.setCutoffFrequencyHz(cutoffFreqNew);
    
    //Check and set resonance
    float resNew = apvts.getRawParameterValue("RESONANCE")->load();
    if(res != resNew)
        Filter.setResonance(resNew);
    
    //Check and set drive
    float driveNew = apvts.getRawParameterValue("DRIVE")->load();
    if(drive != driveNew)
        Filter.setDrive(driveNew);
    
    
    Filter.process(juce::dsp::ProcessContextReplacing<float> (block));
}

juce::AudioProcessorValueTreeState::ParameterLayout LadderFilterBasicAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    params.add(std::make_unique<juce::AudioParameterFloat>("CUTOFF", "Cutoff",
                                                           juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.2f),
                                                           2000.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("RESONANCE", "Resonance", 0.0f, 0.75f, 0.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("DRIVE", "Drive", 1.0f, 10.0f, 1.0f));

    
    return params;
}



//==============================================================================
bool LadderFilterBasicAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LadderFilterBasicAudioProcessor::createEditor()
{
    return new LadderFilterBasicAudioProcessorEditor (*this);
}

//==============================================================================
void LadderFilterBasicAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void LadderFilterBasicAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LadderFilterBasicAudioProcessor();
}
