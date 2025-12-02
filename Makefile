CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

OBJS = main.o graph.o hospital.o priorityqueue.o

all: emergency_system

emergency_system: $(OBJS)
	$(CXX) $(CXXFLAGS) -o emergency_system $(OBJS)

main.o: main.cpp graph.h hospital.h priorityqueue.h patient.h
	$(CXX) $(CXXFLAGS) -c main.cpp

graph.o: graph.cpp graph.h
	$(CXX) $(CXXFLAGS) -c graph.cpp

hospital.o: hospital.cpp hospital.h priorityqueue.h patient.h
	$(CXX) $(CXXFLAGS) -c hospital.cpp

priorityqueue.o: priorityqueue.cpp priorityqueue.h patient.h
	$(CXX) $(CXXFLAGS) -c priorityqueue.cpp

clean:
	rm -f $(OBJS) emergency_system
