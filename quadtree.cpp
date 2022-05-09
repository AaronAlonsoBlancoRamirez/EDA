#include <iostream>
#include <utility>
#include <vector>
#include <array>
using namespace std;



struct punto
{
    pair<double, double> coordenadas;


    punto(double x, double y)
    {
        this->coordenadas.first = x;
        this->coordenadas.second = y;
    }

    void printp()
    {
        cout << "punto: " << this->coordenadas.first << ", " << this->coordenadas.second << endl;
    }
};

struct quad_nodo
{
    
    vector<punto> puntos;
    quad_nodo* hijos[4];
    punto* area[2];
   

  

    quad_nodo(punto eje_x, punto eje_y)
    {
        this->area[0] = new punto(eje_x);
        this->area[1] = new punto(eje_y);

        for (int i = 0; i < 4; ++i)
            hijos[i] = nullptr;
    }


    void printnodo()
    {
        cout << "rango del rectangulo " << endl;
        this->area[0]->printp();
        this->area[1]->printp();
        cout << "puntos dentro: " << endl;
        for (int i = 0; i < puntos.size(); ++i)
        {
            puntos[i].printp();
        }
        cout << endl;
    }
};

struct quad_tree
{
    quad_nodo* root;
    //int hijos[4];
    int profundidad;
    

   
    quad_tree(int p, punto eje_x, punto eje_y)
    {
        this->profundidad = p;
        this->root = new quad_nodo(eje_x, eje_y);

        quad_nodo** b = &root;
        build(b, profundidad);
    }

   

    void build(quad_nodo** gen, int d)
    {
        if (d > 1)
        {
            double eje_x = (*gen)->area[0]->coordenadas.first - ((*gen)->area[0]->coordenadas.first - (*gen)->area[1]->coordenadas.first) / 2;
            double eje_y = (*gen)->area[1]->coordenadas.second - ((*gen)->area[1]->coordenadas.second - (*gen)->area[0]->coordenadas.second) / 2;

            (*gen)->hijos[0] = new quad_nodo(punto((*gen)->area[1]->coordenadas.first, eje_y), punto(eje_x, (*gen)->area[0]->coordenadas.second ));
            (*gen)->hijos[1] = new quad_nodo(punto({ (*gen)->area[1]->coordenadas.first, (*gen)->area[1]->coordenadas.second }), punto({ eje_x, eje_y }));
            (*gen)->hijos[2] = new quad_nodo(punto({ eje_x,  (*gen)->area[1]->coordenadas.second }), punto({ (*gen)->area[0]->coordenadas.first, eje_y }));
            (*gen)->hijos[3] = new quad_nodo(punto({ eje_x, eje_y }), punto({ (*gen)->area[0]->coordenadas.first, (*gen)->area[1]->coordenadas.first }));
                                                                              
            for (int i = 0; i < 4; ++i)
            {
                build(&(*gen)->hijos[i], d - 1);
            }
        }
    }

    void insert(punto p, quad_nodo** ptr)
    {
       
        if ((*ptr) == root) {
                (*ptr)->puntos.push_back(p);
        }
        for (int i = 0; i < 4; ++i)
        {
            if ((*ptr)->hijos[i] and verificar(p, *(*ptr)->hijos[i]))
            {
                (*ptr)->hijos[i]->puntos.push_back(p);
                insert(p, &(*ptr)->hijos[i]);
            }
        }

        
        
    }

    //VERICADO SI EL PUNTO  PERTENECE AL NODO , VERIFICANDO QUE ESTE DENTRO DEL AREA DEL NODO
    bool verificar(punto p, quad_nodo ptr)
    {
        if (p.coordenadas.first >= ptr.area[0]->coordenadas.first and p.coordenadas.second <= ptr.area[0]->coordenadas.second and p.coordenadas.first <= ptr.area[1]->coordenadas.first and p.coordenadas.second >= ptr.area[1]->coordenadas.second)
            return true;
        return false;
    }

   
    //IMPRIME EL QUADTREE
    void printtree(quad_nodo* aux) {
        if (!aux) {
            return;
        }
        aux->printnodo();
        printtree(aux->hijos[0]);
        printtree(aux->hijos[1]);
        printtree(aux->hijos[2]);
        printtree(aux->hijos[3]);

    }

   
};



int main()
{
    punto eje_y(0, 60), eje_x(60, 0);

    quad_tree t(2, eje_x, eje_y);


    quad_nodo** a= &(t.root);

   

    t.insert(punto(1, 2), a);
    t.insert(punto(15, 35), a);
    t.insert(punto(35, 35), a);
    t.insert(punto(35, 15), a);

    t.printtree(t.root);
}
