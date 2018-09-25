#include <iostream>

#include "Computer.h"
#include "Board.h"
#include <vector>
#include <limits.h>


using namespace std;

void SetScore(Board & board,const int i, const int j, const char Who,  
                    vector<vector<int> > & State) ;
int p_Score(int num, int bp[]);
vector<int>  Way(Board & board)  ;

vector<int> Computer::nextMoveAlphaBeta(Board & board){
	vector<int> v = {0,0};
	int M = INT_MIN, x = -1, y = -1;
	
	for (int i = 0; i <= N-1;i++){
		for (int j = 0; j <= N-1; j++){
			if (board.getValue(i, j) == '.' && adjacentPlaced(board, i, j) ){
				int temp = minimaxAlphaBeta (board, 2, true,INT_MIN, INT_MAX, i, j);
				if (M < temp){
					M = temp;
					x = i;
					y = j;
				}
				
			}
		}
	}
	if (x == -1 && y == -1){

		//利用贪心算法求解
		vector<int>  v=Way(board);
		x=v[0];
		y=v[1];
	}

	v[0] = x;
	v[1] = y;
	return v;
}




vector<int>  Way(Board & board)  
{  
    int i,j;  
    vector<vector<int> > HumanState, ComputerState;  
    vector<int> temp(N, 0);  
    int maxP = 0, maxC = 0;  
    vector<int> ps = {0,0};
    vector<int> ps1 = {0,0};
    vector<int> ps2 = {0,0}; 

      
    for (i = 0; i < N; i++)  
    {  
        HumanState.push_back(temp);  
        ComputerState.push_back(temp);  
    }  
      
  
    for ( i = 0; i < N; i++)  
    {  
        for (j = 0; j < N; j++)  
        {  
            if (board.getValue(i,j) == '.')  
            {  
            	//board.getComputerSymbol()  :  board.getPlayerSymbol(); 
                SetScore(board,i, j, board.getPlayerSymbol(), HumanState);  
                SetScore(board,i, j, board.getComputerSymbol() , ComputerState);  
            }  
        }  
    }  
  
      
    for (i = 0; i < N; i++)  
    {  
        for (j = 0; j < N; j++)  
        {  
            if (HumanState[i][j] > maxP)  
            {  
                maxP = HumanState[i][j];  
                ps1[0] = i;  
                ps1[1] = j;  
            }  
            if (ComputerState[i][j] > maxC)  
            {  
                maxC = ComputerState[i][j];  
                ps2[0] = i;  
                ps2[1] = j;  
            }  
        }  
    }  
    if (maxP >= maxC)  
    {  
        ps[0] = ps1[0];  
        ps[1]= ps1[1];  
    }  
    else  
    {  
        ps[0]= ps2[0];  
        ps[1] = ps2[1];  
    }  
    return ps;  
}  


