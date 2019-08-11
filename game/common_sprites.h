#ifndef COMMON_SPRITES_H_
#define COMMON_SPRITES_H_

#include "graphics.h"

enum {
    SPRITE_GAMEOVER,
};

#define GAMEOVER_X 16
#define GAMEOVER_Y 24

const game_sprite *game_get_sprite(uint8_t id);

#endif
