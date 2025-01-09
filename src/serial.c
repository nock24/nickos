#include "serial.h"
#include "drivers/uart.h"

Str serial_read_line(char* buf, size_t buf_len) {
    size_t input_end;
    for (size_t i = 0; i < buf_len; i++) {
        char c = uart_getc();
        uart_send(c);
        if (c == '\n') {
            input_end = i;
            break;
        }
        buf[i] = c;
    }

    return str_slice(buf, 0, input_end);
}
