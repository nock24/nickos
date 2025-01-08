#pragma once

#include "drivers/serial.h"
#include "optional.h"

typedef enum FormatError {
    UnclosedSpecifier,
} FormatError;
DeclOptional(FormatError);

Optional(FormatError) print(const char* format, ...);
