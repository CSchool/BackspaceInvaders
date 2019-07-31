#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "storage.h"
#include "graphics.h"
#include "controls.h"
#include "random.h"
#include "menu.h"
#include "music.h"

static const MenuItem main_menu[] PROGMEM = {
#ifdef FRAME_BUFFER
    { "NY2019", NULL},
    { "Demo", &NY2019},
    { "Tetrisnake", &Tetrisnake },
#endif
    { "DoodleJump", &DoodleJump },

    { "\xcb\xca\xd8 2018", NULL },
#ifdef FRAME_BUFFER
    { "Snake2", &Snake2 },
    { "Snail", &Snail },
    { "LodeRunner", &LodeRunner },
#endif
    { "GhostBuster", &Ghostbuster },
    { "SpaceShips", &SpaceShips },
    { "Mario", &Mario},
    { "TicTacToe", &TicTacToe},
/* Not finished    { "artyom", &artyom }, */

    { "\xcb\xca\xd8 2017", NULL },
    { "Flappy", &Flappy },
    { "BreakOut", &BreakOut },
#ifdef FRAME_BUFFER
    { "Saper", &Saper },
#endif

    { "\xcb\xca\xd8 2016", NULL },
    { "Invaders", &BackspaceInvaders },
    { "Snake", &Snake },

    { "Demos", NULL },
    { "3D", &Raycaster },
#ifndef EMULATED /* for use only on real hardware */
    { "Font", &Tester },
    { "EEPROM", &Dump },
    { "Music", &Player },
#endif
#ifdef FRAME_BUFFER
    { "ColorDemo", &ColorDemo},
#endif
    { "SpriteDemo", &SpriteDemo},
    /* Register your game like so:
     *
     * { "YOUR_GAME_NAME", &YOUR_GAME_NAME },
     */
    { "", NULL }
};

#define UP BITMASK(BUTTON_NE) | BITMASK(BUTTON_UP)
#define DOWN BITMASK(BUTTON_SE) | BITMASK(BUTTON_DOWN)
#define SELECT BITMASK(BUTTON_SW) | BITMASK(BUTTON_START)
#define PAUSE BITMASK(BUTTON_SELECT)

// need some space for stack and system variables
#define AVAIL_SPACE 1024

static uint8_t memory[AVAIL_SPACE];

static Menu *menu;
static const game_instance *ptr;
static long btn_timeout;
static bool is_paused;

void pause_continue()
{
    is_paused = false;
    menu_finish(menu);
    menu = NULL;
    btn_timeout = BUTTON_DELAY;
}

void pause_mute()
{
#ifndef EMULATED
    music_mute();
    pause_continue();
#endif
}

void pause_exit()
{
#ifdef EMULATED
#else
    void (*f)() = 0;
    f();
#endif
}

#define CONTINUE ((void*)0xC0)
#define MUTE     ((void*)0x50)
#define EXIT     ((void*)0xE)

static const MenuItem pause_menu[] PROGMEM = {
    { "Continue", CONTINUE },
    { "Mute/Unmute", MUTE },
    { "Exit game", EXIT },
    { "", NULL }
};

void application_setup()
{
    menu = menu_setup(main_menu);
}

void update(unsigned long delta)
{
    if (!ptr)
    {
        ptr = (const game_instance*)menu_update(menu, delta);
        if (ptr && pgm_read_word(&ptr->data_size) <= AVAIL_SPACE)
        {
            menu_finish(menu);
            menu = NULL;
            random_setup();
            // run game
            *(void**)(pgm_read_pointer(&ptr->data)) = memory;
            ((game_function_prepare)pgm_read_pointer(&ptr->prepare))();
        }
        else
        {
            ptr = NULL;
        }
    }
    else
    {
        btn_timeout -= delta;
        if (btn_timeout <= 0)
        {
            btn_timeout = 0;
        }
        if (!is_paused && !btn_timeout && game_is_any_button_pressed(PAUSE))
        {
            is_paused = true;
            menu = menu_setup(pause_menu, 0, 20, BLUE);
            btn_timeout = BUTTON_DELAY;
        }
        else if (is_paused && !btn_timeout && game_is_any_button_pressed(PAUSE))
        {
            pause_continue();
        }
        else if (!is_paused)
        {
            // shouldn't use SELECT in games
            game_reset_buttons(PAUSE);
            ((game_function_update)pgm_read_pointer(&ptr->update))(delta);
        }
        else
        {
            const void *r = menu_update(menu, delta);
            if (r == CONTINUE)
            {
                pause_continue();
            }
            else if (r == MUTE)
            {
                pause_mute();
            }
            else if (r == EXIT)
            {
                pause_exit();
            }
        }
    }
}

void render()
{
    if (ptr && pgm_read_pointer(&ptr->render))
    {
        ((game_function_render)pgm_read_pointer(&ptr->render))();
    }
    if (menu)
    {
        menu_render(menu);
    }
}

//////////////////////////
// Storage
//////////////////////////

void game_save(const void *buf, uint16_t size)
{
    storage_write((const char*)ptr->name, buf, size);
}

void game_load(void *buf, uint16_t size)
{
    storage_read((const char*)ptr->name, buf, size);
}
