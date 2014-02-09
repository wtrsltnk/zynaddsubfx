/*
  ZynAddSubFX - a software synthesizer

  instrumentcontainer.h - Scrollable container widget for instruments
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
#ifndef INSTRUMENTCONTAINER_H
#define INSTRUMENTCONTAINER_H

#include <QWidget>
#include "../Misc/Part.h"

namespace Ui {
class InstrumentContainer;
}

class InstrumentWindow;

class InstrumentContainer : public QWidget
{
    Q_OBJECT

public:
    explicit InstrumentContainer(QWidget *parent = 0);
    ~InstrumentContainer();

    void clearInstruments();
    InstrumentWindow* addInstrument(int partindex);
    void selectInstrument(InstrumentWindow* instrument);

signals:
    void selectInstrument(int instrument);

private:
    Ui::InstrumentContainer *ui;
    InstrumentWindow* _selectedInstrument;
};

#endif // INSTRUMENTCONTAINER_H
