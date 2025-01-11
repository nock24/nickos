const c = @cImport({
    @cInclude("drivers/sd.h");
});

fn failed(result: c_int) bool {
    return if (result == c.SD_OK) true else false;
}

pub fn init() bool {
    return failed(c.sd_init());
}

pub fn sd_readblock(lba: u32, buffer: [*]u8, num: u32) bool {
    return failed(c.sd_readblock(lba, @ptrCast(buffer), num));
}

pub fn sd_writeblock(lba: u32, buffer: [*]u8, num: u32) bool {
    return failed(c.sd_writeblock(@ptrCast(buffer), lba, num));
}