void SetScore(Board & board,const int i, const int j, const char Who,  
                    vector<vector<int> > & State)  
{  
	// board.getComputerSymbol() 
    char Another; //与要求的状态相对的另一方  
    char empty='.';
    int bp[2], k, num, max = 0, temp = 0;  
    if (Who == board.getComputerSymbol() )  
    {  
        Another = board.getPlayerSymbol() ;  
    }  
    else if (Who == board.getPlayerSymbol())  
    {  
        Another = board.getComputerSymbol();  
    }  
    else  
    {  
        return;  
    }  
      
  
    //横向  
    //向右  
    bp[0] = 1; bp[1] = 1;  
    num = 0;  
    for (k = 1; k < N - i; k++)  
    {  
        if (board.getValue(i+k,j) == Who)  
        {  
            num++;  
            continue;  
        }  
        if (board.getValue(i+k,j) == empty)  
        {  
            bp[1] = 0;  
            break;  
        }  
        if (board.getValue(i+k,j) == Another)  
        {  
            bp[1] = 1;  
            break;  
        }  
    }  
    //向左  
    for (k = 1; k <= i; k++)  
    {  
        if (board.getValue(i-k,j) == Who)  
        {  
            num++;  
            continue;  
        }  
        if (board.getValue(i-k,j) == empty)  
        {  
            bp[0] = 0;  
            break;  
        }  
        if (board.getValue(i-k,j) == Another)  
        {  
            bp[0] = 1;  
            break;  
        }  
    }  
    //调用评分函数  
    temp = p_Score(num, bp);  
    max += temp;  
  
    //纵向  
    bp[0] = 1; bp[1] = 1;  
    num = 0;  
    //向下  
    for (k = 1; k < N - j; k++)  
    {  
        if (board.getValue(i,j+k) == Who)  
        {  
            num++;  
            continue;  
        }  
        if (board.getValue(i,j+k) == empty)  
        {  
            bp[1] = 0;  
            break;  
        }  
        if (board.getValue(i,j+k) == Another)  
        {  
            bp[1] = 1;  
            break;  
        }  
    }  
    //向上  
    for (k = 1; k <= j; k++)  
    {  
        if (board.getValue(i,j-k) == Who)  
        {  
            num++;  
            continue;  
        }  
        if (board.getValue(i,j-k) == empty)  
        {  
            bp[0] = 0;  
            break;  
        }  
        if (board.getValue(i,j-k) == Another)  
        {  
            bp[0] = 1;  
            break;  
        }  
    }  
    temp = p_Score(num, bp);  
    max += temp;  
  
    //从左上到右下  
    bp[0] = 1; bp[1] = 1;  
    num = 0;  
  
    //下  
    for (k = 1; k < N-i && k < N-j; k++)  
    {  
        if (board.getValue(i+k,j+k) == Who)  
        {  
            num++;  
            continue;  
        }  
        if (board.getValue(i+k,j+k) == empty)  
        {  
            bp[1] = 0;  
            break;  
        }  
        if (board.getValue(i+k,j+k) == Another)  
        {  
            bp[1] = 1;  
            break;  
        }  
    }  
    //上  
    for (k = 1; k <= j && k <= i; k++)  
    {  
        if (board.getValue(i-k,j-k) == Who)  
        {  
            num++;  
            continue;  
        }  
        if (board.getValue(i-k,j-k) == empty)  
        {  
            bp[0] = 0;  
            break;  
        }  
        if (board.getValue(i-k,j-k) == Another)  
        {  
            bp[0] = 1;  
            break;  
        }  
    }  
    temp = p_Score(num, bp);  
    max += temp;  
  
    //从右上到左下  
    bp[0] = 1; bp[1] = 1;  
    num = 0;  
    //下  
    for (k = 1; k < N-j && k <= i; k++)  
    {  
        if (board.getValue(i-k,j+k) == Who)  
        {  
            num++;  
            continue;  
        }  
        if (board.getValue(i-k,j+k) == empty)  
        {  
            bp[1] = 0;  
            break;  
        }  
        if (board.getValue(i-k,j+k) == Another)  
        {  
            bp[1] = 1;  
            break;  
        }  
    }  
    //上  
    for (k = 1; k <= j && k < N-i; k++)  
    {  
        if (board.getValue(i+k,j-k) == Who)  
        {  
            num++;  
            continue;  
        }  
        if (board.getValue(i+k,j-k) == empty)  
        {  
            bp[0] = 0;  
            break;  
        }  
        if (board.getValue(i+k,j-k) == Another)  
        {  
            bp[0] = 1;  
            break;  
        }  
    }  
    temp = p_Score(num, bp);  
    max += temp;  
  
    if (max > State[i][j])  
    {  
        State[i][j] = max;  
    }  
}  
  
//实际的评分函数  
int p_Score(int num, int bp[])  
{  
    int max = 0;  
    if (num >= 4)  
    {  
        max += 10000; //成五  
    }  
    else if (num == 3)  
    {  
        if (bp[1] == 1 && bp[0] == 1) //不成五  
        {  
            max += 0;  
        }  
        else if (bp[1] == 0 && bp[0] == 0) //活四  
        {  
            max += 3000;  
        }  
        else  
        {  
            max += 900; //冲四  
        }  
    }  
    else if (num == 2)  
    {  
        if (bp[0] == 0 && bp[1] == 0)  
        {  
            max += 460; //活三  
        }  
        else if (bp[0] == 1 && bp[1] == 1)  
        {  
            max += 0; //不成五  
        }  
        else  
        {  
            max += 30; //死三  
        }  
    }  
    else if (num == 1)  
    {  
        if (bp[0] == 0 && bp[1] == 0)  
        {  
            max += 45;  //活二  
        }  
        else if (bp[0] == 1 && bp[1] == 1)  
        {  
            max += 0;  
        }  
        else  
        {  
            max += 5;  
        }  
    }  
    else if (num == 0)  
    {  
        if (bp[0] == 0 && bp[1] == 0)  
        {  
            max += 3;  
        }  
        else if (bp[0] == 1 && bp[1] == 1)  
        {  
            max += 0;  
        }  
        else  
        {  
            max += 1;  
        }  
    }  
    return max;  
}  


