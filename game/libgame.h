#ifndef LIBGAME_H
#define LIBGAME_H

#include "config.h"

#define WIDTH  64
#define HEIGHT 64

typedef void(*game_function_prepare)();
typedef void(*game_function_render)();
typedef void(*game_function_update)(unsigned long);

struct game_instance
{
    unsigned char name[11];
    game_function_prepare prepare;
    game_function_render render;
    game_function_update update;
    uint16_t data_size;
    void** data;
};

/* List of all games */

extern const game_instance BackspaceInvaders PROGMEM;
extern const game_instance Snake PROGMEM;
#ifdef FRAME_BUFFER
extern const game_instance Snake2 PROGMEM;
extern const game_instance Snail PROGMEM;
#endif
extern const game_instance Ghostbuster PROGMEM;
extern const game_instance Flappy PROGMEM;
extern const game_instance SpaceShips PROGMEM;
extern const game_instance Tester PROGMEM;
extern const game_instance Raycaster PROGMEM;
extern const game_instance BreakOut PROGMEM;
extern const game_instance Saper PROGMEM;
extern const game_instance Mario PROGMEM;
#ifndef EMULATED /* for use only on real hardware */
extern const game_instance Dump PROGMEM;
extern const game_instance Player PROGMEM;
#endif
/* Register your game like so:
 *
 * extern game_instance YOUR_GAME_NAME;
 */

void game_set_ups(int ups);
void game_setup();

void game_save(const void *buf, uint16_t size);
void game_load(void *buf, uint16_t size);

#endif 
