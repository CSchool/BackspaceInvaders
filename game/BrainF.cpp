#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "graphics.h"
#include "binary.h"
#include "controls.h"
#include "music.h"
#include "tunes.h"

/* Встроенные цвета:

    BLACK - Чёрный
    BLUE - Синий
    GREEN - Зелёный
    RED - Красный
    CYAN - Циановый
    PURPLE - Фиолетовый
    BROWN - Коричневый
    WHITE - Белый

    Для использования 64-х цветной палитры, укажите в game.ino COLOR_6BIT = 1

 * */

/* Кнопки:

   НА КОРПУСЕ:
   BUTTON_SW, BUTTON_NW, BUTTON_SE, BUTTON_NE

   NW              NE
    SW            SE


   НА ДЖОЙСТИКЕ:
   BUTTON_UP, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_DOWN, BUTTON_SELECT, BUTTON_START, BUTTON_A, BUTTON_B

        UP
    LEFT+RIGHT     SELECT  START      B  A
       DOWN

 * */

/* Спрайты

   максимальная высота - 16 пикселей

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
    // ШИРИНА, ВЫСОТА, ДАННЫЕ
    13, 10, YourSprite_lines
  };

*/

/* Функции отрисовки

   game_draw_pixel(x, y, color) - Красит точку (x, y) в цвет color
   game_draw_char(char, x, y, color) - Выводит символ char с левым верхним углом в точке (x, y) с цветом color
   game_draw_text(string, x, y, color) - Выводит строку string с левым верхним углом в точке (x, y) с цветом color
   game_draw_sprite(sprite, x, y, color) - Выводит спрайт sprite с левым верхним углом в точке (x, y) с цветом color

 * */

/* Функции ввода

   game_is_button_pressed(button) - Нажата ли кнопка? Например: game_is_button_pressed(BUTTON_START)
   game_is_any_button_pressed(mask) - Нажата ли хотя бы одна кнопка? Например: game_is_any_button_pressed(BITMASK(BUTTON_SW) | BITMASK(BUTTON_DOWN))

 * */

struct BrainFData
{
  uint8_t CursorposX, CursorposY, outputX;
  int Timer, Timer2;
  uint8_t MapX;
  uint8_t Menu;
  uint8_t chars[6][12][9];
  uint8_t output[35];
  uint8_t input[35];

  /* Объявляйте ваши переменные здесь */
  /* Чтобы потом обращаться к ним, пишите data->ПЕРЕМЕННАЯ */
};
static BrainFData* data; /* Эта переменная - указатель на структуру, которая содержит ваши переменные */

