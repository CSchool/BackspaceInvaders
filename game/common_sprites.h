#ifndef _COMMON_SPRITES_H_
#define _COMMON_SPRITES_H_

#include "graphics.h"

#define SPRITE_GAMEOVER 0

#define GAMEOVER_X 16
#define GAMEOVER_Y 24

const game_sprite *game_get_sprite(uint8_t id);

#endif
