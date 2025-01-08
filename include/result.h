#define Result(T, E) result_##T##_##E##_t
#define DeclResult(T, E) \
    typedef struct {     \
        bool ok;         \
        union {          \
            T ok;        \
            E err;       \
        } value_;        \
    } Result(T, E)

#define Ok(T, E, value)     \
    (Result(T, E)){         \
        .ok = true,         \
        .value_.ok = value, \
    }
#define Err(T, E, error)     \
    (Result(T, E)){          \
        .ok = false,         \
        .value_.err = error, \
    }

#define as_value(result) result.value_.ok
#define as_error(result) result.value_.err

