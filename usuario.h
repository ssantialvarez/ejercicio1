#ifndef USUARIO_H_INCLUDED
#define USUARIO_H_INCLUDED


typedef struct {
    double  saldo;
    int     num_cuenta_banco;
    char    nombre[50];
    char    banco[50];
} tUsuario;

// function to read usuarios.csv
void leer_usuarios_csv(const char *nombre_archivo, tLista *usuarios);
int escribir_usuarios_csv(const char *nombre_archivo, tLista *usuarios);


#endif // USUARIO_H_INCLUDED