static void BrainF_ran()
{
  char cpu[50];
  uint8_t charsI;
  for (int i = 0; 35 > i; i++)
    data->output[i] = 0;
  data->outputX = 0;
  int inputX = 0;
  uint16_t j = 5;
  int brc = 0;
  // for (int mz = 0; 6 > mz; mz++)
  //  for (int mj = 0; 9 > mj; mj++)
  for (int i = 0; 12 * 6 * 9 > i; i++)
    if (data->chars[i / 9 / 12 % 6][i  % 12][i / 12 % 9] != 0)
      if (data->chars[i / 9 / 12 % 6][i  % 12][i / 12 % 9] < 19)
      {
        charsI = data->chars[i / 9 / 12 % 6][i  % 12][i / 12 % 9];
        if (charsI == 12) //>
          j++;
        if (charsI == 11) //<
          j--;
        if (charsI == 13) //+
        {
          if ((i > 1) && (data->chars[(i - 2) / 9 / 12 % 6][(i - 2)  % 12][(i - 2) / 12 % 9] <= 10) && (data->chars[(i - 2) / 9 / 12 % 6][(i - 2)  % 12][(i - 2) / 12 % 9] > 0) && (data->chars[(i - 1) / 9 / 12 % 6][(i - 1)  % 12][(i - 1) / 12 % 9] <= 10) && (data->chars[(i - 1) / 9 / 12 % 6][(i - 1)  % 12][(i - 1) / 12 % 9] > 0))
            cpu[j] += ((data->chars[(i - 2) / 9 / 12 % 6][(i - 2)  % 12][(i - 2) / 12 % 9] - 1) * 10) + (data->chars[(i - 1) / 9 / 12 % 6][(i - 1)  % 12][(i - 1) / 12 % 9] - 1);
          else if ((i > 0) && (data->chars[(i - 1) / 9 / 12 % 6][(i - 1)  % 12][(i - 1) / 12 % 9] <= 10) && (data->chars[(i - 1) / 9 / 12 % 6][(i - 1)  % 12][(i - 1) / 12 % 9] > 0))
            cpu[j] += data->chars[(i - 1) / 9 / 12 % 6][(i - 1)  % 12][(i - 1) / 12 % 9] - 1;
          else
            cpu[j] += 1;


        }
        if (charsI == 14) //-
        {
          if ((i > 1) && (data->chars[(i - 2) / 9 / 12 % 6][(i - 2)  % 12][(i - 2) / 12 % 9] <= 10) && (data->chars[(i - 2) / 9 / 12 % 6][(i - 2)  % 12][(i - 2) / 12 % 9] > 0) && (data->chars[(i - 1) / 9 / 12 % 6][(i - 1)  % 12][(i - 1) / 12 % 9] <= 10) && (data->chars[(i - 1) / 9 / 12 % 6][(i - 1)  % 12][(i - 1) / 12 % 9] > 0))
            cpu[j] -= ((data->chars[(i - 2) / 9 / 12 % 6][(i - 2)  % 12][(i - 2) / 12 % 9] - 1) * 10) + (data->chars[(i - 1) / 9 / 12 % 6][(i - 1)  % 12][(i - 1) / 12 % 9] - 1);
          else if ((i > 0) && (data->chars[(i - 1) / 9 / 12 % 6][(i - 1)  % 12][(i - 1) / 12 % 9] <= 10) && (data->chars[(i - 1) / 9 / 12 % 6][(i - 1)  % 12][(i - 1) / 12 % 9] > 0))
            cpu[j] -= data->chars[(i - 1) / 9 / 12 % 6][(i - 1)  % 12][(i - 1) / 12 % 9] - 1;
          else
            cpu[j] -= 1;
        }
        if (charsI == 15) { //.
          data->output[data->outputX] = cpu[j];
          data->outputX++;
        }
        if (charsI == 16) { //,
          cpu[j] = data->input[inputX];
          inputX++;
        }

        if (charsI == 17) {
          charsI = data->chars[i / 9 / 12 % 6][i  % 12][i / 12 % 9];
          if (cpu[j] == 0) {
            ++brc;
            while (brc) {
              ++i;
              charsI = data->chars[i / 9 / 12 % 6][i  % 12][i / 12 % 9];
              if (charsI == 17)
                ++brc;
              if (charsI == 18)
                --brc;
            }
          } else
            continue;
        } else if (charsI == 18) {
          charsI = data->chars[i / 9 / 12 % 6][i  % 12][i / 12 % 9];
          if (cpu[j] == 0)
            continue;
          else {
            if (charsI == 18)
              brc++;
            while (brc) {
              --i;
              charsI = data->chars[i / 9 / 12 % 6][i  % 12][i / 12 % 9];
              if (charsI == 17)
                brc--;
              if (charsI == 18)
                brc++;
            }
            --i;
            charsI = data->chars[i / 9 / 12 % 6][i  % 12][i / 12 % 9];
          }
        }
      }

}

static void BrainF_setmap(int x1, int z1, uint8_t mmapX, uint8_t a)
{
  if (!((0 <= a) && (a <= 19 )))
  {
    return;
  }
  /* if (a == true)
     if (data->RenderBlocks[myx][myz] < 127)
       data->RenderBlocks[myx][myz] = data->RenderBlocks[myx][myz] + 127;
    if (a == false)
     if (data->RenderBlocks[myx][myz] > 127)
       data->RenderBlocks[myx][myz] = data->RenderBlocks[myx][myz] + 127;*/


  data->chars[mmapX][x1][z1] =  a;

}

