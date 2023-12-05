#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/* **************************** *
 * Constantes
******************************* */

// Numero total de vertices en el grafo del Centro Concepcion
#define MAX_VERTICES 112
#define GRADO_MAXIMO 6

// Distancia infinita
#define INF 999

// Nodos de la diagonal
int nodos_diagonal[] = {66,81,96,111};

// Matriz de adyacencia
float mat_ady[MAX_VERTICES][MAX_VERTICES];

// Enums de las calles
enum calles_horizontales {
    LOS_CARRERA,MAIPU,FREIRE,BARROS_ARANA,OHIGGINS,SAN_MARTIN,COCHRANE,CHACABUCO
};

enum calles_verticales {
    ARTURO_PRAT,SERRANO,SALAS,ANGOL,LINCOYAN,RENGO,CAUPOLICAN,ANIBAL_PINTO,COLOCOLO,CASTELLON,TUCAPEL,OROMPELLO,ONGOLMO,PAICAVI
};

enum tipo_calle {
    HORIZONTAL, VERTICAL, DIAGONAL
};

// Arreglos con los nombres de las calles (para asignar strings a los nodos)
char* strings_CallesH[] = {"Los Carrera","Maipu","Freire","Barros Arana","O'Higgins","San Martin","Cochrane","Chacabuco"};
char* strings_CallesV[] = {"Arturo Prat", "Serrano", "Salas", "Angol", "Lincoyan", "Rengo", "Caupolican", "Anibal Pinto", "Colo colo", "Castellon", "Tucapel", "Orompello", "Ongolmo", "Paicavi"};

int inmuebles_hori[] = {0,100,200,300,400,500,600,700,800,900,1000,1100,1200,1300};
int inmuebles_vert[] = {700,600,500,400,300,200,100,0};
int inmuebles_diag[] = {400,300,200,100};

/* **************************** *
 * Structs y enums
******************************* */

typedef struct nodo
{
    int indice;
    char* calles_pertenecientes[3];
    struct nodo* link; // probablemente saque esto, no es utilizado
}Nodo;

/* **************************** *
 * Prototipos
******************************* */

void crear_arco(float mat_ady[MAX_VERTICES][MAX_VERTICES], Nodo* inicio, Nodo* final, float peso);
void borrar_arco(float mat_ady[MAX_VERTICES][MAX_VERTICES], Nodo* inicio, Nodo* final);
void print_arcos(float mat_ady[MAX_VERTICES][MAX_VERTICES], int id);
void print_arcos(float mat_ady[MAX_VERTICES][MAX_VERTICES], int id);
void set_calle_vert_perteneciente(Nodo* node_ar[MAX_VERTICES], int calle, int id_vertice, int id_StrArrayStructMem);
void set_calle_hori_perteneciente(Nodo* node_ar[MAX_VERTICES], int calle, int id_vertice, int id_StrArrayStructMem);
void numrange(int* array, int start, int end);
int* elems_plus_one(int arr[8]);

void eliminar_primer_elem(int* arr, int tamano);
int* dijkstra(int inicio, int final);
int* dijkstraCondicionado(int inicio, int final, int pasandoPor);
void reverse(int* arr, int n);
void dijkstra_print(int desde, int hasta, int pasandoPor);

void copiar_nombre_ingresado(char destino[32], char* original);
int extraer_numero_inmueble(char* str);
int aproximar_inmueble(int num_inmueble, int* array_inmuebles, int tipo_calle);
int inmueble_a_nodo(int inmueble, int* array_calle, int tipo_calle);

/* **************************** *
 * Funciones
******************************* */

