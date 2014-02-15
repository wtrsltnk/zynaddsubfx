/*
  ZynAddSubFX - a software synthesizer

  instrumentcontrol.cpp - Instrument widget for controlling instrument volume and effects
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
#include "instrumentcontrol.h"
#include "ui_instrumentcontrol.h"
#include "../Misc/Master.h"
#include "../Misc/Part.h"
#include "../Effects/EffectMgr.h"

InstrumentControl::InstrumentControl(int partindex, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstrumentControl),
    _partindex(partindex)
{
    ui->setupUi(this);

    this->_effectButtons = new QPushButton[NUM_PART_EFX];
    for (int i = 0; i < NUM_PART_EFX; i++)
    {
        this->_effectButtons[i].setText(EffectMgr::FilterTitles[Master::getInstance().part[this->_partindex]->partefx[i]->geteffect()]);
        this->ui->busEffects->layout()->addWidget(&this->_effectButtons[i]);
    }
    this->ui->busMaster->setValue(Master::getInstance().part[this->_partindex]->Pvolume);
    this->ui->busPan->setValue(Master::getInstance().part[this->_partindex]->Ppanning);

    connect(this->ui->busMaster, SIGNAL(valueChanged(int)), this, SLOT(OnVolumeChanged(int)));
    connect(this->ui->busPan, SIGNAL(valueChanged(int)), this, SLOT(OnPanChanged(int)));
    connect(this->ui->sysEffect0, SIGNAL(valueChanged(int)), this, SLOT(OnSysEffectChanged(int)));
    connect(this->ui->sysEffect1, SIGNAL(valueChanged(int)), this, SLOT(OnSysEffectChanged(int)));
    connect(this->ui->sysEffect2, SIGNAL(valueChanged(int)), this, SLOT(OnSysEffectChanged(int)));
    connect(this->ui->sysEffect3, SIGNAL(valueChanged(int)), this, SLOT(OnSysEffectChanged(int)));
}

InstrumentControl::~InstrumentControl()
{
    for (int i = 0; i < NUM_PART_EFX; i++)
        this->ui->busEffects->layout()->removeWidget(&this->_effectButtons[i]);
    delete []this->_effectButtons;
    delete ui;
}

void InstrumentControl::OnVolumeChanged(int value)
{
    Master::getInstance().part[this->_partindex]->setPvolume(value);
}

void InstrumentControl::OnPanChanged(int value)
{
    Master::getInstance().part[this->_partindex]->setPpanning(value);
}

void InstrumentControl::OnSysEffectChanged(int value)
{
    if (sender() == this->ui->sysEffect0)
        Master::getInstance().setPsysefxvol(this->_partindex, 0, value);
    if (sender() == this->ui->sysEffect1)
        Master::getInstance().setPsysefxvol(this->_partindex, 1, value);
    if (sender() == this->ui->sysEffect2)
        Master::getInstance().setPsysefxvol(this->_partindex, 2, value);
    if (sender() == this->ui->sysEffect3)
        Master::getInstance().setPsysefxvol(this->_partindex, 3, value);
}
