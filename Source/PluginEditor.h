#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"


//==============================================================================
/**
*/
class BitCrusherAudioProcessorEditor  : public AudioProcessorEditor,
                                    Slider::Listener,
                                    Button::Listener,
                                    Timer
{
public:
    BitCrusherAudioProcessorEditor (BitCrusherAudioProcessor&);
    ~BitCrusherAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void sliderValueChanged (Slider* sliderThatHasChanged) override;
    void buttonClicked (Button* buttonThatHasBeenClicked) override;

    void timerCallback() override;

private:
    TextButton cleanRadio;
    TextButton holdRadio;
    TextButton holdIntRadio;
    TextButton halfRectifyRadio;
    TextButton fullRectifyRadio;
    TextButton clipRadio;
    TextButton hardClipRadio;
    TextButton spaceRadio1;
    TextButton spaceRadio2;
    TextButton fuzzRadio1;
    TextButton fuzzRadio2;
    TextButton distortionRadio1;
    TextButton distortionRadio2;
    // Create a slider objects
    Slider inputGainSlider;
    Slider outputGainSlider;
    Slider effectSlider1;
    
    Label inputGainSliderLabel;
    Label effectSlider1Label;
    Label outputGainSliderLabel;
    
    // for customixing my own look
    CustomLookAndFeel myLookAndFeel;

    BitCrusherAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BitCrusherAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
