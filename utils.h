#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#define MAX_USUARIOS 100
#define MAX_CRIPTOMONEDAS 100
#define MAX_TRANSACCIONES 1000

void* mem_cpy(void*, void*, size_t);
void *_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
pid_t _fork();


void menu();
void crear_memoria_compartida(tUsuario **usuarios, tCriptomoneda **criptomonedas, tTransaccionDinero **transacciones,sem_t **mutex, sem_t **console_mutex);
void destruir_memoria_compartida(tUsuario *usuarios, tCriptomoneda *criptomonedas, tTransaccionDinero *transacciones,sem_t *mutex, sem_t *console_mutex);



#endif // UTILS_H_INCLUDED