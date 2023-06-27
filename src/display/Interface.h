#pragma once

#include "Touch.h"
#include "DrawContext.h"
#include "Window.h"
#include "Region.h"

namespace display
{
    class Interface : public Touch
    {
    public:
        virtual ~Interface() = default;

        template <typename TScreen, typename... _Args>
        std::shared_ptr<TScreen> begin(_Args &&...__args)
        {
            std::shared_ptr<TScreen> ptr = std::make_shared<TScreen>(this, std::forward<_Args>(__args)...);
            screen(ptr);
            return ptr;
        }

        virtual void loop() = 0;

        virtual WindowPtr screen() const = 0;
        virtual void screen(WindowPtr value) = 0;

        virtual Size size() const = 0;
        virtual Rect measure(const FontPtr &font, const String &text) const = 0;

        virtual void invalidate(const Rect &rc) = 0;
        virtual void invalidate(WindowPtr window) = 0;
        virtual void invalidate(WindowPtr window, const Rect &rc) = 0;
        virtual Region invalid() const = 0;

        virtual void draw() = 0;

        virtual WindowPtr touchCapture() const = 0;
        virtual void touchCapture(WindowPtr window) = 0;
        void touchRelease() { touchCapture(nullptr); }

        virtual std::unique_ptr<DrawContext> createDrawContext() = 0;
    };

    class BaseInterface : public Interface
    {
    public:
        WindowPtr screen() const override { return _screen; }
        void screen(WindowPtr value) override;

        void loop() override;

        void invalidate(const Rect &rc) override;
        void invalidate(WindowPtr window) override;
        void invalidate(WindowPtr window, const Rect &rc) override;
        Region invalid() const override { return _invalid; }

        void draw() override;

        WindowPtr touchCapture() const override;
        void touchCapture(WindowPtr window) override;

        void touchDown(const Point &value) override;
        void touchMove(const Point &value) override;
        void touchUp(const Point &value) override;

    private:
        void apply_viewport(WindowContext *context, WindowPtr window);

        Region _invalid;
        WindowPtr _screen;

        WindowPtr _touchCapture;
        Point _touchInitial;
    };
}
