#pragma once
#include <string>
#include <iostream>
#include <cstdlib>
class ResultDisplay{
private:
	std::string rowAcolumn;
	std::string chesspadsymbol;

public:
	ResultDisplay() {
		rowAcolumn = "   A B C D E F G H I J K L M N O";
		chesspadsymbol =	"┌┬┬┬┬┬┬┬┬┬┬┬┬┬┐"
							"├┼┼┼┼┼┼┼┼┼┼┼┼┼┤"
							"├┼┼┼┼┼┼┼┼┼┼┼┼┼┤"
							"├┼┼┼┼┼┼┼┼┼┼┼┼┼┤"
							"├┼┼┼┼┼┼┼┼┼┼┼┼┼┤"
							"├┼┼┼┼┼┼┼┼┼┼┼┼┼┤"
							"├┼┼┼┼┼┼┼┼┼┼┼┼┼┤"
							"├┼┼┼┼┼┼┼┼┼┼┼┼┼┤"
							"├┼┼┼┼┼┼┼┼┼┼┼┼┼┤"
							"├┼┼┼┼┼┼┼┼┼┼┼┼┼┤"
							"├┼┼┼┼┼┼┼┼┼┼┼┼┼┤"
							"├┼┼┼┼┼┼┼┼┼┼┼┼┼┤"
							"├┼┼┼┼┼┼┼┼┼┼┼┼┼┤"
							"├┼┼┼┼┼┼┼┼┼┼┼┼┼┤"
							"└┴┴┴┴┴┴┴┴┴┴┴┴┴┘";
	}
	void display(const int chesspadstate[15][15], std::string msg);//刷新显示函数
	void display(std::string msg);//还没开始下棋，提示信息输出

};
