#pragma once
#include "ResultDisplay.h"
#include "Chess.h"
#include "ChessPad.h"
#include "PeopleChesser.h"
#include "ComputerChesser.h"
#include "Judge.h"
#include<string>
class Game{
private:
	int mode;
	int first;//人机对弈谁先下
	std::string blackname, whitename;

	ResultDisplay resultdisplay;
	ChessPad chesspad;
	Judge judge;

	void chooseMode();
	void initOrderAName();
	void initOrder();
	void playchess(Chesser &black, Chesser &white, Judge &judge, ChessPad &chesspad, ResultDisplay &resultdisplay);
public:
	Game() {
		mode = -1;
	}
	void initGame();
	void startGame();
};
