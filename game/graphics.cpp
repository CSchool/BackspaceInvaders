#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "graphics.h"
#include "font.h"
#include "font_data.h"

///////////////////
// Configuration
///////////////////

#ifdef GFX_E
#define ADDR_LOW    0x1f
#define ADDR_HIGH   0xe0
#define ADDR_SHIFT  1
#define BUF_LINES   2
#else
#define ADDR_LOW    0xf
#define ADDR_HIGH   0xf0
#define ADDR_SHIFT  2
#define BUF_LINES   4
#endif

static volatile uint8_t
    *latport, *oeport, *addraport, *addrbport, *addrcport, *addrdport
#ifdef GFX_E
    , addreport
#endif
;
static uint8_t
    sclkpin, latpin, oepin, addrapin, addrbpin, addrcpin, addrdpin
#ifdef GFX_E
    , addrepin
#endif
;

// RENDER VARS
static int8_t game_render_y;
static uint8_t *game_render_buf;
#if defined(COLOR_6BIT) && COLOR_6BIT
static uint8_t color_channel;
#endif
static uint8_t step;

void render(); // render sprites

#ifdef FRAME_BUFFER
static uint8_t frame[HEIGHT][WIDTH];
static bool use_frame_buffer;
#endif
static uint8_t background;

//////////////////////////
// Colors
//////////////////////////

static uint8_t game_make_color(uint8_t color)
{
    return color;
}

static uint8_t game_make_color_id(uint8_t color)
{
    return color;
}

uint8_t game_sprite_width(const struct game_sprite *s)
{
    return pgm_read_byte(&s->width);
}

uint8_t game_sprite_height(const struct game_sprite *s)
{
    return pgm_read_byte(&s->height);
}

const uint8_t *game_sprite_line(const struct game_sprite *s, uint8_t line)
{
    return (pgm_read_byte(&s->width) + 7) / 8 * line
        + (const uint8_t*)pgm_read_pointer(&s->lines);
}

const uint8_t *game_color_sprite_line(const struct game_color_sprite *s, uint8_t line)
{
    return game_sprite_width(s) * line + (const uint8_t*)pgm_read_pointer(&s->lines);
}

static void game_sprite_render_line(const struct game_sprite *s, uint8_t *buf,
    int8_t x, uint8_t y, int8_t color, uint8_t ry, uint8_t rot)
{
    uint8_t line = ry - y;
    uint8_t mask = 0x80;
    uint8_t width = game_sprite_width(s);
    const uint8_t *ptr = game_sprite_line(s, line);
    uint8_t spr = pgm_read_byte(ptr);
    for (uint8_t dx = 0; dx < width; ++dx)
    {
        int xx = x;
        if (rot == SPRITE_NORMAL)
            xx += dx;
        else if (rot == SPRITE_MIRROR_H)
            xx += width - 1 - dx;
        if (xx >= 0 && xx < WIDTH)
        {
            if (spr & mask)
            {
                buf[xx] = color;
            }
        }
        mask >>= 1;
        if (!mask)
        {
            mask = 0x80;
            ++ptr;
            spr = pgm_read_byte(ptr);
        }
    }
}

static void game_color_sprite_render_line(const struct game_color_sprite *s,
    uint8_t *buf, int8_t x, uint8_t y, uint8_t ry)
{
    uint8_t line = ry - y;
    uint8_t width = game_sprite_width(s);
    const uint8_t *ptr = game_color_sprite_line(s, line);
    for (uint8_t dx = 0; dx < width; ++dx)
    {
        uint8_t color = pgm_read_byte(ptr++);
        int xx = x + dx;
        if (xx >= 0 && xx < WIDTH)
        {
            if (color != TRANSPARENT)
                buf[xx] = game_make_color(color);
        }
    }
}

