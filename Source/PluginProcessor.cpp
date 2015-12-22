#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
BitCrusherAudioProcessor::BitCrusherAudioProcessor()
{
    NormalisableRange<float> gainRange(0.0, 1.0, 0.1, 1.0);
    NormalisableRange<float> effectRange(1.0, 100.0, 10.0, 100.0);
    NormalisableRange<float> selectRange(0, 2, 1, 1);
    NormalisableRange<float> wetDryRange(0, 1, 0.5, 1);
    
    addParameter(inputGainParam = new AudioParameterFloat("inputGainParam", "inGain" , gainRange,1.0));
    addParameter(outputGainParam = new AudioParameterFloat("outputGainParam", "outGain" , gainRange,1.0));
    addParameter(wetDryParam = new AudioParameterFloat("wetDryParam", "wetDry", wetDryRange, 0.5));
    addParameter(effectParam1 = new AudioParameterFloat("effectParam1", "Effect", effectRange, 100.0));
    addParameter(effectSelectParam = new AudioParameterFloat("selectParam", "Select", selectRange, 0));
    
    random = ((float) rand()) / (float) RAND_MAX;
}

BitCrusherAudioProcessor::~BitCrusherAudioProcessor()
{
}

//==============================================================================
const String BitCrusherAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

const String BitCrusherAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String BitCrusherAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool BitCrusherAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool BitCrusherAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool BitCrusherAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BitCrusherAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BitCrusherAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double BitCrusherAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BitCrusherAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BitCrusherAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BitCrusherAudioProcessor::setCurrentProgram (int index)
{
}

const String BitCrusherAudioProcessor::getProgramName (int index)
{
    return String();
}

void BitCrusherAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void BitCrusherAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

}

void BitCrusherAudioProcessor::releaseResources()
{

}

void BitCrusherAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    
    const int numSamples = buffer.getNumSamples();
    const float maxAmplitude = buffer.getMagnitude(0, numSamples);
    const float inGain = inputGainParam->getValue();
    const float outGain = outputGainParam->getValue();
    const float effect = effectParam1->getValue();
    const int mode = effectSelectParam->getValue();
    const int numOutputChannels = getNumOutputChannels();
    const int numInputChannels = getNumInputChannels();
    // this is only used in the sample and hold effects
    int iFactor;
    float factor;
    float delta;
    float scaler;
    
    for (int i = getNumInputChannels(); i < numOutputChannels; ++i) {
        buffer.clear (i, 0, numSamples);
    }
    
    for (int channel = 0; channel < numInputChannels; ++channel)
    {
        buffer.applyGain(inGain);
        float* channelData = buffer.getWritePointer (channel);
        
        switch (mode) {
            case SAMPLE_HOLDER:
                channelData = Decimators.sampleHold(channelData);
                break;
                
                
                iFactor = (int)(effect * effect * numSamples * 0.35);
                
                for (int i = 0; i < numSamples; i = i + iFactor){
                    for (int j = 0; j < iFactor; j++){
                        channelData[i+j] = channelData[i];
                    }
                }
                
        
            case SAMPLE_HOLDER_INT:
                channelData = Decimators.sampleHoldInt(channelData);
                
                
                iFactor = (int)(effect * effect * numSamples * 0.35);
                
                for (int i = 0; i < numSamples; i = i + iFactor){
                    
                    float delta = (channelData[i] - channelData[i + iFactor])/iFactor;
                    
                    for (int j = 0; j < iFactor; j++){
                        channelData[i+j] = channelData[i] + delta*j;
                    }
                }
                break;
            
            case CLIP_DISTORTION:
                float factor = 1 - effect;
                factor = pow(factor, 3);
                // make sure our factor is not too small
                if (factor < 0.005){factor = 0.005;};
                // make sure we keep the gain constant by utalizing our maxAmplitude
                if (factor < maxAmplitude){
                    scaler = maxAmplitude/factor;
                }

                for (int i = 0; i < numSamples; i++){
                    
                    if (channelData[i] > factor){
                        channelData[i] = factor;
                    }
                    else if (channelData[i] < -factor){
                        channelData[i] = -factor;
                    }
                    channelData[i] = channelData[i] * scaler;
                }
                break;
            
            case HARD_CLIP_DISTORTION:
                float mEffect = 1 - effect;
                for (int i = 0; i < numSamples; i++){
                    if (channelData[i] > mEffect){
                        channelData[i] = 1;
                    }
                    else if (channelData[i] < - mEffect){
                        channelData[i] = -1;
                    }
                }
                break;
            
            case HALF_RECTIFY:
                for (int i =0; i < numSamples; i++) {
                    if (channelData[i] < 0) {
                        channelData[i] = (1 - effect) * channelData[i];
                    }
                }
                break;
            
            case FULL_RECTIFY:
                for (int i = 0; i < numSamples; i++){
                    // if the sample is below 0 flip it
                    if (channelData[i] < 0){
                        channelData[i] *= -effect;
                    }
                }
                break;
            
            case SPACE_DISTORTION1:
                for (int i = 0; i < numSamples; i ++){
                    if (channelData[i] < 1 - effect ) {
                        channelData[i] = channelData[i] * (1 + 2 * effect);
                    
                        if (channelData[i] > maxAmplitude) {
                            channelData[i] = maxAmplitude;
                        }
                        else if (channelData[i] < -maxAmplitude) {
                            channelData[i] = -maxAmplitude;
                        }
                    }
                }
                break;
            
            case SPACE_DISTORTION2:
                for (int i = 0; i < numSamples; i ++){
                    // if signal is within threshold
                    if (channelData[i] < 1 - effect && channelData[i] > effect - 1) {
                        channelData[i] = channelData[i] * 5;
                        if (channelData[i] > maxAmplitude) {
                            channelData[i] = maxAmplitude;
                        }
                        else if (channelData[i] < -maxAmplitude) {
                            channelData[i] = -maxAmplitude;
                        }
                    }
                    else {
                        channelData[i] = channelData[i] * channelData[i];
                    }
                }
                break;
            
            case FUZZ1:
                for (int i = 0; i < numSamples; i++) {
                    if (channelData[i] > 0.66 * maxAmplitude) {
                        channelData[i] = maxAmplitude;
                    }
                    else if (channelData[i] < -0.66 * maxAmplitude){
                        channelData[i] = -maxAmplitude;
                    }
                    else if (channelData[i] > 0.33 * maxAmplitude) {
                        channelData[i] = (3 - pow(2 - 3 * channelData[i], 2))/3;
                    }
                    else if (channelData[i] < -0.33 *maxAmplitude) {
                        channelData[i] = (3 - pow(2 - 3 * channelData[i], 2))/3;
                    }
                    else {
                        channelData[i] = channelData[i]*2;
                    }
                }
                break;
            
            case FUZZ2:
                // this is more of an overdrive than a fuzz
                for (int i = 0; i < numSamples; i++) {
                    if (channelData[i] > 0.66) {
                        channelData[i] = 1;
                    }
                    else if (channelData[i] < -0.66){
                        channelData[i] = -1;
                    }
                    else if (channelData[i] > 0.33) {
                        channelData[i] = (3 - pow(2 - 3 * channelData[i], 2))/3;
                    }
                    else if (channelData[i] < -0.33) {
                        channelData[i] = (3 - pow(2 - 3 * channelData[i], 2))/3;
                    }
                    else {
                        channelData[i] = channelData[i]*2;
                    }
                }
                break;
                
            case DISTORTION1:
                //
                for (int i = 0; i < numSamples; i++){
                    // found this formula in a blog somehwhere, no idea what is going on...
                    channelData[i] = (channelData[i]/std::abs(channelData[i])) * (1 - pow(e - effect, (pow(channelData[i],(2/std::abs(channelData[i]))))));
                }
                break;
                
            case DISTORTION2:
                for (int i = 0; i < numSamples; i++){
                    // common nonlinear equasion for simulating distortion and fuzz
                    channelData[i] = (channelData[i]/std::abs(channelData[i])) * (1 - pow(e, effect*pow(channelData[i], 2)));
                }
                break;
                
            case CLEAN:
                // nothing its clean
                break;
            
            buffer.applyGain(outGain);
        }
        
    for (int channel = 0; channel < numOutputChannels; ++channel) {
        for (int i =0; i < numSamples; i++){
            channelData[i] = channelData[i] * 1;
        }
    }
}


//==============================================================================
bool BitCrusherAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* BitCrusherAudioProcessor::createEditor()
{
    return new BitCrusherAudioProcessorEditor (*this);
}

//==============================================================================
void BitCrusherAudioProcessor::getStateInformation (MemoryBlock& destData)
{
 
}

void BitCrusherAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    
}

//==============================================================================

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BitCrusherAudioProcessor();
}
