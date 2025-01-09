#pragma once

#include "str.h"

// Reads characters and prints them until the enter key is pressed or `buf` is filled.
Str serial_read_line(char* buf, size_t buf_len);
