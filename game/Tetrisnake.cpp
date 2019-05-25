#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "graphics.h"
#include "binary.h"
#include "controls.h"
#include "music.h"
#include "font.h"
#include "tunes.h"

#ifdef FRAME_BUFFER

#define ROTLEFT BUTTON_SE
#define ROTRIGHT BUTTON_SW

#define LEFT BUTTON_LEFT
#define RIGHT BUTTON_RIGHT
#define UP BUTTON_UP
#define DOWN BUTTON_DOWN

#define GAMEOVER_X 16
#define GAMEOVER_Y 24

#define TETRISNAKE_WIDTH 10
#define TETRISNAKE_HEIGHT 32

#define BLOCK_BUFFER_SIZE (TETRISNAKE_WIDTH*TETRISNAKE_HEIGHT)
#define MAXLEN BLOCK_BUFFER_SIZE

#define LEFT_BOUND 10
#define RIGHT_BOUND (LEFT_BOUND+TETRISNAKE_WIDTH+1)
#define UP_BOUND -1
#define DOWN_BOUND (UP_BOUND+TETRISNAKE_HEIGHT+1)

#define TIME_TO_MOVE 6
#define TIME_TO_GENERATE 90

#define POINTS_FOR_LINE 50

static const uint8_t gameOverLines[] PROGMEM = {
    B00111110, B00111000, B11000110, B11111110,
    B01100000, B01101100, B11101110, B11000000,
    B11000000, B11000110, B11111110, B11000000,
    B11001110, B11000110, B11111110, B11111100,
    B11000110, B11111110, B11010110, B11000000,  
    B01100110, B11000110, B11000110, B11000000,
    B00111110, B11000110, B11000110, B11111110,
    B00000000, B00000000, B00000000, B00000000,
    B01111100, B11000110, B11111110, B11111100,
    B11000110, B11000110, B11000000, B11000110,
    B11000110, B11000110, B11000000, B11000110,
    B11000110, B11000110, B11111100, B11001110,
    B11000110, B01101100, B11000000, B11111000,
    B11000110, B00111000, B11000000, B11011100,
    B01111100, B00010000, B11111110, B11001110
};

static const game_sprite gameOver PROGMEM = {
    31, 15, gameOverLines
};

#define BRICK_FORMS_COUNT 7
static const uint8_t brickForms[BRICK_FORMS_COUNT][4] PROGMEM = {
   {B10000000,
    B10000000,
    B10000000,
    B10000000},

   {B11110000,
    B00000000,
    B00000000,
    B00000000},

   {B10000000,
    B10000000,
    B10000000,
    B11110000},

   {B00010000,
    B00010000,
    B00010000,
    B11110000},

   {B00110000,
    B01100000,
    B11000000,
    B00000000},

   {B01000000,
    B11100000,
    B00000000,
    B00000000},

   {B11100000,
    B10100000,
    B10100000,
    B00000000}

    };

static const game_sprite tetrisnakeBrick[] PROGMEM = {
    {1, 4, brickForms[0]},
    {4, 1, brickForms[1]},
    {4, 4, brickForms[2]},
    {4, 4, brickForms[3]},
    {4, 3, brickForms[4]},
    {3, 2, brickForms[5]},
    {3, 3, brickForms[6]},
};

// TetrisnakeBlock structure
#define block_color(i)            (data->blocks[i] & COLOR)
#define block_property(i, mask) !!(data->blocks[i] & mask)
#define IS_EXIST  0x01
#define IS_BOND_R 0x02
#define IS_BOND_U 0x04
#define PHYS      0x08 // fall already processed
#define DFS       0x10
#define ALIEN     0x20 // mark the cells in which we were already
#define COLOR     0xC0 // the reason for not falling is someone else’s block

enum Phases
{
    PHASE_GAME,
    PHASE_GAMEOVER
};

