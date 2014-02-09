/*
  ZynAddSubFX - a software synthesizer

  partwindow.cpp - Channel for editing sythesizer part
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
#include "channelwindow.h"
#include "ui_channelwindow.h"
#include "../Sequence/sequence.h"
#include "../Sequence/midiclip.h"
#include "../globals.h"
#include <iostream>

using namespace std;

ChannelClip::ChannelClip(MidiClip *clip)
    : _clip(clip)
{
    QPixmap p = this->getPixmapFromClip();
    this->setPixmap(p.scaled(p.width(), 128, Qt::IgnoreAspectRatio, Qt::FastTransformation));
}

ChannelClip::~ChannelClip()
{ }

QPixmap ChannelClip::getPixmapFromClip()
{
    double len = 0;
    unsigned char minrange = 0, maxrange = 0;
    this->_clip->getSize(len, minrange, maxrange);
    minrange -= 15;
    maxrange += 15;
    QPixmap p(100 * len, (maxrange - minrange) * 10);
    p.fill(QColor::fromRgb(255, 0, 0));

    QPainter painter(&p);
    for (std::vector<MidiClip::Note*>::iterator i = this->_clip->Pnotes.begin(); i != this->_clip->Pnotes.end(); ++i)
    {
        MidiClip::Note* n = (MidiClip::Note*)*i;
        painter.fillRect(n->start * 100, p.height() - ((n->note - minrange) * 10) - 5, n->length * 100, 10, Qt::SolidPattern);
    }
    painter.end();
    return p;
}

ChannelWindow::ChannelWindow(int index, ChannelContainer* c, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelWindow),
    _container(c),
    _collapsed(true),
    _channelIndex(index)
{
    ui->setupUi(this);

    QString str = "CHNL ";
    str += str.number(this->_channelIndex);
    ui->lblChannel->setText(str);
    connect(this->ui->collapse, SIGNAL(clicked()), this, SLOT(onToggleCollapse()));

    this->ui->graphicsView->setScene(&this->_scene);

    this->_clips = new QGraphicsItemGroup();
    this->_scene.addItem(this->_clips);
    std::vector<MidiClip*>& clips = Sequence::getInstance().Pclips;
    for (std::vector<MidiClip*>::iterator i = clips.begin(); i != clips.end(); ++i)
    {
        if (((MidiClip*)*i)->Pchannel == this->_channelIndex)
        {
            ChannelClip* clip = new ChannelClip(((MidiClip*)*i));
            this->_clips->addToGroup(clip);
        }
    }
    this->_cursor = new QGraphicsRectItem(0, 0, 4, 128);
    this->_cursor->setBrush(QBrush(QColor::fromRgbF(0, 1, 0, 0.5)));
    this->_cursor->setPen(QPen(Qt::NoPen));
    this->_clips->addToGroup(this->_cursor);
    this->_scene.setBackgroundBrush(QBrush(Qt::lightGray, Qt::Dense7Pattern));

    this->_updateCursor.setInterval(30);
    this->_updateCursor.start();
    connect(&this->_updateCursor, SIGNAL(timeout()), this, SLOT(onUpdateCursor()));

    this->unselect();
    this->collapse();
}

ChannelWindow::~ChannelWindow()
{
    delete ui;
}

void ChannelWindow::onUpdateCursor()
{
    int newx = double(Sequence::getInstance().Cursor()) / double(synth->samplerate_f) * 100;
    this->_cursor->setPos(newx, 0);
}

void ChannelWindow::onToggleCollapse()
{
    this->_container->selectMe(this);

    if (this->_collapsed == false)
        this->collapse();
    else
        this->expand();
}

void ChannelWindow::select()
{
    this->_selected = true;
    this->_clips->setOpacity(1.0);
}

void ChannelWindow::unselect()
{
    this->_selected = false;
    this->_clips->setOpacity(0.7);
}

void ChannelWindow::collapse()
{
    this->_collapsed = true;
    this->ui->graphicsView->setMinimumHeight(64);
    this->ui->graphicsView->setMaximumHeight(64);
    this->setMinimumHeight(72);
    this->setMaximumHeight(72);
    this->_clips->setTransform(QTransform::fromScale(1.0, 0.5));
    this->ui->collapse->setText("+");
}

void ChannelWindow::expand()
{
    this->_collapsed = false;
    this->ui->graphicsView->setMinimumHeight(128);
    this->ui->graphicsView->setMaximumHeight(128);
    this->setMinimumHeight(136);
    this->setMaximumHeight(136);
    this->_clips->setTransform(QTransform::fromScale(1.0, 1.0));
    this->ui->collapse->setText("-");
}
