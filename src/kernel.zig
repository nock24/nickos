const serial = @import("drivers/serial.zig");
const sd = @import("drivers/sd.zig");
const Heap = @import("Heap.zig");
const Vec = @import("vec.zig").Vec;

fn handle(err: Heap.Error) noreturn {
    switch (err) {
        Heap.Error.OutOfMemory => kpanic("Out of memory."),
        Heap.Error.OutOfChunks => kpanic("Out of chunks."),
    }
}

export fn kernel_main() noreturn {
    serial.init();
    if (!sd.init()) kpanic("Failed to initialize SD driver.\n");

    serial.send_int(5, 10);
    serial.send_str("\n");

    var chunk_buf: [64]Heap.Chunk = undefined;
    var heap = Heap.init(&chunk_buf);

    var vec = Vec(u32).init(&heap);
    vec.push(1) catch |e| handle(e);
    vec.push(2) catch |e| handle(e);
    vec.push(3) catch |e| handle(e);

    for (vec.items) |item| {
        serial.send_int(item, 10);
        serial.send_str("\n");
    }

    while (true) {
        serial.send_ch(serial.read_ch());
    }
}

pub fn kpanic(msg: []const u8) noreturn {
    serial.send_str("[KERNEL PANIC]: ");
    serial.send_str(msg);
    while (true) {}
}

pub fn kassert(cond: bool) void {
    if (!cond) {
        kpanic("[KERNEL PANIC]: Failed assertion.");
    }
}
