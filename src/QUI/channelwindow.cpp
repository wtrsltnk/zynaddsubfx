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

ChannelWindow::ChannelWindow(int index, ChannelContainer* c, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChannelWindow),
    container(c),
    collapsed(true),
    channelIndex(index)
{
    ui->setupUi(this);

    QString str = "CHNL ";
    str += str.number(this->channelIndex);
    ui->lblChannel->setText(str);
    connect(this->ui->collapse, SIGNAL(clicked()), this, SLOT(onToggleCollapse()));
    this->onToggleCollapse();
}

ChannelWindow::~ChannelWindow()
{
    delete ui;
}

void ChannelWindow::onToggleCollapse()
{
    this->container->selectMe(this);
    this->collapsed = !this->collapsed;

    if (this->collapsed)
    {
        this->setMinimumHeight(60);
        this->setMaximumHeight(60);
        this->ui->collapse->setText("+");
    }
    else
    {
        this->setMinimumHeight(160);
        this->setMaximumHeight(160);
        this->ui->collapse->setText("-");
    }
}

void ChannelWindow::select()
{
    this->selected = true;
    this->setStyleSheet("background-color:#666;");
}

void ChannelWindow::unselect()
{
    this->selected = false;
    this->setStyleSheet("background-color:#444;");
}
