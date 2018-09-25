//#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<math.h>
#include "Alpha_Beta.h"

	////////////////////////////////
	/*
	printf("%d::\n",side);
	for(int y = 1; y <= MAX_Y; y ++)
	{
		for( int x = 1; x <= MAX_X; x ++)
		{
			printf("%d ",Chess[y][x]);
		}
		printf("\n");
	}
	*/
/////////////////////////////////
#define DEAD 0
#define ALIVE 1

bool Alpha_Beta::IsKill(Move mov,int side)
{	
	static int Direction[4][2] =
	{
		{ -1, 0},{ -1, -1},{ 0, -1},{ 1, -1}
	};
	int prex,prey,x,y,i;
	x = mov.x;
	y = mov.y;
	for(i = 0;i < 4;i ++)
	{
		int count = 0;
		prex = x + Direction[i][0];
		prey = y + Direction[i][1];
		while(side == Chess[prey][prex])
		{
			count ++;
			prex += Direction[i][0];
			prey += Direction[i][1];
		}
		prex = x - Direction[i][0];
		prey = y - Direction[i][1];
		while(side == Chess[prey][prex])
		{
			count ++;
			prex -= Direction[i][0];
			prey -= Direction[i][1];
		}
		if(count >= 4)
			return true;
	}
	return false;
	
}
#define MAXABS(x,y) (abs(x)>abs(y)?(x):(y))

#define KILLPOINT (INFINITY-1)
int Alpha_Beta::Judge(int CountArray[3][2][6],int side)
{
	int score = 0;
	int another = side ^ MASK;
	if(CountArray[another][ALIVE][5] || CountArray[side][ALIVE][5])
	{
		if(CountArray[side][ALIVE][5])
			score = KILLPOINT;
		else
			score = - KILLPOINT;
	}
	///////////////////////////////5子
	if(CountArray[another][ALIVE][4] || CountArray[side][ALIVE][4])
	{
		if(CountArray[side][ALIVE][4])
			score = MAXABS(KILLPOINT / 2,score);
		else
			score = MAXABS(- KILLPOINT / 2,score);
	}
	/////////////////////////////4子活

	if(CountArray[another][DEAD][4] || CountArray[side][DEAD][4])
	{
		if(CountArray[side][DEAD][4] >= 2 || CountArray[side][ALIVE][3] && CountArray[side][DEAD][4])//双死四或死四活三
			score = MAXABS(KILLPOINT / 2,score);
		else if(CountArray[another][DEAD][4] >= 2 || CountArray[another][ALIVE][3] && CountArray[another][DEAD][4])
			score = MAXABS(- KILLPOINT / 2,score);
		else if(CountArray[side][DEAD][4])
			score = MAXABS(KILLPOINT / 2,score);
		else
			score = MAXABS(- KILLPOINT / 6,score);
	}//////////////////////////4子死

	if(CountArray[another][ALIVE][3] || CountArray[side][ALIVE][3])
	{
		if(CountArray[side][ALIVE][3] >= 2)//双活
			score = MAXABS(KILLPOINT / 3,score);
		else if(CountArray[another][ALIVE][3] >= 2)//对方双活
			score = MAXABS(- KILLPOINT / 3,score);
		else if(CountArray[side][ALIVE][3])//
			score = MAXABS(KILLPOINT / 5,score);
		else
			score = MAXABS(- KILLPOINT / 9,score);
	}
	///////////////////////////3子活
 	{
			score += MAXABS(KILLPOINT * CountArray[side][DEAD][3] / 32  - KILLPOINT * CountArray[another][DEAD][3] / 32,score);
	}
	//////////////////////////3子死

	if(CountArray[another][ALIVE][2] || CountArray[side][ALIVE][2])
	{
		score += KILLPOINT / 128 * CountArray[side][ALIVE][2] - KILLPOINT / 128 * CountArray[another][ALIVE][2];
	}

	if(CountArray[another][DEAD][2] || CountArray[side][DEAD][2])
	{
		score += KILLPOINT / 256 * CountArray[side][DEAD][2] - CountArray[another][DEAD][2];
	}
	return score;
}

