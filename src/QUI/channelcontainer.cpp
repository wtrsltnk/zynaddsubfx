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

ChannelContainer::ChannelContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelContainer),
    selectedChannel(0)
{
    ui->setupUi(this);

    bool usedChannels[NUM_MIDI_CHANNELS] = { false };

    // Find all active channels
    for (int i = 0; i < NUM_MIDI_PARTS; i++)
        if (((Part*)Master::getInstance().part[i])->Penabled)
            usedChannels[((Part*)Master::getInstance().part[i])->Prcvchn] = true;

    // Add all active channels
    for (int i = 0; i < NUM_MIDI_CHANNELS; i++)
        if (usedChannels[i])
            this->addChannel(i);
}

ChannelContainer::~ChannelContainer()
{
    delete ui;
}

ChannelWindow* ChannelContainer::addChannel(int index)
{
    ChannelWindow* channel = new ChannelWindow(index, this, ui->scrollArea);
    QLayoutItem* spacer = ui->scrollArea->widget()->layout()->itemAt(ui->scrollArea->widget()->layout()->count()-1);
    ui->scrollArea->widget()->layout()->removeItem(spacer);
    ui->scrollArea->widget()->layout()->addWidget(channel);
    ui->scrollArea->widget()->layout()->addItem(spacer);

    return channel;
}

void ChannelContainer::selectMe(ChannelWindow* part)
{
    if (this->selectedChannel != 0 && this->selectedChannel != part)
        this->selectedChannel->unselect();

    this->selectedChannel = part;
    this->selectedChannel->select();
}
