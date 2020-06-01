#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "omp.h"
#include <numeric>
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
vector<int> obtenerPuntajes(std::string linea);

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

    if (argc > 1) {
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
        std::vector<int> vectores[6];
        long long sumas[] = {0, 0, 0, 0, 0, 0};
        int valores[6];
        std::string strings[] = {"Nem", "Ranking", "Matematica", "Lenguaje", "Ciencias", "Historia"};
        

        if (entrada) {

        #pragma omp parallel
            {
        #pragma omp single
            for (std::string linea; getline(entrada, linea);) {
        #pragma omp task
            {
                vector<int> puntajes = obtenerPuntajes(linea);
                if (puntajes.size() >= 6) {
                    #pragma omp parallel for
                    for(int i = 0; i < 6; i++) {
                        valores[i] = puntajes.at(i+1);
                        vectores[i].push_back(valores[i]);
                        sumas[i] += valores[i];
                    }
                }
                puntajes.clear();
            }
            }
        }
            #pragma omp parallel for
            for(int i = 0; i < 6; i++) {
                calculos(vectores[i], sumas[i], strings[i]);
            }
            participante();
        }
        else {
            // Tirrar un error
            std::cout << "Error, no se pudo abrir el archivo" << std::endl;
        participante();
        }
    }
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
    #pragma omp critical
    std::cout << "===" << nombre << "===" << std::endl << "Promedio: " << avg << std::endl << "Desviación Estándar: " << stdev << std::endl << "Moda: " << moda << std::endl << "Mediana: " << mediana << std::endl;
}