#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#define SAMPLE_HOLDER 0
#define CLIP_DISTORTION 1
#define SPACE_DISTORTION 3
#define FULL_RECTIFY 4
#define HALF_RECTIFY 5

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
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BitCrusherAudioProcessor)
    int pastFactor;
    int factorDelta;
    // for keeping track of different modes
    int mode;
    float random;
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
