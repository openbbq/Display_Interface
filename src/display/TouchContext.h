#pragma once

#include "WindowContext.h"

namespace display
{
    class TouchContext : public WindowContextBase
    {
    public:
        TouchContext(const Point &current, const Point &initial) : _current(current), _initial(initial) {}

        Point current() const
        {
            Point o = origin();
            return Point(_current.x() - o.x(), _current.y() - o.y());
        }

        Point initial() const
        {
            Point o = origin();
            return Point(_initial.x() - o.x(), _initial.y() - o.y());
        }

        bool containsCurrent() const { return extent().contains(_current); }

    private:
        Point _current;
        Point _initial;
    };
} // namespace display
