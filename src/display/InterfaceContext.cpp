#include <Arduino.h>
#include "InterfaceContext.h"

namespace display
{
    InterfaceContext::PriorViewport InterfaceContextBase::enterViewport(const Rect &viewport)
    {
        InterfaceContext::PriorViewport prior = {origin(), extent()};

        Rect frame = viewport.offset(prior.origin);
        origin(frame.origin());
        extent(frame.intersection(prior.extent));
        return prior;
    }

    void InterfaceContextBase::restoreViewport(const PriorViewport &prior)
    {
        origin(prior.origin);
        extent(prior.extent);
    }
} // namespace display
