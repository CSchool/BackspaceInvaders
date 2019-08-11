#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "config.h"

// COLORS
//
// are defined as 6-bit integer: xBGRxbgr

#define BLACK 0
#define BLUE 0x44
#define GREEN 0x22
#define RED 0x11
#define CYAN (BLUE + GREEN)
#define PURPLE (RED + BLUE)
#define YELLOW (RED + GREEN)
#define WHITE (RED + GREEN + BLUE)
/* for 6-bit colored games */
/* brigthness of the msb is higher */
#define RED_DARK 1
#define RED_MIDDLE 0x10
#define GREEN_DARK 2
#define GREEN_MIDDLE 0x20
#define BLUE_DARK 4
#define BLUE_MIDDLE 0x40
#define YELLOW_DARK 3
#define YELLOW_MIDDLE 0x30
#define WHITE_DARK 0x7
#define WHITE_MIDDLE 0x70

#define ORANGE 0x31

#define TRANSPARENT 0xff

// SPRITE

#define SPRITE_NORMAL   0
#define SPRITE_MIRROR_H 1
// #define SPRITE_MIRROR_V 2
// #define SPRITE_ROT_90   3
// #define SPRITE_ROT_180  4
// #define SPRITE_ROT_270  5

struct game_sprite
{
    uint8_t width;
    uint8_t height;
    const uint8_t *lines;
};

#define game_color_sprite game_sprite

// Setup functions

void game_enable_frame_buffer();

// Sprite functions

uint8_t game_sprite_width(const struct game_sprite *s);
uint8_t game_sprite_height(const struct game_sprite *s);

// DRAWING FUNCTIONS

void game_draw_pixel(int8_t x, int8_t y, uint8_t color);
uint8_t game_get_pixel(int8_t x, int8_t y);
void game_draw_vline(int8_t x, int8_t y1, int8_t y2, uint8_t color); // vertical line
void game_draw_rect(int8_t x, int8_t y, int8_t w, int8_t h, uint8_t color);
void game_draw_text(const uint8_t *s, int8_t x, int8_t y, uint8_t color, uint8_t bg = TRANSPARENT); // text is drawn with default 5x7 font
void game_draw_char(uint8_t c, int8_t x, int8_t y, uint8_t color, uint8_t bg = TRANSPARENT); // char is drawn with default 5x7 font
void game_draw_sprite(const struct game_sprite *s, int8_t x, int8_t y,
                      uint8_t color, uint8_t rot = SPRITE_NORMAL);
void game_draw_digits(uint16_t num, int8_t len, int8_t x, int8_t y, uint8_t color, uint8_t bg = TRANSPARENT);
void game_draw_color_sprite(const struct game_color_sprite *s, int8_t x, int8_t y);
/* Clear screen in frame buffer mode */
void game_clear_screen();
void game_set_background(uint8_t color);

bool game_is_drawing_lines(int8_t y, int8_t height);

// Internal functions
// Called only by libgame

void graphics_setup();

#endif

