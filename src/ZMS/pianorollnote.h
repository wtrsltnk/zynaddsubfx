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

    void UpdateMe();

protected:
    virtual void moveItem(int x, int y);
    virtual QGraphicsRectItem* tempCopyRect();
    virtual void copyMe(double start);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    MidiClip::Note* _note;
    QGraphicsRectItem _border;
    QGraphicsRectItem _resizeHandle;

    bool _resizing;
    QPointF _resizingStart;

};

#endif // PIANOROLLNOTE_H