static uint8_t* BrainF_getcharmap(uint8_t x1, uint8_t z1, uint8_t mmapX)
{
  const uint8_t* col[19] PROGMEM = {0, "0"/*1*/, "1"/*2*/, "2",
                                    "3",  "4", "5", "6", "7", "8", "9"/*10*/, "<"/*11*/, ">"/*12*/, "+"/*13*/,
                                    "-"/*14*/, "."/*15*/, ","/*16*/, "["/*17*/, "]"/*18*/
                                   };

  if  ((data->chars[mmapX][x1][z1] >= 0) && (data->chars[mmapX][x1][z1] <= 19))
    return (const uint8_t*)col[data->chars[mmapX][x1][z1]];
  return (const uint8_t*)0;
}
static uint8_t BrainF_getmap(uint8_t x1, uint8_t z1, uint8_t mmapX)
{


  if  ((data->chars[mmapX][x1][z1] >= 0) && (data->chars[mmapX][x1][z1] <= 19))
    return (uint8_t)data->chars[mmapX][x1][z1];
  return (uint8_t)0;
}
static void BrainF_prepare()
{
  game_set_ups(10);
  data->Timer = 0;
  data->CursorposX = 0;
  data->CursorposY = 0;
  data->MapX = 0;
  data->Menu = 0;
  data->Timer2 = 0;
  /* Здесь код, который будет исполнятся один раз */
  /* Здесь нужно инициализировать переменные */
}

static void BrainF_render()
{


  if (data->Menu == 0) {
    for (int i = 0; 9 > i; i++)
      for (int j = 0; 12 > j; j++)
        if (BrainF_getmap(j, i, data->MapX) != 0)
          game_draw_text(BrainF_getcharmap(j, i, data->MapX), (j) * 5, (i) * 7, WHITE);
    /* Здесь код, который будет вывзваться для отрисовки кадра */
    if (data->Timer < 300)
      game_draw_text("_", data->CursorposX, data->CursorposY, WHITE);
  } else if (data->Menu == 1) {
      for (int j = 0; 12 > j; j++)
      for (int i = 0; 3 > i; i++)
      if (i*12+j<35)
          game_draw_char(data->input[i*12+j], (j) * 5, (i) * 7, WHITE);
    /* Здесь код, который будет вывзваться для отрисовки кадра */
    if (data->Timer < 300)
      game_draw_text("_", data->CursorposX, data->CursorposY, WHITE);

  } else {
    for (int i = 0; i < data->outputX; i++)
      //  if (data->output[i] > 9)
    {
      uint8_t* s = data->output[i];
      game_draw_char(s, i * 6, 32, WHITE);
    }
    // else
    game_draw_text((uint8_t*)"Download", 0, 0, RED);
    game_draw_text((uint8_t*)"multisize", 0, 9, RED);
    game_draw_text((uint8_t*)"on", 0, 64-9-9, RED);
    game_draw_text((uint8_t*)"mu..ze.ru", 0, 64-9, RED);
  }

  /* Он не должен менять состояние игры, для этого есть функция update */
  /* Здесь (и только здесь) нужно вызывать функции game_draw_??? */
}

