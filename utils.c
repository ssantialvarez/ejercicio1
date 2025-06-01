#include <sys/types.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "utils.h"


void* mem_cpy(void* dst, void* org, size_t n) {
    while(n--) {
	*(char*)dst = *(char*)org;
        (char*)dst++;
        (char*)org++;
    }

   return dst;
}

pid_t _fork() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    }
    return pid;
}

void *_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
    void *ptr = mmap(addr, length, prot, flags, fd, offset);
    if (ptr == MAP_FAILED) {
        perror("Error al mapear memoria");
        exit(EXIT_FAILURE);
    }
    return ptr;
}