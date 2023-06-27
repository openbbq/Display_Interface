#pragma once

#include "Coords.h"

namespace display
{
    class WindowContext
    {
    public:
        virtual ~WindowContext() = default;

        // in display coordinates, the current location of draw coordinates {0, 0}
        virtual const Point &origin() const = 0;
        virtual void origin(const Point &value) = 0;

        // in display coordinates, the current clipped extent
        virtual const Rect &extent() const = 0;
        virtual void extent(const Rect &value) = 0;

        struct PriorViewport
        {
            Point origin;
            Rect extent;
        };

        virtual PriorViewport enter_viewport(const Rect &viewport) = 0;
        virtual void restore_viewport(const PriorViewport &value) = 0;
    };

    class WindowContextBase : public WindowContext
    {
    public:
        const Point &origin() const override { return _origin; }
        void origin(const Point &value) override { _origin = value; }

      // in display coordinates, the current clipped extent
        const Rect &extent() const override { return _extent; }
        void extent(const Rect &value) override { _extent = value; }

        PriorViewport enter_viewport(const Rect &viewport) override;
        void restore_viewport(const PriorViewport &prior) override;

    private:
        Point _origin;
        Rect _extent;
    };
} // namespace display
