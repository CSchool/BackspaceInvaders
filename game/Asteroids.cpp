#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "graphics.h"
#include "binary.h"
#include "controls.h"
#include "music.h"
#include "tunes.h"

#define INITIAL_ASTEROIDS 1
#define MAX_ASTEROIDS 16
#define MAX_BULLETS 8
#define MAX_PARTICLES 32
#define ASTEROID_TYPES 4
#define PLAYER_ROTATIONS 16
#define FLAME_COLORS 4
#define PARTICLE_TTL 200
#define VELF 128
#define PARTICLE_DELAY 30
#define BULLET_DELAY 200
#define BULLET_TTL 3000
#define BULLET_SPEED 4

#define PLAYER_DECELERATION 100
#define PLAYER_ACCELERATION 3
#define PLAYER_ROT_DECELERATION 100
#define PLAYER_ROT_ACCELERATION 40

static const uint8_t asteroid1_lines[] PROGMEM {
    B01100000,
    B10100000,
    B10010000,
    B01100000,
};
#define ASTEROID1_WIDTH 4
#define ASTEROID1_HEIGHT 4

static const uint8_t asteroid2_lines[] PROGMEM {
    B01110000,
    B10001000,
    B10011000,
    B01010000,
    B01100000,
};
#define ASTEROID2_WIDTH 5
#define ASTEROID2_HEIGHT 5

static const uint8_t asteroid3_lines[] PROGMEM {
    B00111000,
    B01001110,
    B10000011,
    B10000001,
    B10000001,
    B11001111,
    B01111000,
};
#define ASTEROID3_WIDTH 8
#define ASTEROID3_HEIGHT 7

static const uint8_t asteroid4_lines[] PROGMEM {
    B00000001, B11000000,
    B00111110, B00110000,
    B01101100, B00010000,
    B01000000, B00110000,
    B01000000, B00100000,
    B01000000, B00110000,
    B11000000, B00010000,
    B11000000, B00110000,
    B00110000, B00100000,
    B00011110, B11000000,
    B00000011, B10000000,
};
#define ASTEROID4_WIDTH 12
#define ASTEROID4_HEIGHT 11

const game_sprite asteroid_sprites[ASTEROID_TYPES] PROGMEM {
    {ASTEROID1_WIDTH, ASTEROID1_HEIGHT, asteroid1_lines},
    {ASTEROID2_WIDTH, ASTEROID2_HEIGHT, asteroid2_lines},
    {ASTEROID3_WIDTH, ASTEROID3_HEIGHT, asteroid3_lines},
    {ASTEROID4_WIDTH, ASTEROID4_HEIGHT, asteroid4_lines},
};

static bool check_collision(uint8_t type, uint8_t ax, uint8_t ay, uint8_t x, uint8_t y, uint8_t r = 0) {
    switch (type) {
        case 1: ax += 1; ay += 1; r += 2; break;
        case 2: ax += 2; ay += 2; r += 3; break;
        case 3: ax += 4; ay += 4; r += 4; break;
        case 4: ax += 6; ay += 6; r += 7; break;
    }

    return abs(ax - x) + abs(ay - y) <= r;
}

#define PLAYER_WIDTH 7
#define PLAYER_HEIGHT 7
#define PLAYER_CENTER_X 3
#define PLAYER_CENTER_Y 3

static const uint8_t player0_lines[] PROGMEM {
    B00000000,
    B11110000,
    B10001100,
    B01000010,
    B10001100,
    B11110000,
    B00000000,
};

static const uint8_t player1_lines[] PROGMEM {
    B00000000,
    B11111110,
    B10000010,
    B01100100,
    B00101000,
    B00110000,
    B00000000,
};

static const uint8_t player2_lines[] PROGMEM {
    B00011110,
    B01100010,
    B10000010,
    B01110010,
    B00010100,
    B00010100,
    B00001000,
};

static const uint8_t player3_lines[] PROGMEM {
    B00001100,
    B00010100,
    B00100100,
    B01000100,
    B01110100,
    B00010100,
    B00001100,
};

