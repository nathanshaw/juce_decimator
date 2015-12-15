/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 13 Dec 2015 5:43:56pm
    Author:  Nathan Villicaña-Shaw

  ==============================================================================
*/


#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel(){
    setColour(Slider::rotarySliderFillColourId, Colours::darkgrey);
    setColour(Slider::rotarySliderOutlineColourId, Colours::black);
}

CustomLookAndFeel::~CustomLookAndFeel(){
    
}

// The version uses an image generated from knobman
// Based on https://github.com/audioplastic/Juce-look-and-feel-examples and JUCE Forum
void CustomLookAndFeel::drawRotarySlider (Graphics & g, int x, int y, int width, int height,  float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider & slider)
{
    // Load the image from binary data efficiently
    
    Image myStrip = ImageCache::getFromMemory(BinaryData::GlitchKnob2_png, BinaryData::GlitchKnob2_pngSize);
    
    //(val-min) / (max-min)
    const double fractRotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum()-slider.getMinimum());
    
    const int numFrames = myStrip.getHeight() / myStrip.getWidth();
    const int frameIdx = (int)ceil(fractRotation * (numFrames-1.0));
    
    const float radius = jmin(width*0.5, height * 0.5);
    const float centerX = x+width * 0.5;
    const float centerY = y+height * 0.5;
    const float rx = centerX - radius;
    const float ry = centerY - radius;
    
    g.drawImage(myStrip, rx, ry, 2*radius, 2*radius, 0, frameIdx*myStrip.getWidth(), myStrip.getWidth(), myStrip.getWidth());
    
}
