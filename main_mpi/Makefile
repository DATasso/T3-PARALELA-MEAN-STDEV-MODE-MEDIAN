CXX = mpic++
CXXFLAGS = -std=c++11 -g3 -Wall -fopenmp
MKDIR = mkdir -p

LIBS = -lm -lmpi

directorios:
	$(MKDIR) build dist

main.o: directorios main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o build/main.o

all: clean main.o
	$(CXX) $(CXXFLAGS) -o dist/promediador-mpi build/main.o  $(LIBS) 
	rm -fr build

clean:
	rm -fr *.o a.out core promediador-mpi dist build *.csv

.DEFAULT_GOAL := all
