#include "shell/commands.h"
#include "drivers/uart.h"
#include "mem.h"

void os_info() {
    uart_puts("Operating System: NickOS mk1\n");
}

void echo(const CommandParams* params) {
    ConstStr str = params->echo;
    uart_puts(str.chars);
    uart_puts("\n");
}

void run_cmd(const Command* cmd) {
    switch (cmd->name) {
        case OsInfo:
            os_info();
            break;
        case Echo:
            echo(&cmd->params);
            break;
    }
}

typedef struct CommandStr {
    ConstStr str;
    size_t idx;
} CommandStr;

// Returns if the character was there.
bool try_skip_char(CommandStr* cmd_str, char expected) {
    if (cmd_str->idx >= cmd_str->str.len - 1
        || cmd_str->str.chars[cmd_str->idx] != expected)
    {
        return false;
    }
    cmd_str->idx++;
    return true;
}

bool more_args(CommandStr* cmd_str) {
    size_t* i = &cmd_str->idx;
    ConstStr str = cmd_str->str;

    while (*i < str.len && str.chars[*i] == ' ') {
        (*i)++;
    }

    return *i < str.len;
}

DeclResult(ConstStr, ParseCmdError);

Result(ConstStr, ParseCmdError) parse_cmd_arg(CommandStr* cmd_str, char edge_char) {
    size_t* i = &cmd_str->idx;
    ConstStr str = cmd_str->str;

    while (*i < str.len && str.chars[*i] == ' ') {
        (*i)++;
    }

    if (*i >= str.len) {
        return Err(ConstStr, ParseCmdError, NotEnoughArgs);
    }
    if (edge_char != ' ') {
        if (str.chars[*i] != edge_char) {
            return Err(ConstStr, ParseCmdError, InvalidArg);
        }
        (*i)++;
    }

    size_t arg_start = *i;
    size_t arg_end;
    while (*i < str.len) {
        if (str.chars[*i] == edge_char) {
            arg_end = *i;
            (*i)++;
            break;
        } else if (*i == str.len - 1) {
            if (edge_char != ' ') {
                return Err(ConstStr, ParseCmdError, InvalidArg);
            }

            (*i)++;
            arg_end = *i;
            break;
        }
        (*i)++;
    }

    return Ok(ConstStr, ParseCmdError, str_slice(str.chars, arg_start, arg_end));
}

#define MAX_CMD_NAME_LEN 7

Result(Command, ParseCmdError) parse_cmd(ConstStr str) {
    Command cmd;
    CommandStr cmd_str = {
        .str = str,
        .idx = 0,
    };
    char cmd_name_buf[MAX_CMD_NAME_LEN + 1]; // Extra space for null terminator.
    mem_set(cmd_name_buf, MAX_CMD_NAME_LEN + 1, '\0');
    size_t args_start;
    bool has_args;

    for (size_t i = 0; i < str.len; i++) {
        char c = str.chars[i];

        if (c == ' ') {
            has_args = true;
            args_start = i + 1;
            break;
        }

        if (i == str.len - 1) { // end of command
            has_args = false;
            args_start = i + 1;
        }

        if (i >= MAX_CMD_NAME_LEN) {
            return Err(Command, ParseCmdError, InvalidCmd);
        }

        cmd_name_buf[i] = c;
    }

    cmd_str.idx = args_start;

    if (str_cmp(cmd_name_buf, "os-info")) {
        cmd.name = OsInfo;

        if (has_args) {
            return Err(Command, ParseCmdError, TooManyArgs);
        }
    } else if (str_cmp(cmd_name_buf, "echo")) {
        cmd.name = Echo;

        Result(ConstStr, ParseCmdError) arg_result = parse_cmd_arg(&cmd_str, '"');
        if (arg_result.ok) {
            cmd.params.echo = as_value(arg_result);
        } else {
            return Err(Command, ParseCmdError, as_error(arg_result));
        }

        if (more_args(&cmd_str)) {
            return Err(Command, ParseCmdError, TooManyArgs);
        }
    } else {
        return Err(Command, ParseCmdError, InvalidCmd);
    }

    return Ok(Command, ParseCmdError, cmd);
}
