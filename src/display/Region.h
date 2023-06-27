#pragma once

#include "Coords.h"

#include <vector>

namespace display
{
    class Region : public std::vector<Rect>
    {
    public:
        void include(const Rect &rc);
        void exclude(const Rect &rc);

    private:
    };
}
