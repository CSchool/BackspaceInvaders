#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "graphics.h"
#include "binary.h"
#include "controls.h"
#include "menu.h"
#include "music.h"
#include "tunes.h"

#ifdef FRAME_BUFFER

#define MAX_GOLD 32
#define MAX_BREAKING 16
#define MAX_ENEMIES 8

#define BLOCK_WIDTH 6
#define BLOCK_HEIGHT 6

#define SCREEN_WIDTH 10
#define SCREEN_HEIGHT 10
#define SCREEN_PADDING_X 2
#define SCREEN_PADDING_Y 4

// how many different levels of breaking (0 - solid, (BREAKING - 1) - almost broken)
#define BREAKING 4

#define ANIM_INTERVAL 100
#define MOVE_INTERVAL 50
#define ENEMY_MOVE_INTERVAL 75
#define DIG_INTERVAL 250

#define REGEN_TIMEOUT 6000
#define FAST_REGEN_TIMEOUT 500
#define CLIMBING_OUT_TIMEOUT 3000

// character states
#define LAND 0
#define LADDER 1
#define ROPE 2

// enemy states
#define DISABLED 0
#define MOVING_LEFT 1
#define MOVING_RIGHT 2
#define MOVING_UP 3
#define MOVING_DOWN 4
#define RESPAWN 5
#define FALLING 6
#define TRAPPED 7
#define CLIMBING_OUT 8
#define FREE_FALLING 9

// game state
#define SELECTING 0
#define PLAYING 1
#define LOST 2

// level definitions

#define N_LEVELS 3

static const MenuItem level_menu[] PROGMEM = {
    { "Level 01", (void*)1 },
    { "Level 02", (void*)2 },
    { "Level 03", (void*)3 },

    { "", NULL }
};

static const uint8_t levels[] PROGMEM {
    // level 01
    "......F..."
    "...RRRFG.."
    "BLBBGBBB.."
    ".L..E...G."
    "BBBBBBLBBB"
    "......LB.."
    "....P.LBRR"
    "G.LBBBBB.G"
    "..L..G...."
    "BBBBBBBBBB"

    // level 02
    ".....PF..."
    "SBBBBBBBSL"
    "SBBBBBBBSL"
    "SG.GBG.GSL"
    "SGGGBGGGSL"
    "SBBBBBBBSL"
    "SBBBBBBBSL"
    "SG.GBG.GSL"
    "SGGGGGGGGL"
    "SSSSSSSSSS"

    // level 03
    "..G..F.G.."
    "LBBBBBBBBL"
    "L.RRR.E..L"
    "LB.G.BBLBB"
    "L..E...L.."
    "LBBBBBBB.G"
    "L.G......."
    "L..RRRRG.."
    "LBBB..BBBL"
    "L....P...L"
};

enum Animation {
    STANDING = 0,
    WALKING0,
    WALKING1,
    HANGING,
    CLIMBING0,
    CLIMBING1,
    ROPECLIMBING0,
    ROPECLIMBING1,

    ANIMATION_MAX
};

#define GAMEOVER_X 16
#define GAMEOVER_Y 24

