#include "main.h"


int main(int argc, char *argv[]) {    
    int num_usuarios = 0, num_criptomonedas = 0;
    char *ruta_archivo;

    pid_t p_menu, p_extraccion, p_ingreso;
    sem_t *mutex;

    tUsuario *usuarios;
    tCriptomoneda *criptomonedas;
    

    // Crear memoria compartida para usuarios, criptomonedas, mutex y ruta de archivo
    crear_memoria_compartida(&usuarios, &criptomonedas, &mutex, &ruta_archivo);
    

    // Crear un proceso hijo para el menú
    p_menu = _fork();
    
    if (p_menu == 0) {
        // Proceso hijo
        menu(ruta_archivo);       
        exit(EXIT_SUCCESS);
    } 
    
    leer_criptomonedas_csv("src/criptomonedas.csv", criptomonedas, &num_criptomonedas);
    leer_usuarios_csv("src/usuarios.csv", usuarios, &num_usuarios);
      
    waitpid(p_menu, NULL, 0); // Esperar a que el proceso hijo termine
    
    escribir_criptomonedas_csv("src/criptomonedas.csv", criptomonedas, &num_criptomonedas);
    escribir_usuarios_csv("src/usuarios.csv", usuarios, num_usuarios);
    printf("Criptomonedas y usuarios leidos correctamente.\n");

    sem_destroy(mutex);
    destruir_memoria_compartida(usuarios, criptomonedas, mutex, ruta_archivo);
    return 0;
}

void menu(char* ruta_archivo) {
    // menu recibe ruta de archivo con registro de transacciones
    printf("===========================================\n");
    printf("SIMULACION DE CRIPTOMONEDAS Y TRANSFERENCIAS\n");
    printf("===========================================\n");
    printf("Leyendo criptomonedas y cuentas...\n");
    printf("Ingresar ruta de archivo:\n");
    fgets(ruta_archivo, sizeof(ruta_archivo), stdin);
    // Eliminar el salto de línea al final de la cadena
    ruta_archivo[strcspn(ruta_archivo, "\n")] = 0; // Eliminar el salto de línea
    printf("Ruta ingresada: %s\n", ruta_archivo);
}

void crear_memoria_compartida(tUsuario **usuarios, tCriptomoneda **criptomonedas, sem_t **mutex, char **ruta_archivo) {
    *usuarios = (tUsuario*)_mmap(NULL, sizeof(tUsuario) * MAX_USUARIOS, PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    *criptomonedas = (tCriptomoneda*)_mmap(NULL, sizeof(tCriptomoneda) * MAX_CRIPTOMONEDAS, PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    *mutex = (sem_t*)_mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    *ruta_archivo = (char*)_mmap(NULL, 256, PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(*mutex, 1, 1);
}

void destruir_memoria_compartida(tUsuario *usuarios, tCriptomoneda *criptomonedas, sem_t *mutex, char *ruta_archivo) {
    munmap(usuarios, sizeof(tUsuario) * MAX_USUARIOS);
    munmap(criptomonedas, sizeof(tCriptomoneda) * MAX_CRIPTOMONEDAS);
    munmap(mutex, sizeof(sem_t));
    munmap(ruta_archivo, 256);
}