void game_draw_sprite(const struct game_sprite *s, int8_t x, int8_t y,
                      uint8_t color, uint8_t rot)
{
    uint8_t height = game_sprite_height(s);
#ifdef FRAME_BUFFER
    if (use_frame_buffer)
    {
      uint8_t width = game_sprite_width(s);
      for (uint8_t ry = y ; ry < y + height ; ++ry)
      {
        if (ry < 0 || ry >= HEIGHT)
          continue;
        game_sprite_render_line(s, &frame[ry][0],
          x, y, game_make_color(color), ry, rot);
      }
      return;
    }
#endif
    uint8_t ry = game_render_y + (y & ADDR_HIGH);
    if (ry < y) ry += ADDR_LOW + 1;
    if (ry >= HEIGHT) return;
    if (ry < (y + height))
    {
        game_sprite_render_line(s, game_render_buf + ((ry & ADDR_HIGH) << ADDR_SHIFT),
          x, y, game_make_color(color), ry, rot);
    }
}

static void game_draw_line(uint8_t *buf, int8_t x, uint8_t w, int8_t color)
{
    for (uint8_t dx = 0; dx < w; ++dx)
    {
        int xx = x + dx;
        if (xx >= 0 && xx < WIDTH)
        {
            buf[xx] = color;
        }
    }
}

void game_draw_rect(int8_t x, int8_t y, int8_t w, int8_t h, uint8_t color)
{
#ifdef FRAME_BUFFER
    if (use_frame_buffer)
    {
      for (uint8_t ry = y ; ry < y + h ; ++ry)
      {
        if (ry < 0 || ry >= HEIGHT)
          continue;
        game_draw_line(&frame[ry][0], x, w, game_make_color(color));
      }
      return;
    }
#endif
    uint8_t ry = game_render_y + (y & ADDR_HIGH);
    if (ry < y) ry += ADDR_LOW + 1;
    if (ry >= HEIGHT) return;
    if (ry < (y + h))
    {
        game_draw_line(game_render_buf + ((ry & ADDR_HIGH) << ADDR_SHIFT),
          x, w, game_make_color(color));
    }
}

void game_draw_color_sprite(const struct game_color_sprite *s, int8_t x, int8_t y)
{
    uint8_t height = game_sprite_height(s);
#ifdef FRAME_BUFFER
    if (use_frame_buffer)
    {
        for (uint8_t ry = y ; ry < y + height ; ++ry)
        {
            if (ry < 0 || ry >= HEIGHT)
                continue;
            game_color_sprite_render_line(s, &frame[ry][0],
                x, y, ry);
        }
        return;
    }
#endif
    uint8_t ry = game_render_y + (y & ADDR_HIGH);
    if (ry < y) ry += ADDR_LOW + 1;
    if (ry >= HEIGHT) return;
    if (ry < (y + height))
    {
        game_color_sprite_render_line(s,
            game_render_buf + ((ry & ADDR_HIGH) << ADDR_SHIFT),
            x, y, ry);
    }
}

void game_draw_pixel(int8_t x, int8_t y, uint8_t color)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;
#ifdef FRAME_BUFFER
    if (use_frame_buffer)
    {
        frame[y][x] = game_make_color(color);
        return;
    }
#endif
    if (game_render_y == (y & ADDR_LOW))
    {
        game_render_buf[x + ((y & ADDR_HIGH) << ADDR_SHIFT)] = game_make_color(color);
    }
}

uint8_t game_get_pixel(int8_t x, int8_t y)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return 0;
#ifdef FRAME_BUFFER
    if (use_frame_buffer)
    {
        return game_make_color_id(frame[y][x]);
    }
#endif

    return 0;
}


void game_draw_vline(int8_t x, int8_t y1, int8_t y2, uint8_t color)
{
    if (x < 0 || x >= WIDTH || y2 < 0 || y1 >= HEIGHT)
        return;
#ifdef FRAME_BUFFER
    if (use_frame_buffer)
    {
        if (y1 < 0)
            y1 = 0;
        if (y2 >= HEIGHT)
            y2 = HEIGHT - 1;
        for (uint8_t y = y1 ; y <= y2 ; ++y)
        {
            frame[y][x] = game_make_color(color);
        }
    }
#endif
    for (uint8_t y = 0 ; y < (1 << ADDR_SHIFT) ; ++y)
    {
        uint8_t yy = game_render_y + y * (ADDR_LOW + 1);
        if (yy >= y1 && yy <= y2)
            game_render_buf[x + y * WIDTH] = game_make_color(color);
    }
}

