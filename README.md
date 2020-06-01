# T3-PARALELA-MEAN-STDEV-MODE-MEDIAN
El taller consiste en escribir un programa en C++,Dart, openMP(C++) y openMPI(C++), que pueda leer un archivo con puntajes psu de una persona con el siguiente formato "rut;nem;ranking;lenguaje;matematicas;ciencias;historia" y luego deberá mostrar el promedio, desviación estándar, mediana y la moda de cada puntaje (nem;ranking;lenguaje;matematicas;ciencias;historia).

### Archivos
- ` 	calculos.dart` contiene el programa en Dart
- `   main.cpp` contiene el programa en C++
- ` 	main_omp.cpp` contiene el programa en C++ utilizando OMP
- ` 	main_mpi/main_mpi.cpp` contiene el programa en C++ utilizando MPI

### Instalar Packages Dart
```
pub install
```
### Compilación
Para C++ Secuencial:
```
g++ main.cpp
g++ main_omp.cpp -fopenmp
```
### Ejecución
- C++ Windows
    `./main.exe`
- C++ Linux
    `./a.out rutaArchivo.csv`
- Dart
    `dart calculos.dart rutaArchivo.csv`
- C++ MPI Linux (dentro de la carpeta main_mpi)
```
make
./dist/programa-mpi`
```
#### Autores
- Daniel Aguilera Tasso
- Nicolás Andrews Sandoval
- Allan Morales Prado

(Grupo 6)