static const uint8_t player4_lines[] PROGMEM {
    B00010000,
    B00101000,
    B00101000,
    B01000100,
    B01000100,
    B01010100,
    B01101100,
};

static const uint8_t player5_lines[] PROGMEM {
    B01100000,
    B01010000,
    B01001000,
    B01000100,
    B01011100,
    B01010000,
    B01100000,
};

static const uint8_t player6_lines[] PROGMEM {
    B11110000,
    B10001100,
    B10000010,
    B10011100,
    B01010000,
    B01010000,
    B00100000,
};

static const uint8_t player7_lines[] PROGMEM {
    B00000000,
    B11111110,
    B10000010,
    B01001100,
    B00101000,
    B00011000,
    B00000000,
};

static const uint8_t player8_lines[] PROGMEM {
    B00000000,
    B00011110,
    B01100010,
    B10000100,
    B01100010,
    B00011110,
    B00000000,
};

static const uint8_t player9_lines[] PROGMEM {
    B00000000,
    B00011000,
    B00101000,
    B01001100,
    B10000010,
    B11111110,
    B00000000,
};

static const uint8_t player10_lines[] PROGMEM {
    B00100000,
    B01010000,
    B01010000,
    B10011100,
    B10000010,
    B10001100,
    B11110000,
};

static const uint8_t player11_lines[] PROGMEM {
    B01100000,
    B01010000,
    B01011100,
    B01000100,
    B01001000,
    B01010000,
    B01100000,
};

static const uint8_t player12_lines[] PROGMEM {
    B01101100,
    B01010100,
    B01000100,
    B01000100,
    B00101000,
    B00101000,
    B00010000,
};

static const uint8_t player13_lines[] PROGMEM {
    B00001100,
    B00010100,
    B01110100,
    B01000100,
    B00100100,
    B00010100,
    B00001100,
};

static const uint8_t player14_lines[] PROGMEM {
    B00001000,
    B00010100,
    B00010100,
    B01110010,
    B10000010,
    B01100010,
    B00011110,
};

static const uint8_t player15_lines[] PROGMEM {
    B00000000,
    B00110000,
    B00101000,
    B01100100,
    B10000010,
    B11111110,
    B00000000,
};

const game_sprite player_sprites[PLAYER_ROTATIONS] PROGMEM {
    {PLAYER_WIDTH, PLAYER_HEIGHT, player0_lines},
    {PLAYER_WIDTH, PLAYER_HEIGHT, player1_lines},
    {PLAYER_WIDTH, PLAYER_HEIGHT, player2_lines},
    {PLAYER_WIDTH, PLAYER_HEIGHT, player3_lines},
    {PLAYER_WIDTH, PLAYER_HEIGHT, player4_lines},
    {PLAYER_WIDTH, PLAYER_HEIGHT, player5_lines},
    {PLAYER_WIDTH, PLAYER_HEIGHT, player6_lines},
    {PLAYER_WIDTH, PLAYER_HEIGHT, player7_lines},
    {PLAYER_WIDTH, PLAYER_HEIGHT, player8_lines},
    {PLAYER_WIDTH, PLAYER_HEIGHT, player9_lines},
    {PLAYER_WIDTH, PLAYER_HEIGHT, player10_lines},
    {PLAYER_WIDTH, PLAYER_HEIGHT, player11_lines},
    {PLAYER_WIDTH, PLAYER_HEIGHT, player12_lines},
    {PLAYER_WIDTH, PLAYER_HEIGHT, player13_lines},
    {PLAYER_WIDTH, PLAYER_HEIGHT, player14_lines},
    {PLAYER_WIDTH, PLAYER_HEIGHT, player15_lines},
};

const int16_t direction_x[PLAYER_ROTATIONS] PROGMEM {
    256, 237, 181, 98, 0, -98, -181, -237, -256, -237, -181, -98, 0, 98, 181, 237
};

const int16_t direction_y[PLAYER_ROTATIONS] PROGMEM {
    0, -98, -181, -237, -256, -237, -181, -98, 0, 98, 181, 237, 256, 237, 181, 98
};