bool game_is_drawing_lines(int8_t y, int8_t height)
{
#ifdef FRAME_BUFFER
    if (use_frame_buffer)
    {
        return true;
    }
#endif
    for (uint8_t yy = game_render_y ; yy < HEIGHT ; yy += ADDR_LOW + 1)
    {
        if (yy >= y && yy < y + height)
            return true;
    }
    return false;
}

void game_draw_text(const uint8_t *s, int8_t x, int8_t y, uint8_t color, uint8_t bg)
{
#ifdef FRAME_BUFFER
    if (!use_frame_buffer)
#endif
    {
        if (!game_is_drawing_lines(y, FONT_HEIGHT))
            return;
    }
    for (const uint8_t *c = s; *c; ++c)
    {
        game_draw_char(*c, x, y, color, bg);
        x += FONT_WIDTH + 1;
    }
}

void game_draw_char(uint8_t c, int8_t x, int8_t y, uint8_t color, uint8_t bg)
{
    // clipping is not supported
    if (x < 0 || x > WIDTH - FONT_WIDTH)
        return;
#ifdef FRAME_BUFFER
    if (use_frame_buffer)
    {
        int pos = (int)c * FONT_HEIGHT;
        for (uint8_t dy = 0 ; dy < FONT_HEIGHT ; ++dy)
        {
            if (y + dy < 0 || y + dy >= HEIGHT)
                continue;
            uint8_t d = pgm_read_byte_near(font_data + pos + dy);
            for (uint8_t i = 0 ; i < FONT_WIDTH ; ++i)
            {
                if ((d >> (FONT_WIDTH - 1 - i)) & 1)
                {
                    frame[y + dy][x + i] = game_make_color(color);
                }
                else if (bg != TRANSPARENT)
                {
                    frame[y + dy][x + i] = game_make_color(bg);
                }
            }
        }
        return;    
    }
#endif
    
    for (uint8_t s = 0, yy = game_render_y ; s < BUF_LINES ; ++s, yy += ADDR_LOW + 1)
    {
        if (yy < y || yy >= y + FONT_HEIGHT)
            continue;
        int pos = (int)c * FONT_HEIGHT + yy - y;
        uint8_t d = pgm_read_byte_near(font_data + pos);
        for (int i = 0 ; i < FONT_WIDTH ; ++i)
        {
            if ((d >> (FONT_WIDTH - 1 - i)) & 1)
            {
                game_render_buf[x + i + s * WIDTH] = game_make_color(color);
            }
            else if (bg != TRANSPARENT)
            {
                game_render_buf[x + i + s * WIDTH] = game_make_color(bg);
            }
        }
    }
}

