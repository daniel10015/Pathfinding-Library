# TARGETS HERE
EXES = main.exe mainA*.exe mainLPA_star.exe
all: $(EXES)
SRC = ../src/

main.exe: main.o Dijkstra.o
	g++ -g -Wall main.o Dijkstra.o -o ../bin/main.exe

main.o: $(SRC)main.cpp Dijkstra.o
	g++ -g -Wall -c $(SRC)main.cpp 

Dijkstra.o: $(SRC)Dijkstra.cpp $(SRC)Dijkstra.h
	g++ -g -Wall -c $(SRC)Dijkstra.cpp


mainA*.exe: mainA*.o A_star.o
	g++ -g -Wall mainA*.o A_star.o -o ../bin/mainA*.exe

mainA*.o: $(SRC)mainA*.cpp $(SRC)A_star.cpp $(SRC)A_star.h ../lib/BinaryHeap_Astar.hpp
	g++ -g -Wall -c $(SRC)mainA*.cpp 

A_star.o: $(SRC)A_star.cpp $(SRC)A_star.h ../lib/BinaryHeap_Astar.hpp
	g++ -g -Wall -c $(SRC)A_star.cpp


mainLPA_star.exe: mainLPA_star.o LPA_star.o
	g++ -g -Wall mainLPA_star.o LPA_star.o -o ../bin/mainLPA_star.exe

mainLPA_star.o: $(SRC)mainLPA_star.cpp $(SRC)LPA_star.cpp $(SRC)LPA_star.h ../lib/BinaryHeap_LPAstar.hpp
	g++ -g -Wall -c $(SRC)mainLPA_star.cpp 

LPA_star.o: $(SRC)LPA_star.cpp $(SRC)LPA_star.h ../lib/BinaryHeap_LPAstar.hpp
	g++ -g -Wall -c $(SRC)LPA_star.cpp

clean:
	rm *.o