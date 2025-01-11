const Heap = @import("Heap.zig");
const serial = @import("drivers/serial.zig");

pub fn Vec(comptime T: type) type {
    return struct {
        const Self = @This();

        items: []T,
        capacity: usize,
        heap: *Heap,

        pub fn init(heap: *Heap) Self {
            return .{
                .items = &.{},
                .capacity = 0,
                .heap = heap,
            };
        }

        pub fn withCapacity(heap: *Heap, capacity: usize) Heap.Error!Self {
            const ptr = (try heap.allocArr(T, capacity)).ptr;
            return .{
                .items = .{ .ptr = ptr, .len = 0 },
                .capacity = capacity,
                .heap = heap,
            };
        }

        pub fn push(self: *Self, item: T) Heap.Error!void {
            //serial.send_str("len: ");
            //serial.send_hex(@intCast(self.items.len));
            //serial.send_str("\ncapacity: ");
            //serial.send_hex(@intCast(self.capacity));
            //serial.send_str("\n");

            if (self.items.len == self.capacity) {
                if (self.capacity == 0) {
                    self.capacity = 1;
                } else {
                    self.heap.free(self.items.ptr);
                }
                self.capacity *= 2;
                const ptr = (try self.heap.allocArr(T, self.capacity)).ptr;
                self.items = ptr[0 .. self.items.len + 1];
            } else {
                self.items.len += 1;
                self.items[self.items.len - 1] = item;
            }
        }
    };
}
