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


ChannelWindow::ChannelWindow(int index, ChannelContainer* c, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelWindow),
    _container(c),
    _channelIndex(index)
{
    ui->setupUi(this);

    this->ui->btnSelect->setText(QString::number(this->_channelIndex + 1));

    connect(this->ui->btnSelect, SIGNAL(clicked()), this, SIGNAL(ThisChannelIsSelected()));
    connect(this->ui->btnRemove, SIGNAL(clicked()), this, SIGNAL(ThisChannelIsRemoved()));
}

ChannelWindow::~ChannelWindow()
{
    delete ui;
}

void ChannelWindow::resizeEvent(QResizeEvent* event)
{
    this->_container->UpdateClips();
}