struct Particle {
    uint16_t x, y;
    int16_t vx, vy;
    uint8_t state;
    uint16_t ttl;
};

struct Asteroid {
    uint16_t x, y;
    int16_t vx, vy;
    uint8_t type;
};

struct Bullet {
    uint16_t x, y;
    int16_t vx, vy;
    uint16_t ttl;
};

struct Spaceship {
    uint16_t x, y;
    int16_t vx, vy;
    uint16_t r;
    uint8_t ri, vi;
};

struct AsteroidsData {
    Spaceship player;
    Asteroid asteroids[MAX_ASTEROIDS];
    Particle particles[MAX_PARTICLES];
    Bullet bullets[MAX_BULLETS];
    uint8_t next_particle;
    uint8_t next_bullet;
};
static AsteroidsData* data;

inline uint16_t u16inv(uint16_t x) {
    return 65535 / x;
}

static void Asteroids_prepare() {
    data->next_particle = 0;
    data->next_bullet = 0;
    data->player.x = 32 * 256;
    data->player.y = 32 * 256;
    data->player.vx = data->player.vy = 0;
    data->player.r = 0;
    data->player.ri = 0;
    data->player.vi = 0;

    for (uint8_t i = 0; i < MAX_PARTICLES; ++i) {
        data->particles[i].state = 0;
    }

    for (uint8_t i = 0; i < MAX_ASTEROIDS; ++i) {
        data->asteroids[i].type = 0;
    }

    for (uint8_t i = 0; i < MAX_BULLETS; ++i) {
        data->bullets[i].ttl = 0;
    }

    for (uint8_t i = 0; i < INITIAL_ASTEROIDS; ++i) {
        data->asteroids[i].type = ASTEROID_TYPES;
        data->asteroids[i].x = (rand() % WIDTH) << 8;
        data->asteroids[i].y = (rand() % HEIGHT) << 8;
        data->asteroids[i].vx = (int8_t)rand();
        data->asteroids[i].vy = (int8_t)rand();
    }
}

static void Asteroids_render() {
    for (uint8_t i = 0; i < MAX_BULLETS; ++i) {
        if (data->bullets[i].ttl != 0) {
            game_draw_pixel(data->bullets[i].x / 256,
                            data->bullets[i].y / 256,
                            WHITE);
        }
    }

    for (uint8_t i = 0; i < MAX_ASTEROIDS; ++i) {
        if (data->asteroids[i].type) {
            game_draw_sprite(&asteroid_sprites[data->asteroids[i].type - 1],
                    data->asteroids[i].x / 256,
                    data->asteroids[i].y / 256, WHITE);
            game_draw_sprite(&asteroid_sprites[data->asteroids[i].type - 1],
                    data->asteroids[i].x / 256 - 64,
                    data->asteroids[i].y / 256, WHITE);
            game_draw_sprite(&asteroid_sprites[data->asteroids[i].type - 1],
                    data->asteroids[i].x / 256,
                    data->asteroids[i].y / 256 - 64, WHITE);
            game_draw_sprite(&asteroid_sprites[data->asteroids[i].type - 1],
                    data->asteroids[i].x / 256 - 64,
                    data->asteroids[i].y / 256 - 64, WHITE);
        }
    }

    for (uint8_t i = 0; i < MAX_PARTICLES; ++i) {
        if (data->particles[i].state != 0) {
            uint8_t color = 0;

            switch (data->particles[i].state) {
                case 1: color = ORANGE; break;
                case 2: color = RED; break;
                case 3: color = RED_MIDDLE; break;
                case 4: color = RED_DARK; break;
                case 5: color = WHITE; break;
            }

            game_draw_pixel(data->particles[i].x / 256,
                            data->particles[i].y / 256,
                            color);
        }
    }
    game_draw_sprite(&player_sprites[data->player.ri],
        data->player.x / 256 - PLAYER_CENTER_X,
        data->player.y / 256 - PLAYER_CENTER_Y, CYAN);
}

