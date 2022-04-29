#include <iostream>
using namespace std;
#include <vector>
using std::vector;
#include <climits>
#include <memory>
#include <stack>
#include <math.h>
#include <queue>

#define loop(n) for(int ii = 0; ii < n; ++ ii)
#define dim 5;


struct Punto {

    double* data;  // p1(10,20)
    int kd;


    Punto() {
        this->kd = 0;
        this->data = nullptr;
    }
    Punto(int kd, double* data) {
        this->kd = kd;
        this->data = new double[kd];
        loop(kd) {
            this->data[ii] = data[ii];

        }
    }



    double* getData() const;

    void setData(double* data);

    int getKd() const;

    void setKd(int kd);

    bool comparar_punto(Punto p) {
        loop(p.kd) {
            if (this->data[ii] != p.data[ii]) {
                return 0;
            }
        }
        return 1;
    }

    bool iguales(Punto p) {
        return (comparar_punto(p));
    }

    double distancia_euclidiana(Punto p) {
        int ans = 0;
        loop(p.kd) {
            ans = ans + pow(this->data[ii] - p.data[ii], 2);
        }

        return sqrt(ans);
    }

    void printp()
    {
        
        for (int i = 0; i < kd; ++i)
            cout << data[i] << " ";
        cout << endl;
    }


};





double* Punto::getData() const {
    return data;
}

void Punto::setData(double* data) {
    Punto::data = data;
}

int Punto::getKd() const {
    return kd;
}

void Punto::setKd(int kd) {
    Punto::kd = kd;
}

struct Nodo {
    Punto p;

    Nodo* hijos[2]{};


    Nodo() {
        this->hijos[0] = nullptr;
        this->hijos[1] = nullptr;
    }
    explicit Nodo(Punto punto) {
        this->p.setKd(punto.getKd());
        this->p.setData(new double(punto.getKd()));
        this->p = punto;
        this->hijos[0] = nullptr;
        this->hijos[1] = nullptr;
    }
    Nodo(Punto punto, Nodo* izq, Nodo* der) {
        this->p = punto;
        this->hijos[0] = izq;
        this->hijos[1] = der;
    }
};

bool comp(const pair<Nodo*, double> a, const pair<Nodo*, double> b)
{
    return a.second < b.second;
}

struct kd_tree
{

    Nodo* raiz;
    int kd;

    kd_tree() {

    }

    explicit kd_tree(int kd) {
        this->kd = kd;
        this->raiz = nullptr;
    }




    bool buscar(Punto pt, Nodo**& nodoactual, int altura = 0) {

        if (*nodoactual == nullptr) {
            return false;
        }
        if ((*nodoactual)->p.iguales(pt)) {
            return true;
        }
        if (pt.data[altura % kd] <= (*nodoactual)->p.data[altura % kd]) {
            nodoactual = &(*nodoactual)->hijos[0];
            altura++;
            buscar(pt, nodoactual, altura);
        }
        else {
            nodoactual = &(*nodoactual)->hijos[1];
            altura++;
            buscar(pt, nodoactual, altura);
        }

    }

    bool insertar(Punto pt) {
        Nodo** nodoactual = &raiz;

        if (buscar(pt, nodoactual)) {

            return false;
        }
        *nodoactual = new Nodo(pt);
        return true;
    }

    void borrar(Punto p)
    {
        Nodo** ptr = &raiz;
        if (buscar(p, ptr))
        {
            *ptr = nullptr;
            
        }

    }

    bool comparando(vector<pair<Nodo*, double>> k_nearest_neighbors_candidates, double d)
    {
        for (int i = 0; i < k_nearest_neighbors_candidates.size(); ++i)
        {
            if (d < k_nearest_neighbors_candidates[i].second)
                return true;
        }
        return false;


    }

    void n_n(double& bd, Nodo* piv, Nodo* current_node, Nodo*& n_n_candidate, double best_distance = LLONG_MAX, int altura = 0) {
        if (!current_node) {
            return;
        }
        
        double dis = current_node->p.distancia_euclidiana(piv->p);
        if (dis < best_distance) {
            
            best_distance = dis;
            bd = best_distance;
            n_n_candidate = current_node;
        }
        int axis = altura % kd;
        bool right = false;
        if (piv->p.data[axis] > current_node->p.data[axis]) {
            right = true;
            n_n(bd, piv, current_node->hijos[1], n_n_candidate, bd, altura + 1);
        }
        else {
            right = false;
            n_n(bd, piv, current_node->hijos[0], n_n_candidate, bd, altura + 1);

        }
        if (fabs(current_node->p.data[axis] - piv->p.data[axis]) < bd) {
            if (right) {
                n_n(bd, piv, current_node->hijos[0], n_n_candidate, bd, altura + 1);
            }
            else {
                n_n(bd, piv, current_node->hijos[1], n_n_candidate, bd, altura + 1);

            }
        }
    }

    

