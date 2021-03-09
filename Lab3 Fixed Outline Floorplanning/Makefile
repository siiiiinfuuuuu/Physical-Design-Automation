all: lab3

CXX = g++

OFLAGS = -pedantic -static -Wall -fomit-frame-pointer -funroll-all-loops -O3 -DNDEBUG 

lab3: main.o floorplan.o basicDS.o RBT.o 
	$(CXX) $(OFLAGS) main.o floorplan.o basicDS.o RBT.o -o lab3

main.o: main.cpp
	$(CXX) $(OFLAGS)  main.cpp -c
	
floorplan.o: floorplan.cpp 
	$(CXX) $(OFLAGS) floorplan.cpp -c
 
basicDS.o: basicDS.cpp
	$(CXX) $(OFLAGS) basicDS.cpp -c
 
RBT.o: RBT.cpp 
	$(CXX) $(OFLAGS) RBT.cpp -c

clean:
	rm -rf *.o *.gch lab3