void game_draw_digits(uint16_t num, int8_t len, int8_t x, int8_t y, uint8_t color, uint8_t bg)
{
#ifdef FRAME_BUFFER
    if (!use_frame_buffer)
#endif
    {
        if (game_render_y < (y & ADDR_LOW) || game_render_y >= (y & ADDR_LOW) + DIGIT_HEIGHT)
            return;
    }
    if (y <= -DIGIT_HEIGHT || y >= HEIGHT)
        return;
    x += (len - 1) * (DIGIT_WIDTH + 1);
    for (int8_t i = len - 1 ; i >= 0 ; --i, x -= DIGIT_WIDTH + 1)
    {
        uint8_t d = num % 10;
        num /= 10;
#ifdef FRAME_BUFFER
        if (use_frame_buffer)
        {
            for (int8_t dy = 0 ; dy < DIGIT_HEIGHT ; ++dy)
            {
                if (y + dy >= 0 && y + dy < HEIGHT)
                {
                    uint8_t dd = pgm_read_byte_near(digits_data + d * DIGIT_HEIGHT + dy);
                    for (int8_t b = 0 ; b < DIGIT_WIDTH ; ++b)
                    {
                        if (x + b >= 0 && x + b < WIDTH)
                        {
                            if ((dd >> (DIGIT_WIDTH - 1 - b)) & 1)
                            {
                                frame[y + dy][x + b] = game_make_color(color);
                            }
                            else if (bg != TRANSPARENT)
                            {
                                frame[y + dy][x + b] = game_make_color(bg);
                            }
                        }
                    }
                }
            }
        }
        else
#endif
        {
            int pos = d * DIGIT_HEIGHT + (game_render_y - (y & ADDR_LOW));
            uint8_t dd = pgm_read_byte_near(digits_data + pos);
            for (int b = 0; b < DIGIT_WIDTH; ++b)
            {
                if ((dd >> (DIGIT_WIDTH - 1 - b)) & 1)
                {
                    game_render_buf[x + b + ((y & ADDR_HIGH) << ADDR_SHIFT)] = game_make_color(color);
                }
                else if (bg != TRANSPARENT)
                {
                    game_render_buf[x + b + ((y & ADDR_HIGH) << ADDR_SHIFT)] = game_make_color(bg);
                }
            }
        }
    }
}

void game_render_line(uint8_t *buf, int8_t line)
{
    int i = 0;
    for (int8_t y = 0 ; y < BUF_LINES ; ++y)
        for (int8_t x = 0; x < WIDTH ; ++x, ++i)
        {
#ifdef FRAME_BUFFER
            if (use_frame_buffer)
                buf[i] = frame[line + y * (ADDR_LOW + 1)][x];
            else
#endif
                buf[i] = background;
        }

    game_render_buf = buf;
    game_render_y = line;

    // call user render()
    bool fb = use_frame_buffer;
    use_frame_buffer = false;
    render();
    use_frame_buffer = fb;

#ifndef GFX_E
    if (line == WIDTH / 4 - 1) // fix broken led
        buf[WIDTH * 4 - 1] = 0;
#endif
}

void game_enable_frame_buffer()
{
#ifdef FRAME_BUFFER
    use_frame_buffer = true;
#endif
}

void game_set_background(uint8_t color)
{
    background = color;
}

void game_clear_screen()
{
#ifdef FRAME_BUFFER
    if (use_frame_buffer)
    {
        memset(frame, background, sizeof(frame));
    }
#endif
}


