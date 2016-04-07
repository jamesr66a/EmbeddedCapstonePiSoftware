#CC=arm-linux-gnueabihf-gcc
#CXX=arm-linux-gnueabihf-g++
CC=gcc
CXX=g++

CFLAGS=-g -Wall -Werror -Wno-unused-function
CXXFLAGS=-g -Wall -Werror -Wno-unused-function

main: main.cpp uart_receiver debug uart_transmitter serial webserver_model webserver_view RSSIVectorConstructor RoverPose
	$(CXX) $(CXXFLAGS) -std=c++11 main.cpp build/*.o -o main -pthread -lcppcms -lbooster

test: RSSIVectorConstructor_test
	./RSSIVectorConstructor_test

RSSIVectorConstructor_test: RSSIVectorConstructor RSSIVectorConstructor_test.cpp
	$(CXX) $(CXXFLAGS) -std=c++11 -Igtest_include RSSIVectorConstructor_test.cpp build/RSSIVectorConstructor.o build/csiphash.o build/RSSIData.pbo.o build/RoverPose.pbo.o gtest-all.o gtest_main.o -pthread -o RSSIVectorConstructor_test

RSSIVectorConstructor: RSSIData RoverPose RSSIVectorConstructor.h RSSIVectorConstructor.cpp proboc_generate
	$(CXX) $(CXXFLAGS) -c -std=c++11 RSSIVectorConstructor.cpp -o build/RSSIVectorConstructor.o

RSSIData: generated/RSSIData.pbo.c generated/RSSIData.pbo.h csiphash proboc_generate
	$(CXX) $(CXXFLAGS) -c -std=c++11 generated/RSSIData.pbo.c -o build/RSSIData.pbo.o

RoverPose: generated/RoverPose.pbo.c generated/RoverPose.pbo.h csiphash proboc_generate
	$(CXX) $(CXXFLAGS) -c -std=c++11 generated/RoverPose.pbo.c -o build/RoverPose.pbo.o

webserver_view: webserver_view.h webserver_view.cpp webserver_template_cpp
	$(CXX) $(CXXFLAGS) -c -std=c++11 webserver_view.cpp -o build/webserver_view.o

webserver_template_cpp: webserver_template
	$(CXX) $(CXXFLAGS) -c -std=c++11 webserver_template.cpp -o build/webserver_template.o

webserver_template: webserver_view.tmpl
	cppcms_tmpl_cc webserver_view.tmpl -o webserver_template.cpp

webserver_model: webserver_model_public.h webserver_model.h webserver_model.cpp UartRxData DebugInfo
	$(CXX) $(CXXFLAGS) -c -std=c++11 webserver_model.cpp -o build/webserver_model.o

uart_transmitter: WiFlyStringWireFormat serial csiphash debug uart_transmitter.cpp uart_transmitter.h uart_transmitter_public.h
	$(CXX) $(CXXFLAGS) -c -std=c++11 uart_transmitter.cpp -o build/uart_transmiter.o

uart_receiver: debug uart_receiver.h uart_receiver.cpp uart_receiver_public.h SafeQueue.h UartRxData WiFlyStringWireFormat DebugInfo
	$(CXX) $(CXXFLAGS) -c -std=c++11 uart_receiver.cpp -o build/uart_receiver.o

DebugInfo: generated/DebugInfo.pbo.h generated/DebugInfo.pbo.c proboc_generate
	$(CXX) $(CXXFLAGS) -c -std=c++11 generated/DebugInfo.pbo.c -o build/DebugInfo.pbo.o

UartRxData: generated/UartRxData.pbo.h generated/UartRxData.pbo.c csiphash proboc_generate
	$(CXX) $(CXXFLAGS) -c -std=c++11 generated/UartRxData.pbo.c -o build/UartRxData.pbo.o

WiFlyStringWireFormat: generated/WiFlyStringWireFormat.pbo.c generated/WiFlyStringWireFormat.pbo.h csiphash proboc_generate
	$(CXX) $(CXXFLAGS) -c -std=c++11 generated/WiFlyStringWireFormat.pbo.c -o build/WiFlyStringWireFormat.pbo.o

serial: serial.cpp serial.h
	$(CXX) $(CXXFLAGS) -c -std=c++11 serial.cpp -o build/serial.o

csiphash: csiphash.c
	$(CXX) $(CXXFLAGS) -c -std=c++11 csiphash.c -o build/csiphash.o

debug: debug.cpp debug.h
	$(CXX) $(CXXFLAGS) -c -std=c++11 debug.cpp -o build/debug.o

proboc_generate:
	cd generated && ./make.sh && cd ..

clean:
	rm -f main build/* 
