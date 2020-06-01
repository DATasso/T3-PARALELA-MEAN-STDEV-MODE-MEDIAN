#!/bin/bash -x

echo "Construyendo el proyecto"
make clean && make

echo "Ejecutando el proyecto"
time mpirun --hostfile maquinas.txt ./dist/promediador-mpi /media/Compartido/puntajes.csv
