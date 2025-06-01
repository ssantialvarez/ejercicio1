#ifndef CRIPTOMONEDA_H_INCLUDED
#define CRIPTOMONEDA_H_INCLUDED


typedef struct {
    double      precio;
    char        nombre[50];
    char        simbolo[10];
    double      capacidad_mercado;
    double      volumen_24h;
    double      cambio_24h;
} tCriptomoneda;

int leer_criptomonedas_csv(const char *nombre_archivo, tCriptomoneda *criptomonedas, int *num_criptomonedas);
int escribir_criptomonedas_csv(const char *nombre_archivo, tCriptomoneda *criptomonedas, int *num_criptomonedas);



#endif // CRIPTOMONEDA_H_INCLUDED