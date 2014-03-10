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

#include "snappinggraphicsitem.h"
#include "../Sequence/midiclip.h"
class ChannelContainer;

class ChannelClip : public SnappingGraphicsItem
{
public:
    ChannelClip(ChannelContainer* container, int clip);
    virtual ~ChannelClip();

    virtual void Select();
    virtual void Unselect();

    void SetHeight(int height);
    void UpdateClip();

    int ClipIndex() { return this->_clip; }

protected:
    virtual void moveItem(int x, int y);
    virtual QGraphicsRectItem* tempCopyRect();
    virtual void copyMe(double x, double y);

private:
    QGraphicsRectItem _border;
    QGraphicsRectItem _header;
    QGraphicsSimpleTextItem _headertext;
    QGraphicsPixmapItem _notes;
    int _clip;
    double _fineStart;

    QPixmap GetPixmapFromClip(MidiClip* clip);

};

#endif // CHANNELCLIP_H
