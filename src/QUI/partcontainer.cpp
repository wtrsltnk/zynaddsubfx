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
#include "partcontainer.h"
#include "ui_partcontainer.h"
#include "partwindow.h"

PartContainer::PartContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PartContainer),
    selectedPart(0)
{
    ui->setupUi(this);
}

PartContainer::~PartContainer()
{
    delete ui;
}

PartWindow* PartContainer::addPart()
{
    PartWindow* part = new PartWindow(this, ui->scrollArea);
    QLayoutItem* spacer = ui->scrollArea->widget()->layout()->itemAt(ui->scrollArea->widget()->layout()->count()-1);
    ui->scrollArea->widget()->layout()->removeItem(spacer);
    ui->scrollArea->widget()->layout()->addWidget(part);
    ui->scrollArea->widget()->layout()->addItem(spacer);

    return part;
}

void PartContainer::selectMe(PartWindow* part)
{
    if (this->selectedPart != 0 && this->selectedPart != part)
        this->selectedPart->unselect();

    this->selectedPart = part;
    this->selectedPart->select();
}