int Alpha_Beta::Evaluate(RECT_T rect , int side)//评价函数
{

	static int Direction[4][2] =
	{
		{ -1, 0},{ -1, -1},{ 0, -1},{ 1, -1}
	};

	int CountArray[3][2][6] = {};

	int prex,prey;
	int val[3] = {};
	memset(&ChessBoard[rect.top - 1], 0 ,MAX_X * sizeof(ChessMan));

///////////////////////////////////////

	for(int y = rect.top;y <= rect.tail; y ++)
	{
		memset(&ChessBoard[y][rect.left - 1], 0 , sizeof(ChessMan));
		for(int x = rect.left;x <= rect.right; x ++)
		{
			if(Chess[y][x] == SPACE)
			{
				memset(&ChessBoard[y][x] , 0 ,sizeof(ChessMan));
				for(int i = 0; i < 4; i ++)
				{
					prex = x + Direction[i][0];
					prey = y + Direction[i][1];
					if(!Inx(prex) || !Iny(prey))
						continue;
					if(Chess[prey][prex] == SPACE)
						continue;
					else
					{
						if(ChessBoard[prey][prex].Dire[i] >= 2)
						{
							if(ChessBoard[prey][prex].Dire[i] >= 5)
								CountArray[Chess[prey][prex]][ALIVE][5] ++;//杀棋
							else
								CountArray[Chess[prey][prex]][ChessBoard[prey][prex].flag[i]][ChessBoard[prey][prex].Dire[i]] ++;
						}
					}
				}
			}
			else
			{
				for(int i = 0; i < 4; i ++)
				{
					prex = x + Direction[i][0];
					prey = y + Direction[i][1];
					if(!Inx(prex) || !Iny(prey))
						continue;
					if(Chess[prey][prex] == SPACE)
					{
						ChessBoard[y][x].Dire[i] = 1;
						ChessBoard[y][x].flag[i] = true;
					}
					else if(Chess[prey][prex] == Chess[y][x])
					{
						ChessBoard[y][x].Dire[i] = ChessBoard[prey][prex].Dire[i] + 1;
						ChessBoard[y][x].flag[i] = ChessBoard[prey][prex].flag[i];
					}
					else
					{
						if(ChessBoard[prey][prex].Dire[i] >= 5)
							CountArray[Chess[prey][prex]][ALIVE][5] ++;//杀棋 5步全都写在ALIVE 上
						else if(ChessBoard[prey][prex].flag[i])
						{	
							if(ChessBoard[prey][prex].Dire[i] >= 2)
							{
								CountArray[Chess[prey][prex]][DEAD][ChessBoard[prey][prex].Dire[i]] ++;
							}
						}
						ChessBoard[y][x].Dire[i] = 1;
						ChessBoard[y][x].flag[i] = false; //被封死的方向
					}
				}
			}
		}
	}
	///////////////////////
	///这是一个分界线
	//////////////////////////////
/*
	printf("%d::\n",side);
	for(int y = 1; y <= MAX_Y; y ++)
	{
		for( int x = 1; x <= MAX_X; x ++)
		{
			printf("%d ",Chess[y][x]);
		}
		printf("\n");
	}
	for(int k = 1;k <=2 ;k ++)
	{
		printf("%d\n",k);
		printf("DEAD:\n");
		for(int j = 2;j <= 5;j ++)
		{
			printf("%d: %d:\n",j,CountArray[k][0][j]);
		}
		
		printf("ALIVE:\n");
		for(int i = 2;i <= 5;i ++)
			printf("%d: %d\n",i,CountArray[k][1][i]);

		printf("RECT:\n");
		printf("%d %d %d %d\n",rect.left,rect.right,rect.top,rect.tail);
	}

	int sum;
	printf("%d\n",sum = Judge(CountArray,side));
	return sum;*/
	return Judge(CountArray,side);

}

