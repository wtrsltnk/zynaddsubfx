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
    this->setAcceptHoverEvents(true);
    QPolygon p;
    if (n % 12 == 0 || n % 12 == 2 || n % 12 == 4 || n % 12 == 5 || n % 12 == 7 || n % 12 == 9 || n % 12 == 11)
    {
        int a = 0;
        if (n % 12 == 2) a = 1;
        if (n % 12 == 4) a = 2;
        if (n % 12 == 5) a = 3;
        if (n % 12 == 7) a = 4;
        if (n % 12 == 9) a = 5;
        if (n % 12 == 11) a = 6;
        p << QPoint(a * Keyboard::KeyWidth + 0, 0)
          << QPoint(a * Keyboard::KeyWidth + Keyboard::KeyWidth, 0)
          << QPoint(a * Keyboard::KeyWidth + Keyboard::KeyWidth, Keyboard::KeyHeight)
          << QPoint(a * Keyboard::KeyWidth + 0, Keyboard::KeyHeight);
        this->setBrush(QBrush(Qt::white));
        this->setZValue(1);
        this->isSharp = false;
    }
    else
    {
        int a = 0;
        if (n % 12 == 3) a = 1;
        if (n % 12 == 6) a = 3;
        if (n % 12 == 8) a = 4;
        if (n % 12 == 10) a = 5;
        p << QPoint(a * Keyboard::KeyWidth + ((Keyboard::KeyWidth/3) * 2), 0)
          << QPoint(a * Keyboard::KeyWidth + ((Keyboard::KeyWidth/3) * 2) + ((Keyboard::KeyWidth/3) * 2), 0)
          << QPoint(a * Keyboard::KeyWidth + ((Keyboard::KeyWidth/3) * 2) + ((Keyboard::KeyWidth/3) * 2), Keyboard::KeyHeight * 0.6)
          << QPoint(a * Keyboard::KeyWidth + ((Keyboard::KeyWidth/3) * 2), Keyboard::KeyHeight * 0.6);
        this->setBrush(QBrush(Qt::black));
        this->setZValue(2);
        this->isSharp = true;
    }
    this->setPolygon(p);
}

Keyboard::Key::~Key()
{ }

void Keyboard::Key::setOn(bool on)
{
    if (on)
    {
        if (this->isSharp)
            this->setBrush(QBrush(Qt::gray));
        else
            this->setBrush(QBrush(Qt::lightGray));
    }
    else
    {
        if (this->isSharp)
            this->setBrush(QBrush(Qt::black));
        else
            this->setBrush(QBrush(Qt::white));
    }
    this->update();
}

void Keyboard::Key::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Master::getInstance().noteOn(0, this->note, 100);
    if (this->isSharp)
        this->setBrush(QBrush(Qt::gray));
    else
        this->setBrush(QBrush(Qt::lightGray));
    this->update();
}

void Keyboard::Key::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Master::getInstance().noteOff(0, this->note);
    if (this->isSharp)
        this->setBrush(QBrush(Qt::black));
    else
        this->setBrush(QBrush(Qt::white));
    this->update();
}

Keyboard::Octave::Octave(unsigned char o)
    : QGraphicsItemGroup(), octave(o)
{
    this->setHandlesChildEvents(false);
    for (int i = 0; i < 12; i++)
    {
        this->keys[i] = new Key(this, o * 12 + i);
        this->addToGroup(this->keys[i]);
    }
}

Keyboard::Octave::~Octave()
{ }

int Keyboard::KeyWidth = 24;
int Keyboard::KeyHeight = 100;

Keyboard::Keyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Keyboard),
    keywidth(21), keyheight(90), octavecount(9), sendnotes(false)
{
    ui->setupUi(this);

    this->ui->graphicsView->setMouseTracking(true);
    this->ui->graphicsView->viewport()->setMouseTracking(true);

    this->scene = new QGraphicsScene();
    for (int i = 0; i < 10; i++)
    {
        this->octaves[i] = new Octave(i);
        this->scene->addItem(this->octaves[i]);
        this->octaves[i]->setPos(Keyboard::KeyWidth * 7 * i, 0);
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

void Keyboard::selectCharacter(char c, bool on)
{
    unsigned char note = Keyboard::characterNoteMapping[c];
    if (note != 0)
    {
        this->octaves[(note - (note % 12)) / 12]->keys[note % 12]->setOn(on);
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

unsigned char Keyboard::characterToNote(char c)
{
    return 0;
}

unsigned char Keyboard::characterNoteMapping[128] = { 0 };
