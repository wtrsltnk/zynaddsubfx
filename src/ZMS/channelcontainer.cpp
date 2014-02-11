/*
  ZynAddSubFX - a software synthesizer

  partcontainer.cpp - Scrollable container widget for parts
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
#include "channelcontainer.h"
#include "ui_channelcontainer.h"
#include "channelwindow.h"
#include "../Misc/Master.h"
#include "../Misc/Part.h"
#include "../Sequence/sequence.h"
#include <iostream>

using namespace std;

ChannelContainer::ChannelContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelContainer),
    _selectedChannel(0),
    _lastHScroll(0)
{
    ui->setupUi(this);

    for (int i = 0; i < NUM_MIDI_CHANNELS; i++)
    {
        if (((Part*)Master::getInstance().part[i])->Prcvchn == i &&
                ((Part*)Master::getInstance().part[i])->Penabled)
        {
            this->_channels[i] = new ChannelWindow(i, this);
            connect(this->_channels[i], SIGNAL(onRemoveChannel(int)), this, SLOT(onRemoveChannel(int)));
            this->ui->channels->layout()->addWidget(this->_channels[i]);
        }
        else
            this->_channels[i] = 0;
    }
    this->updateMinHeight();

    connect(this->ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(onHScrollChannel(int)));
    connect(this->ui->btnAddChannel, SIGNAL(clicked()), this, SLOT(onAddChannel()));
    this->ui->clips->setScene(&this->_clips);
    this->ui->clips->installEventFilter(this);
    this->ui->clips->setBackgroundBrush(QBrush(QColor(0, 48, 64)));
}

ChannelContainer::~ChannelContainer()
{
    delete ui;
}

void ChannelContainer::resizeEvent(QResizeEvent* event)
{
    this->updateClips();
}

bool ChannelContainer::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == this->ui->clips && event->type() == QEvent::MouseButtonPress)
    {
        for (std::vector<ChannelClip*>::iterator i = this->_channelClips.begin(); i != this->_channelClips.end(); ++i)
        {
            ChannelClip* clip = ((ChannelClip*)*i);
            if (clip != 0)
                clip->unselect();
        }
        return true;
    }
    return false;
}

void ChannelContainer::updateClips()
{
    // TODO optimze this, not very smart!
    while (this->_channelClips.empty() == false)
    {
        ChannelClip* cclip = this->_channelClips.back();
        this->_channelClips.pop_back();
        this->_clips.removeItem(cclip);
        delete cclip;
    }
    this->_clips.addRect(0, 0, 10, 10, QPen(Qt::white), QBrush(Qt::white));
    for (int i = 0; i < Sequence::getInstance().Pclips.size(); i++)
    {
        MidiClip* clip = Sequence::getInstance().Pclips[i];
        ChannelClip* cclip = new ChannelClip(clip);
        int index = this->_channels[clip->Pchannel]->parentWidget()->layout()->indexOf(this->_channels[clip->Pchannel]);
        cclip->setPos(clip->Pstart * 100, this->_channels[clip->Pchannel]->height() * index);
        cclip->setHeight(this->_channels[clip->Pchannel]->height());
        this->_clips.addItem(cclip);
        this->_channelClips.push_back(cclip);
    }
}

void ChannelContainer::onAddChannel()
{
    for (int i = 0; i < NUM_MIDI_CHANNELS; i++)
    {
        if (this->_channels[i] == 0)
        {
            this->_channels[i] = new ChannelWindow(i, this);
            connect(this->_channels[i], SIGNAL(onRemoveChannel(int)), this, SLOT(onRemoveChannel(int)));
            this->ui->channels->layout()->addWidget(this->_channels[i]);
            for (int j = 0; j < NUM_MIDI_PARTS; j++)
            {
                if (((Part*)Master::getInstance().part[j])->Penabled == false)
                {
                    Master::getInstance().partonoff(j, 1);
                    ((Part*)Master::getInstance().part[j])->defaults();
                    ((Part*)Master::getInstance().part[j])->Prcvchn = i;
                    break;
                }
            }
            break;
        }
    }
    this->updateMinHeight();
}

void ChannelContainer::onRemoveChannel(int index)
{
    for (int j = 0; j < NUM_MIDI_PARTS; j++)
    {
        if (((Part*)Master::getInstance().part[j])->Penabled && ((Part*)Master::getInstance().part[j])->Prcvchn == index)
            Master::getInstance().partonoff(j, 0);
    }
    int item = ((QVBoxLayout*)ui->channels->layout())->indexOf(this->_channels[index]);
    ((QVBoxLayout*)ui->channels->layout())->removeItem(((QVBoxLayout*)ui->channels->layout())->itemAt(item));
    delete this->_channels[index];
    this->_channels[index] = 0;
    this->updateMinHeight();
}

void ChannelContainer::onHScrollChannel(int value)
{
    this->ui->clips->scroll(this->_lastHScroll - value, 0);
    this->_lastHScroll = value;
}

void ChannelContainer::selectChannel(ChannelWindow* channel)
{
    if (this->_selectedChannel != 0 && this->_selectedChannel != channel)
        this->_selectedChannel->unselect();

    this->_selectedChannel = channel;
    this->_selectedChannel->select();
    this->selectChannel(channel->channelIndex());
}

void ChannelContainer::updateMinHeight()
{
    int h = 0;
    for (int i = 0; i < NUM_MIDI_CHANNELS; i++)
        if (this->_channels[i] != 0)
            h += 150;
    this->ui->scrollAreaWidgetContents->setMinimumHeight(h);
    this->updateClips();
}
