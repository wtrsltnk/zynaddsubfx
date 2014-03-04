#include "snappingcontainer.h"

SnappingContainer::SnappingContainer()
    : _scrollOffset(0), _scale(25)
{ }

int SnappingContainer::ScrollOffset()
{
    return this->_scrollOffset;
}

void SnappingContainer::SetScrollOffset(int offset)
{
    this->_scrollOffset = offset;
}

int SnappingContainer::Scale()
{
    return this->_scale;
}

void SnappingContainer::SetScale(int scale)
{
    this->_scale = scale;
}
