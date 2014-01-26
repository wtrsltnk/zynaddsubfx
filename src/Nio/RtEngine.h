#ifndef RTENGINE_H
#define RTENGINE_H

#include "MidiIn.h"
#include "RtMidi.h"

class RtEngine:public MidiIn
{
public:
    RtEngine();
    virtual ~RtEngine();

    virtual bool Start();
    virtual void Stop();

    virtual void setMidiEn(bool nval);
    virtual bool getMidiEn() const;

    static void callback(double timeStamp, std::vector<unsigned char> *message, void *userData);
private:
    RtMidiIn* midiin;
};

#endif // RTENGINE_H
