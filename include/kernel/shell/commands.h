#include "str.h"
#include "result.h"

typedef enum CommandName {
    OsInfo,
    Echo,
} CommandName;

typedef union CommandParams {
    StrSlice echo;
} CommandParams;

typedef struct Command {
    CommandName name;
    CommandParams params;
} Command;

void run_cmd(const Command* cmd);


typedef enum ParseCmdError {
    TooManyArgs,
    NotEnoughArgs,
    InvalidCmd,
    InvalidArg,
} ParseCmdError;
DeclResult(Command, ParseCmdError);

Result(Command, ParseCmdError) parse_cmd(StrSlice str);
