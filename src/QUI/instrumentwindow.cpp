/*
  ZynAddSubFX - a software synthesizer

  instrumentwindow.cpp - Instrument widget for editing instrument settings
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
#include "instrumentwindow.h"
#include "ui_instrumentwindow.h"
#include "kitwindow.h"
#include "../Misc/Master.h"

InstrumentWindow::InstrumentWindow(Part* p, InstrumentContainer* c, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstrumentWindow),
    _container(c),
    _part(p)
{
    ui->setupUi(this);
    this->ui->content->hide();

    this->ui->chkActive->setChecked(_part->Penabled);
    this->ui->lblName->setText(QString((const char*)_part->Pname));
    this->ui->dialVolume->setValue(_part->Pvolume);
    this->ui->dialPan->setValue(_part->Ppanning);

    for (int i = 0; i < NUM_KIT_ITEMS; i++)
    {
        if (_part->kit[i].Padenabled)
        {
            KitWindow* kit = new KitWindow(_part, i, this);
            this->ui->content->layout()->addWidget(kit);
        }
    }

    this->setStyleSheet("background-color:#666;");
    connect(this->ui->collapse, SIGNAL(clicked()), this, SLOT(onToggleCollapse()));

    connect(this->ui->dialVolume, SIGNAL(valueChanged(int)), this, SLOT(onChangeVolume(int)));
    connect(this->ui->dialPan, SIGNAL(valueChanged(int)), this, SLOT(onChangePan(int)));
    connect(this->ui->chkActive, SIGNAL(stateChanged(int)), this, SLOT(onChangeEnabled(int)));
}

InstrumentWindow::~InstrumentWindow()
{
    delete ui;
}

void InstrumentWindow::onToggleCollapse()
{
    if (this->ui->content->isVisible())
    {
        this->ui->content->hide();
        this->ui->collapse->setText("+");
    }
    else
    {
        this->ui->content->show();
        this->ui->collapse->setText("-");
    }
}

void InstrumentWindow::onChangeVolume(int value)
{
    this->_part->setPvolume(value);
}

void InstrumentWindow::onChangePan(int value)
{
    this->_part->setPpanning(value);
}

void InstrumentWindow::onChangeEnabled(int value)
{
    this->_part->Penabled = value;
}
