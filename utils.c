#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/mman.h>
#include "criptomoneda.h"
#include "usuario.h"
#include "transaccion.h"
#include "utils.h"

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

void menu(char* ruta_archivo) {
    // menu recibe ruta de archivo con registro de transacciones
    printf("===========================================\n");
    printf("SIMULACION DE CRIPTOMONEDAS Y TRANSFERENCIAS\n");
    printf("===========================================\n");
    printf("Proceso padre PID: %d\n", getpid());
    printf("Leyendo criptomonedas y cuentas...\n");
    printf("Ingresar ruta de archivo:\n");
    fgets(ruta_archivo, 256, stdin);
    // Eliminar el salto de línea al final de la cadena
    ruta_archivo[strcspn(ruta_archivo, "\n")] = 0; // Eliminar el salto de línea
    printf("Ruta ingresada: %s\n", ruta_archivo);
}

void crear_memoria_compartida(tUsuario **usuarios, tCriptomoneda **criptomonedas, tTransaccionDinero **transacciones, sem_t **mutex, sem_t **console_mutex) {
    *usuarios = (tUsuario*)_mmap(NULL, sizeof(tUsuario) * MAX_USUARIOS, PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    *criptomonedas = (tCriptomoneda*)_mmap(NULL, sizeof(tCriptomoneda) * MAX_CRIPTOMONEDAS, PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    *transacciones = (tTransaccionDinero*)mmap(NULL, sizeof(tTransaccionDinero) * MAX_TRANSACCIONES, PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    *mutex = (sem_t*)_mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    *console_mutex = (sem_t*)_mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    sem_init(*console_mutex, MAX_TRANSACCIONES, 1);
    sem_init(*mutex, MAX_TRANSACCIONES, 1);
}

void destruir_memoria_compartida(tUsuario *usuarios, tCriptomoneda *criptomonedas, tTransaccionDinero *transacciones, sem_t *mutex, sem_t *console_mutex) {
    munmap(usuarios, sizeof(tUsuario) * MAX_USUARIOS);
    munmap(criptomonedas, sizeof(tCriptomoneda) * MAX_CRIPTOMONEDAS);
    munmap(mutex, sizeof(sem_t));
    munmap(console_mutex, sizeof(sem_t));
    munmap(transacciones, sizeof(tTransaccionDinero) * MAX_TRANSACCIONES);
}

