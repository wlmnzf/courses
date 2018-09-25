#pragma once
#include "Chesser.h"
class PeopleChesser :public Chesser {
public:
	PeopleChesser(int color, std::string name);
	Chess giveNextChess(const int chesspadstate[15][15]);
	
};