#include <Arduino.h>
#include "Style.h"

#include "cpp/scheme/scheme.cc"
#include "cpp/palettes/core.cc"
#include "cpp/palettes/tones.cc"
#include "cpp/cam/cam.cc"
#include "cpp/cam/hct.cc"
#include "cpp/cam/hct_solver.cc"
#include "cpp/utils/utils.cc"

namespace display
{
    Style::Style(const SchemePtr &scheme, SchemeColors *schemeColors, FontPtr font)
        : _foreground(0x0000), _background(0xffffff), _scheme(scheme), _schemeColors(schemeColors), _font(std::move(font))
    {
        // initialize bg/fg with scheme colors
        _background = schemeBackground(SchemeColors::Default);
        _foreground = schemeForeground(SchemeColors::Default);
    }

    Color Style::schemeBackground(SchemeColors::Modifiers modifiers) const
    {
        if (_scheme && _schemeColors)
        {
            return _schemeColors->background(_scheme.get(), modifiers);
        }
        return background();
    }

    Color Style::schemeForeground(SchemeColors::Modifiers modifiers) const
    {
        if (_scheme && _schemeColors)
        {
            return _schemeColors->foreground(_scheme.get(), modifiers);
        }
        return foreground();
    }

    class SchemeColorsBase : public SchemeColors
    {
    public:
        Color background(const Scheme *scheme, SchemeColors::Modifiers modifiers) const override
        {
            Color bg;
            if (modifiers & SchemeColors::Activated)
            {
                bg = _sbg(scheme);
            }
            else
            {
                bg = _bg(scheme);
            }
            if (modifiers & SchemeColors::Pressed)
            {
                Color fg = foreground(scheme, modifiers);
                bg = Color::blend(.12, fg, bg);
            }
            return bg;
        }

        Color foreground(const Scheme *scheme, SchemeColors::Modifiers modifiers) const override
        {
            Color fg;
            if (modifiers & SchemeColors::Disabled)
            {
                fg = Color::blend(.38, _fg(scheme), _bg(scheme));
            }
            else if (modifiers & SchemeColors::Activated)
            {
                fg = _sfg(scheme);
            }
            else if (modifiers & SchemeColors::Toggled)
            {
                fg = _pfg(scheme);
            }
            else
            {
                fg = _fg(scheme);
            }
            return fg;
        }

    private:
        virtual Color _bg(const Scheme *scheme) const = 0;
        virtual Color _fg(const Scheme *scheme) const = 0;
        virtual Color _pfg(const Scheme *scheme) const = 0;
        virtual Color _sbg(const Scheme *scheme) const = 0;
        virtual Color _sfg(const Scheme *scheme) const = 0;
    };

    class SchemeColorsApplicaton : public SchemeColorsBase
    {
    private:
        Color _bg(const Scheme *scheme) const override { return scheme->surface; }
        Color _fg(const Scheme *scheme) const override { return scheme->on_surface; };
        Color _pfg(const Scheme *scheme) const override { return scheme->primary; };
        Color _sbg(const Scheme *scheme) const override { return scheme->secondary_container; }
        Color _sfg(const Scheme *scheme) const override { return scheme->on_secondary_container; };
    };

    SchemeColorsApplicaton appcolors;
    SchemeColors *SchemeColors::Application = &appcolors;

    class SchemeColorsSystem : public SchemeColorsBase
    {
    private:
        Color _bg(const Scheme *scheme) const override { return scheme->surface_variant; }
        Color _fg(const Scheme *scheme) const override { return scheme->on_surface_variant; };
        Color _pfg(const Scheme *scheme) const override { return scheme->primary; };
        Color _sbg(const Scheme *scheme) const override { return scheme->secondary_container; }
        Color _sfg(const Scheme *scheme) const override { return scheme->on_secondary_container; };
    };

    SchemeColorsSystem syscolors;
    SchemeColors *SchemeColors::System = &syscolors;
} // namespace display
