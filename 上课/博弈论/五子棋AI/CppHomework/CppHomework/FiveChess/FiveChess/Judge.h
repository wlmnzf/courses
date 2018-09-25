#pragma once
class Judge {
private:
	int currentchesser;//0黑方，1白方

public:
	Judge() {
		currentchesser = 1;
	}
	int judgeResult(const int chesspadstate[15][15]);//判断比赛结果：0为还未能判定胜负，1为黑方胜，2为白方胜，3为和棋

	int  nextChesser() {//下一个旗手
		if (currentchesser)
			currentchesser = 0;
		else
			currentchesser = 1;
		return currentchesser;
	}
};
