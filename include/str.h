#pragma once

#include "basic_types.h"

typedef struct Str {
    char* chars;
    u32 len;
} Str;

typedef const Str ConstStr;

Str to_str(char* s);
ConstStr to_const_str(const char* s);
ConstStr str_slice(const char* str, u32 start, u32 end);
bool str_cmp(const char* a, const char* b);
