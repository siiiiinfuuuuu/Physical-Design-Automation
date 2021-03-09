all: Lab5

CXX = g++

OFLAGS = -pedantic -static -Wall -fomit-frame-pointer -funroll-all-loops -O3 -DNDEBUG 

Lab5: main.o greedyChannelRouter.o
	$(CXX) $(OFLAGS) main.o greedyChannelRouter.o  -o Lab5

main.o: main.cpp
	$(CXX) $(OFLAGS)  main.cpp -c
	
greedyChannelRouter.o: greedyChannelRouter.cpp
	$(CXX) $(OFLAGS) greedyChannelRouter.cpp -c

clean:
	rm -rf *.o *.gch Lab5
