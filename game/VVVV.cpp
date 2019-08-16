#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "graphics.h"
#include "binary.h"
#include "controls.h"
#include "music.h"
#include "tunes.h"

#define B BLUE
#define G GREEN
#define R RED
#define S CYAN
#define P PURPLE
#define W WHITE
#define O TRANSPARENT
#define Y YELLOW
#define L BLACK
#define E ORANGE
/* Встроенные цвета:
 *
 *  BLACK - Чёрный
 *  BLUE - Синий
 *  GREEN - Зелёный
 *  RED - Красный
 *  CYAN - Циановый
 *  PURPLE - Фиолетовый
 *  BROWN - Коричневый
 *  WHITE - Белый
 *
 *  Для использования 64-х цветной палитры, укажите в game.ino COLOR_6BIT = 1
 *
 * */

/* Кнопки:
 *
 * НА КОРПУСЕ:
 * BUTTON_SW, BUTTON_NW, BUTTON_SE, BUTTON_NE
 *
 * NW              NE
 *  SW            SE
 *
 *
 * НА ДЖОЙСТИКЕ:
 * BUTTON_UP, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_DOWN, BUTTON_SELECT, BUTTON_START, BUTTON_A, BUTTON_B
 *
 *      UP
 *  LEFT+RIGHT     SELECT  START      B  A
 *     DOWN
 *
 * */

/* Спрайты
 * 
 * максимальная высота - 16 пикселей

 определение спрайта


   x     x      
    x   x       
     x x        
  xxxxxxxxx     
 xxxxxxxxxxx    
xxx  xxx  xxx   
 xxxxxxxxxxx    
  xxxxxxxxx     
    x x x       
   x     x      


 --------------------------------
 
const uint8_t YourSprite_lines[] PROGMEM = {
    B00010000, B01000000,
    B00001000, B10000000,
    B00000101, B00000000,
    B00111111, B11100000,
    B01111111, B11110000,
    B11100111, B00111000,
    B01111111, B11110000,
    B00111111, B11100000,
    B00001010, B10000000,
    B00010000, B01000000
};



*/

/* Функции отрисовки
 *
 * game_draw_pixel(x, y, color) - Красит точку (x, y) в цвет color
 * game_draw_char(char, x, y, color) - Выводит символ char с левым верхним углом в точке (x, y) с цветом color
 * game_draw_text(string, x, y, color) - Выводит строку string с левым верхним углом в точке (x, y) с цветом color
 * game_draw_sprite(sprite, x, y, color) - Выводит спрайт sprite с левым верхним углом в точке (x, y) с цветом color
 *
 * */

/* Функции ввода
 *
 * game_is_button_pressed(button) - Нажата ли кнопка? Например: game_is_button_pressed(BUTTON_START)
 * game_is_any_button_pressed(mask) - Нажата ли хотя бы одна кнопка? Например: game_is_any_button_pressed(BITMASK(BUTTON_SW) | BITMASK(BUTTON_DOWN))
 *
 * */

static const uint8_t tree_lines[] PROGMEM = {
    B11110000, 
    B11110000, 
    B11110000, 
    B01100000, 
};
const game_sprite tree PROGMEM = {
    // ШИРИНА, ВЫСОТА, ДАННЫЕ
    4, 4, tree_lines
};
 
     static const uint8_t mountain_lines[] PROGMEM = {
    B00100000, 
    B01110000, 
    B11110000, 
    B11110000, 
};
const game_sprite mountain PROGMEM = {
    // ШИРИНА, ВЫСОТА, ДАННЫЕ
    4, 4, mountain_lines
};

    static const uint8_t z_sword_lines[] PROGMEM = {
    B10000000, 
    B10000000, 
    B00000000, 
    B00000000, 
};
const game_sprite z_sword PROGMEM = {
    // ШИРИНА, ВЫСОТА, ДАННЫЕ
    1, 2, z_sword_lines
};

static const uint8_t z_4_lines[] PROGMEM = {
    O,G,G,O,
    O,Y,Y,O,
    O,G,Y,O,
    O,E,E,O
};
static const game_color_sprite z_4 PROGMEM = {
    // ШИРИНА, ВЫСОТА, ДАННЫЕ
    4, 4, z_4_lines
};


static const uint8_t z_3_lines[] PROGMEM = {
    O,G,G,O,
    O,Y,Y,O,
    O,Y,G,O,
    O,E,E,O
};
static const game_color_sprite z_3 PROGMEM = {
    // ШИРИНА, ВЫСОТА, ДАННЫЕ
    4, 4, z_3_lines
};

 static const uint8_t z_2_lines[] PROGMEM = {
    O,G,G,O,
    O,G,G,O,
    Y,G,G,Y,
    O,E,E,O
};
static const game_color_sprite z_2 PROGMEM = {
    // ШИРИНА, ВЫСОТА, ДАННЫЕ
    4, 4, z_2_lines
};

 static const uint8_t z_1_lines[] PROGMEM = {
    O,G,G,O,
    O,Y,Y,O,
    Y,G,G,Y,
    O,E,E,O
};
static const game_color_sprite z_1 PROGMEM = {
    // ШИРИНА, ВЫСОТА, ДАННЫЕ
    4, 4, z_1_lines
};

static const game_color_sprite * const z_p[] PROGMEM = {
    &z_2, &z_3, &z_4,&z_1, NULL
};

struct VVVVData
{
    int x,y,p,mapx,mapy;
    /* Объявляйте ваши переменные здесь */
    /* Чтобы потом обращаться к ним, пишите data->ПЕРЕМЕННАЯ */
};
static VVVVData* data; /* Эта переменная - указатель на структуру, которая содержит ваши переменные */

static void VVVV_prepare()
{
    game_enable_frame_buffer();
    data->x=32;
    data->y=32;
    data->p=1;
    data->mapx=1;
    data->mapy=1;
    /* Здесь код, который будет исполнятся один раз */
    /* Здесь нужно инициализировать переменные */
}

static void VVVV_render()
{
     game_set_ups(45);
     const game_color_sprite *spr = pgm_read_pointer(&z_p[data->p]);
    game_draw_color_sprite(spr, data->x, data->y);
    

     
    /* Здесь код, который будет вывзваться для отрисовки кадра */
    /* Он не должен менять состояние игры, для этого есть функция update */

    /* Здесь (и только здесь) нужно вызывать функции game_draw_??? */
}

