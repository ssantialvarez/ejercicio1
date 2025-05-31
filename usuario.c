#include <stdio.h>
#include <stdlib.h>
#include "listas.h"
#include "usuario.h"


void leer_usuarios_csv(const char *nombre_archivo, tLista *usuarios) {
    FILE *archivo = fopen(nombre_archivo, "r");
    tUsuario nuevo_usuario;
    if (!archivo) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    char linea[256];
    

    while (fgets(linea, sizeof(linea), archivo)) {
        sscanf(linea, "%49[^,],%d,%49[^,],%lf",
               nuevo_usuario.nombre,
               &nuevo_usuario.num_cuenta_banco,
               nuevo_usuario.banco,
               &nuevo_usuario.saldo);
        insertarUltimo(usuarios, sizeof(tUsuario), &nuevo_usuario);
    }

    fclose(archivo);
}

int escribir_usuarios_csv(const char *nombre_archivo, tLista *usuarios) {
    FILE *archivo = fopen(nombre_archivo, "w");
    tUsuario nuevo_usuario;
    if (!archivo) {
        perror("Error al abrir el archivo para escritura");
        return -1;
    }
    while(!listaVacia(usuarios)) {
        sacarPrimero(usuarios, sizeof(tUsuario), &nuevo_usuario);
        printf("Escribiendo usuario: %s, Cuenta: %d, Banco: %s, Saldo: %.2f\n",
               nuevo_usuario.nombre,
               nuevo_usuario.num_cuenta_banco,
               nuevo_usuario.banco,
               nuevo_usuario.saldo);
        // Escribir en el archivo CSV
        fprintf(archivo, "%s,%d,%s,%.2f\n",
                nuevo_usuario.nombre,
                nuevo_usuario.num_cuenta_banco,
                nuevo_usuario.banco,
                nuevo_usuario.saldo);
    }

    fclose(archivo);
    return 0;
}
