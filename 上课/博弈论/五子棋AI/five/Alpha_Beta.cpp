#include "Alpha_Beta.h"
#include<malloc.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define IDEBUG(x)// printf("%d ",x);
#define DEBUG(x)// printf("%s\n",x)
#define RAND(x) (rand()%(x))
//我是黑色

static void * alloc(int size)
{
	void * ptr;
	if(!(ptr = (void *) malloc(size)))
	{
		printf("No memory Availble\n");
		exit(0);
	}
	return ptr;
}

Alpha_Beta::Alpha_Beta(void)//初始化
{
	for(int j = 0; j < MAX_DEPTH; j ++)
		moves[j] = (Move *)alloc(sizeof(Move) * MAX_MOVES);
	Chess = (int * *)alloc(sizeof(int *) * (MAX_Y + 2));
	for(int i = 0; i <= MAX_Y + 1 ; i ++)
	{
		Chess[i] = (int *)alloc(sizeof(int) * (MAX_X + 2));
		Chess[i][0] = -1;
		for(int k = 1; k < MAX_X + 1; k ++)
			Chess[i][k] = 0;
		Chess[i][MAX_X + 1] = -1;
	}
	for(int k = 1; k < MAX_X + 1;k ++)
		Chess[0][k] = Chess[MAX_Y + 1][k] = -1;
	Reload();
}

Alpha_Beta::~Alpha_Beta(void)
{
	for(int i = 0;i <= MAX_Y ; i ++)
		free(Chess[i]);
	free(Chess);
}

void Alpha_Beta::Reload()
{
	bound.left = bound.right = bound.tail = bound.top = 0;
	for(int i = 1;i <= MAX_Y;i ++)
		memset(Chess[i],0,sizeof(int) * (MAX_X + 1));
}

int Alpha_Beta::AlphaBeta(int depth,int alpha,int beta,int side,RECT_T rect)
{
	side ^= MASK;
	int val,ptr = 0;
	if(depth >= DEPTH)
		return Evaluate(rect, side);
	
	GetMoves(rect,moves[depth],ptr);
	//printf("D:%d M:%d %d\n",depth,moves[depth][ptr].x,moves[depth][ptr].y);
	if(!depth)//第一层？
	{
		result = moves[depth][RAND(ptr - 1)];
		while(ptr)
		{
			IDEBUG(side);
			DEBUG(":\n");
			Move temp = moves[depth][--ptr];
			RECT_T rect_temp = rect;

			InBox(rect_temp,temp);
			
			//printf("TEMP:%x %x\n",temp.x,temp.y);
			
			MakeMove(temp,side);
		//	printf("depth:%d beta:%d alpha:%d side:%d  LRTT:%d %d %d %d\n", depth,beta,alpha,side,rect_temp.left,rect_temp.right,rect_temp.top,rect_temp.tail);

			val = - AlphaBeta(depth + 1,-beta, -alpha, side , rect_temp);
			//printf("VAL:%d",val);
			UnMakeMove(temp);
			if(val > beta)
				return beta;
			if(val > alpha){
				result = temp;
				alpha = val;
			}
		}
	}
	else{
		while(ptr)
		{
			Move temp = moves[depth][--ptr];
			RECT_T rect_temp = rect;
			InBox(rect_temp,temp);
			MakeMove(temp,side);
			val = - AlphaBeta(depth + 1,-beta, -alpha, side, rect_temp);
			UnMakeMove(temp);
			if(val > beta)
				return beta;
			if(val > alpha)
			{
				alpha = val;
			}
		}
	}
	return alpha;
}

void Alpha_Beta::GetMoves(RECT_T rect, Move * moves,int & ptr)
{
	for(int y = MAX(rect.top,1); y <= rect.tail ; y ++)
		for(int x = MAX(rect.left,1); x <= rect.right ; x ++)
		{
			if(!Chess[y][x])
			{
				moves[ptr].x = x;
				moves[ptr].y = y;
				ptr ++;
			}
		}
}

void Alpha_Beta::UnMakeMove(Move mo)
{
	Chess[mo.y][mo.x] = SPACE;
}
void Alpha_Beta::MakeMove(Move mo,int side)
{
	Chess[mo.y][mo.x] = side;
}

void Alpha_Beta::InBox(RECT_T & rect, Move obj)
{
	rect.left = MIN(obj.x - 1,rect.left);
	if(rect.left <= 0)
		rect.left = 1;

	rect.right = MAX(obj.x + 1,rect.right);
	if(rect.right > MAX_X)
		rect.right = MAX_X;


	rect.top = MIN(obj.y - 1,rect.top);
	if(rect.top <= 0)
		rect.top = 1;

	rect.tail = MAX(obj.y + 1,rect.tail);//初始化搜索范围
	if(rect.tail > MAX_Y)
		rect.tail = MAX_Y;
}

Move Alpha_Beta::Search(Move obj)
{
	if(!bound.left)
	{
		bound.left = obj.x - 1;
		bound.right = obj.x + 1;
		bound.top = obj.y - 1;
		bound.tail = obj.y + 1;
		InBox(bound,obj);
	}
	else
		InBox( bound, obj);
	MakeMove(obj,BLACK);//默认玩家执黑
	AlphaBeta(0 , - INFINITY, INFINITY, BLACK, bound);
	printf("LRTT：%d %d %d %d\n",bound.left,bound.right,bound.top,bound.tail); 
	InBox( bound, result);
//	printf("INBOX：%d %d\n",result.x,result.y);
	MakeMove(result,WHITE);
	return result;
}