struct TetrisnakeData
{
    uint8_t phase;
    uint8_t snakeX[MAXLEN + 1];
    uint8_t snakeY[MAXLEN + 1];
    int8_t velX;
    int8_t velY;
    bool rotLeftPressed;
    bool rotRightPressed;
    bool half;
    uint16_t snakeBegin;
    uint16_t snakeEnd;
    uint8_t hiscore;
    uint8_t tact;
    uint8_t blockTimer;
    uint16_t score;
    uint8_t blocks[BLOCK_BUFFER_SIZE];
    int8_t genFigureX;
    int8_t genFigureY;
    uint8_t genFigureColor;
    uint8_t genFigureIsInverted;
    uint8_t genFigureWidth;
    uint8_t genFigureHeight;
    uint8_t genFigureLines[4];
};
static TetrisnakeData* data;

static void set_blk_prop(uint16_t i, uint8_t mask, uint8_t value)
{
    uint8_t bit = (value & 1);
    data->blocks[i] = bit ? (data->blocks[i] | mask) : (data->blocks[i] & ~mask);
}

static void set_color(uint16_t i, uint8_t value)
{
    data->blocks[i] &= ~COLOR;
    data->blocks[i] |= value;
}

static uint8_t get_color(uint8_t color)
{
    color &= COLOR;
    switch(color) {
        case 0x00: 
            return RED;
        case 0x40:
            return CYAN;
        case 0x80:
            return PURPLE;
        case 0xC0:
            return YELLOW;
    }
    return color ? RED_DARK : RED;
}

void game_over()
{
    if (data->score > data->hiscore)
    {
        data->hiscore = data->score;
        game_save(&data->hiscore, sizeof(data->hiscore));
    }
    data->phase = PHASE_GAMEOVER;
}

static void Tetrisnake_draw_rect(int x, int y, int c)
{
    game_draw_pixel(x * 2, y * 2, c);
    game_draw_pixel((x * 2 + 1) % WIDTH, y * 2, c);
    game_draw_pixel(x * 2, (y * 2 + 1) % HEIGHT, c);
    game_draw_pixel((x * 2 + 1) % WIDTH, (y * 2 + 1) % HEIGHT, c);
}

static void Tetrisnake_draw_borders() {
    for(int dy = UP_BOUND;dy<=DOWN_BOUND;dy++)
    {
        Tetrisnake_draw_rect(LEFT_BOUND, dy, WHITE);
        Tetrisnake_draw_rect(RIGHT_BOUND, dy, WHITE);
    }
    for(int dx = LEFT_BOUND;dx<=RIGHT_BOUND;dx++)
    {
        if(UP_BOUND >= 0)
            Tetrisnake_draw_rect(dx, UP_BOUND, WHITE);
        if(DOWN_BOUND < 32)
            Tetrisnake_draw_rect(dx, DOWN_BOUND, WHITE);
    }
}

static bool is_a_bound(int newX, int newY)
{
    return newX >= RIGHT_BOUND || newX <= LEFT_BOUND || newY >= DOWN_BOUND || newY <= UP_BOUND;
}

static bool is_a_block(int newX, int newY)
{
    uint8_t pixel = game_get_pixel(newX * 2, newY * 2);
    return pixel != GREEN && pixel != BLACK;
}

static bool is_a_snake(int newX, int newY)
{
    uint8_t pixel = game_get_pixel(newX * 2, newY * 2);
    return pixel == GREEN;
}

static bool is_a_snake(uint16_t i)
{
    uint8_t x = (i % TETRISNAKE_WIDTH) + LEFT_BOUND + 1;
    uint8_t y = (i / TETRISNAKE_WIDTH) + UP_BOUND + 1;
    return is_a_snake(x,y);
}

static bool is_alien_block(uint16_t i) // for bottom block
{
    if(i >= BLOCK_BUFFER_SIZE)
        return true;
        
    return block_property(i, IS_EXIST) && !block_property(i, IS_BOND_U);
}

