/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GouodDriveAudioProcessor::GouodDriveAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),filter(juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 200.f)), filter2(juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 200.f))
#endif
{
}

GouodDriveAudioProcessor::~GouodDriveAudioProcessor()
{
}

//==============================================================================
const juce::String GouodDriveAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GouodDriveAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GouodDriveAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GouodDriveAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GouodDriveAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GouodDriveAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GouodDriveAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GouodDriveAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GouodDriveAudioProcessor::getProgramName (int index)
{
    return {};
}

void GouodDriveAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GouodDriveAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumInputChannels();
    spec.maximumBlockSize = samplesPerBlock;

    ch = juce::dsp::Chorus<float>::Chorus();
    ch.setRate(4);
    ch.setDepth(0.8);
    ch.setCentreDelay(50);
    ch.setFeedback(0.5);
    ch.setMix(1);
    ch.prepare(spec);

    filter.coefficients = dsp::IIR::Coefficients<float>::makeFirstOrderLowPass(getSampleRate(), tone);
    filter2.coefficients = dsp::IIR::Coefficients<float>::makeFirstOrderLowPass(getSampleRate(), tone);
}

void GouodDriveAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GouodDriveAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void GouodDriveAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    juce::dsp::AudioBlock<float>block(buffer);
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    juce::dsp::ProcessContextReplacing<float> context(block);
    ch.process(context);
    auto* channelDataL = buffer.getWritePointer(0);
    auto* channelDataR = buffer.getWritePointer(1);
    for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
        channelDataL[sample] = 2 / 3.14159265359 * atan(channelDataL[sample] * gain);
        channelDataL[sample] *= output;
        channelDataL[sample] = filter.processSample(channelDataL[sample]);
        channelDataL[sample] = filter2.processSample(channelDataL[sample]);

        channelDataR[sample] = 2 / 3.14159265359 * atan(channelDataR[sample] * gain);
        channelDataR[sample] *= output;
        channelDataR[sample] = filter.processSample(channelDataR[sample]);
        channelDataR[sample] = filter2.processSample(channelDataR[sample]);
    }
    filter.coefficients = dsp::IIR::Coefficients<float>::makeFirstOrderLowPass(getSampleRate(), tone);
    filter2.coefficients = dsp::IIR::Coefficients<float>::makeFirstOrderLowPass(getSampleRate(), tone);
}

//==============================================================================
bool GouodDriveAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GouodDriveAudioProcessor::createEditor()
{
    return new GouodDriveAudioProcessorEditor (*this);
}

//==============================================================================
void GouodDriveAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GouodDriveAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GouodDriveAudioProcessor();
}
