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

int main(int argc, char *argv[]) {    
    int num_usuarios = 0, num_criptomonedas = 0, num_transacciones = 0, status = 0;
    char ruta_archivo[256]; // Ruta

    pid_t child_pid, wpid;
    sem_t *console_mutex, *usuarios_mutex;

    tUsuario *usuarios;
    tCriptomoneda *criptomonedas;
    tTransaccionDinero *transacciones;

    // Crear memoria compartida para usuarios, criptomonedas, mutex y ruta de archivo
    crear_memoria_compartida(&usuarios, &criptomonedas, &transacciones, &usuarios_mutex, &console_mutex);
    
    leer_criptomonedas_csv("src/criptomonedas.csv", criptomonedas, &num_criptomonedas);
    leer_usuarios_csv("src/usuarios.csv", usuarios, &num_usuarios);
      
    menu(ruta_archivo); // Llamar al menú para ingresar la ruta del archivo de transacciones
    
    leer_archivo_transaccion(ruta_archivo, transacciones, &num_transacciones);
    

    for(int i = 0; i < num_transacciones; i++) {
        if ((child_pid = _fork()) == 0) {
            printf("Ejecutando transacción %d en proceso hijo con PID %d\n", i, getpid());
            
            // Proceso hijo
            if (ejecuta_transaccion_dinero(&transacciones[i], usuarios, criptomonedas, num_usuarios, num_criptomonedas, usuarios_mutex, console_mutex ) < 0) {
                printf("Error al ejecutar la transacción %d\n", i);
            }
            
            exit(0); // Terminar el proceso hijo
        } else if (child_pid < 0) {
            perror("Error al crear el proceso hijo");
            exit(EXIT_FAILURE);
        }
    }
    
    while ((wpid = wait(&status)) > 0); // this way, the father waits for all the child processes 
    sem_destroy(console_mutex);
    sem_destroy(usuarios_mutex);
    destruir_memoria_compartida(usuarios, criptomonedas, transacciones, usuarios_mutex,console_mutex);

    return 0;
}