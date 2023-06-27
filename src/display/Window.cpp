
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

    Interface *BaseWindow::interface() const
    {
        WindowPtr ptr = parent();
        return ptr ? ptr->interface() : nullptr;
    }

    void BaseWindow::invalidate()
    {
        Interface *ptr = interface();
        if (ptr)
        {
            ptr->invalidate(shared_from_this());
        }
    }
    
    void BaseWindow::invalidate(const Rect &rc)
    {
        Interface *ptr = interface();
        if (ptr)
        {
            ptr->invalidate(shared_from_this(), rc);
        }
    }

    void BaseWindow::parent(WindowPtr value)
    {
        auto prior = _parent.lock();
        if (prior == value)
        {
            return;
        }
        if (value)
        {
            value->attach(shared_from_this());
        }
        if (prior)
        {
            prior->detach(shared_from_this());
        }

        invalidate();
        _parent = value;
        invalidate();
    }

    bool BaseWindow::touchCapture() const
    {
        return interface()->touchCapture().get() == this;
    }

    void BaseWindow::touchCapture(bool value)
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

    void BaseWindow::touchRelease()
    {
        if (touchCapture())
        {
            interface()->touchCapture(nullptr);
        }
    }

    void ParentWindow::attach(WindowPtr child)
    {
        adjusting(true);
        _children.push_back(std::move(child));
    }

    void ParentWindow::detach(WindowPtr child)
    {
        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            if (child == *it)
            {
                adjusting(true);
                _children.erase(it);
                return;
            }
        }
        /* invalid if reached here */
    }

    void ParentWindow::loopHandler()
    {
        BaseWindow::loopHandler();

        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            (*it)->loopHandler();
        }
    }

    void ParentWindow::drawHandler(DrawContext *dc)
    {
        BaseWindow::drawHandler(dc);

        for (auto it = _children.rbegin(); it != _children.rend(); ++it)
        {
            auto w = it->get();
            auto prior = dc->enter_viewport(w->position());
            w->drawHandler(dc);
            dc->restore_viewport(prior);
        }
    }

    bool ParentWindow::touchDownHandler(TouchContext *tc)
    {
        for (auto it = _children.rbegin(); it != _children.rend(); ++it)
        {
            auto w = it->get();
            auto prior = tc->enter_viewport(w->position());
            bool handled = w->touchDownHandler(tc);
            tc->restore_viewport(prior);
            if (handled)
            {
                return true;
            }
        }
        return BaseWindow::touchDownHandler(tc);
    }

    bool ParentWindow::touchMoveHandler(TouchContext *tc)
    {
        for (auto it = _children.rbegin(); it != _children.rend(); ++it)
        {
            auto w = it->get();
            auto prior = tc->enter_viewport(w->position());
            bool handled = w->touchMoveHandler(tc);
            tc->restore_viewport(prior);
            if (handled)
            {
                return true;
            }
        }
        return BaseWindow::touchMoveHandler(tc);
    }

    bool ParentWindow::touchUpHandler(TouchContext *tc)
    {
        for (auto it = _children.rbegin(); it != _children.rend(); ++it)
        {
            auto w = it->get();
            auto prior = tc->enter_viewport(w->position());
            bool handled = w->touchUpHandler(tc);
            tc->restore_viewport(prior);
            if (handled)
            {
                return true;
            }
        }
        return BaseWindow::touchUpHandler(tc);
    }
}