int main(int argc, char *argv[]) {
    Nodo* nodos[MAX_VERTICES];

    // inicializacion de todas las distancias a infinito (inicialmente ningun nodo conectado)
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            mat_ady[i][j] = INF;
        }
    }

    // calles "horizontales", todas son largo 14
    int Carrera[14];
    int Maipu[14];
    int Freire[14];
    int BarrosArana[14];
    int OHiggins[14];
    int SanMartin[14];
    int Cochrane[14];
    int Chacabuco[14];

    int* CallesHorizontales[] = {Carrera, Maipu, Freire, BarrosArana, OHiggins, SanMartin, Cochrane, Chacabuco};
    int start_index[] = {0,14,28,42,56,70,84,98};
    for (int i = 0; i < 8; i++) {
        // esta funcion solo inicializa los valores de cada arreglo a numeros en un rango, incluyendo inicio y final del rango
        numrange(CallesHorizontales[i], start_index[i], start_index[i]+13);
    }

    // calles "verticales", todas son largo 8
    int Prat[] = {0,14,28,42,56,70,84,98};
    int Serrano[8];
    int Salas[8]; 
    int Angol[8]; 
    int Lincoyan[8];
    int Rengo[8];
    int Caupolican[8];
    int AnibalPinto[8];
    int ColoColo[8];
    int Castellon[8];
    int Tucapel[8];
    int Orompello[8];
    int Ongolmo[8];
    int Paicavi[8];

    int* CallesVerticales[] = {Prat, Serrano, Salas, Angol, Lincoyan, Rengo, Caupolican, AnibalPinto, ColoColo, Castellon, Tucapel, Orompello, Ongolmo, Paicavi};
    for (int i = 1; i < 14; i++) {
        // los índices de cada calle vertical son los mismos índices de la calle vertical anterior a esta, pero +1
        CallesVerticales[i] = elems_plus_one(CallesVerticales[i-1]);
    }

    // inicializacion de los nodos, los dejé como struct en caso de tener que usarlos asi en algun momento?
    for (int i = 0; i < MAX_VERTICES; i++) {
        nodos[i] = malloc(sizeof(Nodo));
        nodos[i]->calles_pertenecientes[0] = malloc(sizeof(char)*32);
        nodos[i]->calles_pertenecientes[1] = malloc(sizeof(char)*32);
        nodos[i]->calles_pertenecientes[2] = malloc(sizeof(char)*32);
        (nodos[i])->indice = i;
        nodos[i]->link = NULL;

        for (int j = 0; j < MAX_VERTICES; j++) {
            mat_ady[i][j] = INF;
        }
    }


    /* Se conectan los vertices calle por calle mediante un "patrón" donde se crean arcos todos en la misma dirección entre cada par de vertices adyacentes de una calle, luego en direccion opuesta en la calle siguiente, etc...

    1. Las calles del arreglo CallesHorizontales[] con índice PAR en este arreglo, van de derecha a izquierda, las IMPARES de izquierda a derecha. (salvo por las avenidas en cada extremo)
    2. Las calles del arreglo CallesVerticales[] con índice PAR en este arreglo, van de abajo a arriba, las IMPARES de arriba a abajo. (salvo por Paicaví)
    3. Los casos que se salgan del patrón son manejados en un switch dentro de cada for-loop.
    
    */

    // primero, todas las CALLES HORIZONTALES
    for (int id_calle = 0; id_calle < 8; id_calle++) {
        int* calle_actual = CallesHorizontales[id_calle]; // para legibilidad

        for (int posicion_nodo = 0; posicion_nodo < 13; posicion_nodo++) {
            int numero_nodo = calle_actual[posicion_nodo]; // para legibilidad

            // Funcion que unicamente asigna una string con el nombre de la calle al nodo actual.
            set_calle_hori_perteneciente(nodos, id_calle, numero_nodo, 0);
            // Este if es para no saltarse la asignacion de string en la ultima calle del array de calles horizontales
            if (posicion_nodo == 12) {
                set_calle_hori_perteneciente(nodos, id_calle, calle_actual[posicion_nodo+1], 0);
            }
            // Switch utilizado para manejar casos excepcion
            switch (id_calle) {
                case LOS_CARRERA:
                    // esta calle tiene bidireccion, por lo tanto:

                    // arco derecha-izquierda
                    crear_arco(
                        mat_ady,
                        nodos[calle_actual[posicion_nodo+1]],
                        nodos[calle_actual[posicion_nodo]],
                        1);
                    // archo izquierda-derecha
                    crear_arco(
                        mat_ady,
                        nodos[calle_actual[posicion_nodo]],
                        nodos[calle_actual[posicion_nodo+1]],
                        1);
                    continue;
                case BARROS_ARANA:
                    switch (numero_nodo) {
                        case 49:
                            continue;
                        case 50:
                            continue;
                        default:
                            break;
                    }
                    break;
                case COCHRANE:
                    // no estoy seguro del sentido de esta calle en 96-97?
                    if (numero_nodo == 96) {
                        crear_arco(mat_ady, nodos[96], nodos[97], 1);
                        continue;
                    }
                    break;
                case CHACABUCO:
                    // esta calle tiene bidireccion, por lo tanto:

                    // arco derecha-izquierda
                    crear_arco(
                        mat_ady,
                        nodos[calle_actual[posicion_nodo+1]],
                        nodos[calle_actual[posicion_nodo]],
                        1);
                    // archo izquierda-derecha
                    crear_arco(
                        mat_ady,
                        nodos[calle_actual[posicion_nodo]],
                        nodos[calle_actual[posicion_nodo+1]],
                        1);
                    // continue ya que no hay calles por debajo de esta para conectar verticalmente
                    continue;
                default:
                    break;
            }

            if (id_calle%2 == 0) {
                // arco derecha-izquierda
                crear_arco(
                    mat_ady,
                    nodos[calle_actual[posicion_nodo+1]],
                    nodos[calle_actual[posicion_nodo]],
                    1);
            }
            else {
                // arco izquierda-derecha
                crear_arco(
                    mat_ady,
                    nodos[calle_actual[posicion_nodo]],
                    nodos[calle_actual[posicion_nodo+1]],
                    1);
            }
        }
    }
    
    // luego, todas las CALLES VERTICALES
    for (int id_calle = 0; id_calle < 14; id_calle++) {
        int* calle_actual = CallesVerticales[id_calle]; // para legibilidad

        for (int posicion_nodo = 0; posicion_nodo < 7; posicion_nodo++) {
            int numero_nodo = calle_actual[posicion_nodo]; // para legibilidad
            
            // Funcion que unicamente asigna una string con el nombre de la calle al nodo actual.
            set_calle_vert_perteneciente(nodos, id_calle, numero_nodo, 1);
            // Este if es para no saltarse la asignacion de string en la ultima calle del array de calles verticales
            if (posicion_nodo == 6) {
                set_calle_vert_perteneciente(nodos, id_calle, calle_actual[posicion_nodo+1], 1);
            }

            switch (id_calle) {
                case ANIBAL_PINTO:
                    if (numero_nodo == 21 || numero_nodo == 35) {
                        continue;
                    }
                    break;
                case PAICAVI:
                    if (numero_nodo >= 13 && numero_nodo <= 55) {
                        crear_arco(mat_ady,nodos[calle_actual[posicion_nodo+1]],nodos[calle_actual[posicion_nodo]],1);
                        crear_arco(mat_ady,nodos[calle_actual[posicion_nodo]],nodos[calle_actual[posicion_nodo+1]],1);
                        continue;
                    }
                    break;
                default:
                    break;
            }

            if (id_calle%2 == 0) {
                // arco de abajo a arriba
                crear_arco(
                    mat_ady,
                    nodos[calle_actual[posicion_nodo+1]],
                    nodos[calle_actual[posicion_nodo]],
                    1);
            }
            else {
                // arco de arriba a abajo
                crear_arco(
                    mat_ady,
                    nodos[calle_actual[posicion_nodo]],
                    nodos[calle_actual[posicion_nodo+1]],
                    1);
            }
        }

        calle_actual = NULL;
    }
    // Por ultimo, se agrega la diagonal
    crear_arco(mat_ady, nodos[81], nodos[66], 1.41F);
    strcpy(nodos[66]->calles_pertenecientes[2], "Pedro Aguirre Cerda");
    for (int i = 1; i < 3; i++) {
        crear_arco(mat_ady, nodos[nodos_diagonal[i]], nodos[nodos_diagonal[i+1]], 2);
        crear_arco(mat_ady, nodos[nodos_diagonal[i+1]], nodos[nodos_diagonal[i]], 2);
        strcpy(nodos[i]->calles_pertenecientes[2], "Pedro Aguirre Cerda");
        strcpy(nodos[i+1]->calles_pertenecientes[2], "Pedro Aguirre Cerda");
    }

    /* Aqui termina la inicializacion de la matriz */

    /* moví esto a la función print_dijkstra() que ocupo más abajo -Martín
    int desde = 1;
    int hasta = 2;
    int pasandoPor = 104;

   printf("Camino mas corto desde %d a %d: ", desde, hasta);  
    int* recorridoMasCorto = dijkstra(desde, hasta);
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (recorridoMasCorto[i] != -1){
            printf("%d ", recorridoMasCorto[i]);
        } else break;
    } 
    printf("\nCamino mas corto desde %d a %d pasando por %d: ", desde, hasta, pasandoPor);  
       int* recorridoMasCorto2 = dijkstraCondicionado(desde, hasta, pasandoPor);
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (recorridoMasCorto2[i] != -1){
            printf("%d ", recorridoMasCorto2[i]);
        } else break;
    }
    printf("\n");
    */

    /* Intentando validar inputs */
    /*
    int argc es el numero de argumentos, contando el comando para ejecutar el codigo
    char* argv[] son las strings argumentos
    */

    // Inmuebles CallesHorizontales: 0-1300, izquierda-derecha 
    // Inmuebles CallesVerticales: 0-799 (0-800?), abajo-arriba. Al menos según el correo del profe Pierluigi.
    // Inmuebles Diagonal: 100-300 Chacabuco-O'Higgins, tambien según un correo

    int verts_ruta[] = {-1,-1,-1};

    if (argc == 3 || argc == 4) {
        for (int N = 1; N < argc; N++) {
            int var = 0;
            int* calle_a_iterar = NULL;
            int tipo_calle;
            char nombre_calle_ingresado[32];
            char string_num_inmueble[8];

            // Extraer toda la string antes del número de calle, o sea sólo el nombre de calle
            copiar_nombre_ingresado(nombre_calle_ingresado, argv[N]);

            // Comparar la string extraida con los nombres de las calles Horizontales (Es case-sensitive)
            for (int i = 0; i < 8; i++) {
                if (calle_a_iterar != NULL) break;
                if (strcmp(nombre_calle_ingresado, strings_CallesH[i]) == 0) {
                    tipo_calle = HORIZONTAL;
                    calle_a_iterar = CallesHorizontales[i];
                }
            }
            // Comparar la string extraida con los nombres de las calles Verticales (Es case-sensitive)
            for (int i = 0; i < 14; i++) {
                if (calle_a_iterar != NULL) break;
                if (strcmp(nombre_calle_ingresado, strings_CallesV[i]) == 0) {
                    tipo_calle = VERTICAL;
                    calle_a_iterar = CallesVerticales[i];
                }
            }
            // Terminar el programa si el nombre ingresado no coincide con ninguna calle
            if (calle_a_iterar == NULL) {
                printf("Alguna de las calles está mal escrita\n");
                return -1;
            }

            // Extraer el numero de inmueble ingresado
            int inmueble_ingresado = extraer_numero_inmueble(argv[N]);
            inmueble_ingresado = aproximar_inmueble(inmueble_ingresado, inmuebles_hori, tipo_calle);
            
            int indice_vertice_mapeado = -1;
            switch(tipo_calle) {
                case HORIZONTAL:
                    for (int i = 0; i < 14; i++) {
                        if (inmueble_ingresado == inmuebles_hori[i]) {
                            indice_vertice_mapeado = i;
                            verts_ruta[N-1] = calle_a_iterar[i];
                            break;
                        }
                    }
                    break;
                case VERTICAL:
                    for (int i = 0; i < 8; i++) {
                        if (inmueble_ingresado == inmuebles_vert[i]) {
                            indice_vertice_mapeado = i;
                            verts_ruta[N-1] = calle_a_iterar[i];
                            break;
                        }
                    }
                    break;
                default:
                    printf("Ha ocurrido un error al traducir el inmueble a nodo\n");
                    return -1;
            }
            
            printf("Input %i: %s\n", N, argv[N]);
            printf("Inmueble aproximado: %i\n", inmueble_ingresado);
            printf("Primer indice de la calle: %i\n", calle_a_iterar[0]);
            printf("Vertice de ruta: %i\n", verts_ruta[N-1]);
            
        }

        dijkstra_print(verts_ruta[0], verts_ruta[1], verts_ruta[2]);
    }
    else {
        printf("Numero incorrecto de argumentos.");
        exit;
    }
  
}

