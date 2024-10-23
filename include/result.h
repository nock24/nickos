#include "common.h"

#define Result(T, E) result_##T##_##E##_t
#define DeclResult(T, E) \
    typedef struct {     \
        bool ok;         \
        union {          \
            T ok;        \
            E err;       \
        } _value;        \
    } Result(T, E)

#define Ok(value, T, E)    \
    (Result(T, E)){        \
        .ok = true,        \
        ._value.ok = value \
    }
#define Err(error, T, E)    \
    (Result(T, E)){         \
        .ok = false,        \
        ._value.err = error \
    }

#define as_value(result) result._value.ok
#define as_error(result) result._value.err

