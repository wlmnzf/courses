#pragma once

#define INIT_SIZE_HASH 37213

#define INFINITY 100000
#define MAX_X 15
#define MAX_Y 15

#define SPACE 0
#define BLACK 1
#define WHITE 2
#define MAX_MOVES 225
#define MIN(x,y) ((x) > (y) ? (y) : (x))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

#define MAX_DEPTH 10
#define DEPTH 2
#define DEPTH_T 2

#define MASK 3

typedef  unsigned __int64 U64;
struct Move_tag{
	int x,y;
};

typedef struct{
	int Dire[4];
	bool flag[4];
} ChessMan;
typedef struct Move_tag Move;
typedef struct{
	int left,right,top,tail;
}RECT_T;