#pragma once

#include "../Window.h"

namespace display::ui
{
    class Button : public WindowBase
    {
    public:
        Button() {}
        Button(StylePtr style, const String &text = "") : WindowBase(style), _text(text) {}
        Button(StylePtr style, StylePtr pressedStyle, const String &text = "") : WindowBase(style), _pressedStyle(pressedStyle), _text(text) {}
        Button(StylePtr style, StylePtr pressedStyle, StylePtr toggledStyle, const String &text = "") : WindowBase(style), _pressedStyle(pressedStyle), _toggledStyle(toggledStyle), _text(text) {}

        template <typename... _Args>
        static std::shared_ptr<Button> create(_Args &&...__args) { return std::make_shared<Button>(std::forward<_Args>(__args)...); }

        int id() const override { return _id; }
        void id(int value) override { _id = value; }

        const String &text() const override { return _text; }
        void text(const String &value) override
        {
            if (_text != value)
            {
                _text = value;
                invalidate();
            }
        }

        virtual StylePtr pressedStyle() const { return _pressedStyle; }
        virtual StylePtr toggledStyle() const { return _toggledStyle; }
        virtual StylePtr activeStyle() const
        {
            return (pressed() && pressedStyle())
                       ? pressedStyle()
                   : (toggled() && toggledStyle())
                       ? toggledStyle()
                       : style();
        }

        void drawHandler(DrawContext *dc) override
        {
            Rect rc = content();
            dc->draw(rc, activeStyle(), _text, DrawContext::CENTER);
            dc->exclude(rc);
        }

        bool touchDownHandler(TouchContext *ctx) override
        {
            if (ctx->containsCurrent())
            {
                touchCapture(true);
                return true;
            }
            return false;
        }

        bool touchMoveHandler(TouchContext *ctx) override
        {
            if (!touchCapture())
            {
                return false;
            }

            bool hit = ctx->containsCurrent();
            log_v("containsCurrent %d", hit);
            pressed(hit);
            return true;
        }

        bool touchUpHandler(TouchContext *ctx) override
        {
            if (!touchCapture())
            {
                return false;
            }

            touchCapture(false);
            if (pressed())
            {
                pressed(false);
                if (!clickHandler(shared_from_this()))
                {
                    // click not handled, default to changing toggle state.
                    toggled(!toggled());
                }
            }

            return true;
        }

    private:
        StylePtr _pressedStyle;
        StylePtr _toggledStyle;
        int _id;
        String _text;
    };

}
