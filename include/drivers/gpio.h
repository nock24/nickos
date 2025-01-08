#pragma once

#include "basic_types.h"
#include "peripherals/base.h"

#define GPIO_BASE (PBASE + 0x00200000)

typedef struct GpioPinData {
    reg32 reserved;
    reg32 data[2];
} GpioPinData;

typedef struct GpioRegs {
    reg32 func_select[6];
    GpioPinData output_set;
    GpioPinData output_clear;
    GpioPinData level;
    GpioPinData ev_detect_status;
    GpioPinData re_detect_enable;
    GpioPinData fe_detect_enable;
    GpioPinData hi_detect_enable;
    GpioPinData lo_detect_enable;
    GpioPinData async_re_detect;
    GpioPinData async_fe_detect;
    reg32 reserved;
    reg32 pupd_enable;
    reg32 pupd_enable_clocks[2];
} GpioRegs;

#define GPIO_REGS ((GpioRegs*)(GPIO_BASE))

typedef enum GpioFunc {
    GFInput = 0,
    GFOutput = 1,
    GFAlt0 = 4,
    GFAlt1 = 5,
    GFAlt2 = 6,
    GFAlt3 = 7,
    GFAlt4 = 3,
    GFAlt5 = 2,
} GpioFunc;

void gpio_pin_set_func(u8 pin_number, GpioFunc func);
void gpio_pin_enable(u8 pin_number);
