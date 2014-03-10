/*
  ZynAddSubFX - a software synthesizer

  channelclip.h - Graphics item for visualizing a MidiClip
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
#include "channelclip.h"
#include "channelcontainer.h"
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <cmath>
#include <iostream>
#include "../Sequence/sequence.h"

using namespace std;

ChannelClip::ChannelClip(ChannelContainer* container, int clipIndex)
    : SnappingGraphicsItem(container), _clip(clipIndex)
{
    MidiClip* clip = Sequence::getInstance().Pclips[this->_clip];
    QPixmap p = this->GetPixmapFromClip(clip);
    this->_notes.setPixmap(p.scaled(p.width(), 128, Qt::IgnoreAspectRatio, Qt::FastTransformation));
    this->_notes.setPos(0, 22);
    this->addToGroup(&this->_notes);

    this->_header.setRect(0, 0, p.width(), 22);
    this->_header.setPen(QPen(Qt::NoPen));
    this->_header.setBrush(QBrush(QColor::fromRgb(0, 96, 127)));
    this->addToGroup(&this->_header);

    this->_headertext.setText(Sequence::getInstance().Pclips[this->_clip]->Pname.c_str());
    this->_headertext.setPos(2, 2);
    this->_headertext.setFont(QFont("Verdana", 10, QFont::Bold));
    this->_headertext.setBrush(QBrush(Qt::white));
    this->addToGroup(&this->_headertext);

    this->_border.setRect(0, 0, p.width(), 128);
    this->addToGroup(&this->_border);

    this->Unselect();

    this->_fineStart = clip->Pstart;
}

ChannelClip::~ChannelClip()
{ }

QPixmap ChannelClip::GetPixmapFromClip(MidiClip* clip)
{
    double len = 0;
    unsigned char minrange = 0, maxrange = 0;

    clip->getSize(len, minrange, maxrange);
    minrange -= 15;
    maxrange += 15;

    QPixmap p(len * this->_container->HScale(), (maxrange - minrange) * 10);
    p.fill(QColor::fromRgb(0, 143, 191));

    QPainter painter(&p);
    for (std::vector<MidiClip::Note*>::iterator i = clip->Pnotes.begin(); i != clip->Pnotes.end(); ++i)
    {
        MidiClip::Note* n = (MidiClip::Note*)*i;
        painter.fillRect(n->start * this->_container->HScale(),
                         p.height() - ((n->note - minrange) * 10) - 5,
                         n->length * this->_container->HScale(),
                         10,
                         QColor::fromRgb(0, 48, 64));
    }
    painter.end();
    return p;
}

void ChannelClip::Select()
{
    this->_border.setPen(QPen(QBrush(Qt::red), 4));
}

void ChannelClip::Unselect()
{
    this->_border.setPen(QPen(QBrush(QColor::fromRgb(0, 172, 229)), 1));
}

void ChannelClip::UpdateClip()
{
    MidiClip* clip = Sequence::getInstance().Pclips[this->_clip];
    QPixmap p = this->GetPixmapFromClip(clip);
    this->_notes.setPixmap(p.scaled(p.width(), 128, Qt::IgnoreAspectRatio, Qt::FastTransformation));
    this->_header.setRect(this->_header.rect().x(), this->_header.rect().y(), p.width(), this->_header.rect().height());
    this->_border.setRect(this->_border.rect().x(), this->_border.rect().y(), p.width(), this->_border.rect().height());
}

void ChannelClip::SetHeight(int height)
{
    this->_border.setRect(this->_border.rect().x(), this->_border.rect().y(), this->_border.rect().width(), height);
    double scale = double(height - 20) / 128.0;
    this->_notes.setTransform(QTransform::fromScale(1.0, scale));
}

void ChannelClip::moveItem(int x, int y)
{
    MidiClip* c = Sequence::getInstance().Pclips[this->_clip];
    this->_fineStart += double(x) / this->_container->HScale();
    c->Pstart = (int)this->_fineStart;
    this->setPos(c->Pstart * this->_container->HScale(), this->y());
}

QGraphicsRectItem* ChannelClip::tempCopyRect()
{
    QGraphicsRectItem* rect = new QGraphicsRectItem();

    rect->setRect(this->_border.rect());
    rect->setPos(this->pos());
    rect->setBrush(QBrush(QColor::fromRgb(255, 255, 255, 100)));
    rect->setPen(QPen(Qt::transparent));

    return rect;
}

void ChannelClip::copyMe(double x, double y)
{
    int copyclip = Sequence::getInstance().CopyClip(this->_clip);
    MidiClip* clip = Sequence::getInstance().Pclips[copyclip];
    clip->Pstart = x;
    this->_container->UpdateItems();
}
