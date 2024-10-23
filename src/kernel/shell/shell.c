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
        serial_write_str("[guest@nickos] -> ");

        char buf[32];
        mem_set(buf, 32, '\0');
        StrSlice input = serial_read_line(buf, 31); // Free space for null terminator
        Result(Command, ParseCmdError) cmd_result = parse_cmd(input);
        if (cmd_result.ok) {
            run_cmd(&as_value(cmd_result));
            continue;
        }
        switch (as_error(cmd_result)) {
            case NotEnoughArgs:
                serial_write_str("Not enough arguments.\n");
                break;
            case TooManyArgs:
                serial_write_str("Too many arguments.\n");
                break;
            case InvalidCmd:
                serial_write_str("Invalid command.\n");
                break;
            case InvalidArg:
                serial_write_str("Invalid argument.\n");
                break;
        }
    }
}
