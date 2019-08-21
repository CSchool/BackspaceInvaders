#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "graphics.h"
#include "binary.h"
#include "controls.h"

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

const game_sprite YourSprite PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    13, 10, 2, YourSprite_lines
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
 
const uint8_t prep_lines[] PROGMEM = {
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
};
const game_sprite prep PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    5, 5, prep_lines
};
 
const uint8_t YourSprite_lines[] PROGMEM = {
    B00010000,
    B00001000,
    B00000101,
    B00111111,
    B01111111,
    B11100111,
    B01111111,
    B00111111,
    B00001010,
    B00010000,
};
const game_sprite YourSprite PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    5, 10, YourSprite_lines
};
struct artyomData
{
  int flag;
  int schet;
  float prepX;
  int prepY;
  int spriteX;
  int spriteY;
  bool gameover;
  char h;

    /* Объявляйте ваши переменные здесь */
    /* Чтобы потом обращаться к ним, пишите data->ПЕРЕМЕННАЯ */
};
static artyomData* data; /* Эта переменная - указатель на структуру, которая содержит ваши переменные */

static void artyom_prepare()
{

    /* Здесь код, который будет исполнятся один раз */
    data->prepY = 55;
    data->prepX = 64;
    data->spriteX = 10;
    data->spriteY = 50;
    data->flag = 0;
    data-> schet = 0;
    data-> h =  '0';
    data-> gameover  = false;

    /* Здесь нужно инициализировать переменные */
}

static void artyom_render()
{
    /* Здесь код, который будет вывзваться для отрисовки кадра */
    /* Он не должен менять состояние игры, для этого есть функция update */
game_draw_sprite(&prep, data->prepX, data->prepY, WHITE );
game_draw_sprite(&YourSprite, data->spriteX, data->spriteY, RED);
 char s[5];
    sprintf(s, "%d", data->schet);
    game_draw_text((uint8_t*)s, 5, 5, CYAN);
if(data->gameover)game_draw_text("game over", 10, 10, GREEN);

    sprintf(s, "%d", data->spriteY);
    game_draw_text((uint8_t*)s, 43, 7, BLUE);
}


static void artyom_update(unsigned long delta)
{
  if(data->gameover){
    
    return;
  }
 data->h = data->schet + '0';
 if(data->spriteY >= 50)data->flag = 0;
 if(data->flag == 0){
 if(game_is_button_pressed(BUTTON_UP) && data->spriteY >= 30){


    data->spriteY--;
   if(data->spriteY == 30)data->flag = 1;

 }
 }
if(data->flag == 1 && data->spriteY<= 50)  data->spriteY ++;
if(!game_is_button_pressed(BUTTON_UP)){
if(data->spriteY <= 50){
  data->spriteY ++;
}
  }

    /* Здесь код, который будет выполняться в цикле */
    /* Переменная delta содержит количество миллисекунд с последнего вызова */
   if(data->schet < 10)data->prepX -= 0.5;
   if(data->schet >= 10)data->prepX--;
   if(data->prepX == 0){
   data-> prepX = 64;
   data->schet++;
   }
   if((data->spriteX + 5 == data->prepX || data->spriteX + 4 == data->prepX)  && (data->spriteY == 51 || data->spriteY == 49 || data->spriteY == 47|| data->spriteY == 48|| data->spriteY == 46|| data->spriteY <= 45|| data->spriteY == 44 )){
    data->gameover = true;
   }
    /* Здесь можно работать с кнопками и обновлять переменные */
}

const game_instance artyom PROGMEM = {
    "artyom",         /* Имя, отображаемое в меню */
   artyom_prepare,
    artyom_render,
   artyom_update,
    sizeof(artyomData),
    (void**)(&data)
};


/* Не забудьте зарегистрировать игру в application.cpp */
