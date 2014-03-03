#ifndef SNAPPINGGRAPHICSITEM_H
#define SNAPPINGGRAPHICSITEM_H

#include <QGraphicsPixmapItem>

class SnappingGraphicsItem : public QGraphicsItemGroup
{
public:
    SnappingGraphicsItem();

protected:
    virtual SnappingGraphicsItem* CopyItem() = 0;

private:
    int _drag;
    QPointF _dragStart;
    SnappingGraphicsItem* _copyClip;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SNAPPINGGRAPHICSITEM_H