    void k_n_n(int k_vecinos, Nodo* piv, Nodo* current_node, vector<pair<Nodo*, double>> &k_nearest_neighbors_candidates, int altura = 0) {

        if (!current_node) {
            return;
        }
        
        
        k_nearest_neighbors_candidates.push_back(pair<Nodo*, double>{current_node, current_node->p.distancia_euclidiana(piv->p)});
        sort(k_nearest_neighbors_candidates.begin(), k_nearest_neighbors_candidates.end(), comp);

        if (k_nearest_neighbors_candidates.size() > k_vecinos) {
            k_nearest_neighbors_candidates.pop_back();
        }



        int axis = altura % kd;
        bool right = false;
        if (piv->p.data[axis] > current_node->p.data[axis]) {
            right = true;
            k_n_n(k_vecinos, piv, current_node->hijos[1], k_nearest_neighbors_candidates, altura + 1);
        }
        else {
            right = false;
            k_n_n(k_vecinos, piv, current_node->hijos[0], k_nearest_neighbors_candidates, altura + 1);

        }
        if(comparando(k_nearest_neighbors_candidates,fabs(current_node->p.data[axis] - piv->p.data[axis]))) { 
            if (right) {
                k_n_n(k_vecinos, piv, current_node->hijos[0], k_nearest_neighbors_candidates, altura + 1);
            }
            else {
                k_n_n(k_vecinos, piv, current_node->hijos[1], k_nearest_neighbors_candidates, altura + 1);

            }
        }


    }
    void print_knn(vector<pair<Nodo*, double>> knn) {
        for (int i = 0; i < knn.size(); ++i)
        {
            knn[i].first->p.printp();
            cout << " distancia: "<<knn[i].second<<endl;
        }
       
    }
   
    void range_query(double rango,Nodo *piv, Nodo *current_node, vector<pair<Nodo *, double>>& k_nearest_neighbors_candidates , int altura=0) {

        if (!current_node) {
            return;
        }

        if (current_node->p.distancia_euclidiana(piv->p) < rango) {
            k_nearest_neighbors_candidates.push_back(pair<Nodo*, double>{current_node, current_node->p.distancia_euclidiana(piv->p)});
            sort(k_nearest_neighbors_candidates.begin(), k_nearest_neighbors_candidates.end(), comp);
        }

        int axis = altura % kd;
        bool right = false;
        if (piv->p.data[axis] > current_node->p.data[axis]) {
            right = true;
            range_query( rango,piv, current_node->hijos[1], k_nearest_neighbors_candidates, altura + 1);
        }
        else {
            right = false;
            range_query(rango, piv, current_node->hijos[0], k_nearest_neighbors_candidates, altura + 1);

        }
        if (comparando(k_nearest_neighbors_candidates, fabs(current_node->p.data[axis] - piv->p.data[axis]))) {
            if (right) {
                range_query(rango, piv, current_node->hijos[0], k_nearest_neighbors_candidates, altura + 1);
            }
            else {
                range_query(rango, piv, current_node->hijos[1], k_nearest_neighbors_candidates, altura + 1);

            }
        }

    }



};







int main() {

    double arr1[2] = { 51, 75 };
    Punto p1(2, arr1);
    double arr2[2] = { 70, 70 };
    Punto p2(2, arr2);
    double arr3[2] = { 25, 40 };
    Punto p3(2, arr3);
    double arr4[2] = { 10, 30 };
    Punto p4(2, arr4);
    double arr5[2] = { 35, 90 };
    Punto p5(2, arr5);
    double arr6[2] = { 55, 1 };
    Punto p6(2, arr6);
    double arr7[2] = { 60, 80 };
    Punto p7(2, arr7);
    double arr8[2] = { 1, 10 };
    Punto p8(2, arr8);
    double arr9[2] = { 50, 50 };
    Punto p9(2, arr9);

   



    kd_tree tree(2);

    tree.insertar(p1);
    tree.insertar(p2);
    tree.insertar(p3);
    tree.insertar(p4);
    tree.insertar(p5);
    tree.insertar(p6);
    tree.insertar(p7);
    tree.insertar(p8);
    tree.insertar(p9);


    //EL P10 ES EL PUNTO QUE SIRVE COMO PIVOTE PARA LAS FUNCIONES NEAREST_NEIGHBOR , KNN Y RANGE QUERY

    double arr10[2] = { 20, 10 };
    Punto p10(2, arr10);
    Nodo* ptr = new Nodo(p10), * nn = nullptr;
    
    priority_queue<double> knn;
    double best_distance = 0;
    vector<pair<Nodo*, double>> vecinos;
    int k = 4;                  //SE DECLARA EL NUMERO DE VECINOS QUE SE QUIERE ENCONTRAR
    double rango = 30;          //SE DECLARA EL RANGO PARA LA FUNCION RANGE QUERY


    //PROBANDO LA FUNCION NEAREST NEIGHBOR "tree.n_n"

    tree.n_n(best_distance, ptr, tree.raiz, nn);
    
    cout << "Punto ingresado: ";
    ptr->p.printp(); 
    cout << "nearest neighbor: "; nn->p.printp();
    cout << "con distancia: " << best_distance;


    //PROBANDO LA FUNCION K-NEAREST NEIGHBORS "tree.k_n_n"
    /*tree.k_n_n(k, ptr, tree.raiz, vecinos);
    tree.print_knn(vecinos);*/

    //PROBANDO EL RANGE QUERY "tree.range_query"
    /*tree.range_query(rango, ptr, tree.raiz, vecinos);
    tree.print_knn(vecinos);*/
    

    return 0;
}
