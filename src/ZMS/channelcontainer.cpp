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
#include "mainwindow.h"
#include "channelwindow.h"
#include "synthdata.h"
#include "../Misc/Part.h"
#include "../Sequence/sequence.h"
#include <QMouseEvent>
#include <iostream>

using namespace std;

ChannelContainer::ChannelContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelContainer),
    _selectedChannel(0),
    _lastHScroll(0),
    _scene(0)
{
    this->_rangeselection.start = this->_rangeselection.end = this->_rangeselection.draw = 0;

    for (int i = 0; i < NUM_MIDI_CHANNELS; i++)
        this->_channels[i] = 0;
    for (int i = 0; i < NUM_MAX_CLIPS; i++)
        this->_clips[i] = 0;

    ui->setupUi(this);

    this->_splitter = new QSplitter(this->ui->channels);
    this->_splitter->setOrientation(Qt::Vertical);
    this->ui->channels->layout()->addWidget(this->_splitter);

    this->ui->timeline->installEventFilter(this);
    connect(this->ui->clipScroller, SIGNAL(valueChanged(int)), this, SLOT(HScrollChannel(int)));
    connect(this->ui->addChannel, SIGNAL(clicked()), this, SLOT(AddChannel()));

    this->_scene = new QGraphicsScene();
    this->ui->clips->setScene(this->_scene);
    this->ui->clips->installEventFilter(this);
    this->ui->clips->setBackgroundBrush(QBrush(QColor(35, 35, 35)));

    this->_group = new QGraphicsItemGroup();
    this->_group->setFiltersChildEvents(false);
    this->_group->setHandlesChildEvents(false);
    this->_scene->addItem(this->_group);

    this->_cursor = new QGraphicsLineItem();
    this->_cursor->setLine(0, 0, 0, this->ui->clips->height());
    this->_scene->addItem(this->_cursor);

    this->_updateTimer = new QTimer(this);
    connect(this->_updateTimer, SIGNAL(timeout()), this, SLOT(UpdateUI()));
    this->_updateTimer->start(10);

    connect(this->ui->viewscale, SIGNAL(valueChanged(int)), this, SLOT(SetViewScale(int)));
    connect(&SynthData::Instance(), SIGNAL(ClipsChanged()), this, SLOT(OnClipsChanged()));
    connect(&SynthData::Instance(), SIGNAL(ClipDataChanged(MidiClip*)), this, SLOT(OnClipChanged(MidiClip*)));

    this->UpdateChannels();
}

ChannelContainer::~ChannelContainer()
{
    this->_scene->removeItem(this->_group);
    delete this->_group;
    this->ui->clips->setScene(0);
    delete this->_scene;
    delete this->_updateTimer;
    delete ui;
}

void ChannelContainer::OnClipsChanged()
{
    this->UpdateItems();
}

void ChannelContainer::OnClipChanged(MidiClip* clip)
{
    for (int i = 0; i < SynthData::Instance().GetClipCount(); i++)
    {
        MidiClip* tmp = SynthData::Instance().GetClipAt(i);
        if (tmp == clip)
        {
            if (this->_clips[i] != 0)
            {
                this->_clips[i]->UpdateClip();
            }
        }
    }
}

void ChannelContainer::UpdateUI()
{
    this->ui->timeline->update();
    int c = Sequence::getInstance().FramesToBeats(Sequence::getInstance().Cursor()) * this->Scale();
    this->_cursor->setLine(c + this->ScrollOffset(), 0, c + this->ScrollOffset(), this->ui->clips->height());
}

void ChannelContainer::resizeEvent(QResizeEvent* event)
{
    this->UpdateItems();
}

bool ChannelContainer::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == this->ui->timeline && event->type() == QEvent::Paint)
    {
        this->PaintTimeline();
        return true;
    }
    if (watched == this->ui->timeline && event->type() == QEvent::MouseButtonPress)
    {
        this->_rangeselection.start = ((QMouseEvent*)event)->pos().x();
        return true;
    }
    if (watched == this->ui->timeline && event->type() == QEvent::MouseMove)
    {
        this->_rangeselection.end = ((QMouseEvent*)event)->pos().x();
        this->_rangeselection.draw = true;
        this->ui->timeline->update();
        return true;
    }
    if (watched == this->ui->timeline && event->type() == QEvent::MouseButtonRelease)
    {
        this->_rangeselection.draw = false;
        int s = (this->_rangeselection.start - this->ScrollOffset()) - (this->_rangeselection.start % this->Scale());
        int e = (this->_rangeselection.end - this->ScrollOffset()) - (this->_rangeselection.end % this->Scale());
        Sequence::getInstance().SetPlayRange(
                    Sequence::getInstance().BeatsToFrames(s / this->Scale()),
                    Sequence::getInstance().BeatsToFrames(e / this->Scale()));
        this->ui->timeline->update();
        return true;
    }
    if (watched == this->ui->clips && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mevent = (QMouseEvent*)event;
        ChannelClip* item = dynamic_cast<ChannelClip*> (this->ui->clips->itemAt(mevent->pos()));
        if (item == 0)
        {
            for (int i = 0; i < NUM_MAX_CLIPS; i++)
                if (this->_clips[i] != 0)
                    this->_clips[i]->Unselect();
            SynthData::Instance().ClipSelectionChanged(QList<int>());
        }
    }
    return false;
}

