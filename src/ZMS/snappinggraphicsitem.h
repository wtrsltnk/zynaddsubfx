#ifndef SNAPPINGGRAPHICSITEM_H
#define SNAPPINGGRAPHICSITEM_H

#include <QGraphicsPixmapItem>

class SnappingContainer;

class SnappingGraphicsItem : public QGraphicsItemGroup
{
public:
    SnappingGraphicsItem(SnappingContainer* container);

    virtual void Select() = 0;
    virtual void Unselect() = 0;

protected:
    virtual void moveItem(int x, int y) = 0;
    virtual QGraphicsRectItem* tempCopyRect() = 0;
    virtual void copyMe(double x, double y) = 0;

    SnappingContainer* _container;
    bool _allowVerticalMove;
    bool _allowHorizontalMove;

private:
    int _dragState;
    QPointF _dragStart;
    QGraphicsRectItem* _dragCopyGhost;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

};

#endif // SNAPPINGGRAPHICSITEM_H
