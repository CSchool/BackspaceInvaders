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
  const uint8_t YourSprite_lines[] PROGMEM = {
    B00000000, B00000000, B00000000,/ *1* /
    B00000000, B00000000, B00000000,/ *2* /
    B00000000, B00000000, B00000000,/ *3* /
    B00000000, B00000000, B00000000,/ *4* /
    B00000000, B00000000, B00000000,/ *5* /
    B00000000, B00000000, B00000000,/ *6* /
    B00000000, B00000000, B00000000,/ *7* /
    B00000000, B00000000, B00000000,/ *8* /
    B00000000, B00000000, B00000000,/ *9* /
    B00000000, B00000000, B00000000,/ *10* /
    B00000000, B00000000, B00000000,/ *11* /
    B00000000, B00000000, B00000000,/ *12* /
    B00000000, B00000000, B00000000,/ *13* /
    B00000000, B00000000, B00000000,/ *14* /
    B00000000, B00000000, B00000000,/ *15* /
    B00000000, B00000000, B00000000 / *16* /
  };

  const game_sprite YourSprite PROGMEM = {
    // ШИРИНА, ВЫСОТА, ДАННЫЕ
    16, 16, YourSprite_lines
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
#define GR 0x70//серый
#define BR 0x31//коричневый
#define BR2 0x13//коричневый более тёмный
#define R RED
#define G GREEN
#define T TRANSPARENT
#define K BLACK
#define B BLUE
//#define Y YELLOW
//#define W WHITE
//const uint8_t TestBlock_lines[] PROGMEM = {
//  B01000000, B00000000, B00000000,/*1*/
//  B00000000, B00000000, B00000000,/*2*/
//  B00000000, B00000000, B00000000,/*3*/
//  B00000000, B00000000, B00000000,/*4*/
//  B00000000, B00000000, B00000000,/*5*/
//  B00000000, B00000000, B00000000,/*6*/
//  B00000000, B00000000, B00000000,/*7*/
//  B00000000, B00000000, B00000000,/*8*/
//  B00000000, B00000000, B00000000,/*9*/
//  B00000000, B00000000, B00000000,/*10*/
//  B00000000, B00000000, B00000000,/*11*/
//  B00000000, B00000000, B00000000,/*12*/
//  B00000000, B00000000, B00000000,/*13*/
//  B00000000, B00000000, B00000000,/*14*/
//  B00000000, B00000000, B00000000,/*15*/
//  B00000000, B00000000, B00000000 /*16*/
//};
const uint8_t Minecraft_Texture[] PROGMEM = {
  K, K, K, K, K, K, K, K, K, K, K, K,/*0*///кислород
  GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR,/*0*///камень
  BR, BR, BR, BR, BR, BR, BR, BR, BR, BR, G, G,/*1*///трава
  BR2, BR2, BR2, BR2, BR2, BR2, BR2, BR2, BR2, BR2, BR2, BR2,/*2*///земля
  B, B, B, B, B, B, B, B, B, B, B, B,/*3*///вода
};



const uint8_t mysinarray[] PROGMEM = {
  127, 129, 130, 132, 134, 136, 137, 139, 141, 143, 144, 146, 148, 149, 151, 153, 155, 156, 158, 160, 161, 163, 164, 166, 168, 169, 171, 172, 174, 175, 177, 179, 180, 181, 183, 184, 186, 187, 189, 190, 191, 193, 194, 195, 196, 198, 199, 200, 201, 202, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 214, 215, 216, 217, 218, 218, 219, 220, 220, 221, 222, 222, 223, 223, 224, 224, 224, 225, 225, 225, 226, 226, 226, 226, 227, 227, 227, 227, 227, 227, 227, 227, 227, 227, 227, 226, 226, 226, 226, 225, 225, 225, 224, 224, 224, 223, 223, 222, 222, 221, 220, 220, 219, 218, 218, 217, 216, 215, 214, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 202, 201, 200, 199, 198, 196, 195, 194, 193, 191, 190, 189, 187, 186, 184, 183, 181, 180, 179, 177, 175, 174, 172, 171, 169, 168, 166, 164, 163, 161, 160, 158, 156, 155, 153, 151, 150, 148, 146, 144, 143, 141, 139, 137, 136, 134, 132, 130, 129, 127, 125, 124, 122, 120, 118, 117, 115, 113, 111, 110, 108, 106, 105, 103, 101, 99, 98, 96, 94, 93, 91, 90, 88, 86, 85, 83, 82, 80, 79, 77, 76, 74, 73, 71, 70, 68, 67, 65, 64, 63, 61, 60, 59, 58, 56, 55, 54, 53, 52, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 40, 39, 38, 37, 36, 36, 35, 34, 34, 33, 32, 32, 31, 31, 30, 30, 30, 29, 29, 29, 28, 28, 28, 28, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 29, 29, 29, 30, 30, 30, 31, 31, 32, 32, 33, 34, 34, 35, 36, 36, 37, 38, 39, 40, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 52, 53, 54, 55, 56, 58, 59, 60, 61, 63, 64, 65, 67, 68, 70, 71, 73, 74, 75, 77, 79, 80, 82, 83, 85, 86, 88, 90, 91, 93, 94, 96, 98, 99, 101, 103, 104, 106, 108, 110, 111, 113, 115, 117, 118, 120, 122, 123, 125
};
//pgm_read_byte(&mysinarray[sinx]);


struct MinecraftData
{
  int i;
  int x;
  int y;
  float q;
  uint8_t MapHight[128];
  int selectblockX;
  int selectblockY;
  int selectblockZ;
  int drawblockX;
  int drawblockY;
  int drawblockZ;

  float CameraPositionX;
  float CameraPositionY;
  float CameraPositionZ;
  unsigned int v_rand;
  int CameraRotateX;
  int IdCube;
  int CameraRotateY;
  int XYZCube;
  int XYZCubeNumber;
};





















static MinecraftData* data;











static int Minecraft_random()
{
  data->v_rand = data->v_rand * 1103515245 + 12345;
  return ((unsigned int)(data->v_rand / 65536) % 32768);
}
static void Minecraft_rendermap(int x1, int z1)
{
  data->v_rand = 1;
  x1 = (x1 % 50);
  z1 = (z1 % 50);
  int height = 50;
  for (int y1 = 0; y1 < 200; y1++) {
    float v = (float)11.22 - ((float)(Minecraft_random() % 707) / (float)100);
    float v2 = (float)50 - ((float)(Minecraft_random() % 10000) / (float)100);
    float v3 = (float)50 - ((float)(Minecraft_random() % 10000) / (float)100);
    float v4 = (float)0.5 - ((float)(Minecraft_random() % 5000) / (float)10000);
    int v5 = (v * v) - (((v2 - x1) * (v2 - x1) + (v3 - z1) * (v3 - z1)) * v4);
    if (max(height, v5) == v5)
      height = v5;
  }
  for (int y1 = 0; y1 < 127; y1++)
    data->MapHight[y1] = (0);
  for (int y1 = 0; y1 <= height; y1++)
  {

    if ( y1 < 50) {
      data->MapHight[y1] = (4);

    } else {

      if (  height - 1 == y1) {
        data-> MapHight[y1] = (2);
      }
      if (  height - 1 > y1) {
        data->MapHight[y1] = (3);
      }
    }
  }
}





























static void Minecraft_prepare()
{ /* Здесь нужно инициализировать переменные */

  game_set_ups(20);
  data->XYZCube = 0;
  data->IdCube = 0;
  data->q = 10;
  game_enable_frame_buffer();
  data->x = 1;
  data->y = 45;

  data->CameraPositionX = 0;// -∞ .. ∞
  //YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
  Minecraft_rendermap(0, 0);
  int myy = 127;
  while ( data->MapHight[myy] == data->MapHight[127])
    myy--;
  data->CameraPositionY = myy + 3; // -∞ .. ∞
  //YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
  data->CameraPositionZ = 0;// -∞ .. ∞

  data->CameraRotateX = 0;// 0 .. 360
  data->CameraRotateY = 89;// -90 .. 90



}

static void Minecraft_render()
{
}

static float Minecraft_sin(int sinx)
{
  if ((sinx >= 0) && (sinx < 359)) {
    return ( ((int)pgm_read_byte(&mysinarray[sinx]) - (int)128) / (float)100);
  }

  if ((sinx >= -359) && (sinx < 0)) {
    return ( ((int)pgm_read_byte(&mysinarray[sinx + 359]) - (int)128) / (float)100);
  }
  //game_draw_text("error",32,32,RED,TRANSPARENT);
  game_draw_digits(sinx, 32, 32, 6, RED );
}

static float Minecraft_cos(int cosx)
{

  return ( Minecraft_sin(cosx + 90) );

}
static void fillTopFlatTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint8_t color)
{ if (y1 != y2)
    return;
  float invslope1 = (x3 - x1) / (y3 - y1 + 0.0001);
  float invslope2 = (x3 - x2) / (y3 - y2 + 0.0001);
  float curx1 = x3;
  float curx2 = x3;

  for (int scanlineY = y3; scanlineY > y1; scanlineY--)
  {
    for (int x0 = max(min((int)curx1, (int)curx2), 0); x0 < min(max((int)curx1, (int)curx2), 64); x0++)
      if (scanlineY > 0 && scanlineY < 64)
        if (game_get_pixel(x0, scanlineY) == BLACK) {
          game_draw_pixel(x0, scanlineY, color);
          if (x0 == 32 && scanlineY == 32) {
            data->selectblockX = data->drawblockX;
            data->selectblockY = data->drawblockY;
            data->selectblockZ = data->drawblockZ;
            game_draw_pixel(1, 1, RED);
          }
        }
    curx1 -= invslope1;
    curx2 -= invslope2;
  }

}

