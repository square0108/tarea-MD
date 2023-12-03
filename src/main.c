#include <stdio.h>
#include <stdlib.h>

/* **************************** *
 * Constantes
******************************* */

// Numero total de vertices en el grafo del Centro Concepcion
#define MAX_VERTICES 110
#define GRADO_MAXIMO 6

// Distancia infinita
#define INF 999

// Matriz de adyacencia
float mat_ady[MAX_VERTICES][MAX_VERTICES];

/* **************************** *
 * Structs y enums
******************************* */

typedef struct nodo
{
    int indice;
    struct nodo* link; // lista de nodos al que esta instancia de nodo apunta
}Nodo;

/* **************************** *
 * Prototipos
******************************* */

void crear_arco(Nodo* inicio, Nodo* final, float peso);
void borrar_arco(Nodo* inicio, Nodo* final);
void print_arcos(int id);
void print_peso_arco(int inicio, int final);
int* numrange(int start, int end);
int* elems_plus_one(int arr[8]);
void eliminar_primer_elem(int* arr, int tamano);
int* dijkstra(int inicio, int final);

/* **************************** *
 * Funciones
******************************* */

int main(void) {
    // calles "horizontales", todas son largo 14, menos Freire y Barros Arana.
    int* Carrera = numrange(0,13);
    int* Maipu = numrange(14,27);
    int* Freire = numrange(28,40);
    int* BarrosArana = numrange(41,53);
    int* OHiggins = numrange(54,67);
    int* SanMartin = numrange(68,81);
    int* COCKrane = numrange(82,95);
    int* Chacabuco = numrange(96,109);

    // calles "verticales", todas son largo 8, menos Anibal Pinto, Colo Colo.
    int Prat[] = {0,14,28,41,54,68,82,96};
    int* Serrano = elems_plus_one(Prat);
    int* Salas = elems_plus_one(Serrano);
    int* Angol = elems_plus_one(Salas);
    int* Lincoyan = elems_plus_one(Angol);
    int* Rengo = elems_plus_one(Lincoyan);
    int* Caupolican = elems_plus_one(Rengo);
    int AnibalPinto[] = {7,21,48,61,75,89,103};
    int ColoColo[] = {8,22,35,62,76,90,104};
    int Castellon[] = {9,23,36,49,63,77,91,105};
    int* Tucapel = elems_plus_one(Castellon);
    int* Orompello = elems_plus_one(Tucapel);
    int* Ongolmo = elems_plus_one(Orompello);
    int* Paicavi = elems_plus_one(Ongolmo);

    Nodo* nodos[MAX_VERTICES];
    int* CallesHorizontales[] = {Carrera, Maipu, Freire, BarrosArana, OHiggins, SanMartin, COCKrane, Chacabuco};
    int* CallesVerticales[] = {Prat, Serrano, Salas, Angol, Lincoyan, Rengo, Caupolican, AnibalPinto, ColoColo, Castellon, Tucapel, Orompello, Ongolmo, Paicavi};

    // inicializacion de los nodos, los deje como struct en caso de algun momento tener que usarlos como tal?
    for (int i = 0; i < MAX_VERTICES; i++) {
        nodos[i] = malloc(sizeof(Nodo));
        (nodos[i])->indice = i;
        nodos[i]->link = NULL;

        for (int j = 0; j < MAX_VERTICES; j++) {
            mat_ady[i][j] = INF;
        }
    }


    // conectando los vertices calle por calle
    // primero, todas las calles horizontales
    for (int indiceCalle = 0; indiceCalle < 8; indiceCalle++) {
        for (int indiceNodo = 0; indiceNodo < 13; indiceNodo++) {
            // las calles Carrera, Freire, OHiggins... ("indice par"), van de derecha-izquierda
            // por ahora solo crear el grafo uniformemente, y luego con la funcion borrar_arco
            // ocuparnos de los nodos "especiales"

            // calles con solo 13 nodos en vez de 14
            if ((CallesHorizontales[indiceCalle] == Freire || CallesHorizontales[indiceCalle] == BarrosArana)
            && indiceNodo == 12) {
                continue;
            }
            // calles con bidireccion: Carrera, Chacabuco
            if ((CallesHorizontales[indiceCalle] == Carrera || CallesHorizontales[indiceCalle] == Chacabuco)) {
                // arco derecha-izquierda
                crear_arco(
                    nodos[(CallesHorizontales[indiceCalle])[indiceNodo+1]],
                    nodos[(CallesHorizontales[indiceCalle])[indiceNodo]],
                    1);
                // archo izquierda-derecha
                crear_arco(
                    nodos[(CallesHorizontales[indiceCalle])[indiceNodo]],
                    nodos[(CallesHorizontales[indiceCalle])[indiceNodo+1]],
                    1);
                continue;
            }

            if (indiceCalle%2 == 0) {
                // arco derecha-izquierda
                crear_arco(
                    nodos[(CallesHorizontales[indiceCalle])[indiceNodo+1]],
                    nodos[(CallesHorizontales[indiceCalle])[indiceNodo]],
                    1);
            }
            else {
                // arco izquierda-derecha
                crear_arco(
                    nodos[(CallesHorizontales[indiceCalle])[indiceNodo]],
                    nodos[(CallesHorizontales[indiceCalle])[indiceNodo+1]],
                    1);
            }
        }
    }
    // luego, todas las calles verticales
    for (int indiceCalle = 0; indiceCalle < 14; indiceCalle++) {
        for (int indiceNodo = 0; indiceNodo < 7; indiceNodo++) {
            if ((CallesVerticales[indiceCalle] == AnibalPinto 
            || CallesVerticales[indiceCalle] == ColoColo)
            && indiceNodo == 6) {
                continue;
            }
            if (indiceCalle%2 == 0) {
                // arco de abajo a arriba
                crear_arco(
                    nodos[(CallesVerticales[indiceCalle])[indiceNodo+1]],
                    nodos[(CallesVerticales[indiceCalle])[indiceNodo]],
                    1);
            }
            else {
                // arco de arriba a abajo
                crear_arco(
                    nodos[(CallesVerticales[indiceCalle])[indiceNodo]],
                    nodos[(CallesVerticales[indiceCalle])[indiceNodo+1]],
                    1);
            }
        }
    }

    // hasta aqui el grafo es solo cuadras regulares. Ahora hay que remover/arreglar arcos.

    // 35-(2)->34: 
    borrar_arco(nodos[35], nodos[34]);
    crear_arco(nodos[35],nodos[34],2);
    // borrar 21-48
    borrar_arco(nodos[21],nodos[48]);
    // 62-(2)->35:
    borrar_arco(nodos[62],nodos[35]);
    crear_arco(nodos[62],nodos[35],2);
    // borrar 48->49
    borrar_arco(nodos[48],nodos[49]);
    // agregar direcciones faltantes a Paicavi (hasta ahora va solo en direccion arriba-abajo)
    crear_arco(nodos[67],nodos[53],1);
    crear_arco(nodos[53],nodos[40],1);
    crear_arco(nodos[40],nodos[27],1);
    crear_arco(nodos[27],nodos[13],1);
    // agregar la diagonal
    crear_arco(nodos[109],nodos[94],1.41F);
    crear_arco(nodos[94],nodos[109],1.41F);
    crear_arco(nodos[94],nodos[79],1.41F);
    crear_arco(nodos[79],nodos[94],1.41F);
    crear_arco(nodos[79],nodos[64],1.41F);

    dijkstra(0, 1423);
}

