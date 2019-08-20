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
 */
    const uint8_t krestikup_lines[] PROGMEM = {
    B11100000, B00000000, B01110000,
    B11110000, B00000000, B11110000,
    B11111000, B00000001, B11110000,
    B01111100, B00000011, B11100000,
    B00111110, B00000111, B11000000,
    B00011111, B00001111, B10000000,
    B00001111, B10011111, B00000000,
    B00000111, B11111110, B00000000,
    B00000011, B11111100, B00000000,
    B00000001, B11111000, B00000000,
   
};

const game_sprite krestikup PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    20, 10, krestikup_lines
};


/*
 * ------------------------------
xxx       xxx
 xxx     xxx
  xxx   xxx
    xxxxx
  xxx   xxx
 xxx     xxx
xxx       xxx


 --------------------------------
 */

    const uint8_t krestikdow_lines[] PROGMEM = {
    B00000001, B11111000, B00000000,
    B00000011, B11111100, B00000000,
    B00000111, B11111110, B00000000,
    B00001111, B10011111, B00000000,
    B00011111, B00001111, B10000000,
    B00111110, B00000111, B11000000,
    B01111100, B00000011, B11100000,
    B11111000, B00000001, B11110000,
    B11110000, B00000000, B11110000,
    B11100000, B00000000, B01110000,
   
};

const game_sprite krestikdow PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    20, 10, krestikdow_lines
};

   const uint8_t nolikup_lines[] PROGMEM = {
    B00000001, B11111000, B00000000,
    B00000111, B11111110, B00000000,
    B00011111, B00001111, B10000000,
    B00111100, B00000011, B11000000,
    B00110000, B00000000, B11000000,
    B01100000, B00000000, B01100000,
    B01100000, B00000000, B01100000,
    B11000000, B00000000, B00110000,
    B11000000, B00000000, B00110000,
    B11000000, B00000000, B00110000,
   
};

const game_sprite nolikup PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    20, 10, nolikup_lines
};
const uint8_t nolikdow_lines[] PROGMEM = {
    B11000000, B00000000, B00110000,
    B11000000, B00000000, B00110000,
    B11000000, B00000000, B00110000,
    B01100000, B00000000, B01100000,
    B01100000, B00000000, B01100000,
    B00110000, B00000000, B11000000,
    B00111100, B00000011, B11000000,
    B00011111, B00001111, B10000000,
    B00000011, B11111100, B00000000,
    B00000001, B11111000, B00000000

};

const game_sprite nolikdow PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    20, 10, nolikdow_lines
};


/*const uint8_t kursor_lines[] PROGMEM = {
    B00000000, B00000000,
    B00000000, B00000000, 
    B00000000, B00000000,
    B00000000, B00000000, 
    B00000000, B00000000, 
    B00000000, B00000000, 
    B00000000, B00000000, 
    B00000000, B00000000, 
    B00000000, B00000000, 
    B00000000, B00000000

};


const game_sprite kursor PROGMEM = {
    // ШИРИНА, ВЫСОТА, КОЛИЧЕСТВО БАЙТ НА СТРОКУ, ДАННЫЕ
    20, 10, 3, kursor_lines
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


 bool is_Win( bool &flag );
 bool is_Lose( bool &flag );
 bool is_draw();
 void reload();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// queue class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////



enum WHATs_INSIDE
{
    CROSS = -1,
    NOTHING,
    TOE
};

const int SIZE = 2;

struct kek
{
    WHATs_INSIDE inside;
    int index;
};

bool operator==(kek a, kek b)
{
    return ((a.inside == b.inside) && (a.index == b.index));
}
class queue
{

    kek A[SIZE];
  
    int first;
    int last; 

public:
  
    queue()
    {
        last = first = 0;
    }
  
    ~queue(){}

    kek front() { return A[0];  }
    kek back()  { return A[1];  }
  
    bool push(WHATs_INSIDE x, int index);

  
};



bool queue::push(WHATs_INSIDE x, int index)
{
    if (last + 1  ==  first)
    {
        if (A[0] == A[1])
        {
            return true;
        }
    }
    else
    {
        A[(last) % SIZE].inside = x;
        A[(last++) % SIZE].index = index;
        return false;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// variables
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// data
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Tic_Tac_toeData
{
    bool joystick[3][3];

    WHATs_INSIDE field[3][3];

    int x, y;

    bool xod_igroka;
    bool menu, DRAW, LOSE, WIN;
    long long pause;
    bool flafg;
};
static Tic_Tac_toeData* data; /* Эта переменная - указатель на структуру, которая содержит ваши переменные */

