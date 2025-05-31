#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include "listas.h"
#include "criptomoneda.h"
#include "usuario.h"



int main(int argc, char *argv[]) {    
    tLista usuarios;
    tLista criptomonedas;

    pid_t p_criptomonedas;
    sem_t *mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    crearLista(&usuarios);
    crearLista(&criptomonedas);
    sem_init(mutex, 1, 1);
    

    p_criptomonedas = fork();
    
    
    if (p_criptomonedas < 0) {
        perror("Error al crear el proceso para criptomonedas");
        exit(EXIT_FAILURE);
    } 
    
    if (p_criptomonedas == 0) {
        // Proceso hijo para criptomonedas
        leer_criptomonedas_csv("src/criptomonedas.csv", &criptomonedas);
        
        sem_wait(mutex);
        printf("Criptomonedas leídas:\n");
        escribir_criptomonedas_csv("src/criptomonedas.csv", &criptomonedas);
        sem_post(mutex); 
        exit(0);
    } else{
        leer_usuarios_csv("src/usuarios.csv", &usuarios);
        
        sem_wait(mutex);
        printf("Usuarios leídos: \n");
        escribir_usuarios_csv("src/usuarios.csv", &usuarios);    
        sem_post(mutex); 
    }
    
    // Proceso padre espera a los hijos
    waitpid(p_criptomonedas, NULL, 0);

    sem_destroy(mutex);
    munmap(mutex, sizeof(sem_t));

    vaciarLista(&usuarios);
    vaciarLista(&criptomonedas);
    return 0;
}