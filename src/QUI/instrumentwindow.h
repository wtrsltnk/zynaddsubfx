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
#include "instrumentcontainer.h"
#include "kitwindow.h"
#include "../Misc/Part.h"

namespace Ui {
class InstrumentWindow;
}

class InstrumentWindow : public QWidget
{
    Q_OBJECT

public:
    explicit InstrumentWindow(int partindex, InstrumentContainer* c, QWidget *parent = 0);
    ~InstrumentWindow();

    void select();
    void unselect();

    int partIndex();

    void updateUI();

protected slots:
    void onToggleCollapse();
    void onChangeEnabled(int value);
    void onAddKititem();
    void onRemoveKititem();
    void onKeyRangeChanged();
    void onKitModeChanged(int value);

private:
    Ui::InstrumentWindow *ui;
    InstrumentContainer* _container;
    KitWindow* _kitWindows[NUM_KIT_ITEMS];
    Part* _part;
    int _partIndex;
};

#endif // INSTRUMENTWINDOW_H
