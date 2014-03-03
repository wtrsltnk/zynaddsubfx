#include "snappinggraphicsitem.h"

SnappingGraphicsItem::SnappingGraphicsItem()
    : _drag(0)
{
}

void SnappingGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() &= Qt::LeftButton && event->modifiers().testFlag(Qt::ControlModifier))
    {
        this->_drag = 2;
        this->_dragStart = event->pos();
        this->_copyClip = this->CopyItem();
        this->_copyClip->setRect(this->_border.rect());
        this->_copyClip->setPos(this->pos());
        this->_copyClip->setBrush(QBrush(QColor::fromRgb(255, 255, 255, 100)));
        this->_copyClip->setPen(QPen(Qt::transparent));
        ((QGraphicsItemGroup*)this->parentItem())->addToGroup(this->_copyClip);
    }
    else if (event->buttons() &= Qt::LeftButton)
    {
        this->_drag = 1;
        this->_dragStart = event->pos();
    }
}

void SnappingGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    int x = this->mapToScene(event->pos()).x() - this->_dragStart.x() - this->boundingRect().x();
    if (this->_drag == 1)   // normal drag
    {
        this->setPos(x - (x % (4 * this->_container->vscale())), this->y());

        MidiClip* c = Sequence::getInstance().Pclips[this->_clip];
        c->Pstart = this->x() / this->_container->vscale();
    }
    if (this->_drag == 2)
        this->_copyClip->setPos(x - (x % (4 * this->_container->vscale())), this->y());
}

void SnappingGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (this->_drag == 2)
    {
        int copyclip = Sequence::getInstance().CopyClip(this->_clip);
        MidiClip* clip = Sequence::getInstance().Pclips[copyclip];
        clip->Pstart = this->_copyClip->x() / this->_container->vscale();
        this->_container->UpdateClips();

        this->removeFromGroup(this->_copyClip);
        delete this->_copyClip;
        this->_copyClip = 0;
    }
    this->_drag = 0;
}

void SnappingGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // todo : open this clip in a pianoroll
}
