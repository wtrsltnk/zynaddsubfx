#include "snappinggraphicsitem.h"
#include "snappingcontainer.h"
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>
#include <iostream>

using namespace std;

SnappingGraphicsItem::SnappingGraphicsItem(SnappingContainer* container)
    : _dragState(0), _container(container), _allowHorizontalMove(true), _allowVerticalMove(false)
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
        this->_dragStart = this->mapToScene(event->pos());
    }
}

void SnappingGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF mapped = this->mapToScene(event->pos());
    int x = mapped.x() - this->_dragStart.x();
    int y = mapped.y() - this->_dragStart.y();

    if (this->_dragState == 1)   // normal drag
        this->moveItem((this->_allowHorizontalMove ? x : 0),
                       (this->_allowVerticalMove ? y : 0));

    if (this->_dragState == 2)
        this->_dragCopyGhost->setPos((this->_allowHorizontalMove ? this->x() + x : this->x()),
                                     (this->_allowVerticalMove ? this->y() + y : this->y()));

    this->_dragStart = mapped;
}

void SnappingGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (this->_dragState == 2)
    {
        this->copyMe(this->_dragCopyGhost->x() / this->_container->HScale(),
                     this->_dragCopyGhost->y() / this->_container->HScale());

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