static void BrainF_update(unsigned long delta)
{

  data->Timer += delta;
  if (data->Timer2 > 0)
    data->Timer2 -= delta;

  if (data->Timer > 600)
    data->Timer = 0;
  if  (game_is_button_pressed ( BUTTON_START) && (data->Timer2 <= 0))
  {
    if ((data->Menu == 2))
      data->Menu = 0;
    else
      data->Menu = data->Menu + 1;
    if ((data->Menu == 2))  {
      BrainF_ran();
      for (int i = 0; 35 > i; i++)
        data->input[i] = 0;
    }

    if ((data->Menu == 1))  
    {
      data->CursorposX=0;
      data->CursorposY=0;
    }
    data->Timer2 = 200;
    return;
  }

if (data->Menu == 1) {
 if ( game_is_button_pressed (BUTTON_A)) {
      if (data->input[(data->CursorposY / 7)*12+(data->CursorposX / 5)] < 255) {
        data->input[(data->CursorposY / 7)*12+(data->CursorposX / 5)] +=  1;
        data->Timer = 300;
      } else {
        data->input[(data->CursorposY / 7)*12+(data->CursorposX / 5)] =  0;
        data->Timer = 300;
      }
    }
    if ( game_is_button_pressed (BUTTON_B)) {
      if (data->input[(data->CursorposY / 7)*12+(data->CursorposX / 5)] > 0 ) {
        data->input[(data->CursorposY / 7)*12+(data->CursorposX / 5)] -=  1;
        data->Timer = 300;
      } else {
       data->input[(data->CursorposY / 7)*12+(data->CursorposX / 5)]=255 ;
        data->Timer = 300;
      }
    }

    

    if (data->CursorposX < 64 - 9)
      if  (game_is_button_pressed (BUTTON_RIGHT)) {
        data->CursorposX += 5;
        data->Timer = -200;
      }
    if (data->CursorposX > 0)
      if  ( game_is_button_pressed (BUTTON_LEFT)) {
        data->CursorposX -= 5;
        data->Timer = -200;
      }

    if (data->CursorposY < 3*8)
      if ( game_is_button_pressed (BUTTON_DOWN)) {
        data->CursorposY += 7;
        data->Timer = -200;
      }
    if (data->CursorposY > 0)
      if  (game_is_button_pressed (BUTTON_UP)) {
        data->CursorposY -= 7;
        data->Timer = -200;
      }

  
}







  
  if (data->Menu == 0) {
    if ( game_is_button_pressed (BUTTON_A)) {
      if (BrainF_getmap(data->CursorposX / 5, data->CursorposY / 7, data->MapX) < 18) {
        BrainF_setmap(data->CursorposX / 5, data->CursorposY / 7, data->MapX, BrainF_getmap(data->CursorposX / 5, data->CursorposY / 7, data->MapX) + 1);
        data->Timer = 300;
      } else {
        BrainF_setmap(data->CursorposX / 5, data->CursorposY / 7, data->MapX, 0);
        data->Timer = 300;
      }
    }
    if ( game_is_button_pressed (BUTTON_B)) {
      if (BrainF_getmap(data->CursorposX / 5, data->CursorposY / 7, data->MapX) > 0 ) {
        BrainF_setmap(data->CursorposX / 5, data->CursorposY / 7, data->MapX, BrainF_getmap(data->CursorposX / 5, data->CursorposY / 7, data->MapX) - 1);
        data->Timer = 300;
      } else {
        BrainF_setmap(data->CursorposX / 5, data->CursorposY / 7, data->MapX, 18);
        data->Timer = 300;
      }
    }

    if ( game_is_button_pressed (BUTTON_DOWN))
      if (data->CursorposY > 64 - 8 - 8)
        if (data->MapX < 6) {
          data->MapX += 1;
          data->CursorposY = 0;
          data->Timer = -200;
          return;
        }
    if  (game_is_button_pressed (BUTTON_UP))
      if (data->CursorposY < 2)
        if (data->MapX > 0) {
          data->MapX -= 1;
          data->CursorposY = 64 - 8;
          data->Timer = -200;
          return;
        }

    if (data->CursorposX < 64 - 9)
      if  (game_is_button_pressed (BUTTON_RIGHT)) {
        data->CursorposX += 5;
        data->Timer = -200;
      }
    if (data->CursorposX > 0)
      if  ( game_is_button_pressed (BUTTON_LEFT)) {
        data->CursorposX -= 5;
        data->Timer = -200;
      }

    if (data->CursorposY < 64 - 8)
      if ( game_is_button_pressed (BUTTON_DOWN)) {
        data->CursorposY += 7;
        data->Timer = -200;
      }
    if (data->CursorposY > 0)
      if  (game_is_button_pressed (BUTTON_UP)) {
        data->CursorposY -= 7;
        data->Timer = -200;
      }
  }

  /* Здесь код, который будет выполняться в цикле */
  /* Переменная delta содержит количество миллисекунд с последнего вызова */
  /* Здесь можно работать с кнопками и обновлять переменные */
}

const game_instance BrainF PROGMEM = {
  "BrainF",         /* Название игры.
                           Используется как имя записи в EEPROM.
                           Не больше 10 символов */
  BrainF_prepare,
  BrainF_render,
  BrainF_update,
  sizeof(BrainFData),
  (void**)(&data)
};


/* Не забудьте зарегистрировать игру в application.cpp, libgame.h */
/* Также нужно добавить файл с игрой в qt_emulator/CMakeLists.txt */