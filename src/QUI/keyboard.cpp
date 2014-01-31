/*
  ZynAddSubFX - a software synthesizer

  keyboard.cpp - Keyboard widget for running notes to the synth
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
#include "keyboard.h"
#include "ui_keyboard.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "../Misc/Master.h"

using namespace std;

Keyboard::Key::Key(Octave* o, unsigned char n)
    : QGraphicsPolygonItem(o), note(n)
{
    QPolygon p;
    p << QPoint((n % 12) * Keyboard::KeyWidth + 0, 0)
      << QPoint((n % 12) * Keyboard::KeyWidth + Keyboard::KeyWidth, 0)
      << QPoint((n % 12) * Keyboard::KeyWidth + Keyboard::KeyWidth, 50)
      << QPoint((n % 12) * Keyboard::KeyWidth + 0, 50);
    this->setPolygon(p);
}

Keyboard::Key::~Key()
{ }

Keyboard::Octave::Octave(unsigned char o)
    : QGraphicsItemGroup(), octave(o)
{
    for (int i = 0; i < 12; i++)
    {
        this->keys[i] = new Key(this, o * 12 + i);
        this->addToGroup(this->keys[i]);
    }
}

Keyboard::Octave::~Octave()
{ }

int Keyboard::KeyWidth = 24;

Keyboard::Keyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Keyboard),
    keywidth(21), keyheight(90), octavecount(9), sendnotes(false)
{
    ui->setupUi(this);

    this->scene = new QGraphicsScene();
    for (int i = 0; i < 10; i++)
    {
        this->octaves[i] = new Octave(i);
        this->scene->addItem(this->octaves[i]);
        this->octaves[i]->setPos(Keyboard::KeyWidth * 7 * i, i*4);
    }

    this->ui->graphicsView->setScene(this->scene);

    for (int i = 0; i < 128; i++)
        this->selectedNotes[i] = false;

    int startoctave = 48;
    Keyboard::characterNoteMapping['Z'] = startoctave+0;
     Keyboard::characterNoteMapping['S'] = startoctave+1;
    Keyboard::characterNoteMapping['X'] = startoctave+2;
     Keyboard::characterNoteMapping['D'] = startoctave+3;
    Keyboard::characterNoteMapping['C'] = startoctave+4;
    Keyboard::characterNoteMapping['V'] = startoctave+5;
     Keyboard::characterNoteMapping['G'] = startoctave+6;
    Keyboard::characterNoteMapping['B'] = startoctave+7;
     Keyboard::characterNoteMapping['H'] = startoctave+8;
    Keyboard::characterNoteMapping['N'] = startoctave+9;
     Keyboard::characterNoteMapping['J'] = startoctave+10;
    Keyboard::characterNoteMapping['M'] = startoctave+11;
    Keyboard::characterNoteMapping[','] = startoctave+12;
     Keyboard::characterNoteMapping['L'] = startoctave+13;
    Keyboard::characterNoteMapping['.'] = startoctave+14;
     Keyboard::characterNoteMapping[';'] = startoctave+15;
    Keyboard::characterNoteMapping['/'] = startoctave+16;
}

Keyboard::~Keyboard()
{
    delete ui;
}

bool Keyboard::eventFilter(QObject* watched, QEvent* event)
{
//    if (watched == ui->piano && event->type() == QEvent::Paint)
//    {
//        QPainter painter;
//        painter.begin(ui->piano);
//        painter.setRenderHint(QPainter::Antialiasing, false);
//        for (int i = 0; i < this->octavecount * 7; i++)
//        {
//            painter.fillRect(1, 0, this->keywidth-2, this->keyheight, QBrush(Qt::white));
//            painter.setPen(Qt::red);
//            if (this->selectedNotes[this->keyToNote(i)])
//                painter.drawEllipse(this->keywidth/3, this->keyheight-20, this->keywidth/3, this->keywidth/3);
//            painter.setPen(Qt::gray);
//            painter.setFont(QFont("Verdana", 8));
//            QString note = 'A' + (i % 7) + QString("%1").arg(i / 7);
//            painter.drawText(1, this->keyheight-12, this->keywidth-2, 10, Qt::AlignHCenter | Qt::AlignVCenter, note);
//            painter.translate(this->keywidth, 0);
//        }

//        painter.resetTransform();
//        for (int i = 0; i < this->octavecount * 7 - 1; i++)
//        {
//            if (i % 7 != 2 && i % 7 != 5)
//            {
//                painter.fillRect((this->keywidth/3) * 2, 0, (this->keywidth/3) * 2, this->keyheight * 0.6, QBrush(Qt::black));
//                if (this->selectedNotes[this->keyToNote(i + 1000)])
//                    painter.drawEllipse(this->keywidth - (this->keywidth/6) - 1, (this->keyheight/3), this->keywidth/3, this->keywidth/3);
//            }
//            painter.translate(this->keywidth, 0);
//        }
//        painter.end();
//    }
//    if (watched == ui->piano && event->type() == QEvent::MouseMove)
//    {
//        int key = ((QMouseEvent*)event)->pos().x() / this->keywidth;
//        if (((QMouseEvent*)event)->pos().y() < (this->keyheight / 3) * 2
//                && (
//                    (((QMouseEvent*)event)->pos().x() % this->keywidth) <= (this->keywidth / 3)
//                    || (((QMouseEvent*)event)->pos().x() % this->keywidth) >= (this->keywidth / 3) * 2
//                ))
//        {
//            int i = (((QMouseEvent*)event)->pos().x() - ((this->keywidth / 3) * 2)) / this->keywidth;
//            if (i % 7 != 2 && i % 7 != 5)
//                key = 1000 + i;
//        }

//        this->selectKeyByMouse(key);
//        this->ui->piano->update();
//    }
//    if (watched == ui->piano && event->type() == QEvent::MouseButtonPress)
//    {
//        this->sendnotes = true;
//        Master::getInstance().noteOn(0, this->keyToNote(this->selectedkey), 100);
//    }
//    if (watched == ui->piano && event->type() == QEvent::MouseButtonRelease)
//    {
//        this->sendnotes = false;
//        Master::getInstance().noteOff(0, this->keyToNote(this->selectedkey));
//    }
    return false;
}

void Keyboard::selectKeyByMouse(int key)
{
//    if (this->selectedkey != key)
//    {
//        if (sendnotes)
//        {
//            Master::getInstance().noteOff(0, this->keyToNote(this->selectedkey));
//            Master::getInstance().noteOn(0, this->keyToNote(key), 100);
//        }
//    }
//    this->selectedkey = key;
}

void Keyboard::selectCharacter(char c, bool on)
{
    unsigned char note = Keyboard::characterNoteMapping[c];
    if (note != 0)
    {
        if (this->selectedNotes[note] != on)
        {
            if (on)
                Master::getInstance().noteOn(0, note, 100);
            else
                Master::getInstance().noteOff(0, note);

            this->selectedNotes[note] = on;
        }
    }
}

unsigned char Keyboard::keyToNote(int key)
{
    if (key >= 0 && key < 1000)
    {
        unsigned char a[] = { 0, 2, 4, 5, 7, 9, 11 };
        unsigned char b = key - (key % 7);
        b = (b / 7) * 12;
        b += a[key % 7];
        return b;
    }
    else
    {
        key -= 1000;
        unsigned char a[] = { 1, 3, 5, 6, 8, 10, 10 };
        unsigned char b = key - (key % 7);
        b = (b / 7) * 12;
        b += a[key % 7];
        return b;
    }
}

unsigned char Keyboard::characterToNote(char c)
{
    return 0;
}

unsigned char Keyboard::characterNoteMapping[128] = { 0 };
