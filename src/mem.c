#include "mem.h"

void mem_set(void* arr, u32 n, u8 value) {
    u8* bytes = (u8*)arr;
    for (u32 i = 0; i < n; i++) {
        bytes[i] = value;
    }
}
