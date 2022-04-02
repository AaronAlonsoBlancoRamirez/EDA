#include <iostream>
#include <random>
using namespace std;
#include <math.h>
#include <vector>
#include <algorithm>
#include <cstdlib>

int main(){
    const int nrolls = 20000;
    vector<int> distancias;
    int number;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, 100);


    //de acuerdo a la dimension se creara una matriz
    // de tamaño 20000*dimension y almacenara los datos
    //aleatorios
    const int dimension = 1;
    int p1[20000][1];


    //insercion de datos aleatorios 
    for (int i = 0; i < nrolls; ++i) {
        for (int j = 0; j < dimension; j++) {

            number = distribution(generator);

            p1[i][j] = number;

        }
    }


    int distancia=0;
    int resta=0;
    int sumatoria=0;

    int value = p1[1][dimension];//PIVOTE PARA SACAR DISTANCIAS


    //SACA LAS DISTANCIAS SEGUN EL TAMAÑO DE DIMENSION  
    for (int i = 0; i < 20000; ++i)
    {
        for (int j = 0; j < dimension; j++) {
            if (p1[1][j] != value)
            {
                resta = p1[i][j] - p1[1][j];
                distancia = resta * resta;
                sumatoria = sumatoria + distancia;


            }

        }
        sumatoria = sqrt(sumatoria);
        distancias.push_back(sumatoria);
    }

    //ORDENA EL VECTOR "DISTANCIAS" QUE ALMACENA 
    //LAS DISTANCIAS
    sort(distancias.begin(), distancias.end());
    int cont = 0;

    //SE CALCULA LAS INSTANCIAS QUE HAY 
    //DE LAS DIFERENTES DISTANCIAS QUE
    //SE ALMACENARON
    for (int j = 0; j < 19998; j++) {
        if (distancias[j] == distancias[j + 1]) {
            cont++;
        }
        else {
            cout << distancias[j] << " " << cont << endl;
            cont = 0;
        }


    }


    return 0;
}
