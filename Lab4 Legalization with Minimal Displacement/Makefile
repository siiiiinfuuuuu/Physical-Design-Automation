all: Lab4

CXX = g++

OFLAGS = -pedantic -static -Wall -fomit-frame-pointer -funroll-all-loops -O3 -DNDEBUG 

Lab4: main.o placement.o basicDS.o 
	$(CXX) $(OFLAGS) main.o placement.o basicDS.o  -o Lab4

main.o: main.cpp
	$(CXX) $(OFLAGS)  main.cpp -c
	
placement.o: placement.cpp 
	$(CXX) $(OFLAGS) placement.cpp -c
 
basicDS.o: basicDS.cpp
	$(CXX) $(OFLAGS) basicDS.cpp -c


clean:
	rm -rf *.o *.gch Lab4
