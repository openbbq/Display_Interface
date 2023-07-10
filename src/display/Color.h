#pragma once

#include "Kind.h"

namespace display
{
    class Color
    {
    public:
        Color() : _color888(0) {}
        Color(uint16_t color565) : _color888(from565to888(color565)) {}
        Color(uint32_t color888) : _color888(color888) {}
        Color(int color565) : _color888(from565to888(color565)) {}

        operator uint16_t() const { return from888to565(_color888); }

        uint16_t to565() const { return from888to565(_color888); }
        uint32_t to888() const { return _color888; }

        static Color blend(float opacity, Color fg, Color bg)
        {
            uint32_t o1 = uint32_t(opacity * 256) & 0xff;
            uint32_t o2 = 256 - o1;

            uint32_t r = (((fg._color888 & 0b111111110000000000000000) >> 16) * o1 +
                          ((bg._color888 & 0b111111110000000000000000) >> 16) * o2) /
                         256;
            uint32_t g = (((fg._color888 & 0b000000001111111100000000) >> 8) * o1 +
                          ((bg._color888 & 0b000000001111111100000000) >> 8) * o2) /
                         256;
            uint32_t b = (((fg._color888 & 0b000000000000000011111111) >> 0) * o1 +
                          ((bg._color888 & 0b000000000000000011111111) >> 0) * o2) /
                         256;
            uint32_t c888 = (r << 16) + (g << 8) + b;
            return Color(c888);
        }

    private:
        static uint32_t from565to888(uint16_t value)
        {
            uint32_t r = (value & 0b1111100000000000) >> 8;
            uint32_t g = (value & 0b0000011111100000) >> 3;
            uint32_t b = (value & 0b0000000000011111) << 3;
            uint32_t c888 = (r << 16) + (g << 8) + b;
            log_v("c565:0x%04X r:%d g:%d b:%d c888:0x%08X", (uint32_t)value, r, g, b, c888);
            return c888;
        }
        static uint16_t from888to565(uint32_t value)
        {
            uint32_t r = (value & 0b111110000000000000000000) >> 16;
            uint32_t g = (value & 0b000000001111110000000000) >> 8;
            uint32_t b = (value & 0b000000000000000011111000);
            uint32_t c565 = (r << 8) + (g << 3) + (b >> 3);
            log_v("c888:0x%08X r:%d g:%d b:%d c565:0x%08X", value, r, g, b, c565);
            return c565;
        }
        uint32_t _color888;
    };
} // namespace display
