/*
  ZynAddSubFX - a software synthesizer

  kitwindow.h - Kit widget for changing instrument kit items
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
#ifndef KITWINDOW_H
#define KITWINDOW_H

#include <QWidget>
#include "../Misc/Part.h"

namespace Ui {
class KitWindow;
}

class KitWindow : public QWidget
{
    Q_OBJECT

public:
    explicit KitWindow(Part* part, int kitindex, QWidget *parent = 0);
    ~KitWindow();

    static QColor KitColors[NUM_KIT_ITEMS];

    QIcon GetIcon();
    void UpdateUI();

signals:
    void KeyRangeChanged();

protected slots:
    void OnAdEnabledChanged(int state);
    void OnSubEnabledChanged(int state);
    void OnPadEnabledChanged(int state);
    void OnMuteChanged(int state);
    void OnMinKeyrangeChanged(int value);
    void OnMaxKeyrangeChanged(int value);

private:
    Ui::KitWindow *ui;
    Part* _part;
    int _kitindex;
};

#endif // KITWINDOW_H
