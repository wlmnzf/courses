#include "ChessPad.h"

void ChessPad::addChess(Chess chess) {

	if (chesspadstate[lastrow][lastcolumn])//有棋子
		chesspadstate[lastrow][lastcolumn] -= 2;//不是最后一步了

	chess.getPosition(lastrow, lastcolumn);//重新设置最后一步记录

	if (chess.getColor())//1白旗
		chesspadstate[lastrow][lastcolumn] = 4;
	else//黑棋
		chesspadstate[lastrow][lastcolumn] = 3;

}

void ChessPad::getChessPadState(int chesspadstate[ROWNUM][COLUMN]) {
	for (int i = 0;i < ROWNUM;i++)
		for (int j = 0;j < COLUMN;j++)
			chesspadstate[i][j] = this->chesspadstate[i][j];
}