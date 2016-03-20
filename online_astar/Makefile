CC=clang
CXX=clang++

CFLAGS=-g -Wall -Werror
CXXFLAGS=-g -Wall -Werror -std=c++11 -fno-exceptions

GTEST=-Igtest_include ../gtest-all.o ../gtest_main.o -pthread

# Build rules
node: node.h node.cpp
	$(CXX) $(CXXFLAGS) -c -o build/node.o node.cpp

TwoDSparseNodeMatrix: TwoDSparseNodeMatrix.h TwoDSparseNodeMatrix.cpp node.h
	$(CXX) $(CXXFLAGS) -c -o build/TwoDSparseNodeMatrix.o TwoDSparseNodeMatrix.cpp

# Test rules

test: node_test KvPair_test
	./node_test
	./KvPair_test

node_test: node.h node_test.cpp node
	$(CXX) $(CXXFLAGS) node_test.cpp build/node.o $(GTEST) -o node_test

KvPair_test: KvPair.h KvPair_test.cpp
	$(CXX) $(CXXFLAGS) KvPair_test.cpp $(GTEST) -o KvPair_test
