#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Constantes
#define N 32     // Tamaño de la red (N x N)
#define ITERACIONES 10000*32*32// Número de iteraciones
#define T  1 // Temperatura
#define K_BOLTZMANN 1.0 // Constante de Boltzmann (J/K)


// Función para inicializar la red con espines aleatorios o magnetización neta nula
void inicializarRed(int red[N][N], int opcion) {
    if (opcion == 1) {
        // Magnetización aleatoria
        for (int j = 0; j < N; j++) {
            for (int i = 0; i < N; i++) {
                if (i == 0) {
                    // Fijar los espines en i=0 a -1
                    red[i][j] = -1;
                } else if (i == N - 1) {
                    // Fijar los espines en i=N-1 a +1
                    red[i][j] = 1;
                } else {
                    // Espines aleatorios en el resto de la red
                    red[i][j] = (rand() % 2) * 2 - 1; // Genera +1 o -1
                }
            }
        }
    } else if (opcion == 2) {
        // Magnetización neta nula
        int totalEspines = (N - 2) * N; // Total de espines no fijos
        int mitad = totalEspines / 2;  // Mitad de espines para +1 y -1
        int contadorPositivos = 0, contadorNegativos = 0;

        for (int j = 0; j < N; j++) {
            for (int i = 0; i < N; i++) {
                if (i == 0) {
                    // Fijar los espines en i=0 a -1
                    red[i][j] = -1;
                } else if (i == N - 1) {
                    // Fijar los espines en i=N-1 a +1
                    red[i][j] = 1;
                } else {
                    // Asignar espines de forma que la suma sea cero
                    if (contadorPositivos < mitad) {
                        red[i][j] = 1;
                        contadorPositivos++;
                    } else {
                        red[i][j] = -1;
                        contadorNegativos++;
                    }
                }
            }
        }

        // Mezclar los espines para desordenarlos
        for (int j = 0; j < N; j++) {
            for (int i = 1; i < N - 1; i++) { // Solo mezclar los espines no fijos
                int x = rand() % (N - 2) + 1; // Índice aleatorio en el rango [1, N-2]
                int y = rand() % N;

                // Intercambiar espines
                int temp = red[i][j];
                red[i][j] = red[x][y];
                red[x][y] = temp;
            }
        }
    }
}


// Función para imprimir la red
void imprimirRed(int red[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%2d ", red[i][j]);
        }
        printf("\n");
    }
}

void guardarRed(FILE *archivo, int red[N][N]) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            fprintf(archivo, "%d", red[i][j]);
            if (j < N - 1) {
                fprintf(archivo, ","); // Separador entre columnas
            }
        }
        fprintf(archivo, "\n"); // Nueva línea al final de cada fila
    }
    fprintf(archivo, "\n"); // Línea en blanco para separar iteraciones
}


int calcularEnergia(int red[N][N], int x1, int y1, int x2, int y2) {
    int E;
    E=0;

    // Vecinos del primer espín (x1, y1)
    int arriba1, abajo1, izquierda1, derecha1;

    // Vecino de arriba
    if (x1 == 1) {
        arriba1 = -1; // Bordes fijos en "x=0"
    } else {
        arriba1 = red[x1-1][y1];
    }

    // Vecino de abajo
    if (x1 == N - 2) {
        abajo1 = 1; // Bordes fijos en "x=N-1"
    } else {
        abajo1 = red[x1+1][y1];
    }

    // Vecino de la izquierda (condiciones periódicas en "y")
    if (y1 == 0) {
        izquierda1 = red[x1][N-1];
    } else {
        izquierda1 = red[x1][y1-1];
    }

    // Vecino de la derecha (condiciones periódicas en "y")
    if (y1 == N - 1) {
        derecha1 = red[x1][0];
    } else {
        derecha1 = red[x1][y1+1];
    }

    // Vecinos del segundo espín (x2, y2)
    int arriba2, abajo2, izquierda2, derecha2;

    // Vecino de arriba
    if (x2== 1) {
        arriba2 = -1; // Bordes fijos en "x=0"
    } else {
        arriba2 = red[x2-1][y2];
    }

    // Vecino de abajo
    if (x2 == N - 2) {
        abajo2 = 1; // Bordes fijos en "x=N-1"
    } else {
        abajo2 = red[x2+1][y2];
    }

    // Vecino de la izquierda (condiciones periódicas en "y")
    if (y2 == 0) {
        izquierda2 = red[x2][N-1];
    } else {
        izquierda2 = red[x2][y2-1];
    }

    // Vecino de la derecha (condiciones periódicas en "y")
    if (y2 == N - 1) {
        derecha2 = red[x2][0];
    } else {
        derecha2 = red[x2][y2+1];
    }

    //No es necesario calcular las interacciones entre los espines que se van a intercamciar, ya que se cancelan entre sí en la diferencia de energía
    // Anular las contribuciones de (x2, y2) en la energía de (x1, y1)

    
    //Si y1=N-1, y1+1=N y al aplicar el modulo N, se obtiene 0
    //Si y1=0, y1-1+N=N-1 y al aplicar el modulo N, se obtiene N-1

    if ((x1 == x2 && (y1 + 1) % N == y2)) derecha1, derecha2 = 0; // (x2, y2) es el vecino a la derecha de (x1, y1)
    if ((x1 == x2 && (y1 - 1 + N) % N == y2)) izquierda1, izquierda2 = 0; // (x2, y2) es el vecino a la izquierda de (x1, y1)
    if ((x1 + 1  == x2 && y1 == y2)) abajo1, abajo2 = 0; // (x2, y2) es el vecino abajo de (x1, y1)
    if ((x1-1 == x2 && y1 == y2)) arriba1, arriba2 = 0;// (x2, y2) es el vecino arriba de (x1, y1)
    

    // Energía del primer espín
    E = -red[x1][y1] * (arriba1 + abajo1 + izquierda1 + derecha1);
    // Energía del segundo espín
    E += -red[x2][y2] * (arriba2 + abajo2 + izquierda2 + derecha2);

  //Al final solo realizamos 6 evaluaciones antes del intercambio y 6 después del intercambio. 

    return E;
}

