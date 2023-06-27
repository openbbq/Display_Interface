#pragma once

namespace display::ui
{
    class Solid : public WindowBase
    {
    public:
        using WindowBase::WindowBase; // inherits constructors

        Solid(StylePtr style, const Rect& pos) : WindowBase(style) { position(pos); }

        void drawHandler(DrawContext *dc) override
        {
            WindowBase::drawHandler(dc);

            Rect rc = content();
            dc->draw(rc, background());
            dc->exclude(rc);
        }
    };
}
