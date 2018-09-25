#include "Game.h"
#include <iostream>
using namespace std;

void Game::chooseMode() {
	//选择模式
	do {
		resultdisplay.display("选择对弈模式0为人机对弈，1为人人对弈：");
		cin.clear();
		cin.sync();//VS2015不能清空缓冲区，vs2012可以！！！
		cin >> mode;
	} while (2 <= mode || mode < 0);
}

void Game::initOrder() {
	//初始化
	do {//选择顺序
		resultdisplay.display("电脑先下请输入0，你先下请输入1：");
		cin.clear();
		cin.sync();//VS2015不能清空缓冲区，vs2012可以！！！
		cin >> first;
	} while (2 <= first || first < 0);
}
void Game::initOrderAName() {
	
	if (mode) {//人人模式
			   //初始化
		resultdisplay.display("请输入黑方旗手的名字（默认黑方先下）：");
		cin.clear();
		cin.sync();//VS2015不能清空缓冲区，vs2012可以
		getline(cin,blackname);
		resultdisplay.display("请输入白方旗手的名字（默认黑方先下）：");
		cin.clear();
		cin.sync();//VS2015不能清空缓冲区，vs2012可以
		getline(cin,whitename);
	}
	else {//人机模式
		initOrder();//选择顺序
		if (first) {
			blackname = "用户";
			whitename = "电脑";
		}
		else {
			blackname = "电脑";
			whitename = "用户";
		}
	}
}

void Game::initGame(){
	chooseMode();//选择模式
	initOrderAName();//选择下棋顺序和初始化名字
}

void Game::startGame() {
	if (mode) {//人人模式
		
		//初始化选手
		PeopleChesser black(0, blackname);
		PeopleChesser white(1, whitename);
		//下棋
		playchess(black, white, judge, chesspad, resultdisplay);
	}
	else {//人机
		if (first) {//机器后下
			
			//初始化选手
			ComputerChesser white(1, "电脑");
			PeopleChesser black(0, "用户");
			//下棋
			playchess(black, white, judge, chesspad, resultdisplay);
		}
		else {//机器先下
			
			//初始化选手
			ComputerChesser black(0, "电脑");
			PeopleChesser white(1, "用户");
			//下棋
			playchess(black, white, judge, chesspad, resultdisplay);
		}
	}
}

void Game::playchess(Chesser &black, Chesser &white, Judge &judge, ChessPad &chesspad, ResultDisplay &resultdisplay) {
	int result;
	int state[15][15] = { 0 };
	//下棋阶段
	while (!(result = judge.judgeResult(state))) {
		if (judge.nextChesser()) {
			resultdisplay.display(state, "白方：" + white.getName() + "\n");
			chesspad.addChess(white.giveNextChess(state));
		}
		else {
			resultdisplay.display(state, "黑方：" + black.getName() + "\n");
			chesspad.addChess(black.giveNextChess(state));
		}
		chesspad.getChessPadState(state);
	}
	//出结果
	switch (result) {
	case 1:
		resultdisplay.display(state, "黑方:" + black.getName() + "胜\n按e键退出");
		break;
	case 2:
		resultdisplay.display(state, "白方:" + white.getName() + "胜\n按e键退出");
		break;
	case 3:
		resultdisplay.display(state, "本局结束，你们打平\n按e键退出");
		break;
	default:
		//差错控制
		break;
	}
	//退出
	char exit;
	do {
		cin.clear();
		cin.sync();
		cin >> exit;
	} while (exit != 'e' && exit != 'E');
}
