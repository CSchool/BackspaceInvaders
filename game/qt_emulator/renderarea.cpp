#ifndef ARDUINO
#include "renderarea.h"

#include "../libgame.h"
#include "../graphics.h"
#include "../font.h"
#include "../font_data.h"

#include <QPainter>

/* static functions and data for Arduino project compatibility */
static QColor frame[HEIGHT][WIDTH];
static QColor screen[HEIGHT][WIDTH];
static bool use_frame_buffer;
static int background;

static int color_map[4] = {0, 128, 192, 255};

static int rev_color_map(int c)
{
    switch (c)
    {
    case 128:
        return 1;
    case 192:
        return 2;
    case 255:
        return 3;
    }
    return 0;
}

static QColor map_color(uint8_t c)
{
    int r = c & RED;
    r = (r & 0xf) + ((r & 0xf0) >> 3);
    int g = c & GREEN;
    g = ((g & 0xf) >> 1) + ((g & 0xf0) >> 4);
    int b = c & BLUE;
    b = ((b & 0xf) >> 2) + ((b & 0xf0) >> 5);
    QColor color(color_map[r], color_map[g], color_map[b]);
    return color;
}

void game_draw_pixel(int8_t x, int8_t y, uint8_t c)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;
    QColor color = map_color(c);
    if (use_frame_buffer)
        frame[y][x] = color;
    else
        screen[y][x] = color;
}

