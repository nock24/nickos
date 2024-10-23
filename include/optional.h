#include "common.h"
#define Optional(T) optional_##T##_t
#define DeclOptional(T) \
    typedef struct {    \
        bool none;      \
        T value;        \
    } Optional(T)

#define Some(val, T)    \
    (Optional(T)){      \
        .none = false,  \
        .value = val    \
    }
#define None(T) (Optional(T)){ true, {} }
