#include <iostream>
#include "Game.h"
#include "Board.h"
#include "Computer.h"
#include <vector>

using namespace std;

Game::Game(){
	board.clear();
}


void Game::playConsole(){
	board.clear();
	Computer computer;
	bool flag = true;
	int current = 1;
	int x = -1, y = -1;
	vector<int> v; 
	int playerChoice;
	
	cout << "Please choose 'X' or 'O' to play.\nEnter 1 to choose 'X', 2 to choose 'O': "<<endl;
	cin >> playerChoice;
	while (playerChoice != 1 && playerChoice != 2){
		cout << "Incorrect choice! Enter your choice again. "<<endl;
		cout << "Enter 1 to choose 'X', 2 to choose 'O': "<<endl;
		cin >>playerChoice;
		if (playerChoice == 1 || playerChoice == 2) break;
	}
	
	if (playerChoice == 1){
		board.setPlayerSymbol('X');
		board.setComputerSymbol('O');
	}else {
		board.setPlayerSymbol('O');
		board.setComputerSymbol('X');	}

	cout << "You play as " << board.getPlayerSymbol() <<". Let's start!"<<endl;
	cout << "Do you want to go first? y/n"<<endl;
	char playerGoFirst;
	cin >> playerGoFirst;
	
	if (playerGoFirst == 'Y' || playerGoFirst == 'y'){
		current = 1;
		cout << "OK. You go first!"<<endl;
	} else {
		cout << "OK. I go first!"<<endl;
		current = 2;
	}
		
	board.print();	
	while (flag){
		//board.print();
		cout <<endl;
		if (current == 1){ //get player move
			cout <<"Your turn. (" << getCurrentSymbol(current)<<" )"<<endl;
			cout << "Enter X coordinate of your move: "<<endl;
			cin >> x;
			cout << "Enter Y coordinate of your move: "<<endl;
			cin >> y;
		} else { // get computer move
		
			v = computer.nextMoveAlphaBeta(board);
			x = v[0];
			y = v[1];
			//cout << x <<" "<<y<<endl;
			/*cout <<"Your turn. (" << getCurrentSymbol(current)<<" )"<<endl;
			cout << "Enter X coordinate of your move: ";
			cin >> x;
			cout << "Enter Y coordinate of your move: ";
			cin >> y;*/
		}
		

		if (!board.checkEmpty(x, y)){
			cout <<endl<< "Not a legal move. Try again! (x: "<<x<<",y: "<<y<<")  (current: "<<current<<" content: "<<board.getValue(x, y)<<" )"<<endl;
			continue; 
		}else {
			
			board.setValue(x, y, getCurrentSymbol(current));
			board.print();
			if (current == 1) {
				cout << "Your move is : (" << x << ", " << y<<") "<<endl;
			}else {
				cout <<	"My move is : (" << x << ", " << y<<") "<<endl;
			}
		}
		

		if (board.win(x, y)){
			board.print();
			if (current == 1) {
				cout << "CONGRATULATIONS! YOU WON!"<<endl;
			}else {
				cout << "I WON!!!"<<endl;
			}
			break;
		}else if (board.draw()){
			board.print();
			cout << "Draw game!"<<endl;
			break;
		}else{ 
			current = 3 - current;
		}
	}
}


char Game::getCurrentSymbol(int current){
	if (current == 1){
		return board.getPlayerSymbol();
	}else{
		return board.getComputerSymbol();
	}
}








void Game::run(){
	bool flag = true;
	char x;
	while (flag){
		playConsole();
		cout << "Do you want to play another game? y/n"<<endl;
		cin >> x;
		if (x == 'N' || x == 'n'){
			flag = false;
		}else{
			cout << "------------------------------------------------------"<<endl;
		}
	}
}



