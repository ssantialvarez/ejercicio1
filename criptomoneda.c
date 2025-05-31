#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "listas.h"
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

void leer_criptomonedas_csv(const char *nombre_archivo, tLista *criptomonedas) {
    FILE *archivo = fopen(nombre_archivo, "r");
    tCriptomoneda nueva_cripto;
    if (!archivo) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    char linea[256];
    

    while (fgets(linea, sizeof(linea), archivo)) {
        
        sscanf(linea, "%49[^,],%9[^,],%lf,%lf,%lf,%lf",
               nueva_cripto.nombre,
               nueva_cripto.simbolo,
               &nueva_cripto.precio,
               &nueva_cripto.capacidad_mercado,
               &nueva_cripto.volumen_24h,
               &nueva_cripto.cambio_24h);
        insertarComienzo(criptomonedas, sizeof(tCriptomoneda), &nueva_cripto);
    }

    fclose(archivo);
}

int escribir_criptomonedas_csv(const char *nombre_archivo, tLista *criptomonedas) {
    FILE *archivo = fopen(nombre_archivo, "w");
    tCriptomoneda cripto;
    if (!archivo) {
        perror("Error al abrir el archivo para escritura");
        return -1;
    }

    while(!listaVacia(criptomonedas)) {
        sacarPrimero(criptomonedas, sizeof(tCriptomoneda), &cripto);
        printf("Escribiendo criptomoneda: %s, Simbolo: %s, Precio: %.2f, Capacidad de mercado: %.2f, Volumen 24h: %.2f, Cambio 24h: %.2f\n",
               cripto.nombre,
               cripto.simbolo,
               cripto.precio,
               cripto.capacidad_mercado,
               cripto.volumen_24h,
               cripto.cambio_24h);
        // Escribir en el archivo CSV
        fprintf(archivo, "%s,%s,%.2f,%.2f,%.2f,%.2f\n",
                cripto.nombre,
                cripto.simbolo,
                cripto.precio,
                cripto.capacidad_mercado,
                cripto.volumen_24h,
                cripto.cambio_24h);
    }

    fclose(archivo);
    return 0;
}