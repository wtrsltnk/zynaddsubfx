#include "sequence.h"
#include "midiclip.h"
#include "../globals.h"
#include "../Misc/Master.h"

Sequence::Sequence()
    : _currentframe(synth->samplerate * 2), // 2 seconds
      _startframe(_currentframe),
      _endframe(synth->samplerate * 5), // 5 seconds
      _bpm(120),
      _running(Sequence::Stopped)
{ }

Sequence& Sequence::getInstance()
{
    static Sequence sequence;
    return sequence;
}

Sequence::~Sequence()
{ }

void Sequence::AddFrames(long frames)
{
    if (this->_running == Sequence::Running)
    {
        long min = this->_currentframe;
        this->_currentframe += frames;

        // Special case where we go past the _endframes
        if (this->_currentframe > this->_endframe)
        {
            // Recalculate the minimum from startframe
            min = this->_startframe;
            this->_currentframe = this->_startframe + (this->_currentframe - this->_endframe);

            // and shutup everything from before.
            Master::getInstance().ShutUp();
        }

        // Send alle notes from all clips between our calculated min/max frame
        for (std::vector<MidiClip*>::iterator i = this->Pclips.begin(); i != this->Pclips.end(); ++i)
            ((MidiClip*)*i)->sendNotes(min, this->_currentframe);
    }
}

void Sequence::Start()
{
    this->_running = Sequence::Running;
}

void Sequence::Pause()
{
    this->_running = Sequence::Paused;
}

void Sequence::Reset()
{
    this->_currentframe = this->_startframe;
    Master::getInstance().ShutUp();
}

void Sequence::Stop()
{
    this->_running = Sequence::Stopped;
    this->Reset();
}

void Sequence::SetPlayRange(long start, unsigned long length)
{
    this->_startframe = start;
    this->_endframe = start + length;
}

double Sequence::FramesToBeats(long frames)
{
    return double(frames) * ((double(this->_bpm) / 60.0) / double(synth->samplerate_f));
}
