#include <Arduino.h>
#include "WindowContext.h"

namespace display
{
    WindowContext::PriorViewport WindowContextBase::enter_viewport(const Rect &viewport)
    {
        WindowContext::PriorViewport prior = {origin(), extent()};

        Rect frame = viewport.offset(prior.origin);
        origin(frame.origin());
        extent(frame.intersection(prior.extent));
        return prior;
    }

    void WindowContextBase::restore_viewport(const PriorViewport &prior)
    {
        origin(prior.origin);
        extent(prior.extent);
    }
} // namespace display
