#pragma once

#include "str.h"

void serial_init();
char serial_read_byte();
void serial_write_byte(char c);
void serial_write_str(const char* str);
void serial_write_str_slice(const StrSlice str);
// Reads characters and prints them until the enter key is pressed or `buf` is filled.
StrSlice serial_read_line(char* buf, u32 buf_len);
