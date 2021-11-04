/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

class GALookAndFeel : public LookAndFeel_V3
{
public:
    Image knob = ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
        width *= 0.7;
        height *= 0.7;
        int radius = jmin(width, height);
        int centreX = int(width / 2)+22;
        int centreY = int(height / 2);
        float ventilCentreX = float(knob.getWidth()) * .5f;
        float ventilCentreY = float(knob.getHeight()) * .5f;
        float rotation = (MathConstants<float>::twoPi * sliderPos) - 2.3;
        if(rotation > 2.2) {
            rotation = 2.2;
        }
        //knob = knob.rescaled(radius, radius, Graphics::ResamplingQuality::highResamplingQuality);

        AffineTransform transform;
        float centreImage = float(centreX) - ventilCentreX;
        transform = transform.rotation(rotation, ventilCentreX, ventilCentreY).translated(centreImage,0);
        g.drawImageTransformed(knob, transform, false);
    }
};

using namespace juce;

//==============================================================================
GouodDriveAudioProcessorEditor::GouodDriveAudioProcessorEditor (GouodDriveAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    Image knob = ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
    sliderOutput.setSliderStyle(Slider::SliderStyle::Rotary);
    sliderGain.setSliderStyle(Slider::SliderStyle::Rotary);
    sliderTone.setSliderStyle(Slider::SliderStyle::Rotary);
    
    sliderOutput.setRange(0, 5, 0.01);
    sliderGain.setRange(0.1, 300, 0.01);
    sliderTone.setRange(20, 22000, 0.01);
    
    sliderOutput.setTextBoxStyle(Slider::NoTextBox, true, 100, 25);
    sliderGain.setTextBoxStyle(Slider::NoTextBox, true, 100, 25);
    sliderTone.setTextBoxStyle(Slider::NoTextBox, true, 100, 25);
    
    sliderGain.addListener(this);
    sliderOutput.addListener(this);
    sliderTone.addListener(this);
    
    lookandfeel1 = new GALookAndFeel;
    lookandfeel2 = new GALookAndFeel;
    lookandfeel3 = new GALookAndFeel;
    
    sliderGain.setLookAndFeel(lookandfeel1);
    sliderOutput.setLookAndFeel(lookandfeel2);
    sliderTone.setLookAndFeel(lookandfeel3);
    
    sliderOutput.setVisible(true);
    sliderGain.setVisible(true);
    sliderTone.setVisible(true);
    
    addAndMakeVisible(sliderOutput);
    addAndMakeVisible(sliderGain);
    addAndMakeVisible(sliderTone);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (370, 472);
}

GouodDriveAudioProcessorEditor::~GouodDriveAudioProcessorEditor()
{
}

//==============================================================================
void GouodDriveAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::black);

    g.setColour (juce::Colours::white);
    Image bg = ImageCache::getFromMemory(BinaryData::bg_png, BinaryData::bg_pngSize);
    g.drawImageAt(bg, 0, 0);
}
void GouodDriveAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    audioProcessor.gain = sliderGain.getValue();
    audioProcessor.output = sliderOutput.getValue();
    audioProcessor.tone = sliderTone.getValue();
}

void GouodDriveAudioProcessorEditor::resized()
{
    sliderGain.setBounds(120, 140, 120, 130);
    sliderOutput.setBounds(193, 275, 120, 130);
    sliderTone.setBounds(50, 275, 120, 130);
}
