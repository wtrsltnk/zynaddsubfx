#ifndef SNAPPINGCONTAINER_H
#define SNAPPINGCONTAINER_H

class SnappingContainer
{
public:
    SnappingContainer();

    int ScrollOffset();
    void SetScrollOffset(int offset);
    int Scale();
    void SetScale(int scale);

    virtual void UpdateClips() = 0;
private:
    int _scrollOffset;
    int _scale;

};

#endif // SNAPPINGCONTAINER_H
