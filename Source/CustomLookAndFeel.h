/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 13 Dec 2015 5:43:56pm
    Author:  Nathan Villicaña-Shaw

  ==============================================================================
*/

#ifndef CUSTOMLOOKANDFEEL_H_INCLUDED
#define CUSTOMLOOKANDFEEL_H_INCLUDED

#include "JuceHeader.h"

class CustomLookAndFeel : public LookAndFeel_V3
{
public:
    CustomLookAndFeel();
    ~CustomLookAndFeel();
    
    void drawRotarySlider (Graphics & g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotartStartAngle,
                           float rotartEndAngle, Slider & slider);
private:
    
};

#endif  // CUSTOMLOOKANDFEEL_H_INCLUDED
