#include "kernel/shell.h"
#include "kernel/panic.h"
#include "drivers/serial.h"
#include "str.h"
#include "mem.h"

typedef enum CommandName {
    CmdName_OsInfo,
    CmdName_Echo,
} CommandName;

typedef union CommandParams {
    const char* echo;
} CommandParams;

typedef struct Command {
    CommandName name;
    CommandParams params;
} Command;

typedef enum ParseCmdError {
    ParseCmdErr_None = 0,
    ParseCmdErr_TooManyArgs,
    ParseCmdErr_NotEnoughArgs,
    ParseCmdErr_InvalidCmd,
} ParseCmdError;

ParseCmdError parse_cmd_args(const StrSlice str, StrSlice* arg_buf, u16 arg_buf_len) {
    if (str.len == 0) {
        return ParseCmdErr_NotEnoughArgs;
    }

    u16 arg_idx = 0;
    u16 arg_start = 0;

    for (u16 i = 0; i < str.len; i++) {
        char c = str.chars[i];

        if (c != ' ' && i != str.len - 1) continue;

        if (c == ' ') {
            if (arg_idx >= arg_buf_len - 1) {
                return ParseCmdErr_TooManyArgs;
            }
            arg_buf[arg_idx] = str_slice(str.chars, arg_start, i);
            arg_idx++;
            arg_start = i + 1;
        } else if (i == str.len - 1) {
            if (arg_idx != arg_buf_len - 1) {
                return ParseCmdErr_NotEnoughArgs;
            }
            arg_buf[arg_idx] = str_slice(str.chars, arg_start, i + 1);
        }
    }

    return ParseCmdErr_None;
}

#define MAX_CMD_NAME_LEN 7

ParseCmdError parse_cmd(Command* cmd, const StrSlice str) {
    char cmd_name_buf[MAX_CMD_NAME_LEN + 1]; // Extra space for null terminator.
    mem_set(cmd_name_buf, MAX_CMD_NAME_LEN + 1, '\0');
    u16 cmd_name_len;
    u16 args_start;
    bool has_args;

    for (u32 i = 0; i < str.len; i++) {
        char c = str.chars[i];

        if (c == ' ') {
            has_args = true;
            cmd_name_len = i;
            args_start = i + 1;
            break;
        }
        if (i == str.len - 1) { // end of command
            has_args = false;
            cmd_name_len = i + 1;
            args_start = i + 1;
            break;
        }

        if (i >= MAX_CMD_NAME_LEN) {
            return ParseCmdErr_InvalidCmd;
        }

        cmd_name_buf[i] = c;
    }

    StrSlice cmd_name = str_slice(cmd_name_buf, 0, cmd_name_len);
    StrSlice args_str;
    if (has_args) {
        args_str = str_slice(str.chars, args_start, str.len);
    } else {
        args_str.chars = "";
        args_str.len = 0;
    }

    if (str_cmp(cmd_name.chars, "os-info")) {
        if (has_args) {
            return ParseCmdErr_TooManyArgs;
        }
        cmd->name = CmdName_OsInfo;
    } else if (str_cmp(cmd_name.chars, "echo")) {
        cmd->name = CmdName_Echo;
        StrSlice args[1];
        ParseCmdError error = parse_cmd_args(args_str, args, 1);
        if (error) return error;
        cmd->params.echo = args[0].chars;
    } else {
        return ParseCmdErr_InvalidCmd;
    }

    return ParseCmdErr_None;
}

void run_cmd(Command cmd) {
    switch (cmd.name) {
        case CmdName_OsInfo:
            serial_write_str("Operating System: NickOS mk1\n");
            break;
        case CmdName_Echo:
            serial_write_str(cmd.params.echo);
            serial_write_str("\n");
            break;
    }
}

__attribute((noreturn)) void start_shell() {
    while (true) {
        serial_write_str("[guest@nickos] -> ");

        char buf[32];
        mem_set(buf, 32, '\0');
        StrSlice input = serial_read_line(buf, 31); // Free space for null terminator
        Command cmd;
        switch (parse_cmd(&cmd, input)) {
            case ParseCmdErr_None:
                run_cmd(cmd);
                break;

            case ParseCmdErr_NotEnoughArgs:
                serial_write_str("Not enough arguments.\n");
                break;
            case ParseCmdErr_TooManyArgs:
                serial_write_str("Too many arguments.\n");
                break;
            case ParseCmdErr_InvalidCmd:
                serial_write_str("Invalid command.\n");
                break;
        }
    }
}
