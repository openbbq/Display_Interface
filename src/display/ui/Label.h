#pragma once

#include "../Window.h"

namespace display::ui
{
    class Label : public WindowBase
    {
    public:
        Label() {}
        Label(StylePtr style, const String &text = "", DrawContext::Alignment alignment = DrawContext::TOP_LEFT) : WindowBase(style), _text(text), _alignment(alignment) {}

        template <typename... _Args>
        static std::shared_ptr<Label> create(_Args &&...__args) { return std::make_shared<Label>(std::forward<_Args>(__args)...); }

        const String &text() const override { return _text; }
        void text(const String &value) override
        {
            if (_text != value)
            {
                _text = value;
                invalidate();
            }
        }

        DrawContext::Alignment alignment() const { return _alignment; }
        void alignment(DrawContext::Alignment value)
        {
            if (_alignment != value)
            {
                _alignment = value;
                invalidate();
            }
        }

        void alignment(int value) { alignment(DrawContext::Alignment(value)); }

        void drawHandler(DrawContext *dc) override
        {
            Rect rc = content();
            dc->draw(rc, style(), _text, _alignment);
            dc->exclude(rc);
        }

        Size measureHandler(const Size &available) const override
        {
            return interface()->measure(style()->font(), _text).size();
        };

    private:
        String _text;
        DrawContext::Alignment _alignment;
    };
}
