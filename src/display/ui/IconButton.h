#pragma once

#include "Button.h"

namespace display::ui
{
    struct Icon
    {
        FontPtr font;
        String glyph;
    };

    class IconButton : public Button
    {
    public:
        IconButton(StylePtr style, const Icon &icon, const Icon &iconToggled)
            : Button(style), _icon(icon), _iconToggled(iconToggled)
        {
        }
        IconButton(StylePtr style, const FontPtr &font, int icon, int iconToggled)
            : Button(style), _icon(Icon{font, String((char)icon)}), _iconToggled(Icon{font, String((char)iconToggled)})
        {
        }

        Size measureHandler(const Size &available) const override
        {
            Rect rc = _icon.font->measure(_icon.glyph);
            int w = rc.width();
            int h = rc.height();
            if (_iconToggled.glyph)
            {
                rc = _iconToggled.font->measure(_iconToggled.glyph);
                if (w < rc.width())
                {
                    w = rc.width();
                }
                if (h < rc.height())
                {
                    h = rc.height();
                }
            }
            return Size(w, h);
        }

        void drawHandler(DrawContext *ctx) override
        {
            SchemeColors::Modifiers mod = SchemeColors::Default;
            if (disabled())
            {
                mod = mod | SchemeColors::Disabled;
            }
            if (toggled())
            {
                mod = mod | SchemeColors::Toggled;
            }
            if (activated())
            {
                mod = mod | SchemeColors::Activated;
            }
            if (pressed())
            {
                mod = mod | SchemeColors::Pressed;
            }

            auto s = style();
            Color bbg = s->schemeBackground(SchemeColors::Default);
            Color bg = s->schemeBackground(mod);
            Color fg = s->schemeForeground(mod);

            Icon &icon = (toggled()||activated()) ? _iconToggled : _icon;

            Rect rc = content();
            // TODO - center and draw smaller, then round-rect with sl and fill outside with bg
            ctx->draw(rc, bg, fg, icon.font, icon.glyph, DrawContext::CENTER);
            ctx->exclude(rc);
        }

    private:
        static void IconButton_drawCallback(int32_t x, int32_t y, uint32_t w, uint32_t color, void *param)
        {
            static_cast<DrawContext *>(param)->draw(Rect(x, y, x + w, y + 1), color);
        }

        Icon _icon;
        Icon _iconToggled;
    };
} // namespace display::ui
