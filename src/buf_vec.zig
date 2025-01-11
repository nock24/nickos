/// A dynamically sized array which grows within a preallocated buffer.
pub fn BufVec(comptime T: type) type {
    return struct {
        const Self = @This();

        buffer: []T,
        len: usize,

        pub fn init(buffer: []T) Self {
            return .{
                .buffer = buffer,
                .len = 0,
            };
        }

        pub fn push(self: *Self, item: T) error{OutOfMemory}!void {
            if (self.len >= self.buffer.len)
                return error.OutOfMemory;

            self.buffer[self.len] = item;
            self.len += 1;
        }

        pub fn get(self: *const Self, idx: usize) ?*const T {
            if (idx >= self.len) return null;
            return &self.buffer[idx];
        }

        pub fn getMut(self: *Self, idx: usize) ?*T {
            if (idx >= self.len) return null;
            return &self.buffer[idx];
        }

        pub fn items(self: *const Self) []const T {
            return self.buffer[0..self.len];
        }

        pub fn itemsMut(self: *Self) []T {
            return self.buffer[0..self.len];
        }

        pub fn remove(self: *Self, idx: usize) void {
            for (idx .. self.len - 1) |i| {
                self.getMut(i).?.* = self.get(i+1).?.*;
            }
            self.getMut(self.len - 1).?.* = undefined;
            self.len -= 1;
        }
    };
}
