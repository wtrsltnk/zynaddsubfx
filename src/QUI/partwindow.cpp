/*
  ZynAddSubFX - a software synthesizer

  partwindow.cpp - Part for editing sythesizer part
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
#include "partwindow.h"
#include "ui_partwindow.h"

PartWindow::PartWindow(PartContainer* c, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PartWindow),
    container(c),
    collapsed(true)
{
    ui->setupUi(this);

    connect(this->ui->collapse, SIGNAL(clicked()), this, SLOT(onToggleCollapse()));
    this->onToggleCollapse();
}

PartWindow::~PartWindow()
{
    delete ui;
}

void PartWindow::onToggleCollapse()
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

void PartWindow::select()
{
    this->selected = true;
    this->setStyleSheet("background-color:#999;");
}

void PartWindow::unselect()
{
    this->selected = false;
    this->setStyleSheet("background-color:#333;");
}
