#ifndef CHANNELCLIP_H
#define CHANNELCLIP_H

#include <QGraphicsPixmapItem>
#include "../Sequence/midiclip.h"

class ChannelClip : public QGraphicsItemGroup
{
public:
    ChannelClip(int clip);
    virtual ~ChannelClip();

    void SetHeight(int height);

    void Select();
    void Unselect();

private:
    QGraphicsRectItem _border;
    QGraphicsRectItem _header;
    QGraphicsSimpleTextItem _headertext;
    QGraphicsPixmapItem _notes;
    int _clip;
    bool _drag;
    QPointF _dragStart;

    QPixmap GetPixmapFromClip();

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // CHANNELCLIP_H
