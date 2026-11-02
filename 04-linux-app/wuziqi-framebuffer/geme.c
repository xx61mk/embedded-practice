#include "game.h"
#include "framebuffer.h"
#include <stdio.h>

#define RED   0x00FF0000
#define GREEN 0x0000FF00
#define BLUE  0x000000FF
#define BLACK 0x00000000
#define WHITE 0x00FFFFFF
#define YELLOW 0x00FFFF00

//棋盘横向起始位置的偏移量
#define BROAD_X_START 150
//棋盘纵向起始位置的偏移量
#define BROAD_Y_START 75

//棋盘横向位置个数
#define BROAD_X_CNT 20
//棋盘纵向位置个数
#define BROAD_Y_CNT 20
//棋盘单个格子的宽度
#define BROAD_W_ONE 25

#define WHITE_CHESS 1
#define BLACK_CHESS -1

//当前光标所在的位置
int posx_g = BROAD_X_START;
int posy_g = BROAD_Y_START;

int chess_g[BROAD_Y_CNT][BROAD_X_CNT] = {0};
int chess=BLACK_CHESS;

void show_chess_broad()
{
	draw_clear(0xC8E0D8);
	
	for (int i = 0; i < BROAD_Y_CNT; ++i)
	{
		draw_h_line(BROAD_X_START, BROAD_Y_START+i*BROAD_W_ONE, (BROAD_X_CNT-1)*BROAD_W_ONE,BLACK);
	}
	
	for (int i = 0; i < BROAD_X_CNT; ++i)
	{
		draw_s_line(BROAD_X_START+i*BROAD_W_ONE, BROAD_Y_START, (BROAD_Y_CNT-1)*BROAD_W_ONE, BLACK);
	}
	
	draw_full_rect(posx_g-5, posy_g-5, 10, 10, RED);
	
	for (int i = 0; i < BROAD_Y_CNT; ++i)
	{
		for (int j = 0; j < BROAD_X_CNT; ++j)
		{
            int x =j*BROAD_W_ONE+BROAD_X_START;
            int y =i*BROAD_W_ONE+BROAD_Y_START;
			if (WHITE_CHESS == chess_g[i][j])
			{
				draw_circle(x,y,10, WHITE);
			}
			else if (BLACK_CHESS == chess_g[i][j])
			{
				draw_circle(x,y,10, BLACK);
			}
		}
	}
	
}

int is_win(int chess_g[][BROAD_X_CNT],int i,int j)
{
    int col=chess_g[i][j];
    if(col==0)
    {
        return 0;
    }
    int cnt = 1;
    for(int k =j-1;k>=0&&chess_g[i][k]==col;k--)
    {
        ++cnt;
    }
    for(int k = j+1; k<=BROAD_X_CNT&&chess_g[i][k]==col;k++)
    {
        ++cnt;
    }
    if(cnt>=5)
    {
        return col;
    }
     cnt = 1;
    for(int k=i-1;k>=0&&chess_g[k][j]==col;k--)
    {
        ++cnt;
    }
    for(int k=i+1;k<=BROAD_Y_CNT&&chess_g[k][j]==col;k++)
    {
        ++cnt;
    }
    if(cnt>=5)
    {
        return col;
    }
     cnt = 1;
    for(int k=1;(i-k)>=0&&(j-k)>=0&&chess_g[i-k][j-k]==col;k++)
    {
        cnt++;
    }
    for(int k=1;(i+k)<=BROAD_X_CNT&&(j+k)<=BROAD_Y_CNT&&chess_g[i+k][j+k]==col;k++)
    {
        cnt++;
    }
    if(cnt>=5)
    {
        return col;
    }
     cnt = 1;
    for(int k=1;(i-k)>=0&&(j+k)<=BROAD_X_CNT&&chess_g[i-k][j+k]==col;k++)
    {
        cnt++;
    }
    for(int k=1;(i+k)<=BROAD_Y_CNT&&(j-k)>=0&&chess_g[i+k][j-k]==col;k++)
    {
        cnt++;
    }
    if(cnt>=5)
    {
        return col;
    }
    return 0;

}
void begin_game()
{
	char choose = 0;

	while (1)
	{
		choose = getchar();
		if ('w' == choose)
		{
			if (posy_g > BROAD_Y_START)
			{
				posy_g -= BROAD_W_ONE;
			}
		}
		else if ('a' == choose)
		{
            if(posx_g>BROAD_X_START)
            {
                posx_g-=BROAD_W_ONE;

            }
		
		}
		else if ('s' == choose)
		{
			if (posy_g < BROAD_Y_START + (BROAD_Y_CNT-1)*BROAD_W_ONE)
			{
				posy_g += BROAD_W_ONE;
			}
		}
		else if ('d' == choose)
		{
            if(posx_g<BROAD_X_START+(BROAD_X_CNT -1)*BROAD_W_ONE)
            {
                posx_g+=BROAD_W_ONE;
            }
		
		}
		else if (' ' == choose)
		{
            int x=(posy_g-BROAD_Y_START)/BROAD_W_ONE;
            int y=(posx_g-BROAD_X_START)/BROAD_W_ONE;
            if(chess_g[x][y]!=0)
            {
                return;
            }
			chess_g[x][y]=chess;
            if(is_win(chess_g,x,y)==1)
            {
                show_chess_broad();
                draw_bmp(200,200,"./B.bmp");
                return;

            }
            else if(is_win(chess_g,x,y)==-1)
            {
                show_chess_broad();
                draw_bmp(200,200,"./H.bmp");
                return;
            }
            else
            {
                chess=-chess;
            }
        }
		show_chess_broad();	
	}	
}