// devuelve un arreglo con secuencia de numeros start-end, incluyendo el inicio y final dentro del arreglo
void numrange(int* array, int start, int end) {
    int value = start;
    for (int i = 0; i <= end-start; i++) {
        array[i] = value;
        value++;
    }
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

void set_calle_hori_perteneciente(Nodo* node_ar[MAX_VERTICES], int calle, int id_vertice, int id_StrArrayStructMem) {
    strcpy(node_ar[id_vertice]->calles_pertenecientes[id_StrArrayStructMem], strings_CallesH[calle]);
}

void set_calle_vert_perteneciente(Nodo* node_ar[MAX_VERTICES], int calle, int id_vertice, int id_StrArrayStructMem) {
    strcpy(node_ar[id_vertice]->calles_pertenecientes[id_StrArrayStructMem], strings_CallesV[calle]);
}

void crear_arco(float mat_ady[MAX_VERTICES][MAX_VERTICES], Nodo* inicio, Nodo* final, float peso) {
    mat_ady[inicio->indice][final->indice] = peso;
    inicio->link = final; // placeholder

}

void borrar_arco(float mat_ady[MAX_VERTICES][MAX_VERTICES], Nodo* inicio, Nodo* final) {
    mat_ady[inicio->indice][final->indice] = INF;
    inicio->link = NULL; // placeholder
}

void print_peso_arco(float mat_ady[MAX_VERTICES][MAX_VERTICES], int inicio, int final) {
    printf("%f", mat_ady[inicio][final]);
}

// para debugging, printea los pares ordenados/arcos que contienen a este vertice
// es decir, aquellos arcos que empiezen o terminen en el vertice ingresado
void print_arcos(float mat_ady[MAX_VERTICES][MAX_VERTICES], int id) {
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (mat_ady[id][i] != INF) printf("(%i --(%f)--> %i),", id, mat_ady[id][i], i);
        if (mat_ady[i][id] != INF) printf("(%i --(%f)--> %i),", i, mat_ady[i][id], id);
    }
}

