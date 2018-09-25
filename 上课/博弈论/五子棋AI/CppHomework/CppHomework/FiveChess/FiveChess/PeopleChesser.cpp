#include "PeopleChesser.h"
#include <iostream>
PeopleChesser::PeopleChesser(int color, std::string name) {
	this->color = color;
	this->name = name;
}
Chess PeopleChesser::giveNextChess(const int chesspadstate[15][15]) {
	int x, y;
	char a, b;

	while (1) {
		
		do {
			std::cout << "请输入下步棋，如H行A列,则输入HA即可(注意大写):";
			std::cin.clear();
			std::cin.sync();
			std::cin >> a;
			std::cin >> b;
		} while (!('A' <= a && a <= 'O') || !('A' <= b && b <= 'O'));//输入符合规则

		x = a - 'A';//判断是否有棋子
		y = b - 'A';
		if (chesspadstate[x][y]) {//有棋子了
			std::cout << "该位置已经有棋子了，请下别的位置\n";
			continue;
		}
		else//符合
			break;

	}
	return Chess(color, x, y);
}