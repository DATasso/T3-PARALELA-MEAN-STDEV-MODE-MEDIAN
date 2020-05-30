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
void calculos(std::vector<int> &V, long int suma, std::string nombre);

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
        std::vector<int> vNem, vRanking, vMatematica, vLenguaje, vCiencias, vHistoria;
        long int sumaNem = 0, sumaRanking = 0, sumaMatematica = 0, sumaLenguaje = 0, sumaCiencias = 0, sumaHistoria = 0;
        

        if (entrada) {
            for (std::string linea; getline(entrada, linea);) {
                vector<int> puntajes = obtenerPuntajes(linea);
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
                puntajes.clear();
            }
            calculos(vNem, sumaNem, "Nem");
            calculos(vRanking, sumaRanking, "Ranking");
            calculos(vMatematica, sumaMatematica, "Matemática");
            calculos(vLenguaje, sumaLenguaje, "Lenguaje");
            calculos(vCiencias, sumaCiencias, "Ciencias");
            calculos(vHistoria, sumaHistoria, "Historia");
            participante();
        }
        else {
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
