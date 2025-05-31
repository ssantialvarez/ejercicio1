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