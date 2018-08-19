#pragma GCC optimize ("-O3")

#include "libgame.h"
#include "graphics.h"
#include "binary.h"
#include "controls.h"
#define B BLUE
#define G GREEN
#define R RED
#define S CYAN
#define P PURPLE
#define W WHITE
#define O TRANSPARENT
#define Y YELLOW
#define L BLACK
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
    BOOO1OOOO, BO1OOOOOO,
    BOOOO1OOO, B1OOOOOOO,
    BOOOOO1O1, BOOOOOOOO,
    BOO111111, B111OOOOO,
    BO1111111, B1111OOOO,
    B111OO111, BOO111OOO,
    BO1111111, B1111OOOO,
    BOO111111, B111OOOOO,
    BOOOO1O1O, B1OOOOOOO,
    BOOO1OOOO, BO1OOOOOO
};

const game_sprite YourSprite PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    13, 1O, 2, YourSprite_lines
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
  const uint8_t player_lines[] PROGMEM = {
   O, O, O, O, P, P, P, P, P, P, O, O, O,
   O, O, O, P, P, P, P, P, P, P, P, P, O,
   O, P, P, Y, Y, Y, Y, Y, L, Y, P, O, O,
   P, Y, P, Y, Y, Y, Y, Y, L, Y, Y, Y, O,
   P, Y, P, P, Y, Y, Y, Y, Y, Y, Y, Y, Y,
   O, B, Y, Y, Y, Y, Y, Y, Y, Y, Y, Y, O,
   B, B, S, S, W, S, S, S, S, O, O, O, O,
   B, S, S, S, W, R, R, W, S, S, S, O, O,
   S, S, S, S, R, W, 0, R, S, S, S, S, O,
   Y, B, S, W, R, 0, W, R, W, S, B, Y, O,
   Y, B, B, B, B, B, B, B, B, B, B, B, B,
   Y, Y, B, B, B, B, B, B, B, B, B, B, B,
   O, O, W, W, W, O, O, W, W, W, B, O, O,
   O, P, P, P, O, O, O, O, P, P, P, O, O,
   P, P, P, P, O, O, O, O, P, P, P, P, O
 };


 const game_color_sprite player PROGMEM = {
    13, 15, player_lines
 };

 const uint8_t player_left_lines[] PROGMEM = {

    O, O, O, P, P, P, P, P, P, O, O, O, O,
    O, P, P, P, P, P, P, P, P, P, O, O, O,
    O,O ,P ,Y ,L ,Y ,Y ,Y ,Y ,Y ,P ,P , O,
    O, Y, Y, Y, L, Y, Y, Y, Y, Y, P, Y, P,
    Y, Y, Y, Y, Y, Y, Y, Y, Y, P, P, Y, P,
    O, Y, Y, Y, Y, Y, Y, Y, Y, Y, Y, B, O,
    O, O, O, O, S, S, S, S, W, S, S, B, B,
    O, O, S, S, S, W, R, R, W, S, S, S, B,
    O, S, S, S, S, R, 0, W, R, S, S, S, S,
    O, Y, B, S, W, R, W, 0, R, W, S, B, Y,
    B, B, B, B, B, B, B, B, B, B, B, B, Y,
    B, B, B, B, B, B, B, B, B, B, B, Y, Y,
    O, O, B, W, W, W, O, O, W, W, W, O, O,
    O, O, P, P, P, O, O, O, O, P, P, P, O,
    O, P, P, P, P, O, O, O, O, P, P, P, P

 };

 const game_color_sprite player_left PROGMEM = {
  13,15,player_left_lines
 };

 const uint8_t slimer_lines[] PROGMEM = {
    O,O,G,G,G,O,O,
    O,G,G,G,G,G,O,
    G,G,O,G,O,G,G,
    G,G,G,G,G,G,G,
    G,W,W,W,W,G,G,
    G,R,R,R,R,G,G,
    G,W,R,R,W,G,G,
    G,G,R,R,G,G,G,
    O,G,G,G,G,G,O,
    O,O,O,O,G,G,O,
    O,O,O,O,O,G,G,
    O,O,O,O,O,O,G
 };

 const game_color_sprite slimer PROGMEM = {

    7,12,slimer_lines

 };

 const uint8_t shoot_lines[] PROGMEM = {

    S,R,
    R,S

 };

 const game_color_sprite shoot PROGMEM = {

    2,2,shoot_lines

 };

 const uint8_t place_lines[] PROGMEM = {
      R,R,
      R,R
 };

 const game_color_sprite place PROGMEM = {
    2,2,place_lines
 };

 const uint8_t block_lines[] PROGMEM = {
  B,B,B,B,B,B,
  B,R,R,R,R,B,
  B,R,R,R,R,B,
  B,R,R,R,R,B,
  B,R,R,R,R,B,
  B,B,B,B,B,B
 };

 const game_color_sprite block PROGMEM = {
  6,6,block_lines
 };
 
