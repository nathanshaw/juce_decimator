#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
BitCrusherAudioProcessor::BitCrusherAudioProcessor()
{
    NormalisableRange<float> gainRange(0.0, 1.0, 0.1, 1.0);
    NormalisableRange<float> effectRange(1.0, 100.0, 10.0, 100.0);
    NormalisableRange<float> selectRange(0, 2, 1, 1);
    
    addParameter(inputGainParam = new AudioParameterFloat("inputGainParam", "inGain" , gainRange,1.0));
    addParameter(outputGainParam = new AudioParameterFloat("outputGainParam", "outGain" , gainRange,1.0));
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
    const float inGain = inputGainParam->getValue();
    const float outGain = outputGainParam->getValue();
    const float effect = effectParam1->getValue();
    const int mode = effectSelectParam->getValue();
    
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, numSamples);
    
    for (int channel = 0; channel < getNumInputChannels(); ++channel)
    {
        buffer.applyGain(inGain);
        float* channelData = buffer.getWritePointer (channel);
        
        if (mode == SAMPLE_HOLDER){
            //   map this to a knob
            int interpolation = 0;
            int iFactor = (int)(effect * effect * numSamples * 0.35);
            
            for (int i = iFactor; i < numSamples; i = i + iFactor){
                for (int j = 0; j < iFactor; j++){
                    if (interpolation){
                        channelData[i+j] = ((channelData[i] * j/iFactor) + (1 - channelData[i + iFactor] * j/iFactor)) * 0.5;
                    }
                    else{
                        channelData[i+j] = channelData[i];
                    }
                }
            }
        }
        else if ( mode == CLIP_DISTORTION){
            float factor = 1 - effect;
            factor = factor * factor * factor;
            
            if (factor < 0.01)
            {
                factor = 0.01;
            }
 
            buffer.applyGain(2 - factor);

            for (int i = 0; i < numSamples; i++){
                
              
                if (channelData[i] > factor){
                    channelData[i] = factor;
                }
                else if (channelData[i] < -factor){
                    channelData[i] = -factor;
                }
            }
            
            if (effect > 0.7){
                buffer.applyGain(inGain*(3.6 - factor));
            }
            else{
                buffer.applyGain(inGain*(2 - factor));
            }
        }
        else if ( mode == HALF_RECTIFY) {
            for (int i =0; i < numSamples; i++) {
                if (channelData[i] < 0) {
                    channelData[i] = (1 - effect) * channelData[i];
                }
            }
        }
        
        else if ( mode == FULL_RECTIFY){
            for (int i = 0; i < numSamples; i++){
                // if the sample is below 0 flip it
                if (channelData[i] < 0){
                    channelData[i] *= -effect;
                }
            }
        }
        
        else if ( mode == SPACE_DISTORTION) {
            
            for (int i = 0; i < numSamples; i ++){
                if (channelData[i] < 1 - effect ) {
                    channelData[i] = channelData[i] * 2;
                }
            }
        }
        buffer.applyGain(outGain);
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