int Computer::minimaxAlphaBeta(Board & board, int depth, bool isMax, int alpha, int beta, int x , int y){
	char c = isMax ?  board.getComputerSymbol()  :  board.getPlayerSymbol(); 
	board.setValue(x,y,c); 
	
	if (board.win(x,y)){
		board.setValue(x,y,'.');	
		if (isMax){//computer wins
			return INT_MAX;
		}else {
			return INT_MIN;
		}
	}else if (board.draw()){
		board.setValue(x,y,'.');	
		return 0;
	}
	if (depth == 0){
		int value = 0;
		if (checkVisitedBoard(board) == true){
			value = getEvaluation(board); 
		}else{
			value = evaluation(board, isMax); 
			insertToHashMap(board, value); 
		}
		board.setValue(x,y,'.');
		
		//cout <<"evaluation at "<<x <<" "<<y<< " is "<< value <<endl;
		return value;
	}
	
	
	vector<int> firstCoord;
	vector<int> secondCoord;
	for (int i = 0; i < N; i++){
		for (int j = 0; j<N; j++){
			if (board.getValue(i,j) == '.' && adjacentPlaced(board, i, j)){
				firstCoord.push_back(i);
				secondCoord.push_back(j);
			}
		}
	}
	
	int len = (int) firstCoord.size();
	if (isMax == true){
		int m = INT_MAX;
		for (int i = 0; i < len; i++){
			int temp = minimaxAlphaBeta(board,depth - 1, false, alpha, beta, firstCoord[i], secondCoord[i]);
			if (m > temp){
				m = temp;
			}
			if (beta > m){
				beta = m;
			 }	
			 if (alpha >= beta){
				break;
			 }
		}
		board.setValue(x,y,'.');

		return m;
	}else {
		int M = INT_MIN;
		for (int i = 0; i < len; i++){
			int temp = minimaxAlphaBeta(board, depth - 1, true, alpha, beta, firstCoord[i], secondCoord[i]);
			if (M < temp){
				M = temp;
			}
			if (alpha < M){
				alpha = M;
			 }
			 if (alpha >= beta){
				break;
			 }
		}
		board.setValue(x,y,'.');

		return M;
	}
}




