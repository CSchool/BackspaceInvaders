#pragma GCC optimize ("-O3")

#include <math.h> 
#include "libgame.h"
#include "graphics.h"
#include "binary.h"
#include "controls.h"

#define DOODLE_WIDTH 5
#define DOODLE_HEIGHT 4
#define DOODLE_JUMP_STR 30
#define PLANKS_MAX_COUNT 30

#define DEBUG_ENABLE false
#define DEBUG(...) do {          \
        if (DEBUG_ENABLE) {      \
            printf(__VA_ARGS__); \
            fflush(stdout);      \
        }                        \
    } while (0)

static const uint8_t spriteLines[] PROGMEM = {
   0x00, 0x37, 0x37, 0x37, 0x00, 0x00, 0x00,
   0x37, 0x33, 0x33, 0x33, 0x37, 0x00, 0x00,
   0x33, 0x33, 0x04, 0x33, 0x04, 0x00, 0x33,
   0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
   0x60, 0x60, 0x60, 0x60, 0x60, 0xff, 0x33,
   0x25, 0x25, 0x25, 0x25, 0x25, 0xff, 0xff,
   0x60, 0xff, 0x60, 0xff, 0x60, 0xff, 0xff,
};

static const game_color_sprite sprite_doodle PROGMEM = {7, 7, spriteLines};

struct Entity {
    uint8_t x;
    uint32_t y;
    uint8_t w;
};

struct Doodle : Entity {
    int8_t jump_counter;
    const game_color_sprite *sprite;
};

struct DoodleJumpData
{
    uint32_t scene_height;
    int32_t scene_motion;
    Doodle doodle;
    Entity planks[PLANKS_MAX_COUNT];
    uint32_t planks_size;
    uint32_t planks_last_gen;
    uint32_t planks_last_gen_complete;
    uint16_t score;
    uint16_t high_score;
};
static DoodleJumpData* data;

static void remove_plank(uint32_t index)
{
    int last = data->planks_size - 1;
    Entity *planks = data->planks;
    Entity *tmp = &planks[index];
    planks[index] = planks[last];
    planks[last] = *tmp;
    data->planks_size--;
}

static void add_plank(uint8_t x, uint32_t y, uint8_t w)
{
    if (data->planks_size >= PLANKS_MAX_COUNT) {
        DEBUG("Add plank failed! %d (%d:%d) \n", data->planks_size, x, y);
        return;
    }

    Entity *plank = &data->planks[data->planks_size];
    data->planks_size++;
    plank->x = x;
    plank->y = y;
    plank->w = w;

    DEBUG("Add plank %d (%d:%d)\n", data->planks_size, plank->x, plank->y);
}

static void add_score(uint16_t value)
{
    data->score += value;
    if (data->score > data->high_score) {
        data->high_score = data->score;
    }
}

static void DoodleJump_reset()
{
    DEBUG("reset game\n");

    data->scene_height = 0;
    data->scene_motion = 0;
    data->planks_size = 0;
    data->planks_last_gen = 0;
    data->planks_last_gen_complete = 0;

    data->doodle.w = DOODLE_WIDTH;
    data->doodle.y = 10;
    data->doodle.x = (WIDTH - DOODLE_WIDTH) / 2;
    data->doodle.jump_counter = 0;
    data->doodle.sprite = &sprite_doodle;

    data->score = 0;

    add_plank(0, 1, WIDTH);
}

static void render_plank(Entity *obj)
{
    int y = HEIGHT - obj->y + data->scene_height;
    game_draw_rect(obj->x, y, obj->w, 2, WHITE);
}

static void render_doodle(Doodle *obj)
{
    if (obj->sprite) {
        int x = obj->x + (obj->w - pgm_read_byte(&obj->sprite->width)) / 2;
        int y = HEIGHT + data->scene_height - obj->y - pgm_read_byte(&obj->sprite->height);
        game_draw_color_sprite(obj->sprite, x, y);
    } else {
        DEBUG("no sprite\n");
    }
}

