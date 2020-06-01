#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "mpi.h"
#include <numeric>
#include <map>
#include <cmath>
#include <algorithm>

#define LARGO 100

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
vector<int> obtenerPuntajes(std::string linea);

/**
 * 
 * @param procesados Arreglo con procesadores
 * @param largo Cantidad de procesadores
 * @return true si alguno se true 
 */
bool isAnyOk(bool *procesados, int largo);

void calculos(std::vector<int> &V, long int suma, std::string nombre);

/**
 * Taller computacional
 * @param argc cantidad de argumentos
 * @param argv argumentos
 * @return El código de salida del programa
 */
int main(int argc, char** argv) {
    int mi_rango; /* rango del proceso    */
    int procesadores; /* numero de procesos   */
    int maestro = 0; /* Identificador maestro */
    int escritor = 1; /* Procesador escritor */
    int tag = 0; /* etiqueta del mensaje */
    MPI_Status estado; /* devuelve estado al recibir*/
    std::vector<int> vNem, vRanking, vMatematica, vLenguaje, vCiencias, vHistoria;
    long int sumaNem = 0, sumaRanking = 0, sumaMatematica = 0, sumaLenguaje = 0, sumaCiencias = 0, sumaHistoria = 0;
    /* Este string se usara para detener los hilos paralelos */
    std::string parar("STOP");

    if (argc > 1) {


        /* Comienza las llamadas a MPI */
        MPI_Init(&argc, &argv);

        /* Averiguamos el rango de nuestro proceso */
        MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);

        /* Averiguamos el número de procesos que estan 
         * ejecutando nuestro porgrama 
         */
        MPI_Comm_size(MPI_COMM_WORLD, &procesadores);

        if (procesadores < 3) {
            fprintf(stderr, "\nLa implementación requiere al menos 3 procesadores\n");
            return EXIT_FAILURE;
        }


        if (mi_rango == 0) {
            /* Si paso por acá, soy el proceso MAESTRO */
            /**
             * 
             * Generación de archivos
             */

            std::string ruta(argv[1]);
            std::ifstream entrada(ruta);
            if (entrada) {
                /*
                 * En esta estrategia usaremos 
                 * El procesador 0 como maestro (lee y distribuye)
                 * El procesador 1 como escritor (recibe los promedios y escribe en disco)
                 * Los demás procesadores como "promediadores"
                 */
                int procesador = 2;
                for (std::string lectura; getline(entrada, lectura);) {
                    if (!lectura.empty()) {
                        /* Usa length +1 para que la marca /0 se transmita */
                        MPI_Send((char *) lectura.c_str(), lectura.length() + 1, MPI_CHAR, procesador, tag, MPI_COMM_WORLD);

                        procesador += 1;
                        if (procesador >= procesadores) {
                            procesador = 2;
                        }
                    }
                }
                std::cout << std::endl;

                for (procesador = 2; procesador < procesadores; procesador++) {
                    MPI_Send((char *) parar.c_str(), parar.length() + 1, MPI_CHAR, procesador, tag, MPI_COMM_WORLD);
                }

                entrada.close();
            }
        } else {
            /* Obtengo el mensajes */
            bool procesados[procesadores];
            for (int i = 0; i < procesadores; i++) {
                procesados[i] = true;
            }
            procesados[0] = false;
            while (true) {
                char* mensaje = (char *) calloc(LARGO, sizeof (char));
                MPI_Recv(mensaje, LARGO, MPI_CHAR, maestro, tag, MPI_COMM_WORLD, &estado);
                std::string fila(mensaje);
                if (parar.compare(fila) == 0 || fila.empty()) {
                    procesados[mi_rango] = false;
                    // No hay datos, se debe salir del loop
                    break;
                } else {
                    vector<int> puntajes = obtenerPuntajes(fila);
                    if (puntajes.size() >= 6) {
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
                        sumaNem += nem;
                        sumaRanking += ranking;
                        sumaMatematica += matematica;
                        sumaLenguaje += lenguaje;
                        sumaCiencias += ciencias;
                        sumaHistoria += historia;
                    }
                }
                free(mensaje);
            }
            if (!isAnyOk(procesados, procesadores)){
                calculos(vNem, sumaNem, "Nem");
                calculos(vRanking, sumaRanking, "Ranking");
                calculos(vMatematica, sumaMatematica, "Matemática");
                calculos(vLenguaje, sumaLenguaje, "Lenguaje");
                calculos(vCiencias, sumaCiencias, "Ciencias");
                calculos(vHistoria, sumaHistoria, "Historia");
                participante();
            }

        }

        /* Termina con MPI. Recordemos que después de 
         * esta llamada no podemos llamar a funciones 
         * MPI, ni siquiera de nuevo a MPI_Init 
         */
        MPI_Finalize();
    }
    /**
     * 
     * Muestro los participantes.
     */


    return EXIT_SUCCESS;
}

void participante() {
    std::cout << std::endl << "=== Tarea ===" << std::endl;
    std::cout << "Daniel Aguilera Tasso" << std::endl << "Nicolas Andrews Sandolva" << std::endl << "Allan Morales Prado" << std::endl;
}

vector<int> obtenerPuntajes(std::string linea) {
    vector<int> arreglo;
    std::stringstream ss(linea);
    std::string item;

    while (std::getline(ss, item, ';')) {
        int valor = atoi(item.c_str());
        arreglo.push_back(valor);
    }

    return arreglo;
}

bool isAnyOk(bool *procesados, int largo) {
    bool result = false;
    for (int i = 0; i < largo; i++) {
        bool ok = procesados[i];
        if (ok) {
            result = true;
        }
    }
    return result;
}

void calculos(std::vector<int> &V, long int suma, std::string nombre){
    sort(V.begin(), V.end());
    int largo = V.size();
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
    unsigned maximoActual = 0;
    unsigned moda = 0;
    map<int,int> contadores;
    for (int it = 0; it < largo ; it++){
        ++contadores[V.at(it)];
        double resta = V.at(it) - avg;
        sumaCuadrados += resta*resta;
        if(contadores[V.at(it)]>maximoActual){
            moda = V.at(it);
            maximoActual = contadores[V.at(it)];
        }
    }
    float stdev = sqrt(sumaCuadrados/largo);
    cout << "===" << nombre << "===" << std::endl << "Promedio: " << avg << std::endl << "Desviación Estándar: " << stdev << std::endl << "Moda: " << moda << std::endl << "Mediana: " << mediana << std::endl;
}