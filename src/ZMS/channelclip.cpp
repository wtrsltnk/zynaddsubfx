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

ChannelClip::ChannelClip(ChannelContainer* container, int clip)
    : _container(container), _clip(clip), _drag(0)
{
    QPixmap p = this->GetPixmapFromClip();
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
}

ChannelClip::~ChannelClip()
{ }

QPixmap ChannelClip::GetPixmapFromClip()
{
    MidiClip* clip = Sequence::getInstance().Pclips[this->_clip];
    double len = 0;
    unsigned char minrange = 0, maxrange = 0;

    clip->getSize(len, minrange, maxrange);
    minrange -= 15;
    maxrange += 15;

    QPixmap p(len * this->_container->vscale(), (maxrange - minrange) * 10);
    p.fill(QColor::fromRgb(0, 143, 191));

    QPainter painter(&p);
    for (std::vector<MidiClip::Note*>::iterator i = clip->Pnotes.begin(); i != clip->Pnotes.end(); ++i)
    {
        MidiClip::Note* n = (MidiClip::Note*)*i;
        painter.fillRect(n->start * this->_container->vscale(),
                         p.height() - ((n->note - minrange) * 10) - 5,
                         n->length * this->_container->vscale(),
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
    QPixmap p = this->GetPixmapFromClip();
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

void ChannelClip::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->Select();
    this->_container->SelectClip(this);

    if (event->buttons() &= Qt::LeftButton && event->modifiers().testFlag(Qt::ControlModifier))
    {
        this->_drag = 2;
        this->_dragStart = event->pos();
        this->_copyClip = new QGraphicsRectItem(this->parentItem());
        this->_copyClip->setRect(this->_border.rect());
        this->_copyClip->setPos(this->pos());
        this->_copyClip->setBrush(QBrush(QColor::fromRgb(255, 255, 255, 100)));
        this->_copyClip->setPen(QPen(Qt::transparent));
        ((QGraphicsItemGroup*)this->parentItem())->addToGroup(this->_copyClip);
    }
    else if (event->buttons() &= Qt::LeftButton)
    {
        this->_drag = 1;
        this->_dragStart = event->pos();
    }
}

void ChannelClip::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    int x = this->mapToScene(event->pos()).x() - this->_dragStart.x() - this->boundingRect().x();
    if (this->_drag == 1)   // normal drag
    {
        this->setPos(x - (x % (4 * this->_container->vscale())), this->y());

        MidiClip* c = Sequence::getInstance().Pclips[this->_clip];
        c->Pstart = this->x() / this->_container->vscale();
    }
    if (this->_drag == 2)
        this->_copyClip->setPos(x - (x % (4 * this->_container->vscale())), this->y());
}

void ChannelClip::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (this->_drag == 2)
    {
        int copyclip = Sequence::getInstance().CopyClip(this->_clip);
        MidiClip* clip = Sequence::getInstance().Pclips[copyclip];
        clip->Pstart = this->_copyClip->x() / this->_container->vscale();
        this->_container->UpdateClips();

        this->removeFromGroup(this->_copyClip);
        delete this->_copyClip;
        this->_copyClip = 0;
    }
    this->_drag = 0;
}

void ChannelClip::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // todo : open this clip in a pianoroll
}

void ChannelClip::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{ }

void ChannelClip::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{ }

void ChannelClip::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{ }
