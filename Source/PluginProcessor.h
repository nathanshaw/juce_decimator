#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "DistortionEffects.h"
#include "DecimationEffects.h"

#define SAMPLE_HOLDER 0
#define SAMPLE_HOLDER_INT 1
#define CLIP_DISTORTION 2
#define HARD_CLIP_DISTORTION 3
#define SPACE_DISTORTION1 4
#define SPACE_DISTORTION2 5
#define HALF_RECTIFY 6
#define FULL_RECTIFY 7
#define FUZZ1 8
#define FUZZ2 9
#define DISTORTION1 10
#define DISTORTION2 11
#define CLEAN 12
//==============================================================================
/**
*/
class BitCrusherAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    BitCrusherAudioProcessor();
    ~BitCrusherAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    AudioProcessorParameter* inputGainParam;
    AudioProcessorParameter* outputGainParam;
    AudioProcessorParameter* effectParam1;
    AudioProcessorParameter* effectParam2;
    AudioProcessorParameter* effectSelectParam;
    AudioProcessorParameter* wetDryParam;
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BitCrusherAudioProcessor)
    int pastFactor;
    int factorDelta;
    // for keeping track of different modes
    int mode;
    float random;
    const float e = 2.71828182845904523536;
    // instances of our actual effects processors :
    Decimators = new;
    Distorters = new;

};


#endif  // PLUGINPROCESSOR_H_INCLUDED