void ChannelContainer::PaintTimeline()
{
    QPainter p(this->ui->timeline);

    // Draw timeline
    p.setPen(QPen(this->palette().brightText().color()));
    p.setFont(QFont("Verdana", 10));
    for (int x = 0; (x * this->Scale()) < 1000; x++)
    {
        if (x % 4)
            p.drawLine((x * this->Scale()) + this->ScrollOffset(),
                       this->ui->top->height()/2,
                       (x * this->Scale()) + this->ScrollOffset(),
                       this->ui->top->height());
        else
        {
            p.drawLine((x * this->Scale()) + this->ScrollOffset(),
                       0,
                       (x * this->Scale()) + this->ScrollOffset(),
                       this->ui->top->height());
            p.drawText((x * this->Scale()) + this->ScrollOffset()+2,
                       p.fontInfo().pointSize(),
                       QString::number(x+1));
        }
    }

    // Draw time range
    int x = Sequence::getInstance().FramesToBeats(Sequence::getInstance().StartPlayAt()) * this->Scale();
    int w = Sequence::getInstance().FramesToBeats(Sequence::getInstance().StopPlayAt()) * this->Scale();
    p.fillRect(x + this->ScrollOffset(), this->ui->timeline->height()/2, w - x, this->ui->timeline->height()/2, QBrush(QColor::fromRgb(0, 143, 191, 155)));
    p.setPen(QPen(QColor::fromRgb(0, 191, 255), 2));
    p.drawLine(x + this->ScrollOffset(), this->ui->timeline->height()/2, x + this->ScrollOffset(), this->ui->timeline->height());
    p.drawLine(w + this->ScrollOffset(), this->ui->timeline->height()/2, w + this->ScrollOffset(), this->ui->timeline->height());

    // Draw cursor
    p.setPen(QPen(QColor::fromRgb(255, 255, 255), 2));
    int c = Sequence::getInstance().FramesToBeats(Sequence::getInstance().Cursor()) * this->Scale();
    p.drawLine(c + this->ScrollOffset(), 0, c + this->ScrollOffset(), this->ui->timeline->height());

    // Draw change timerange
    if (this->_rangeselection.draw)
    {
        int s = (this->_rangeselection.start - this->ScrollOffset()) - (this->_rangeselection.start % this->Scale());
        int e = (this->_rangeselection.end - this->ScrollOffset()) - (this->_rangeselection.end % this->Scale());
        if (e < s) { int tmp = s; s = e; e = tmp; }
        p.setPen(QPen(QColor::fromRgb(0, 191, 255), 4));
        p.drawLine(s + this->ScrollOffset(), this->ui->timeline->height() - 2, e + this->ScrollOffset(), this->ui->timeline->height() - 2);
    }
}

void ChannelContainer::HScrollChannel(int value)
{
    this->SetScrollOffset(-(value * 10));

    this->_group->moveBy((this->_lastHScroll - value) * 10, 0);
    this->ui->timeline->update();

    this->_lastHScroll = value;
}

void ChannelContainer::UpdateChannels()
{
    int h = 0;
    for (int i = 0; i < NUM_MIDI_CHANNELS; i++)
    {
        for (int j = 0; j < NUM_MIDI_PARTS; j++)
        {
            Part* part = SynthData::Instance().GetPartAt(j);
            if (part->Prcvchn == i && part->Penabled)
            {
                h += 150;
                if (this->_channels[i] == 0)
                {
                    this->_channels[i] = new ChannelWindow(i, this);
                    connect(this->_channels[i], SIGNAL(ThisChannelIsSelected()), this, SLOT(ChannelIsSelected()));
                    connect(this->_channels[i], SIGNAL(ThisChannelIsRemoved()), this, SLOT(ChannelIsRemoved()));
                }
                if (this->_splitter->indexOf(this->_channels[i]) < 0)
                    this->_splitter->addWidget(this->_channels[i]);
            }
        }
    }
    this->ui->scrollAreaWidgetContents->setMinimumHeight(h);
    this->UpdateItems();
}