static void VVVV_update(unsigned long delta) 

{
  bool flag1 = false;
    if (data->y>64)
    {
      data->y=20;
      data->mapy=data->mapy-1;
      flag1 = true;
    }

    if (data->y<20)
    {
      data->y=60;
      data->mapy=data->mapy+1;
      flag1 = true;
    }

    if (data->x>60)
    {
      data->x=0;
      data->mapx=data->mapx+1;
      flag1 = true;
    }

    if (data->x<0)
    {
      data->x=60;
      data->mapx=data->mapx-1;
      flag1 = true;
    }

    
    /* Здесь код, который будет выполняться в цикле */
    /* Переменная delta содержит количество миллисекунд с последнего вызова */

    /* Здесь можно работать с кнопками и обновлять переменные */



     //game_get_pixel(data->x, data->y+1) ==0
     game_draw_rect(0,19, 64,1, WHITE);
bool flag = false;

     if(game_is_button_pressed(BUTTON_UP) && game_get_pixel(data->x, data->y-1) ==0)
    {
     data->y=data->y-1;
     data->p=0;
     flag = true;
    }

    if(game_is_button_pressed(BUTTON_DOWN))
    {
     data->y=data->y+1;
     data->p=3;
     flag = true;
    }

    if(game_is_button_pressed(BUTTON_RIGHT))
    {
     data->x=data->x+1;
     data->p=2;
     flag = true;
    }

    if(game_is_button_pressed(BUTTON_LEFT))
    {
     data->x=data->x-1;
     data->p=1;
     flag = true;
    }

if (flag) {

  //локация 13
  if (data->mapx==0 && data->mapy==3)
  {
    if (flag1)
  {
    game_clear_screen();
  }
        game_draw_sprite(&tree, 12, 60, PURPLE);
        game_draw_sprite(&tree, 16, 60, PURPLE);
  }
  //конец локации 13

  //лщкация 03
if (data->mapx==0 && data->mapy==3)
  {
   if (flag1)
  {
    game_clear_screen();
  }
        game_draw_sprite(&mountain, 0, 60, PURPLE);
        game_draw_sprite(&mountain, 4, 60, PURPLE);
        game_draw_sprite(&mountain, 8, 60, PURPLE);
        game_draw_sprite(&tree, 12, 60, PURPLE);
        game_draw_sprite(&tree, 16, 60, PURPLE);
        game_draw_sprite(&tree, 20, 60, PURPLE);
        game_draw_sprite(&tree, 24, 60, PURPLE);
        game_draw_sprite(&tree, 28, 60, PURPLE);
        game_draw_sprite(&tree, 32, 60, PURPLE);
        game_draw_sprite(&tree, 36, 60, PURPLE);
        game_draw_sprite(&tree, 40, 60, PURPLE);
        game_draw_sprite(&tree, 44, 60, PURPLE);
        game_draw_sprite(&tree, 48, 60, PURPLE);
        game_draw_sprite(&tree, 52, 60, PURPLE);
        game_draw_sprite(&tree, 56, 60, PURPLE);
        game_draw_sprite(&tree, 60, 60, PURPLE);

        game_draw_sprite(&mountain, 0, 56, PURPLE);
        game_draw_sprite(&mountain, 4, 56, PURPLE);
        game_draw_sprite(&mountain, 8, 56, PURPLE);
        game_draw_sprite(&tree, 12, 56, PURPLE);
        game_draw_sprite(&tree, 16, 56, PURPLE);
        game_draw_sprite(&tree, 20, 56, PURPLE);
        game_draw_sprite(&tree, 24, 56, PURPLE);
        game_draw_sprite(&tree, 28, 56, PURPLE);
        game_draw_sprite(&tree, 32, 56, PURPLE);
        game_draw_sprite(&tree, 36, 56, PURPLE);
        game_draw_sprite(&tree, 40, 56, PURPLE);
        game_draw_sprite(&tree, 44, 56, PURPLE);
        game_draw_sprite(&tree, 48, 56, PURPLE);
        game_draw_sprite(&tree, 52, 56, PURPLE);
        game_draw_sprite(&tree, 56, 56, PURPLE);
        game_draw_sprite(&tree, 60, 56, PURPLE);

        game_draw_sprite(&mountain, 0, 52, PURPLE);
        game_draw_sprite(&mountain, 4, 52, PURPLE);
        game_draw_sprite(&mountain, 8, 52, PURPLE);

        game_draw_sprite(&mountain, 0, 48, PURPLE);
        game_draw_sprite(&mountain, 4, 48, PURPLE);
        game_draw_sprite(&mountain, 8, 48, PURPLE);

        game_draw_sprite(&mountain, 0, 44, PURPLE);
        game_draw_sprite(&mountain, 4, 44, PURPLE);
        game_draw_sprite(&mountain, 8, 44, PURPLE);
        game_draw_sprite(&tree, 53, 44, PURPLE);
        game_draw_sprite(&tree, 48, 44, PURPLE);
        game_draw_sprite(&tree, 40, 44, PURPLE);

        game_draw_rect(0, 36, 4, 4, BLUE);
        game_draw_rect(4, 36, 4, 4, BLUE);
        game_draw_rect(8, 36, 4, 4, BLUE);
        game_draw_rect(12, 36, 4, 4, BLUE);
        game_draw_rect(16, 36, 4, 4, BLUE);
        game_draw_rect(20, 36, 4, 4, BLUE);
        game_draw_rect(24, 36, 4, 4, BLUE);
        game_draw_sprite(&tree, 53, 36, PURPLE);
        game_draw_sprite(&tree, 48, 36, PURPLE);
        game_draw_sprite(&tree, 40, 36, PURPLE);

        game_draw_rect(0, 32, 4, 4, BLUE);
        game_draw_rect(4, 32, 4, 4, BLUE);
        game_draw_rect(8, 32, 4, 4, BLUE);
        game_draw_rect(12, 32, 4, 4, BLUE);
        game_draw_rect(16, 32, 4, 4, BLUE);
        game_draw_rect(20, 32, 4, 4, BLUE);
        game_draw_rect(24, 32, 4, 4, BLUE);

        game_draw_rect(0, 28, 4, 4, BLUE);
        game_draw_rect(4, 28, 4, 4, BLUE);
        game_draw_rect(8, 28, 4, 4, BLUE);
        game_draw_rect(12, 28, 4, 4, BLUE);
        game_draw_rect(16, 28, 4, 4, BLUE);
        game_draw_rect(20, 28, 4, 4, BLUE);
        game_draw_rect(24, 28, 4, 4, BLUE);

        game_draw_rect(0, 24, 4, 4, BLUE);
        game_draw_rect(4, 24, 4, 4, BLUE);
        game_draw_rect(8, 24, 4, 4, BLUE);
        game_draw_rect(12, 24, 4, 4, BLUE);
        game_draw_rect(16, 24, 4, 4, BLUE);
        game_draw_rect(20, 24, 4, 4, BLUE);
        game_draw_rect(24, 24, 4, 4, BLUE);
        game_draw_sprite(&tree, 36, 24, PURPLE);
        game_draw_sprite(&tree, 40, 24, PURPLE);
        game_draw_sprite(&tree, 44, 24, PURPLE);
        game_draw_sprite(&tree, 48, 24, PURPLE);
        game_draw_sprite(&tree, 52, 24, PURPLE);
        game_draw_sprite(&tree, 56, 24, PURPLE);
        game_draw_sprite(&tree, 60, 24, PURPLE);

        game_draw_rect(0, 20, 4, 4, BLUE);
        game_draw_rect(4, 20, 4, 4, BLUE);
        game_draw_rect(8, 20, 4, 4, BLUE);
        game_draw_rect(12, 20, 4, 4, BLUE);
        game_draw_rect(16, 20, 4, 4, BLUE);
        game_draw_rect(20, 20, 4, 4, BLUE);
        game_draw_rect(24, 20, 4, 4, BLUE);
        game_draw_sprite(&tree, 36, 20, PURPLE);
        game_draw_sprite(&tree, 40, 20, PURPLE);
        game_draw_sprite(&tree, 44, 20, PURPLE);
        game_draw_sprite(&tree, 48, 20, PURPLE);
        game_draw_sprite(&tree, 52, 20, PURPLE);
        game_draw_sprite(&tree, 56, 20, PURPLE);
        game_draw_sprite(&tree, 60, 20, PURPLE);
  }
  //конец локации 03

//локация 02
if (data->mapx==0 && data->mapy==2)
  {
    if (flag1)
  {
    game_clear_screen();
  }
        game_draw_sprite(&mountain, 0, 60, PURPLE);
        game_draw_sprite(&mountain, 4, 60, PURPLE);
        game_draw_sprite(&mountain, 8, 60, PURPLE);
        game_draw_sprite(&mountain, 12, 60, PURPLE);
        game_draw_sprite(&mountain, 16, 60, PURPLE);
        game_draw_sprite(&mountain, 20, 60, PURPLE);
        game_draw_sprite(&mountain, 24, 60, PURPLE);
        game_draw_sprite(&mountain, 28, 60, PURPLE);
        game_draw_sprite(&mountain, 32, 60, PURPLE);
        game_draw_sprite(&mountain, 36, 60, PURPLE);
        game_draw_sprite(&mountain, 40, 60, PURPLE);
        game_draw_sprite(&mountain, 44, 60, PURPLE);
        game_draw_sprite(&mountain, 56, 60, PURPLE);
        game_draw_sprite(&mountain, 60, 60, PURPLE);

        game_draw_sprite(&mountain, 0, 56, PURPLE);
        game_draw_sprite(&mountain, 4, 56, PURPLE);
        game_draw_sprite(&mountain, 8, 56, PURPLE);
        game_draw_sprite(&mountain, 12, 56, PURPLE);
        game_draw_sprite(&mountain, 16, 56, PURPLE);
        game_draw_sprite(&mountain, 20, 56, PURPLE);
        game_draw_sprite(&mountain, 24, 56, PURPLE);
        game_draw_sprite(&mountain, 28, 56, PURPLE);
        game_draw_sprite(&mountain, 32, 56, PURPLE);
        game_draw_sprite(&mountain, 36, 56, PURPLE);
        game_draw_sprite(&mountain, 40, 56, PURPLE);
        game_draw_sprite(&mountain, 44, 56, PURPLE);
        game_draw_sprite(&mountain, 56, 56, PURPLE);
        game_draw_sprite(&mountain, 60, 56, PURPLE);

        game_draw_sprite(&mountain, 56, 52, PURPLE);
        game_draw_sprite(&mountain, 60, 52, PURPLE);

        game_draw_sprite(&mountain, 12, 48, PURPLE);
        game_draw_sprite(&mountain, 16, 48, PURPLE);
        game_draw_sprite(&mountain, 20, 48, PURPLE);
        game_draw_sprite(&mountain, 24, 48, PURPLE);
        game_draw_sprite(&mountain, 60, 48, PURPLE);

        game_draw_sprite(&mountain, 32, 44, PURPLE);
        game_draw_sprite(&mountain, 36, 44, PURPLE);
        game_draw_sprite(&mountain, 40, 44, PURPLE);

        game_draw_sprite(&mountain, 12, 40, PURPLE);
        game_draw_sprite(&mountain, 16, 40, PURPLE);
        game_draw_sprite(&mountain, 20, 40, PURPLE);
        game_draw_sprite(&mountain, 24, 40, PURPLE);

        game_draw_sprite(&mountain, 32, 36, PURPLE);
        game_draw_sprite(&mountain, 36, 36, PURPLE);
        game_draw_sprite(&mountain, 40, 36, PURPLE);

        game_draw_sprite(&mountain, 12, 32, PURPLE);
        game_draw_sprite(&mountain, 16, 32, PURPLE);
        game_draw_sprite(&mountain, 20, 32, PURPLE);
        game_draw_sprite(&mountain, 24, 32, PURPLE);
        game_draw_sprite(&mountain, 60, 32, PURPLE);

        game_draw_sprite(&mountain, 56, 28, PURPLE);
        game_draw_sprite(&mountain, 60, 28, PURPLE);

        game_draw_sprite(&mountain, 0, 24, PURPLE);
        game_draw_sprite(&mountain, 4, 24, PURPLE);
        game_draw_sprite(&mountain, 8, 24, PURPLE);
        game_draw_sprite(&mountain, 12, 24, PURPLE);
        game_draw_sprite(&mountain, 16, 24, PURPLE);
        game_draw_sprite(&mountain, 20, 24, PURPLE);
        game_draw_sprite(&mountain, 24, 24, PURPLE);
        game_draw_rect(28, 24, 4, 4, WHITE);
        game_draw_sprite(&mountain, 32, 24, PURPLE);
        game_draw_sprite(&mountain, 36, 24, PURPLE);
        game_draw_sprite(&mountain, 40, 24, PURPLE);
        game_draw_sprite(&mountain, 44, 24, PURPLE);
        game_draw_sprite(&mountain, 48, 24, PURPLE);
        game_draw_sprite(&mountain, 52, 24, PURPLE);
        game_draw_sprite(&mountain, 56, 24, PURPLE);
        game_draw_sprite(&mountain, 60, 24, PURPLE);

        game_draw_sprite(&mountain, 0, 20, PURPLE);
        game_draw_sprite(&mountain, 4, 20, PURPLE);
        game_draw_sprite(&mountain, 8, 20, PURPLE);
        game_draw_sprite(&mountain, 12, 20, PURPLE);
        game_draw_sprite(&mountain, 16, 20, PURPLE);
        game_draw_sprite(&mountain, 20, 20, PURPLE);
        game_draw_sprite(&mountain, 24, 20, PURPLE);
        game_draw_sprite(&mountain, 28, 20, PURPLE);
        game_draw_sprite(&mountain, 32, 20, PURPLE);
        game_draw_sprite(&mountain, 36, 20, PURPLE);
        game_draw_sprite(&mountain, 40, 20, PURPLE);
        game_draw_sprite(&mountain, 44, 20, PURPLE);
        game_draw_sprite(&mountain, 48, 20, PURPLE);
        game_draw_sprite(&mountain, 52, 20, PURPLE);
        game_draw_sprite(&mountain, 56, 20, PURPLE);
        game_draw_sprite(&mountain, 60, 20, PURPLE);
  }
//конец локации 02
  
  //локация01
if (data->mapx==0 && data->mapy==1)
  {
     if (flag1)
  {
    game_clear_screen();
  }
        game_draw_sprite(&mountain, 0, 60, PURPLE);
        game_draw_sprite(&mountain, 4, 60, PURPLE);
        game_draw_sprite(&mountain, 8, 60, PURPLE);
        game_draw_sprite(&mountain, 12, 60, PURPLE);
        game_draw_sprite(&mountain, 16, 60, PURPLE);
        game_draw_sprite(&mountain, 20, 60, PURPLE);
        game_draw_sprite(&mountain, 24, 60, PURPLE);
        game_draw_sprite(&mountain, 28, 60, PURPLE);
        game_draw_sprite(&mountain, 32, 60, PURPLE);
        game_draw_sprite(&mountain, 36, 60, PURPLE);
        game_draw_sprite(&mountain, 40, 60, PURPLE);
        game_draw_sprite(&mountain, 44, 60, PURPLE);
        game_draw_sprite(&mountain, 48, 60, PURPLE);
        game_draw_sprite(&mountain, 52, 60, PURPLE);
        game_draw_sprite(&mountain, 56, 60, PURPLE);
        game_draw_sprite(&mountain, 60, 60, PURPLE);

        game_draw_sprite(&mountain, 0, 56, PURPLE);
        game_draw_sprite(&mountain, 4, 56, PURPLE);
        game_draw_sprite(&mountain, 8, 56, PURPLE);
        game_draw_sprite(&mountain, 12, 56, PURPLE);
        game_draw_sprite(&mountain, 16, 56, PURPLE);
        game_draw_sprite(&mountain, 20, 56, PURPLE);
        game_draw_sprite(&mountain, 24, 56, PURPLE);
        game_draw_sprite(&mountain, 28, 56, PURPLE);
        game_draw_sprite(&mountain, 32, 56, PURPLE);
        game_draw_sprite(&mountain, 36, 56, PURPLE);
        game_draw_sprite(&mountain, 40, 56, PURPLE);
        game_draw_sprite(&mountain, 44, 56, PURPLE);
        game_draw_sprite(&mountain, 48, 56, PURPLE);
        game_draw_sprite(&mountain, 52, 56, PURPLE);
        game_draw_sprite(&mountain, 56, 56, PURPLE);
        game_draw_sprite(&mountain, 60, 56, PURPLE);

        game_draw_sprite(&mountain, 56, 52, PURPLE);
        game_draw_sprite(&mountain, 60, 52, PURPLE);

        game_draw_sprite(&mountain, 32, 48, PURPLE);
        game_draw_sprite(&mountain, 56, 48, PURPLE);
        game_draw_sprite(&mountain, 60, 48, PURPLE);

        game_draw_sprite(&mountain, 60, 44, PURPLE);
        game_draw_sprite(&mountain, 40, 44, PURPLE);

        game_draw_sprite(&mountain, 32, 40, PURPLE);
        game_draw_sprite(&mountain, 20, 40, PURPLE);
        game_draw_sprite(&mountain, 8, 40, PURPLE);

        game_draw_sprite(&mountain, 60, 36, PURPLE);
        game_draw_sprite(&mountain, 40, 36, PURPLE);

        game_draw_sprite(&mountain, 32, 32, PURPLE);
        game_draw_sprite(&mountain, 56, 32, PURPLE);
        game_draw_sprite(&mountain, 60, 32, PURPLE);

        game_draw_sprite(&mountain, 56, 28, PURPLE);
        game_draw_sprite(&mountain, 60, 28, PURPLE);

        game_draw_sprite(&mountain, 0, 24, PURPLE);
        game_draw_sprite(&mountain, 4, 24, PURPLE);
        game_draw_sprite(&mountain, 8, 24, PURPLE);
        game_draw_sprite(&mountain, 12, 24, PURPLE);
        game_draw_sprite(&mountain, 16, 24, PURPLE);
        game_draw_sprite(&mountain, 20, 24, PURPLE);
        game_draw_sprite(&mountain, 24, 24, PURPLE);
        game_draw_sprite(&mountain, 28, 24, PURPLE);
        game_draw_sprite(&mountain, 32, 24, PURPLE);
        game_draw_sprite(&mountain, 36, 24, PURPLE);
        game_draw_sprite(&mountain, 40, 24, PURPLE);
        game_draw_sprite(&mountain, 44, 24, PURPLE);
        game_draw_sprite(&mountain, 56, 24, PURPLE);
        game_draw_sprite(&mountain, 60, 24, PURPLE);

        game_draw_sprite(&mountain, 0, 20, PURPLE);
        game_draw_sprite(&mountain, 4, 20, PURPLE);
        game_draw_sprite(&mountain, 8, 20, PURPLE);
        game_draw_sprite(&mountain, 12, 20, PURPLE);
        game_draw_sprite(&mountain, 16, 20, PURPLE);
        game_draw_sprite(&mountain, 20, 20, PURPLE);
        game_draw_sprite(&mountain, 24, 20, PURPLE);
        game_draw_sprite(&mountain, 28, 20, PURPLE);
        game_draw_sprite(&mountain, 32, 20, PURPLE);
        game_draw_sprite(&mountain, 36, 20, PURPLE);
        game_draw_sprite(&mountain, 40, 20, PURPLE);
        game_draw_sprite(&mountain, 44, 20, PURPLE);
        game_draw_sprite(&mountain, 56, 20, PURPLE);
        game_draw_sprite(&mountain, 60, 20, PURPLE);
  }
//конец локациии 01


    
//локация 22
   if (data->mapx==2 && data->mapy==2)
   {

   if (flag1)
  {
    game_clear_screen();
  }
    
      game_draw_sprite(&mountain, 0, 60, PURPLE);
      game_draw_sprite(&mountain, 4, 60, PURPLE);
      game_draw_sprite(&tree, 8, 60, PURPLE);
      game_draw_sprite(&tree, 16, 60, PURPLE);
      game_draw_sprite(&tree, 24, 60, PURPLE);
      game_draw_sprite(&tree, 36, 60, PURPLE);
      game_draw_sprite(&tree, 44, 60, PURPLE);
      game_draw_sprite(&tree, 52, 60, PURPLE);
      game_draw_sprite(&tree, 60, 60, PURPLE);

      game_draw_sprite(&mountain, 0, 56, PURPLE);
      game_draw_sprite(&mountain, 4, 56, PURPLE);
      game_draw_sprite(&tree, 8, 56, PURPLE);
      game_draw_sprite(&tree, 16, 56, PURPLE);
      game_draw_sprite(&tree, 24, 56, PURPLE);
      game_draw_sprite(&tree, 36, 56, PURPLE);
      game_draw_sprite(&tree, 44, 56, PURPLE);
      game_draw_sprite(&tree, 52, 56, PURPLE);
      game_draw_sprite(&tree, 60, 56, PURPLE);

      game_draw_sprite(&mountain, 0, 52, PURPLE);
      game_draw_sprite(&mountain, 4, 52, PURPLE);
      game_draw_sprite(&tree, 60, 52, PURPLE);

      game_draw_sprite(&mountain, 0, 48, PURPLE);
      game_draw_sprite(&tree, 16, 48, PURPLE);
      game_draw_sprite(&tree, 36, 48, PURPLE);
      game_draw_sprite(&tree, 52, 48, PURPLE);
      game_draw_sprite(&tree, 60, 48, PURPLE);

      game_draw_sprite(&tree, 8, 44, PURPLE);

      game_draw_sprite(&tree, 16, 40, PURPLE);
      game_draw_sprite(&tree, 24, 40, PURPLE);
      game_draw_sprite(&tree, 36, 40, PURPLE);
      game_draw_sprite(&tree, 44, 40, PURPLE);
      game_draw_sprite(&tree, 52, 40, PURPLE);

      game_draw_sprite(&tree, 8, 36, PURPLE);

      game_draw_sprite(&mountain, 0, 32, PURPLE);
      game_draw_sprite(&tree, 16, 32, PURPLE);
      game_draw_sprite(&tree, 36, 32, PURPLE);
      game_draw_sprite(&tree, 52, 32, PURPLE);
      game_draw_sprite(&tree, 60, 32, PURPLE);

      game_draw_sprite(&mountain, 0, 28, PURPLE);
      game_draw_sprite(&mountain, 4, 28, PURPLE);
      game_draw_sprite(&tree, 60, 28, PURPLE);

      game_draw_sprite(&mountain, 0, 24, PURPLE);
      game_draw_sprite(&mountain, 4, 24, PURPLE);
      game_draw_sprite(&tree, 8, 24, PURPLE);
      game_draw_sprite(&tree, 16, 24, PURPLE);
      game_draw_sprite(&tree, 24, 24, PURPLE);
      game_draw_sprite(&tree, 36, 24, PURPLE);
      game_draw_sprite(&tree, 44, 24, PURPLE);
      game_draw_sprite(&tree, 52, 24, PURPLE);
      game_draw_sprite(&tree, 60, 24, PURPLE);

      game_draw_sprite(&mountain, 0, 20, PURPLE);
      game_draw_sprite(&mountain, 4, 20, PURPLE);
      game_draw_sprite(&tree, 8, 20, PURPLE);
      game_draw_sprite(&tree, 16, 20, PURPLE);
      game_draw_sprite(&tree, 24, 20, PURPLE);
      game_draw_sprite(&tree, 36, 20, PURPLE);
      game_draw_sprite(&tree, 44, 20, PURPLE);
      game_draw_sprite(&tree, 52, 20, PURPLE);
      game_draw_sprite(&tree, 60, 20, PURPLE);
   }
//конец локации 22
  
//локация 21
   if (data->mapx==2 && data->mapy==1)
   {

   if (flag1)
  {
    game_clear_screen();
  }

      game_draw_sprite(&mountain, 0, 60, PURPLE);
      game_draw_sprite(&mountain, 4, 60, PURPLE);
      game_draw_sprite(&tree, 8, 60, PURPLE);
      game_draw_sprite(&tree, 12, 60, PURPLE);
      game_draw_sprite(&tree, 16, 60, PURPLE);
      game_draw_sprite(&tree, 20, 60, PURPLE);
      game_draw_sprite(&tree, 24, 60, PURPLE);
      game_draw_sprite(&tree, 28, 60, PURPLE);
      game_draw_sprite(&tree, 32, 60, PURPLE);
      game_draw_sprite(&tree, 36, 60, PURPLE);
      game_draw_sprite(&tree, 40, 60, PURPLE);
      game_draw_sprite(&tree, 44, 60, PURPLE);
      game_draw_sprite(&tree, 48, 60, PURPLE);
      game_draw_sprite(&tree, 52, 60, PURPLE);
      game_draw_sprite(&tree, 56, 60, PURPLE);
      game_draw_sprite(&tree, 60, 60, PURPLE);

      game_draw_sprite(&mountain, 0, 56, PURPLE);
      game_draw_sprite(&mountain, 4, 56, PURPLE);
      game_draw_sprite(&tree, 8, 56, PURPLE);
      game_draw_sprite(&tree, 12, 56, PURPLE);
      game_draw_sprite(&tree, 16, 56, PURPLE);
      game_draw_sprite(&tree, 20, 56, PURPLE);
      game_draw_sprite(&tree, 24, 56, PURPLE);
      game_draw_sprite(&tree, 28, 56, PURPLE);
      game_draw_sprite(&tree, 32, 56, PURPLE);
      game_draw_sprite(&tree, 36, 56, PURPLE);
      game_draw_sprite(&tree, 40, 56, PURPLE);
      game_draw_sprite(&tree, 44, 56, PURPLE);
      game_draw_sprite(&tree, 48, 56, PURPLE);
      game_draw_sprite(&tree, 52, 56, PURPLE);
      game_draw_sprite(&tree, 56, 56, PURPLE);
      game_draw_sprite(&tree, 60, 56, PURPLE);

      game_draw_sprite(&mountain, 0, 52, PURPLE);
      game_draw_sprite(&mountain, 4, 52, PURPLE);

      game_draw_sprite(&mountain, 0, 48, PURPLE);
      game_draw_sprite(&mountain, 4, 48, PURPLE);
      game_draw_sprite(&tree, 24, 48, PURPLE);
      game_draw_sprite(&tree, 36, 48, PURPLE);
      game_draw_sprite(&tree, 44, 48, PURPLE);
      game_draw_sprite(&tree, 52, 48, PURPLE);

      game_draw_sprite(&mountain, 0, 44, PURPLE);
      game_draw_sprite(&tree, 8, 44, PURPLE);
      game_draw_sprite(&tree, 16, 44, PURPLE);

      game_draw_sprite(&tree, 24, 40, PURPLE);
      game_draw_sprite(&tree, 36, 40, PURPLE);
      game_draw_sprite(&tree, 44, 40, PURPLE);
      game_draw_sprite(&tree, 52, 40, PURPLE);

      game_draw_sprite(&mountain, 0, 36, PURPLE);
      game_draw_sprite(&tree, 8, 36, PURPLE);
      game_draw_sprite(&tree, 16, 36, PURPLE);

      game_draw_sprite(&mountain, 0, 32, PURPLE);
      game_draw_sprite(&mountain, 4, 32, PURPLE);
      game_draw_sprite(&tree, 24, 32, PURPLE);
      game_draw_sprite(&tree, 36, 32, PURPLE);
      game_draw_sprite(&tree, 44, 32, PURPLE);
      game_draw_sprite(&tree, 52, 32, PURPLE);

      game_draw_sprite(&mountain, 0, 28, PURPLE);
      game_draw_sprite(&mountain, 4, 28, PURPLE);

      game_draw_sprite(&mountain, 0, 24, PURPLE);
      game_draw_sprite(&mountain, 4, 24, PURPLE);
      game_draw_sprite(&tree, 8, 24, PURPLE);
      game_draw_sprite(&tree, 16, 24, PURPLE);
      game_draw_sprite(&tree, 24, 24, PURPLE);
      game_draw_sprite(&tree, 36, 24, PURPLE);
      game_draw_sprite(&tree, 44, 24, PURPLE);
      game_draw_sprite(&tree, 52, 24, PURPLE);
      game_draw_sprite(&tree, 60, 24, PURPLE);

      game_draw_sprite(&mountain, 0, 20, PURPLE);
      game_draw_sprite(&mountain, 4, 20, PURPLE);
      game_draw_sprite(&tree, 8, 20, PURPLE);
      game_draw_sprite(&tree, 16, 20, PURPLE);
      game_draw_sprite(&tree, 24, 20, PURPLE);
      game_draw_sprite(&tree, 36, 20, PURPLE);
      game_draw_sprite(&tree, 44, 20, PURPLE);
      game_draw_sprite(&tree, 52, 20, PURPLE);
      game_draw_sprite(&tree, 60, 20, PURPLE);
   }

//конец локации 21
    
//локация 12
     if (data->mapx==1 && data->mapy==2)
     {
     if (flag1)
  {
    game_clear_screen();
  }
      
      game_draw_sprite(&mountain, 0, 60, PURPLE);
      game_draw_sprite(&mountain, 4, 60, PURPLE);
      game_draw_sprite(&mountain, 8, 60, PURPLE);
      game_draw_sprite(&mountain, 12, 60, PURPLE);
      game_draw_sprite(&mountain, 16, 60, PURPLE);
      game_draw_sprite(&mountain, 20, 60, PURPLE);
      game_draw_sprite(&mountain, 24, 60, PURPLE);
      game_draw_sprite(&mountain, 36, 60, PURPLE);
      game_draw_sprite(&mountain, 40, 60, PURPLE);
      game_draw_sprite(&mountain, 44, 60, PURPLE);
      game_draw_sprite(&mountain, 48, 60, PURPLE);
      game_draw_sprite(&mountain, 52, 60, PURPLE);
      game_draw_sprite(&mountain, 56, 60, PURPLE);
      game_draw_sprite(&mountain,60, 60, PURPLE);

      game_draw_sprite(&mountain, 0, 56, PURPLE);
      game_draw_sprite(&mountain, 4, 56, PURPLE);
      game_draw_sprite(&mountain, 8, 56, PURPLE);
      game_draw_sprite(&mountain, 12, 56, PURPLE);
      game_draw_sprite(&mountain, 16, 56, PURPLE);
      game_draw_sprite(&mountain, 20, 56, PURPLE);
      game_draw_sprite(&mountain, 24, 56, PURPLE);
      game_draw_sprite(&mountain, 36, 56, PURPLE);
      game_draw_sprite(&mountain, 40, 56, PURPLE);
      game_draw_sprite(&mountain, 44, 56, PURPLE);
      game_draw_sprite(&mountain, 48, 56, PURPLE);
      game_draw_sprite(&mountain, 52, 56, PURPLE);
      game_draw_sprite(&mountain, 56, 56, PURPLE);
      game_draw_sprite(&mountain, 60, 56, PURPLE);

      game_draw_sprite(&mountain, 0, 52, PURPLE);
      game_draw_sprite(&mountain, 4, 52, PURPLE);
      game_draw_sprite(&mountain, 36, 52, PURPLE);
      game_draw_sprite(&mountain, 40, 52, PURPLE);
      game_draw_sprite(&mountain, 56, 52, PURPLE);
      game_draw_sprite(&mountain, 60, 52, PURPLE);

      game_draw_sprite(&mountain, 0, 48, PURPLE);
      game_draw_sprite(&mountain, 12, 48, PURPLE);
      game_draw_sprite(&mountain, 20, 48, PURPLE);
      game_draw_sprite(&mountain, 48, 48, PURPLE);
      game_draw_sprite(&mountain, 60, 48, PURPLE);

      game_draw_sprite(&mountain, 12, 40, PURPLE);
      game_draw_sprite(&mountain, 20, 40, PURPLE);
      game_draw_sprite(&mountain, 48, 40, PURPLE);

      game_draw_sprite(&mountain, 0, 32, PURPLE);
      game_draw_sprite(&mountain, 12, 32, PURPLE);
      game_draw_sprite(&mountain, 20, 32, PURPLE);
      game_draw_sprite(&mountain, 48, 32, PURPLE);
      game_draw_sprite(&mountain, 60, 32, PURPLE);

      game_draw_sprite(&mountain, 0, 28, PURPLE);
      game_draw_sprite(&mountain, 4, 28, PURPLE);
      game_draw_sprite(&mountain, 36, 28, PURPLE);
      game_draw_sprite(&mountain, 40, 28, PURPLE);
      game_draw_sprite(&mountain, 56, 28, PURPLE);
      game_draw_sprite(&mountain, 60, 28, PURPLE);

      game_draw_sprite(&mountain, 0, 24, PURPLE);
      game_draw_sprite(&mountain, 4, 24, PURPLE);
      game_draw_sprite(&mountain, 8, 24, PURPLE);
      game_draw_sprite(&mountain, 12, 24, PURPLE);
      game_draw_sprite(&mountain, 16, 24, PURPLE);
      game_draw_sprite(&mountain, 20, 24, PURPLE);
      game_draw_sprite(&mountain, 24, 24, PURPLE);
      game_draw_sprite(&mountain, 28, 24, PURPLE);
      game_draw_sprite(&mountain, 32, 24, PURPLE);
      game_draw_sprite(&mountain, 36, 24, PURPLE);
      game_draw_sprite(&mountain, 40, 24, PURPLE);
      game_draw_sprite(&mountain, 44, 24, PURPLE);
      game_draw_sprite(&mountain, 48, 24, PURPLE);
      game_draw_sprite(&mountain, 52, 24, PURPLE);
      game_draw_sprite(&mountain, 56, 24, PURPLE);
      game_draw_sprite(&mountain, 60, 24, PURPLE);

      game_draw_sprite(&mountain, 0, 20, PURPLE);
      game_draw_sprite(&mountain, 4, 20, PURPLE);
      game_draw_sprite(&mountain, 8, 20, PURPLE);
      game_draw_sprite(&mountain, 12, 20, PURPLE);
      game_draw_sprite(&mountain, 16, 20, PURPLE);
      game_draw_sprite(&mountain, 20, 20, PURPLE);
      game_draw_sprite(&mountain, 24, 20, PURPLE);
      game_draw_sprite(&mountain, 28, 20, PURPLE);
      game_draw_sprite(&mountain, 32, 20, PURPLE);
      game_draw_sprite(&mountain, 36, 20, PURPLE);
      game_draw_sprite(&mountain, 40, 20, PURPLE);
      game_draw_sprite(&mountain, 44, 20, PURPLE);
      game_draw_sprite(&mountain, 48, 20, PURPLE);
      game_draw_sprite(&mountain, 52, 20, PURPLE);
      game_draw_sprite(&mountain, 56, 20, PURPLE);
      game_draw_sprite(&mountain, 60, 20, PURPLE);
     }
//конец локации 12  
    
    //локация 11
    
      if (data->mapx==1 && data->mapy==1)
  {
     if (flag1)
  {
    game_clear_screen();
  }
    
        game_draw_sprite(&mountain, 0, 60, PURPLE);
        game_draw_sprite(&mountain, 4, 60, PURPLE);
        game_draw_sprite(&mountain, 8, 60, PURPLE);
        game_draw_sprite(&mountain, 12, 60, PURPLE);
        game_draw_sprite(&mountain, 16, 60, PURPLE);
        game_draw_sprite(&mountain, 20, 60, PURPLE);
        game_draw_sprite(&mountain, 24, 60, PURPLE);
        game_draw_sprite(&mountain, 28, 60, PURPLE);
        game_draw_sprite(&mountain, 32, 60, PURPLE);
        game_draw_sprite(&mountain, 36, 60, PURPLE);
        game_draw_sprite(&mountain, 40, 60, PURPLE);
        game_draw_sprite(&mountain, 44, 60, PURPLE);
        game_draw_sprite(&mountain, 48, 60, PURPLE);
        game_draw_sprite(&mountain, 52, 60, PURPLE);
        game_draw_sprite(&mountain, 56, 60, PURPLE);
        game_draw_sprite(&mountain, 60, 60, PURPLE);
        
        game_draw_sprite(&mountain, 0, 56, PURPLE);
        game_draw_sprite(&mountain, 4, 56, PURPLE);
        game_draw_sprite(&mountain, 8, 56, PURPLE);
        game_draw_sprite(&mountain, 12, 56, PURPLE);
        game_draw_sprite(&mountain, 16, 56, PURPLE);
        game_draw_sprite(&mountain, 20, 56, PURPLE);
        game_draw_sprite(&mountain, 24, 56, PURPLE);
        game_draw_sprite(&mountain, 28, 56, PURPLE);
        game_draw_sprite(&mountain, 32, 56, PURPLE);
        game_draw_sprite(&mountain, 36, 56, PURPLE);
        game_draw_sprite(&mountain, 40, 56, PURPLE);
        game_draw_sprite(&mountain, 44, 56, PURPLE);
        game_draw_sprite(&mountain, 48, 56, PURPLE);
        game_draw_sprite(&mountain, 52, 56, PURPLE);
        game_draw_sprite(&mountain, 56, 56, PURPLE);
        game_draw_sprite(&mountain, 60, 56, PURPLE);

        game_draw_sprite(&mountain, 0, 52, PURPLE);
        game_draw_sprite(&mountain, 4, 52, PURPLE);
        game_draw_sprite(&mountain, 56, 52, PURPLE);
        game_draw_sprite(&mountain, 60, 52, PURPLE);

        game_draw_sprite(&mountain, 0, 48, PURPLE);
        game_draw_sprite(&mountain, 4, 48, PURPLE);
        game_draw_sprite(&mountain, 56, 48, PURPLE);
        game_draw_sprite(&mountain, 60, 48, PURPLE);

        game_draw_sprite(&mountain, 0, 44, PURPLE);
        game_draw_sprite(&mountain, 4, 44, PURPLE);
        game_draw_sprite(&mountain, 56, 44, PURPLE);
        game_draw_sprite(&mountain, 60, 44, PURPLE);

        game_draw_sprite(&mountain, 0, 36, PURPLE);
        game_draw_sprite(&mountain, 4, 36, PURPLE);
        game_draw_sprite(&mountain, 36, 36, PURPLE);
        game_draw_sprite(&mountain, 40, 36, PURPLE);
        game_draw_sprite(&mountain, 44, 36, PURPLE);
        game_draw_sprite(&mountain, 48, 36, PURPLE);
        game_draw_sprite(&mountain, 52, 36, PURPLE);
        game_draw_sprite(&mountain, 56, 36, PURPLE);
        game_draw_sprite(&mountain, 60, 36, PURPLE);

        game_draw_sprite(&mountain, 0, 32, PURPLE);
        game_draw_sprite(&mountain, 4, 32, PURPLE);
        game_draw_sprite(&mountain, 8, 32, PURPLE);
        game_draw_sprite(&mountain, 36, 32, PURPLE);
        game_draw_sprite(&mountain, 40, 32, PURPLE);
        game_draw_sprite(&mountain, 44, 32, PURPLE);
        game_draw_sprite(&mountain, 48, 32, PURPLE);
        game_draw_sprite(&mountain, 52, 32, PURPLE);
        game_draw_sprite(&mountain, 56, 32, PURPLE);
        game_draw_sprite(&mountain, 60, 32, PURPLE);

        game_draw_sprite(&mountain, 0, 28, PURPLE);
        game_draw_sprite(&mountain, 4, 28, PURPLE);
        game_draw_sprite(&mountain, 8, 28, PURPLE);
        game_draw_sprite(&mountain, 12, 28, PURPLE);
        game_draw_sprite(&mountain, 36, 28, PURPLE);
        game_draw_sprite(&mountain, 40, 28, PURPLE);
        game_draw_sprite(&mountain, 44, 28, PURPLE);
        game_draw_sprite(&mountain, 48, 28, PURPLE);
        game_draw_sprite(&mountain, 52, 28, PURPLE);
        game_draw_sprite(&mountain, 56, 28, PURPLE);
        game_draw_sprite(&mountain, 60, 28, PURPLE);

        game_draw_sprite(&mountain, 0, 24, PURPLE);
        game_draw_sprite(&mountain, 4, 24, PURPLE);
        game_draw_sprite(&mountain, 8, 24, PURPLE);
        game_draw_sprite(&mountain, 12, 24, PURPLE);
        game_draw_rect(16, 24, 4, 4, WHITE);
        game_draw_sprite(&mountain, 20, 24, PURPLE);
        game_draw_sprite(&mountain, 24, 24, PURPLE);
        game_draw_sprite(&mountain, 36, 24, PURPLE);
        game_draw_sprite(&mountain, 40, 24, PURPLE);
        game_draw_sprite(&mountain, 44, 24, PURPLE);
        game_draw_sprite(&mountain, 48, 24, PURPLE);
        game_draw_sprite(&mountain, 52, 24, PURPLE);
        game_draw_sprite(&mountain, 56, 24, PURPLE);
        game_draw_sprite(&mountain, 60, 24, PURPLE);

        game_draw_sprite(&mountain, 0, 20, PURPLE);
        game_draw_sprite(&mountain, 4, 20, PURPLE);
        game_draw_sprite(&mountain, 8, 20, PURPLE);
        game_draw_sprite(&mountain, 12, 20, PURPLE);
        game_draw_sprite(&mountain, 16, 20, PURPLE);
        game_draw_sprite(&mountain, 20, 20, PURPLE);
        game_draw_sprite(&mountain, 24, 20, PURPLE);
        game_draw_sprite(&mountain, 36, 20, PURPLE);
        game_draw_sprite(&mountain, 40, 20, PURPLE);
        game_draw_sprite(&mountain, 44, 20, PURPLE);
        game_draw_sprite(&mountain, 48, 20, PURPLE);
        game_draw_sprite(&mountain, 52, 20, PURPLE);
        game_draw_sprite(&mountain, 56, 20, PURPLE);
        game_draw_sprite(&mountain, 60, 20, PURPLE);
    }
     //локация 1 конец
}
}

const game_instance VVVV PROGMEM = {
    "VVVV",         /* Название игры. 
                           Используется как имя записи в EEPROM. 
                           Не больше 10 символов */
    VVVV_prepare,
    VVVV_render,
    VVVV_update,
    sizeof(VVVVData),
    (void**)(&data)
};


/* Не забудьте зарегистрировать игру в application.cpp, libgame.h */
/* Также нужно добавить файл с игрой в qt_emulator/CMakeLists.txt */