void graphics_setup()
{
    sclkpin   = digitalPinToBitMask(GFX_CLK);
    latport   = portOutputRegister(digitalPinToPort(GFX_LAT));
    latpin    = digitalPinToBitMask(GFX_LAT);
    oeport    = portOutputRegister(digitalPinToPort(GFX_OE));
    oepin     = digitalPinToBitMask(GFX_OE);
    addraport = portOutputRegister(digitalPinToPort(GFX_A));
    addrapin  = digitalPinToBitMask(GFX_A);
    addrbport = portOutputRegister(digitalPinToPort(GFX_B));
    addrbpin  = digitalPinToBitMask(GFX_B);
    addrcport = portOutputRegister(digitalPinToPort(GFX_C));
    addrcpin  = digitalPinToBitMask(GFX_C); 
    addrdport = portOutputRegister(digitalPinToPort(GFX_D));
    addrdpin  = digitalPinToBitMask(GFX_D); 
#ifdef E
    addreport = portOutputRegister(digitalPinToPort(GFX_E));
    addrepin  = digitalPinToBitMask(GFX_E);
#endif
  
    pinMode(GFX_CLK, OUTPUT);
    pinMode(GFX_LAT, OUTPUT);
    pinMode(GFX_OE, OUTPUT);
    pinMode(GFX_A, OUTPUT);
    pinMode(GFX_B, OUTPUT);
    pinMode(GFX_C, OUTPUT);
    pinMode(GFX_D, OUTPUT);
#ifdef GFX_E
    pinMode(GFX_E, OUTPUT);
#endif

    pinMode(GFX_IR1, OUTPUT);
    pinMode(GFX_IG1, OUTPUT);
    pinMode(GFX_IB1, OUTPUT);
    pinMode(GFX_IR2, OUTPUT);
    pinMode(GFX_IG2, OUTPUT);
    pinMode(GFX_IB2, OUTPUT);

    GFX_SCLKPORT   |= sclkpin;
    *latport   &= ~latpin;
    *oeport    &= ~oepin;
    *addraport &= ~addrapin;
    *addrbport &= ~addrbpin;
    *addrcport &= ~addrcpin;
    *addrdport &= ~addrdpin;
#ifdef GFX_E
    *addreport &= ~addrepin;
#endif

    digitalWrite(GFX_IR1, HIGH);
    digitalWrite(GFX_IG1, HIGH);
    digitalWrite(GFX_IB1, HIGH);
    digitalWrite(GFX_IR2, HIGH);
    digitalWrite(GFX_IG2, HIGH);
    digitalWrite(GFX_IB2, HIGH);

    // Set up Timer1 for interrupt:
    TCCR1A  = _BV(WGM11); // Mode 14 (fast PWM), OC1A off
    TCCR1B  = _BV(WGM13) | _BV(WGM12) | _BV(CS10) | _BV(CS11); // Mode 14, div 64
    ICR1    = 0;
    TIMSK1 |= _BV(TOIE1); // Enable Timer1 interrupt

    /* Emulator hack */
    *((uint8_t*)0x58) = 0x01;
}

static void graphics_update()
{
    int tock;
    int tick;
    tock = GFX_SCLKPORT;
    tick = tock & ~sclkpin;

    uint8_t lines[BUF_LINES * WIDTH];
    game_render_line((uint8_t*)lines, step);

    uint8_t *line1 = &lines[(BUF_LINES - 1) * WIDTH];
    uint8_t *line2 = &lines[(BUF_LINES - 2) * WIDTH];

#ifdef GFX_PORT2
    uint8_t tmp;
    #define pew asm volatile(                 \
      "ld  %[tmp], %a[ptr1]+"       "\n\t"    \
      "out %[data2], %[tmp]"        "\n\t"    \
      "ld  %[tmp], %a[ptr2]+"       "\n\t"    \
      "out %[data], %[tmp]"         "\n\t"    \
      "out %[clk]     , %[tick]"     "\n\t"   \
      "out %[clk]     , %[tock]"     "\n"     \
      :  [ptr1] "+e" (line1),                 \
         [ptr2] "+e" (line2),                 \
         [tmp] "=r" (tmp)                     \
      :  [data] "I" (_SFR_IO_ADDR(GFX_DATAPORT)), \
         [data2] "I" (_SFR_IO_ADDR(GFX_DATAPORT2)), \
         [clk]  "I" (_SFR_IO_ADDR(GFX_SCLKPORT)), \
         [tick] "r" (tick),                   \
         [tock] "r" (tock) );
#else
    uint8_t const8 = 8;
    uint8_t const4 = 4;
    uint8_t tmp;
    #define pew asm volatile(                 \
      "ld  %[tmp], %a[ptr1]+"       "\n\t"    \
      "mul %[tmp], %[c8]"           "\n\t"    \
      "ld  %[tmp], %a[ptr2]+"       "\n\t"    \
      "or  r0, %[tmp]"              "\n\t"    \
      "mul r0, %[c4]"                "\n\t"   \
      "out %[data]    , r0"          "\n\t"   \
      "out %[clk]     , %[tick]"     "\n\t"   \
      "out %[clk]     , %[tock]"     "\n"     \
      :  [ptr1] "+e" (line1),                 \
         [ptr2] "+e" (line2),                 \
         [tmp] "=r" (tmp),                    \
         [c8] "+r" (const8),                  \
         [c4] "+r" (const4)                   \
      :  [data] "I" (_SFR_IO_ADDR(GFX_DATAPORT)), \
         [clk]  "I" (_SFR_IO_ADDR(GFX_SCLKPORT)), \
         [tick] "r" (tick),                   \
         [tock] "r" (tock): "r0", "r1");
