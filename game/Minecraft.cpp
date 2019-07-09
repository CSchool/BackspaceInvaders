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
#define WaterLine 60
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
  GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR, GR,/*1*///камень
  BR, BR, BR, BR, BR, BR, BR, BR, BR, BR, G, G,/*2*///трава
  BR2, BR2, BR2, BR2, BR2, BR2, BR2, BR2, BR2, BR2, BR2, BR2,/*3*///земля
  B, B, B, B, B, B, B, B, B, B, B, B/*4*///вода
};



const uint8_t mysinarray[] PROGMEM = {
  127, 129, 130, 132, 134, 136, 137, 139, 141, 143, 144, 146, 148, 149, 151, 153, 155, 156, 158, 160, 161, 163, 164, 166, 168, 169, 171, 172, 174, 175, 177, 179, 180, 181, 183, 184, 186, 187, 189, 190, 191, 193, 194, 195, 196, 198, 199, 200, 201, 202, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 214, 215, 216, 217, 218, 218, 219, 220, 220, 221, 222, 222, 223, 223, 224, 224, 224, 225, 225, 225, 226, 226, 226, 226, 227, 227, 227, 227, 227, 227, 227, 227, 227, 227, 227, 226, 226, 226, 226, 225, 225, 225, 224, 224, 224, 223, 223, 222, 222, 221, 220, 220, 219, 218, 218, 217, 216, 215, 214, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 202, 201, 200, 199, 198, 196, 195, 194, 193, 191, 190, 189, 187, 186, 184, 183, 181, 180, 179, 177, 175, 174, 172, 171, 169, 168, 166, 164, 163, 161, 160, 158, 156, 155, 153, 151, 150, 148, 146, 144, 143, 141, 139, 137, 136, 134, 132, 130, 129, 127, 125, 124, 122, 120, 118, 117, 115, 113, 111, 110, 108, 106, 105, 103, 101, 99, 98, 96, 94, 93, 91, 90, 88, 86, 85, 83, 82, 80, 79, 77, 76, 74, 73, 71, 70, 68, 67, 65, 64, 63, 61, 60, 59, 58, 56, 55, 54, 53, 52, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 40, 39, 38, 37, 36, 36, 35, 34, 34, 33, 32, 32, 31, 31, 30, 30, 30, 29, 29, 29, 28, 28, 28, 28, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 29, 29, 29, 30, 30, 30, 31, 31, 32, 32, 33, 34, 34, 35, 36, 36, 37, 38, 39, 40, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 52, 53, 54, 55, 56, 58, 59, 60, 61, 63, 64, 65, 67, 68, 70, 71, 73, 74, 75, 77, 79, 80, 82, 83, 85, 86, 88, 90, 91, 93, 94, 96, 98, 99, 101, 103, 104, 106, 108, 110, 111, 113, 115, 117, 118, 120, 122, 123, 125, 127
};



struct Vector3D
{
  int x;
  int y;
  int z;
};



//pgm_read_byte(&mysinarray[sinx]);
//**********************DATA**********************DATA**********************DATA**********************DATA**********************
//**********************DATA**********************DATA**********************DATA**********************DATA**********************
//**********************DATA**********************DATA**********************DATA**********************DATA**********************
//**********************DATA**********************DATA**********************DATA**********************DATA**********************
//**********************DATA**********************DATA**********************DATA**********************DATA**********************
//**********************DATA**********************DATA**********************DATA**********************DATA**********************
//**********************DATA**********************DATA**********************DATA**********************DATA**********************
//**********************DATA**********************DATA**********************DATA**********************DATA**********************
struct MinecraftData
{
  int i;
  int x;
  int y;
  float q;
  int SaveX;
  //uint8_t MapHight[128];

  uint8_t RenderBlocks[16][16];
  //int RenderBlocksPoint;

  float matrixRotateX[3][3];
  float matrixRotateY[3][3];
  int selectblockX;
  int selectblockY;
  int selectblockZ;
  int drawblockX;
  int drawblockY;
  int drawblockZ;
  float CameraPositionX;
  float CameraPositionY;
  float CameraPositionZ;
  int v_rand;
  float CameraRotateX;
  int IdCube;
  float CameraRotateY;
  int XYZCube;
  int XYZCubeNumber;
  uint8_t fps;
  Vector3D UxUy, Uxy, xUy, xy;
};


static MinecraftData* data;
//**********************DATA**********************DATA**********************DATA**********************DATA**********************
//**********************DATA**********************DATA**********************DATA**********************DATA**********************
//**********************DATA**********************DATA**********************DATA**********************DATA**********************
//**********************DATA**********************DATA**********************DATA**********************DATA**********************
//**********************DATA**********************DATA**********************DATA**********************DATA**********************
//**********************DATA**********************DATA**********************DATA**********************DATA**********************
//**********************DATA**********************DATA**********************DATA**********************DATA**********************
//**********************DATA**********************DATA**********************DATA**********************DATA**********************

//**********************SIN**********************SIN**********************SIN**********************SIN**********************
static float Minecraft_sin(int sinx)
{
  sinx = sinx % 360;
  if ((sinx >= 0) && (sinx <= 360)) {
    return ( ((int)pgm_read_byte(&mysinarray[sinx]) - (int)128) * (float)0.01);
  }

  if ((sinx >= -359) && (sinx < 0)) {
    return ( ((int)pgm_read_byte(&mysinarray[sinx + 359]) - (int)128) * (float)0.01);
  }
  //game_draw_text("error",32,32,RED,TRANSPARENT);
  game_draw_digits(sinx, 6, 32, 32, RED );
}
//**********************SIN**********************SIN**********************SIN**********************SIN**********************
//**********************COS**********************COS**********************COS**********************COS**********************
static float Minecraft_cos(int cosx)
{

  return ( Minecraft_sin(cosx + 90) );

}
//**********************COS**********************COS**********************COS**********************COS**********************
//**********************RAND**********************RAND**********************RAND**********************RAND**********************
static int Minecraft_random()
{
  data->v_rand = (data->v_rand * 419 + 6173) % 32000;
  return abs(data->v_rand % 29282);
}

