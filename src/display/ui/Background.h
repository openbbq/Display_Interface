#pragma once

#include "Layout.h"

namespace display::ui
{
    class Background : public Layout
    {
    public:
        using Layout::Layout; // inherits constructors

        template <typename... _Args>
        static std::shared_ptr<Background> create(_Args &&...__args) { return std::make_shared<Background>(std::forward<_Args>(__args)...); }

        void drawHandler(DrawContext *dc) override
        {
            Layout::drawHandler(dc);

            Rect rc = content();
            dc->draw(rc, background());
            dc->exclude(rc);
        }
    };
}
