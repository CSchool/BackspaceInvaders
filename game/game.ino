#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "font.h"
#include "graphics.h"
#include "storage.h"
#include "controls.h"
#include "music.h"

void application_setup();
// update logic (ms since last update), enforced by ups / ticks
void update(unsigned long delta);

static unsigned long ticks;
static unsigned long last_update = 0;

void game_set_ups(int ups)
{
    if (ups)
        ticks = 1000 / ups;
    else
        ticks = 1;
}

void setup()
{
    tune_disable();
    graphics_setup();
    controls_setup();
    storage_init();
    game_set_ups(60);
    application_setup();
    // Enable global interrupts
    sei();
}

void loop()
{
    unsigned long cur_time = millis();

    if ((cur_time - last_update >= ticks)/* && step == 0*/)
    {
        controls_update();
        update(cur_time - last_update);
        last_update = cur_time;
    }
}
