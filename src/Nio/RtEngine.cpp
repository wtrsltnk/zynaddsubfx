#include "RtEngine.h"
#include "InMgr.h"

RtEngine::RtEngine()
    : midiin(0)
{
    name = "RT";
}

RtEngine::~RtEngine()
{
    this->Stop();
}

bool RtEngine::Start()
{
    this->midiin = new RtMidiIn();
    this->midiin->setCallback(RtEngine::callback, this);
    this->midiin->openPort(0);

    return true;
}

void RtEngine::Stop()
{
    if (this->midiin != 0)
        delete this->midiin;
    this->midiin = 0;
}

void RtEngine::setMidiEn(bool nval)
{
    if (nval)
        this->Start();
    else
        this->Stop();
}

bool RtEngine::getMidiEn() const
{
    return (this->midiin != 0);
}

void RtEngine::callback(double timeStamp, std::vector<unsigned char> *message, void *userData)
{
    MidiIn::midiProcess(message->at(0), message->at(1), message->at(2));
}
