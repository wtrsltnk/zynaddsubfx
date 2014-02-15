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

#include <QMainWindow>
#include <QTimer>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void SelectChannel(int channel);
    void SelectPart(int part);

protected slots:
    void OnMasterGainChanged(int value);
    void OnVuTimer();
    void OnShowMidiDevices();
    void OnSelectMidiDevice();
    void OnShowAudioDevices();
    void OnSelectAudioDevice();
    void OnSelectChannel(int channel);
    void OnSelectPart(int part);
    void OnPlay();
    void OnPause();
    void OnStop();

private:
    Ui::MainWindow *ui;
    QTimer* _vutimer;
    int _selectedChannel;
    QPushButton* _effectButtons;

    bool eventFilter(QObject* watched, QEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    float dbl;
    float dbr;
    float rmsdbl;
    float rmsdbr;

};

#endif // MAINWINDOW_H
