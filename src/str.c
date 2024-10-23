#include "str.h"

Str to_str(char* s) {
    char* tmp = s;
    u32 len = 0;
    while (*tmp) {
        len++;
        tmp++;
    }
    return (Str){ s, len };
}

StrSlice str_slice(const char* str, u32 start, u32 end) {
    return (StrSlice){ &str[start], end - start };
}

void str_cpy(const Str src, char* dest) {
    for (u32 i = 0; i < src.len; i++) {
        if (!dest[i]) break;
        dest[i] = src.chars[i];
    } 
}

bool str_cmp(const char* a, const char* b) {
    u32 i = 0;
    while (true) {
        if (a[i] != b[i]) return false;
        if (!a[i] && !b[i]) return true; // we are at the end of both strings
        i++;
    }
}