static void render_score()
{
    int len1 = ceil(log10(data->score + 1));
    int len2 = ceil(log10(data->high_score + 1));
    len1 = len1 < 1 ? 1 : len1;
    len2 = len2 < 1 ? 1 : len2;
    uint8_t color = data->score >= data->high_score ? GREEN : RED;
    game_draw_digits(data->score, len1, 0, 0, GREEN);
    game_draw_digits(data->high_score, len2, WIDTH - len2 - 5, 0, color);
}

static bool collide_with(Entity *src, Entity *target) {
    if (src->y == target->y) {
        int src_x1 = src->x;
        int src_x2 = src->x + src->w;
        int target_x1 = target->x;
        int target_x2 = target->x + target->w;

        return (target_x1 <= src_x1 && src_x1 < target_x2) ||
               (target_x1 < src_x2 && src_x2 <= target_x2) ||
               (src_x1 < target_x1 && target_x1 <= src_x2);
    }
    return false;
}

static void remove_unused_planks(void) {
    for (int i = 0; i < data->planks_size; ++i) {
        Entity *plank = &data->planks[i];

        if (plank->y <= data->scene_height) {
            remove_plank(i);
        }
    }
}

static void generate_planks() {
    int least = data->scene_height + HEIGHT - data->planks_last_gen;
    int planks_width = 10 - sqrt(data->scene_height) / 10;
    int chance = sqrt(data->scene_height) / 5;

    if (chance < 1) {
        chance = 1;
    }

    if (planks_width < 1) {
        planks_width = 1;
    }

    while (least > 0) {
        data->planks_last_gen++;
        if (rand() % chance == 0 ||
           (DOODLE_JUMP_STR / 2 < data->planks_last_gen - data->planks_last_gen_complete)) {
            add_plank(rand() % (64 - planks_width), data->planks_last_gen, planks_width);
            data->planks_last_gen_complete = data->planks_last_gen;
            data->planks_last_gen += 3;
            least -= 3;
        } else {
            least--;
        }
    }
}

static void update_controller()
{
    if (game_is_button_pressed(BUTTON_LEFT)) {
        if (data->doodle.x > 0) {
            data->doodle.x--;
        } else {
            data->doodle.x = WIDTH;
        }
    } else if (game_is_button_pressed(BUTTON_RIGHT)) {
        if (data->doodle.x + data->doodle.w < WIDTH) {
            data->doodle.x++;
        } else {
            data->doodle.x = 0;
        }
    }
}

static void update_entities()
{
    Doodle *doodle = &data->doodle;
    doodle->jump_counter--;

    if (doodle->jump_counter > 0) {
        doodle->y += (2 * doodle->jump_counter + DOODLE_JUMP_STR / 2) / DOODLE_JUMP_STR;
    } else {
        bool collide = false;
        for (int i = 0; i < data->planks_size; ++i) {
            Entity *obj = &data->planks[i];
            if (collide_with(doodle, obj)) {
                collide = true;
                break;
            }
        }

        if (collide) {
            doodle->jump_counter = DOODLE_JUMP_STR;
            data->scene_motion = doodle->y - data->scene_height - 10;
            if (data->scene_motion < 0) {
                data->scene_motion = 0;
            }
        } else {
            doodle->y--;
        }
    }

    if (data->scene_motion > 0) {
        data->scene_motion--;
        int prev = data->scene_height / 10;
        data->scene_height++;
        int curr = data->scene_height / 10;
        int changed = curr - prev;
        if (changed > 0) {
            add_score(changed);
        }
    }
}

static void DoodleJump_prepare()
{
    DoodleJump_reset();
    game_load(&data->high_score, sizeof(data->high_score));
}

static void DoodleJump_render()
{
    Entity *plank;
    for (int i = 0; i < data->planks_size; ++i) {
        render_plank(&data->planks[i]);
    }

    render_doodle(&data->doodle);
    render_score();
}

static void DoodleJump_update(unsigned long delta)
{
    remove_unused_planks();
    generate_planks();
    update_controller();
    update_entities();

    if (data->doodle.y < data->scene_height) {
        DoodleJump_reset();
    }
}

const game_instance DoodleJump PROGMEM = {
    "DoodleJump",
    DoodleJump_prepare,
    DoodleJump_render,
    DoodleJump_update,
    sizeof(DoodleJumpData),
    (void**)(&data)
};
