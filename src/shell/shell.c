#include "shell/shell.h"
#include "shell/commands.h"
#include "panic.h"
#include "drivers/uart.h"
#include "serial.h"
#include "str.h"
#include "optional.h"
#include "mem.h"
#include "result.h"

__attribute((noreturn)) void start_shell() {
    while (true) {
        uart_puts("[guest@nickos] -> ");

        char buf[32];
        mem_set(buf, 32, '\0');
        ConstStr input = serial_read_line(buf, 31); // Free space for null terminator

        Result(Command, ParseCmdError) cmd_result = parse_cmd(input);
        if (cmd_result.ok) {
            run_cmd(&as_value(cmd_result));
        } else switch (as_error(cmd_result)) {
            case NotEnoughArgs:
                uart_puts("Not enough arguments.\n");
                break;
            case TooManyArgs:
                uart_puts("Too many arguments.\n");
                break;
            case InvalidCmd:
                uart_puts("Invalid command.\n");
                break;
            case InvalidArg:
                uart_puts("Invalid argument.\n");
                break;
        }
    }
}
