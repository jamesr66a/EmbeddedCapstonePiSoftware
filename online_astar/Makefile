CC=gcc
CXX=g++

CFLAGS=-g -Wall -Werror
CXXFLAGS=-g -Wall -Werror -std=c++11 -fno-exceptions -Wno-unused-private-field

GTEST=-Igtest_include ../gtest-all.o ../gtest_main.o -pthread

online_astar: node TwoDSparseNodeMatrix online_astar.h online_astar.cpp
	$(CXX) $(CXXFLAGS) -c -o build/online_astar.o online_astar.cpp

# Build rules
node: node.h node.cpp
	$(CXX) $(CXXFLAGS) -c -o build/node.o node.cpp

TwoDSparseNodeMatrix: TwoDSparseNodeMatrix.h TwoDSparseNodeMatrix.cpp node.h
	$(CXX) $(CXXFLAGS) -c -o build/TwoDSparseNodeMatrix.o TwoDSparseNodeMatrix.cpp


# Test rules

test: node_test TwoDSparseNodeMatrix_test online_astar_test
	./node_test
	./TwoDSparseNodeMatrix_test
	./online_astar_test

node_test: node.h node_test.cpp node
	$(CXX) $(CXXFLAGS) node_test.cpp build/node.o $(GTEST) -o node_test

TwoDSparseNodeMatrix_test: TwoDSparseNodeMatrix_test.cpp TwoDSparseNodeMatrix
	$(CXX) $(CXXFLAGS) TwoDSparseNodeMatrix_test.cpp build/TwoDSparseNodeMatrix.o $(GTEST) -o TwoDSparseNodeMatrix_test

online_astar_test: online_astar_test.cpp online_astar TwoDSparseNodeMatrix
	$(CXX) $(CXXFLAGS) online_astar_test.cpp build/node.o build/online_astar.o build/TwoDSparseNodeMatrix.o $(GTEST) -o online_astar_test

clean:
	rm build/* node_test TwoDSparseNodeMatrix_test online_astar_test
