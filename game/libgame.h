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

/* LKSH 2016 */
extern const game_instance BackspaceInvaders PROGMEM;

extern const game_instance Snake PROGMEM;
extern const game_instance Raycaster PROGMEM;

/* LKSH 2017 */
extern const game_instance Flappy PROGMEM;
extern const game_instance BreakOut PROGMEM;
#ifdef FRAME_BUFFER
extern const game_instance Saper PROGMEM;
#endif

/* LKSH 2018 */
#ifdef FRAME_BUFFER
extern const game_instance Snake2 PROGMEM;
extern const game_instance Snail PROGMEM;
extern const game_instance LodeRunner PROGMEM;
#endif
extern const game_instance SpaceShips PROGMEM;
extern const game_instance Mario PROGMEM;
/*extern const game_instance Minecraft PROGMEM;*/
extern const game_instance Ghostbuster PROGMEM;
/*extern const game_instance artyom;*/

/* NY2019 mini-hackaton */
#ifdef FRAME_BUFFER
extern const game_instance NY2019 PROGMEM;
extern const game_instance Tetrisnake PROGMEM;
#endif
extern const game_instance DoodleJump PROGMEM;

/* LKSH 2019 */
#ifdef FRAME_BUFFER
extern const game_instance Maze3D PROGMEM;
extern const game_instance VVVV PROGMEM;
#endif
extern const game_instance BrainF PROGMEM;
extern const game_instance TicTacToe PROGMEM;
extern const game_instance Pacman PROGMEM;

/* Test applications */
#ifndef EMULATED
extern const game_instance Dump PROGMEM;
extern const game_instance Player PROGMEM;
#endif
extern const game_instance Tester PROGMEM;
#ifdef FRAME_BUFFER
extern const game_instance ColorDemo PROGMEM;
extern const game_instance SpriteDemo PROGMEM;
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
