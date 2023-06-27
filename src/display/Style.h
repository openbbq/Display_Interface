#pragma once

#include <gfxfont.h>
#include <memory>
#include "Coords.h"

namespace display
{
    typedef uint16_t Color;

    class Font
    {
    public:
        Font(int size = 1) : _glyphs(nullptr), _size(size) {}
        Font(const GFXfont *glyphs, int size = 1);

        template <typename... _Args>
        static std::shared_ptr<Font> create(_Args &&...__args) { return std::make_shared<Font>(std::forward<_Args>(__args)...); }

        const GFXfont *glyphs() const { return _glyphs; }
        int size() const { return _size; }

        Rect measure(const String &text) const;
        Rect cell() const { return Rect(0, _celltop * _size, _cellwidth, _cellbottom * _size); }

    private:
        const GFXfont *_glyphs;
        int _size = 1;

        int _glyphtop = 0;    // highest set pixel (negative)
        int _glyphbottom = 0; // lowest set pixel +1 (positive)
        int _celltop = 0;     // cell top (includes yAdvance padding)
        int _cellbottom = 0;  // cell bottom (includes yAdvance padding)
        int _cellwidth = 0;   // xAdvance of character '0'
    };

    typedef std::shared_ptr<Font> FontPtr;

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
