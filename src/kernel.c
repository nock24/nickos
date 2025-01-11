/*
#include "drivers/uart.h"
#include "basic_types.h"
#include "panic.h"
#include "shell/shell.h"
#include "drivers/sd.h"

extern u8 __bss_end__;

__attribute__((noreturn)) void kernel_main() {
    uart_init();
    uart_puts("Hello world!\n");

    u32* counter = (u32*)(&bss_end + 508);

    if(sd_init()==SD_OK) {
        if(sd_readblock(1, &__bss_end__ ,1)) {
            (*counter)++;
            if(sd_writeblock(&__bss_end__, 1 ,1)) {
                uart_puts("Boot counter ");
                uart_hex(*counter);
                uart_puts(" written to SD card.\n");
            }
        }
    }

    start_shell();
}

__attribute__((noreturn)) void panic(const char* msg) {
    uart_puts("[KERNEL PANIC]: ");
    uart_puts(msg);
    while (true) {}
}
*/