void eliminar_primer_elem(int* arr, int tamano) {
    for (int i = 0; i < tamano-1; i++) arr[i] = arr[i+1];
    arr[tamano-1] = -1;
}

void reverse(int* arr, int n) {
    int aux[n];
    for (int i = 0; i < n; i++) aux[n-1-i] = arr[i];
    for (int i = 0; i < n; i++) arr[i] = aux[i];
}
// por ahora no devuelve nada, pero distancia es un array de MAX_VERTICES elementos con cada indice siendo la distancia
// desde inicio hacia ese nodo

// esta funcion lo que tiene que hacer es recibir indice de inicio e indice final y devolver un array de los nodos por los que pasa
// para esto se me ocurria usar el array padre que basicamente indicaba (por ejemplo, padre[i]) "desde que nodo vino el algoritmo a i para
// recorrer la menor distancia"

// puedes hacerlo como tu quieras, pero eso es la idea que yo tenia por si te sirve
int* dijkstra(int inicio, int final) {
    int* recorrido = malloc(sizeof(int) * MAX_VERTICES);
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
    /*
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
    */
    recorrido[0] = -1;
    recorrido[1] = final;
    int padreAnterior = padre[final];
    int pasos = 1;
    for (int i = 2; i < MAX_VERTICES; i++) {
        if (padreAnterior != -1){
            recorrido[i] = padreAnterior;
            padreAnterior = padre[padreAnterior];
            pasos++;
        } else{
            pasos++;
            break;
        } 
    }
    reverse(recorrido,pasos);

    return recorrido;
}

