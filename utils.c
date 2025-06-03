#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include "criptomoneda.h"
#include "usuario.h"
#include "transaccion.h"
#include "utils.h"

#define nombreMemoria "/memoriaUsuarios"


void* mem_cpy(void* dst, void* org, size_t n) {
    while(n--) {
	*(char*)dst = *(char*)org;
        (char*)dst++;
        (char*)org++;
    }

   return dst;
}

pid_t _fork() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    }
    return pid;
}

void *_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
    void *ptr = mmap(addr, length, prot, flags, fd, offset);
    if (ptr == MAP_FAILED) {
        perror("Error al mapear memoria");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

int menu() {
    char caracter;
    int i = 2;
    // menu recibe ruta de archivo con registro de transacciones
    printf("===========================================\n");
    printf("SIMULACION DE CRIPTOMONEDAS Y TRANSFERENCIAS\n");
    printf("===========================================\n");
    printf("Proceso padre PID: %d\n", getpid());
    printf("Leyendo criptomonedas y cuentas...\n");
    // Ingresa Y o N para iniciar la simulación
    printf("¿Desea iniciar la simulación? (Y/N): ");
    do{
    scanf(" %c", &caracter);
    if (caracter == 'Y' || caracter == 'y') {
        printf("Iniciando simulación...\n");
        i = 0;
    } else if (caracter == 'N' || caracter == 'n') {
        printf("Simulación cancelada.\n");
        i = 1; 
    }
    }while(i != 0 && i != 1);
    printf("===========================================\n");
    return i;
}

void crear_memoria_compartida(    
    tUsuario **usuarios, 
    tCriptomoneda **criptomonedas, 

    sem_t **usuarios_mutex, 
    sem_t **console_mutex, 
    sem_t **cripto_mutex) {

    int idMemoriaUsuarios = shm_open(nombreMemoria, O_CREAT | O_RDWR, 0600);
    ftruncate(idMemoriaUsuarios, sizeof(tUsuario) * MAX_USUARIOS); 

    *usuarios = (tUsuario*)_mmap(NULL, sizeof(tUsuario) * MAX_USUARIOS, PROT_READ | PROT_WRITE,
                        MAP_SHARED , idMemoriaUsuarios, 0);
    
    *criptomonedas = (tCriptomoneda*)_mmap(NULL, sizeof(tCriptomoneda) * MAX_CRIPTOMONEDAS, PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    *usuarios_mutex = sem_open("/usuarios_mutex", O_CREAT, 0666, 1);
    if (*usuarios_mutex == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    *cripto_mutex = sem_open("/cripto_mutex", O_CREAT, 0666, 1);
    if (*cripto_mutex == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    *console_mutex = sem_open("/console_mutex", O_CREAT, 0666, 1);
    if (*console_mutex == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    if (close(idMemoriaUsuarios)==-1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    
}

void destruir_memoria_compartida(
    tUsuario *usuarios, 
    tCriptomoneda *criptomonedas
) {
    munmap(usuarios, sizeof(tUsuario) * MAX_USUARIOS);
    munmap(criptomonedas, sizeof(tCriptomoneda) * MAX_CRIPTOMONEDAS);
    sem_unlink("/usuarios_mutex");
    sem_unlink("/cripto_mutex");
    sem_unlink("/console_mutex");

    shm_unlink(nombreMemoria);
}



void realiza_transacciones(int num_transacciones, tTransaccionDinero *transacciones, tTransaccionCripto *trans_cripto,tUsuario *usuarios, tCriptomoneda *criptomonedas, int num_usuarios, int num_criptomonedas, sem_t *usuarios_mutex, sem_t *console_mutex, sem_t *cripto_mutex) {
    int i, j;
    pid_t child_pid;

    
    
    for(i = 0; i < MAX_TRANSACCIONES; i++) {
        if ((child_pid = _fork()) == 0) {
            printf("Ejecutando transacción %d en proceso hijo con PID %d\n", i, getpid());
                        
            if (ejecuta_transaccion_dinero(&transacciones[i], usuarios, criptomonedas, num_usuarios, num_criptomonedas, usuarios_mutex, console_mutex ) < 0) {
                printf("Error al ejecutar la transacción %d\n", i);
            }

            exit(EXIT_SUCCESS);
        } else if (child_pid < 0) {
            
            perror("Error al crear el proceso hijo");
            exit(EXIT_FAILURE);
        }
    }
    
    for(j = 0; j < MAX_TRANSACCIONES; j++) {
        if ((child_pid = _fork()) == 0) {
            printf("Ejecutando transacción %d en proceso hijo con PID %d\n", j, getpid());
            
            if (ejecuta_transaccion_cripto(&trans_cripto[j], usuarios, criptomonedas, num_usuarios, num_criptomonedas, usuarios_mutex, console_mutex, cripto_mutex ) < 0) {
                printf("Error al ejecutar la transacción %d\n", j);
            } 
            exit(EXIT_SUCCESS);
        } else if (child_pid < 0) {
            perror("Error al crear el proceso hijo");
            exit(EXIT_FAILURE);
        } 
    }
}
