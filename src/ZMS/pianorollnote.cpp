#include "pianorollnote.h"
#include "snappingcontainer.h"
#include "synthdata.h"
#include <cmath>
#include <QBrush>
#include <QPen>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <iostream>

using namespace std;

PianoRollNote::PianoRollNote(SnappingContainer* container, MidiClip::Note* note)
    : SnappingGraphicsItem(container), _note(note), _resizeHandle(this), _resizing(false)
{
    this->UpdateMe();

    this->addToGroup(&this->_border);
    this->addToGroup(&this->_resizeHandle);

    this->Unselect();

    this->_allowHorizontalMove = true;
    this->_allowVerticalMove = true;

    this->_fineStart = double(note->start);
    this->_fineLength = double(note->length);
    this->_fineNote = double(note->note);
}

void PianoRollNote::Select()
{
    this->_border.setPen(QPen(Qt::red));
}

void PianoRollNote::Unselect()
{
    this->_border.setPen(QPen(Qt::green));
}

void PianoRollNote::UpdateMe()
{
    int x = this->_note->start * this->_container->HScale();
    int w = (this->_note->length) * this->_container->HScale();
    int y = (128 * this->_container->VScale()) - this->_note->note * this->_container->VScale();

    this->_border.setRect(x, y, w, this->_container->VScale());
    this->_border.setPen(QPen(Qt::red));
    this->_border.setBrush(QBrush(Qt::red));
    this->_border.setCursor(Qt::SizeAllCursor);

    this->_resizeHandle.setRect(x + w, y, 25, this->_container->VScale());
    this->_resizeHandle.setBrush(QBrush(QColor::fromRgb(0, 0, 0, 0)));
    this->_resizeHandle.setPen(QPen(QColor::fromRgb(0, 0, 0, 50)));
    this->_resizeHandle.setCursor(Qt::SizeHorCursor);
}

void PianoRollNote::moveItem(int x, int y)
{
    this->_fineStart += (double(x) / this->_container->HScale());
    this->_note->start = (int)this->_fineStart;
    this->_fineNote -= (double(y) / this->_container->VScale());
    this->_note->note = (unsigned char)this->_fineNote;

    this->UpdateMe();
    SynthData::Instance().ClipDataChanged(this->_note->_clip);
}

QGraphicsRectItem* PianoRollNote::tempCopyRect()
{
    QGraphicsRectItem* rect = new QGraphicsRectItem();

    rect->setRect(this->_border.rect());
    rect->setPos(this->pos());
    rect->setBrush(QBrush(QColor::fromRgb(155, 200, 255, 100)));
    rect->setPen(QPen(Qt::transparent));

    return rect;
}

void PianoRollNote::copyMe(double x, double y)
{
    MidiClip::Note* note = new MidiClip::Note(this->_note->_clip);
    this->_note->_clip->Pnotes.push_back(note);
    note->start = this->_note->start + x;
    note->note = this->_note->note - y;
    note->length = this->_note->length;
    note->velocity = this->_note->velocity;
    SynthData::Instance().ClipsChanged();
}

void PianoRollNote::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (this->_resizeHandle.contains(event->pos()))
    {
        this->_resizing = true;
        this->_resizingStart = this->mapToScene(event->pos());
    }
    else
        SnappingGraphicsItem::mousePressEvent(event);
}

void PianoRollNote::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (this->_resizing)
    {
        QPointF mapped = this->mapToScene(event->pos());
        double x = (mapped.x() - this->_resizingStart.x())  / this->_container->HScale();
        this->_resizingStart = mapped;

        this->_fineLength += x;
        this->_note->length = (int)this->_fineLength;
        if (this->_note->length < 0) this->_note->length = 0;

        this->UpdateMe();
        SynthData::Instance().ClipDataChanged(this->_note->_clip);
    }
    else
        SnappingGraphicsItem::mouseMoveEvent(event);
}

void PianoRollNote::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    this->_resizing = false;
    SnappingGraphicsItem::mouseReleaseEvent(event);
}