// devuelve un arreglo con secuencia de numeros start-end, incluyendo el inicio y final dentro del arreglo
int* numrange(int start, int end) {
    int sum = 0;
    for (int i = start; i <= end; i++) {
        sum++;
    }
    int* ar = malloc(sizeof(int)*sum);
    for (int i = 0; i < sum; i++) {
        ar[i] = start+i;
    }
    return ar;
}

// devuelve un arreglo, que es igual al arreglo de entrada, pero sumando 1 a todos sus valores
int* elems_plus_one(int arr[8]) {
    int* ray;
    ray = malloc(sizeof(int)*8);
    for (int i = 0; i < 8; i++) {
        ray[i] = arr[i] + 1;
    }
    return ray;
}

void crear_arco(Nodo* inicio, Nodo* final, float peso) {
    mat_ady[inicio->indice][final->indice] = peso;
    inicio->link = final; // placeholder
}

void borrar_arco(Nodo* inicio, Nodo* final) {
    mat_ady[inicio->indice][final->indice] = INF;
    inicio->link = NULL; // placeholder
}

void print_peso_arco(int inicio, int final) {
    printf("%f", mat_ady[inicio][final]);
}

// para debugging, printea los pares ordenados/arcos que contienen a este vertice
// es decir, aquellos arcos que empiezen o terminen en el vertice ingresado
void print_arcos(int id) {
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (mat_ady[id][i] != INF) printf("(%i --(%f)--> %i),", id, mat_ady[id][i], i);
        if (mat_ady[i][id] != INF) printf("(%i --(%f)--> %i),", i, mat_ady[i][id], id);
    }
}