//**********************RAND**********************RAND**********************RAND**********************RAND**********************
//**********************MAP**********************MAP**********************MAP**********************MAP**********************
static uint8_t Minecraft_rendermap()
{
  int x1;
  int z1;
  data->v_rand = 29;
  // x1 = (x1 % 50);
  // z1 = (z1 % 50);
  //uint8_t myheight = WaterLine + 10;
  for (int myx = 0; myx < 16; myx++)
    for (int myz = 0; myz < 16; myz++)
      data->RenderBlocks[myx][myz] = 20;

  for (int myi = 0; myi <= 10; myi++) {
    float v = (float)9.21 - ((float)(Minecraft_random() % 473) / (float)100);
    float v2 = (float)50 - ((float)(Minecraft_random() % 10000) / (float)100);
    float v3 = (float)50 - ((float)(Minecraft_random() % 10000) / (float)100);
    float v4 = (float)0.5 - ((float)(Minecraft_random() % 3000) / (float)10000);


    for (int myx = 0; myx < 16; myx++)
      for (int myz = 0; myz < 16; myz++) {
        int  x1 = data->CameraPositionX + myx - 8;
        int  z1 = data->CameraPositionZ + myz - 8;
        x1 = (x1 % 50);
        z1 = (z1 % 50);

        int v5 = round( (v * v) - (((v2 - x1) * (v2 - x1) + (v3 - z1) * (v3 - z1)) * v4) );
        if (data->RenderBlocks[myx][myz] < v5)
          data->RenderBlocks[myx][myz] = v5;
      }
  }

  // for (int myz = 0; myz < 16; myz++)
  //     game_draw_pixel(myz , data->RenderBlocks[8][myz]/4, RED);
  for (int myx = 0; myx < 16; myx++)
    for (int myz = 0; myz < 16; myz++)
      // if (false)
      if (data->RenderBlocks[myx][myz] > 84)
        data->RenderBlocks[myx][myz] = 84;


  return data->RenderBlocks[8][8];
  // game_draw_pixel(32 + x1, 32 + z1, RED);
}
static uint8_t Minecraft_getmap(int x1, int z1)
{
  return (data->RenderBlocks[x1][z1] % 85);
}
static void Minecraft_setviewmap(int x1, int z1, uint8_t a)
{
  if (!((0 <= a) && (a <= 2 )))
  {
    return;
  }
  /* if (a == true)
     if (data->RenderBlocks[myx][myz] < 127)
       data->RenderBlocks[myx][myz] = data->RenderBlocks[myx][myz] + 127;
    if (a == false)
     if (data->RenderBlocks[myx][myz] > 127)
       data->RenderBlocks[myx][myz] = data->RenderBlocks[myx][myz] + 127;*/
  data->RenderBlocks[x1][z1] =  Minecraft_getmap(x1, z1);
  if (a == 1)
    data->RenderBlocks[x1][z1] = data->RenderBlocks[x1][z1] + 85;
  if (a == 2)
    data->RenderBlocks[x1][z1] = data->RenderBlocks[x1][z1] + 85 + 85;
}
static uint8_t Minecraft_getviewmap(uint8_t x1, uint8_t z1)
{
  if (data->RenderBlocks[x1][z1] < 85)
    return 0;
  else if (data->RenderBlocks[x1][z1] < 85 + 85)
    return 1;
  else
    return 2;
}
static void Minecraft_clearmap()
{

  for (int myx = 0; myx < 16; myx++)
    for (int myz = 0; myz < 16; myz++)
      Minecraft_setviewmap(myx, myz, 0);



}

//**********************MAP**********************MAP**********************MAP**********************MAP**********************



//**********************FrustumCulling**********************FrustumCulling**********************FrustumCulling**********************FrustumCulling**********************
static bool Minecraft_FrustumCulling(float x, float y, float z)
{
  x = x + data->CameraPositionX;
  y = - y + data->CameraPositionY;
  z = z + data->CameraPositionZ;

  float qx, qy, qz;
  bool q = false;
  qx = (x * data->matrixRotateX[0][0] + y * data->matrixRotateX[1][0] + z * data->matrixRotateX[2][0]);
  qy = (x * data->matrixRotateX[0][1] + y * data->matrixRotateX[1][1] + z * data->matrixRotateX[2][1]);
  qz = (x * data->matrixRotateX[0][2] + y * data->matrixRotateX[1][2] + z * data->matrixRotateX[2][2]);

  x = (qx * data->matrixRotateY[0][0] + qz * data->matrixRotateY[2][0]);
  y = (qy);
  z = (qx * data->matrixRotateY[0][2] + qz * data->matrixRotateY[2][2]);

  if (z > 0) {
    qx = x / z * 0.99;
    qy = y / z * 0.99;
    if ((qx < 1) && (qx > -1))
      if ((qy < 1) && (qy > -1))
        q = true;

  }

  return (q);
}
//**********************FrustumCulling**********************FrustumCulling**********************FrustumCulling**********************FrustumCulling**********************

























