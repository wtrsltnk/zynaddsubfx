#ifndef CHANNELCLIP_H
#define CHANNELCLIP_H

#include <QGraphicsPixmapItem>
#include "../Sequence/midiclip.h"
class ChannelContainer;

class ChannelClip : public QGraphicsItemGroup
{
public:
    ChannelClip(ChannelContainer* container, int clip);
    virtual ~ChannelClip();

    void Select();
    void Unselect();

    void SetHeight(int height);
    void UpdateClip();

    int ClipIndex() { return this->_clip; }

private:
    ChannelContainer* _container;
    QGraphicsRectItem _border;
    QGraphicsRectItem _header;
    QGraphicsSimpleTextItem _headertext;
    QGraphicsPixmapItem _notes;
    int _clip;
    int _drag;
    QGraphicsRectItem* _copyClip;
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
