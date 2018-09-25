objects =  main.o Computer.o Board.o Game.o 

five : $(objects) 
	g++ -o five $(objects) -std=c++11

# $(objects) :Computer.h Game.h Board.h
main.o : main.cpp Computer.h Game.h Board.h
	g++ -c main.cpp -std=c++11

Computer.o : Computer.cpp Computer.h Game.h Board.h zobrist.h
	g++ -c Computer.cpp -std=c++11

Board.o : Board.cpp Computer.h Game.h Board.h
	g++ -c Board.cpp -std=c++11

Game.o : Game.cpp Computer.h Game.h Board.h
	g++ -c Game.cpp -std=c++11

# zobrist.o: zobrist.c  zobrist.h 
# 	gcc -c zobrist.c -fPIC -std=c99 -O3  -march=native -Wall -Wextra -Wshadow -Iinclude
	
# benchmark: zobrist.o benchmark.c zobrist.h
# 	gcc -c benchmark.c -fPIC -std=c99 -O3  -march=native -Wall -Wextra -Wshadow -Iinclude

clean :　　　　　　　　　　
	rm five $(objects)
