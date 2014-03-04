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
    ui(new Ui::PianoRoll),
    _hscale(25)
{
    ui->setupUi(this);

    this->_scene = new QGraphicsScene();
    this->ui->notes->setScene(this->_scene);
    this->ui->notes->installEventFilter(this);

    this->_group = new QGraphicsItemGroup();
    this->_group->setFiltersChildEvents(false);
    this->_group->setHandlesChildEvents(false);
    this->_scene->addItem(this->_group);
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

void PianoRoll::SelectItem(SnappingGraphicsItem* item)
{
    item->Select();
}

void PianoRoll::ShowSelectedNotes()
{
    while (this->_scene->items().empty() == false)
    {
        QGraphicsItemGroup* group = (QGraphicsItemGroup*)this->_scene->items().back();
        while (group->childItems().empty() == false)
        {
            QGraphicsItem* item = group->childItems().back();
            group->removeFromGroup(item);
            delete item;
        }
        this->_scene->removeItem(group);
        delete group;
    }
    for (QList<int>::iterator i = this->_clips.begin(); i != this->_clips.end(); ++i)
    {
        MidiClip* clip = Sequence::getInstance().Pclips[*i];
        if (clip != 0)
        {
            QGraphicsItemGroup* grp = new QGraphicsItemGroup();
            grp->data(0).setValue(*i);
            this->_group->addToGroup(grp);
            for (std::vector<MidiClip::Note*>::iterator j = clip->Pnotes.begin(); j != clip->Pnotes.end(); ++j)
            {
                MidiClip::Note* note = *j;
                QGraphicsItemGroup* n = new PianoRollNote(this, note);
                grp->addToGroup(n);
            }
        }
    }
}

bool PianoRoll::eventFilter(QObject* watched, QEvent* event)
{
    return false;
}
