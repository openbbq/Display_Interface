#pragma once

#include <memory>
#include "Coords.h"
#include "Font.h"

namespace display
{
    typedef uint16_t Color;

    class Style
    {
    public:
        Style(Color color)
            : _background(color), _foreground(color) {}
        Style(Color background, Color foreground, FontPtr font = nullptr)
            : _background(background), _foreground(foreground), _font(std::move(font)) {}

        template <typename... _Args>
        static std::shared_ptr<Style> create(_Args &&...__args) { return std::make_shared<Style>(std::forward<_Args>(__args)...); }

        Color background() const { return _background; }
        void background(Color value) { _background = value; }

        Color foreground() const { return _foreground; }
        void foreground(Color value) { _foreground = value; }

        FontPtr font() const { return _font; }
        void font(FontPtr value) { _font = value; }

    private:
        Color _foreground;
        Color _background;
        FontPtr _font;
    };

    typedef std::shared_ptr<Style> StylePtr;
}
