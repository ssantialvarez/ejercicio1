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
/*
reads this structure from a CSV file:
typedef struct {
    int         num_cuenta_origen;     // Source bank account number
    int         num_cuenta_destino; // Destination bank account number
    double      cantidad;       // Amount of money involved in the transaction
} tTransaccionDinero;
*/
int leer_archivo_transaccion(const char* nombre_archivo, tTransaccionDinero* transacciones, int* num_transacciones) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        perror("Error al abrir el archivo de transacciones");
        return -1;
    }

    char linea[256];
    *num_transacciones = 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        // Eliminar el salto de línea al final de la línea
        linea[strcspn(linea, "\n")] = 0; // Eliminar el salto de línea

        // Parsear la línea de transacción dinero
        sscanf(linea, "%d,%d,%lf",
               &transacciones[*num_transacciones].num_cuenta_origen,
               &transacciones[*num_transacciones].num_cuenta_destino,
               &transacciones[*num_transacciones].cantidad);

        (*num_transacciones)++;
    }

    fclose(archivo);
    return 0;
}

int ejecuta_transaccion_dinero(tTransaccionDinero* t, tUsuario* usuarios, tCriptomoneda* criptomonedas, int num_usuarios, int num_criptomonedas, sem_t *usuarios_mutex, sem_t *console_mutex) {
    // Verificar si las cuentas de origen y destino existen
    int cuenta_origen_existe = 0, cuenta_destino_existe = 0;

    
    for (int i = 0; i < num_usuarios; i++) {
        if (usuarios[i].num_cuenta_banco == t->num_cuenta_origen) 
            cuenta_origen_existe = 1;
        
        if (usuarios[i].num_cuenta_banco == t->num_cuenta_destino) 
            cuenta_destino_existe = 1;
        
    }
    
    if (!cuenta_origen_existe || !cuenta_destino_existe) {
        printf("Error: Una o ambas cuentas no existen.\n");
        return -1;
    }

    sem_wait(usuarios_mutex); // Bloquear el mutex para acceso exclusivo a los usuarios
    // Verificar si la cuenta de origen tiene suficiente saldo
    double saldo_origen = 0.0;
    for (int i = 0; i < num_usuarios; i++) {
        if (usuarios[i].num_cuenta_banco == t->num_cuenta_origen) {
            saldo_origen = usuarios[i].saldo;
            break;
        }
    }
    
    if (saldo_origen < t->cantidad) {
        printf("Error: Saldo insuficiente en la cuenta de origen.\n");
        return -1;
    }
    
    // Realizar la transacción
    for (int i = 0; i < num_usuarios; i++) {
        if (usuarios[i].num_cuenta_banco == t->num_cuenta_origen) {
            usuarios[i].saldo -= t->cantidad; // Restar de la cuenta de origen
        }
        if (usuarios[i].num_cuenta_banco == t->num_cuenta_destino) {
            usuarios[i].saldo += t->cantidad; // Sumar a la cuenta de destino
        }
    }
    sem_post(usuarios_mutex); // Liberar el mutex
    sleep(10); // Simular un pequeño retraso para la transacción
    
    sem_wait(console_mutex); // Bloquear el mutex para acceso exclusivo a la consola
    printf("Transacción exitosa: %d -> %d, Cantidad: %.2f\n",
           t->num_cuenta_origen, t->num_cuenta_destino, t->cantidad);
    printf("Proceso PID: %d\n", getpid());
    printf("---------------------\n");
    sem_post(console_mutex); // Liberar el mutex de la consola
    
    return 0;
}

