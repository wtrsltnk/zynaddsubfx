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
#include <QPainter>
#include <QMenu>
#include <QKeyEvent>
#include <iostream>
#include "../Misc/Master.h"
#include "../Misc/Part.h"
#include "../Effects/EffectMgr.h"
#include "../Nio/InMgr.h"
#include "../Nio/OutMgr.h"
#include "../Nio/EngineMgr.h"
#include "../Nio/MidiIn.h"
#include "../Nio/AudioOut.h"
#include "channelwindow.h"
#include "instrumentwindow.h"
#include "instrumentcontrol.h"
#include "keyboard.h"
#include "kitwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->_effectButtons = new QPushButton[NUM_SYS_EFX];
    for (int i = 0; i < NUM_SYS_EFX; i++)
    {
        this->_effectButtons[i].setText(EffectMgr::FilterTitles[Master::getInstance().sysefx[i]->geteffect()]);
        this->ui->syseffects->layout()->addWidget(&this->_effectButtons[i]);
    }
    this->ui->sysmaster->setValue(Master::getInstance().Pvolume);

    for (int i = 0; i < NUM_MIDI_PARTS; i++)
    {
        if (Master::getInstance().part[i]->Penabled)
        {
            InstrumentControl* instrument = new InstrumentControl(i);
            this->ui->busses->layout()->addWidget(instrument);
            this->ui->busses->layout()->setAlignment(instrument, Qt::AlignLeft);
        }
    }

    this->ui->btnMidi->setText(InMgr::getInstance().getSource().c_str());
    connect(this->ui->btnMidi, SIGNAL(clicked()), this, SLOT(OnShowOptionalMidiDevice()));
    this->ui->btnAudio->setText(OutMgr::getInstance().getDriver().c_str());
    connect(this->ui->btnAudio, SIGNAL(clicked()), this, SLOT(OnShowOptionalAudioDevice()));

    this->ui->sysuvL->installEventFilter(this);
    this->ui->sysuvR->installEventFilter(this);

    connect(&this->_vutimer, SIGNAL(timeout()), this, SLOT(OnVuTimer()));
    connect(this->ui->sysmaster, SIGNAL(valueChanged(int)), this, SLOT(OnMasterGainChanged(int)));

    this->_vutimer.setInterval(1000/40);
    this->_vutimer.start();

    connect(this->ui->channels, SIGNAL(selectChannel(int)), this, SLOT(OnSelectChannel(int)));
    connect(this->ui->instruments, SIGNAL(selectInstrument(int)), this, SLOT(OnSelectPart(int)));
}

MainWindow::~MainWindow()
{
    for (int i = 0; i < NUM_SYS_EFX; i++)
        this->ui->syseffects->layout()->removeWidget(&this->_effectButtons[i]);
    delete []this->_effectButtons;
    delete ui;
}

void MainWindow::OnSelectChannel(int channel)
{
    this->_selectedChannel = channel;
    this->SelectChannel(channel);
}

void MainWindow::SelectChannel(int channel)
{
    this->ui->instruments->resetInstruments();
    for (int i = 0;i < NUM_MIDI_PARTS; i++)
    {
        Part* part = Master::getInstance().part[i];
        if (part->Prcvchn == channel)
        {
            this->ui->instruments->addInstrument(i);
            this->SelectPart(i);
        }
    }
}

void MainWindow::OnSelectPart(int part)
{
    this->SelectPart(part);
}

void MainWindow::SelectPart(int index)
{
    Part* part = Master::getInstance().part[index];

    for (int k = 0; k < 128; k++)
        this->ui->keyboard->setNoteEnabled(k, false);

    for (int i = (NUM_KIT_ITEMS-1); i >= 0; i--)
    {
        if (part->kit[i].Penabled)
        {
            for (int k = 0; k < 128; k++)
            {
                if (k >= part->kit[i].Pminkey && k <= part->kit[i].Pmaxkey)
                {
                    this->ui->keyboard->setNoteEnabled(k, true);
                    this->ui->keyboard->setNoteColor(k, KitWindow::KitColors[i]);
                }
            }
        }
    }
}

