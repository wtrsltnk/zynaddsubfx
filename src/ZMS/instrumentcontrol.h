/*
  ZynAddSubFX - a software synthesizer

  instrumentcontrol.h - Instrument widget for controlling instrument volume and effects
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
#ifndef INSTRUMENTCONTROL_H
#define INSTRUMENTCONTROL_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class InstrumentControl;
}

class InstrumentControl : public QWidget
{
    Q_OBJECT

public:
    explicit InstrumentControl(int partindex, QWidget *parent = 0);
    ~InstrumentControl();

protected slots:
    void onVolumeChanged(int value);
    void onPanChanged(int value);
    void onSysEffectChanged(int value);

private:
    Ui::InstrumentControl *ui;
    QPushButton* _effectButtons;
    int _partindex;
};

#endif // INSTRUMENTCONTROL_H
