#include <iostream>
#include "Board.h"

using namespace std;


void Board::clear(){
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
			board[i][j] = '.';
		}
	}
	count = 0;
}


Board::Board(){
	clear();
}


void Board::print(){
	cout <<"   ";
	for (int i = 0 ; i < N; i++){
		string s = i < 10?"  ":" ";
		cout << i <<s;
	}
	cout <<endl;
	for (int i = 0; i < N; i++){
		string s = i < 10?"  ":" ";
		cout << i<<s;
		for (int j = 0; j < N; j++){
			cout << board[i][j]<<"  ";
		}
		cout <<i;
		cout <<endl<<endl;
	}
	cout <<"   ";
	for (int i = 0 ; i < N; i++){
		string s = i < 10?"  ":" ";
		cout << i <<s;
	}
	cout <<endl;
}


bool Board::checkEmpty(int x, int y){
	if (x >= 0 && x <= N-1 && y >= 0 && y <= N-1 && board[x][y] == '.'){
		return true;
	}
	return false;
}


void Board::setValue(int x, int y, char c){
	board[x][y] = c;
	if ( c == '.'){
		count--;
	}else {
		count++;
	}
	return;
}


bool Board::win(int x, int y){
	return rowOfFive(x,y) || columnOfFive(x,y) || mainDiagnolOfFive(x,y) || reverseDiagnolOfFive(x,y);
}



bool Board::rowOfFive(int x, int y){
	int temp = 1;
	int i = 1;
	while (y-i >= 0 && board[x][y-i] == board[x][y]){
		temp++;
		i++;
	}
	i = 1;
	while (y + i <= N-1 && board[x][y+i] == board[x][y]){
		temp++;
		i++;
	}
	return (temp >= M);	
}

bool Board::columnOfFive(int x, int y){
	int temp = 1;
	int i = 1;
	while (x-i >= 0 && board[x-i][y] == board[x][y]){
		temp++;
		i++;
	}
	i = 1;
	while (x + i <= N-1 && board[x+i][y] == board[x][y]){
		temp++;
		i++;
	}
	return (temp >= M);
}

bool Board::mainDiagnolOfFive(int x, int y){
	int temp = 1;
	int i = 1;
	while (x-i >= 0 && y - i >= 0 && board[x-i][y-i] == board[x][y]){
		temp++;
		i++;
	}
	i = 1;
	while (x + i <= N-1 && y + i <= N-1 && board[x+i][y+i] == board[x][y]){
		temp++;
		i++;
	}
	return (temp >= M);
}

bool Board::reverseDiagnolOfFive(int x, int y){
	int temp = 1;
	int i = 1;
	while (x-i >= 0 && y + i <= N-1 && board[x-i][y+i] == board[x][y]){
		temp++;
		i++;
	}
	i = 1;
	while (x + i <= N-1 && y - i >= 0 && board[x+i][y-i] == board[x][y]){
		temp++;
		i++;
	}
	return (temp >= M);
}


bool Board::draw(){
	if (count == N*N){
		return true;
	}
	return false;
}


char Board::getValue(int i, int j){
	return board[i][j];
}


int Board::getCount(){
	return count;
}

string Board::toString(){
	string s = "";
	for (int i = 0; i < N; i++){
		for (int j = 0 ; j < N; j++){
			s+= getValue(i, j);
		}
	}
	return s;
}

