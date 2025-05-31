#ifndef LISTAS_H_INCLUDED
#define LISTAS_H_INCLUDED

#define TODO_OK 0
#define SIN_MEM 1
#define CLA_DUP 2

typedef struct sNodo {
    size_t        tam;
    void*         dato;
    struct sNodo* sig;
} tNodo;

typedef tNodo* tLista;

void crearLista(tLista* pl);

int insertarComienzo(tLista* pl, size_t tamDato, const void *dato);

int insertarUltimo(tLista* pl, size_t tamDato, const void *dato); 

int sacarPrimero(tLista* pl, size_t tamDato, void* dato);

int sacarUltimo(tLista* pl, size_t tamDato, void* dato);

int verPrimero(const tLista* pl, size_t tamDato, void* dato);

int verUltimo(const tLista* pl, size_t tamDato, void* dato);

int listaVacia(const tLista* pl);

int listaLlena(const tLista* pl, size_t tamDato);

void vaciarLista(tLista*);


#endif // LISTAS_H_INCLUDED