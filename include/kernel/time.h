#pragma once

#include "common.h"
#include "peripherals/base.h"

#define TIME_BASE (PBASE + 0x3000)

u32 get_sys_time_us();
u32 get_sys_time_ms();
void sleep_us(u32 us);
void sleep_ms(u32 ms);

// Assembly function
extern void delay(u64 ticks);