uint8_t game_get_pixel(int8_t x, int8_t y)
{
    if (!use_frame_buffer || x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return 0;
    QColor c = frame[y][x];
    int r = rev_color_map(c.red());
    int g = rev_color_map(c.green());
    int b = rev_color_map(c.blue());
    r = ((r & 2) << 3) + (r & 1);
    g = ((g & 2) << 4) + ((g & 1) << 1);
    b = ((b & 2) << 5) + ((b & 1) << 2);
    return r + g + b;
}

void game_draw_vline(int8_t x, int8_t y1, int8_t y2, uint8_t color)
{
    for (int i = y1 ; i <= y2 ; ++i)
        game_draw_pixel(x, i, color);
}

void game_draw_rect(int8_t x, int8_t y, int8_t w, int8_t h, uint8_t color)
{
    for (int dx = 0; dx < w ; ++dx)
        for (int dy = 0; dy < h ; ++dy)
            game_draw_pixel(x + dx, y + dy, color);
}

void game_draw_sprite(const struct game_sprite *s, int8_t x, int8_t y,
                      uint8_t color, uint8_t rot)
{
    for (int dx = 0; dx < s->width; ++dx)
    {
        for (int dy = 0; dy < s->height; ++dy)
        {
            int xx = x + dx;
            int yy = y + dy;
            if (xx < 0 || xx >= WIDTH || yy < 0 || yy >= HEIGHT)
                continue;
            uint8_t dw = s->width % 8 ? 8 - s->width % 8 : 0;
            int byte = (s->width + 7) / 8 * dy;
            if (rot == SPRITE_NORMAL)
                byte += dx / 8;
            else if (rot == SPRITE_MIRROR_H)
                byte += (s->width - 1 - dx) / 8;
            int bit = 0;
            if (rot == SPRITE_NORMAL)
                bit = 7 - dx % 8;
            else if (rot == SPRITE_MIRROR_H)
                bit = (dx + dw) % 8;
            if ((s->lines[byte] >> bit) & 1)
                game_draw_pixel(xx, yy, color);
        }
    }
}

void game_draw_color_sprite(const struct game_color_sprite *s, int8_t x, int8_t y)
{
    const uint8_t *data = s->lines;
    for (int dy = 0; dy < s->height ; ++dy)
    {
        for (int dx = 0; dx < s->width ; ++dx, ++data)
        {
            int xx = x + dx;
            int yy = y + dy;
            if (xx < 0 || xx >= WIDTH || yy < 0 || yy >= HEIGHT)
                continue;
            int color = *data;
            if (color != TRANSPARENT)
                game_draw_pixel(xx, yy, color);
        }
    }
}

void game_draw_char(uint8_t c, int8_t x, int8_t y, uint8_t color, uint8_t bg)
{
    int pos = (int)c * FONT_HEIGHT;
    for (int i = 0; i < FONT_WIDTH; ++i)
    {
        for (int j = 0; j < FONT_HEIGHT; ++j)
        {
            uint8_t d = font_data[pos + j];
            if ((d >> (FONT_WIDTH - 1 - i)) & 1)
                game_draw_pixel(x + i, y + j, color);
            else if (bg != TRANSPARENT)
                game_draw_pixel(x + i, y + j, bg);
        }
    }
}

void game_draw_text(const uint8_t *s, int8_t x, int8_t y, uint8_t color, uint8_t bg)
{
    int xx = x;
    int yy = y;
    for (const uint8_t *c = s; *c; ++c)
    {
        game_draw_char(*c, xx, yy, color, bg);
        xx += FONT_WIDTH + 1;
    }
}

void game_draw_digits(uint16_t num, int8_t len, int8_t x, int8_t y, uint8_t color, uint8_t bg)
{
    x += (len - 1) * (DIGIT_WIDTH + 1);
    for (int i = len - 1 ; i >= 0 ; --i, x -= DIGIT_WIDTH + 1)
    {
        uint8_t d = num % 10;
        num /= 10;
        for (int8_t dy = 0 ; dy < DIGIT_HEIGHT ; ++dy)
        {
            uint8_t dd = digits_data[d * DIGIT_HEIGHT + dy];
            for (int b = 0 ; b < DIGIT_WIDTH ; ++b)
            {
                if ((dd >> (DIGIT_WIDTH - 1 - b)) & 1)
                    game_draw_pixel(x + b, y + dy, color);
                else if (bg != TRANSPARENT)
                    game_draw_pixel(x + b, y + dy, bg);
            }
        }
    }
}

bool game_is_drawing_lines(int8_t y, int8_t height)
{
    return true;
}

void game_clear_screen()
{
    for (int y = 0 ; y < HEIGHT ; ++y)
    {
        for (int x = 0 ; x < WIDTH ; ++x)
        {
            if (use_frame_buffer)
                frame[y][x] = map_color(background);
            else
                screen[y][x] = background ? map_color(background) : QColor(0, 0, 0, 0);
        }
    }
}

void game_set_background(uint8_t color)
{
    background = color;
}

void game_enable_frame_buffer()
{
    use_frame_buffer = true;
    game_clear_screen();
}

/**************************************************************/

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    framebuffer = false;
}

void RenderArea::beginRender()
{
    framebuffer = use_frame_buffer;
    use_frame_buffer = false;
    game_clear_screen();
}

void RenderArea::endRender()
{
    use_frame_buffer = framebuffer;
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(WIDTH * 4 + 1, HEIGHT * 4 + 1);
}

QSize RenderArea::sizeHint() const
{
    return QSize(WIDTH * 10 + 1, HEIGHT * 10 + 1);
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setPen(QColor(0, 0, 0));
    int pixelW = width() / WIDTH;
    int pixelH = height() / HEIGHT;
    for (int y = 0 ; y < HEIGHT ; ++y)
    {
        for (int x = 0 ; x < WIDTH ; ++x)
        {
            QBrush brush;
            if (screen[y][x].alpha())
            {
                brush = QBrush(screen[y][x]);
            }
            else if (use_frame_buffer)
            {
                brush = QBrush(frame[y][x]);
            }
            else
            {
                brush = QBrush(QColor(0, 0, 0));
            }
            painter.setBrush(brush);
            painter.drawRect(QRect(x * pixelW, y * pixelH, pixelW, pixelH));
            painter.setBrush(Qt::NoBrush);
        }
    }
}
#endif
