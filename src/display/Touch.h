#pragma once

#include "Coords.h"

namespace display
{
    class Touch
    {
    public:
        virtual void touchDown(const Point &value) = 0;
        virtual void touchMove(const Point &value) = 0;
        virtual void touchUp(const Point &value) = 0;
    };
}
