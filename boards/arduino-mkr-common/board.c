/*
 * Copyright (C)  2017 Inria
 *                2017 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_arduino-mkr-common
 *
 * @file
 * @brief       Board common implementations for the Arduino MKR boards
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
  *@author      Hauke Pertersen  <hauke.pertersen@fu-berlin.de>
 * @}
 */

#include "cpu.h"
#include "board.h"
#include "periph/gpio.h"

void board_init(void)
{
    /* initialize the CPU */
    cpu_init();
    /* initialize the on-board Amber "L" LED */
    gpio_init(LED0_PIN, GPIO_OUT);
}
