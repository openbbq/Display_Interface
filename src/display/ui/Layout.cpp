#include <Arduino.h>
#include "Layout.h"

namespace display::ui
{
    void Layout::resizeHandler()
    {
        Rect rc = content();
        log_d("content:%s", rc.toString().c_str());

        for (const Entry &entry : _entries)
        {
            uint16_t width = rc.width();
            uint16_t height = rc.height();
            if (entry.size == -1)
            {
                Size preferred = entry.window->measureHandler(rc.size());
                width = min(width, preferred.width());
                height = min(height, preferred.height());
            }
            else
            {
                width = min(width, (uint16_t)entry.size);
                height = min(height, (uint16_t)entry.size);
            }

            switch (entry.kind)
            {
            case 1:
                entry.window->position(Rect(rc.left(), rc.top(), rc.left() + width, rc.bottom()));
                rc = Rect(rc.left() + width, rc.top(), rc.right(), rc.bottom());
                break;
            case 2:
                entry.window->position(Rect(rc.left(), rc.top(), rc.right(), rc.top() + height));
                rc = Rect(rc.left(), rc.top() + height, rc.right(), rc.bottom());
                break;
            case 3:
                entry.window->position(Rect(rc.right() - width, rc.top(), rc.right(), rc.bottom()));
                rc = Rect(rc.left(), rc.top(), rc.right() - width, rc.bottom());
                break;
            case 4:
                entry.window->position(Rect(rc.left(), rc.bottom() - height, rc.right(), rc.bottom()));
                rc = Rect(rc.left(), rc.top(), rc.right(), rc.bottom() - height);
                break;
            case 5:
                entry.window->position(rc);
                rc = Rect(rc.left(), rc.bottom(), rc.right(), rc.bottom());
                break;
            }

            log_d("measured:{%d,%d} positioned:%s remaining:%s",
                  width, height,
                  entry.window->position().toString().c_str(),
                  rc.toString().c_str());
        }
    }
}