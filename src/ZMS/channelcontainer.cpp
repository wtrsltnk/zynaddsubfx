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
#include <QMouseEvent>
#include <iostream>

using namespace std;

ChannelContainer::ChannelContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelContainer),
    _selectedChannel(0),
    _vscrollOffset(0),
    _vscale(25),
    _lastHScroll(0),
    _clips(0)
{
    this->_rangeselection.start = this->_rangeselection.end = this->_rangeselection.draw = 0;
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
    this->_clips = new QGraphicsScene();
    this->ui->clips->setScene(this->_clips);
    this->ui->clips->installEventFilter(this);
    this->ui->clips->setBackgroundBrush(QBrush(QColor(35, 35, 35)));

    this->_group = new QGraphicsItemGroup();
    this->_group->setFiltersChildEvents(false);
    this->_group->setHandlesChildEvents(false);
    this->_clips->addItem(this->_group);

    this->ui->scale->installEventFilter(this);
}

ChannelContainer::~ChannelContainer()
{
    this->_clips->removeItem(this->_group);
    delete this->_group;
    this->ui->clips->setScene(0);
    delete this->_clips;
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
    if (watched == this->ui->scale && event->type() == QEvent::Paint)
    {
        QPainter p(this->ui->scale);
        p.setPen(QPen(this->palette().brightText().color()));
        p.setFont(QFont("Verdana", 10));
        for (int x = 0; (x * this->_vscale) < 1000; x++)
        {
            if (x % 4)
                p.drawLine((x * this->_vscale) + this->_vscrollOffset,
                           this->ui->top->height()/2,
                           (x * this->_vscale) + this->_vscrollOffset,
                           this->ui->top->height());
            else
            {
                p.drawLine((x * this->_vscale) + this->_vscrollOffset,
                           0,
                           (x * this->_vscale) + this->_vscrollOffset,
                           this->ui->top->height());
                p.drawText((x * this->_vscale) + this->_vscrollOffset+2,
                           p.fontInfo().pointSize(),
                           QString::number(x+1));
            }
        }

        int x = Sequence::getInstance().FramesToBeats(Sequence::getInstance().StartPlayAt()) * this->_vscale;
        int w = Sequence::getInstance().FramesToBeats(Sequence::getInstance().StopPlayAt()) * this->_vscale;
        p.fillRect(x, 0, w - x, this->ui->scale->height(), QBrush(QColor::fromRgb(0, 143, 191, 155)));

        p.setPen(QPen(QColor::fromRgb(0, 191, 255), 2));
        p.drawLine(x, 0, x, this->ui->scale->height());
        p.drawLine(w, 0, w, this->ui->scale->height());

        if (this->_rangeselection.draw)
        {
            int s = this->_rangeselection.start - (this->_rangeselection.start % this->_vscale);
            int e = this->_rangeselection.end - (this->_rangeselection.end % this->_vscale);
            if (e < s) { int tmp = s; s = e; e = tmp; }
            p.setPen(QPen(QColor::fromRgb(0, 191, 255), 4));
            p.drawLine(s, this->ui->scale->height() - 2, e, this->ui->scale->height() - 2);
        }
        return true;
    }
    if (watched == this->ui->scale && event->type() == QEvent::MouseButtonPress)
    {
        this->_rangeselection.start = ((QMouseEvent*)event)->pos().x();
        return true;
    }
    if (watched == this->ui->scale && event->type() == QEvent::MouseButtonRelease)
    {
        this->_rangeselection.draw = false;
        this->ui->scale->update();
        return true;
    }
    if (watched == this->ui->scale && event->type() == QEvent::MouseMove)
    {
        this->_rangeselection.end = ((QMouseEvent*)event)->pos().x();
        this->_rangeselection.draw = true;
        this->ui->scale->update();
        return true;
    }
    return false;
}

void ChannelContainer::updateClips()
{
    if (this->_clips == 0)
        return;

    // TODO optimze this, not very smart!
    while (this->_channelClips.empty() == false)
    {
        ChannelClip* cclip = this->_channelClips.back();
        this->_channelClips.pop_back();
        this->_group->removeFromGroup(cclip);
        delete cclip;
    }
    this->_clips->addRect(0, 0, 10, 10, QPen(QColor::fromRgb(255, 255, 255, 0)), QBrush(QColor::fromRgb(255, 255, 255, 0)));
    for (int i = 0; i < Sequence::getInstance().Pclips.size(); i++)
    {
        MidiClip* clip = Sequence::getInstance().Pclips[i];
        ChannelClip* cclip = new ChannelClip(clip);
        int index = this->_channels[clip->Pchannel]->parentWidget()->layout()->indexOf(this->_channels[clip->Pchannel]);
        cclip->setPos(clip->Pstart * 100, this->_channels[clip->Pchannel]->height() * index + 4);
        cclip->setHeight(this->_channels[clip->Pchannel]->height() - 8);
        this->_group->addToGroup(cclip);
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
    this->_vscrollOffset = -(value * 10);

    this->_group->moveBy((this->_lastHScroll - value) * 10, 0);
    this->ui->scale->update();

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
