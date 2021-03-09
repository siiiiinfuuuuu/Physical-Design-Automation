all: Lab1

CXX = g++ -g

OFLAGS = -pedantic -Wall -fomit-frame-pointer -funroll-all-loops -O3 -DNDEBUG 

LINKFLAGS = -static 

Lab1: main.o algo.o edge.o node.o dm.o parser.o
	$(CXX) $(LINKFLAGS) main.o algo.o edge.o node.o dm.o parser.o -o Lab1

main.o: main.cpp
	$(CXX) $(LINKFLAGS) -DCOMPILETIME="\"`date`\"" main.cpp -c

algo.o: algo.cpp algo.h
	$(CXX) $(LINKFLAGS) algo.cpp -c

node.o: node.cpp dm.h
	$(CXX) $(LINKFLAGS) node.cpp -c

edge.o: edge.cpp dm.h
	$(CXX) $(LINKFLAGS) edge.cpp -c

dm.o: dm.cpp dm.h
	$(CXX) $(LINKFLAGS) dm.cpp -c

parser.o: parser.cpp parser.h
	$(CXX) $(LINKFLAGS) parser.cpp -c

clean:
	rm -rf *.o *.gch Lab1
