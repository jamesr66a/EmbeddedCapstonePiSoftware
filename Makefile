CC=gcc
CXX=g++

main: main.cpp uart_receiver debug
	$(CXX) -std=c++11 main.cpp build/*.o -o main -pthread

uart_receiver: debug uart_receiver.h uart_receiver.cpp uart_receiver_public.h SafeQueue.h UartRxData WiFlyStringWireFormat
	$(CXX) -c -std=c++11 uart_receiver.cpp -o build/uart_receiver.o

UartRxData: generated/UartRxData.pbo.h generated/UartRxData.pbo.c csiphash
	$(CXX) -c -std=c++11 generated/UartRxData.pbo.c -o build/UartRxData.pbo.o

WiFlyStringWireFormat: generated/WiFlyStringWireFormat.pbo.c generated/WiFlyStringWireFormat.pbo.h csiphash
	$(CXX) -c -std=c++11 generated/WiFlyStringWireFormat.pbo.c -o build/WiFlyStringWireFormat.pbo.o

csiphash: csiphash.c
	$(CXX) -c -std=c++11 csiphash.c -o build/csiphash.o

debug: debug.cpp debug.h
	$(CXX) -c -std=c++11 debug.cpp -o build/debug.o