static bool is_any_obstacle(int newX, int newY)
{
    if(is_a_bound(newX, newY)) 
    {
        return true;
    }
    uint8_t pixel = game_get_pixel(newX * 2, newY * 2);
    return pixel != BLACK;
}

static bool is_finish_gen()
{
    return data->genFigureY > UP_BOUND;
}

static bool is_fell_on_brick(int newX, int newY)
{
    uint8_t i = (newX - LEFT_BOUND - 1) + (TETRISNAKE_WIDTH * (newY - UP_BOUND-1));
    return block_property(i, IS_EXIST) && block_property(i, ALIEN);
}

static bool is_fell_on_snake(int newX, int newY)
{
    uint8_t i = (newX - LEFT_BOUND - 1) + (TETRISNAKE_WIDTH * (newY - UP_BOUND-1));
    return is_a_snake(newX, newY) || block_property(i, IS_EXIST) && !block_property(i, ALIEN);
}

static void remove_line(uint16_t i)
{
    uint8_t y = (i / TETRISNAKE_WIDTH) + UP_BOUND + 1; 
    uint8_t x = LEFT_BOUND + 1;
    for(uint16_t j = i; j < i+TETRISNAKE_WIDTH; j++,x++)
    {
        set_blk_prop(j, IS_EXIST, 0);
        Tetrisnake_draw_rect(x, y, BLACK);
    }
}

static bool check_lines()
{
    for(uint16_t i=0;i<BLOCK_BUFFER_SIZE;i+=TETRISNAKE_WIDTH)
    {
        bool isLine = true;
        for(uint16_t j = i; j < i+TETRISNAKE_WIDTH; j++)
        {
            if(block_property(j, IS_EXIST)==0 || block_property(j, ALIEN)==0)
            {
                isLine = false;
                break;
            }
        }
        if(isLine)
        {
            remove_line(i);
            return true;
        }
    }
    return false;
}

static void cut_snake()
{
    uint16_t length = (MAXLEN + data->snakeEnd - data->snakeBegin) % MAXLEN;
    uint16_t half_l = length >= 6 ? (length / 2) : (length - 3);
    for(uint16_t i = 0, j = (MAXLEN + data->snakeEnd-1)%MAXLEN; i<half_l; i++, j = (MAXLEN + j-1) % MAXLEN)
    {
        Tetrisnake_draw_rect(data->snakeX[j], data->snakeY[j], BLACK);
    }
    data->snakeEnd = (MAXLEN + data->snakeEnd - half_l) % MAXLEN; 
}

static void generate_brick()
{
    uint8_t  formIdx = rand() % BRICK_FORMS_COUNT;
    data->genFigureWidth  = pgm_read_byte(&tetrisnakeBrick[formIdx].width);
    data->genFigureHeight = pgm_read_byte(&tetrisnakeBrick[formIdx].height);
    int8_t  x =LEFT_BOUND + 1 + rand() % (RIGHT_BOUND-LEFT_BOUND-data->genFigureWidth);
    int8_t y = UP_BOUND-data->genFigureHeight+1;
    uint8_t color = (rand() % 4) << 6;
    data->genFigureX = x;
    data->genFigureY = y;
    data->genFigureColor = color;
    data->genFigureIsInverted = rand() % 2;
    for(uint8_t i=0;i<4;i++)
    {
        data->genFigureLines[i] = pgm_read_byte((uint8_t*)pgm_read_pointer(&tetrisnakeBrick[formIdx].lines)+i);
    }
}

static void generate_block(int x, bool left, bool right, bool up, uint8_t color)
{
    uint16_t it = (x - LEFT_BOUND - 1) + (TETRISNAKE_WIDTH * (UP_BOUND+1));
    set_blk_prop(it, IS_EXIST, 1);
    set_blk_prop(it, IS_BOND_R, right);
    set_blk_prop(it, IS_BOND_U, up);
    set_blk_prop(it, PHYS, !data->tact);
    set_blk_prop(it, DFS, !data->tact);
    set_color(it, color);
    Tetrisnake_draw_rect(x, UP_BOUND+1, get_color(color));
}

