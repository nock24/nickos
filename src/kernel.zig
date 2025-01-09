const uart = @import("drivers/uart.zig");

export fn kernel_main() noreturn {
    uart.init();
    uart.puts("hello\n");

    while (true) {}
}