void calcularMagnetizacionMitades(int red[N][N], double *magnetizacionSuperior, double *magnetizacionInferior) {
    double sumaSuperior = 0.0;
    double sumaInferior = 0.0;
    // Intervalo para calcular la magnetización
    int intervalo = 100 * N * N;

    // Calcular la suma de espines para la mitad superior
    for (int i = 0; i < N / 2; i++) {
        for (int j = 0; j < N; j++) {
            sumaSuperior += red[i][j];
        }
    }

    // Calcular la suma de espines para la mitad inferior
    for (int i = N / 2; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sumaInferior += red[i][j];
        }
    }

    // Normalizar por el número de espines en cada mitad y acumular
    *magnetizacionSuperior += sumaSuperior / (N * N / 2);
    *magnetizacionInferior += sumaInferior / (N * N / 2);
}

// Algoritmo de Kawasaki con dinámica de Metrópolis
void kawasakiIsing(int red[N][N], double beta, int iteraciones) {

    FILE *archivo = fopen("matriz_red.txt", "w");
    if (archivo == NULL) {
        fprintf(stderr, "Error al abrir el archivo para guardar la red.\n");
        exit(1);
    }

    double magnetizacionSuperior = 0.0;
    double magnetizacionInferior = 0.0;
    int conteoIntervalos = 0; // Contador de intervalos para el promedio

    // Intervalo para calcular la magnetización
    int intervalo = 100 * N * N;

    for (int iter = 0; iter < iteraciones; iter++) {
        // Elegir un espín al azar
        int y1 = rand() % N;
        int x1 = rand() % (N-2) +1 ; // Elegir un espín en el rango [1, N-2] para evitar los bordes fijos
        
        // Calcular un vecino al azar (arriba, abajo, derecha o izquierda)
        int deltaX = 0, deltaY = 0, dirección=0; 

        if (x1 == 1) {
            // Descartar el intercambio en el borde fijo x=1
            int direccion = rand() % 3; // Generar un número aleatorio entre 0 y 1 para elegir la dirección
            if (direccion == 0) {
                deltaY = 1;  // Vecino a la derecha
            } else if (direccion == 1) {
                deltaY = -1; // Vecino a la izquierda
            } else {
                deltaX = 1;  // Vecino abajo
            }

        }else if (x1 == N - 2) {
           // Descartar el intercambio en el borde fijo y=N-1
            int direccion = rand() % 3; // Generar un número aleatorio entre 0 y 1 para elegir la dirección
            if (direccion == 0) {
                deltaY= -1; // Vecino a la izquierda
            } else if (direccion == 1) {
                deltaY = 1;  // Vecino a la derecha
            } else {
                deltaX = -1; // Vecino arriba
            }
        } else {
            int direccion = rand() % 4; // Generar un número aleatorio entre 0 y 3 para elegir la dirección
            if (direccion == 0) {
                deltaY= 1;  // Vecino a la derecha
            } else if (direccion == 1) {
                deltaY = -1; // Vecino a la izquierda
            } else if (direccion == 2) {
                deltaX = 1;  // Vecino abajo
            } else if (direccion == 3) {
                deltaX = -1; // Vecino arriba
            }
        }

        // Calcular las coordenadas del vecino
        int x2 = x1 + deltaX;
        int y2 = y1 + deltaY;

        // Aplicar condiciones de contorno periódicas en "y"
        if (y2 < 0) {
            y2 = N - 1; // Si se sale por la izquierda, vuelve al borde derecho
        } else if (y2 >= N) {
            y2 = 0; // Si se sale por la derecha, vuelve al borde izquierdo
        }


         // Asegurar que los espines tengan distinto signo para intercambiarlos
         if (red[x1][y1] == red[x2][y2]) {
             continue; // Si tienen el mismo signo, pasar a la siguiente iteración
        }

        // Calcular la energía antes del intercambio
        int energiaAntes = calcularEnergia(red, x1, y1, x2, y2);

        // Intercambiar los espines
        int temp = red[x1][y1];
        red[x1][y1] = red[x2][y2];
        red[x2][y2] = temp;
       
    
        // Calcular la energía después del intercambio
        int energiaDespues = calcularEnergia(red, x1, y1, x2, y2);

        // Calcular la diferencia de energía
        int difE = energiaDespues - energiaAntes;

        // Generar un número aleatorio entre 0 y 1
        double r = (double)rand() / RAND_MAX;

        double probabilidad; 
            if (difE <= 0) {
                probabilidad = 1.0; 
            } else {
                probabilidad = exp(-beta * difE); // Calcular la probabilidad de aceptación
            }

        if (probabilidad > r) {
            // El intercambio se acepta, guardar la matriz en el archivo
            guardarRed(archivo,red);
        
        } else {
            // Revertir el intercambio si no se acepta
            temp = red[x1][y1];
            red[x1][y1] = red[x2][y2];
            red[x2][y2] = temp;
        }

         // Calcular y acumular la magnetización cada 100 * N^2 iteraciones
         if ((iter + 1) % intervalo == 0) {
            calcularMagnetizacionMitades(red, &magnetizacionSuperior, &magnetizacionInferior);
            conteoIntervalos++; // Incrementar el contador de intervalos
        }

    }

    fclose(archivo); // Cerrar el archivo después de todas las iteraciones
    if (conteoIntervalos > 0) {
        magnetizacionSuperior /= conteoIntervalos;
        magnetizacionInferior /= conteoIntervalos;
    } else {
        printf("No se calcularon intervalos de magnetización.\n");
        magnetizacionSuperior = 0.0;
        magnetizacionInferior = 0.0;
    }

    printf("Magnetización superior: %f\n", magnetizacionSuperior);
    printf("Magnetización inferior: %f\n", magnetizacionInferior);

}