static void Tic_Tac_toe_prepare()
{
    game_set_ups(10);
    data->xod_igroka = true;
    data->flafg = false;
    data->menu = data->DRAW = data->LOSE = data->WIN = false;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            data->field[i][j] = NOTHING;
            data->joystick[i][j] = false;    
        }
    }
    data->x = data->y = 0;
    data->pause = 0;
}

static void Tic_Tac_toe_render()
{
    if(!data->menu)
    {    
        game_draw_vline(20, 0, 63, WHITE); // vertical line
        game_draw_vline(21, 0, 63, WHITE); // vertical line
        game_draw_vline(43, 0, 63, WHITE); // vertical line
        game_draw_vline(42, 0, 63, WHITE); // vertical line
        game_draw_rect(0, 20, 64, 2, WHITE);
        game_draw_rect(0, 42, 64, 2, WHITE);
    
        /* Здесь код, который будет вывзваться для отрисовки кадра */
        /* Он не должен менять состояние игры, для этого есть функция update */
    
        /* Здесь (и только здесь) нужно вызывать функции game_draw_??? */
    
    /*  game_draw_sprite(&krestikup, 0, 0, RED);  //      КРЕСТИК
        game_draw_sprite(&krestikdow, 0, 10, RED);//      КРЕСТИК
    */
    
    /*
        game_draw_sprite(&nolikup, 22, 0, BLUE);         НОЛИК
        game_draw_sprite(&nolikdow, 22, 10, BLUE);       НОЛИК
    */
    
        if(44>=data->x && data->x>=0 && 44>=data->y && data->y>=0)
        {
            game_draw_vline(data->x, data->y, data->y+19, YELLOW); // vertical line
            game_draw_vline(data->x+19, data->y, data->y+19, YELLOW); // vertical line
            game_draw_rect(data->x, data->y, 20, 1, YELLOW);
            game_draw_rect(data->x, data->y+19, 20, 1, YELLOW);
        }
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                if(data->field[i][j] == CROSS)
                {
                    game_draw_sprite(&krestikup, i*22, j*22, RED);           //КРЕСТИК
                    game_draw_sprite(&krestikdow, i*22, (j*22)+10, RED);     // КРЕСТИК
                }
                if(data->field[i][j] == TOE)
                {
                    game_draw_sprite(&nolikup, i*22, j*22, BLUE);           // nolik
                    game_draw_sprite(&nolikdow, i*22, (j*22)+10, BLUE);     // NOLIK
                }
            }
        }
    }
    else
    {
        if(data->DRAW)  game_draw_text((const uint8_t*)"DRAW", 22, 26, BLUE);
        else if(data->WIN)   game_draw_text((const uint8_t*)"You Win!", 10, 26, GREEN);
        else if(data->LOSE)  game_draw_text((const uint8_t*)"You Lose!", 6, 26, RED);

        game_draw_text((const uint8_t*)" To start ", 1, 35, WHITE);
        game_draw_text((const uint8_t*)" new game ", 1, 43, WHITE);
        game_draw_text((const uint8_t*)"  press B ", 1, 51, WHITE);
    }
}