int* dijkstraCondicionado(int inicio, int final, int pasandoPor){
    int* recorrido = malloc(sizeof(int) * MAX_VERTICES);
    int* tramo1 = dijkstra(inicio, pasandoPor);
    int* tramo2 = dijkstra(pasandoPor, final);
    int pasos = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (tramo1[i] != -1) {
            recorrido[pasos] = tramo1[i];
            pasos++;
        } else break;
    }
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (tramo2[i] != -1) {
            recorrido[pasos-1] = tramo2[i];
            pasos++;
        } else{
            recorrido[pasos-1] = -1;
            break;
        }
            
    }
    return recorrido;
}

void copiar_nombre_ingresado(char destino[32], char* original) {
    int var = 0;
    while (original[var] != '\0') {
        if (isdigit(original[var+1]) && original[var] == ' ') {
            destino[var] = '\0';
            break;
        }
        destino[var] = original[var];
        var++;
    }
}

int extraer_numero_inmueble(char* str) {
    int var_a = 0;
    int var_b = 0;
    char str_num_inmueble[16];
    while (str[var_b] != '\0') {
        if (isdigit(str[var_b]) != 0) {
            str_num_inmueble[var_a] = str[var_b];
            var_a++;
        }
        if (isdigit(str[var_b] != 0) && (isdigit(str[var_b+1] == 0))){
            str_num_inmueble[var_a] = '\0';
            break;
        }
        var_b++;

        if (var_b > 128) {
            printf("oepsie woepsie");
            return -1;
        }
    }
    return atoi(str_num_inmueble);
}