void ChannelContainer::UpdateItems()
{
    if (this->_scene == 0)
        return;

    //this->_clips->addRect(0, 0, 10, 10, QPen(QColor::fromRgb(255, 255, 255, 0)), QBrush(QColor::fromRgb(255, 255, 255, 0)));
    for (int i = 0; i < SynthData::Instance().GetClipCount(); i++)
    {
        MidiClip* clip = SynthData::Instance().GetClipAt(i);
        if (clip != 0)
        {
            if (this->_clips[i] == 0)
            {
                this->_clips[i] = new ChannelClip(this, i);
                this->_group->addToGroup(this->_clips[i]);
            }
            else
                this->_clips[i]->UpdateClip();
            int height = -2;
            for (int j = 0; j < this->_splitter->children().size(); j++)
            {
                height += ((QWidget*)this->_splitter->children()[j])->height() + 4;
                if (this->_splitter->children()[j] == this->_channels[clip->Pchannel])
                {
                    this->_clips[i]->setPos(clip->Pstart * this->Scale(), this->_splitter->height() - height + 4);
                    this->_clips[i]->SetHeight(this->_channels[clip->Pchannel]->height() - 8);
                    break;
                }
            }
        }
        else
        {
            if (this->_clips[i] != 0)
            {
                this->_group->removeFromGroup(this->_clips[i]);
                delete this->_clips[i];
                this->_clips[i] = 0;
            }
        }
    }
}

void ChannelContainer::SelectItem(SnappingGraphicsItem* item)
{
    QList<int> selectedClips;
    for (int i = 0; i < NUM_MAX_CLIPS; i++)
    {
        if (this->_clips[i] != 0 && this->_clips[i] != item)
            this->_clips[i]->Unselect();
        else if (this->_clips[i] != 0)
        {
            this->_clips[i]->Select();
            selectedClips.append(i);
        }
    }
    SynthData::Instance().ClipSelectionChanged(selectedClips);
}

void ChannelContainer::AddChannel()
{
    for (int i = 0; i < NUM_MIDI_CHANNELS; i++)
    {
        if (this->_channels[i] == 0)
        {
            for (int j = 0; j < NUM_MIDI_PARTS; j++)
            {
                Part* part = SynthData::Instance().GetPartAt(j);
                if (part->Penabled == false)
                {
                    part->defaults();
                    part->Prcvchn = i;
                    SynthData::Instance().EnablePartAt(j, true);
                    break;
                }
            }
            break;
        }
    }
    this->UpdateChannels();
}

void ChannelContainer::SetSelectedChannel(int channel)
{
    if (channel < 0 || channel >= NUM_MIDI_CHANNELS)
        return;
    this->_selectedChannel = this->_channels[channel];
    this->SelectedChannelChanged(channel);
}

void ChannelContainer::ChannelIsSelected()
{
    ChannelWindow* sender = dynamic_cast<ChannelWindow*>(this->sender());
    if (sender != 0)
    {
        this->SetSelectedChannel(sender->ChannelIndex());
    }
}

void ChannelContainer::RemoveChannel(int index)
{
    // Disable all part for this channel
    for (int j = 0; j < SynthData::Instance().GetPartCount(); j++)
    {
        Part* part = SynthData::Instance().GetPartAt(j);
        if (part->Penabled &&
                part->Prcvchn == index)
            SynthData::Instance().EnablePartAt(j, false);
    }
    // Disable all clips for this channel
    for (int j = 0; j < SynthData::Instance().GetClipCount(); j++)
    {
        MidiClip* clip = SynthData::Instance().GetClipAt(j);
        if (clip != 0 && clip->Pchannel == index)
            SynthData::Instance().RemoveClipAt(j);
    }
    int itemindex = ((QVBoxLayout*)ui->channels->layout())->indexOf(this->_channels[index]);
    QLayoutItem* item = ((QVBoxLayout*)ui->channels->layout())->itemAt(itemindex);
    ((QVBoxLayout*)ui->channels->layout())->removeItem(item);
    delete item;
    delete this->_channels[index];
    this->_channels[index] = 0;
    this->UpdateChannels();
}

void ChannelContainer::ChannelIsRemoved()
{
    ChannelWindow* sender = dynamic_cast<ChannelWindow*>(this->sender());
    if (sender != 0)
    {
        this->RemoveChannel(sender->ChannelIndex());
    }
}

void ChannelContainer::RemoveClip(int index)
{
    for (int i = 0; i < SynthData::Instance().GetClipCount(); i++)
    {
        MidiClip* clip = SynthData::Instance().GetClipAt(i);
        if (clip != 0)
        {
            if (clip->Pchannel == index)
            {
                ChannelClip* cclip = this->_clips[i];
                if (cclip != 0)
                {
                    delete cclip;
                    this->_clips[i] = 0;
                }
            }
        }
    }
}

void ChannelContainer::SetViewScale(int scale)
{
    this->SetScale(scale);
    this->update();
    this->UpdateChannels();
}
