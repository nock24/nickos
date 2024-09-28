#include "kernel/time.h"

u32 get_sys_time_us() {
    const reg32* sys_time_us = (reg32*)(TIME_BASE + 0x4);
    return *sys_time_us;
}

u32 get_sys_time_ms() {
    return get_sys_time_us() / 1000;
}

void sleep_us(u32 us) {
    const u32 start_time = get_sys_time_us();
    while (get_sys_time_us() - start_time < us) {}
}

void sleep_ms(u32 ms) {
    sleep_us(ms * 1000);
}