int Computer::evaluation(Board & board, bool isMax){

	int sum = 0;
	vector<int> computerPattern(M+1,0);
	vector<int> playerPattern(M+1,0);
	
	for (int  i = 0 ; i < N; i++){
		for (int j = 0; j < N ; j++){
			if (board.getValue(i,j) != '.'){
				

				char c = board.getValue(i,j);
				bool needMax = c == board.getComputerSymbol();
				
				
				
				int sameSymbol = 1; 
				int k = 1;
				while (i- k >= 0 && board.getValue(i-k, j)  == c){
					sameSymbol++;
					k++;
				}
				
		
				int l = 1;
				while (i + l <= N-1 && board.getValue(i+l, j) == c){
					sameSymbol++;
					l++;
				}

				if (sameSymbol >= M){
					if (needMax) computerPattern[M]++;
					else playerPattern[M]++;
				}else if (sameSymbol == M - 1 && (board.checkEmpty(i-k,j) || board.checkEmpty(i+l,j))){ 				
					if (needMax) computerPattern[M-1]++;
					else playerPattern[M-1]++;
				}else if (sameSymbol == M - 2 && (board.checkEmpty(i-k,j) || board.checkEmpty(i+l,j))){
					if (needMax) computerPattern[M-2]++;
					else playerPattern[M-2]++;
				}else if (sameSymbol == M - 2 && (board.checkEmpty(i-k,j) && board.checkEmpty(i+l,j))){
					if (needMax) computerPattern[M-3]++;
					else playerPattern[M-3]++;
				}else if (sameSymbol == M - 3 && board.checkEmpty(i-k,j) && board.checkEmpty(i+l,j)){
					if (needMax) computerPattern[M-4]++;
					else playerPattern[M-4]++;
				}
				
				
				//-------------------------------------------------------------------------------
				sameSymbol = 1; // count same symbols in rows
				k = 1;
				while (j - k >= 0 && board.getValue(i, j-k)  == c){
					sameSymbol++;
					k++;
				}
				
				
				//consider value at i - k later to see if it's blocked or not
				l = 1;
				while (j + l <= N-1 && board.getValue(i, j+l) == c){
					sameSymbol++;
					l++;
				}

				if (sameSymbol >= M){
					if (needMax) computerPattern[M]++;
					else playerPattern[M]++;
				}else if (sameSymbol == M - 1 && (board.checkEmpty(i,j-k) || board.checkEmpty(i,j+l))){ 				
					if (needMax) computerPattern[M-1]++;
					else playerPattern[M-1]++;
				}else if (sameSymbol == M - 2 && (board.checkEmpty(i,j-k) || board.checkEmpty(i,j+l))){
					if (needMax) computerPattern[M-2]++;
					else playerPattern[M-2]++;
				}else if (sameSymbol == M - 2 && (board.checkEmpty(i,j-k) && board.checkEmpty(i,j+l))){
					if (needMax) computerPattern[M-3]++;
					else playerPattern[M-3]++;
				}else if (sameSymbol == M - 3 && board.checkEmpty(i,j-k) && board.checkEmpty(i,j+l)){
					if (needMax) computerPattern[M-4]++;
					else playerPattern[M-4]++;
				}
				
				//--------------------------------------------------------------
				
				sameSymbol = 1;// count same symbols in main diagnol
				k = 1;
				while (i - k >= 0 && j - k >= 0 && board.getValue(i-k, j- k)  == c){
					sameSymbol++;
					k++;
				}
				
				
				//consider value at i - k later to see if it's blocked or not
				l = 1;
				while (i + l <= N-1 && j + l <= N-1 && board.getValue(i+l, j+l ) == c){
					sameSymbol++;
					l++;
				}

				if (sameSymbol >= M){
					if (needMax) computerPattern[M]++;
					else playerPattern[M]++;
				}else if (sameSymbol == M - 1 && (board.checkEmpty(i-k,j-k) || board.checkEmpty(i+l,j+l))){ 				
					if (needMax) computerPattern[M-1]++;
					else playerPattern[M-1]++;
				}else if (sameSymbol == M - 2 && (board.checkEmpty(i-k,j-k) || board.checkEmpty(i+l,j+l))){
					if (needMax) computerPattern[M-2]++;
					else playerPattern[M-2]++;
				}else if (sameSymbol == M - 2 && (board.checkEmpty(i-k,j-k) && board.checkEmpty(i+l,j+l))){
					if (needMax) computerPattern[M-3]++;
					else playerPattern[M-3]++;
				}else if (sameSymbol == M - 3 && board.checkEmpty(i-k,j-k) && board.checkEmpty(i+l,j+l)){
					if (needMax) computerPattern[M-4]++;
					else playerPattern[M-4]++;
				}
				
				
				
				//-----------------------------------------------------------------------
				
				
				sameSymbol = 1;
				k = 1;
				while (i - k >= 0 && j + k <= N-1 && board.getValue(i-k, j+ k)  == c){
					sameSymbol++;
					k++;
				}
				
				
		
				l = 1;
				while (i + l <= N-1 && j - l >= 0 && board.getValue(i+l, j-l ) == c){
					sameSymbol++;
					l++;
				}

				if (sameSymbol >= M){
					if (needMax) computerPattern[M]++;
					else playerPattern[M]++;
				}else if (sameSymbol == M - 1 && (board.checkEmpty(i-k,j+k) || board.checkEmpty(i+l,j-l))){ 				
					if (needMax) computerPattern[M-1]++;
					else playerPattern[M-1]++;
				}else if (sameSymbol == M - 2 && (board.checkEmpty(i-k,j+k) || board.checkEmpty(i+l,j-l))){
					if (needMax) computerPattern[M-2]++;
					else playerPattern[M-2]++;
				}else if (sameSymbol == M - 2 && (board.checkEmpty(i-k,j+k) && board.checkEmpty(i+l,j-l))){
					if (needMax) computerPattern[M-3]++;
					else playerPattern[M-3]++;
				}else if (sameSymbol == M - 3 && board.checkEmpty(i-k,j+k) && board.checkEmpty(i+l,j-l)){
					if (needMax) computerPattern[M-4]++;
					else playerPattern[M-4]++;
				}
				
				
			}	
		}
	}
	if (computerPattern[M] > 0) return INT_MAX;
	if (playerPattern[M] > 0) return INT_MIN;
	int x = 1;
	sum += computerPattern[1];
	sum -= playerPattern[1]*5;
	for (int i = 2 ; i < M ; i++){
		//cout <<computerPattern[i] << " : "<<playerPattern[i]<<endl;
		x *= 100;
		sum += computerPattern[i] * x;
		sum -= playerPattern[i] * x * 10;
	}	
	return sum;
}




bool Computer::adjacentPlaced(Board & board , int x, int y){

	bool value = false;
	if (board.getValue(x,y) != '.') return false;
	vector<vector<int>> adjacent = {{-1,-1},{-1,0}, {-1,1},{0,1},{0,-1},{1,-1},{1,0},{1,1}};
	for (auto d:adjacent){

		if (x+d[0] >=0 && y+d[1]>=0 && x+d[0] <= N-1 && y + d[1] <= N-1){
			value = value || (board.getValue(x+d[0],y+d[1]) != '.');
		}
	}
	return value;
}



bool Computer::checkVisitedBoard(Board & board){
	string s = board.toString();
	if (hashMap.find(s) != hashMap.end()){
		return true;
	}
	return false;
}



int Computer::getEvaluation(Board & board){
	if (checkVisitedBoard(board)){
		return hashMap[board.toString()]; 
	}
	return -1;
}


void Computer::insertToHashMap(Board & board, int eval){
	string s = board.toString();
	// zobrist_t k;
	// init_zobrist(& k);// call once

	// then you can hash as many strings as you want:
	// uint64_t ss = zobrist_nt(s.c_str(), &k);

	hashMap[s] = eval;
	return;
}






