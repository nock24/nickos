/*

#include "print.h"
#include <stdarg.h>
#include "result.h"
#include "str.h"

typedef enum FmtSpecifier {
    Int,
    String,
} FmtSpecifier;
DeclOptional(FmtSpecifier);

const size_t MAX_SPECIFIER_LEN = 3;

Optional(FmtSpecifier) parse_specifier(ConstStr str, size_t* idx) {
    char buf[MAX_SPECIFIER_LEN];
    size_t specifier_len = 0;
    for (size_t i = 0; i < MAX_SPECIFIER_LEN; i++) {
        char c = str.chars[*idx+i];
        if (c == '}') {
            specifier_len = i;
            break;
        }
    }
}

Optional(FormatError) print(const char* format, ...) {
    ConstStr fmt = to_const_str(format);

    va_list args;
    va_start(args, format);

    for (size_t i = 0; i < fmt.len; i++) {
        if (fmt.chars[i] == '{') {
            parse_specifier()
        }
    }

    return None(FormatError);
}

*/