const uint8_t gameover_lines[] PROGMEM = {
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

const game_sprite gameover_sprite PROGMEM = {
    31, 15, gameover_lines
};

static const uint8_t player_standing_lines[] PROGMEM {
    B00110000,
    B10110100,
    B01111000,
    B00110000,
    B01001000,
    B01001000,
};

static const uint8_t player_walking0_lines[] PROGMEM {
    B00110000,
    B10110100,
    B01111000,
    B00110000,
    B01001000,
    B00001000,
};

static const uint8_t player_walking1_lines[] PROGMEM {
    B00110000,
    B10110100,
    B01111000,
    B00110000,
    B01001000,
    B01000000,
};

static const uint8_t player_hanging_lines[] PROGMEM {
    B10110100,
    B10110100,
    B01111000,
    B00110000,
    B01001000,
    B01001000,
};

static const uint8_t player_climbing0_lines[] PROGMEM {
    B10110000,
    B10110100,
    B01111000,
    B00110000,
    B01001000,
    B01000000,
};

static const uint8_t player_climbing1_lines[] PROGMEM {
    B00110100,
    B10110100,
    B01111000,
    B00110000,
    B01001000,
    B00001000,
};

static const uint8_t player_ropeclimbing0_lines[] PROGMEM {
    B10110000,
    B10110100,
    B01111000,
    B00110000,
    B01001000,
    B01001000,
};

static const uint8_t player_ropeclimbing1_lines[] PROGMEM {
    B00110100,
    B10110100,
    B01111000,
    B00110000,
    B01001000,
    B01001000,
};

const game_sprite player_sprites[ANIMATION_MAX] PROGMEM  {
    {BLOCK_WIDTH, BLOCK_HEIGHT, player_standing_lines},
    {BLOCK_WIDTH, BLOCK_HEIGHT, player_walking0_lines},
    {BLOCK_WIDTH, BLOCK_HEIGHT, player_walking1_lines},
    {BLOCK_WIDTH, BLOCK_HEIGHT, player_hanging_lines},
    {BLOCK_WIDTH, BLOCK_HEIGHT, player_climbing0_lines},
    {BLOCK_WIDTH, BLOCK_HEIGHT, player_climbing1_lines},
    {BLOCK_WIDTH, BLOCK_HEIGHT, player_ropeclimbing0_lines},
    {BLOCK_WIDTH, BLOCK_HEIGHT, player_ropeclimbing1_lines},
};

static const uint8_t ladder_lines[] PROGMEM {
    B10000100,
    B11111100,
    B10000100,
    B10000100,
    B11111100,
    B10000100,
};

const game_sprite ladder_sprite PROGMEM {
    BLOCK_WIDTH,
    BLOCK_HEIGHT,
    ladder_lines,
};

static const uint8_t ladder_partial_lines[] PROGMEM {
    B10000100,
    B10000100,
    B11111100,
    B10000100,
};

const game_sprite ladder_partial_sprite PROGMEM {
    BLOCK_WIDTH,
    SCREEN_PADDING_Y,
    ladder_partial_lines,
};

static const uint8_t brick_lines[] PROGMEM {
    B10111100,
    B10111100,
    B00000000,
    B11110100,
    B11110100,
    B00000000,
};

const game_sprite brick_sprite PROGMEM {
    BLOCK_WIDTH,
    BLOCK_HEIGHT,
    brick_lines,
};

static const uint8_t rope_lines[] PROGMEM {
    B11111100,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
};

const game_sprite rope_sprite PROGMEM {
    BLOCK_WIDTH,
    BLOCK_HEIGHT,
    rope_lines,
};

static const uint8_t gold_lines[] PROGMEM {
    B00000000,
    B00000000,
    B00000000,
    B01111000,
    B01001000,
    B01111000,
};

const game_sprite gold_sprite PROGMEM {
    BLOCK_WIDTH,
    BLOCK_HEIGHT,
    gold_lines,
};

static const uint8_t solid_lines[] PROGMEM {
    B11111100,
    B11111100,
    B11111100,
    B11111100,
    B11111100,
    B11111100,
};

const game_sprite solid_sprite PROGMEM {
    BLOCK_WIDTH,
    BLOCK_HEIGHT,
    solid_lines,
};

static const uint8_t breaking1_lines[] PROGMEM {
    B01111000,
    B00110000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
};

static const uint8_t breaking2_lines[] PROGMEM {
    B11111100,
    B01111000,
    B00110000,
    B00000000,
    B00000000,
    B00000000,
};

static const uint8_t breaking3_lines[] PROGMEM {
    B11111100,
    B11111100,
    B01111000,
    B01111000,
    B00110000,
    B00110000,
};

const game_sprite breaking_sprites[BREAKING - 1] PROGMEM {
    {BLOCK_WIDTH, BLOCK_HEIGHT, breaking1_lines},
    {BLOCK_WIDTH, BLOCK_HEIGHT, breaking2_lines},
    {BLOCK_WIDTH, BLOCK_HEIGHT, breaking3_lines},
};

struct Enemy {
    uint8_t state, anim;
    int8_t x, y;
    int8_t sx, sy;
    unsigned long timeout;
};

struct LodeRunnerData
{
    int8_t player_x, player_y;
    uint8_t level;
    uint8_t level_width, level_height;
    uint8_t level_gold;
    uint8_t *level_ptr;
    uint8_t player_anim;
    uint8_t player_state;

    unsigned long anim_update;
    unsigned long move_update;
    unsigned long dig_update;
    unsigned long enemy_move_update;
    unsigned long time;

    int8_t picked_gold_x[MAX_GOLD];
    int8_t picked_gold_y[MAX_GOLD];
    uint8_t picked_gold;

    int8_t breaking_x[MAX_BREAKING];
    int8_t breaking_y[MAX_BREAKING];
    uint8_t breaking_br[MAX_BREAKING];
    unsigned long breaking_regen[MAX_BREAKING];

    Enemy enemies[MAX_ENEMIES];

    Menu *menu;
    uint8_t state;
};

static LodeRunnerData* data;

static uint8_t get(int8_t x, int8_t y)
{
    if (x < 0 || x >= data->level_width || y < 0 || y >= data->level_height) return 'S';
    return pgm_read_byte(&data->level_ptr[x + y * SCREEN_WIDTH]);
}

static void draw_brick(int8_t x, int8_t y, int8_t br) {
    int8_t sx = SCREEN_PADDING_X + x * BLOCK_WIDTH;
    int8_t sy = SCREEN_PADDING_Y + y * BLOCK_HEIGHT;
    if (br >= BREAKING) {
        game_draw_sprite(&solid_sprite, sx, sy, BLACK);
    } else {
        game_draw_sprite(&solid_sprite, sx, sy, WHITE_DARK);
        game_draw_sprite(&brick_sprite, sx, sy, RED_DARK);
        if (br > 0) {
            game_draw_sprite(&breaking_sprites[br - 1], sx, sy, BLACK);
        }
    }
}

static void dig(int8_t x, int8_t y) {
    for (uint8_t i = 0; i < MAX_BREAKING; ++i) {
        if (data->breaking_x[i] == x && data->breaking_y[i] == y) {
            if (data->breaking_br[i] < BREAKING) {
                data->breaking_br[i]++;
                data->breaking_regen[i] = data->time + FAST_REGEN_TIMEOUT;
                if (data->breaking_br[i] == BREAKING) {
                    data->breaking_regen[i] = data->time + REGEN_TIMEOUT;
                }
                draw_brick(x, y, data->breaking_br[i]);
            }
            return;
        }
    }
    for (uint8_t i = 0; i < MAX_BREAKING; ++i) {
        if (!data->breaking_br[i]) {
            data->breaking_br[i] = 1;
            data->breaking_regen[i] = data->time + FAST_REGEN_TIMEOUT;
            data->breaking_x[i] = x;
            data->breaking_y[i] = y;
            draw_brick(x, y, 1);
            return;
        }
    }
}

static void regen() {
    for (uint8_t i = 0; i < MAX_BREAKING; ++i) {
        if (data->breaking_br[i] && data->breaking_regen[i]) {
            if (data->breaking_regen[i] > data->time) {
                continue;
            }

            if (!--data->breaking_br[i]) {
                data->breaking_regen[i] = 0;
            } else {
                data->breaking_regen[i] = data->time + FAST_REGEN_TIMEOUT;
            }
            draw_brick(data->breaking_x[i], data->breaking_y[i], data->breaking_br[i]);
        }
    }
}

static void draw_map()
{
    game_clear_screen();
    int8_t x;
    int8_t y = SCREEN_PADDING_Y;
    for (int8_t j = 0; j < SCREEN_HEIGHT; ++j) {
        x = SCREEN_PADDING_X;
        for (int8_t i = 0; i < SCREEN_WIDTH; ++i) {
            uint8_t block = get(i, j);
            const game_sprite *spr = NULL;
            uint8_t color;
            uint8_t bg = 0xff;
            switch (block) {
                case 'B':
                    spr = &brick_sprite;
                    color = RED_DARK;
                    bg = WHITE_DARK;
                    break;

                case 'S':
                    spr = &solid_sprite;
                    color = RED_DARK;
                    break;

                case 'R':
                    spr = &rope_sprite;
                    color = WHITE;
                    break;

                case 'L':
                    spr = &ladder_sprite;
                    color = WHITE;
                    break;

                case 'F':
                    if (data->level_gold == data->picked_gold) {
                        spr = &ladder_sprite;
                        color = CYAN;
                        if (j == 0) {
                            game_draw_sprite(&ladder_partial_sprite, x, 0, color);
                        }
                    }
                    break;

                case 'G':
                    spr = &gold_sprite;
                    color = YELLOW;
                    break;
            }
            if (bg != 0xff) {
                game_draw_sprite(&solid_sprite, x, y, bg);
            }
            if (spr) {
                game_draw_sprite(spr, x, y, color);
            }
            x += BLOCK_WIDTH;
        }
        y += BLOCK_HEIGHT;
    }
    for (uint8_t i = 0; i < data->picked_gold; ++i) {
        int8_t bx = data->picked_gold_x[i];
        int8_t by = data->picked_gold_y[i];
        game_draw_sprite(&solid_sprite, SCREEN_PADDING_X + BLOCK_WIDTH * bx, SCREEN_PADDING_Y + BLOCK_HEIGHT * by, BLACK);
    }
    for (uint8_t i = 0; i < MAX_BREAKING; ++i) {
        if (data->breaking_br[i]) {
            draw_brick(data->breaking_x[i], data->breaking_y[i], data->breaking_br[i]);
        }
    }
}

static bool wall_at_impl(int8_t x, int8_t y, uint8_t br, bool ignore)
{
    int8_t bx = x / BLOCK_WIDTH;
    int8_t by = y / BLOCK_HEIGHT;
    if (x % BLOCK_WIDTH != 0) {
        uint8_t c = get(bx + 1, by);
        if (c == 'S') {
            return true;
        }

        if (c == 'B') {
            bool flag = true;
            for (uint8_t i = 0; i < MAX_BREAKING; ++i) {
                if (data->breaking_x[i] == bx + 1 && data->breaking_y[i] == by) {
                    if (data->breaking_br[i] >= br) {
                        flag = false;
                        break;
                    }
                }
            }
            if (flag) {
                return true;
            }
        }

        if (!ignore) {
            for (uint8_t i = 0; i < MAX_ENEMIES; ++i) {
                if (data->enemies[i].state == TRAPPED) {
                    if ((bx + 1 == data->enemies[i].x / BLOCK_WIDTH) && (by == data->enemies[i].y / BLOCK_HEIGHT)) {
                        return true;
                    }
                }
            }
        }
    }
    uint8_t c = get(bx, by);
    if (c == 'S') {
        return true;
    }

    if (c == 'B') {
        bool flag = true;
        for (uint8_t i = 0; i < MAX_BREAKING; ++i) {
            if (data->breaking_x[i] == bx && data->breaking_y[i] == by) {
                if (data->breaking_br[i] >= br) {
                    flag = false;
                    break;
                }
            }
        }
        if (flag) {
            return true;
        }
    }

    if (!ignore) {
        for (uint8_t i = 0; i < MAX_ENEMIES; ++i) {
            if (data->enemies[i].state == TRAPPED) {
                if ((bx == data->enemies[i].x / BLOCK_WIDTH) && (by == data->enemies[i].y / BLOCK_HEIGHT)) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

static inline bool wall_at(int8_t x, int8_t y) {
    return wall_at_impl(x, y, BREAKING, false);
}

static bool ladder_at(int8_t x, int8_t y)
{
    int8_t bx = x / BLOCK_WIDTH;
    int8_t by = y / BLOCK_HEIGHT;
    if (x % BLOCK_WIDTH != 0) {
        uint8_t c = get(bx + 1, by);
        if (c == 'L' || (c == 'F' && data->level_gold == data->picked_gold))
            return true;
    }
    uint8_t c = get(bx, by);
    if (c == 'L' || (c == 'F' && data->level_gold == data->picked_gold))
        return true;
    return false;
}

static void nearest_ladder(int8_t x, int8_t y)
{
    int8_t bx = (x + 3) / BLOCK_WIDTH;
    int8_t by = y / BLOCK_HEIGHT;
    uint8_t c = get(bx, by);
    if (c == 'L' || (c == 'F' && data->level_gold == data->picked_gold)) {
        data->player_x = bx * BLOCK_WIDTH;
        data->player_state = LADDER;
        data->player_anim = CLIMBING0;
        return;
    }
}

static void pick_gold()
{
    int8_t bx = (data->player_x + 3) / BLOCK_WIDTH;
    int8_t by = (data->player_y + 2) / BLOCK_HEIGHT;
    uint8_t c = get(bx, by);
    if (c == 'G') {
        for (uint8_t i = 0; i < data->picked_gold; ++i) {
            if (data->picked_gold_x[i] == bx && data->picked_gold_y[i] == by) {
                return;
            }
        }
        data->picked_gold_x[data->picked_gold] = bx;
        data->picked_gold_y[data->picked_gold] = by;
        data->picked_gold++;
        if (data->picked_gold != data->level_gold) {
            game_draw_sprite(&solid_sprite, SCREEN_PADDING_X + BLOCK_WIDTH * bx, SCREEN_PADDING_Y + BLOCK_HEIGHT * by, BLACK);
        } else {
            draw_map();
        }
        return;
    }
}

static bool collide(int8_t x, int8_t y)
{
    if (x < 0) return true;
    if (x + BLOCK_WIDTH > data->level_width * BLOCK_WIDTH) return true; 
    if (wall_at(x, y + BLOCK_HEIGHT - 1)) return true;
    return false;
}

static void init_level()
{
    data->level_width = SCREEN_WIDTH;
    data->level_height = SCREEN_HEIGHT;
    data->picked_gold = 0;
    data->player_state = LAND;
    data->player_anim = STANDING;
    data->level_gold = 0;
    data->level_ptr = (uint8_t*)levels;
    data->level_ptr += SCREEN_WIDTH * SCREEN_HEIGHT * data->level;
    for (uint8_t i = 0; i < MAX_BREAKING; ++i) {
        data->breaking_br[i] = 0;
    }

    for (uint8_t i = 0; i < MAX_ENEMIES; ++i) {
        data->enemies[i].timeout = 0;
        data->enemies[i].state = DISABLED;
    }

    uint8_t enemies = 0;

    for (int8_t i = 0; i < data->level_width; ++i) {
        for (int8_t j = 0; j < data->level_height; ++j) {
            if (get(i, j) == 'P') {
                data->player_x = BLOCK_WIDTH * i;
                data->player_y = BLOCK_HEIGHT * j;
            }

            if (get(i, j) == 'G') {
                data->level_gold++;
            }

            if (get(i, j) == 'E') {
                data->enemies[enemies].state = RESPAWN;
                data->enemies[enemies].anim = STANDING;
                data->enemies[enemies].x = data->enemies[enemies].sx = BLOCK_WIDTH * i;
                data->enemies[enemies].y = data->enemies[enemies].sy = BLOCK_HEIGHT * j;
                enemies++;
            }
        }
    }
    draw_map();
}

static bool can_move()
{
    return (data->player_state == ROPE ||
            collide(data->player_x, data->player_y + 1) ||
            ladder_at(data->player_x, data->player_y + BLOCK_HEIGHT));
}

static void LodeRunner_prepare()
{
    game_enable_frame_buffer();
    data->level = 0;

    data->anim_update = 0;
    data->move_update = 0;
    data->time = 0;
    data->menu = menu_setup(level_menu);
    data->state = SELECTING;

#ifndef EMULATED
    tune_init(cantina_addr());
    tune_enable();
#endif
}

static void LodeRunner_render()
{
    if (data->state == SELECTING) {
        menu_render(data->menu);
    }
    if (data->state == PLAYING) { 
        game_draw_sprite(&player_sprites[data->player_anim],
                        data->player_x + SCREEN_PADDING_X,
                        data->player_y + SCREEN_PADDING_Y, GREEN);

        for (uint8_t i = 0; i < MAX_ENEMIES; ++i) {
            if (data->enemies[i].state) {
                game_draw_sprite(&player_sprites[data->enemies[i].anim],
                                data->enemies[i].x + SCREEN_PADDING_X,
                                data->enemies[i].y + SCREEN_PADDING_Y, RED);
            }
        }
    }
}

static void enemy_select_direction(Enemy *e)
{
    if (wall_at(e->x - BLOCK_WIDTH, e->y) && wall_at(e->x + BLOCK_WIDTH, e->y))
        return;
    e->anim = STANDING;
    if (data->player_x < e->x) {
        e->state = MOVING_LEFT;
    } else {
        e->state = MOVING_RIGHT;
    }

    if (e->state == MOVING_LEFT && wall_at(e->x - BLOCK_WIDTH, e->y)) {
        e->state = MOVING_RIGHT;
    }

    if (e->state == MOVING_RIGHT && wall_at(e->x + BLOCK_WIDTH, e->y)) {
        e->state = MOVING_LEFT;
    }
}

static inline bool is_transparent(uint8_t c) {
    return (c == '.' || c == 'G' || c == 'P' || c == 'E' || (c == 'F' && data->picked_gold != data->level_gold));
}

static void LodeRunner_update(unsigned long delta)
{
    if (game_is_button_pressed(BUTTON_START)) {
        game_clear_screen();
        data->state = SELECTING;
        return;
    }

    if (data->state == SELECTING) {
        ptrdiff_t p = (ptrdiff_t)menu_update(data->menu, delta);
        if (p)
        {
            data->level = (uint8_t)(p - 1);
            init_level();
            data->state = PLAYING;
        }
        return;
    }

    if (data->state == LOST) {
        return;
    }

    bool left = game_is_button_pressed(BUTTON_LEFT);
    bool right = game_is_button_pressed(BUTTON_RIGHT);
    bool up = game_is_button_pressed(BUTTON_UP);
    bool down = game_is_button_pressed(BUTTON_DOWN);
    bool dig_left = game_is_button_pressed(BUTTON_B);
    bool dig_right = game_is_button_pressed(BUTTON_A);

    data->time += delta;

    bool do_anim = false;
    bool do_move = false;
    bool do_enemy_move = false;
    bool do_dig = false;

    if (data->time - data->anim_update > ANIM_INTERVAL) {
        data->anim_update = data->time;
        do_anim = true;
    }
    
    if (data->time - data->move_update > MOVE_INTERVAL) {
        do_move = true;
        data->move_update = data->time;
    }

    if (data->time - data->enemy_move_update > ENEMY_MOVE_INTERVAL) {
        do_enemy_move = true;
        data->enemy_move_update = data->time;
    }

    if (data->time - data->dig_update > DIG_INTERVAL) {
        do_dig = true;
    }

    if (do_dig) {
        if (dig_left ^ dig_right) {
            int8_t y = (data->player_y + BLOCK_HEIGHT) / BLOCK_HEIGHT;
            int8_t x;
            if (dig_left) {
                x = (data->player_x - 3) / BLOCK_WIDTH;
            } else {
                x = (data->player_x + BLOCK_WIDTH + 3) / BLOCK_WIDTH;
            }
            uint8_t b = get(x, y);
            if (b == 'B' && !wall_at(x * BLOCK_WIDTH, (y - 1) * BLOCK_HEIGHT)) {
                data->dig_update = data->time;
                dig(x, y);
            }
        }
    }

    if (do_move) {
        if (data->player_state != ROPE) {
            if (!collide(data->player_x, data->player_y + 1) && !ladder_at(data->player_x, data->player_y + BLOCK_HEIGHT)) {
                data->player_y++;
            }
        }

        if (can_move() && (up ^ down)) {
            if (up) {
                if (data->player_state != LADDER) {
                    nearest_ladder(data->player_x, data->player_y);
                }

                if (data->player_state == LADDER) {
                    if (data->player_anim == CLIMBING0) data->player_anim = CLIMBING1;
                    else if (data->player_anim == CLIMBING1) data->player_anim = CLIMBING0;
                    data->player_y--;
                    if (!ladder_at(data->player_x, data->player_y + BLOCK_HEIGHT - 1)) {
                        data->player_state = LAND;
                        data->player_anim = STANDING;
                    }
                }
            } else {
                if (data->player_state != LADDER) {
                    nearest_ladder(data->player_x, data->player_y + BLOCK_HEIGHT);
                }

                if (data->player_state == LADDER) {
                    if (data->player_anim == CLIMBING0) data->player_anim = CLIMBING1;
                    else if (data->player_anim == CLIMBING1) data->player_anim = CLIMBING0;
                    if (!ladder_at(data->player_x, data->player_y + BLOCK_HEIGHT)) {
                        data->player_state = LAND;
                        data->player_anim = STANDING;
                    } else {
                        data->player_y++;
                    }
                }

                if (data->player_state == ROPE) {
                    data->player_state = LAND;
                    data->player_anim = STANDING;
                    if (!collide(data->player_x, data->player_y + 1) && !ladder_at(data->player_x, data->player_y + BLOCK_HEIGHT)) {
                        data->player_y++;
                    }
                }
            }
        }
    }

    if (left ^ right) {
        if (data->player_state == LAND) {
            if (do_anim) {
                if (data->player_anim == WALKING0) data->player_anim = WALKING1;
                else if (data->player_anim == WALKING1) data->player_anim = WALKING0;
            }

            if (do_move && can_move()) {
                if (data->player_anim == STANDING) {
                    data->player_anim = WALKING0;
                }
                if (left) {
                    if (!collide(data->player_x - 1, data->player_y)) {
                        data->player_x--;
                    }
                } else {
                    if (!collide(data->player_x + 1, data->player_y)) {
                        data->player_x++;
                    }
                }
            }
        }

        if (data->player_state == LADDER) {
            if (left) {
                if (!collide(data->player_x - 1, data->player_y) && !collide(data->player_x - 1, data->player_y - BLOCK_HEIGHT + 1)) {
                    data->player_x--;
                    data->player_state = LAND;
                    data->player_anim = WALKING0;
                }
            } else {
                if (!collide(data->player_x + 1, data->player_y) && !collide(data->player_x + 1, data->player_y - BLOCK_HEIGHT + 1)) {
                    data->player_x++;
                    data->player_state = LAND;
                    data->player_anim = WALKING0;
                }
            }
        }

        if (data->player_state == ROPE) {
            if (do_anim) {
                if (data->player_anim == ROPECLIMBING0) data->player_anim = ROPECLIMBING1;
                else if (data->player_anim == ROPECLIMBING1) data->player_anim = ROPECLIMBING0;
            }

            if (do_move) {
                if (data->player_anim == HANGING) {
                    data->player_anim = ROPECLIMBING0;
                }
                if (left) {
                    if (!collide(data->player_x - 1, data->player_y)) {
                        data->player_x--;
                    }
                } else {
                    if (!collide(data->player_x + 1, data->player_y)) {
                        data->player_x++;
                    }
                }
            }
        }
    }

    if (data->player_state == LAND && !(left ^ right)) {
        data->player_anim = STANDING;
    }

    if (data->player_state != ROPE) {
        int8_t bx = data->player_x / BLOCK_WIDTH;
        int8_t bx2 = (data->player_x + BLOCK_WIDTH - 1) / BLOCK_WIDTH;
        int8_t by = data->player_y / BLOCK_HEIGHT;
        if (by * BLOCK_HEIGHT == data->player_y && get(bx, by) == 'R' && get(bx2, by) == 'R') {
            data->player_state = ROPE;
            data->player_anim = HANGING;
        }
    } else {
        int8_t bx = data->player_x / BLOCK_WIDTH;
        int8_t bx2 = (data->player_x + BLOCK_WIDTH - 1) / BLOCK_WIDTH;
        int8_t by = data->player_y / BLOCK_HEIGHT;
        if (by * BLOCK_HEIGHT == data->player_y && (get(bx, by) != 'R' || get(bx2, by) != 'R')) {
            data->player_state = LAND;
            data->player_anim = STANDING;
        }
    }

    if (do_move) {
        pick_gold();
        if (data->player_y == -SCREEN_PADDING_Y) {
            data->level++;
            if (data->level == N_LEVELS) {
                data->level = 0;
                game_clear_screen();
                data->state = SELECTING;
                return;
            }
            init_level();
        }

        if (wall_at_impl(data->player_x, data->player_y, 1, true) || 
                wall_at_impl(data->player_x, data->player_y + BLOCK_HEIGHT - 1, 1, true)) {
            data->state = LOST;
            game_draw_sprite(&gameover_sprite, GAMEOVER_X, GAMEOVER_Y, WHITE);
            return;
        }
    }

    if (do_enemy_move) {
        for (uint8_t i = 0; i < MAX_ENEMIES; ++i) {
            Enemy e = data->enemies[i];
            int8_t bx = e.x / BLOCK_WIDTH;
            int8_t by = e.y / BLOCK_HEIGHT;
            if (!e.state) continue;

            if (e.state == TRAPPED && e.timeout < data->time && e.timeout) {
                e.state = CLIMBING_OUT;
                e.timeout = 0;
                e.anim = CLIMBING0;
                goto cnt;
            }

            if (e.state == RESPAWN) {
                e.x = e.sx;
                e.y = e.sy;
                e.timeout = 0;
                enemy_select_direction(&e);
            }

            if (bx * BLOCK_WIDTH == e.x && by * BLOCK_HEIGHT == e.y) {
                if (e.state == TRAPPED) {
                    if (wall_at_impl(e.x, e.y, 1, true)) {
                        e.state = RESPAWN;
                    }
                    goto cnt;
                }

                if (e.state == CLIMBING_OUT) {
                    enemy_select_direction(&e);
                    goto cnt;
                }

                if (e.state == FALLING) {
                    e.state = TRAPPED;
                    e.timeout = data->time + CLIMBING_OUT_TIMEOUT;
                    goto cnt;
                }

                if (e.state == FREE_FALLING) {
                    if (wall_at(e.x, e.y + BLOCK_HEIGHT)) {
                        enemy_select_direction(&e);
                    }
                }

                if (!wall_at(e.x, e.y + BLOCK_HEIGHT) && !ladder_at(e.x, e.y + BLOCK_HEIGHT)) {
                    if (get(bx, by) != 'R') {
                        if (is_transparent(get(bx, by + 1))) {
                            e.state = FREE_FALLING;
                        } else {
                            e.state = FALLING;
                        }
                    } else {
                        e.anim = ROPECLIMBING0;
                    }
                    goto cnt;
                } else {
                    if (e.state == MOVING_LEFT || e.state == MOVING_RIGHT) {
                        e.anim = STANDING;
                    }
                }

                if (e.state == MOVING_LEFT || e.state == MOVING_RIGHT) {
                    if ((get(bx, by) == 'R') && !wall_at(e.x, e.y + BLOCK_HEIGHT)) {
                        if (data->player_y > e.y) {
                            e.state = FREE_FALLING;
                            e.anim = STANDING;
                            goto cnt;
                        }
                    }

                    if (ladder_at(e.x, e.y + BLOCK_HEIGHT)) {
                        if (data->player_y > e.y) {
                            e.state = MOVING_DOWN;
                            e.anim = CLIMBING0;
                            goto cnt;
                        }
                    }
                    if (ladder_at(e.x, e.y)) {
                        if (data->player_y < e.y) {
                            e.state = MOVING_UP;
                            e.anim = CLIMBING0;
                            goto cnt;
                        }
                    }

                    if (e.state == MOVING_LEFT) {
                        if (wall_at(e.x - BLOCK_WIDTH, e.y)) {
                            e.state = MOVING_RIGHT;
                            e.anim = STANDING;
                        }
                    } else {
                        if (wall_at(e.x + BLOCK_WIDTH, e.y)) {
                            e.state = MOVING_LEFT;
                            e.anim = STANDING;
                        }
                    }
                }
                if (e.state == MOVING_DOWN || e.state == MOVING_UP) {
                    if (e.state == MOVING_DOWN) {
                        if (e.y == data->player_y || !ladder_at(e.x, e.y + BLOCK_HEIGHT)) {
                            enemy_select_direction(&e);
                        }
                    } else {
                        if (e.y == data->player_y || !ladder_at(e.x, e.y)) {
                            enemy_select_direction(&e);
                        }
                    }
                }
            }
cnt:;
            switch (e.state) {
                case MOVING_LEFT:
                    e.x--;
                    break;
                case MOVING_RIGHT:
                    e.x++;
                    break;
                case MOVING_UP:
                case CLIMBING_OUT:
                    e.y--;
                    break;
                case MOVING_DOWN:
                case FALLING:
                case FREE_FALLING:
                    e.y++;
                    break;
            }

            if (e.state == FALLING || e.state == FREE_FALLING || e.state == TRAPPED) {
                e.anim = STANDING;
            }

            if (e.state != TRAPPED) {
                if (e.anim == STANDING) e.anim = WALKING0;
                else if (e.anim == WALKING0) e.anim = WALKING1;
                else if (e.anim == WALKING1) e.anim = WALKING0;
                else if (e.anim == CLIMBING0) e.anim = CLIMBING1;
                else if (e.anim == CLIMBING1) e.anim = CLIMBING0;
                else if (e.anim == ROPECLIMBING0) e.anim = ROPECLIMBING1;
                else if (e.anim == ROPECLIMBING1) e.anim = ROPECLIMBING0;
            }

            data->enemies[i] = e;
        }
    }

    if (do_dig) {
        regen();
    }

    if (do_move || do_enemy_move) {
        for (uint8_t i = 0; i < MAX_ENEMIES; ++i) {
            Enemy e = data->enemies[i];
            if (!e.state) continue;

            int8_t dx = e.x - data->player_x;
            int8_t dy = e.y - data->player_y;
            if (dx < 0) dx = -dx;
            if (dy < 0) dy = -dy;
            if (dx < BLOCK_WIDTH && dy < BLOCK_HEIGHT) {
                data->state = LOST;
                game_draw_sprite(&gameover_sprite, GAMEOVER_X, GAMEOVER_Y, WHITE);
                return;
            }
        }
        
 
    }
}

const game_instance LodeRunner PROGMEM {
    "LodeRunner",
    LodeRunner_prepare,
    LodeRunner_render,
    LodeRunner_update,
    sizeof(LodeRunnerData),
    (void**)(&data)
};

#endif
