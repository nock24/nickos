const Self = @This();
const BufVec = @import("buf_vec.zig").BufVec;
const serial = @import("drivers/serial.zig");

pub const Error = error{
    OutOfMemory,
    OutOfChunks,
};

extern const __heap_start__: u8;
extern const __heap_end__: u8;

fn linkerPtr(x: u8) [*]u8 {
    return @ptrCast(@constCast(&x));
}

pub const Chunk = struct {
    addr: usize,
    size: usize,
    free: bool,
};

buffer: []u8,
cur_addr: usize = 0,
chunks: BufVec(Chunk),

pub fn init(chunk_buf: []Chunk) Self {
    const heap_start = linkerPtr(__heap_start__);
    const heap_end = linkerPtr(__heap_end__);
    const heap_size = @intFromPtr(heap_end) - @intFromPtr(heap_start);

    serial.send_str("Heap start address: ");
    serial.send_int(@intFromPtr(heap_start), 16);
    serial.send_str("\n");
    serial.send_str("Heap end address: ");
    serial.send_int(@intFromPtr(heap_end), 16);
    serial.send_str("\n");
    serial.send_str("Heap size: ");
    serial.send_int(heap_size, 16);
    serial.send_str("\n");

    return .{
        .buffer = heap_start[0..heap_size],
        .chunks = BufVec(Chunk).init(chunk_buf),
    };
}

pub fn allocSize(self: *Self, size: usize) Error!*u8 {
    var opt_ptr: ?*u8 = null;
    for (self.chunks.itemsMut()) |*chunk| { 
        if (!chunk.free or chunk.size < size) continue;

        self.chunks.push(Chunk{ 
            .addr = chunk.addr + size,
            .size = chunk.size - size,
            .free = true,
        }) catch return Error.OutOfChunks;

        chunk.size = size;
        chunk.free = false;
        opt_ptr = &self.buffer[chunk.addr];
    }
    if (opt_ptr) |ptr| {
        return ptr;
    }

    //serial.send_str("Current addr: ");
    //serial.send_hex(@intCast(self.cur_addr));
    //serial.send_str("\nBuffer length: ");
    //serial.send_hex(@intCast(self.buffer.len));
    //serial.send_str("\n");
    if (self.cur_addr + size > self.buffer.len)
        return Error.OutOfMemory;

    const ptr = &self.buffer[self.cur_addr];
    self.chunks.push(Chunk{
        .addr = self.cur_addr,
        .size = size,
        .free = false,
    }) catch return Error.OutOfChunks;
    self.cur_addr += 1;

    return ptr;
}

pub fn alloc(self: *Self, comptime T: type) Error!*T {
    return @ptrCast(try self.allocSize(@sizeOf(T)));
}

pub fn allocArr(self: *Self, comptime T: type, len: usize) Error![]T {
    const ptr: [*]T = @alignCast(@ptrCast(try self.allocSize(@sizeOf(T) * len)));
    return ptr[0..len];
}

fn defrag(self: *Self) void {
    var i: usize = 0;
    while (i < self.chunks.len - 1) {
        const chunk = self.chunks.getMut(i).?;
        const next_chunk = self.chunks.get(i+1).?;
        
        if (!chunk.free or !next_chunk.free) {
            i += 1;
        } else {
            chunk.size += next_chunk.size;
            self.chunks.remove(i+1);
        }
    }
}

pub fn free(self: *Self, pointer: anytype) void {
    const ptr: *u8 = @ptrCast(pointer); 

    for (self.chunks.itemsMut()) |*chunk| {
        if (&self.buffer[chunk.addr] == ptr) {
            chunk.free = true;
            break;
        }
    }

    self.defrag();
}
