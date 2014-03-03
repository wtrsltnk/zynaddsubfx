/*
  ZynAddSubFX - a software synthesizer

  channelclip.h - Graphics item for visualizing a MidiClip
  Copyright (C) 2014 Wouter Saaltink
  Author: Wouter Saaltink

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/
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
    QPointF _dragStart;
    QGraphicsRectItem* _copyClip;

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
