#ifndef USUARIO_H_INCLUDED
#define USUARIO_H_INCLUDED


typedef struct {
    double  saldo;
    int     num_cuenta_banco;
    char    nombre[50];
    char    banco[50];
} tUsuario;

// function to read usuarios.csv
void leer_usuarios_csv(const char *nombre_archivo, tUsuario *usuarios, int* num_usuarios);
int escribir_usuarios_csv(const char *nombre_archivo, tUsuario *usuarios, int num_usuarios);
void mostrar_usuarios(tUsuario *usuarios, int num_usuarios);

#endif // USUARIO_H_INCLUDED