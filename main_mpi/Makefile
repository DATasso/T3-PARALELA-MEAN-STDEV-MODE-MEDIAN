CXX = mpic++
CXXFLAGS = -std=c++11 -g3 -Wall -fopenmp
MKDIR = mkdir -p

LIBS = -lm -lmpi

directorios:
	$(MKDIR) build dist

main_mpi.o: directorios main_mpi.cpp
	$(CXX) $(CXXFLAGS) -c main_mpi.cpp -o build/main_mpi.o

all: clean main_mpi.o
	$(CXX) $(CXXFLAGS) -o dist/promediador-mpi build/main_mpi.o  $(LIBS) 
	rm -fr build

clean:
	rm -fr *.o a.out core promediador-mpi dist build *.csv

.DEFAULT_GOAL := all
