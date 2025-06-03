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
    int num_usuarios = 0, num_criptomonedas = 0, num_transacciones = 0, status = 0, end = 0;

    pid_t wpid;
    sem_t *console_mutex, *usuarios_mutex, *cripto_mutex;

    tUsuario *usuarios;
    tCriptomoneda *criptomonedas;
    tTransaccionDinero transacciones[MAX_TRANSACCIONES];
    tTransaccionCripto transacciones_cripto[MAX_TRANSACCIONES];

    // Crear memoria compartida para usuarios, criptomonedas, mutex y ruta de archivo
    crear_memoria_compartida(&usuarios, &criptomonedas, &usuarios_mutex, &console_mutex, &cripto_mutex);
    
    leer_criptomonedas_csv("src/criptomonedas.csv", criptomonedas, &num_criptomonedas);
    leer_usuarios_csv("src/usuarios.csv", usuarios, &num_usuarios);
    leer_archivo_transaccion("src/transacciones_transferencias.csv", transacciones, &num_transacciones);
    leer_archivo_transaccion_cripto("src/transacciones_criptomonedas.csv", transacciones_cripto, &num_transacciones);
    
    mostrar_criptomonedas(criptomonedas, num_criptomonedas);
    mostrar_usuarios(usuarios, num_usuarios);
    end = menu(); 
    if(end == 0) {
        realiza_transacciones(num_transacciones, transacciones, transacciones_cripto,usuarios, criptomonedas, num_usuarios, num_criptomonedas, usuarios_mutex, console_mutex, cripto_mutex);
    
        while ((wpid = wait(&status)) > 0); // this way, the father waits for all the child processes 
    }
    printf("Simulación finalizada.\n");
    printf("Proceso padre PID: %d\n", getpid());
    
    
    
    mostrar_criptomonedas(criptomonedas, num_criptomonedas);
    mostrar_usuarios(usuarios, num_usuarios);
    sem_close(console_mutex);
    sem_close(usuarios_mutex);
    sem_close(cripto_mutex);
    
    destruir_memoria_compartida(usuarios, criptomonedas);

    return 0;
}