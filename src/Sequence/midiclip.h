#ifndef MIDICLIP_H
#define MIDICLIP_H

#include <vector>
#include <string>

class MidiClip
{
public:
    class Note
    {
    public:
        unsigned char note, velocity;
        double start, length; //start en length of this note in beats
    };

public:
    MidiClip();
    virtual ~MidiClip();

    void addNote(unsigned char note, unsigned char velocity, double start, double length);
    void sendNotes(long min, long max);

    void getSize(double& length, unsigned char& minrange, unsigned char& maxrange);

    unsigned char Pchannel; //to what midi channel it sends commands
    double Pstart; //beat number this clip starts. time can be calculated with bpm and time signature
    std::vector<Note*> Pnotes;
    std::string Pname;

};

#endif // MIDICLIP_H
