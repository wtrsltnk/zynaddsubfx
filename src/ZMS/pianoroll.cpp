/*
  ZynAddSubFX - a software synthesizer

  pianoroll.cpp - User control for editing midi clips
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
#include "pianoroll.h"
#include "ui_pianoroll.h"
#include "pianorollnote.h"
#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>
#include "../Sequence/sequence.h"

PianoRoll::PianoRoll(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PianoRoll)
{
    ui->setupUi(this);

    this->_scene = new QGraphicsScene();
    this->ui->notes->setScene(this->_scene);
    this->ui->notes->installEventFilter(this);
    this->ui->notes->setBackgroundBrush(QBrush(QColor(35, 35, 35)));

    this->_group = new QGraphicsItemGroup();
    this->_group->setFiltersChildEvents(false);
    this->_group->setHandlesChildEvents(false);
    this->_scene->addItem(this->_group);

    this->_bg = new QGraphicsRectItem();
    this->_bg->setRect(0, 0, 1000, 127 * this->VScale());
    this->_scene->addItem(this->_bg);
}

PianoRoll::~PianoRoll()
{
    delete ui;
}

void PianoRoll::ShowClips(const QList<int>& clips)
{
    this->_clips = clips;
    this->ShowSelectedNotes();
}

void PianoRoll::UpdateItems()
{
    this->ShowSelectedNotes();
}

void PianoRoll::SelectItem(SnappingGraphicsItem* item)
{
    item->Select();
}

void PianoRoll::ShowSelectedNotes()
{
    while (this->_group->childItems().empty() == false)
    {
        PianoRollNote* note = (PianoRollNote*)this->_group->childItems().back();
        this->_group->removeFromGroup(note);
        delete note;
    }

    for (QList<int>::iterator i = this->_clips.begin(); i != this->_clips.end(); ++i)
    {
        MidiClip* clip = Sequence::getInstance().Pclips[*i];
        if (clip != 0)
        {
            for (std::vector<MidiClip::Note*>::iterator j = clip->Pnotes.begin(); j != clip->Pnotes.end(); ++j)
            {
                MidiClip::Note* note = *j;
                PianoRollNote* n = new PianoRollNote(this, note);
                this->_group->addToGroup(n);
            }
        }
    }
}

bool PianoRoll::eventFilter(QObject* watched, QEvent* event)
{
    return false;
}