//**********************DRAW**********************DRAW**********************DRAW**********************DRAW**********************
//**********************DRAW**********************DRAW**********************DRAW**********************DRAW**********************
//**********************DRAW**********************DRAW**********************DRAW**********************DRAW**********************
//**********************DRAW**********************DRAW**********************DRAW**********************DRAW**********************
//**********************DRAW**********************DRAW**********************DRAW**********************DRAW**********************
//**********************DRAW**********************DRAW**********************DRAW**********************DRAW**********************
//**********************DRAW**********************DRAW**********************DRAW**********************DRAW**********************
//**********************DRAW**********************DRAW**********************DRAW**********************DRAW**********************
static void fillTopFlatTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint8_t color)
{ if (y1 != y2)
    return;
  float invslope1 = (x3 - x1) / (y3 - y1 + 0.0001);
  float invslope2 = (x3 - x2) / (y3 - y2 + 0.0001);
  float curx1 = x3;
  float curx2 = x3;

  for (int scanlineY = y3; scanlineY > y1; scanlineY--)
  { if (scanlineY >= 0 && scanlineY <= 64)
      for (int x0 = max(min((int)curx1, (int)curx2), 0); x0 < min(max((int)curx1, (int)curx2), 64); x0++)

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
{ //if (y2 != y3)
  //  return;

  float invslope1 = (x2 - x1) / (y2 - y1 + 0.0001);
  float invslope2 = (x3 - x1) / (y3 - y1 + 0.0001);
  float curx1 = x1;
  float curx2 = x1;

  for (int scanlineY = y1; scanlineY <= y2; scanlineY++)
  {
    // game_draw_pixel( min((int)curx1, (int)curx2),scanlineY, color);
    //game_draw_pixel( max((int)curx1, (int)curx2),scanlineY, color);
    if ((scanlineY >= 0) && (scanlineY <= 64) )
      for (int x0 = max(min((int)curx1, (int)curx2), 0); x0 < min(max((int)curx1, (int)curx2), 64); x0++)

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
static void Minecraft_DrawTriangle(int myx1, int myy1, int myx2, int myy2, int myx3, int myy3, uint8_t color)
{
  if ((color == BLACK))
    return;
  int x1, y1, x2, y2, x3, y3;
  x1 = myx1;
  y1 = myy1;
  x2 = myx2;
  y2 = myy2;
  x3 = myx3;
  y3 = myy3;

  if (true) {
    int k = 0;
    for (int i = 0; i < 3; i++) {
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
    }



  }

  if (y2 == y3)
  {
    fillBottomFlatTriangle(x1, y1, x2, y2, x3, y3, color);

  }

  else if (y1 == y2)
  {
    fillTopFlatTriangle( x1, y1, x2, y2, x3, y3, color);
  } else if ((y3 - y1) != 0)
  {


    int v41 = ((float)x1 + (((float)(y2 - y1) / (float)(y3 - y1)) * (float)(x3 - x1)));

    int v42 =  y2;
    //game_draw_pixel( v41, v42, RED);
    fillBottomFlatTriangle( x1, y1, x2, y2, v41, v42, color);
    fillTopFlatTriangle( x2, y2, v41, v42, x3, y3, color);
  }
  // game_draw_digits(invslope2, 50, 50, 6, BLUE );

  /*for (int y0 = max(min(min(y1, y2), y3), 0); y0 < min( max(max(y1, y2), y3), 64); y0++) {

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



/* qx1 = (x1 * (Minecraft_cos(myA) + (  (1 - Minecraft_cos(myA)) * myX * myX)) + y1 * (   - (Minecraft_sin(myA) * myZ)  ) + z1 * (  (1 - Minecraft_cos(myA)) * myX * myZ   )) ;
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
    wz3 = (- qx3 * Minecraft_sin(data->CameraRotateX) + qz3 * Minecraft_cos(data->CameraRotateX))  ;*/


static bool Minecraft_3DTo2DUsingRotation(float xyz[][3], int xy[][2], int sizexyz)
{

  //int sizexyz = sizeof(xyz) / sizeof(xyz[0]);

  for (int i = 0; i < sizexyz ; i++)
  {
    xyz[i][0] = xyz[i][0] + data->CameraPositionX;
    xyz[i][1] = - xyz[i][1] + data->CameraPositionY;
    xyz[i][2] = xyz[i][2] + data->CameraPositionZ;

    float qx, qy, qz;
    qx = (xyz[i][0] * data->matrixRotateX[0][0] + xyz[i][1] * data->matrixRotateX[1][0] + xyz[i][2] * data->matrixRotateX[2][0]);
    qy = (xyz[i][0] * data->matrixRotateX[0][1] + xyz[i][1] * data->matrixRotateX[1][1] + xyz[i][2] * data->matrixRotateX[2][1]);
    qz = (xyz[i][0] * data->matrixRotateX[0][2] + xyz[i][1] * data->matrixRotateX[1][2] + xyz[i][2] * data->matrixRotateX[2][2]);

    xyz[i][0] = (qx * data->matrixRotateY[0][0] + qz * data->matrixRotateY[2][0]);
    xyz[i][1] = (qy);
    xyz[i][2] = (qx * data->matrixRotateY[0][2] + qz * data->matrixRotateY[2][2]);

    if ( xyz[i][2] > 0)   {
      float q2x, q2y;
      q2x = xyz[i][0]  / xyz[i][2];
      q2y = xyz[i][1]  / xyz[i][2];
      if ((q2x < 3) && (q2x > -3) && (q2y < 3) && (q2y > -3))
        // if (true)
      {
        //--------------------------------------------------------------------------------------------
        xy[i][0] = q2x * 32 + 32;
        xy[i][1] = q2y * 32 + 32;
        // game_draw_pixel( xy[i][0], xy[i][1], BLUE);

      } else {
        return false;
      }

    } else {
      return false;
    }

  }
  return true;
}
static void Minecraft_Draw3DTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, uint8_t color)
{
  float xyz[3][3];
  int xy[3][2];
  xyz[0][0] = x1;
  xyz[0][1] = y1;
  xyz[0][2] = z1;

  xyz[1][0] = x2;
  xyz[1][1] = y2;
  xyz[1][2] = z2;

  xyz[2][0] = x3;
  xyz[2][1] = y3;
  xyz[2][2] = z3;
  if (Minecraft_3DTo2DUsingRotation(xyz, xy, 3)) {
    Minecraft_DrawTriangle(xy[0][0], xy[0][1], xy[1][0] , xy[1][1] ,    xy[2][0] , xy[2][1] , color);
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
    data->XYZCube++;*/

  if (color == 0) {
    return;
  }
  if ((x1 == 0) && (y1 == 0) && (z1 == 0)) {
    return;
  }
  //color = 4;
  // game_draw_digits(color, 3, 32, 32, RED);

  float xyz[8][3];
  int xy[8][2];
  xyz[0][0] = x1 - 0.5;
  xyz[0][1] = y1 - 0.5;
  xyz[0][2] = z1 - 0.5;

  xyz[1][0] = x1 + 0.5;
  xyz[1][1] = y1 - 0.5;
  xyz[1][2] = z1 - 0.5;

  xyz[2][0] = x1 - 0.5;
  xyz[2][1] = y1 + 0.5;
  xyz[2][2] = z1 - 0.5;

  xyz[3][0] = x1 + 0.5;
  xyz[3][1] = y1 + 0.5;
  xyz[3][2] = z1 - 0.5;

  xyz[4][0] = x1 - 0.5;
  xyz[4][1] = y1 - 0.5;
  xyz[4][2] = z1 + 0.5;

  xyz[5][0] = x1 + 0.5;
  xyz[5][1] = y1 - 0.5;
  xyz[5][2] = z1 + 0.5;

  xyz[6][0] = x1 - 0.5;
  xyz[6][1] = y1 + 0.5;
  xyz[6][2] = z1 + 0.5;

  xyz[7][0] = x1 + 0.5;
  xyz[7][1] = y1 + 0.5;
  xyz[7][2] = z1 + 0.5;


  if (!Minecraft_3DTo2DUsingRotation(xyz, xy, 8)) {
    return;
  }






  //game_draw_digits(color , 5, 0, 6, BLUE);
  //game_draw_pixel(1, 1, );
  data->drawblockX = x1;
  data->drawblockY = y1;
  data->drawblockZ = z1;
  /*if (data->CameraPositionZ > -z1 + 0.5 ) {
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
    Minecraft_Draw3DTriangle(x1 + 0.5, y1  - 0.5, z1 + 0.5,   x1 - 0.5, y1 - 0.5, z1 + 0.5,    x1 + 0.5, y1 - 0.5, z1 - 0.5   , (uint8_t)pgm_read_byte(&Minecraft_Texture[color * 12 + 9])); //нижняя
    }
    if (data->CameraPositionY > y1 + 0.5) {
    Minecraft_Draw3DTriangle(x1 - 0.5, y1 + 0.5, z1 - 0.5,   x1 - 0.5, y1 + 0.5, z1 + 0.5,    x1 + 0.5, y1 + 0.5, z1 - 0.5   , (uint8_t)pgm_read_byte(&Minecraft_Texture[color * 12 + 10]));
    Minecraft_Draw3DTriangle(x1 + 0.5, y1 + 0.5, z1 + 0.5,   x1 - 0.5, y1 + 0.5, z1 + 0.5,    x1 + 0.5, y1 + 0.5, z1 - 0.5   , (uint8_t)pgm_read_byte(&Minecraft_Texture[color * 12 + 11])); //верхния
    }*/

  //  game_draw_pixel(32+color, 32+color, RED);
  //color = 2;

  if (data->CameraPositionZ > -z1 + 0.5 ) {
    Minecraft_DrawTriangle(xy[3][0] * 1, xy[3][1] * 1, xy[1][0] * 1 , xy[1][1] * 1 ,    xy[2][0] * 1 , xy[2][1] * 1 , pgm_read_byte(&Minecraft_Texture[color * 12 + 0])  );
    Minecraft_DrawTriangle(xy[0][0] * 1, xy[0][1] * 1, xy[1][0] * 1 , xy[1][1] * 1 ,    xy[2][0] * 1 , xy[2][1] * 1 , pgm_read_byte(&Minecraft_Texture[color * 12 + 1])  ); //передняя
  }
  if (data->CameraPositionZ < -z1 - 0.5 ) {
    Minecraft_DrawTriangle(xy[7][0], xy[7][1], xy[5][0] , xy[5][1] ,    xy[6][0] , xy[6][1] , pgm_read_byte(&Minecraft_Texture[color * 12 + 2])  );
    Minecraft_DrawTriangle(xy[4][0], xy[4][1], xy[5][0] , xy[5][1] ,    xy[6][0] , xy[6][1] , pgm_read_byte(&Minecraft_Texture[color * 12 + 3])  ); //задняя
  }
  if (data->CameraPositionX > -x1 + 0.5) {
    Minecraft_DrawTriangle(xy[0][0], xy[0][1], xy[4][0] , xy[4][1] ,    xy[2][0] , xy[2][1] , pgm_read_byte(&Minecraft_Texture[color * 12 + 4])  );
    Minecraft_DrawTriangle(xy[2][0], xy[2][1], xy[4][0] , xy[4][1] ,    xy[6][0] , xy[6][1] , pgm_read_byte(&Minecraft_Texture[color * 12 + 5])  ); //левая
  }
  if (data->CameraPositionX < -x1 - 0.5) {
    Minecraft_DrawTriangle(xy[3][0], xy[3][1], xy[7][0] , xy[7][1] ,    xy[1][0] , xy[1][1] , pgm_read_byte(&Minecraft_Texture[color * 12 + 6]));
    Minecraft_DrawTriangle(xy[1][0], xy[1][1], xy[7][0] , xy[7][1] ,    xy[5][0] , xy[5][1] , pgm_read_byte(&Minecraft_Texture[color * 12 + 7])); //правая
  }
  if (data->CameraPositionY < y1 - 0.5 ) {
    Minecraft_DrawTriangle(xy[0][0], xy[0][1], xy[4][0] , xy[4][1] ,    xy[1][0] , xy[1][1] , pgm_read_byte(&Minecraft_Texture[color * 12 + 8])  );
    Minecraft_DrawTriangle(xy[5][0], xy[5][1], xy[4][0] , xy[4][1] ,    xy[1][0] , xy[1][1] , pgm_read_byte(&Minecraft_Texture[color * 12 + 9])  ); //нижняя
  }
  if (data->CameraPositionY > y1 + 0.5) {
    Minecraft_DrawTriangle(xy[2][0], xy[2][1], xy[6][0] , xy[6][1] ,    xy[3][0] , xy[3][1] , pgm_read_byte(&Minecraft_Texture[color * 12 + 10])  );
    Minecraft_DrawTriangle(xy[7][0], xy[7][1], xy[6][0] , xy[6][1] ,    xy[3][0] , xy[3][1] , pgm_read_byte(&Minecraft_Texture[color * 12 + 11])  ); //верхния
  }
}
//**********************DRAW**********************DRAW**********************DRAW**********************DRAW**********************
//**********************DRAW**********************DRAW**********************DRAW**********************DRAW**********************
//**********************DRAW**********************DRAW**********************DRAW**********************DRAW**********************
//**********************DRAW**********************DRAW**********************DRAW**********************DRAW**********************
//**********************DRAW**********************DRAW**********************DRAW**********************DRAW**********************
//**********************DRAW**********************DRAW**********************DRAW**********************DRAW**********************
//**********************DRAW**********************DRAW**********************DRAW**********************DRAW**********************
//**********************DRAW**********************DRAW**********************DRAW**********************DRAW**********************

static uint8_t Minecraft_Draw3DCubesWithMap(int8_t myx, int8_t myz) {
  int mypx = round(-data->CameraPositionX) - myx;
  int mypy = Minecraft_getmap(myx + 8, myz + 8);
  int mypz = round(-data->CameraPositionZ) - myz;

  if ( Minecraft_FrustumCulling( mypx, mypy, mypz ) || Minecraft_FrustumCulling( mypx, data->CameraPositionY - 1, mypz ) || Minecraft_FrustumCulling( mypx, data->CameraPositionY, mypz ) || Minecraft_FrustumCulling( mypx, data->CameraPositionY + 1, mypz )  ) {
    //if (true){

    //  game_draw_pixel(mypx,mypz,mypy);
    if (mypy > WaterLine)
      Minecraft_Draw3DCube(   mypx, mypy, mypz , 2);
    else
      Minecraft_Draw3DCube(   mypx, WaterLine , mypz , 4);


    return 2;
  } else
    return 1;


}
















static void Minecraft_renderscrean()
{
  game_clear_screen();
  data->selectblockX = 0;
  data->selectblockY = 0;
  data->selectblockZ = 0;
  data->XYZCube = 0;
  data->SaveX=99;

  float myX, /*myY,*/ myZ, myA;
  myX = Minecraft_cos(data->CameraRotateX);
  //  myY = 0;
  myZ = Minecraft_sin(data->CameraRotateX);
  myA = data->CameraRotateY;
  //неоптимизированный отросток
  data->matrixRotateX[0][0] = (Minecraft_cos(myA) + (  (1 - Minecraft_cos(myA)) * myX * myX));
  data->matrixRotateX[1][0] = (   - (Minecraft_sin(myA) * myZ)  ) ;
  data->matrixRotateX[2][0] = (  (1 - Minecraft_cos(myA)) * myX * myZ   ) ;

  data->matrixRotateX[0][1] = (   (Minecraft_sin(myA) * myZ)  );
  data->matrixRotateX[1][1] = (  Minecraft_cos(myA)   ) ;
  data->matrixRotateX[2][1] = (   - (Minecraft_sin(myA) * myX)  );

  data->matrixRotateX[0][2] = (  (1 - Minecraft_cos(myA)) * myZ * myX   );
  data->matrixRotateX[1][2] = (  (Minecraft_sin(myA) * myX)  );
  data->matrixRotateX[2][2] = (   Minecraft_cos(myA) + (1 - Minecraft_cos(myA)) * myZ * myZ   );

  data->matrixRotateY[0][0] =  Minecraft_cos(data->CameraRotateX);
  data->matrixRotateY[2][0] = Minecraft_sin(data->CameraRotateX);
  data->matrixRotateY[0][2] = -  Minecraft_sin(data->CameraRotateX);
  data->matrixRotateY[2][2] = Minecraft_cos(data->CameraRotateX);

  //Minecraft_Draw3DCube(- data->CameraPositionX, data->CameraPositionY, -data->CameraPositionZ + 2, 4);
  // data->RenderBlocksPoint = 0;
  //for (int i = 0; i < 64; i++) {
  //  data->RenderBlocks[i][0] = 0;
  //  data->RenderBlocks[i][1] = 0;
  //  data->RenderBlocks[i][2] = 0;
  //}
  if ((data->UxUy.x != data->CameraPositionX) || (data->UxUy.z != data->CameraPositionZ)) {
    data->UxUy.x = data->CameraPositionX;
    data->UxUy.y = 0;
    data->UxUy.z = data->CameraPositionZ;

    Minecraft_rendermap();
  } else {
    Minecraft_clearmap();
  }
  Minecraft_setviewmap(8, 8, 2);
}







static void Minecraft_TimeIsButton()
{
  bool renderscrean = false;
  if  (game_is_button_pressed (BUTTON_A)) {
    renderscrean = true;
    data->CameraPositionX +=  (Minecraft_sin(data->CameraRotateX) * 0.126);
    data->CameraPositionY -=  (Minecraft_sin(data->CameraRotateY) * 0.126);
    data->CameraPositionZ -=  (Minecraft_cos(data->CameraRotateX) * 0.126);
    Minecraft_rendermap();
  }
  if (data->CameraRotateX <= 180)
    if  (game_is_button_pressed (BUTTON_LEFT)) {
      renderscrean = true;
      //data->CameraRotateX += delta / 10;
      data->CameraRotateX += 2;
    }
  if (data->CameraRotateX >= -180)
    if  ( game_is_button_pressed (BUTTON_RIGHT)) {
      renderscrean = true;
      //data->CameraRotateX -= delta / 10;
      data->CameraRotateX -= 2;
    }

  if (data->CameraRotateY <= 90)
    if ( game_is_button_pressed (BUTTON_DOWN)) {
      renderscrean = true;
      // data->CameraRotateY += delta / 10;
      data->CameraRotateY += 2;
    }
  if (data->CameraRotateY >= -90)

    if  (game_is_button_pressed (BUTTON_UP)) {
      renderscrean = true;
      //data->CameraRotateY -= delta / 10;
      data->CameraRotateY -= 2;
    }


  if (data->CameraRotateX > 178) {
    data->CameraRotateX = -177;
  }
  if (data->CameraRotateX < -178) {
    data->CameraRotateX = 177;
  }
  if (renderscrean == true) {
    Minecraft_renderscrean();
  }
  // data->CameraRotateX = data->x;
  //data->CameraRotateY = data->y;
}























static void Minecraft_prepare()
{ /* Здесь нужно инициализировать переменные */

  game_set_ups(60);
  data->XYZCube = 0;
  data->IdCube = 0;
  data->q = 10;
  game_enable_frame_buffer();
  data->x = 0;
  data->y = 45;

  data->CameraPositionX = 0;// -∞ .. ∞
  //YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
  data->CameraPositionY =  Minecraft_rendermap() + 3; // -∞ .. ∞
  //YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
  data->CameraPositionZ = 0;// -∞ .. ∞

  data->CameraRotateX = 0;// -180 .. 180
  data->CameraRotateY = 45;// -90 .. 90

  Minecraft_renderscrean();
}

static void Minecraft_render()
{
  //-------------------------------------GUI------------------
  //-------------------------------------GUI------------------
  game_draw_text((const uint8_t*)"select:", 0, 0, GREEN);
  game_draw_text((const uint8_t*)"FPS", 0, 54, GREEN);
  game_draw_rect(20, 56, 7, 5, BLACK);
  game_draw_digits(data->fps, 2, 20, 56, BLUE);
  game_draw_text((const uint8_t*)"Ren  %", 30, 54, GREEN);
  game_draw_rect(48, 56, 11, 5, BLACK);
  game_draw_digits(round(data->XYZCube * 6.25), 3, 48, 56, BLUE);
   game_draw_digits(abs(data->selectblockX), 5,  0, 10,  BLUE);
   game_draw_digits(data->selectblockY, 5, 21,  10, BLUE);
   game_draw_digits(abs(data->selectblockZ), 5,  42, 10,  BLUE);
  /*game_draw_digits(data->CameraRotateX, 5,  0, 10,  BLUE);
  game_draw_digits(data->CameraRotateY, 5, 21,  10, BLUE);
  game_draw_digits(data->CameraPositionY, 5,  42, 10,  BLUE);*/

  game_draw_pixel(32, 32, RED);
  //-------------------------------------GUI------------------
  //-------------------------------------GUI------------------
}


static void Minecraft_update(unsigned long delta)
{

  if (delta > 0)
    data->fps = 1000 / (abs(delta));
  /*  for (int i = 0; i < 16; i++)
      for (int j = 0; j < 16; j++)
        data->RenderBlocks[i][j] = 95;
    for (int i = 0; i < 16; i++)
      for (int j = 0; j < 16; j++)
        data->RenderBlocks[i][j] = data->RenderBlocks[i][j] - 1;*/
  //data->IdCube = 0; // data->q;
  /*if (data->XYZCube < 256) {
    data->XYZCube++;
    //if ((data->XYZCube < 256)&& (data->XYZCube >0)) {

    int x =  ((data->XYZCube / (int)16) % 16);

    int z = ((data->XYZCube) % 16);
    game_draw_pixel(x,z,RED);
    if ((x >= 0) && (z >= 0) && (x < 16) && (z < 16))
    for (int y=0;y<255;y++)
      if ((data->RenderBlocks[x][z] > 0) && (data->RenderBlocks[x][z] < 250))
      {
        //int y =  data->RenderBlocks[x][z];
        int realX = round(-data->CameraPositionX) + x - 8;
        int realZ = round(-data->CameraPositionZ) + z - 8;
         if (Minecraft_FrustumCulling(realX, y, realZ)){

        game_draw_digits(data->RenderBlocks[x][z], 5, 32, 0, BLUE);
        Minecraft_Draw3DCube( realX , y,  realZ , 1);
         }
        // }else{
        //   Minecraft_update(delta);
        //   }
      }
    }*/


  //x-лево право y-высота z-даль
  //data->XYZCubeNumber = data->XYZCube;
  /*
     if (data->XYZCube < 999)
      for (int myx = 0 ; myx < 1 ; myx++)
       {
       bool iii = true ;
       while ((data->XYZCube < 1000) && (iii == true)){

           data->XYZCube++;
           int myx, myy, myz, ModXYZCube = data->XYZCube % 125;
           if ((0 <= data->XYZCube) && (data->XYZCube < 125))
           {
             myx = round(-data->CameraPositionX) + (  (   (ModXYZCube / (int)5) % 5)  );
             myy = round( data->CameraPositionY) + (  (   (ModXYZCube) % 5)  );
             myz = round(-data->CameraPositionZ) + (  (   (ModXYZCube / (int)25) % 5)  );
           } else if ((125 <= data->XYZCube) && (data->XYZCube < 250))
           {
             myx = round(-data->CameraPositionX) + (  (  -(ModXYZCube / (int)5) % 5)  );
             myy = round( data->CameraPositionY) + (  (   (ModXYZCube) % 5)  );
             myz = round(-data->CameraPositionZ) + (  (   (ModXYZCube / (int)25) % 5)  );
           } else if ((250 <= data->XYZCube) && (data->XYZCube < 375))
           {
             myx = round(-data->CameraPositionX) + (  (   (ModXYZCube / (int)5) % 5)  );
             myy = round( data->CameraPositionY) + (  (  -(ModXYZCube) % 5)  );
             myz = round(-data->CameraPositionZ) + (  (   (ModXYZCube / (int)25) % 5)  );
           } else if ((375 <= data->XYZCube) && (data->XYZCube < 500))
           {
             myx = round(-data->CameraPositionX) + (  (  -(ModXYZCube / (int)5) % 5)  );
             myy = round( data->CameraPositionY) + (  (  -(ModXYZCube) % 5)  );
             myz = round(-data->CameraPositionZ) + (  (   (ModXYZCube / (int)25) % 5)  );
           } else if ((500 <= data->XYZCube) && (data->XYZCube < 625))
           {
             myx = round(-data->CameraPositionX) + (  (   (ModXYZCube / (int)5) % 5)  );
             myy = round( data->CameraPositionY) + (  (   (ModXYZCube) % 5)  );
             myz = round(-data->CameraPositionZ) + (  (  -(ModXYZCube / (int)25) % 5)  );
           } else if ((625 <= data->XYZCube) && (data->XYZCube < 750))
           {
             myx = round(-data->CameraPositionX) + (  (  -(ModXYZCube / (int)5) % 5)  );
             myy = round( data->CameraPositionY) + (  (   (ModXYZCube) % 5)  );
             myz = round(-data->CameraPositionZ) + (  (  -(ModXYZCube / (int)25) % 5)  );
           } else if ((750 <= data->XYZCube) && (data->XYZCube < 875))
           {
             myx = round(-data->CameraPositionX) + (  (   (ModXYZCube / (int)5) % 5)  );
             myy = round( data->CameraPositionY) + (  (  -(ModXYZCube) % 5)  );
             myz = round(-data->CameraPositionZ) + (  (  -(ModXYZCube / (int)25) % 5)  );
           } else if ((875 <= data->XYZCube) && (data->XYZCube < 1000))
           {
             myx = round(-data->CameraPositionX) + (  (  -(ModXYZCube / (int)5) % 5)  );
             myy = round( data->CameraPositionY) + (  (  -(ModXYZCube) % 5)  );
             myz = round(-data->CameraPositionZ) + (  (  -(ModXYZCube / (int)25) % 5)  );
           }


           if (Minecraft_FrustumCulling(myx, myy, myz)) {
            // Minecraft_rendermap(myx,  myz);
             // game_draw_digits(data->x, 2, 0, 40, WHITE);
             if ((myy < 126) && (myy > 0)){
             // DrawCubeWithMap(myx, myy, myz);
               //Minecraft_Draw3DCube( myx, myy, myz, data->MapHight[myy]data->RenderBlocks[8][8] );
               game_draw_pixel(32 + myx, 32 + myz, myy);
             }
             iii = false;
           }
         }
       }

  */
  //Minecraft_Draw3DCube( 2, 2, 2, 1 );
  // for (int myx = 0 ; myx < 64; myx++)
  //  Minecraft_DrawTriangle(64, 30, 1 , 60, 10, myx, RED);
  //game_clear_screen();
  //Minecraft_DrawTriangle(34, 0, 34, 63,  data->x, data->y, RED);
  // game_draw_digits(memoryFree(), 6, 32, 0, RED);
  //game_draw_digits(data->y, 2, 32, 40, WHITE);


  // game_draw_digits(data->CameraRotateX, 5, 32, 0, BLUE);
  // game_draw_digits(data->CameraRotateY, 5, 32, 6, BLUE);
  //  game_draw_digits(data->CameraRotateZ, 5, 32, 12, BLUE);

  //game_draw_digits((data->CameraPositionY), 5, 21, 6, BLUE);

  if (true)
    if (delta > 0)
      if (game_is_any_button_pressed(-1))
        Minecraft_TimeIsButton();
  /* Здесь код, который будет выполняться в цикле */
  /* Переменная delta содержит количество миллисекунд с последнего вызова */

  /* Здесь можно работать с кнопками и обновлять переменные */


  // data->XYZCube++;

  /*  if     ((data->CameraRotateX <= -135) || (data->CameraRotateX > 135))
     Minecraft_Draw3DCube(  0 + 5 * (Minecraft_sin(data->CameraRotateX) / Minecraft_cos(data->CameraRotateX))  , 10+ 5 * (Minecraft_sin(-data->CameraRotateY) / Minecraft_cos(-data->CameraRotateY))  ,   0 - 5 , 2);
    if     ((data->CameraRotateX <= 45) && (data->CameraRotateX > -45))
      Minecraft_Draw3DCube(  0 - 5 * (Minecraft_sin(data->CameraRotateX) / Minecraft_cos(data->CameraRotateX))  , 10+ 5 * (Minecraft_sin(-data->CameraRotateY) / Minecraft_cos(-data->CameraRotateY))  ,   0 + 5 , 2);
    if     ((data->CameraRotateX <= -45) && (data->CameraRotateX > -135))
      Minecraft_Draw3DCube(  0 + 5  , 10+ 5 * (Minecraft_sin(-data->CameraRotateY) / Minecraft_cos(-data->CameraRotateY))  ,   0 - 5 * (Minecraft_cos(data->CameraRotateX)/Minecraft_sin(data->CameraRotateX)), 2);
    if     ((data->CameraRotateX <= 135) && (data->CameraRotateX > 45))
      Minecraft_Draw3DCube(  0 - 5  , 10+ 5 * (Minecraft_sin(-data->CameraRotateY) / Minecraft_cos(-data->CameraRotateY))  ,   0 + 5 * (Minecraft_cos(data->CameraRotateX)/Minecraft_sin(data->CameraRotateX)), 2);
  */
  //for (int y = 0; y < 10; y++)
  // Minecraft_Draw3DCube(  5 - y ,  10 + y , 0, 2);
  /*if (data->CameraRotateY < -45)
    Minecraft_Draw3DCube(  round(-data->CameraPositionX) , round(data->CameraPositionY) + 10 ,   round(-data->CameraPositionZ)  , 3);
    else if (data->CameraRotateY > 45)
    Minecraft_Draw3DCube(  round(-data->CameraPositionX) , round(data->CameraPositionY) - 10 ,   round(-data->CameraPositionZ)  , 3);
    else if ((data->CameraRotateX < 45) && (data->CameraRotateX > -45))
    Minecraft_Draw3DCube(  round(-data->CameraPositionX) , round(data->CameraPositionY)  ,   round(-data->CameraPositionZ) + 10 , 3);
    else if ((data->CameraRotateX < 135) && (data->CameraRotateX > 45))
    Minecraft_Draw3DCube(  round(-data->CameraPositionX) - 10 , round(data->CameraPositionY) ,   round(-data->CameraPositionZ)  , 3);
    else if ((data->CameraRotateX < -45) && (data->CameraRotateX > -135))
    Minecraft_Draw3DCube(  round(-data->CameraPositionX) + 10 , round(data->CameraPositionY) ,   round(-data->CameraPositionZ)  , 3);
    else
    Minecraft_Draw3DCube(  round(-data->CameraPositionX) , round(data->CameraPositionY)  ,   round(-data->CameraPositionZ) - 10 , 3);
  */
  //if (data->XYZCube < 15) {
  while (data->XYZCube<=15) {
    if (data->XYZCube == 0)
      Minecraft_Draw3DCubesWithMap(0, 0);
int a = data->XYZCube % 15;
   if  (a<=data->SaveX){ 
    data->XYZCube ++;

data->SaveX=-(data->XYZCube % 15);
     }
    // for (int i=0;i< data->XYZCube/(int)2;i++){
    //    int myx = 0;
    //    int myz = 0;

     a = data->XYZCube % 15;
    //for (int x = -a; a >= x; x++) {
while(a>data->SaveX){
  data->SaveX=data->SaveX+1;
    bool retu = false;
    int x=data->SaveX;
    if (x < 1)
      if (x > -9) {
        if ((8 + x + a >= 0) and (8 + x + a < 16)) {
          int myx = x + 8;
          int myz = 8 + x + a;
          if ((Minecraft_getviewmap(myx - 1, myz) == 2) || (Minecraft_getviewmap(myx, myz - 1) == 2) || (Minecraft_getviewmap(myx + 1, myz) == 2) || (Minecraft_getviewmap(myx, myz + 1) == 2)) if (Minecraft_getviewmap(myx, myz) == 0)
              Minecraft_setviewmap(myx, myz, Minecraft_Draw3DCubesWithMap(myx - 8, myz - 8) );
retu = true;
          //game_draw_pixel(myx,myz,RED);
        }
        if (x > -a)
          if ((8 - x - a >= 0) and (8 - x - a < 16))
          {
            int myx = x + 8;
            int myz = 8 - x - a;
            if ((Minecraft_getviewmap(myx - 1, myz) == 2) || (Minecraft_getviewmap(myx, myz - 1) == 2) || (Minecraft_getviewmap(myx + 1, myz) == 2) || (Minecraft_getviewmap(myx, myz + 1) == 2)) if (Minecraft_getviewmap(myx, myz) == 0)
                Minecraft_setviewmap(myx, myz, Minecraft_Draw3DCubesWithMap(myx - 8, myz - 8) );
retu = true;
 
            // game_draw_pixel(myx,myz,RED);
          }

      }

    if (x > 0)
      if (x < 8)
      {
        if ((8 + x - a >= 0) and (8 + x - a < 16))
        {
          int myx = x + 8;
          int myz = 8 + x - a;
          if ((Minecraft_getviewmap(myx - 1, myz) == 2) || (Minecraft_getviewmap(myx, myz - 1) == 2) || (Minecraft_getviewmap(myx + 1, myz) == 2) || (Minecraft_getviewmap(myx, myz + 1) == 2)) if (Minecraft_getviewmap(myx, myz) == 0)
              Minecraft_setviewmap(myx, myz, Minecraft_Draw3DCubesWithMap(myx - 8, myz - 8) );
              retu = true;
          // game_draw_pixel(myx,myz,RED);
        }

        if (x < a)
          if ((8 - x + a >= 0) and (8 - x + a < 16))
          {
            int myx = x + 8;
            int myz = 8 - x + a;
            if ((Minecraft_getviewmap(myx - 1, myz) == 2) || (Minecraft_getviewmap(myx, myz - 1) == 2) || (Minecraft_getviewmap(myx + 1, myz) == 2) || (Minecraft_getviewmap(myx, myz + 1) == 2)) if (Minecraft_getviewmap(myx, myz) == 0)
                Minecraft_setviewmap(myx, myz, Minecraft_Draw3DCubesWithMap(myx - 8, myz - 8) );
                retu = true;
            //game_draw_pixel(myx,myz,RED);
          }

      }
      if (retu==true)
      return;
      

  }





  //if ((Minecraft_getviewmap(myx - 1, myz) == 2) || (Minecraft_getviewmap(myx, myz - 1) == 2) || (Minecraft_getviewmap(myx + 1, myz) == 2) || (Minecraft_getviewmap(myx, myz + 1) == 2)) if (Minecraft_getviewmap(myx, myz) == 0) {
  // Minecraft_setviewmap(myx, myz, Minecraft_Draw3DCubesWithMap(myx - 8, myz - 8) );
  //}
  //return;
  //}
  //}
}
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
