#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

void* mem_cpy(void*, void*, size_t);
void *_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
pid_t _fork();

#endif // UTILS_H_INCLUDED