#include "snappingcontainer.h"

SnappingContainer::SnappingContainer()
    : _scrollOffset(0), _hscale(25), _vscale(10)
{ }

int SnappingContainer::ScrollOffset()
{
    return this->_scrollOffset;
}

void SnappingContainer::SetScrollOffset(int offset)
{
    this->_scrollOffset = offset;
}

int SnappingContainer::HScale()
{
    return this->_hscale;
}

void SnappingContainer::SetHScale(int scale)
{
    this->_hscale = scale;
}

int SnappingContainer::VScale()
{
    return this->_vscale;
}

void SnappingContainer::SetVScale(int scale)
{
    this->_vscale = scale;
}
