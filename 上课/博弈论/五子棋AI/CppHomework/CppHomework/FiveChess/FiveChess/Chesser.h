#pragma once
#include <string>
#include "Chess.h"
class Chesser {
protected:
	int color;//0黑方，1白方
	std::string name;//名字
public:
	virtual Chess giveNextChess(const int chesspadstate[15][15]) = 0;//纯虚函数
	std::string getName() {
		return name;
	}
};

