/*
  ZynAddSubFX - a software synthesizer

  partwindow.h - Part for editing sythesizer part
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
#ifndef PARTWINDOW_H
#define PARTWINDOW_H

#include <QWidget>
#include "partcontainer.h"

namespace Ui {
class PartWindow;
}

class PartWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PartWindow(PartContainer* c, QWidget *parent = 0);
    ~PartWindow();

    void select();
    void unselect();

protected slots:
    void onToggleCollapse();

private:
    Ui::PartWindow *ui;
    PartContainer* container;
    bool collapsed;
    bool selected;
};

#endif // PARTWINDOW_H
