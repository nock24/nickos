const c = @cImport({
    @cInclude("drivers/uart.h");
});

pub fn init() void {
    c.uart_init();
}

pub fn send(ch: u8) void {
    c.uart_send(ch);
}

pub fn getc() u8 {
    return c.uart_getc();
}

pub fn puts(str: []const u8) void {
    c.uart_puts(@ptrCast(str));
}

pub fn hex(x: u32) void {
    c.uart_hex(x);
}
