#include <Arduino.h>
#include "Interface.h"
#include "TouchContext.h"

namespace display
{
    void BaseInterface::screen(WindowPtr value)
    {
        _screen = value;
        screen()->position(Rect(size()));
    }

    void BaseInterface::add(std::shared_ptr<LoopHandler> device) 
    {
        _devices.push_back(device);
    }

    void BaseInterface::loop()
    {
        log_v("---");
        for (auto &device : _devices)
        {
            device->loopHandler();
        }
        screen()->loopHandler();
        draw();
    }

    void BaseInterface::invalidate(const Rect &rc)
    {
        _invalid.include(rc);
    }

    void BaseInterface::invalidate(WindowPtr window)
    {
        InterfaceContextBase ctx;
        ctx.extent(window->size());
        apply_viewport(&ctx, window);
        _invalid.include(ctx.extent());
    }

    void BaseInterface::invalidate(WindowPtr window, const Rect &rc)
    {
        InterfaceContextBase ctx;
        ctx.extent(Rect(window->size()).intersection(rc));
        apply_viewport(&ctx, window);
        _invalid.include(ctx.extent());
    }

    void BaseInterface::draw()
    {
        auto dc = createDrawContext();
        dc->initialize(size(), _invalid);
        _invalid.clear();
        screen()->drawHandler(dc.get());
    }

    WindowPtr BaseInterface::touchCapture() const
    {
        return _touchCapture;
    }

    void BaseInterface::touchCapture(WindowPtr window)
    {
        _touchCapture = window;
    }

    void BaseInterface::touchDown(const Point &current)
    {
        _touchInitial = current;

        WindowPtr window = _touchCapture ? _touchCapture : _screen;
        TouchContext ctx(current, _touchInitial);
        ctx.extent(window->size());
        apply_viewport(&ctx, window);
        window->touchDownHandler(&ctx);
    }

    void BaseInterface::touchMove(const Point &current)
    {
        WindowPtr window = _touchCapture ? _touchCapture : _screen;
        TouchContext ctx(current, _touchInitial);
        ctx.extent(window->size());
        apply_viewport(&ctx, window);
        window->touchMoveHandler(&ctx);
    }

    void BaseInterface::touchUp(const Point &current)
    {
        WindowPtr window = _touchCapture ? _touchCapture : _screen;
        TouchContext ctx(current, _touchInitial);
        ctx.extent(window->size());
        apply_viewport(&ctx, window);
        window->touchUpHandler(&ctx);
        touchCapture(nullptr);
    }

    void BaseInterface::apply_viewport(InterfaceContext *context, WindowPtr window)
    {
        WindowPtr walk = window;
        for (WindowPtr parent = walk->parent(); parent; walk = parent, parent = walk->parent())
        {
            // origin shift's based on position relative to parent
            context->origin(context->origin() + walk->origin());

            // extent is shifted by the same amount, and clipped by what's visible in that
            // parent's nominal area.
            context->extent(context->extent().offset(walk->origin()).intersection(parent->size()));
        }
    }

} // namespace display
