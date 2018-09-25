#include "Judge.h"
int Judge::judgeResult(const int chesspadstate[15][15]) {
	int lastrow, lastcolumn;
	int i, j;
	int count, result;
	int rowmin, rowmax, columnmin, columnmax;


	int flag = 0;
	for (i = 0;i < 15;i++) {
		for (j = 0;j < 15;j++) {
			if (chesspadstate[i][j] == 0) {
				flag = 1;
				break;
			}
		}
		if (flag)
			break;
	}
	if (i == 15 && j == 15)
		return 3;//和局

	flag = 0;
	for (i = 0;i < 15;i++) {//最后一步的坐标
		for (j = 0;j < 15;j++)
			if (chesspadstate[i][j]>2) {
				lastrow = i;
				lastcolumn = j;
				result = chesspadstate[i][j] - 2;//返回当前旗手赢的标志
				flag = 1;
				break;
			}
		if (flag)
			break;
	}
	if (i == 15 && j == 15)//还没开始下棋
		return 0;
	
	//横向
	count = 0;
	columnmin = lastcolumn - 4 < 0 ? 0 : lastcolumn - 4;
	columnmax = lastcolumn + 4 > 14 ? 14 : lastcolumn + 4;
	for (i = lastrow, j = columnmin;j <= columnmax;j++) {
		if (chesspadstate[i][j] == result || chesspadstate[i][j] == result + 2) {//返回结果标志和棋标志相同
			count++;
			if (count == 5)//赢了
				return result;
		}
		else 
			count = 0;//重头数起
	}

	//纵向
	count = 0;
	rowmin = lastrow - 4 < 0 ? 0 : lastrow - 4;
	rowmax = lastrow + 4 > 14 ? 14 : lastrow + 4;
	for (i = rowmin, j = lastcolumn;i <= rowmax;i++) {
		if (chesspadstate[i][j] == result || chesspadstate[i][j] == result + 2) {//返回结果标志和棋标志相同
			count++;
			if (count == 5)//赢了
				return result;
		}
		else
			count = 0;//重头数起
	}

	//西北斜
	count = 0;
	rowmin = lastrow - 4;
	columnmin = lastcolumn - 4;
	if (rowmin < 0 || columnmin < 0) {//出界
		if (lastrow > lastcolumn) {//出界步数小先出界
			columnmin = 0;//先出界的为边界值
			rowmin = lastrow - lastcolumn;//后出界的根据斜率1
		}
		else {
			rowmin = 0;
			columnmin = lastcolumn - lastrow;
		}
	}
	rowmax = lastrow + 4;
	columnmax = lastcolumn + 4;
	if (rowmax > 14 || columnmax > 14) {//出界
		if (14 - lastrow < 14 - lastcolumn) {//出界步数小先出界
			rowmax = 14;//先出界的为边界值
			columnmax = lastcolumn + 14 - lastrow;
		}
		else {
			columnmax = 14;
			rowmax = lastrow + 14 - lastcolumn;
		}
	}
	for (i = rowmin, j = columnmin;i <= rowmax;i++, j++) {
		if (chesspadstate[i][j] == result || chesspadstate[i][j] == result + 2) {//返回结果标志和棋标志相同
			count++;
			if (count == 5)//赢了
				return result;
		}
		else
			count = 0;//重头数起
	}

	//东北斜
	count = 0;
	rowmin = lastrow - 4;
	columnmax = lastcolumn + 4;
	if (rowmin < 0 || columnmax>14) {//出界
		if (lastrow - 0 < 14 - lastcolumn) {//出界步数小先出界
			rowmin = 0;//先出界为边界值
			columnmax = lastcolumn + lastrow;
		}
		else {
			columnmax = 14;
			rowmin = lastrow - (14 - lastcolumn);
		}
	}
	rowmax = lastrow + 4;
	columnmin = lastcolumn - 4;
	if (rowmax > 14 || columnmin < 0) {//出界
		if (14 - lastrow < lastcolumn - 0) {//出界步数小先出界
			rowmax = 14;
			columnmin = lastcolumn - (14 - lastrow);
		}
		else {
			columnmin = 0;
			rowmax = lastrow + lastcolumn - 0;
		}
	}
	for (i = rowmin, j = columnmax;i <= rowmax;i++, j--) {
		if (chesspadstate[i][j] == result || chesspadstate[i][j] == result + 2) {//返回结果标志和棋标志相同
			count++;
			if (count == 5)//赢了
				return result;
		}
		else
			count = 0;//重头数起
	}
	return 0;//未分胜负
}