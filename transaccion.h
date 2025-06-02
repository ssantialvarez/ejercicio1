#ifndef TRANSACCION_H_INCLUDED
#define TRANSACCION_H_INCLUDED

typedef struct {
    int         num_cuenta_banco;     // User ID associated with the transaction
    double      cantidad;       // Amount of cryptocurrency involved in the transaction
    char        simbolo[10];    // Cryptocurrency ID involved in the transaction
} tTransaccionCripto;

typedef struct {
    int         num_cuenta_origen;     // Source bank account number
    int         num_cuenta_destino; // Destination bank account number
    double      cantidad;       // Amount of money involved in the transaction
} tTransaccionDinero;

int ejecuta_transaccion_dinero(
                            tTransaccionDinero* t, 
                            tUsuario* usuarios, 
                            tCriptomoneda* criptomonedas, 
                            int num_usuarios, 
                            int num_criptomonedas, 
                            sem_t *usuario_mutex, 
                            sem_t *console_mutex);

int ejecuta_transaccion_cripto(
                            tTransaccionCripto* t, 
                            tUsuario* usuarios, 
                            tCriptomoneda* criptomonedas, 
                            int num_usuarios, 
                            int num_criptomonedas, 
                            sem_t *usuario_mutex, 
                            sem_t *console_mutex, 
                            sem_t *cripto_mutex);

int leer_archivo_transaccion(const char* nombre_archivo, tTransaccionDinero* transacciones, int* num_transacciones);
int leer_archivo_transaccion_cripto(const char* nombre_archivo, tTransaccionCripto* transacciones, int* num_transacciones);


#endif // TRANSACCION_H_INCLUDED