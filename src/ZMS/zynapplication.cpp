/*
  ZynAddSubFX - a software synthesizer

  zynapplication.cpp - The QApplication for running zyn music studio
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
#include "zynapplication.h"
#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>

ZynApplication::ZynApplication(int argc, char *argv[]) :
    QApplication(argc, argv)
{
    this->setStyle(QStyleFactory::create("Fusion"));
    QPalette p = this->palette();
    p.setColor(QPalette::Window, QColor(53,53,53));
    p.setColor(QPalette::Button, QColor(53,53,53));
    p.setColor(QPalette::Highlight, QColor(142,45,197));
    p.setColor(QPalette::ButtonText, QColor(255,255,255));
    p.setColor(QPalette::WindowText, QColor(255,255,255));
    this->setPalette(p);
}

int ZynApplication::exec()
{
    MainWindow wnd;
    wnd.show();
    return QApplication::exec();
}
