#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "usuario.h"

void leer_usuarios_csv(const char *nombre_archivo, tUsuario *usuarios, int* num_usuarios) {
    // Esta función lee un archivo CSV y llena el vector de usuarios
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }
    char linea[256];
    *num_usuarios = 0;
    while (fgets(linea, sizeof(linea), archivo)) {
        // Eliminar el salto de línea al final de la línea
        linea[strcspn(linea, "\n")] = 0; // Eliminar el salto de línea
        // Parsear la línea
        sscanf(linea, "%49[^,],%d,%49[^,],%lf",
               usuarios[*num_usuarios].nombre,
               &usuarios[*num_usuarios].num_cuenta_banco,
               usuarios[*num_usuarios].banco,
               &usuarios[*num_usuarios].saldo);
        (*num_usuarios)++;
    }
}

void mostrar_usuarios(tUsuario *usuarios, int num_usuarios) {
    printf("Lista de usuarios:\n");
    for (int i = 0; i < num_usuarios; i++) {
        printf("Usuario %d: %s, Cuenta: %d, Banco: %s, Saldo: %.2f\n",
               i + 1,
               usuarios[i].nombre,
               usuarios[i].num_cuenta_banco,
               usuarios[i].banco,
               usuarios[i].saldo);
    }
}

int escribir_usuarios_csv(const char *nombre_archivo, tUsuario *usuarios, int num_usuarios) {
    // Esta función escribe el vector de usuarios en un archivo CSV
    FILE *archivo = fopen(nombre_archivo, "w");
    if (!archivo) {
        perror("Error al abrir el archivo para escritura");
        return -1;
    }
    for (int i = 0; i < num_usuarios; i++) {
        fprintf(archivo, "%s,%d,%s,%.2f\n",
                usuarios[i].nombre,
                usuarios[i].num_cuenta_banco,
                usuarios[i].banco,
                usuarios[i].saldo);
        printf("Usuario %d: %s, Cuenta: %d, Banco: %s, Saldo: %.2f\n",
               i + 1,
               usuarios[i].nombre,
               usuarios[i].num_cuenta_banco,
               usuarios[i].banco,
               usuarios[i].saldo);
    }
    fclose(archivo);
    return 0;
}