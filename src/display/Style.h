#pragma once

#include <memory>
#include "Coords.h"
#include "Color.h"
#include "Font.h"

#include "cpp/scheme/scheme.h"

namespace display
{
    using material_color_utilities::Scheme;
    typedef std::shared_ptr<Scheme> SchemePtr;

    class SchemeColors
    {
    public:
        enum Modifiers
        {
            Default = 0,        // standard, enabled text
            Disabled = 1 << 0,  // aka not enabled
            Toggled = 1 << 1,   // aka selected
            Pressed = 1 << 2,   // touchDown over element
            Activated = 1 << 3, // current nav element
        };

        virtual Color background(const Scheme *scheme, Modifiers modifiers) const = 0;
        virtual Color foreground(const Scheme *scheme, Modifiers modifiers) const = 0;

        static SchemeColors *Application;
        static SchemeColors *System;
    };

    class Style
    {
    public:
        Style(Color color)
            : _background(color), _foreground(color) {}
        Style(Color background, Color foreground, FontPtr font = nullptr)
            : _background(background), _foreground(foreground), _font(std::move(font)) {}
        Style(const SchemePtr &scheme, SchemeColors *colors, FontPtr font = nullptr);

        template <typename... _Args>
        static std::shared_ptr<Style> create(_Args &&...__args) { return std::make_shared<Style>(std::forward<_Args>(__args)...); }

        Color background() const { return _background; }
        void background(Color value) { _background = value; }

        Color foreground() const { return _foreground; }
        void foreground(Color value) { _foreground = value; }

        FontPtr font() const { return _font; }
        void font(FontPtr value) { _font = value; }

        SchemePtr scheme() const { return _scheme; }
        void scheme(const SchemePtr &value) { _scheme = value; }

        SchemeColors *schemeColors() const { return _schemeColors; }
        void schemeColors(SchemeColors *value) { _schemeColors = value; }

        Color schemeBackground(SchemeColors::Modifiers modifiers) const;
        Color schemeForeground(SchemeColors::Modifiers modifiers) const;

    private:
        Color _foreground;
        Color _background;
        FontPtr _font;
        SchemePtr _scheme;
        SchemeColors *_schemeColors = nullptr;
    };

    typedef std::shared_ptr<Style> StylePtr;
}

inline display::SchemeColors::Modifiers operator|(display::SchemeColors::Modifiers a, display::SchemeColors::Modifiers b)
{
    return display::SchemeColors::Modifiers(int(a) | int(b));
}
