#pragma once

#include "Layout.h"

namespace display::ui
{
    class Screen : public Layout
    {
    public:
        Screen(Interface *interface, StylePtr style) : Layout(style), _interface(interface) {}

        Interface *interface() const override { return _interface; }

        void drawHandler(DrawContext *dc) override
        {
            Layout::drawHandler(dc);

            Rect rc = content();
            dc->draw(rc, background());
            dc->exclude(rc);
        }

    private:
        Interface *_interface;
    };
}
