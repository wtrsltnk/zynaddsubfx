#include "channelclip.h"
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <cmath>

ChannelClip::ChannelClip(MidiClip *clip)
    : _clip(clip), _drag(false)
{
    QPixmap p = this->getPixmapFromClip();
    this->_notes.setPixmap(p.scaled(p.width(), 128, Qt::IgnoreAspectRatio, Qt::FastTransformation));
    this->_notes.setPos(0, 22);
    this->addToGroup(&this->_notes);

    this->_header.setRect(0, 0, p.width(), 22);
    this->_header.setPen(QPen(Qt::NoPen));
    this->_header.setBrush(QBrush(QColor::fromRgb(0, 96, 127)));
    this->addToGroup(&this->_header);

    this->_headertext.setText("test");
    this->_headertext.setPos(2, 2);
    this->_headertext.setFont(QFont("Verdana", 12, QFont::Bold));
    this->_headertext.setBrush(QBrush(Qt::white));
    this->addToGroup(&this->_headertext);

    this->_border.setPen(QPen(QColor::fromRgb(0, 172, 229)));
    this->_border.setRect(0, 0, p.width(), 128);
    this->addToGroup(&this->_border);

    this->unselect();
}

ChannelClip::~ChannelClip()
{ }

void ChannelClip::select()
{
    this->_border.setPen(QPen(Qt::red));
    this->setOpacity(1.0);
}

void ChannelClip::unselect()
{
    this->_border.setPen(QPen(QColor::fromRgb(0, 172, 229)));
    this->setOpacity(0.6);
}

QPixmap ChannelClip::getPixmapFromClip()
{
    double len = 0;
    unsigned char minrange = 0, maxrange = 0;
    this->_clip->getSize(len, minrange, maxrange);
    minrange -= 15;
    maxrange += 15;
    QPixmap p(100 * len, (maxrange - minrange) * 10);
    p.fill(QColor::fromRgb(0, 143, 191));

    QPainter painter(&p);
    for (std::vector<MidiClip::Note*>::iterator i = this->_clip->Pnotes.begin(); i != this->_clip->Pnotes.end(); ++i)
    {
        MidiClip::Note* n = (MidiClip::Note*)*i;
        painter.fillRect(n->start * 100,
                         p.height() - ((n->note - minrange) * 10) - 5,
                         n->length * 100,
                         10,
                         QColor::fromRgb(0, 48, 64));
    }
    painter.end();
    return p;
}

void ChannelClip::setHeight(int height)
{
    this->_border.setRect(this->_border.rect().x(), this->_border.rect().y(), this->_border.rect().width(), height);
    double scale = double(height - 20) / 128.0;
    this->_notes.setTransform(QTransform::fromScale(1.0, scale));
}

void ChannelClip::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{

}

void ChannelClip::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{

}

void ChannelClip::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{

}

void ChannelClip::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->select();
    this->_drag = true;
    this->_dragStart = event->pos();
}

void ChannelClip::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (this->_drag)
    {
        int x = this->mapToScene(event->pos()).x() - this->_dragStart.x();
        this->setPos(x - (x % 100), this->y());
    }
}

void ChannelClip::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    this->_drag = false;
}

void ChannelClip::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // todo : open this clip in a pianoroll
}
