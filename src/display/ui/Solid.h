#pragma once

#include "Solid.h"

namespace display::ui
{
    class Solid : public BaseWindow
    {
    public:
        using BaseWindow::BaseWindow; // inherits constructors

        Solid(StylePtr style, const Rect& pos) : BaseWindow(style) { position(pos); }

        void drawHandler(DrawContext *dc) override
        {
            BaseWindow::drawHandler(dc);

            Rect rc = content();
            dc->draw(rc, background());
            dc->exclude(rc);
        }
    };
}
