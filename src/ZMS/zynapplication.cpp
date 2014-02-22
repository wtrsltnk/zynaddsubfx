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
    p.setColor(QPalette::Window, QColor(53, 53, 53));
    p.setColor(QPalette::Button, QColor(53, 53, 53));
    p.setColor(QPalette::Highlight, QColor(0, 143, 191));
    p.setColor(QPalette::ButtonText, QColor(255, 255, 255));
    p.setColor(QPalette::WindowText, QColor(255, 255, 255));
    p.setColor(QPalette::WindowText, QColor(255, 255, 255));
    this->setPalette(p);

    this->setStyleSheet("QSlider::groove:horizontal {"
                        "    border-radius: 3px;"
                        "    height: 8px; /* the groove expands to the size of the slider by default. by giving it a height, it has a fixed size */"
                        "    background: #00607F;"
                        "    margin: 2px 0;"
                        "}"
                        "QSlider::handle:horizontal {"
                        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
                        "    width: 18px;"
                        "    margin: -2px;"
                        "    border-radius: 3px;"
                        "}"

                        "QSlider::groove:vertical {"
                        "    border-radius: 3px;"
                        "    width: 8px; /* the groove expands to the size of the slider by default. by giving it a height, it has a fixed size */"
                        "    background: #00607F;"
                        "}"
                        "QSlider::handle:vertical {"
                        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
                        "    height: 18px;"
                        "    margin: -2px;"
                        "    border-radius: 3px;"
                        "}"

                        "QPushButton {"
                        "    border-radius: 3px;"
                        "    background: #00607F;"
                        "    margin: 2px;"
                        "    padding: 2px 5px;"
                        "}"

                        "QPushButton:pressed {"
                        "    background: #008FBF;"
                        "}"
                        );
}

int ZynApplication::exec()
{
    MainWindow wnd;
    wnd.show();
    return QApplication::exec();
}
