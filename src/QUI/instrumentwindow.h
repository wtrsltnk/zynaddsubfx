/*
  ZynAddSubFX - a software synthesizer

  instrumentwindow.h - Instrument widget for editing instrument settings
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
#ifndef INSTRUMENTWINDOW_H
#define INSTRUMENTWINDOW_H

#include <QWidget>

namespace Ui {
class InstrumentWindow;
}

class InstrumentWindow : public QWidget
{
    Q_OBJECT

public:
    explicit InstrumentWindow(QWidget *parent = 0);
    ~InstrumentWindow();

private:
    Ui::InstrumentWindow *ui;
};

#endif // INSTRUMENTWINDOW_H