#endif

#ifdef GFX_PORT2
    #define pow asm volatile(                 \
      "ld  %[tmp], %a[ptr1]+"       "\n\t"    \
      "swap %[tmp]"                 "\n\t"    \
      "out %[data2], %[tmp]"        "\n\t"    \
      "ld  %[tmp], %a[ptr2]+"       "\n\t"    \
      "swap %[tmp]"                 "\n\t"    \
      "out %[data], %[tmp]"         "\n\t"    \
      "out %[clk]     , %[tick]"     "\n\t"   \
      "out %[clk]     , %[tock]"     "\n"     \
      :  [ptr1] "+e" (line1),                 \
         [ptr2] "+e" (line2),                 \
         [tmp] "=r" (tmp)                     \
      :  [data] "I" (_SFR_IO_ADDR(GFX_DATAPORT)), \
         [data2] "I" (_SFR_IO_ADDR(GFX_DATAPORT2)), \
         [clk]  "I" (_SFR_IO_ADDR(GFX_SCLKPORT)), \
         [tick] "r" (tick),                   \
         [tock] "r" (tock) );
#endif
#ifdef GFX_PORT2
if (!color_channel)
#endif
{
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew
#ifndef GFX_E
    line1 = &lines[1 * WIDTH];
    line2 = &lines[0 * WIDTH];

    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
    pew pew pew pew pew pew pew pew 
#endif
}
#ifdef GFX_PORT2
else
{
    pow pow pow pow pow pow pow pow 
    pow pow pow pow pow pow pow pow 
    pow pow pow pow pow pow pow pow 
    pow pow pow pow pow pow pow pow 
    pow pow pow pow pow pow pow pow 
    pow pow pow pow pow pow pow pow 
    pow pow pow pow pow pow pow pow 
    pow pow pow pow pow pow pow pow
#ifndef GFX_E
    line1 = &lines[1 * WIDTH];
    line2 = &lines[0 * WIDTH];

    pow pow pow pow pow pow pow pow 
    pow pow pow pow pow pow pow pow 
    pow pow pow pow pow pow pow pow 
    pow pow pow pow pow pow pow pow 
    pow pow pow pow pow pow pow pow 
    pow pow pow pow pow pow pow pow 
    pow pow pow pow pow pow pow pow 
    pow pow pow pow pow pow pow pow 
#endif
}
#endif

    *oeport |= oepin;

    if (step & 1)
        *addraport |= addrapin;
    else
        *addraport &= ~addrapin;
    if (step & 2)
        *addrbport |= addrbpin;
    else
        *addrbport &= ~addrbpin;
    if (step & 4)
        *addrcport |= addrcpin;
    else
        *addrcport &= ~addrcpin;
    if (step & 8)
        *addrdport |= addrdpin;
    else
        *addrdport &= ~addrdpin;
#ifdef GFX_E
    if (step & 16)
        *addreport |= addrepin;
    else
        *addreport &= ~addrepin;
#endif

    *latport |= latpin;
    *latport &= ~latpin;
    *oeport &= ~oepin;

    step = (step + 1) & ADDR_LOW;
#if defined(COLOR_6BIT) && COLOR_6BIT
    if (step == 0)
        color_channel ^= 1;
#endif
}

ISR(TIMER1_OVF_vect, ISR_BLOCK) { // ISR_BLOCK important
  graphics_update();              // Call refresh func
  TIFR1 |= TOV1;                  // Clear Timer1 interrupt flag
  // Interval 2 is too small even for invaders, but ok for 6-bit colors
  // 64 is ok for 3-bit colors
  ICR1      = color_channel ? 32 : 8;        // Set interval for next interrupt
  TCNT1     = 0;        // Restart interrupt timer
}