void eliminar_primer_elem(int* arr, int tamano) {
    for (int i = 0; i < tamano-1; i++) arr[i] = arr[i+1];
    arr[tamano-1] = -1;
}

// por ahora no devuelve nada, pero distancia es un array de MAX_VERTICES elementos con cada indice siendo la distancia
// desde inicio hacia ese nodo

// esta funcion lo que tiene que hacer es recibir indice de inicio e indice final y devolver un array de los nodos por los que pasa
// para esto se me ocurria usar el array padre que basicamente indicaba (por ejemplo, padre[i]) "desde que nodo vino el algoritmo a i para
// recorrer la menor distancia"

// puedes hacerlo como tu quieras, pero eso es la idea que yo tenia por si te sirve
int* dijkstra(int inicio, int final) {
    float* distancia = malloc(sizeof(float) * MAX_VERTICES);
    int* visto = malloc(sizeof(int) * MAX_VERTICES);
    int* padre = malloc(sizeof(int) * MAX_VERTICES);
    int tamanoCola = 0;
    int* cola = calloc(MAX_VERTICES, sizeof(int));
    for (int i = 0; i < MAX_VERTICES; i++) {
        distancia[i] = INF;
        visto[i] = 0;
        padre[i] = -1;
    }
    distancia[inicio] = 0;
    int nodoActual = inicio;
    while (1) {
        int numVerticesAdy = 0;
        int listaAdy[GRADO_MAXIMO];
        for (int i = 0; i < MAX_VERTICES; i++) {
            if (mat_ady[nodoActual][i] != INF && !visto[i]) {
                listaAdy[numVerticesAdy] = i;
                numVerticesAdy++;
                int enCola = 0;
                for (int j = 0; j < tamanoCola; j++) {
                    if (cola[j] == i) enCola = 1;
                }
                if (!enCola) {
                    cola[tamanoCola] = i;
                    tamanoCola++;
                }
            }
        }
        for (int i = 0; i < numVerticesAdy; i++) {
            int nodoSig = listaAdy[i];
            if (distancia[nodoSig] > distancia[nodoActual] + mat_ady[nodoActual][nodoSig]) {
                distancia[nodoSig] = distancia[nodoActual] + mat_ady[nodoActual][nodoSig];
                padre[nodoSig] = nodoActual;
            }
        }
        visto[nodoActual] = 1;
        if (tamanoCola != 0) {
            nodoActual = cola[0];
            eliminar_primer_elem(cola, tamanoCola);
            tamanoCola--;
        } else break;
    }
    int graphPos = 0;
    int count = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 14; j++) {
            if (count != 35 && count != 50) {
                printf("%02.0f ", distancia[graphPos]);
                graphPos++;
            } else {
                printf("   ");
            }
            count++;
        }
        printf("\n");
    }
}