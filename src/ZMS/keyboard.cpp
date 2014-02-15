/*
  ZynAddSubFX - a software synthesizer

  keyboard.cpp - Keyboard widget for running notes to the synth
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
#include "keyboard.h"
#include "ui_keyboard.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTextDocument>
#include <QTextOption>
#include <iostream>
#include "../Misc/Master.h"

using namespace std;

Keyboard::Key::Key(Octave* o, unsigned char n)
    : QGraphicsPolygonItem(o), _parent(o), _note(n), _isEnabled(true)
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
        this->_isSharp = false;
        QString txt = QString("%1%2").arg(QString('A' + a), QString::number(o->_octave));
        QGraphicsTextItem* t = new QGraphicsTextItem(txt);
        t->setPos(a * Keyboard::KeyWidth, Keyboard::KeyHeight);
        o->addToGroup(t);
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
        this->_isSharp = true;
    }
    this->setPolygon(p);
}

Keyboard::Key::~Key()
{ }

void Keyboard::Key::SetOn(bool on)
{
    if (this->_isEnabled)
    {
        if (on)
        {
            if (this->_isSharp)
                this->setBrush(QBrush(QColor::fromRgb(0, 48, 64)));
            else
                this->setBrush(QBrush(QColor::fromRgb(0, 172, 229)));
        }
        else
        {
            if (this->_isSharp)
                this->setBrush(QBrush(Qt::black));
            else
                this->setBrush(QBrush(Qt::white));
        }
        this->update();
    }
}

void Keyboard::Key::SetEnabled(bool enabled)
{
    this->_isEnabled = enabled;
    if (this->_isEnabled == false)
    {
        if (this->_isSharp)
            this->setBrush(QBrush(Qt::gray));
        else
            this->setBrush(QBrush(Qt::lightGray));
    }
    else
    {
        if (this->_isSharp)
            this->setBrush(QBrush(Qt::black));
        else
            this->setBrush(QBrush(Qt::white));
    }
    this->update();
}

void Keyboard::Key::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Master::getInstance().noteOn(0, this->_note, 100);
    this->SetOn(true);
}

void Keyboard::Key::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Master::getInstance().noteOff(0, this->_note);
    this->SetOn(false);
}

Keyboard::Octave::Octave(unsigned char o)
    : QGraphicsItemGroup(), _octave(o)
{
    this->setHandlesChildEvents(false);
    for (int i = 0; i < 12; i++)
    {
        if (o * 12 + i < 128)
        {
            this->_keys[i] = new Key(this, o * 12 + i);
            this->addToGroup(this->_keys[i]);
        }
        else
            this->_keys[i] = 0;
    }
}

Keyboard::Octave::~Octave()
{ }

int Keyboard::KeyWidth = 24;
int Keyboard::KeyHeight = 72;

Keyboard::Keyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Keyboard), _sendnotes(false)
{
    ui->setupUi(this);

    this->ui->graphicsView->setMouseTracking(true);
    this->ui->graphicsView->viewport()->setMouseTracking(true);

    this->_scene = new QGraphicsScene();
    for (int i = 0; i < MAX_OCTAVES; i++)
    {
        this->_octaves[i] = new Octave(i);
        this->_scene->addItem(this->_octaves[i]);
        this->_octaves[i]->setPos(Keyboard::KeyWidth * 7 * i, 0);
    }

    this->ui->graphicsView->setScene(this->_scene);

    for (int i = 0; i < 128; i++)
        this->_selectedNotes[i] = false;

    int startoctave = 48;
    Keyboard::_characterNoteMapping['Z'] = startoctave+0;
     Keyboard::_characterNoteMapping['S'] = startoctave+1;
    Keyboard::_characterNoteMapping['X'] = startoctave+2;
     Keyboard::_characterNoteMapping['D'] = startoctave+3;
    Keyboard::_characterNoteMapping['C'] = startoctave+4;
    Keyboard::_characterNoteMapping['V'] = startoctave+5;
     Keyboard::_characterNoteMapping['G'] = startoctave+6;
    Keyboard::_characterNoteMapping['B'] = startoctave+7;
     Keyboard::_characterNoteMapping['H'] = startoctave+8;
    Keyboard::_characterNoteMapping['N'] = startoctave+9;
     Keyboard::_characterNoteMapping['J'] = startoctave+10;
    Keyboard::_characterNoteMapping['M'] = startoctave+11;
    Keyboard::_characterNoteMapping[','] = startoctave+12;
     Keyboard::_characterNoteMapping['L'] = startoctave+13;
    Keyboard::_characterNoteMapping['.'] = startoctave+14;
     Keyboard::_characterNoteMapping[';'] = startoctave+15;
    Keyboard::_characterNoteMapping['/'] = startoctave+16;
}

Keyboard::~Keyboard()
{
    delete ui;
}

void Keyboard::SelectCharacter(char c, bool on)
{
    unsigned char note = Keyboard::_characterNoteMapping[c];
    if (note >= 0 && note < 128)
    {
        int key = note % 12;
        int octave = (note - key) / 12;
        if (octave < MAX_OCTAVES && key < 12)
            this->_octaves[(note - (note % 12)) / 12]->_keys[note % 12]->SetOn(on);
        if (this->_selectedNotes[note] != on)
        {
            if (on)
                Master::getInstance().noteOn(0, note, 100);
            else
                Master::getInstance().noteOff(0, note);

            this->_selectedNotes[note] = on;
        }
    }
}

void Keyboard::SetNoteOn(unsigned char note, bool on)
{
    if (note >= 0 && note < 128)
    {
        int key = note % 12;
        int octave = (note - key) / 12;
        if (octave < MAX_OCTAVES && key < 12)
            this->_octaves[(note - (note % 12)) / 12]->_keys[note % 12]->SetOn(on);
    }
}

void Keyboard::SetNoteEnabled(unsigned char note, bool enabled)
{
    if (note >= 0 && note < 128)
    {
        int key = note % 12;
        int octave = (note - key) / 12;
        if (octave < MAX_OCTAVES && key < 12)
            this->_octaves[octave]->_keys[key]->SetEnabled(enabled);
    }
}

void Keyboard::SetNoteColor(unsigned char note, const QColor& color)
{
    if (note >= 0 && note < 128)
    {
        int key = note % 12;
        int octave = (note - key) / 12;
        if (octave < MAX_OCTAVES && key < 12)
            this->_octaves[octave]->_keys[key]->setPen(QPen(color));
    }
}

unsigned char Keyboard::_characterNoteMapping[128] = { 0 };
