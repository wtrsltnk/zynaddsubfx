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
#include "../Misc/Master.h"

InstrumentWindow::InstrumentWindow(Part* p, InstrumentContainer* c, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstrumentWindow),
    container(c),
    part(p)
{
    ui->setupUi(this);
    this->ui->content->hide();

    this->ui->chkActive->setChecked(part->Penabled);
    this->ui->lblName->setText(QString((const char*)part->Pname));
    this->ui->dialVolume->setValue(part->Pvolume);
    this->ui->dialPan->setValue(part->Ppanning);

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
    this->part->setPvolume(value);
}

void InstrumentWindow::onChangePan(int value)
{
    this->part->setPpanning(value);
}

void InstrumentWindow::onChangeEnabled(int value)
{
    this->part->Penabled = value;
}
