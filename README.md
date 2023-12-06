# tarea-MD
Universidad de Concepcion, "Tarea Computacional" Matematicas Discretas, 2023-2
# Indicación
Para encontrar la ruta más corta, se debe:
1. compilar desde el archivo del proyecto con gcc src/main.c -Iinclude -o nombrearchivo
2. ejecutar mediante ./nombrearchivo "(NombreCalle) (NumeroInmueble)" "(NombreCalle) (NumeroInmueble)", opcionalmente añadiendo un 3er parámetro

# Ejemplo de uso:
Input:
```console
$ gcc src/main.c -Wfatal-errors -Wall -Iinclude -o rutas
$ ./rutas "Chacabuco 611" "Freire 1001" "San Martin 1300"
```
Output:
```console
input: Chacabuco 611, aproximado a: Chacabuco 600 (Nodo: 104)
input: Freire 1001, aproximado a: Freire 1000 (Nodo: 38)
input: San Martin 1300, aproximado a: San Martin 1299 (Nodo: 83)
Ruta: vertice 104 hasta vertice 38, pasando por 83

Camino mas corto desde Chacabuco 611 a Freire 1001 pasando por San Martin 1300: 
Chacabuco-Caupolican -> Cochrane-Caupolican -> San Martin-Caupolican -> San Martin-Anibal Pinto -> San Martin-Colo colo -> San Martin-Castellon -> San Martin-Tucapel -> San Martin-Orompello -> San Martin-Ongolmo -> San Martin-Paicavi -> Cochrane-Paicavi -> Chacabuco-Paicavi -> Cochrane-Ongolmo -> San Martin-Orompello -> O'Higgins-Tucapel -> Barros Arana-Tucapel -> Freire-Tucapel
Distancia recorrida: 1723 metros
```
# Grafo de referencia
![Model](https://github.com/square0108/tarea-MD/blob/main/GrafoConcepcion.png)

