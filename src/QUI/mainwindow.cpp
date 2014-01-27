/*
  ZynAddSubFX - a software synthesizer

  mainwindow.cpp - Mainwindow voor Zyn Music Studio
  Copyright (c) 2002-2009 Nasca Octavian Paul
  Copyright (C) 2014 Wouter Saaltink
  Author: Wouter Saaltink, Nasca Octavian Paul

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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../Misc/Master.h"
#include "../Nio/InMgr.h"
#include "../Nio/OutMgr.h"
#include "../Nio/EngineMgr.h"
#include "../Nio/MidiIn.h"
#include "../Nio/AudioOut.h"
#include <QPainter>
#include "partwindow.h"
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->ui->masterGain->setMinimum(0);
    this->ui->masterGain->setMaximum(100);
    this->ui->masterGain->setValue(Master::getInstance().Pvolume);

    this->ui->btnMidi->setText(InMgr::getInstance().getSource().c_str());
    connect(this->ui->btnMidi, SIGNAL(clicked()), this, SLOT(OnShowOptionalMidiDevice()));
    this->ui->btnAudio->setText(OutMgr::getInstance().getDriver().c_str());
    connect(this->ui->btnAudio, SIGNAL(clicked()), this, SLOT(OnShowOptionalAudioDevice()));

    this->ui->vu->installEventFilter(this);

    connect(this->ui->masterGain, SIGNAL(valueChanged(int)), this, SLOT(OnMasterGainChanged(int)));
    connect(this->ui->note, SIGNAL(stateChanged(int)), this, SLOT(OnNoteCheckChanged(int)));
    connect(&this->vutimer, SIGNAL(timeout()), this, SLOT(OnVuTimer()));

    this->vutimer.setInterval(1000/40);
    this->vutimer.start();

    this->ui->widget->addPart();
    this->ui->widget->addPart();
    this->ui->widget->addPart();
    this->ui->widget->addPart();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnMasterGainChanged(int value)
{
    Master::getInstance().setPvolume(value);
}

void MainWindow::OnNoteCheckChanged(int state)
{
    if (state)
        Master::getInstance().noteOn(0, 70, 100);
    else
        Master::getInstance().noteOff(0, 70);
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    // This part is copied from MasterUI.fl by Nasca Octavian Paul
    static float olddbl = 0;
    static float olddbr = 0;
    static float oldrmsdbl = 0;
    static float oldrmsdbr = 0;
    if (watched == ui->vu && event->type() == QEvent::Paint)
    {
        QPainter painter;
        painter.begin(ui->vu);
        vuData data = Master::getInstance().getVuData();

        int ox=0; int oy=0; int lx=ui->vu->width(); int ly=ui->vu->height();

        float dbl=rap2dB(data.outpeakl);
        float dbr=rap2dB(data.outpeakr);
        float rmsdbl=rap2dB(data.rmspeakl);
        float rmsdbr=rap2dB(data.rmspeakr);
        #define MIN_DB (-48)

        dbl=(MIN_DB-dbl)/MIN_DB;
        if (dbl<0.0) dbl=0.0;
          else if (dbl>1.0)dbl=1.0;

        dbr=(MIN_DB-dbr)/MIN_DB;
        if (dbr<0.0) dbr=0.0;
          else if (dbr>1.0) dbr=1.0;

        dbl=dbl*0.4+olddbl*0.6;
        dbr=dbr*0.4+olddbr*0.6;

        olddbl=dbl;
        olddbr=dbr;

        #define VULENX (lx)
        #define VULENY (ly/2-2)

        dbl*=VULENX;dbr*=VULENX;

        int idbl=(int) dbl;
        int idbr=(int) dbr;

        //compute RMS - start
        rmsdbl=(MIN_DB-rmsdbl)/MIN_DB;
        if (rmsdbl<0.0) rmsdbl=0.0;
          else if (rmsdbl>1.0) rmsdbl=1.0;

        rmsdbr=(MIN_DB-rmsdbr)/MIN_DB;
        if (rmsdbr<0.0) rmsdbr=0.0;
          else if (rmsdbr>1.0) rmsdbr=1.0;

        rmsdbl=rmsdbl*0.4+oldrmsdbl*0.6;
        rmsdbr=rmsdbr*0.4+oldrmsdbr*0.6;

        oldrmsdbl=rmsdbl;
        oldrmsdbr=rmsdbr;

        rmsdbl*=VULENX;rmsdbr*=VULENX;

        int irmsdbl=(int) rmsdbl;
        int irmsdbr=(int) rmsdbr;
        //compute RMS - end

        painter.fillRect(ox,oy,idbr,VULENY, QBrush(Qt::green, Qt::Dense7Pattern));
        painter.fillRect(ox,oy+ly/2,idbl,VULENY, QBrush(Qt::green, Qt::Dense7Pattern));

        painter.fillRect(ox+idbr,oy,VULENX-idbr,VULENY, QBrush(Qt::white, Qt::Dense7Pattern));
        painter.fillRect(ox+idbl,oy+ly/2,VULENX-idbl,VULENY, QBrush(Qt::white, Qt::Dense7Pattern));

        painter.fillRect(ox+irmsdbr-1,oy,3,VULENY, QBrush(Qt::yellow, Qt::SolidPattern));
        painter.fillRect(ox+irmsdbl-1,oy+ly/2,3,VULENY, QBrush(Qt::yellow, Qt::SolidPattern));

        painter.end();
        return true;
    }
    return false;
}

void MainWindow::OnVuTimer()
{
    this->ui->vu->repaint();
}

void MainWindow::OnShowOptionalMidiDevice()
{
    QMenu m;
    std::list<Engine *> en = EngineMgr::getInstance().engines;
    for (std::list<Engine *>::iterator i = en.begin(); i != en.end(); ++i)
    {
        MidiIn* e = dynamic_cast<MidiIn *>(*i);
        if (e != 0)
        {
            QAction* a = m.addAction(e->name.c_str());
            connect(a, SIGNAL(triggered()), this, SLOT(OnSelectMidiDevice()));
        }
    }

    m.exec(QCursor::pos());
}

void MainWindow::OnSelectMidiDevice()
{
    QString sel = ((QAction*)sender())->text();
    if (InMgr::getInstance().setSource(sel.toStdString()))
        this->ui->btnMidi->setText(sel);
}

void MainWindow::OnShowOptionalAudioDevice()
{
    QMenu m;
    std::list<Engine *> en = EngineMgr::getInstance().engines;
    for (std::list<Engine *>::iterator i = en.begin(); i != en.end(); ++i)
    {
        AudioOut* e = dynamic_cast<AudioOut *>(*i);
        if (e != 0)
        {
            QAction* a = m.addAction(e->name.c_str());
            connect(a, SIGNAL(triggered()), this, SLOT(OnSelectAudioDevice()));
        }
    }

    m.exec(QCursor::pos());
}

void MainWindow::OnSelectAudioDevice()
{
    QString sel = ((QAction*)sender())->text();
    if (OutMgr::getInstance().setSink(sel.toStdString()))
        this->ui->btnAudio->setText(sel);
}
