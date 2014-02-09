/*
  ZynAddSubFX - a software synthesizer

  channelwindow.h - Part for editing sythesizer part
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
#ifndef CHANNELWINDOW_H
#define CHANNELWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup>
#include <QTimer>
#include "channelcontainer.h"
#include "../Sequence/midiclip.h"

namespace Ui {
class ChannelWindow;
}

class ChannelClip : public QGraphicsPixmapItem
{
public:
    ChannelClip(MidiClip* clip);
    virtual ~ChannelClip();

    QPixmap getPixmapFromClip();

    MidiClip* _clip;
};

class ChannelWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelWindow(int index, ChannelContainer* c, QWidget *parent = 0);
    ~ChannelWindow();

    void select();
    void unselect();

    void collapse();
    void expand();

    int channelIndex() { return this->_channelIndex; }

protected slots:
    void onToggleCollapse();
    void onUpdateCursor();

private:
    Ui::ChannelWindow *ui;
    ChannelContainer* _container;
    bool _collapsed;
    bool _selected;
    int _channelIndex;
    QGraphicsScene _scene;
    QGraphicsRectItem* _cursor;
    QGraphicsItemGroup* _clips;
    QTimer _updateCursor;
};

#endif // CHANNELWINDOW_H
