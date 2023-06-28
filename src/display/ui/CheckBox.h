
#include "Button.h"
#include <tuple>

namespace display::ui
{
    class CheckBox : public Button
    {
    public:
        using Button::Button; // inherit constructors

        void drawHandler(DrawContext *ctx) override
        {
            auto s0 = style();
            auto s1 = toggledStyle();
            auto s2 = pressedStyle();

            Color fg = toggled() ? s1->foreground() : s0->foreground();
            Color bg = pressed() ? s2->background() : s0->background();
            Color mg = blendPixelColor(8, fg, bg);

            Rect all = content();
            int16_t target = _min(all.width(), all.height());
            int16_t state = target * 40 / 48;
            int16_t check = target * 18 / 48;

            Rect rcCheck = Size(check, check);
            rcCheck = rcCheck.offset((all.size() - rcCheck.size()) / 2);
            Rect rcState = rcCheck.indent((check - state) / 2);
            if (toggled())
            {
                Rect edge, remaining;

                std::tie(edge, remaining) = rcCheck.topSplit(1);
                drawSides(ctx, edge, 1, mg);
                ctx->draw(edge, fg);

                std::tie(edge, remaining) = remaining.bottomSplit(1);
                drawSides(ctx, edge, 1, mg);
                ctx->draw(edge, fg);

                ctx->draw(remaining, fg);
            }
            else
            {
                Rect edge, remaining;

                std::tie(edge, remaining) = rcCheck.topSplit(1);
                drawSides(ctx, edge, 1, mg);
                ctx->draw(edge, fg);
                std::tie(edge, remaining) = remaining.topSplit(1);
                ctx->draw(edge, fg);

                std::tie(edge, remaining) = remaining.bottomSplit(1);
                drawSides(ctx, edge, 1, mg);
                ctx->draw(edge, fg);
                std::tie(edge, remaining) = remaining.bottomSplit(1);
                ctx->draw(edge, fg);

                drawSides(ctx, remaining, 2, fg);
                ctx->draw(remaining, bg);
            }

            ctx->exclude(rcCheck);
            ctx->draw(all, s0->background());
            ctx->draw(RoundRect(rcState, state / 2), bg);
            ctx->exclude(all);
        }

    protected:
        void drawSides(DrawContext *ctx, Rect &rc, uint16_t width, Color color)
        {
            Rect side;
            std::tie(side, rc) = rc.leftSplit(width);
            ctx->draw(side, color);
            std::tie(side, rc) = rc.rightSplit(width);
            ctx->draw(side, color);
        }

        // static const uint8_t PROGMEM toneToShade[] = {
        //     0, 37, 60, 81, 99, 116, 133, 148, 163, 177, 191, 204, 217, 230, 243, 255};

        // from https://github.com/sillycowvalley/TinyTextTFT/blob/9b6f2a30866fe5b63de56ce43f6bc06b0ea5a825/Library/TinyTextTFT.cpp#L522C1-L558C2
        uint16_t blendPixelColor(uint8_t tone, uint16_t foreColor, uint16_t backColor)
        {
            // tone: 0..15
            // const uint8_t* taddr = (const uint8_t*)toneToShade;
            // uint8_t shade = pgm_read_word(taddr + tone); // not a linear conversion
            uint8_t shade = 163;
            uint8_t shade255 = 255 - 163;
            // shade: 0..255

            uint8_t rForeground = (foreColor >> 8);
            uint8_t gForeground = ((foreColor >> 4) & (0x0F));
            uint8_t bForeground = (foreColor & 0x0F);

            uint8_t rBackground = (backColor >> 8);
            uint8_t gBackground = ((backColor >> 4) & (0x0F));
            uint8_t bBackground = (backColor & 0x0F);

            uint16_t rBlended = ((shade255 * rForeground) + (shade * rBackground)) >> 8;
            uint16_t gBlended = ((shade255 * gForeground) + (shade * gBackground)) >> 8;
            uint16_t bBlended = ((shade255 * bForeground) + (shade * bBackground)) >> 8;

            uint16_t c565 = (uint16_t)((rBlended << 12) + (gBlended << 7) + (bBlended << 1));
            if (rBlended & 0x01 != 0)
            {
                c565 |= 0x0800;
            }
            if (gBlended & 0x01 != 0)
            {
                c565 |= 0x0060;
            }
            if (bBlended & 0x01 != 0)
            {
                c565 |= 0x0001;
            }
            return c565;
        }
    };
}