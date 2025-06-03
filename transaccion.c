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


int leer_archivo_transaccion(const char* nombre_archivo, tTransaccionDinero* transacciones, int* num_transacciones) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        perror("Error al abrir el archivo de transacciones");
        return -1;
    }

    char linea[256];
    *num_transacciones = 0;

    while ((*num_transacciones) < MAX_TRANSACCIONES && fgets(linea, sizeof(linea), archivo)) {
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

int leer_archivo_transaccion_cripto(const char* nombre_archivo, tTransaccionCripto* transacciones, int* num_transacciones) {
    FILE *archivo = fopen(nombre_archivo, "r");
    int i = 0;
    if (!archivo) {
        perror("Error al abrir el archivo de transacciones de criptomonedas");
        return -1;
    }

    char linea[256];

    while (i < MAX_TRANSACCIONES && fgets(linea, sizeof(linea), archivo)) {

        // Eliminar el salto de línea al final de la línea
        linea[strcspn(linea, "\n")] = 0; // Eliminar el salto de línea

 
        sscanf(linea, "%d,%9[^,],%lf",
               &transacciones[i].num_cuenta_banco,
               transacciones[i].simbolo,
               &transacciones[i].cantidad
            );
        i++;
    }
    *num_transacciones += i;
    fclose(archivo);
    return 0;
}

int ejecuta_transaccion_dinero(tTransaccionDinero* t, tUsuario* usuarios, tCriptomoneda* criptomonedas, int num_usuarios, int num_criptomonedas, sem_t *usuarios_mutex, sem_t *console_mutex) {
    // Verificar si las cuentas de origen y destino existen
    tUsuario *cuenta_origen = NULL, *cuenta_destino = NULL;

    sem_wait(usuarios_mutex); // Bloquear el mutex para acceso exclusivo a los usuarios
    for (int i = 0; i < num_usuarios; i++) {
        if (usuarios[i].num_cuenta_banco == t->num_cuenta_origen) 
            cuenta_origen = &usuarios[i];
        
        if (usuarios[i].num_cuenta_banco == t->num_cuenta_destino) 
            cuenta_destino = &usuarios[i];
        
    }
    
    if (!cuenta_destino || !cuenta_origen) {
        printf("Error: Una o ambas cuentas no existen.\n");
        sem_post(usuarios_mutex); // Liberar el mutex
        return -1;
    }

    // Verificar si la cuenta de origen tiene suficiente saldo
    if (cuenta_origen->saldo < t->cantidad) {
        printf("Error: Saldo insuficiente en la cuenta de origen.\n");
        sem_post(usuarios_mutex); // Liberar el mutex
        return -1;
    }
    
    // Realizar la transacción
    cuenta_origen->saldo -= t->cantidad; // Restar de la cuenta de origen
    cuenta_destino->saldo += t->cantidad; // Sumar a la cuenta de destino
    
    //sleep(2); // Simular un pequeño retraso para la transacción
    sem_post(usuarios_mutex); // Liberar el mutex
    
    
    sem_wait(console_mutex); // Bloquear el mutex para acceso exclusivo a la consola
    printf("Transacción exitosa: %d -> %d, Cantidad: %.2f\n",
           t->num_cuenta_origen, t->num_cuenta_destino, t->cantidad);
    printf("Proceso PID: %d\n", getpid());
    printf("---------------------\n");
    sem_post(console_mutex); // Liberar el mutex de la consola
    
    return 0;
}

int ejecuta_transaccion_cripto(tTransaccionCripto* t, tUsuario* usuarios, tCriptomoneda* criptomonedas, int num_usuarios, int num_criptomonedas, sem_t *usuarios_mutex, sem_t *console_mutex, sem_t *cripto_mutex){
    tUsuario *usuario = NULL;
    tCriptomoneda *cripto = NULL;
    double saldo_origen = 0.0, precio_cripto = 0.0;

    sem_wait(usuarios_mutex); // Bloquear el mutex para acceso exclusivo a los usuarios
    sem_wait(cripto_mutex); // Bloquear el mutex para acceso exclusivo a las criptomonedas
    // Verificar si la cuentas de origen existe    
    for (int i = 0; i < num_usuarios; i++){
        if (usuarios[i].num_cuenta_banco == t->num_cuenta_banco){
            usuario = &usuarios[i];
            break;
        }
    }           
    for(int i = 0; i < num_criptomonedas; i++) {
        if (strcmp(criptomonedas[i].simbolo, t->simbolo) == 0) {
            cripto = &criptomonedas[i];
            break;
        }
    }
    
    if (!usuario || !cripto) {
        printf("Error: Una o ambas cuentas no existen.\n");
        sem_post(usuarios_mutex); // Liberar el mutex
        sem_post(cripto_mutex); // Liberar el mutex
        return -1;
    }

    if (saldo_origen < t->cantidad * precio_cripto) {
        printf("Error: Saldo insuficiente en la cuenta de origen.\n");
        sem_post(cripto_mutex); // Liberar el mutex
        sem_post(usuarios_mutex); // Liberar el mutex
        return -1;
    }
    
    // Realizar la transacción
    usuario->saldo -= t->cantidad * precio_cripto; // Restar de la cuenta de origen
    if(cripto->capacidad_mercado - t->cantidad >= 0)
        cripto->capacidad_mercado -= t->cantidad; // Restar de la capacidad de mercado
    else {
        printf("Error: Saldo insuficiente en la cuenta de origen.\n");
        sem_post(cripto_mutex); // Liberar el mutex
        sem_post(usuarios_mutex); // Liberar el mutex
        return -1;
    }
    cripto->volumen_24h *= 1.05; // Aumentar el volumen en las últimas 24 horas
    cripto->cambio_24h *= 1.05; // Aumentar la variación en las últimas 24 horas
    if (cripto->capacidad_mercado < 1000) {
        cripto->precio *= 1.10; // Aumentar el precio en un 10% si las compras superan las 1000
    }
    
    //sleep(2); // Simular un pequeño retraso para la transacción
    sem_post(usuarios_mutex); // Liberar el mutex
    sem_post(cripto_mutex); // Liberar el mutex de criptomonedas
    
    sem_wait(console_mutex); // Bloquear el mutex para acceso exclusivo a la consola
    printf("Transacción exitosa: %d -> %s, Cantidad: %.2f\n",
           t->num_cuenta_banco, t->simbolo, t->cantidad);
    printf("Proceso PID: %d\n", getpid());
    printf("---------------------\n");
    sem_post(console_mutex); // Liberar el mutex de la consola
    
    return 0;
}
