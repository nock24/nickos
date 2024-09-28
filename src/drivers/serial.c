#include "drivers/serial.h"
#include "peripherals/aux.h"
#include "drivers/gpio.h"

#define TXD 14
#define RXD 15

void serial_init() {
    gpio_pin_set_func(TXD, GFAlt5);
    gpio_pin_set_func(RXD, GFAlt5);

    gpio_pin_enable(TXD);
    gpio_pin_enable(RXD);

    AUX_REGS->enables = 1;
    AUX_REGS->mu_control = 0;
    AUX_REGS->mu_ier = 0;
    AUX_REGS->mu_lcr = 3;
    AUX_REGS->mu_mcr = 0;
    AUX_REGS->mu_baud_rate = 270;
    AUX_REGS->mu_control = 3;

    serial_write_byte('\r');
    serial_write_byte('\n');
    serial_write_byte('\n');
}

void serial_write_byte(char byte) {
    while (!(AUX_REGS->mu_lsr & 0x20));

    AUX_REGS->mu_io = byte;
}

void serial_write_str(const char* str) {
    while (*str) {
        if (*str == '\n') {
            serial_write_byte('\r');
        }
        serial_write_byte(*str);
        str++;
    }
}

char serial_read_byte() {
    while (!(AUX_REGS->mu_lsr & 1));

    return AUX_REGS->mu_io & 0xff;
}

void flush() {
    while (AUX_REGS->mu_msr & 0x20) {}
}

StrSlice serial_read_line(char* buf, u32 buf_len) {
    flush();

    u32 input_end;
    for (u32 i = 0; i < buf_len; i++) {
        char c = serial_read_byte();
        if (c == '\r') {
            serial_write_str("\n");
            input_end = i + 1;
            break;
        }
        serial_write_byte(c);
        buf[i] = c;
    }
    return str_slice(buf, 0, input_end);
}
