/*
 * Copyright (C) 2015 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     cpu_samd21
 * @{
 *
 * @file
 * @brief       Implementation of the CPU initialization
 *
 * @author      Thomas Eichinger <thomas.eichinger@fu-berlin.de>
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 * @}
 */

#include "cpu.h"
#include "periph_conf.h"
#include "periph/init.h"

#ifndef VDD_MILLIVOLTS
#define VDD_MILLIVOLTS 3300
#endif

/**
 * @brief   Configure clock sources and the cpu frequency
 */
static void clk_init(void)
{
    /* enable clocks for the power, sysctrl and gclk modules */
    PM->APBAMASK.reg = (PM_APBAMASK_PM | PM_APBAMASK_SYSCTRL |
                        PM_APBAMASK_GCLK);

    /* adjust NVM wait states, see SAMD21 datasheet
       Rev A (2017) table 37-40 , page 816 */
    PM->APBBMASK.reg |= PM_APBBMASK_NVMCTRL;
#if (VDD_MILLIVOLTS > 2700)
#if (CLOCK_CORECLOCK > 24000000)
    NVMCTRL->CTRLB.reg |= NVMCTRL_CTRLB_RWS(1);
#else
    NVMCTRL->CTRLB.reg |= NVMCTRL_CTRLB_RWS(0);
#endif
#else /* VDD_MILLIVOLTS <= 2700 */
#if (CLOCK_CORECLOCK > 42000000)
    NVMCTRL->CTRLB.reg |= NVMCTRL_CTRLB_RWS(3);
#elif (CLOCK_CORECLOCK > 28000000)
    NVMCTRL->CTRLB.reg |= NVMCTRL_CTRLB_RWS(2);
#elif (CLOCK_CORECLOCK > 14000000)
    NVMCTRL->CTRLB.reg |= NVMCTRL_CTRLB_RWS(1);
#else
    NVMCTRL->CTRLB.reg |= NVMCTRL_CTRLB_RWS(0);
#endif
#endif /* VDD_MILLIVOLTS */
    PM->APBBMASK.reg &= ~PM_APBBMASK_NVMCTRL;

    /* configure internal 8MHz oscillator to run without prescaler */
    SYSCTRL->OSC8M.bit.PRESC = 0;
    SYSCTRL->OSC8M.bit.ONDEMAND = 1;
    SYSCTRL->OSC8M.bit.RUNSTDBY = 0;
    SYSCTRL->OSC8M.bit.ENABLE = 1;
    while (!(SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_OSC8MRDY)) {}

#if CLOCK_USE_PLL
    /* reset the GCLK module so it is in a known state */
    GCLK->CTRL.reg = GCLK_CTRL_SWRST;
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) {}

    /* setup generic clock 1 to feed DPLL with 1MHz */
    GCLK->GENDIV.reg = (GCLK_GENDIV_DIV(8) |
                        GCLK_GENDIV_ID(1));
    GCLK->GENCTRL.reg = (GCLK_GENCTRL_GENEN |
                         GCLK_GENCTRL_SRC_OSC8M |
                         GCLK_GENCTRL_ID(1));
    GCLK->CLKCTRL.reg = (GCLK_CLKCTRL_GEN(1) |
                         GCLK_CLKCTRL_ID(1) |
                         GCLK_CLKCTRL_CLKEN);
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) {}

    /* enable PLL */
    SYSCTRL->DPLLRATIO.reg = (SYSCTRL_DPLLRATIO_LDR(CLOCK_PLL_MUL));
    SYSCTRL->DPLLCTRLB.reg = (SYSCTRL_DPLLCTRLB_REFCLK_GCLK);
    SYSCTRL->DPLLCTRLA.reg = (SYSCTRL_DPLLCTRLA_ENABLE);
    while(!(SYSCTRL->DPLLSTATUS.reg &
           (SYSCTRL_DPLLSTATUS_CLKRDY | SYSCTRL_DPLLSTATUS_LOCK))) {}

    /* select the PLL as source for clock generator 0 (CPU core clock) */
    GCLK->GENDIV.reg =  (GCLK_GENDIV_DIV(CLOCK_PLL_DIV) |
                        GCLK_GENDIV_ID(0));
    GCLK->GENCTRL.reg = (GCLK_GENCTRL_GENEN |
                         GCLK_GENCTRL_SRC_FDPLL |
                         GCLK_GENCTRL_ID(0));
#else /* do not use PLL, use internal 8MHz oscillator directly */
    GCLK->GENDIV.reg =  (GCLK_GENDIV_DIV(CLOCK_DIV) |
                        GCLK_GENDIV_ID(0));
    GCLK->GENCTRL.reg = (GCLK_GENCTRL_GENEN |
                         GCLK_GENCTRL_SRC_OSC8M |
                         GCLK_GENCTRL_ID(0));
#endif

    /* make sure we synchronize clock generator 0 before we go on */
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) {}

    /* Setup Clock generator 2 with divider 1 (32.768kHz) */
    GCLK->GENDIV.reg  = (GCLK_GENDIV_ID(2)  | GCLK_GENDIV_DIV(0));
    GCLK->GENCTRL.reg = (GCLK_GENCTRL_ID(2) | GCLK_GENCTRL_GENEN |
            GCLK_GENCTRL_RUNSTDBY |
            GCLK_GENCTRL_SRC_OSCULP32K);

    while (GCLK->STATUS.bit.SYNCBUSY) {}

    /* redirect all peripherals to a disabled clock generator (7) by default */
    for (int i = 0x3; i <= 0x22; i++) {
        GCLK->CLKCTRL.reg = ( GCLK_CLKCTRL_ID(i) | GCLK_CLKCTRL_GEN_GCLK7 );
        while (GCLK->STATUS.bit.SYNCBUSY) {}
    }
}

void cpu_init(void)
{
    /* disable the watchdog timer */
    WDT->CTRL.bit.ENABLE = 0;
    /* initialize the Cortex-M core */
    cortexm_init();
    /* Initialise clock sources and generic clocks */
    clk_init();
    /* trigger static peripheral initialization */
    periph_init();
}
