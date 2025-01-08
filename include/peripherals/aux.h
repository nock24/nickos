#pragma once

#include "basic_types.h"
#include "peripherals/base.h"

#define AUX_BASE (PBASE + 0x00215000)

typedef struct AuxRegs {
    reg32 irq_status;
    reg32 enables;
    reg32 reserved[14];
    reg32 mu_io;
    reg32 mu_ier;
    reg32 mu_iir;
    reg32 mu_lcr;
    reg32 mu_mcr;
    reg32 mu_lsr;
    reg32 mu_msr;
    reg32 mu_scratch;
    reg32 mu_control;
    reg32 mu_status;
    reg32 mu_baud_rate;
} AuxRegs;

#define AUX_REGS ((AuxRegs*)(AUX_BASE))
