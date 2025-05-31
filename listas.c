#include <stdlib.h>
#include "listas.h"
#include "utils.h"

size_t minimo(size_t a, size_t b) {
    return (a < b) ? a : b;
}

void crearLista(tLista* pl) {
    *pl = NULL;
}

int insertarUltimo(tLista* pl, size_t tamDato, const void *dato) {
    tNodo* pn = malloc(sizeof(tNodo)); // Allocate memory for the new node
    if(!pn) // Check if memory allocation was successful
        return -1;
    pn->tam = tamDato;
    pn->dato = malloc(pn->tam);
    if(!pn->dato) { // Check if memory allocation for data was successful
        free(pn);
        return -1;
    }
    mem_cpy(pn->dato, (void*)dato, pn->tam);
    
    while(*pl)
        pl = &(*pl)->sig;
    *pl = pn;
    pn->sig = NULL;
    
    return 1;
}

int insertarComienzo(tLista* pl, size_t tamDato, const void *dato) {
    tNodo* pn = malloc(sizeof(tNodo)); // Allocate memory for the new node
    if(!pn) // Check if memory allocation was successful
        return -1;
    pn->tam = tamDato;
    pn->dato = malloc(tamDato);
    if(!pn->dato) { // Check if memory allocation for data was successful
        free(pn);
        return -1;
    }
    mem_cpy(pn->dato, (void*)dato, tamDato);
    
    pn->sig = *pl; // Point the new node to the current first node
    *pl = pn; // Update the head of the list to the new node
    
    return 1;
}

int sacarPrimero(tLista* pl, size_t n, void* info) {
    tNodo* pn;
    if(!(*pl))
        return -1;
    pn = *pl;
    *pl = pn->sig;
    mem_cpy(info, pn->dato, minimo(n, pn->tam));
    free(pn->dato);
    free(pn);
    return 1;
}

int sacarUltimo(tLista* pl, size_t n, void* info) {
    if(!(*pl))
        return -1;

    while((*pl)->sig) 
        pl = &(*pl)->sig;
    

    mem_cpy(info, (*pl)->dato, minimo(n, (*pl)->tam));
    free((*pl)->dato);
    free(*pl);
    *pl = NULL;
    return 1;
}

int verPrimero(const tLista* pl, size_t n, void* info) {
    if(!(*pl))
        return -1;
    mem_cpy(info, (*pl)->dato, minimo(n, (*pl)->tam));
    return 1;
}

int verUltimo(const tLista* pl, size_t n, void* info) {
    if(!(*pl))
        return -1;

    while((*pl)->sig)
        pl = &(*pl)->sig;

    mem_cpy(info, (*pl)->dato, minimo(n, (*pl)->tam));
    return 1;
}

int listaVacia(const tLista* pl) {
    return *pl == NULL;
}

int listaLlena(const tLista* pl, size_t tamDato) {
    tNodo* pn = (tNodo*)malloc(sizeof(tNodo));
    void *dato = malloc(tamDato);

    free(dato);
    free(pn);
    return dato == NULL || dato == NULL ;
}

void vaciarLista(tLista* pl) {
    tNodo* pn;
    while(*pl) {
        pn = *pl;
        *pl = pn->sig;
        free(pn->dato);
        free(pn);
    }
}