static void Asteroids_update(unsigned long delta) {
    if (game_is_button_pressed(BUTTON_LEFT)) {
        data->player.r += delta * PLAYER_ROT_ACCELERATION;
    }

    if (game_is_button_pressed(BUTTON_RIGHT)) {
        data->player.r -= delta * PLAYER_ROT_ACCELERATION;
    }

    data->player.ri = data->player.r >> 12;

    if (data->next_bullet > delta) {
        data->next_bullet -= delta;
    } else {
        data->next_bullet = 0;
    }

    if (game_is_button_pressed(BUTTON_A)) {
        if (data->next_bullet == 0) {
            data->next_bullet = BULLET_DELAY;
            for (uint8_t i = 0; i < MAX_BULLETS; ++i) {
                if (data->bullets[i].ttl == 0) {
                    data->bullets[i].ttl = BULLET_TTL;
                    data->bullets[i].vx = BULLET_SPEED * pgm_read_word(direction_x + data->player.ri);
                    data->bullets[i].vy = BULLET_SPEED * pgm_read_word(direction_y + data->player.ri);
                    data->bullets[i].x = data->player.x + data->bullets[i].vx;
                    data->bullets[i].y = data->player.y + data->bullets[i].vy;
                    data->bullets[i].vx += data->player.vx;
                    data->bullets[i].vy += data->player.vy;
                    break;
                }
            }
        }
    }

    if (game_is_button_pressed(BUTTON_B)) {
        uint16_t ox = data->player.vx;
        uint16_t oy = data->player.vy;
        data->player.vx += delta * pgm_read_word(direction_x + data->player.ri) / 256;
        data->player.vy += delta * pgm_read_word(direction_y + data->player.ri) / 256;
        uint16_t x = data->player.vx >> 8;
        uint16_t y = data->player.vy >> 8;
        uint16_t t = x * x + y * y;
        if (t > 255) {
            data->player.vx = ox;
            data->player.vy = oy;
        } else {
            data->player.vi = t;
        }

        if (data->next_particle < delta) {
            for (uint8_t i = 0; i < MAX_PARTICLES; ++i) {
                if (data->particles[i].state == 0) {
                    data->particles[i].state = 1;
                    data->particles[i].x = data->player.x;
                    data->particles[i].y = data->player.y;
                    data->particles[i].vx = -pgm_read_word(direction_x + data->player.ri);
                    data->particles[i].vy = -pgm_read_word(direction_y + data->player.ri);
                    data->particles[i].x += 4 * data->particles[i].vx;
                    data->particles[i].y += 4 * data->particles[i].vy;
                    data->particles[i].vx += (int8_t)rand();
                    data->particles[i].vy += (int8_t)rand();
                    data->particles[i].vx += data->player.vx;
                    data->particles[i].vy += data->player.vy;
                    break;
                }
            }
            if (data->next_particle + PARTICLE_DELAY < delta) {
                data->next_particle = 0;
            } else {
                data->next_particle += PARTICLE_DELAY - delta;
            }
        } else {
            data->next_particle -= delta;
        }
    }

    if (data->player.vi) {
        uint16_t ox = data->player.vx;
        uint16_t oy = data->player.vy;

        if (data->player.vx > 0) {
            data->player.vx -= data->player.vi * delta / PLAYER_DECELERATION;
        } else {
            data->player.vx += data->player.vi * delta / PLAYER_DECELERATION;
        }

        if (data->player.vy > 0) {
            data->player.vy -= data->player.vi * delta / PLAYER_DECELERATION;
        } else {
            data->player.vy += data->player.vi * delta / PLAYER_DECELERATION;
        }

        uint16_t x = data->player.vx >> 8;
        uint16_t y = data->player.vy >> 8;
        uint16_t t = x * x + y * y;
        if (t > 255) {
            data->player.vx = ox;
            data->player.vy = oy;
        } else {
            data->player.vi = t;
        }
    }

    data->player.x += data->player.vx * delta / VELF;
    data->player.y += data->player.vy * delta / VELF;
    data->player.x &= 0x3fff;
    data->player.y &= 0x3fff;

    for (uint8_t i = 0; i < MAX_BULLETS; ++i) {
        if (data->bullets[i].ttl > delta) {
            data->bullets[i].ttl -= delta;
            data->bullets[i].x += data->bullets[i].vx * delta / VELF;
            data->bullets[i].y += data->bullets[i].vy * delta / VELF;
            data->bullets[i].x &= 0x3fff;
            data->bullets[i].y &= 0x3fff;
        } else {
            data->bullets[i].ttl = 0;
        }
    }

    for (uint8_t i = 0; i < MAX_ASTEROIDS; ++i) {
        if (data->asteroids[i].type) {
            data->asteroids[i].x += data->asteroids[i].vx * delta / VELF;
            data->asteroids[i].y += data->asteroids[i].vy * delta / VELF;
            data->asteroids[i].x &= 0x3fff;
            data->asteroids[i].y &= 0x3fff;
        }
    }

    for (uint8_t i = 0; i < MAX_PARTICLES; ++i) {
        if (data->particles[i].state) {
            data->particles[i].x += data->particles[i].vx * delta / VELF;
            data->particles[i].y += data->particles[i].vy * delta / VELF;
            data->particles[i].x &= 0x3fff;
            data->particles[i].y &= 0x3fff;
            if (data->particles[i].ttl > delta) {
                data->particles[i].ttl -= delta;
            } else {
                data->particles[i].ttl += PARTICLE_TTL - delta;
                if (++data->particles[i].state > FLAME_COLORS) {
                    data->particles[i].state = 0;
                }
            }
        }
    }

    for (uint8_t i = 0; i < MAX_ASTEROIDS; ++i) {
        if (!data->asteroids[i].type) continue;
        for (uint8_t j = 0; j < MAX_BULLETS; ++j) {
            if (!data->bullets[j].ttl) continue;
            if (check_collision(data->asteroids[i].type, data->asteroids[i].x >> 8,
                    data->asteroids[i].y >> 8, data->bullets[j].x >> 8,
                    data->bullets[j].y >> 8)) {
                uint8_t t = data->asteroids[i].type;
                int16_t vx = data->bullets[j].vx / 4;
                int16_t vy = data->bullets[j].vy / 4;
                data->asteroids[i].type = 0;
                Asteroid me = data->asteroids[i];
                if (t > 1) {
                    uint8_t cnt = 0;
                    for (uint8_t k = i; k < MAX_ASTEROIDS && cnt < 2; ++k) {
                        if (data->asteroids[k].type) continue;
                        data->asteroids[k].x = me.x;
                        data->asteroids[k].y = me.y;
                        data->asteroids[k].vx = me.vx;
                        data->asteroids[k].vy = me.vy;

                        if (cnt == 0) {
                            data->asteroids[k].vx -= vy;
                            data->asteroids[k].vy += vx;
                        } else {
                            data->asteroids[k].vx += vy;
                            data->asteroids[k].vy -= vx;
                        }
                        data->asteroids[k].type = t - 1;
                        ++cnt;
                    }
                }
                else
                for (uint8_t k = 0, cnt = 0; k < MAX_PARTICLES && cnt <= 4; ++k) {
                    if (data->particles[k].state) continue;
                    uint8_t delta = (t - 1) * 2;
                    if (delta == 0) delta = 1;
                    data->particles[k].x = (me.x + delta) & 0x3fff;
                    data->particles[k].y = (me.y + delta) & 0x3fff;
                    data->particles[k].vx = (int8_t)rand() * (int16_t)3 + me.vx;
                    data->particles[k].vy = (int8_t)rand() * (int16_t)3 + me.vy;
                    data->particles[k].x += data->particles[k].vx;
                    data->particles[k].y += data->particles[k].vy;
                    data->particles[k].ttl = (uint8_t)rand() + (uint16_t)500;
                    data->particles[k].state = 5;
                    ++cnt;
                }
                data->bullets[j].ttl = 0;
                break;
            }
        }
    }
}

const game_instance Asteroids PROGMEM {
    "Asteroids",
    Asteroids_prepare,
    Asteroids_render,
    Asteroids_update,
    sizeof(AsteroidsData),
    (void**)(&data)
};