struct GhostbusterData
{
   int8_t pposy, ShootCounter, pyposy;
   int8_t sposy[10],ShootPosy[10];
   bool flag, isShoot, loli, gameover;
   int8_t syposy[10];
   int8_t BlockPosy[100], BlockYPosy[100];
   bool isCanWalk;
   int8_t BeginOfJump, EndOfJump;
   bool isJump;
   bool kek;
    /* Объявляйте ваши переменные здесь */
    /* Чтобы потом обращаться к ним, пишите data->ПЕРЕМЕННАЯ */
};
static GhostbusterData* data; /* Эта переменная - указатель на структуру, которая содержит ваши переменные */

static void Ghostbuster_prepare()
{
    /* Здесь код, который будет исполнятся один раз */
    /* Здесь нужно инициализировать переменные */
    data->flag = true;
    data->isShoot = false;
    data->ShootCounter = 0;
    data->sposy[0] = 30;
    data->sposy[1] = 100;
    data->pposy = 10;
    data->pyposy = 47;
    for(int i = 0; i < 2; i++)
      data->syposy[i] = 50;
    data->BlockPosy[0] = 36;
    data->BlockYPosy[0] = 56;
    data->BeginOfJump = 47;
    data->EndOfJump = 37;
    data->isJump = false;
    data->BlockPosy[1] = 42;
    data->BlockYPosy[1] = 56;
    data->BlockPosy[2] = 42;
    data->BlockYPosy[2] = 50;
    data->BlockPosy[3] = 48;
    data->BlockYPosy[3] = 56;
    data->BlockPosy[4] = 48;
    data->BlockYPosy[4] = 50;
    data->BlockPosy[5] = 48;
    data->BlockYPosy[5] = 44;
    data->BlockPosy[6] = 54;
    data->BlockYPosy[6] = 44;
    data->BlockPosy[7] = 60;
    data->BlockYPosy[7] = 56;
    data->BlockPosy[8] = 60;
    data->BlockYPosy[8] = 50;
    data->BlockPosy[9] = 60;
    data->BlockYPosy[9] = 44;
    data->kek = true;
}
static void Ghostbuster_render()
{
  if(data->gameover)
  {
    game_draw_text((const unsigned char *)"GAME OVER", 6, 29, RED);
  }
    /* Здесь код, который будет вывзваться для отрисовки кадра */
    /* Он не должен менять состояние игры, для этого есть функция update */
    for(int i = 0; i < 2; i++)
      game_draw_color_sprite(&slimer, data->sposy[i], data->syposy[i]);

    for(int i = 0; i < 34; i++)
      game_draw_color_sprite(&place, 0+i*2, 62);

    if(data->flag)
      game_draw_color_sprite(&player, data->pposy, data->pyposy);
    else
      game_draw_color_sprite(&player_left, data->pposy, data->pyposy);

      if(data->isShoot)
        for(int i = 0; i < data->ShootCounter; i+=10) {
          game_draw_color_sprite(&shoot, data->ShootPosy[i/10], 57);
        }
    for(int i = 0; i < 10; i++)
      game_draw_color_sprite(&block, data->BlockPosy[i], data->BlockYPosy[i]);
    /* Здесь (и только здесь) нужно вызывать функции game_draw_??? */
}

