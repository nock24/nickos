#include "str.h"

Str to_str(char* str) {
    char* tmp = str;
    size_t len = 0;
    while (*tmp) {
        len++;
        tmp++;
    }
    return (Str){
        .chars = str,
        .len = len,
    };
}

const Str to_const_str(const char* str) {
    const char* tmp = str;
    size_t len = 0; 
    while (*tmp) {
        len++;
        tmp++;
    }
    return (const Str){
        .chars = (char*)str,
        .len = len,
    };
}

const Str str_slice(const char* str, u32 start, u32 end) {
    return (Str){ 
        .chars = (char*)&str[start],
        .len = end - start,
    };
}

void str_cpy(const Str src, char* dest) {
    for (u32 i = 0; i < src.len; i++) {
        if (!dest[i]) break;
        dest[i] = src.chars[i];
    } 
}

bool str_cmp(const char* a, const char* b) {
    size_t i = 0;
    while (true) {
        if (a[i] != b[i]) return false;
        if (!a[i] && !b[i]) return true; // we are at the end of both strings
        i++;
    }
}

/// Sets all characters to null terminators.
void set_null(char* str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        str[i] = '\0';
    }
}
