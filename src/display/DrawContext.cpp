#include <Arduino.h>
#include "DrawContext.h"

namespace display
{
    void DrawContextBase::initialize(const Rect &rect, const Region &invalid)
    {
        origin(rect.origin());
        extent(rect);
        for (const Rect &rc : invalid)
        {
            _region.push_back(rc);
        }
    }

    Region DrawContextBase::invalid() const
    {
        Region r;
        for (const Rect &rc : _region)
        {
            Rect clip = rc.intersection(extent()).offset(-origin());
            if (!clip.empty())
            {
                r.include(clip);
            }
        }
        return r;
    }

    void DrawContextBase::include(const Rect &rc)
    {
        _region.include(rc.offset(origin()).intersection(extent()));
    }

    void DrawContextBase::exclude(const Rect &rc)
    {
        _region.exclude(rc.offset(origin()).intersection(extent()));
    }
}
