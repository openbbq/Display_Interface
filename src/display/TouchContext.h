#pragma once

#include "InterfaceContext.h"

namespace display
{
    class TouchContext : public InterfaceContextBase
    {
    public:
        TouchContext(const Point &current, const Point &initial) : _current(current), _initial(initial) {}

        Point current() const;
        Point initial() const;
        bool containsCurrent() const;

    private:
        Point _current;
        Point _initial;
    };
} // namespace display