static void generate_blocks()
{
    if(data->genFigureY <= UP_BOUND) {
        int8_t height = data->genFigureHeight;
        int8_t width = data->genFigureWidth;
        int16_t startX = data->genFigureX;

        int8_t  y = data->genFigureIsInverted ? height + data->genFigureY : -data->genFigureY-1;
        uint8_t line = data->genFigureLines[y];
        uint8_t upLine;
        if(data->genFigureIsInverted)
            upLine = y < height-1 ? data->genFigureLines[y+1] : 0x00;
        else
            upLine = y > 0 ? data->genFigureLines[y-1] : 0x00;
        uint8_t mask = 0x80;
        bool isFree = true;
        for(uint8_t dx=0;dx<width;++dx)
        {
            if(line & mask)
            {
                if(is_fell_on_snake(startX+dx, UP_BOUND+1)) {
                    isFree = false;
                    break;
                } 
                else if(is_fell_on_brick(startX+dx,UP_BOUND+1))
                {
                    game_over();
                }
            }
            mask >>= 1;
        }
        if(isFree)
        {
            mask = 0x80;
            for(uint8_t dx=0;dx<width;++dx)
            {
                if(line & mask)
                {
                    bool left = line & (mask << 1);
                    bool right = line & (mask >> 1);
                    bool up = upLine & mask;
                    generate_block(startX+dx, left, right, up, data->genFigureColor);
                }
                mask >>= 1;
            }
            data->genFigureY++;
        }
    }
}

static bool fall_dfs(int16_t i)
{
    bool isFall = block_property(i, PHYS) != data->tact;
    bool isAlien = false;
    if(block_property(i, DFS) != data->tact)
    {
        int16_t blocks[16];
        uint8_t count = 1;
        blocks[0]=i;
        for(uint8_t j=0; j<count && j<16; j++)
        {
            int16_t k = blocks[j];
            set_blk_prop(k, DFS, data->tact);
            uint8_t x = k%TETRISNAKE_WIDTH;
            if(x > 0 && block_property(k-1, IS_EXIST) && block_property(k-1, IS_BOND_R)  && 
                block_property(k-1, DFS) != data->tact)
            {
                blocks[count]=k-1;
                count++;
            }
            if(x < TETRISNAKE_WIDTH - 1 && block_property(k, IS_BOND_R)&& block_property(k+1, IS_EXIST) && block_property(k+1, DFS) != data->tact)
            {
                blocks[count]=k+1;
                count++;
            }
            
            if(block_property(k, IS_BOND_U) && k-TETRISNAKE_WIDTH >= 0 && block_property(k-TETRISNAKE_WIDTH, IS_EXIST) &&
               block_property(k-TETRISNAKE_WIDTH, DFS) != data->tact)
            {
                blocks[count]=k-TETRISNAKE_WIDTH;
                count++;
            }
            
            
            if(k+TETRISNAKE_WIDTH < BLOCK_BUFFER_SIZE && block_property(k+TETRISNAKE_WIDTH, IS_EXIST) && 
                block_property(k+TETRISNAKE_WIDTH, IS_BOND_U)  && block_property(k+TETRISNAKE_WIDTH, DFS) != data->tact)
            {
                blocks[count]=k+TETRISNAKE_WIDTH;
                count++;
            }
            if(is_alien_block(k+TETRISNAKE_WIDTH))
            {
                isFall = false;
                isAlien = true;
            }
            if(k+TETRISNAKE_WIDTH < BLOCK_BUFFER_SIZE && is_a_snake(k+TETRISNAKE_WIDTH))
                isFall = false;
        }
        if(!isFall)
        {
            for(uint8_t j=0; j<count; j++)
            {
                int16_t k = blocks[j];
                set_blk_prop(k, PHYS, data->tact);
                set_blk_prop(k, ALIEN, isAlien);
            }
        }
    }
    return isFall;
}

