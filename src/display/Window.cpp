
#include <Arduino.h>
#include "Window.h"
#include "Interface.h"

namespace display
{
    void Window::add(std::initializer_list<WindowPtr> children)
    {
        for (auto &w : children)
        {
            w->parent(shared_from_this());
        }
    }

    Interface *WindowBase::interface() const
    {
        WindowPtr ptr = parent();
        return ptr ? ptr->interface() : nullptr;
    }

    void WindowBase::invalidate()
    {
        invalidate(size());
    }

    void WindowBase::invalidate(const Rect &rc)
    {
        if (hidden())
        {
            return;
        }

        Interface *ptr = interface();
        if (!ptr)
        {
            return;
        }

        ptr->invalidate(shared_from_this(), rc);
    }

    void WindowBase::parent(WindowPtr value)
    {
        auto prior = _parent.lock();
        if (prior == value)
        {
            return;
        }
        if (value)
        {
            value->attachHandler(shared_from_this());
        }
        if (prior)
        {
            prior->detachHandler(shared_from_this());
        }

        invalidate();
        _parent = value;
        invalidate();
    }

    bool WindowBase::touchCapture() const
    {
        return interface()->touchCapture().get() == this;
    }

    void WindowBase::touchCapture(bool value)
    {
        if (value)
        {
            interface()->touchCapture(shared_from_this());
        }
        else
        {
            touchRelease();
        }
    }

    void WindowBase::touchRelease()
    {
        if (touchCapture())
        {
            interface()->touchCapture(nullptr);
        }
    }

    bool WindowBase::clickHandler(const WindowPtr &clicked)
    {
        WindowPtr ptr = parent();
        if (ptr)
        {
            return ptr->clickHandler(clicked);
        }
        return false;
    }

    void ParentWindow::attachHandler(WindowPtr child)
    {
        resizing(true);
        _children.push_back(std::move(child));
    }

    void ParentWindow::detachHandler(WindowPtr child)
    {
        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            if (child == *it)
            {
                resizing(true);
                _children.erase(it);
                return;
            }
        }
        /* invalid if reached here */
    }

    void ParentWindow::loopHandler()
    {
        WindowBase::loopHandler();

        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            (*it)->loopHandler();
        }
    }

    void ParentWindow::drawHandler(DrawContext *dc)
    {
        WindowBase::drawHandler(dc);

        for (auto it = _children.rbegin(); it != _children.rend(); ++it)
        {
            auto w = it->get();
            auto prior = dc->enterViewport(w->position());
            w->drawHandler(dc);
            dc->restoreViewport(prior);
        }
    }

    bool ParentWindow::touchDownHandler(TouchContext *tc)
    {
        for (auto it = _children.rbegin(); it != _children.rend(); ++it)
        {
            auto w = it->get();
            auto prior = tc->enterViewport(w->position());
            bool handled = w->touchDownHandler(tc);
            tc->restoreViewport(prior);
            if (handled)
            {
                return true;
            }
        }
        return WindowBase::touchDownHandler(tc);
    }

    bool ParentWindow::touchMoveHandler(TouchContext *tc)
    {
        for (auto it = _children.rbegin(); it != _children.rend(); ++it)
        {
            auto w = it->get();
            auto prior = tc->enterViewport(w->position());
            bool handled = w->touchMoveHandler(tc);
            tc->restoreViewport(prior);
            if (handled)
            {
                return true;
            }
        }
        return WindowBase::touchMoveHandler(tc);
    }

    bool ParentWindow::touchUpHandler(TouchContext *tc)
    {
        for (auto it = _children.rbegin(); it != _children.rend(); ++it)
        {
            auto w = it->get();
            auto prior = tc->enterViewport(w->position());
            bool handled = w->touchUpHandler(tc);
            tc->restoreViewport(prior);
            if (handled)
            {
                return true;
            }
        }
        return WindowBase::touchUpHandler(tc);
    }
}
