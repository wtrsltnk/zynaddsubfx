/*
  ZynAddSubFX - a software synthesizer

  mainwindow.h - Mainwindow voor Zyn Music Studio
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updateUi();

protected slots:
    void OnMasterGainChanged(int value);
    void OnNoteCheckChanged(int state);
    void OnVuTimer();
    void OnShowOptionalMidiDevice();
    void OnSelectMidiDevice();
    void OnShowOptionalAudioDevice();
    void OnSelectAudioDevice();

private:
    Ui::MainWindow *ui;
    QTimer vutimer;

    bool eventFilter(QObject* watched, QEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

};

#endif // MAINWINDOW_H
