#pragma once

#include "InterfaceContext.h"
#include "Coords.h"
#include "Style.h"
#include "Region.h"

namespace display
{
    class DrawContext : public InterfaceContextBase
    {
    public:
        virtual ~DrawContext() = default;

        virtual void initialize(const Rect &rect, const Region &invalid) = 0;

        enum Alignment
        {
            HORIZONTAL_LEFT = 0x01,
            HORIZONTAL_CENTER = 0x02,
            HORIZONTAL_RIGHT = 0x04,
            VERTICAL_TOP = 0x10,
            VERTICAL_CENTER = 0x20,
            VERTICAL_BOTTOM = 0x40,

            LEFT = HORIZONTAL_LEFT | VERTICAL_CENTER,
            RIGHT = HORIZONTAL_RIGHT | VERTICAL_CENTER,
            TOP = HORIZONTAL_CENTER | VERTICAL_TOP,
            BOTTOM = HORIZONTAL_CENTER | VERTICAL_BOTTOM,
            CENTER = HORIZONTAL_CENTER | VERTICAL_CENTER,
            TOP_LEFT = HORIZONTAL_LEFT | VERTICAL_TOP,
            BOTTOM_RIGHT = HORIZONTAL_RIGHT | VERTICAL_BOTTOM,
        };

        virtual void draw(const Rect &rc, Color color) = 0;
        virtual void draw(const RoundRect &rr, Color color) = 0;
        virtual void draw(Point pt0, Point pt1, Color color) = 0;
        virtual void draw(const Rect &rc, Color background, Color foreground, const FontPtr &font, const String &text, int alignment = TOP_LEFT) = 0;
        virtual Rect measure(const FontPtr &font, const String &text) const = 0;

        void draw(const Rect &rc, const StylePtr &style)
        {
            draw(rc, style->background());
        }
        void draw(const Rect &rc, const StylePtr &style, const String &text, int alignment = TOP_LEFT)
        {
            draw(rc, style->background(), style->foreground(), style->font(), text, alignment);
        }

        virtual Region invalid() const = 0;
        virtual void include(const Rect &rc) = 0;
        virtual void exclude(const Rect &rc) = 0;
    };

    class DrawContextBase : public DrawContext
    {
    public:
        void initialize(const Rect &rect, const Region &invalid) override;

        Region invalid() const override;
        void include(const Rect &rc) override;
        void exclude(const Rect &rc) override;

    protected:
        Region _region;
    };
}