static void fillBottomFlatTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint8_t color)
{ if (y2 != y3)
    return;
  float invslope1 = (x2 - x1) / (y2 - y1 + 0.0001);
  float invslope2 = (x3 - x1) / (y3 - y1 + 0.0001);
  float curx1 = x1;
  float curx2 = x1;

  for (int scanlineY = y1; scanlineY <= y2; scanlineY++)
  {

    for (int x0 = max(min((int)curx1, (int)curx2), 0); x0 < min(max((int)curx1, (int)curx2), 64); x0++)
      if (scanlineY > 0 && scanlineY < 64)
        if (game_get_pixel(x0, scanlineY) == BLACK) {
          game_draw_pixel(x0, scanlineY, color);
          if (x0 == 32 && scanlineY == 32) {
            data->selectblockX = data->drawblockX;
            data->selectblockY = data->drawblockY;
            data->selectblockZ = data->drawblockZ;
            game_draw_pixel(1, 1, RED);
          }
        }

    // game_draw_pixel((int)curx2, scanlineY, BLUE);
    curx1 += invslope1;
    curx2 += invslope2;
  }

}
static void Minecraft_DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint8_t color)
{
  if ((color == BLACK) && (color < 256))
    return;
  if (true) {
    int k;
    if (y1 > y2) {
      k = y1;
      y1 = y2;
      y2 = k;

      k = x1;
      x1 = x2;
      x2 = k;
    }
    if (y2 > y3) {
      k = y2;
      y2 = y3;
      y3 = k;

      k = x2;
      x2 = x3;
      x3 = k;
    }
    if (y1 > y2) {
      k = y1;
      y1 = y2;
      y2 = k;

      k = x1;
      x1 = x2;
      x2 = k;
    }


  }

  if (y2 == y3)
  {
    fillBottomFlatTriangle(x1, y1, x2, y2, x3, y3, color);
  }

  else if (y1 == y2)
  {
    fillTopFlatTriangle( x1, y1, x2, y2, x3, y3, color);
  } else

  {




    int v41 = (int)((float)x1 + (((float)(y2 - y1) / (float)(y3 - y1)) * (float)(x3 - x1)));

    int v42 =  y2;
    fillBottomFlatTriangle( x1, y1, x2, y2, v41, v42, color);
    fillTopFlatTriangle( x2, y2, v41, v42, x3, y3, color);
  }
  // game_draw_digits(invslope2, 50, 50, 6, BLUE );
  /*
    for (int y0 = max(min(min(y1, y2), y3), 0); y0 < min( max(max(y1, y2), y3), 64); y0++) {

    for (int x0 = max(min(min(x1, x2), x3), 0); x0 < min(max(max(x1, x2), x3), 64); x0++) {
    int a = (x1 - x0) * (y2 - y1) - (x2 - x1) * (y1 - y0);
    int b = (x2 - x0) * (y3 - y2) - (x3 - x2) * (y2 - y0);
    int c = (x3 - x0) * (y1 - y3) - (x1 - x3) * (y3 - y0);

    if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0))
    {


      if (game_get_pixel(x0, y0) == BLACK)
        if (data->IdCube > 0) {
          game_draw_pixel(x0, y0, color);
      //    data->IdCube--;
          if (x0 == 32 && y0 == 32) {
            data->selectblockX = data->drawblockX;
            data->selectblockY = data->drawblockY;
            data->selectblockZ = data->drawblockZ;
            //game_draw_pixel(1, 1, RED);
          }
        }
      // delay(1);
    }

    }
    }*/
  /*int b1, k1;
    int b2, k2;
    int b3, k3;
    int y11, y12;
    float test1, test2;

    test1 = -y1 - y2;
    test2 = -x1 - x2;

    if (test2 != 0)
    {
    k1 = (test1 / test2) * 100;
    b1 = y1 - ((float)(k1 / 100.0) * x1);
    }
    else
    {
    k1 = 0;
    b1 = y1 - ((float)(k1 / 100.0) * x1);
    }

    test1 = -y2 - y3;
    test2 = -x2 - x3;

    if (test2 != 0)
    {
    k2 = (test1 / test2) * 100;
    b2 = y2 - ((float)(k2 / 100.0) * x2);
    }
    else
    {
    k2 = 0;
    b2 = y2 - ((float)(k2 / 100.0) * x2);
    }

    test1 = -y3 - y1;
    test2 = -x3 - x1;

    if (test2 != 0)
    {
    k3 = (test1 / test2) * 100;
    b3 = y3 - ((float)(k3 / 100.0) * x3);
    }
    else
    {
    k3 = 0;
    b3 = y3 - ((float)(k3 / 100.0) * x3);
    }


    for (int i1 = min(min(x1, x2), x3); i1 < max(max(x1, x2), x3); i1++) {
    y11 = -1;
    y12 = -1;
    if ((min(x1, x2) <= i1) && (i1 < max(x1, x2) )) {
      y11 = (-((float)(k1 / 100.0) * i1) - (b1));
    }
    if ((min(x1, x3) <= i1) && ( i1 < max(x1, x3) )) {
      if (y11 == -1) {
        y11 = (-((float)(k3 / 100.0) * i1) + (b3)) ;
      } else {
        y12 = (-((float)(k3 / 100.0) * i1) + (b3)) ;
      }
    }
    if ((min(x2, x3) <= i1) && (i1 < max(x2, x3) )) {
      y12 = (-((float)(k2 / 100.0) * i1) - (b2)) ;
    }
    game_draw_vline(i1, min( y11,  y12) ,  max( y11,  y12) , color);
    }*/

}
static void Minecraft_Draw3DTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, uint8_t color)
{
  x1 = x1 + data->CameraPositionX;
  y1 = - y1 + data->CameraPositionY;
  z1 = z1 + data->CameraPositionZ;

  x2 = x2 + data->CameraPositionX;
  y2 = - y2 + data->CameraPositionY;
  z2 = z2 + data->CameraPositionZ;

  x3 = x3 + data->CameraPositionX;
  y3 = - y3 + data->CameraPositionY;
  z3 = z3 + data->CameraPositionZ;
  float qx1, qy1, qz1, qx2, qy2, qz2, qx3, qy3, qz3;
  float wx1, wy1, wz1, wx2, wy2, wz2, wx3, wy3, wz3;
  //up down

  float myX, /*myY,*/ myZ, myA;
  myX = Minecraft_cos(data->CameraRotateX);
  //  myY = 0;
  myZ = Minecraft_sin(data->CameraRotateX);
  myA = data->CameraRotateY;
  //неоптимизированный отросток

  qx1 = (x1 * (Minecraft_cos(myA) + (  (1 - Minecraft_cos(myA)) * myX * myX)) + y1 * (   - (Minecraft_sin(myA) * myZ)  ) + z1 * (  (1 - Minecraft_cos(myA)) * myX * myZ   )) ;
  qy1 = (x1 * (   (Minecraft_sin(myA) * myZ)  ) + y1 * (  Minecraft_cos(myA)   ) + z1 * (   - (Minecraft_sin(myA) * myX)  )) ;
  qz1 = (x1 * (  (1 - Minecraft_cos(myA)) * myZ * myX   ) + y1 * (  (Minecraft_sin(myA) * myX)  ) + z1 * (   Minecraft_cos(myA) + (1 - Minecraft_cos(myA)) * myZ * myZ   ));

  qx2 = (x2 * (Minecraft_cos(myA) + (  (1 - Minecraft_cos(myA)) * myX * myX)) + y2 * (   - (Minecraft_sin(myA) * myZ)  ) + z2 * (  (1 - Minecraft_cos(myA)) * myX * myZ   )) ;
  qy2 = (x2 * (Minecraft_sin(myA) * myZ) +            y2 * (Minecraft_cos(myA))      +        z2 * (-Minecraft_sin(myA) * myX)      );
  qz2 = (x2 * (  (1 - Minecraft_cos(myA)) * myZ * myX   ) + y2 * (   (Minecraft_sin(myA) * myX)  ) + z2 * (   Minecraft_cos(myA) + (1 - Minecraft_cos(myA)) * myZ * myZ   ));

  qx3 = (x3 * (Minecraft_cos(myA) + (  (1 - Minecraft_cos(myA)) * myX * myX)) + y3 * (   - (Minecraft_sin(myA) * myZ)  ) + z3 * (  (1 - Minecraft_cos(myA)) * myX * myZ   )) ;
  qy3 = (x3 * (  (Minecraft_sin(myA) * myZ)  ) + y3 * (  Minecraft_cos(myA)  ) + z3 * (   - (Minecraft_sin(myA) * myX)  ) );
  qz3 = (x3 * (  (1 - Minecraft_cos(myA)) * myZ * myX   ) + y3 * (  (Minecraft_sin(myA) * myX)  ) + z3 * (   Minecraft_cos(myA) + (1 - Minecraft_cos(myA)) * myZ * myZ   ));

  wx1 = (qx1 * Minecraft_cos(data->CameraRotateX) + qz1 * Minecraft_sin(data->CameraRotateX)) ;
  wy1 = (qy1);
  wz1 = (- qx1 * Minecraft_sin(data->CameraRotateX) + qz1 * Minecraft_cos(data->CameraRotateX));

  wx2 = (qx2 * Minecraft_cos(data->CameraRotateX) + qz2 * Minecraft_sin(data->CameraRotateX));
  wy2 = (qy2)  ;
  wz2 = (- qx2 * Minecraft_sin(data->CameraRotateX) + qz2 * Minecraft_cos(data->CameraRotateX));

  wx3 = (qx3 * Minecraft_cos(data->CameraRotateX) + qz3 * Minecraft_sin(data->CameraRotateX));
  wy3 = qy3  ;
  wz3 = (- qx3 * Minecraft_sin(data->CameraRotateX) + qz3 * Minecraft_cos(data->CameraRotateX))  ;


  x1 = wx1;
  y1 = wy1;
  z1 = wz1;

  x2 = wx2;
  y2 = wy2;
  z2 = wz2;

  x3 = wx3;
  y3 = wy3;
  z3 = wz3;

  if ((z1 > 0) && (z2 > 0) && (z3 > 0)) {
    wx1 = x1 / z1;
    wy1 = y1 / z1;


    wx2 = x2 / z2;
    wy2 = y2 / z2;


    wx3 = x3 / z3;
    wy3 = y3 / z3;

    if ((wx1 < 1) && (wx1 > -1))
      if ((wx1 < 1) && (wx1 > -1))


        if ((wx2 < 1) && (wx2 > -1))
          if ((wx2 < 1) && (wx2 > -1))


            if ((wx3 < 1) && (wx3 > -1))
              if ((wx3 < 1) && (wx3 > -1))

                Minecraft_DrawTriangle((wx1 * 64) + 32, (wy1 * 64) + 32, (wx2 * 64) + 32, (wy2 * 64) + 32,    (wx3 * 64) + 32, (wy3 * 64) + 32, color);
  }
}
static void Minecraft_Draw3DCube(int x1, int y1, int z1, uint8_t color)
{
  /*    BLACK - Чёрный
    BLUE - Синий
    GREEN - Зелёный
    RED - Красный
    CYAN - Циановый
    PURPLE - Фиолетовый
    BROWN - Коричневый
    WHITE - Белый*/

  /*  if (data->IdCube <= 0) {
      return;
    }
    if (data->XYZCubeNumber > 0) {
      data->XYZCubeNumber--;
      return;
    }
    //:=0;
    data->IdCube--;
    data->XYZCube++;
    if (color == 0) {
      return;
    }*/
  if ((x1 == 0) && (y1 == 0) && (z1 == 0)) {
    return;
  }
  //game_draw_digits(color , 5, 0, 6, BLUE);
  //game_draw_pixel(1, 1, );
  data->drawblockX = x1;
  data->drawblockY = y1;
  data->drawblockZ = z1;
  if (data->CameraPositionZ > -z1 + 0.5 ) {
    Minecraft_Draw3DTriangle(x1 + 0.5, y1 + 0.5, z1 - 0.5,   x1 + 0.5, y1 - 0.5, z1 - 0.5,    x1 - 0.5, y1 + 0.5, z1 - 0.5   , (uint8_t)pgm_read_byte(&Minecraft_Texture[color * 12 + 0]));
    Minecraft_Draw3DTriangle(x1 - 0.5, y1 - 0.5, z1 - 0.5,   x1 + 0.5, y1 - 0.5, z1 - 0.5,    x1 - 0.5, y1 + 0.5, z1 - 0.5   , (uint8_t)pgm_read_byte(&Minecraft_Texture[color * 12 + 1])); //передняя
  }
  if (data->CameraPositionZ < -z1 - 0.5 ) {
    Minecraft_Draw3DTriangle(x1 + 0.5, y1 + 0.5, z1 + 0.5,   x1 + 0.5, y1 - 0.5, z1 + 0.5,    x1 - 0.5, y1 + 0.5, z1 + 0.5   , (uint8_t)pgm_read_byte(&Minecraft_Texture[color * 12 + 2]));
    Minecraft_Draw3DTriangle(x1 - 0.5, y1 - 0.5, z1 + 0.5,   x1 + 0.5, y1 - 0.5, z1 + 0.5,    x1 - 0.5, y1 + 0.5, z1 + 0.5   , (uint8_t)pgm_read_byte(&Minecraft_Texture[color * 12 + 3])); //задняя
  }

  if (data->CameraPositionX > -x1 + 0.5) {
    Minecraft_Draw3DTriangle(x1 - 0.5, y1  - 0.5, z1 - 0.5,   x1 - 0.5, y1 - 0.5, z1 + 0.5,    x1 - 0.5, y1 + 0.5, z1 - 0.5   , (uint8_t)pgm_read_byte(&Minecraft_Texture[color * 12 + 4]));
    Minecraft_Draw3DTriangle(x1 - 0.5, y1  + 0.5, z1 - 0.5,   x1 - 0.5, y1 - 0.5, z1 + 0.5,    x1 - 0.5, y1 + 0.5, z1 + 0.5   , (uint8_t)pgm_read_byte(&Minecraft_Texture[color * 12 + 5])); //левая
  }
  if (data->CameraPositionX < -x1 - 0.5) {
    Minecraft_Draw3DTriangle(x1 + 0.5, y1 + 0.5, z1 - 0.5,   x1 + 0.5, y1 + 0.5, z1 + 0.5,    x1 + 0.5, y1 - 0.5, z1 - 0.5   , (uint8_t)pgm_read_byte(&Minecraft_Texture[color * 12 + 6]));
    Minecraft_Draw3DTriangle(x1 + 0.5, y1 - 0.5, z1 - 0.5,   x1 + 0.5, y1 + 0.5, z1 + 0.5,    x1 + 0.5, y1 - 0.5, z1 + 0.5   , (uint8_t)pgm_read_byte(&Minecraft_Texture[color * 12 + 7])); //правая
  }
  if (data->CameraPositionY < y1 - 0.5 ) {
    Minecraft_Draw3DTriangle(x1 - 0.5, y1  - 0.5, z1 - 0.5,   x1 - 0.5, y1 - 0.5, z1 + 0.5,    x1 + 0.5, y1 - 0.5, z1 - 0.5   , (uint8_t)pgm_read_byte(&Minecraft_Texture[color * 12 + 8]));
    Minecraft_Draw3DTriangle(x1 + 0.5, y1  - 0.5, z1 + 0.5,   x1 - 0.5, y1 - 0.5, z1 + 0.5,    x1 + 0.5, y1 - 0.5, z1 - 0.5   , (uint8_t)pgm_read_byte(&Minecraft_Texture[color * 12 + 9])); //верхния
  }
  if (data->CameraPositionY > y1 + 0.5) {
    Minecraft_Draw3DTriangle(x1 - 0.5, y1 + 0.5, z1 - 0.5,   x1 - 0.5, y1 + 0.5, z1 + 0.5,    x1 + 0.5, y1 + 0.5, z1 - 0.5   , (uint8_t)pgm_read_byte(&Minecraft_Texture[color * 12 + 10]));
    Minecraft_Draw3DTriangle(x1 + 0.5, y1 + 0.5, z1 + 0.5,   x1 - 0.5, y1 + 0.5, z1 + 0.5,    x1 + 0.5, y1 + 0.5, z1 - 0.5   , (uint8_t)pgm_read_byte(&Minecraft_Texture[color * 12 + 11])); //нижняя
  }


}

