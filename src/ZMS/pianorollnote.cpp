#include "pianorollnote.h"
#include <QBrush>
#include <QPen>

PianoRollNote::PianoRollNote(SnappingContainer* container, MidiClip::Note* note)
    : SnappingGraphicsItem(container), _note(note)
{
    this->_border.setRect(note->start * 25, note->note * -25, note->length * 25, 25);
    this->_border.setPen(QPen(Qt::red));
    this->_border.setBrush(QBrush(Qt::red));
    this->addToGroup(&this->_border);

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

void PianoRollNote::moveItem(int x, int y)
{
    this->setPos(x, y);
}

QGraphicsRectItem* PianoRollNote::tempCopyRect()
{
    QGraphicsRectItem* rect = new QGraphicsRectItem();

    rect->setRect(this->_border.rect());
    rect->setPos(this->pos());
    rect->setBrush(QBrush(QColor::fromRgb(255, 255, 255, 100)));
    rect->setPen(QPen(Qt::transparent));

    return rect;}
