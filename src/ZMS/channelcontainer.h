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
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimer>
#include "../globals.h"
#include "channelclip.h"

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

    int vscrollOffset() { return this->_vscrollOffset; }
    int vscale() { return this->_vscale; }

    void UpdateChannels();
    void UpdateClips();

protected slots:
    void AddChannel();
    void RemoveChannel(int index);
    void HScrollChannel(int value);
    void UpdateUI();
    void SetSelectedChannel(int channel);
    void ChannelIsSelected();
    void ChannelIsRemoved();
    void SetSelectedClip(int clip);
    void ClipIsSelected();

signals:
    void SelectedChannelChanged(int channel);

private:
    Ui::ChannelContainer *ui;
    ChannelWindow* _channels[NUM_MIDI_CHANNELS];
    ChannelWindow* _selectedChannel;
    ChannelClip* _clips[NUM_MAX_CLIPS];
    QGraphicsScene* _scene;
    QGraphicsItemGroup* _group;
    QGraphicsLineItem* _cursor;
    int _vscrollOffset;
    int _vscale;
    int _lastHScroll;
    QTimer* _updateTimer;

    struct {
        int start;
        int end;
        bool draw;
    } _rangeselection;

    virtual void resizeEvent(QResizeEvent* event);
    virtual bool eventFilter(QObject* watched, QEvent* event);

    void PaintTimeline();
    void RemoveClip(int index);
};

#endif // CHANNELCONTAINER_H
