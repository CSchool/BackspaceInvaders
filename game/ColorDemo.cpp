#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "graphics.h"
#include "binary.h"
#include "controls.h"
#include "music.h"
#include "tunes.h"

#ifdef FRAME_BUFFER

struct ColorDemoData
{
};
static ColorDemoData* data;

static void ColorDemo_prepare()
{
    game_enable_frame_buffer();
    for (int c = 0 ; c < 8 ; ++c)
        for (int r = 0 ; r < 8 ; ++r)
        {
            game_draw_rect(c * 8, r * 8, 8, 8, (r << 4) + c);
            for (int i = 0 ; i < 4 ; ++i)
                if (r & (1 << i))
                    game_draw_pixel(c * 8 + i, r * 8, 0);
            for (int i = 0 ; i < 4 ; ++i)
                if (c & (1 << i))
                    game_draw_pixel(c * 8 + i, r * 8 + 1, 0);
        }
}

static void ColorDemo_render()
{
}

static void ColorDemo_update(unsigned long delta)
{
}

const game_instance ColorDemo PROGMEM = {
    "ColorDemo",
    ColorDemo_prepare,
    ColorDemo_render,
    ColorDemo_update,
    sizeof(ColorDemoData),
    (void**)(&data)
};

#endif
