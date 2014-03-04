#ifndef SNAPPINGCONTAINER_H
#define SNAPPINGCONTAINER_H

class SnappingGraphicsItem;

class SnappingContainer
{
public:
    SnappingContainer();

    int ScrollOffset();
    void SetScrollOffset(int offset);
    int Scale();
    void SetScale(int scale);

    virtual void UpdateItems() = 0;
    virtual void SelectItem(SnappingGraphicsItem* item) = 0;
private:
    int _scrollOffset;
    int _scale;

};

#endif // SNAPPINGCONTAINER_H
