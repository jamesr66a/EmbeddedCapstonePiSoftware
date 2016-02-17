CC=distcc arm-linux-gnueabihf-gcc
CXX=distcc arm-linux-gnueabihf-g++

CFLAGS=-g
CXXFLAGS=-g

main: main.cpp uart_receiver debug uart_transmitter serial webserver_model webserver_view
	$(CXX) $(CXXFLAGS) -std=c++11 main.cpp build/*.o -o main -pthread -lcppcms -lbooster

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

DebugInfo: generated/DebugInfo.pbo.h generated/DebugInfo.pbo.c
	$(CXX) $(CXXFLAGS) -c -std=c++11 generated/DebugInfo.pbo.c -o build/DebugInfo.pbo.o

UartRxData: generated/UartRxData.pbo.h generated/UartRxData.pbo.c csiphash
	$(CXX) $(CXXFLAGS) -c -std=c++11 generated/UartRxData.pbo.c -o build/UartRxData.pbo.o

WiFlyStringWireFormat: generated/WiFlyStringWireFormat.pbo.c generated/WiFlyStringWireFormat.pbo.h csiphash
	$(CXX) $(CXXFLAGS) -c -std=c++11 generated/WiFlyStringWireFormat.pbo.c -o build/WiFlyStringWireFormat.pbo.o

serial: serial.cpp serial.h
	$(CXX) $(CXXFLAGS) -c -std=c++11 serial.cpp -o build/serial.o

csiphash: csiphash.c
	$(CXX) $(CXXFLAGS) -c -std=c++11 csiphash.c -o build/csiphash.o

debug: debug.cpp debug.h
	$(CXX) $(CXXFLAGS) -c -std=c++11 debug.cpp -o build/debug.o

clean:
	rm -f main build/* 
