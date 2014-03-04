#ifndef PIANOROLLNOTE_H
#define PIANOROLLNOTE_H

#include "snappinggraphicsitem.h"
#include "../Sequence/midiclip.h"

class PianoRollNote : public SnappingGraphicsItem
{
public:
    PianoRollNote(SnappingContainer* container, MidiClip::Note* note);

    virtual void Select();
    virtual void Unselect();

protected:
    virtual void moveItem(int x, int y);
    virtual QGraphicsRectItem* tempCopyRect();
    virtual void copyMe(double start) { }

private:
    MidiClip::Note* _note;
    QGraphicsRectItem _border;
};

#endif // PIANOROLLNOTE_H
