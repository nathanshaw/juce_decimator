/*
  ==============================================================================

    DistortionEffects.h
    Created: 22 Dec 2015 10:02:15am
    Author:  Nathan Villicaña-Shaw

  ==============================================================================
*/

#ifndef DISTORTIONEFFECTS_H_INCLUDED
#define DISTORTIONEFFECTS_H_INCLUDED

class Distorters
{
public:
    Distorters();
    ~Distorters();
    
    float* fuzz1(float* channelSamples, float amount);
    float* fuzz2(float* channelSamples, float amount);
    float* distortion1(float* channelSamples, float amount);
    float* distortion2(float* channelSamples, float amount);
    float* spaceDistortion1(float* channelSamples, float amount);
    float* spaceDistortion2(float* channelSamples, float amount);
    float* clipDistortion(float* channelSamples, float amount);
    float* hardClipDistortion(float* channelSamples, float amount);
    float* halfWaveRecify(float* channelSamples, float amount);
    float* fullWaveRecify(float* channelSamples, float amount);
    
private:
    
};



#endif  // DISTORTIONEFFECTS_H_INCLUDED
