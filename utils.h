#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED


#define MAX_USUARIOS 100
#define MAX_CRIPTOMONEDAS 100
#define MAX_TRANSACCIONES 100


void* mem_cpy(void*, void*, size_t);
void *_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
pid_t _fork();


int menu();
void crear_memoria_compartida(tUsuario **usuarios, tCriptomoneda **criptomonedas, sem_t **mutex, sem_t **console_mutex, sem_t **cripto_mutex);
void destruir_memoria_compartida(tUsuario *usuarios, tCriptomoneda *criptomonedas);
void realiza_transacciones(int num_transacciones, tTransaccionDinero *transacciones, tTransaccionCripto *trans_cripto, tUsuario *usuarios, tCriptomoneda *criptomonedas, int num_usuarios, int num_criptomonedas, sem_t *usuarios_mutex, sem_t *console_mutex, sem_t *cripto_mutex);
#endif // UTILS_H_INCLUDED