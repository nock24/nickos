#include "kernel/shell/shell.h"
#include "kernel/shell/commands.h"
#include "kernel/panic.h"
#include "drivers/serial.h"
#include "str.h"
#include "optional.h"
#include "mem.h"
#include "result.h"

__attribute((noreturn)) void start_shell() {
    while (true) {
        serial_write("[guest@nickos] -> ");

        char buf[32];
        mem_set(buf, 32, '\0');
        ConstStr input = serial_read_line(buf, 31); // Free space for null terminator

        Result(Command, ParseCmdError) cmd_result = parse_cmd(input);
        if (cmd_result.ok) {
            run_cmd(&as_value(cmd_result));
        } else switch (as_error(cmd_result)) {
            case NotEnoughArgs:
                serial_write("Not enough arguments.\n");
                break;
            case TooManyArgs:
                serial_write("Too many arguments.\n");
                break;
            case InvalidCmd:
                serial_write("Invalid command.\n");
                break;
            case InvalidArg:
                serial_write("Invalid argument.\n");
                break;
        }
    }
}
