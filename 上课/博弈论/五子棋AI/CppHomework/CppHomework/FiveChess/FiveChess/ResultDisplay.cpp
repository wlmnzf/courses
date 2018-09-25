#include "ResultDisplay.h"
using namespace std;

void ResultDisplay::display(string msg) {
	int initstate[15][15] = { 0 };
	display(initstate, msg);
}

void ResultDisplay::display(const int chesspadstate[15][15], string msg) {
	system("cls");//先清屏
	cout << rowAcolumn << endl;//列号
	for (int row = 0;row < 15;row++) {//第row行
		cout << rowAcolumn.at((row + 1) * 2 + 1) << " ";//行号
		for (int column = 0;column < 15;column++) {//第column列
			switch (chesspadstate[row][column]) {
				case 0://空白
					cout << chesspadsymbol.at((row * 15 + column) * 2) 
						<< chesspadsymbol.at((row * 15 + column) * 2 + 1);//unicode 2个字节
					break;
				case 1://黑棋
					cout << "○";
					break;
				case 2://白旗
					cout << "●";
					break;
				case 3://黑棋(最后一步)
					cout << "▽";
					break;
				case 4://白棋(最后一步)
					cout << "▼";
					break;
				default:
					//差错控制添加代码
					break;
			}
		}
		cout << endl;
	}
	cout << msg;
}