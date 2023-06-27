#include <Arduino.h>
#include "TouchContext.h"

namespace display
{
    Point TouchContext::current() const
    {
        Point o = origin();
        return Point(_current.x() - o.x(), _current.y() - o.y());
    }

    Point TouchContext::initial() const
    {
        Point o = origin();
        return Point(_initial.x() - o.x(), _initial.y() - o.y());
    }

    bool TouchContext::containsCurrent() const
    {
        return extent().contains(_current);
    }
}
