#include "pianorollnote.h"
#include "snappingcontainer.h"
#include "synthdata.h"
#include <cmath>
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

PianoRollNote::PianoRollNote(SnappingContainer* container, MidiClip::Note* note)
    : SnappingGraphicsItem(container), _note(note), _resizeHandle(this), _resizing(false)
{
    this->UpdateMe();

    this->addToGroup(&this->_border);
    this->addToGroup(&this->_resizeHandle);

    this->Unselect();
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
    this->_border.setRect(this->_note->start * 25, this->_note->note * -25, this->_note->length * 25, 25);
    this->_border.setPen(QPen(Qt::red));
    this->_border.setBrush(QBrush(Qt::red));

    this->_resizeHandle.setRect((this->_note->start + this->_note->length) * 25, this->_note->note * -25, 25, 25);
    this->_resizeHandle.setBrush(QBrush(Qt::yellow));
}

void PianoRollNote::moveItem(int x, int y)
{
    this->setPos(x, y);
    int oldstart = this->_note->start;
    this->UpdateMe();
    if (oldstart != this->_note->start)
        SynthData::Instance().ClipDataChanged(this->_note->_clip);
}

QGraphicsRectItem* PianoRollNote::tempCopyRect()
{
    QGraphicsRectItem* rect = new QGraphicsRectItem();

    rect->setRect(this->_border.rect());
    rect->setPos(this->pos());
    rect->setBrush(QBrush(QColor::fromRgb(255, 255, 255, 100)));
    rect->setPen(QPen(Qt::transparent));

    return rect;
}

void PianoRollNote::copyMe(double start)
{
    MidiClip::Note* note = new MidiClip::Note(this->_note->_clip);
    this->_note->_clip->Pnotes.push_back(note);
    note->start = start;
    note->length = this->_note->length;
    note->note = this->_note->note;
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
        double x = this->mapToScene(event->pos()).x() - this->_resizingStart.x();
        this->_note->length = x / this->_container->Scale() - this->_note->start;
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
