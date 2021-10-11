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
using namespace juce;
class GALookAndFeel;
class GouodDriveAudioProcessorEditor  : public juce::AudioProcessorEditor, public Slider::Listener
{
public:
    GouodDriveAudioProcessorEditor (GouodDriveAudioProcessor&);
    ~GouodDriveAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    Slider sliderOutput;
    Slider sliderGain;
    Slider sliderTone;
    Label debug;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    void sliderValueChanged(Slider* slider);
    GouodDriveAudioProcessor& audioProcessor;
    ScopedPointer<GALookAndFeel> lookandfeel1;
    ScopedPointer<GALookAndFeel> lookandfeel2;
    ScopedPointer<GALookAndFeel> lookandfeel3;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GouodDriveAudioProcessorEditor)
};
