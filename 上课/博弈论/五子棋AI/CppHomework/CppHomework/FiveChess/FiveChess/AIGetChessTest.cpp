#include "ArtificialIntelligence.h"
#include <iostream>

using namespace std;
int main() {
	int state[15][15] = {
		0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
		0,0,1,2,0,0,0,0,0,0,0,1,0,2,0,
		0,1,0,2,0,0,0,0,0,0,0,0,0,0,2,
		0,2,0,2,0,0,0,0,0,0,0,0,1,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
		0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,
		0,0,0,0,1,0,2,0,0,1,0,0,0,0,0,
		0,0,0,0,0,0,2,1,2,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		0,2,0,2,0,0,0,0,0,0,0,0,2,0,0,
		1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
		0,0,1,0,0,0,0,0,0,0,0,0,1,0,0
	};
	int chess[9];
	ArtificialIntelligence ai;
	Position position = {13,12};
	for (int color = 1;color <= 2;color++) {
		cout << "color:" << color << endl;
		for (int direction = 0;direction < 4;direction++) {
			cout << "direction:" << direction << endl;
			ai.getChess(chess, state, position, color, direction);
			for (int k = 0;k < 9;k++)
				cout << chess[k] << " ";
			cout << endl;
		}
	}
	return 0;
}