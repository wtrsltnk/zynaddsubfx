#include "snappinggraphicsitem.h"
#include "snappingcontainer.h"
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>

SnappingGraphicsItem::SnappingGraphicsItem(SnappingContainer* container)
    : _dragState(0), _container(container)
{ }

void SnappingGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->_container->SelectItem(this);
    if (event->buttons() &= Qt::LeftButton && event->modifiers().testFlag(Qt::ControlModifier))
    {
        this->_dragState = 2;
        this->_dragStart = event->pos();
        this->_dragCopyGhost = this->tempCopyRect();
        ((QGraphicsItemGroup*)this->parentItem())->addToGroup(this->_dragCopyGhost);
    }
    else if (event->buttons() &= Qt::LeftButton)
    {
        this->_dragState = 1;
        this->_dragStart = event->pos();
    }
}

void SnappingGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    int x = this->mapToScene(event->pos()).x() - this->_dragStart.x();// - this->boundingRect().x();
    if (this->_dragState == 1)   // normal drag
        this->moveItem(x - (x % (4 * this->_container->Scale())), this->y());
    if (this->_dragState == 2)
        this->_dragCopyGhost->setPos(x - (x % (4 * this->_container->Scale())), this->y());
}

void SnappingGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (this->_dragState == 2)
    {
        this->copyMe(this->_dragCopyGhost->x() / this->_container->Scale());

        this->removeFromGroup(this->_dragCopyGhost);
        delete this->_dragCopyGhost;
        this->_dragCopyGhost = 0;

        this->_container->UpdateItems();
    }
    this->_dragState = 0;
}

void SnappingGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // todo : open this clip in a pianoroll
}

void SnappingGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{ }

void SnappingGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{ }

void SnappingGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{ }
