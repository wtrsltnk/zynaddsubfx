/*
  ZynAddSubFX - a software synthesizer

  instrumentcontainer.cpp - Scrollable container widget for instruments
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
#include "instrumentcontainer.h"
#include "ui_instrumentcontainer.h"
#include "instrumentwindow.h"

InstrumentContainer::InstrumentContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstrumentContainer)
{
    ui->setupUi(this);
}

InstrumentContainer::~InstrumentContainer()
{
    delete ui;
}

void InstrumentContainer::resetInstruments()
{
    while (ui->scrollArea->widget()->layout()->count() > 2)
    {
        QLayoutItem* part = ui->scrollArea->widget()->layout()->itemAt(ui->scrollArea->widget()->layout()->count()-2);
        ui->scrollArea->widget()->layout()->removeItem(part);
        delete part->widget();
        delete part;
    }
}

InstrumentWindow* InstrumentContainer::addInstrument(int partindex)
{
    InstrumentWindow* instrument = new InstrumentWindow(partindex, this, ui->scrollArea);
    ((QVBoxLayout*)ui->scrollArea->widget()->layout())->insertWidget(ui->scrollArea->widget()->layout()->count()-2, instrument);
    ((QVBoxLayout*)ui->scrollArea->widget()->layout())->setAlignment(instrument, Qt::AlignTop);

    return instrument;
}

void InstrumentContainer::selectMe(InstrumentWindow* instrument)
{
    if (this->_selectedInstrument != 0 && this->_selectedInstrument != instrument)
        this->_selectedInstrument->unselect();

    this->_selectedInstrument = instrument;
    this->_selectedInstrument->select();
    this->selectInstrument(instrument->partIndex());
}