void MainWindow::OnMasterGainChanged(int value)
{
    Master::getInstance().setPvolume(value);
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->sysuvL && event->type() == QEvent::Paint)
    {
        int lx=ui->sysuvL->width(); int ly=ui->sysuvL->height();

        int idbl=(int) (this->dbl*ly);
        int irmsdbl=(int) (this->rmsdbl*ly);

        QPainter painter;
        painter.begin(ui->sysuvL);

        painter.fillRect(0,ly-idbl,lx,idbl, QBrush(Qt::green, Qt::Dense7Pattern));
        painter.fillRect(0,0,lx,ly-idbl, QBrush(Qt::white, Qt::Dense7Pattern));
        painter.fillRect(0,ly-irmsdbl-1,lx,3, QBrush(Qt::yellow, Qt::SolidPattern));

        painter.end();
        return true;
    }
    if (watched == ui->sysuvR && event->type() == QEvent::Paint)
    {
        int lx=ui->sysuvR->width(); int ly=ui->sysuvR->height();

        int idbr=(int) (this->dbr*ly);
        int irmsdbr=(int) (this->rmsdbr*ly);

        QPainter painter;
        painter.begin(ui->sysuvR);

        painter.fillRect(0,ly-idbr,lx,idbr, QBrush(Qt::green, Qt::Dense7Pattern));
        painter.fillRect(0,0,lx,ly-idbr, QBrush(Qt::white, Qt::Dense7Pattern));
        painter.fillRect(0,ly-irmsdbr-1,lx,3, QBrush(Qt::yellow, Qt::SolidPattern));

        painter.end();
        return true;
    }
    return false;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat() == false)
        this->ui->keyboard->selectCharacter(char(event->key()), true);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat() == false)
        this->ui->keyboard->selectCharacter(char(event->key()), false);
}

void MainWindow::OnVuTimer()
{
    // This part is copied from MasterUI.fl by Nasca Octavian Paul
    static float olddbl = 0;
    static float olddbr = 0;
    static float oldrmsdbl = 0;
    static float oldrmsdbr = 0;
#define MIN_DB (-48)

    vuData data = Master::getInstance().getVuData();

    this->dbl=rap2dB(data.outpeakl);
    this->dbr=rap2dB(data.outpeakr);
    this->dbl=(MIN_DB-this->dbl)/MIN_DB;
    if (this->dbl<0.0) this->dbl=0.0;
      else if (this->dbl>1.0)this->dbl=1.0;

    this->dbr=(MIN_DB-this->dbr)/MIN_DB;
    if (this->dbr<0.0) this->dbr=0.0;
      else if (this->dbr>1.0) this->dbr=1.0;

    this->dbl=this->dbl*0.4+olddbl*0.6;
    this->dbr=this->dbr*0.4+olddbr*0.6;

    olddbl=this->dbl;
    olddbr=this->dbr;

    //compute RMS - start
    this->rmsdbl=rap2dB(data.rmspeakl);
    this->rmsdbr=rap2dB(data.rmspeakr);
    this->rmsdbl=(MIN_DB-this->rmsdbl)/MIN_DB;
    if (this->rmsdbl<0.0) this->rmsdbl=0.0;
      else if (this->rmsdbl>1.0) this->rmsdbl=1.0;

    this->rmsdbr=(MIN_DB-this->rmsdbr)/MIN_DB;
    if (this->rmsdbr<0.0) this->rmsdbr=0.0;
      else if (this->rmsdbr>1.0) this->rmsdbr=1.0;

    this->rmsdbl=this->rmsdbl*0.4+oldrmsdbl*0.6;
    this->rmsdbr=this->rmsdbr*0.4+oldrmsdbr*0.6;

    oldrmsdbl=rmsdbl;
    oldrmsdbr=rmsdbr;

    this->ui->sysuvL->repaint();
    this->ui->sysuvR->repaint();
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