static void Tic_Tac_toe_update(unsigned long delta)
{
    // работа с кнопками, переменные
    bool flag = false;
    int q;  int w = q = 0;
    ////////////////////////////////////////////////////////////////////////////////////
    // joystick
    ////////////////////////////////////////////////////////////////////////////////////
if(!data->menu)
{
  if (data->pause > 0)

    {
        data->pause -= delta;
        if(data->pause <= 0)
        {
            data->menu = true;
            data->LOSE = is_Lose(flag);
            return;
        }
    }
    else
    {
        if(data->xod_igroka)
        {
            if((!(game_is_button_pressed(BUTTON_A)))/* || data->flafg*/)
            {
                if (game_is_button_pressed(BUTTON_LEFT))
                {
                    if (data->x>0)
                    {
                        data->x-=22;
                    }            
                }
                else if (game_is_button_pressed(BUTTON_RIGHT))
                {
                    if (data->x<44)
                    {
                        data->x+=22;
                    }            
                }
                else if (game_is_button_pressed(BUTTON_UP))
                {
                    if (data->y>0)
                    {
                        data->y-=22;
                    }            
                }
                else if (game_is_button_pressed(BUTTON_DOWN))
                {
                    if (data->y<44)
                    {
                        data->y+=22;
                    }             
                }
            } 
        
            else
            {
                q = (( data->x == 0 )  ?  0 : ( (data->x  ==  22)  ?  1 : 2 ));
        
                w = (( data->y == 0 )  ?  0 : ( (data->y  ==  22)  ?  1 : 2 ));
                
                if(data->field[q][w] == NOTHING)
                {
                    data->field[q][w] = CROSS;
                }
                else  data->flafg = true;
                if(!data->flafg)  data->xod_igroka = false;
                data->flafg = false;
             }
        }
        else
        {
            data->WIN = is_Win(flag);
            if(!flag)
            {
                for(int _ = 0; _ < 3; ++_)
                {
                    if(data->field[_][0] == data->field[_][1] && data->field[_][2] == NOTHING || data->field[_][0] == data->field[_][2] && data->field[_][1] == NOTHING || data->field[_][1] == data->field[_][2] && data->field[_][0] == NOTHING)
                    {
                        if(data->field[_][0] == TOE || data->field[_][1] == TOE)
                        {
                            (data->field[_][0] == NOTHING) ? (data->field[_][0] = TOE)  :  (data->field[_][1] == NOTHING) ? (data->field[_][1] = TOE)  :  (data->field[_][2] = TOE);
                            goto s;
                        }
                    }
                }
                for(int _ = 0; _ < 3; ++_)
                {
                    if(data->field[0][_] == data->field[1][_] && data->field[2][_] == NOTHING || data->field[0][_] == data->field[2][_] && data->field[1][_] == NOTHING || data->field[1][_] == data->field[2][_] && data->field[0][_] == NOTHING)
                    {
                        if(data->field[0][_] == TOE || data->field[1][_] == TOE)
                        {
                            (data->field[0][_] == NOTHING) ? (data->field[0][_] = TOE)  :  (data->field[1][_] == NOTHING) ? (data->field[1][_] = TOE)  :  (data->field[2][_] = TOE);
                            goto s;
                        }
                    }
                }

                //////////////////////////////////////////////////////////////////////////////////////////
                // write something about diagonals here:
                //////////////////////////////////////////////////////////////////////////////////////////

                if (
                         data->field[0][0] == data->field[1][1] && data->field[2][2] == NOTHING && data->field[0][0] == TOE
                      || data->field[0][0] == data->field[2][2] && data->field[1][1] == NOTHING && data->field[0][0] == TOE
                      || data->field[2][2] == data->field[1][1] && data->field[0][0] == NOTHING && data->field[2][2] == TOE
                   )
                {
                    (data->field[0][0] == NOTHING) ? (data->field[0][0] = TOE) : ((data->field[1][1] == NOTHING) ? (data->field[1][1] = TOE) : (data->field[2][2] = TOE));
                    goto s;
                }
                if (
                         data->field[0][2] == data->field[1][1] && data->field[2][0] == NOTHING && data->field[0][2] == TOE
                      || data->field[0][2] == data->field[2][0] && data->field[1][1] == NOTHING && data->field[0][2] == TOE
                      || data->field[2][0] == data->field[1][1] && data->field[0][2] == NOTHING && data->field[2][0] == TOE
                   )
                {
                    (data->field[0][2] == NOTHING) ? (data->field[0][2] = TOE) : ((data->field[1][1] == NOTHING) ? (data->field[1][1] = TOE) : (data->field[2][0] = TOE));
                    goto s;
                }
                //////////////////////////////////////////////////////////////////////////////////////////
                // end of something about diagonals;
                //////////////////////////////////////////////////////////////////////////////////////////
                for(int _ = 0; _ < 3; ++_)
                {
                    if(data->field[_][0] == data->field[_][1] && data->field[_][2] == NOTHING || data->field[_][0] == data->field[_][2] && data->field[_][1] == NOTHING || data->field[_][1] == data->field[_][2] && data->field[_][0] == NOTHING)
                    {
                        if(data->field[_][0] == CROSS || data->field[_][1] == CROSS)
                        {
                            (data->field[_][0] == NOTHING) ? (data->field[_][0] = TOE)  :  (data->field[_][1] == NOTHING) ? (data->field[_][1] = TOE)  :  (data->field[_][2] = TOE);
                            goto s;
                        }
                    }
                    
                    if(data->field[0][_] == data->field[1][_] && data->field[2][_] == NOTHING || data->field[0][_] == data->field[2][_] && data->field[1][_] == NOTHING || data->field[1][_] == data->field[2][_] && data->field[0][_] == NOTHING)
                    {
                        if(data->field[0][_] == CROSS || data->field[1][_] == CROSS)
                        {
                            (data->field[0][_] == NOTHING) ? (data->field[0][_] = TOE)  :  (data->field[1][_] == NOTHING) ? (data->field[1][_] = TOE)  :  (data->field[2][_] = TOE);
                            goto s;
                        }
                    }
                }
                //////////////////////////////////////////////////////////////////////////////////////////
                // and here may be, but if you will not want do it, you can forget it.
                //////////////////////////////////////////////////////////////////////////////////////////
                
                if     (data->field[1][1] == NOTHING)
                    data->field[1][1] = TOE;
                else if(data->field[0][0] == NOTHING)
                    data->field[0][0] = TOE;
                else if(data->field[0][2] == NOTHING)
                    data->field[0][2] = TOE;
                else if(data->field[2][0] == NOTHING)
                    data->field[2][0] = TOE;
                else if(data->field[2][2] == NOTHING)
                    data->field[2][2] = TOE;
              
                  
              else if(data->field[0][1] == NOTHING)
                  data->field[0][1] = TOE;
              else if(data->field[1][2] == NOTHING)
                  data->field[1][2] = TOE;
              else if(data->field[2][1] == NOTHING)
                  data->field[2][1] = TOE;
              else if(data->field[1][0] == NOTHING)
                  data->field[1][0] = TOE;
              s:
    
              if(is_Lose(flag))
              {
                  data->pause = 600;
              }
              else
                  data->xod_igroka = true;        
              data->DRAW = is_draw();
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////////
    // end of joystick
    ////////////////////////////////////////////////////////////////////////////////
}
else
{
    if(game_is_button_pressed(BUTTON_B))
        reload();
}
}

bool is_Win( bool &flag )
{
    
    for(int i = 0; i < 3; ++i)
    {
        if(data->field[i][0] == data->field[i][1] && data->field[i][0] == data->field[i][2] && data->field[i][0] == CROSS)
        {
            flag = true;
            goto here;
        }
        if(data->field[0][i] == data->field[1][i] && data->field[0][i] == data->field[2][i] && data->field[0][i] == CROSS)
        {
            flag = true;
            goto here;
        }
        if(data->field[0][0] == data->field[1][1] && data->field[0][0] == data->field[2][2] && data->field[0][0] == CROSS)
        {
            flag = true;
            goto here;
        }
        if(data->field[2][0] == data->field[1][1] && data->field[2][0] == data->field[0][2] && data->field[2][0] == CROSS)
        {
            flag = true;
            goto here;
        }

    }
    here:
    if(flag)
    {
        data->menu = true;
        return true;
    }
    return false;
}

bool is_Lose( bool &flag )
{
    for(int i = 0; i < 3; ++i)
    {
        if(data->field[i][0] == data->field[i][1] && data->field[i][0] == data->field[i][2] && data->field[i][0] == TOE)
        {
            flag = true;
            goto herea;
        }
        if(data->field[0][i] == data->field[1][i] && data->field[0][i] == data->field[2][i] && data->field[0][i] == TOE)
        {
            flag = true;
            goto herea;
        }
        if(data->field[0][0] == data->field[1][1] && data->field[0][0] == data->field[2][2] && data->field[0][0] == TOE)
        {
            flag = true;
            goto herea;
        }
        if(data->field[2][0] == data->field[1][1] && data->field[2][0] == data->field[0][2] && data->field[2][0] == TOE)
        {
            flag = true;
            goto herea;
        }
    }
    herea:
    if(flag)
    {
        return true;
    }
    return false;
}

bool is_draw()
{
    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
            if(data->field[i][j] == NOTHING)
                return false;
    
    if(!data->WIN && !data->LOSE)
    {
        data->menu = true;
        return true;
    }
    return false;
}

void reload()
{
    data->xod_igroka = true;
    data->flafg = false;
    data->menu = data->DRAW = data->LOSE = data->WIN = false;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            data->field[i][j] = NOTHING;
            data->joystick[i][j] = false;    
        }
    }
    data->x = data->y = 0;
    data->pause = 0;
}



const game_instance TicTacToe PROGMEM = {
    "TicTactoe",         /* Имя, отображаемое в меню */
    Tic_Tac_toe_prepare,
    Tic_Tac_toe_render,
    Tic_Tac_toe_update,
    sizeof(Tic_Tac_toeData),
    (void**)(&data)
};
/* Не забудьте зарегистрировать игру в application.cpp */
