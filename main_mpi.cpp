#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "omp.h"
#include <map>
#include <cmath>
#include <algorithm>
#include <omp.h>


using namespace std;

/**
 * Función que muestra los participantes del grupo
 */
void participante();

/**
 * 
 * @param linea Línea leída desde un archivo
 * @return el arreglo con los puntajes
 */
void obtenerPuntajes(std::string linea, int puntajes[]);

// funcion no utilizada
// void anadirSeparados(std::vector<int> puntajes, std::vector<int> &vNem, std::vector<int> &vRanking, std::vector<int> &vMatematica, std::vector<int> &vLenguaje, std::vector<int> &vCiencias, std::vector<int> &vHistoria);

void calculos(std::vector<int> &V, long long suma, std::string nombre);

/**
 * Taller computacional
 * @param argc cantidad de argumentos
 * @param argv argumentos
 * @return El código de salida del programa
 */


int main(int argc, char** argv) {
    int mi_rango;
    int procesadores;
    int master = 0;
    int tag = 0;
    MPI_status estado;

    if (argc > 1) {

        MPI_INIT(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);
        MPI_Comm_size(MPI_COMM_WORLD, &procesadores);
        if(procesadores < 2) {
            fprintf(stderr, "Se require al menos 2 procesadores");
            return EXIT_FAILURE;
        }
        std::vector<int> vectores[6];
        long long sumas[] = {0, 0, 0, 0, 0, 0};
        int valores[6];
        std::string strings[] = {"Nem", "Ranking", "Matematica", "Lenguaje", "Ciencias", "Historia"};

        if(mi_rango == 0) {
            std::string ruta(argv[1]);

            std::ifstream entrada(ruta);
            /**
             * 0 = Nem
             * 1 = Ranking
             * 2 = Matematica
             * 3 = Lenguaje
             * 4 = Ciencias
             * 5 = Historia
             */
        
            int procesador = 1;

            if (entrada) {
                for (std::string linea; getline(entrada, linea);) {
                    MPI_Send(linea.c_str(), lectura.length() +1, MPI_CHAR, procesador, tag, MPI_COMM_WORLD);

                    procesador += 1;
                    if (procesador >= procesadores) {
                        procesador = 2;
                    }
                }
                MPI_Send("STOP", 5, MPI_CHAR, procesador, tag, MPI_COMM_WORLD)
                entrada.close();
            }
            else {
                // Tirrar un error
                std::cout << "Error, no se pudo abrir el archivo" << std::endl;
                participante();
            }
        } else if (mi_rango == 1) {
            while(true) {
                char* textoxd = (char*) calloc(100, sizeof(char));
                MPI_Recv(textoxd, 100, MPI_CHAR, master, tag, MPI_COMM_WORLD, &estado);
                std::string flojera(textoxd);
                if(flojera != "STOP") { 
                    int puntajes[7];
                    obtenerPuntajes(textoxd, puntajes);
                    
                    #pragma omp parallel for
                    for(int i = 0; i < 6; i++) {
                        valores[i] = puntajes[i+1];
                        vectores[i].push_back(valores[i]);
                        sumas[i] += valores[i];
                    }
                    free(textoxd);
                } else {
                    //ITS TIME TO STOP
                    break;
                }
            }
        }
    }
    MPI_Finalize();
    
  /// MOVER AL FINAL
    #pragma omp parallel for
    for(int i = 0; i < 6; i++) {
        calculos(vectores[i], sumas[i], strings[i]);
    }
    participante();

    return EXIT_SUCCESS;
}

void participante() {
    std::cout << std::endl << "=== Tarea ===" << std::endl;
    std::cout << "Daniel Aguilera Tasso" << std::endl << "Nicolas Andrews Sandolva" << std::endl << "Allan Morales Prado" << std::endl;
}

void obtenerPuntajes(std::string linea, int puntajes[]) {
    vector<int> arreglo;
    std::stringstream ss(linea);
    std::string item;

    while (std::getline(ss, item, ';')) {
        int valor = atoi(item.c_str());
        arreglo.push_back(valor);
    }

    #pragma omp simd
    for(int i = 0; i < arreglo.size(); i++) {
        puntajes[i] = arreglo[i];
    }
}

/* No utilizada
void anadirSeparados(std::vector<int> puntajes, std::vector<int> &vNem, std::vector<int> &vRanking, std::vector<int> &vMatematica, std::vector<int> &vLenguaje, std::vector<int> &vCiencias, std::vector<int> &vHistoria) {
    int nem = puntajes.at(1);
    vNem.push_back(nem);
    int ranking = puntajes.at(2);
    vRanking.push_back(ranking);
    int matematica = puntajes.at(3);
    vMatematica.push_back(matematica);
    int lenguaje = puntajes.at(4);
    vLenguaje.push_back(lenguaje);
    int ciencias = puntajes.at(5);
    vCiencias.push_back(ciencias);
    int historia = puntajes.at(6);
    vHistoria.push_back(historia);
} */

void calculos(std::vector<int> &V, long long suma, std::string nombre){
    sort(V.begin(), V.end());
    int largo = V.size();/* 
    long int suma = 0;
    for(int i = 0; i < largo ; i++){
        suma += V.at(i);
    } */
    float avg = (float)(suma)/(float)(largo);
    float mediana;
    int aux = largo/2;
    if(largo%2==0){
        mediana = (V.at(aux) + V.at(aux-1))/2;
    }
    else{
        mediana = V.at(aux);
    }
    double sumaCuadrados = 0;
    map<int,int> contadores;
    for (int it = 0; it < largo ; it++){
        ++contadores[V.at(it)];
        double resta = V.at(it) - avg;
        sumaCuadrados += resta*resta;
    }
    unsigned maximoActual = 0;
    unsigned moda = 0;
    for(auto it = contadores.cbegin(); it != contadores.cend();++it){
        if (it ->second > maximoActual){
            moda = it->first;
            maximoActual = it->second;
        }
    }
    float stdev = sqrt(sumaCuadrados/largo);
    #pragma omp critical
    std::cout << "===" << nombre << "===" << std::endl << "Promedio: " << avg << std::endl << "Desviación Estándar: " << stdev << std::endl << "Moda: " << moda << std::endl << "Mediana: " << mediana << std::endl;
}