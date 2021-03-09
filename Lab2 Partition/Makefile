all: Lab2

CXX = g++

OFLAGS = -pedantic -static -Wall -fomit-frame-pointer -funroll-all-loops -O3 -DNDEBUG 

Lab2: main.o partition.o
	$(CXX) $(OFLAGS) main.o partition.o -o Lab2

main.o: main.cpp
	$(CXX) $(OFLAGS)  main.cpp -c
	
partition.o: partition.cpp 
	$(CXX) $(OFLAGS) partition.cpp -c

clean:
	rm -rf *.o *.gch Lab2