static void fall_block(uint16_t i)
{
    if(block_property(i, PHYS) != data->tact && block_property(i, IS_EXIST)) {
        uint8_t x = (i % TETRISNAKE_WIDTH) + LEFT_BOUND + 1;
        uint8_t y = (i / TETRISNAKE_WIDTH) + UP_BOUND + 1; 
        uint16_t j = i + TETRISNAKE_WIDTH;
        if(fall_dfs(i)) // fall_dfs(i) && !is_any_obstacle(x, y+1)
        {
            set_blk_prop(i, PHYS, data->tact);
            set_blk_prop(i, DFS, data->tact);
            data->blocks[j] = data->blocks[i];
            set_blk_prop(i, IS_EXIST, 0);
            Tetrisnake_draw_rect(x, y, BLACK);
            Tetrisnake_draw_rect(x, y+1, get_color(block_color(i)));
        }
    }
    set_blk_prop(i, PHYS, data->tact);
    set_blk_prop(i, DFS, data->tact);
}

static void fall_blocks()
{
    for(int16_t i=BLOCK_BUFFER_SIZE - 1; i >= 0;i--)
    {
        fall_block(i);
    }
    data->tact = !data->tact;
}

static void Tetrisnake_clear_blocks()
{
    for(uint16_t i=0; i<BLOCK_BUFFER_SIZE; i++)
    {
        data->blocks[i] = 0;
    }
}

static void Tetrisnake_reset()
{
    game_clear_screen();
    data->phase = PHASE_GAME;
    data->snakeX[0] = 17;
    data->snakeY[0] = 16;
    data->snakeX[1] = 16;
    data->snakeY[1] = 16;
    data->snakeX[2] = 15;
    data->snakeY[2] = 16;
    data->snakeBegin = 0;
    data->snakeEnd = 3;
    data->velX = 1;
    data->velY = 0;
    data->rotLeftPressed = false;
    data->rotRightPressed = false;
    data->half = false;
    data->genFigureX = 0;
    data->genFigureY = 0;
    data->genFigureColor = 0;
    data->genFigureIsInverted = 0;
    data->tact = 0;
    data->blockTimer = 0;
    data->score = 0;
    Tetrisnake_clear_blocks();
    Tetrisnake_draw_borders();
    generate_brick();
}

static void Tetrisnake_prepare()
{
    game_set_ups(25);
    game_enable_frame_buffer();
    Tetrisnake_reset();
    data->hiscore = 0;
    game_load(&data->hiscore, sizeof(data->hiscore));
#ifndef EMULATED
    tune_init(tetris2_addr());
    tune_enable();
#endif
}

static int velsign(int x)
{
    if (x == 0) return 0;
    if (x == 1) return 1;
    return -1;
}

static void Tetrisnake_render()
{
    // draw score
    game_draw_digits(data->score < 999 ? data->score : 999,
        3, 0, 0, WHITE, BLACK);
    // draw hiscore
    game_draw_digits(data->hiscore < 999 ? data->hiscore : 999, 3, WIDTH - (DIGIT_WIDTH + 1) * 3,
        0, WHITE, BLACK);
    if (data->phase == PHASE_GAMEOVER)
        game_draw_sprite(&gameOver, GAMEOVER_X, GAMEOVER_Y, BLUE);
}

