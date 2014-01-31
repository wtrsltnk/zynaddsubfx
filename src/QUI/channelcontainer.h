/*
  ZynAddSubFX - a software synthesizer

  partcontainer.h - Scrollable container widget for parts
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
#ifndef CHANNELCONTAINER_H
#define CHANNELCONTAINER_H

#include <QWidget>

namespace Ui {
class ChannelContainer;
}

class ChannelWindow;

class ChannelContainer : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelContainer(QWidget *parent = 0);
    ~ChannelContainer();

    int vscrollOffset();
    int vscale();

    ChannelWindow* addChannel(int index);
    void selectMe(ChannelWindow* part);

private:
    Ui::ChannelContainer *ui;

    ChannelWindow* selectedChannel;
    int _vscrollOffset;
    int _vscale;
};

#endif // CHANNELCONTAINER_H
