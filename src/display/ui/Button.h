
#include "../Window.h"

namespace display::ui
{
    class Button : public BaseWindow
    {
    public:
        Button() {}
        Button(StylePtr style, const String &text = "") : BaseWindow(style), _text(text) {}
        Button(StylePtr style, StylePtr pressedStyle, const String &text = "") : BaseWindow(style), _pressedStyle(pressedStyle), _text(text) {}
        Button(StylePtr style, StylePtr pressedStyle, StylePtr toggledStyle, const String &text = "") : BaseWindow(style), _pressedStyle(pressedStyle), _toggledStyle(toggledStyle), _text(text) {}

        template <typename... _Args>
        static std::shared_ptr<Button> create(_Args &&...__args) { return std::make_shared<Button>(std::forward<_Args>(__args)...); }

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

        bool touchDownHandler(TouchContext *tc) override
        {
            if (tc->extent().contains(tc->current()))
            {
                interface()->touchCapture(shared_from_this());
                return true;
            }
            return false;
        }

        bool touchMoveHandler(TouchContext *tc) override
        {
            if (interface()->touchCapture().get() != this)
            {
                Serial.println("Button::touchMoveHandler but touchCapture() != this");
                return false;
            }

            bool hit = tc->extent().contains(tc->current());
            Serial.printf("Button::touchMoveHandler hit %s", hit ? "true" : "false");
            pressed(hit);
            return true;
        }

        bool touchUpHandler(TouchContext *tc) override
        {
            if (interface()->touchCapture().get() != this)
            {
                return false;
            }
            if (pressed())
            {
                pressed(false);
                toggled(!toggled());
            }
            interface()->touchCapture(nullptr);
            return true;
        }

    private:
        StylePtr _pressedStyle;
        StylePtr _toggledStyle;
        String _text;
    };
}
