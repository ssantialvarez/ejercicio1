#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include "criptomoneda.h"
#include "usuario.h"
#include "transaccion.h"
#include "utils.h"

#define MAX_USUARIOS 100
#define MAX_CRIPTOMONEDAS 100
#define MAX_TRANSACCIONES 1000


void menu();
void crear_memoria_compartida(tUsuario **usuarios, tCriptomoneda **criptomonedas, sem_t **mutex, char **ruta_archivo);
void destruir_memoria_compartida(tUsuario *usuarios, tCriptomoneda *criptomonedas, sem_t *mutex, char *ruta_archivo);

#endif // MAIN_H_INCLUDED