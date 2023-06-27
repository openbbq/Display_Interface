#pragma once

#include "Style.h"
#include "Coords.h"
#include "DrawContext.h"
#include "TouchContext.h"

#include <vector>
#include <initializer_list>

namespace display
{
    class Interface;
    class Window;

    // template <typename TWindow>
    // class enable_shared_create
    // {
    // public:
    //     template <typename... _Args>
    //     static std::shared_ptr<TWindow> create(_Args &&...__args) { return std::make_shared<TWindow>(std::forward<_Args>(__args)...); }
    // };

    typedef std::shared_ptr<Window> WindowPtr;

    class Window : public std::enable_shared_from_this<Window>
    {
    public:
        virtual ~Window() = default;

        virtual Interface *interface() const = 0;
        virtual void invalidate() = 0;
        virtual void invalidate(const Rect &rc) = 0;

        virtual WindowPtr parent() const = 0;
        virtual void parent(WindowPtr value) = 0;

        void add(std::initializer_list<WindowPtr> children);

        template <typename TWindow, typename... _Args>
        std::shared_ptr<TWindow> add(_Args &&...__args)
        {
            auto ptr = std::make_shared<TWindow>(std::forward<_Args>(__args)...);
            ptr->parent(shared_from_this());
            return ptr;
        }

        virtual StylePtr style() const = 0;
        virtual void style(StylePtr value) = 0;

        virtual const Rect &position() const = 0;
        virtual void position(const Rect &value) = 0;

        virtual Rect content() const = 0;

        // false - normal visible conditions.
        // true - no display rendering, no touch effects.
        virtual bool hidden() const = 0;
        virtual void hidden(bool value) = 0;

        // false - no effect on appearance/interaction.
        // true - visually muted (greyed), touch/keyboard interactions not accepted, (focus refused?)
        virtual bool disabled() const = 0;
        virtual void disabled(bool value) = 0;

        // false - no effect on appearance/interaction.
        // true - "input" state - toggle is selected, checkbox is marked, two-state button is pressed, nav item is current
        virtual bool toggled() const = 0;
        virtual void toggled(bool value) = 0;

        // false - no effect on appearance/interaction.
        // true - (unique) visually highlight current "keyboard" input of form, or current item of menu
        virtual bool focused() const = 0;
        virtual void focused(bool value) = 0;

        // false - no effect on appearance/interaction.
        // true - extra visual feedback during a touch gesture of the widget that will be clicked on release.
        virtual bool pressed() const = 0;
        virtual void pressed(bool value) = 0;

        // true - resizeHandler must be called in next loop
        virtual bool resizing() const = 0;
        virtual void resizing(bool value) = 0;

        virtual bool touchCapture() const = 0;
        virtual void touchCapture(bool value) = 0;
        virtual void touchRelease() = 0;

        virtual const String &text() const = 0;
        virtual void text(const String &value) = 0;

        virtual void attachHandler(WindowPtr child) = 0;
        virtual void detachHandler(WindowPtr child) = 0;
        virtual void loopHandler() = 0;
        virtual void drawHandler(DrawContext *dc) = 0;
        virtual void resizeHandler() = 0;
        virtual Size measureHandler(const Size &available) const = 0;
        virtual bool touchDownHandler(TouchContext *tc) = 0;
        virtual bool touchMoveHandler(TouchContext *tc) = 0;
        virtual bool touchUpHandler(TouchContext *tc) = 0;

        const Point &origin() const { return position().origin(); }
        const Size &size() const { return position().size(); }

        Color background() const
        {
            auto s = style();
            return s ? s->background() : 0;
        }
    };

    class WindowBase : public Window
    {
    public:
        WindowBase() {}
        WindowBase(StylePtr style) : _style(style) {}

        Interface *interface() const override;
        void invalidate() override;
        void invalidate(const Rect &rc) override;

        WindowPtr parent() const override { return _parent.lock(); }
        void parent(WindowPtr value) override;

        virtual StylePtr style() const override { return _style; }
        virtual void style(StylePtr value) override
        {
            if (_style != value)
            {
                _style = value;
                invalidate();
            }
        }

        const Rect &position() const override { return _position; }
        void position(const Rect &value) override
        {
            if (_position == value)
            {
                return;
            }
            log_v("from %s to %s", _position.toString().c_str(), value.toString().c_str());
            Size prior = _position.size();
            invalidate();
            _position = value;
            invalidate();
            if (prior != _position.size())
            {
                resizing(true);
            }
        }

        Rect content() const override { return Rect(Point(0, 0), size()); }

        bool hidden() const override { return _hidden; }
        void hidden(bool value) override
        {
            if (_hidden != value)
            {
                if (!_hidden)
                {
                    invalidate();
                }
                _hidden = value;
                if (!_hidden)
                {
                    invalidate();
                }
            }
        }
        bool disabled() const override { return _disabled; }
        void disabled(bool value) override
        {
            if (_disabled != value)
            {
                _disabled = value;
                invalidate();
            }
        }
        bool toggled() const override { return _toggled; }
        void toggled(bool value) override
        {
            if (_toggled != value)
            {
                _toggled = value;
                invalidate();
            }
        }
        bool focused() const override { return _focused; }
        void focused(bool value) override { _focused = value; }
        bool pressed() const override { return _pressed; }
        void pressed(bool value) override
        {
            if (_pressed != value)
            {
                _pressed = value;
                invalidate();
            }
        }
        bool resizing() const override { return _resizing; }
        void resizing(bool value) override
        {
            if (_resizing != value)
            {
                log_v("set to %d", value);
                _resizing = value;
            }
        }

        bool touchCapture() const override;
        void touchCapture(bool value) override;
        void touchRelease() override;

        const String &text() const override
        { /*invalid*/
            return "";
        }
        void text(const String &value) override
        { /*invalid*/
        }

        void loopHandler() override
        {
            if (resizing())
            {
                resizing(false);
                resizeHandler();
            }
        }

        void drawHandler(DrawContext *dc) override {}
        void resizeHandler() override {}
        Size measureHandler(const Size &available) const override { return size(); };
        bool touchDownHandler(TouchContext *tc) override { return false; }
        bool touchMoveHandler(TouchContext *tc) override { return false; }
        bool touchUpHandler(TouchContext *tc) override { return false; }

    protected:
        void attachHandler(WindowPtr child) override
        { /*invalid*/
        }
        void detachHandler(WindowPtr child) override
        { /*invalid*/
        }

    private:
        std::weak_ptr<Window> _parent;
        Rect _position;
        StylePtr _style;

        bool _hidden : 1 = false;
        bool _disabled : 1 = false;
        bool _toggled : 1 = false;
        bool _focused : 1 = false;
        bool _pressed : 1 = false;
        bool _resizing : 1 = false;
    };

    class ParentWindow : public WindowBase
    {
    public:
        using WindowBase::WindowBase; // inherits constructors

        void loopHandler() override;
        void drawHandler(DrawContext *dc) override;
        bool touchDownHandler(TouchContext *tc) override;
        bool touchMoveHandler(TouchContext *tc) override;
        bool touchUpHandler(TouchContext *tc) override;

    protected:
        void attachHandler(WindowPtr child) override;
        void detachHandler(WindowPtr child) override;
        const std::vector<WindowPtr> &children() const { return _children; }

    private:
        std::vector<WindowPtr> _children;
    };
}
