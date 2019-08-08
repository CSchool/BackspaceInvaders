#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "graphics.h"
#include "binary.h"
#include "controls.h"
#include "music.h"
#include "tunes.h"

//#ifdef FRAME_BUFFER

enum
{
    LEFT,
    UP,
    RIGHT,
    DOWN,
};

#define CCW(d) (((d) + 4 - 1) % 4)
#define CW(d) (((d) + 1) % 4)

#define MW 32
#define MH 32

const uint8_t maze[] PROGMEM = {
    B11111111, B11111111, B11111111, B11111111,
    B11100000, B01101100, B11101110, B11000001,
    B10111110, B00111000, B11000110, B11111111,
    B11100000, B01101100, B11101110, B11000001,
    B11000000, B11000110, B11111110, B11000001,
    B11001110, B11000110, B11111110, B11111101,
    B11000110, B11111110, B11010110, B11000001,
    B11000110, B11111110, B11010110, B11000001,
    B11100110, B11000110, B11000110, B11000001,
    B10111110, B11000110, B11000110, B11111111,
    B10000000, B00000000, B00000000, B00000001,
    B11111100, B11000110, B11111110, B11111101,
    B11000110, B11000110, B11000000, B11000111,
    B11000110, B11000110, B11000000, B11000111,
    B11000110, B11000110, B11111100, B11001111,
    B11000110, B01101100, B11000000, B11111001,
    B11000110, B00111000, B11000000, B11011101,
    B11000000, B11000110, B11111110, B11000001,
    B11001110, B11000110, B11111110, B11111101,
    B11000110, B11111110, B11010110, B11000001,
    B11001110, B11000110, B11111110, B11111101,
    B11000110, B11111110, B11010110, B11000001,
    B11100110, B11000110, B11000110, B11000001,
    B10111110, B11000110, B11000110, B11111111,
    B10000000, B00000000, B00000000, B00000001,
    B11111100, B11000110, B11111110, B11111101,
    B11000110, B11000110, B11000000, B11000111,
    B11000110, B11000110, B11000000, B11000111,
    B11000110, B11000110, B11111100, B11001111,
    B11000110, B01101100, B11000000, B11111001,
    B11000110, B00111000, B11000000, B11011101,
    B11111111, B11111111, B11111111, B11111111,
};

static const uint8_t wall0Lines[] PROGMEM = {
    B10000000, B00000000, 
    B11000000, B00000000, 
    B11000000, B00000000, 
    B11010000, B00000000, 
    B11011000, B00000000, 
    B11011100, B00000000, 
    B11011110, B00000000, 
    B11011111, B00000000, 
    B11011111, B10000000, 
    B00011111, B10000000, 
    B00011111, B10000000, 
    B11001111, B10000000, 
    B11100011, B10000000, 
    B11111001, B10000000, 
    B11111100, B00000000, 
    B11111101, B00000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B00011101, B10000000, 
    B00000001, B10000000, 
    B11000000, B00000000, 
    B11011100, B00000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11010000, B00000000, 
    B00000000, B00000000, 
    B00001101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B10000000, 
    B11111100, B00000000, 
    B11111000, B00000000, 
    B11000001, B10000000, 
    B00000111, B10000000, 
    B00011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B10000000, 
    B11011111, B00000000, 
    B11011100, B00000000, 
    B11011000, B10000000, 
    B11010001, B10000000, 
    B11000101, B10000000, 
    B10001101, B10000000, 
    B00111101, B10000000, 
    B01111101, B10000000, 
    B11111101, B10000000, 
    B11111101, B00000000, 
    B11111100, B00000000, 
    B11111100, B00000000, 
    B11111000, B00000000, 
    B11110000, B00000000, 
    B11100000, B00000000, 
    B11000000, B00000000, 
    B10000000, B00000000, 
};
static const game_sprite wall0sprite PROGMEM = {9, 64, wall0Lines};


struct Maze3DData
{
    int8_t x, y, dir;
};
static Maze3DData* data;

static bool IsWall(uint8_t x, uint8_t y)
{
    uint16_t off = x / 8 + y * MW / 8;
    uint8_t walls = pgm_read_byte(maze + off);
    return walls & (1 << (7 - x % 8));
}

static int8_t GetDx(uint8_t dir)
{
    return dir == LEFT ? -1 : dir == RIGHT ? 1 : 0;
}

static int8_t GetDy(uint8_t dir)
{
    return dir == UP ? -1 : dir == DOWN ? 1 : 0;
}

static void Maze3D_draw()
{
    game_clear_screen();
    int8_t dx = GetDx(data->dir);
    int8_t dy = GetDy(data->dir);
    int8_t left = CCW(data->dir);
    int8_t right = CW(data->dir);
    int8_t dxl = GetDx(left);
    int8_t dyl = GetDy(left);
    int8_t dxr = GetDx(right);
    int8_t dyr = GetDy(right);
    if (IsWall(data->x + dx, data->y + dy))
        game_draw_rect(2, 2, 60, 60, RED_DARK);
    else
    {
        if (IsWall(data->x + 2 * dx, data->y + 2 * dy))
            game_draw_rect(24, 24, 16, 16, RED_DARK);
        else if (IsWall(data->x + 3 * dx, data->y + 3 * dy))
            game_draw_rect(28, 28, 8, 8, RED_DARK);
        if (IsWall(data->x + dxl, data->y + dyl))
            game_draw_sprite(&wall0sprite, 0, 0, RED_DARK);
        if (IsWall(data->x + dxr, data->y + dyr))
            game_draw_sprite(&wall0sprite, 55, 0, RED_DARK, SPRITE_MIRROR_H);
    }
}

static void Maze3D_prepare()
{
    game_enable_frame_buffer();
    game_set_background(BLACK);
    game_set_ups(10);
    data->y = 2;
    data->x = 7;
    data->dir = DOWN;

    Maze3D_draw();
}

static void Maze3D_render()
{
}

static void Maze3D_update(unsigned long delta)
{
    bool upd = false;
 
    if (game_is_button_pressed(BUTTON_UP))
    {
        int8_t x = data->x + GetDx(data->dir);
        int8_t y = data->y + GetDy(data->dir);
        if (!IsWall(x, y))
        {
            upd = true;
            data->x = x;
            data->y = y;
        }
    }
    else if (game_is_button_pressed(BUTTON_LEFT))
    {
        upd = true;
        data->dir = CCW(data->dir);
    }
    else if (game_is_button_pressed(BUTTON_RIGHT))
    {
        upd = true;
        data->dir = CW(data->dir);
    }

    if (upd)
    {
        Maze3D_draw();
    }
}

const game_instance Maze3D PROGMEM = {
    "Maze3D",
    Maze3D_prepare,
    Maze3D_render,
    Maze3D_update,
    sizeof(Maze3DData),
    (void**)(&data)
};

//#endif
