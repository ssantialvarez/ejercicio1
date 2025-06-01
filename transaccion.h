#ifndef TRANSACCION_H_INCLUDED
#define TRANSACCION_H_INCLUDED

typedef struct {
    int         num_cuenta_banco;     // User ID associated with the transaction
    char        simbolo[10];    // Cryptocurrency ID involved in the transaction
    double      cantidad;       // Amount of cryptocurrency involved in the transaction
} tTransaccionCripto;

typedef struct {
    int         num_cuenta_origen;     // Source bank account number
    int         num_cuenta_destino; // Destination bank account number
    double      cantidad;       // Amount of money involved in the transaction
} tTransaccionDinero;







#endif // TRANSACCION_H_INCLUDED