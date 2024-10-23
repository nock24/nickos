#include "drivers/serial.h"
#include "kernel/panic.h"
#include "common.h"
#include "kernel/shell/shell.h"

__attribute__((noreturn)) void kernel_main() {
    serial_init();
    serial_write_str("Hello world!\n");

    start_shell();
}

__attribute__((noreturn)) void panic(char* msg) {
    serial_write_str("[KERNEL PANIC]: ");
    serial_write_str(msg);
    while (true) {}
}
