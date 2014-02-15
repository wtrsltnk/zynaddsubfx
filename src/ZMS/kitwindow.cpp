/*
  ZynAddSubFX - a software synthesizer

  kitwindow.cpp - Kit widget for changing instrument kit items
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
#include "kitwindow.h"
#include "ui_kitwindow.h"

QColor KitWindow::KitColors[NUM_KIT_ITEMS] = {
    Qt::red,
    Qt::blue,
    Qt::green,
    Qt::yellow,
    Qt::darkYellow,
    Qt::darkBlue,
    Qt::darkCyan,
    Qt::darkGreen,
    Qt::darkMagenta,
    Qt::darkRed
};

KitWindow::KitWindow(Part* part, int kitindex, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KitWindow),
    _part(part), _kitindex(kitindex)
{
    ui->setupUi(this);

    this->UpdateUI();

    connect(this->ui->chkAd, SIGNAL(stateChanged(int)), this, SLOT(OnAdEnabledChanged(int)));
    connect(this->ui->chkSub, SIGNAL(stateChanged(int)), this, SLOT(OnSubEnabledChanged(int)));
    connect(this->ui->chkPad, SIGNAL(stateChanged(int)), this, SLOT(OnPadEnabledChanged(int)));
    connect(this->ui->chkMute, SIGNAL(stateChanged(int)), this, SLOT(OnMuteChanged(int)));
    connect(this->ui->minKeyrange, SIGNAL(valueChanged(int)), this, SLOT(OnMinKeyrangeChanged(int)));
    connect(this->ui->maxKeyrange, SIGNAL(valueChanged(int)), this, SLOT(OnMaxKeyrangeChanged(int)));
}

KitWindow::~KitWindow()
{
    delete ui;
}

QIcon KitWindow::GetIcon()
{
    QPixmap pixmap(16, 16);
    pixmap.fill(KitWindow::KitColors[this->_kitindex]);
    return QIcon(pixmap);
}

void KitWindow::UpdateUI()
{
    this->ui->chkAd->setChecked(this->_part->kit[this->_kitindex].Padenabled);
    this->ui->chkSub->setChecked(this->_part->kit[this->_kitindex].Psubenabled);
    this->ui->chkPad->setChecked(this->_part->kit[this->_kitindex].Ppadenabled);
    this->ui->chkMute->setChecked(this->_part->kit[this->_kitindex].Pmuted);
    this->ui->minKeyrange->setValue(this->_part->kit[this->_kitindex].Pminkey);
    this->ui->maxKeyrange->setValue(this->_part->kit[this->_kitindex].Pmaxkey);
}

void KitWindow::OnAdEnabledChanged(int state)
{
    this->_part->kit[this->_kitindex].Padenabled = state;
}

void KitWindow::OnSubEnabledChanged(int state)
{
    this->_part->kit[this->_kitindex].Psubenabled = state;
}

void KitWindow::OnPadEnabledChanged(int state)
{
    this->_part->kit[this->_kitindex].Ppadenabled = state;
}

void KitWindow::OnMuteChanged(int state)
{
    this->_part->kit[this->_kitindex].Pmuted = state;
}

void KitWindow::OnMinKeyrangeChanged(int value)
{
    this->_part->kit[this->_kitindex].Pminkey = value;
    if (this->_part->Pkitmode == 2)
    {
        if (this->_kitindex > 0)
            this->_part->kit[this->_kitindex - 1].Pmaxkey = value;
    }
    this->KeyRangeChanged();
}

void KitWindow::OnMaxKeyrangeChanged(int value)
{
    this->_part->kit[this->_kitindex].Pmaxkey = value;
    if (this->_part->Pkitmode == 2)
    {
        if (this->_kitindex < NUM_KIT_ITEMS - 1)
            this->_part->kit[this->_kitindex + 1].Pminkey = value;
    }
    this->KeyRangeChanged();
}