int aproximar_inmueble(int num_inmueble, int* array_inmuebles, int tipo_calle) {
    int min_dist = (int)INFINITY;
    int aproximacion = -1;

    switch(tipo_calle) {
        case HORIZONTAL:
            for (int i = 0; i < 14; i++) {
                if (abs(num_inmueble-(i*100)) < min_dist) {
                    min_dist = abs(num_inmueble-(i*100));
                    aproximacion = i*100;
                }
            }
            break;
        case VERTICAL:
            for (int i = 0; i < 8; i++) {
                if (abs(num_inmueble-(i*100)) < min_dist) {
                    min_dist = abs(num_inmueble-(i*100));
                    aproximacion = i*100;
                }
            }
            break;
        case DIAGONAL:
            // TODO
            break;
        default:
            printf("Algo raro ocurrió al aproximar el inmueble\n");
            return -1;
    }

    if (aproximacion == -1 || min_dist == -999) {
        printf("Algo raro ocurrió al aproximar el inmueble\n");
    }
    return aproximacion;
}

// crédito a Bastián por esta función
void dijkstra_print(int desde, int hasta, int pasandoPor) {
    if (pasandoPor == -1) {
        printf("Camino mas corto desde %d a %d: ", desde, hasta);  
        int* recorridoMasCorto = dijkstra(desde, hasta);
        for (int i = 0; i < MAX_VERTICES; i++) {
            if (recorridoMasCorto[i] != -1){
                printf("%d ", recorridoMasCorto[i]);
            } else break;
        }
    }
    else {
        printf("\nCamino mas corto desde %d a %d pasando por %d: ", desde, hasta, pasandoPor);  
        int* recorridoMasCorto2 = dijkstraCondicionado(desde, hasta, pasandoPor);
        for (int i = 0; i < MAX_VERTICES; i++) {
            if (recorridoMasCorto2[i] != -1){
                printf("%d ", recorridoMasCorto2[i]);
            } else break;
        }
    }
    printf("\n");
}
