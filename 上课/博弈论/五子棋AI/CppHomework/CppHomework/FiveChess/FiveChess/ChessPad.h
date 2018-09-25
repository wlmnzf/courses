#pragma once
#include "Chess.h"
class ChessPad {
private:
	int lastrow;//上一步位置的 行
	int lastcolumn;//上一步位置的 列
	static const int ROWNUM = 15;//棋盘行数
	static const int COLUMN = 15;//棋盘列数
	int chesspadstate[ROWNUM][COLUMN];//棋盘标志
	
public:
	ChessPad() {
		lastrow = 0;
		lastcolumn = 0;
		for (int i = 0;i < ROWNUM;i++)
			for (int j = 0;j < COLUMN;j++)
				chesspadstate[i][j] = 0;
	}
	void addChess(Chess chess);//添加棋子
	void getChessPadState(int chesspadstate[15][15]);//获取棋盘状态
};
