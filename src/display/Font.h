#pragma once

#include <memory>
#include "Color.h"

namespace display
{
    class DrawContext;

    class Font
    {
    public:
        virtual int kind() const = 0;

        virtual Rect measure(const String &text) const = 0;
        virtual void draw(DrawContext *ctx, const Rect &clipBox, Color background, Color foreground, int16_t x, int16_t y, const String &text) const = 0;
    };

    typedef std::shared_ptr<Font> FontPtr;

} // namespace display
