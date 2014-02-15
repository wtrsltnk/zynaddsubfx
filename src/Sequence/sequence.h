#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "../globals.h"
#include <vector>

class MidiClip;

class Sequence
{
public:
    enum
    {
        Stopped = 0,
        Paused,
        Running
    };
private:
    Sequence();
public:
    static Sequence& getInstance();
    virtual ~Sequence();

    void Start();
    void Pause();
    void Reset();
    void Stop();

    unsigned char Status() { return this->_running; }
    int Tempo() { return this->_bpm; }
    long Cursor() { return this->_currentframe; }
    long StartPlayAt() { return this->_startframe; }
    long StopPlayAt() { return this->_endframe; }
    void SetPlayRange(long start, long end);

    void AddFrames(long frames);

    double FramesToBeats(long frames);
    long BeatsToFrames(double beats);

    MidiClip* Pclips[NUM_MAX_CLIPS];
private:
    long _currentframe;
    long _startframe;
    long _endframe;
    int _bpm;
    unsigned char _running;

};

#endif // SEQUENCE_H
