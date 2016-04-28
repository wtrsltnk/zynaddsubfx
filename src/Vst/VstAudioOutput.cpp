#include <iostream>
#include <audioeffectx.h>

#define NUM_PARAMS 0

class YourProjectName : public AudioEffectX
{
public:
    YourProjectName(audioMasterCallback audioMaster);
    ~YourProjectName();

    void processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames);
};

YourProjectName::YourProjectName(audioMasterCallback audioMaster)
    : AudioEffectX(audioMaster, 0, NUM_PARAMS)
{ }

YourProjectName::~YourProjectName()
{ }

void YourProjectName::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames)
{
    // Real processing goes here
}


AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new YourProjectName(audioMaster);
}
