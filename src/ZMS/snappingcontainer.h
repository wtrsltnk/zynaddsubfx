#ifndef SNAPPINGCONTAINER_H
#define SNAPPINGCONTAINER_H

#include <QGraphicsScene>
#include <QGraphicsItemGroup>

class SnappingGraphicsItem;

class SnappingContainer
{
public:
    SnappingContainer();

    int ScrollOffset();
    void SetScrollOffset(int offset);
    int HScale();
    void SetHScale(int scale);
    int VScale();
    void SetVScale(int scale);

    virtual void UpdateItems() = 0;
    virtual void SelectItem(SnappingGraphicsItem* item) = 0;

protected:
    QGraphicsScene* _scene;
    QGraphicsItemGroup* _group;

private:
    int _scrollOffset;
    int _hscale, _vscale;

};

#endif // SNAPPINGCONTAINER_H
