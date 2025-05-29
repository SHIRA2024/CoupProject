# Makefile for the Coup project

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g -std=c++17

# Source files
SRC = Game.cpp Player.cpp Governor.cpp Spy.cpp Baron.cpp General.cpp Judge.cpp Merchant.cpp

# SFML libraries
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Target to build and run the GUI demo
Main: main.cpp $(SRC)
	$(CXX) $(CXXFLAGS) -o demo main.cpp $(SRC) $(SFML_LIBS)
	./demo

# Target to build and run the unit tests
test: test_coup.cpp $(SRC)
	$(CXX) $(CXXFLAGS) -o test_coup test_coup.cpp $(SRC)
	./test_coup

# Target to run memory leak check using valgrind
valgrind: test_coup.cpp $(SRC)
	$(CXX) $(CXXFLAGS) -o test_coup test_coup.cpp $(SRC)
	valgrind --leak-check=full ./test_coup

# Target to clean up generated files
clean:
	rm -f demo test_coup coup_gui

	