static void Ghostbuster_update(unsigned long delta)
{
    if(data->gameover)
      return;

      
    for(int i = 0; i < 10; i++) {

        if( (data->pposy+13 == data->BlockPosy[i] && data->flag || data->pposy == data->BlockPosy[i]+6 && !data->flag ) &&  data->pyposy+13 >= data->BlockYPosy[i] ) {
          data->isCanWalk = false;
          break;
        }

        else 
            data->isCanWalk = true;
          
      
    }
    data->BeginOfJump = 47;
    for(int i = 0; i<10; i++) {

        if ( ( (data->pposy+2 >= data->BlockPosy[i]) && (data->pposy+2 <= data->BlockPosy[i] + 6) ) ||
        ( (data->pposy+11 >= data->BlockPosy[i]) && (data->pposy+11 <= data->BlockPosy[i] + 6) ) ||
        ( (data->pposy+6 >= data->BlockPosy[i]) && (data->pposy+6 <= data->BlockPosy[i] + 6) ) )
                 { data->BeginOfJump = data->BlockYPosy[i]-15;}
             
                
          
    }

   
    /* Здесь код, который будет выполняться в цикле */
    /* Переменная delta содержит количество миллисекунд с последнего вызова */
      data->loli = !data->loli;
      for(int i = 0; i < 2; i++) {
          if( data->sposy[i] != data->pposy && data->loli )
            data->sposy[i]+=data->sposy[i]>data->pposy?-1:1;

            
          if( ((data->sposy[i] == data->pposy) || (data->pposy + 13 == data->sposy[i])) && (data->syposy[i] == 50) )
            data->gameover = true;
      }
      
        if( (game_is_button_pressed(BUTTON_LEFT)) && (!data->isShoot) ) {
              data->flag = false;
          }
          else if ( (game_is_button_pressed(BUTTON_RIGHT)) && (!data->isShoot) ) {
            data->flag = true;
          }
        
      if(data->isCanWalk) {
        
          if( (game_is_button_pressed(BUTTON_LEFT)) && (!data->isShoot) ) {
            for(int i = 0; i < 2; i++)
              data->sposy[i]++;
            for(int i = 0; i < 10; i++)
              data->BlockPosy[i]++;
          }
          else if ( (game_is_button_pressed(BUTTON_RIGHT)) && (!data->isShoot) ) {
            for(int i = 0; i < 2; i++)
              data->sposy[i]--;
            for(int i = 0; i < 10; i++)
            data->BlockPosy[i]--;
          }
      }

      if( (game_is_button_pressed(BUTTON_A)) && (data->pyposy == 47) )
        data->isShoot = true;
      else
        data->isShoot = false;

      if( (data->isShoot) && (data->ShootCounter <= 90) )
        data->ShootCounter++;
      else if(!data->isShoot)
        data->ShootCounter = 0;

      if( (data->isShoot) && (data->flag) )
        for(int i = 0; i < data->ShootCounter; i+=10) {
          data->ShootPosy[i/10] = data->pposy + 13 + i/5;
        }
      else if(data->isShoot)
          for(int i = 0; i < data->ShootCounter; i+=10) {
          data->ShootPosy[i/10] = data->pposy - 2 - i/5;
        }

        for(int i = 0; i < data->ShootCounter; i+=10)
          for(int j = 0; j < 2; j++)
            if(data->ShootPosy[i/10] == data->sposy[j])
              data->syposy[j] = 100;


      if( (data->pyposy == data->BeginOfJump) && (game_is_button_pressed(BUTTON_UP) ) ) {
          data->isJump = true;
          data->EndOfJump = data->BeginOfJump - 10;
      }
      if(data->pyposy <= data->EndOfJump)
          data->isJump = false;

      if(data->isJump) 
        data->pyposy--;

      if( (!data->isJump) && (data->pyposy < data->BeginOfJump))
          data->pyposy++;

    /* Здесь можно работать с кнопками и обновлять переменные */
}

game_instance Ghostbuster = {
    "Ghostbuster",         /* Имя, отображаемое в меню */
    Ghostbuster_prepare,
    Ghostbuster_render,
    Ghostbuster_update,
    sizeof(GhostbusterData),
    (void**)(&data)
};


/* Не забудьте зарегистрировать игру в application.cpp */