static void Tetrisnake_update(unsigned long delta)
{
    if (data->phase == PHASE_GAME) data->half = !data->half; else data->half = false;
    if (data->phase == PHASE_GAME && !data->half)
    {
        // move snake forward
        int newX = (data->snakeX[data->snakeBegin] + data->velX);
        int newY = (data->snakeY[data->snakeBegin] + data->velY);
        if(is_a_bound(newX, newY)) 
        {
            int altX = (data->snakeX[data->snakeBegin] + data->velY);
            int altY = (data->snakeY[data->snakeBegin] + data->velX);
            if(!is_a_bound(altX, altY) && !is_a_snake(altX, altY))
            {
                int t = data->velX;
                data->velX = data->velY;
                data->velY = t;
                newX = altX;
                newY = altY;
            }
            else
            {
                int t = -data->velX;
                data->velX = -data->velY;
                data->velY = t;
                newX = (data->snakeX[data->snakeBegin] + data->velX);
                newY = (data->snakeY[data->snakeBegin] + data->velY);
            }
        }
        data->snakeBegin = (data->snakeBegin + MAXLEN - 1) % MAXLEN;
        data->snakeEnd = (data->snakeEnd + MAXLEN - 1) % MAXLEN;
        Tetrisnake_draw_rect(data->snakeX[data->snakeEnd], data->snakeY[data->snakeEnd], BLACK);
        data->snakeX[data->snakeBegin] = newX;
        data->snakeY[data->snakeBegin] = newY; 
        if (is_a_block(newX, newY))
        {
            uint16_t i = (newX - LEFT_BOUND - 1) + (TETRISNAKE_WIDTH * (newY - UP_BOUND-1));
            if((i % TETRISNAKE_WIDTH) > 0)
                set_blk_prop(i-1, IS_BOND_R, 0);
            if((i + TETRISNAKE_WIDTH) < BLOCK_BUFFER_SIZE)
                set_blk_prop(i + TETRISNAKE_WIDTH, IS_BOND_U, 0);
            set_blk_prop(i, IS_EXIST, 0);
            if ((data->snakeEnd + 1) % MAXLEN != data->snakeBegin)
            {
                Tetrisnake_draw_rect(data->snakeX[data->snakeEnd], data->snakeY[data->snakeEnd], GREEN);
                data->snakeEnd = (data->snakeEnd + 1) % MAXLEN;
                data->score++;
            }
        }
        if(is_a_bound(newX, newY) || is_a_snake(newX, newY))
        {
            game_over();
        }
        Tetrisnake_draw_rect(newX, newY, GREEN);

        data->blockTimer++;
        if(data->blockTimer % TIME_TO_MOVE == 0)
        {
            fall_blocks();
            generate_blocks();
            while(check_lines())
            {
                data->score += POINTS_FOR_LINE;
                cut_snake();
            }
        }
        if(data->blockTimer >= TIME_TO_GENERATE && is_finish_gen() && data->blockTimer % TIME_TO_MOVE == 0)
        {
            generate_brick();
            data->blockTimer = 0;
        }

        if (game_is_button_pressed(ROTLEFT) && !data->rotLeftPressed)
        {
            int newVelX = -data->velY;
            int newVelY = data->velX;
            data->velX = newVelX;
            data->velY = newVelY;
        }
        else if (game_is_button_pressed(ROTRIGHT) && !data->rotRightPressed)
        {
            int newVelX = data->velY;
            int newVelY = -data->velX;
            data->velX = newVelX;
            data->velY = newVelY;
        }
        else if (game_is_button_pressed(LEFT) && data->velY != 0)
        {
            data->velX = -1;
            data->velY = 0;
        }
        else if (game_is_button_pressed(RIGHT) && data->velY != 0)
        {
            data->velX = 1;
            data->velY = 0;
        }
        else if (game_is_button_pressed(UP) && data->velX != 0)
        {
            data->velX = 0;
            data->velY = -1;
        }
        else if (game_is_button_pressed(DOWN) && data->velX != 0)
        {
            data->velX = 0;
            data->velY = 1;
        }
    }
    if (data->phase == PHASE_GAMEOVER)
    {
        if (game_is_any_button_pressed(-1))
        {
            Tetrisnake_reset(); 
        }
    }
    data->rotLeftPressed = game_is_button_pressed(ROTLEFT);
    data->rotRightPressed = game_is_button_pressed(ROTRIGHT);
}

const game_instance Tetrisnake PROGMEM = {
    "Tetrisnake",
    Tetrisnake_prepare,
    Tetrisnake_render,
    Tetrisnake_update,
    sizeof(TetrisnakeData),
    (void**)(&data)
};

#endif
