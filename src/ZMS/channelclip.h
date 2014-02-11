#ifndef CHANNELCLIP_H
#define CHANNELCLIP_H

#include <QGraphicsPixmapItem>
#include "../Sequence/midiclip.h"

class ChannelClip : public QGraphicsItemGroup
{
public:
    ChannelClip(MidiClip* clip);
    virtual ~ChannelClip();

    void select();
    void unselect();

    void setHeight(int height);
    QPixmap getPixmapFromClip();

    QGraphicsRectItem _border;
    QGraphicsRectItem _header;
    QGraphicsSimpleTextItem _headertext;
    QGraphicsPixmapItem _notes;
    MidiClip* _clip;
    bool _drag;
    QPointF _dragStart;

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CHANNELCLIP_H
