#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "criptomoneda.h"

/*
typedef struct {
    double      precio;
    char        nombre[50];
    char        simbolo[10];
    double      capacidad_mercado;
    double      volumen_24h;
    double      cambio_24h;
} tCriptomoneda;
*/

int leer_criptomonedas_csv(const char *nombre_archivo, tCriptomoneda *criptomonedas, int *num_criptomonedas) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        perror("Error al abrir el archivo");
        return -1;
    }

    char linea[256];
    *num_criptomonedas = 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        // Eliminar el salto de línea al final de la línea
        linea[strcspn(linea, "\n")] = 0; // Eliminar el salto de línea

        // Parsear la línea
        sscanf(linea, "%49[^,],%9[^,],%lf,%lf,%lf,%lf",
               criptomonedas[*num_criptomonedas].nombre,
               criptomonedas[*num_criptomonedas].simbolo,
               &criptomonedas[*num_criptomonedas].precio,
               &criptomonedas[*num_criptomonedas].capacidad_mercado,
               &criptomonedas[*num_criptomonedas].volumen_24h,
               &criptomonedas[*num_criptomonedas].cambio_24h);

        (*num_criptomonedas)++;
    }

    fclose(archivo);
    return 0;
}

int escribir_criptomonedas_csv(const char *nombre_archivo, tCriptomoneda *criptomonedas, int* num_criptomonedas) {
    FILE *archivo = fopen(nombre_archivo, "w");
    if (!archivo) {
        perror("Error al abrir el archivo para escritura");
        return -1;
    }

    for (int i = 0; i < *num_criptomonedas; i++) {
        fprintf(archivo, "%s,%s,%.2f,%.2f,%.2f,%.2f\n",
                criptomonedas[i].nombre,
                criptomonedas[i].simbolo,
                criptomonedas[i].precio,
                criptomonedas[i].capacidad_mercado,
                criptomonedas[i].volumen_24h,
                criptomonedas[i].cambio_24h);
        printf("Criptomoneda %d: %s, Simbolo: %s, Precio: %.2f, Capacidad de Mercado: %.2f, Volumen 24h: %.2f, Cambio 24h: %.2f\n",
               i + 1,
               criptomonedas[i].nombre,
               criptomonedas[i].simbolo,
               criptomonedas[i].precio,
               criptomonedas[i].capacidad_mercado,
               criptomonedas[i].volumen_24h,
               criptomonedas[i].cambio_24h);
    }

    fclose(archivo);
    return 0;
}