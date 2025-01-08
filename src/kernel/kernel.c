#include "drivers/serial.h"
#include "kernel/panic.h"
#include "kernel/shell/shell.h"

__attribute__((noreturn)) void kernel_main() {
    serial_init();
    serial_write("Hello world!\n");

    start_shell();
}

__attribute__((noreturn)) void panic(const char* msg) {
    serial_write("[KERNEL PANIC]: ");
    serial_write(msg);
    while (true) {}
}
