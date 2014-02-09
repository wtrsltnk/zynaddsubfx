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

class ChannelWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelWindow(int index, ChannelContainer* c, QWidget *parent = 0);
    ~ChannelWindow();

public:
    void select();
    void unselect();

    int channelIndex() { return this->_channelIndex; }

signals:
    void onRemoveChannel(int index);

protected slots:
    void onSelectChannelEvent();
    void onRemoveChannelEvent();

private:
    Ui::ChannelWindow *ui;
    ChannelContainer* _container;
    bool _selected;
    int _channelIndex;

    virtual void resizeEvent(QResizeEvent* event);
};

#endif // CHANNELWINDOW_H