static void Minecraft_renderscrean()
{
  game_clear_screen();
  data->selectblockX = 0;
  data->selectblockY = 0;
  data->selectblockZ = 0;
  data->XYZCube = 0;
}
static void Minecraft_TimeIsButton(unsigned long delta)
{
  if  (game_is_button_pressed (BUTTON_A)) {
    Minecraft_renderscrean();
    data->CameraPositionX = data->CameraPositionX + (Minecraft_sin(data->CameraRotateX) * delta / 500);
    data->CameraPositionY = data->CameraPositionY - (Minecraft_sin(data->CameraRotateY) * delta / 500);
    data->CameraPositionZ = data->CameraPositionZ - (Minecraft_cos(data->CameraRotateX) * delta / 500);
  }
  if (data->x < 180)
    if  (game_is_button_pressed (BUTTON_LEFT)) {
      Minecraft_renderscrean();
      data->x += delta / 10;
    }
  if (data->x > -180)
    if  ( game_is_button_pressed (BUTTON_RIGHT)) {
      Minecraft_renderscrean();
      data->x -= delta / 10;
    }

  if (data->y < 90)
    if ( game_is_button_pressed (BUTTON_DOWN)) {
      Minecraft_renderscrean();
      data->y += delta / 10;
    }
  if (data->y > -90)

    if  (game_is_button_pressed (BUTTON_UP)) {


      Minecraft_renderscrean();
      data->y -= delta / 10;
    }
  if (data->x > 178) {
    data->x = -177;
  }
  if (data->x < -178) {
    data->x = 177;
  }
  data->CameraRotateX = data->x;
  data->CameraRotateY = data->y;
}
static void Minecraft_update(unsigned long delta)
{
  uint8_t fps = 1000 / delta;
  //game_draw_digits(data->q, 5, 32, 14, BLACK);
  /*  if    ((fps < 20) && (fps > 3) && (data->q < 100)) {
      data->q = data->q * 2;
    }

    if ((fps <= 3) && (data->q > 1)) {
      data->q = round( data->q / 2);

    }*/
  data->IdCube = 1; // data->q;

  if (game_is_any_button_pressed(65535))
    Minecraft_TimeIsButton(delta);


  //-------------------------------------GUI------------------
  //-------------------------------------GUI------------------
  game_draw_text("select:", 0, 0, GREEN);
  game_draw_text("FPS", 0, 54, GREEN);
  game_draw_rect(20, 56, 7, 5, BLACK);
  game_draw_digits(fps, 2, 20, 56, GREEN);
  game_draw_text("DrB", 30, 54, GREEN);
  game_draw_rect(52, 56, 11, 5, BLACK);
  game_draw_digits(data->XYZCube, 3, 52, 56, GREEN);
  game_draw_digits(abs(data->selectblockX), 5,  0, 10,  GREEN);
  game_draw_digits(data->selectblockY, 5, 21,  10, GREEN);
  game_draw_digits(abs(data->selectblockZ), 5,  42, 10,  GREEN);
  //-------------------------------------GUI------------------
  //-------------------------------------GUI------------------
  //x-лево право y-высота z-даль
  //data->XYZCubeNumber = data->XYZCube;
  if (data->XYZCube < 1000) {
    data->XYZCube++;

    int myx = round(-data->CameraPositionX) + ((((int)data->XYZCube / (int)10) % 10) - 5);
    int myy = round(data->CameraPositionY) + ((((int)data->XYZCube) % 10) - 5);
    int myz = round(-data->CameraPositionZ) + ((((int)data->XYZCube / (int)100) % 10) - 5);

    Minecraft_rendermap(myx,  myz);
    // game_draw_digits(data->x, 2, 0, 40, WHITE);
    if ((myy < 126)&&(myy > 0))
      Minecraft_Draw3DCube( myx, myy, myz, data->MapHight[myy] );
  }
  /*for (int myx = round(-data->CameraPositionX) ; myx < round(-data->CameraPositionX) + 5 ; myx++)
    for (int myz = round(-data->CameraPositionZ) ; myz < round(-data->CameraPositionZ) + 5 ; myz++) {
      Minecraft_rendermap(myx,  myz);
        Minecraft_DrawTriangle(random(64),random(64),random(64),random(64),random(64),random(64),RED);
      for (int myy = round(data->CameraPositionY); myy > round(data->CameraPositionY) - 5; myy--)
        Minecraft_Draw3DCube( myx, myy, myz, data->MapHight[myy] );
      for (int myy = round(data->CameraPositionY); myy < round(data->CameraPositionY) + 5; myy++)
        Minecraft_Draw3DCube( myx, myy, myz, data->MapHight[myy] );
    }
    for (int myx = round(-data->CameraPositionX) ; myx > round(-data->CameraPositionX) - 5 ; myx--)
    for (int myz = round(-data->CameraPositionZ) ; myz > round(-data->CameraPositionZ) - 5 ; myz--) {
      Minecraft_rendermap(myx,  myz);
      for (int myy = round(data->CameraPositionY); myy > round(data->CameraPositionY) - 10; myy--)
        Minecraft_Draw3DCube( myx, myy, myz, data->MapHight[myy] );
      for (int myy = round(data->CameraPositionY); myy < round(data->CameraPositionY) + 10; myy++)
        Minecraft_Draw3DCube( myx, myy, myz, data->MapHight[myy] );
    }
    for (int myx = round(-data->CameraPositionX) ; myx > round(-data->CameraPositionX) - 5 ; myx--)
    for (int myz = round(-data->CameraPositionZ) ; myz < round(-data->CameraPositionZ) + 5 ; myz++) {
      Minecraft_rendermap(myx,  myz);
      for (int myy = round(data->CameraPositionY); myy > round(data->CameraPositionY) - 10; myy--)
        Minecraft_Draw3DCube( myx, myy, myz, data->MapHight[myy] );
      for (int myy = round(data->CameraPositionY); myy < round(data->CameraPositionY) + 10; myy++)
        Minecraft_Draw3DCube( myx, myy, myz, data->MapHight[myy] );
    }
    for (int myx = round(-data->CameraPositionX) ; myx < round(-data->CameraPositionX) + 5 ; myx++)
    for (int myz = round(-data->CameraPositionZ) ; myz > round(-data->CameraPositionZ) - 5 ; myz--) {
      Minecraft_rendermap(myx,  myz);
      for (int myy = round(data->CameraPositionY); myy > round(data->CameraPositionY) - 10; myy--)
        Minecraft_Draw3DCube( myx, myy, myz, data->MapHight[myy] );
      for (int myy = round(data->CameraPositionY); myy < round(data->CameraPositionY) + 10; myy++)
        Minecraft_Draw3DCube( myx, myy, myz, data->MapHight[myy] );
    }*/

  // for (int myx = 0 ; myx < 64; myx++)
  //  Minecraft_DrawTriangle(64, 30, 1 , 60, 10, myx, RED);
  //game_clear_screen();
  //Minecraft_DrawTriangle(34, 0, 34, 63,  data->x, data->y, RED);
  //game_draw_digits(data->x, 2, 0, 40, WHITE);
  //game_draw_digits(data->y, 2, 32, 40, WHITE);


  //game_draw_digits(data->CameraRotateX, 5, 32, 0, BLUE);
  //game_draw_digits(data->CameraRotateY, 5, 32, 6, BLUE);
  //game_draw_digits(data->CameraRotateZ, 5, 32, 12, BLUE);

  //game_draw_digits((data->CameraPositionY), 5, 21, 6, BLUE);
  game_draw_pixel(32, 32, RED);


  /* Здесь код, который будет выполняться в цикле */
  /* Переменная delta содержит количество миллисекунд с последнего вызова */

  /* Здесь можно работать с кнопками и обновлять переменные */
}

const game_instance Minecraft PROGMEM = {
  "Minecraft",         /* Название игры.
                           Используется как имя записи в EEPROM.
                           Не больше 10 символов */
  Minecraft_prepare,
  Minecraft_render,
  Minecraft_update,
  sizeof(MinecraftData),
  (void**)(&data)
};


/* Не забудьте зарегистрировать игру в application.cpp, libgame.h */
/* Также нужно добавить файл с игрой в qt_emulator/CMakeLists.txt */
