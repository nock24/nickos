const std = @import("std");
const fmt = std.fmt;
const math = std.math;

const c = @cImport({
    @cInclude("drivers/uart.h");
});

pub fn init() void {
    c.uart_init();
}

pub fn send_ch(ch: u8) void {
    c.uart_send(ch);
}

pub fn read_ch() u8 {
    return c.uart_getc();
}

pub fn send_str(str: []const u8) void {
    c.uart_puts(@ptrCast(str));
}

pub fn send_int(n: anytype, base: u8) void {
    const IntType = if (@TypeOf(n) == comptime_int)
        math.IntFittingRange(n, n)
    else
        @TypeOf(n);
    const max_digits = @typeInfo(IntType).int.bits;
    var buf: [max_digits]u8 = undefined;
    const digits = fmt.formatIntBuf(&buf, n, base, .upper, .{});
    for (0..digits) |i| {
        send_ch(buf[i]);
    }
}
