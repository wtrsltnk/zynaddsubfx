#include "midiclip.h"
#include "../Misc/Master.h"
#include "sequence.h"
#include <iostream>

using namespace std;

MidiClip::MidiClip()
    : Pchannel(0), Pstart(0.0)
{ }

MidiClip::~MidiClip()
{
    while (this->Pnotes.empty() == false)
    {
        Note* n = this->Pnotes.back();
        this->Pnotes.pop_back();
        delete n;
    }
}

void MidiClip::addNote(unsigned char note, unsigned char velocity, double start, double length)
{
    Note* n = new Note;
    n->note = note;
    n->velocity = velocity;
    n->start = start;
    n->length = length;
    this->Pnotes.push_back(n);
}

void MidiClip::sendNotes(long min, long max)
{
    double frames_per_beat = double(synth->samplerate) / (double(Sequence::getInstance().Tempo()) / 60);
    Master& m = Master::getInstance();
    for (std::vector<Note*>::iterator i = this->Pnotes.begin(); i != this->Pnotes.end(); ++i)
    {
        Note* n = *i;
        long notestart = long(frames_per_beat * (this->Pstart + n->start));
        long notelength = long(frames_per_beat * n->length);
        if (notestart >= min && notestart <= max)
            m.noteOn(this->Pchannel, n->note, n->velocity);
        if (notestart + notelength > min && notestart + notelength < max)
            m.noteOff(this->Pchannel, n->note);
    }
}

void MidiClip::getSize(double& length, unsigned char& minrange, unsigned char& maxrange)
{
    minrange = 128;
    maxrange = 0;
    length = 0;
    for (std::vector<Note*>::iterator i = this->Pnotes.begin(); i != this->Pnotes.end(); ++i)
    {
        if (((Note*)*i)->note < minrange) minrange = ((Note*)*i)->note;
        if (((Note*)*i)->note > maxrange) maxrange = ((Note*)*i)->note;
        if (length < (((Note*)*i)->start + ((Note*)*i)->length))
            length = (((Note*)*i)->start + ((Note*)*i)->length);
    }
}
