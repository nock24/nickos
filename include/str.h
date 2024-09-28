#pragma once

#include "common.h"

typedef struct Str {
    char* chars;
    u32 len;
} Str;

typedef struct StrSlice {
    const char* chars;
    u32 len;
} StrSlice;

Str to_str(char* s);
void str_cpy(const Str src, char* dest);
StrSlice str_slice(const char* str, u32 start, u32 end);
bool str_cmp(const char* a, const char* b);