void calcularDensidadColumna(int red[N][N]) {
    int columna = N / 2; // Columna y = N / 2
    int espinesPositivos = 0;
    int espinesNegativos = 0;

    // Recorrer la columna y contar los espines positivos y negativos
    for (int i = 0; i < N; i++) {
        if (red[i][columna] == 1) {
            espinesPositivos++;
        } else if (red[i][columna] == -1) {
            espinesNegativos++;
        }
    }

    // Calcular las densidades
    double densidadPositivos = (double)espinesPositivos / N;
    double densidadNegativos = (double)espinesNegativos / N;

    // Imprimir los resultados
    printf("\nDensidad de espines positivos en la columna y = %d: %.6f\n", columna, densidadPositivos);
    printf("Densidad de espines negativos en la columna y = %d: %.6f\n", columna, densidadNegativos);
}



int main() {
    srand(time(NULL)); // Inicializar la semilla para números aleatorios

    int red[N][N];
    double beta = 1.0 / (K_BOLTZMANN * T);

     // Preguntar al usuario por el tipo de magnetización
     int opcion;
     printf("Seleccione el tipo de magnetización:\n");
     printf("1. Magnetización aleatoria\n");
     printf("2. Magnetización neta nula\n");
     printf("Opción: ");
     scanf("%d", &opcion);
 
     // Inicializar la red según la opción seleccionada
     inicializarRed(red, opcion);

    // Imprimir la configuración inicial
    printf("Configuración inicial de la red:\n");
    imprimirRed(red);

    // Ejecutar el algoritmo de Kawasaki
    kawasakiIsing(red, beta, ITERACIONES);



    // Imprimir la configuración final
    printf("\nConfiguración final de la red:\n");
    imprimirRed(red);

    // Calcular la densidad promedio de espines positivos y negativos en la columna y = N / 2
    calcularDensidadColumna(red);


    return 0;
}