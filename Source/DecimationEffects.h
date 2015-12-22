/*
  ==============================================================================

    DecimationEffects.h
    Created: 22 Dec 2015 10:02:42am
    Author:  Nathan Villicaña-Shaw

  ==============================================================================
*/

#ifndef DECIMATIONEFFECTS_H_INCLUDED
#define DECIMATIONEFFECTS_H_INCLUDED

class Decimators {
    
public:
    Decimators();
    ~Decimators();
    
    float* sampleHold(float* channelSamples, int holdLength);
    float* sampleHoldInt(float* channelSamples, int holdLength);
    float* bitCrusher1(float* channelSamples, float bitDeph);
    
private:
    
};



#endif  // DECIMATIONEFFECTS_H_INCLUDED
