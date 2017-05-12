/*
 * Copyright (C) 2017 OTA keys S.A.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    boards_stm32f7discovery stm32f769 Discovery board
 * @ingroup     boards
 * @brief       Board specific files for the stm32f769 Discovery board
 * @{
 *
 * @file
 * @brief       Board specific definitions for the stm32f769 Discovery board
 *
 * @author      Vincent Dupont <vincent@otakeys.com>
 */

#ifndef BOARD_H
#define BOARD_H

#include "cpu.h"
#include "periph_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name LED pin definitions
 * @{
 */
/** @} */

/**
 * @name Macros for controlling the on-board LEDs.
 * @{
 */
#define LED0_PIN            GPIO_PIN(PORT_J, 13)
#define LED1_PIN            GPIO_PIN(PORT_J, 5)
#define LED2_PIN            GPIO_PIN(PORT_A, 12)
#define LED3_PIN            GPIO_PIN(PORT_D, 4)

#define LED0_PORT           GPIOJ
#define LED1_PORT           GPIOJ
#define LED2_PORT           GPIOA
#define LED3_PORT           GPIOD
#define LED0_MASK           (1 << 13)
#define LED1_MASK           (1 << 5)
#define LED2_MASK           (1 << 12)
#define LED3_MASK           (1 << 4)

#define LED0_ON             (LED_PORT->BSRR = LED0_MASK)
#define LED0_OFF            (LED_PORT->BSRR = (LED0_MASK << 16))
#define LED0_TOGGLE         (LED_PORT->ODR  ^= LED0_MASK)

#define LED1_ON             (LED_PORT->BSRR = LED1_MASK)
#define LED1_OFF            (LED_PORT->BSRR = (LED1_MASK << 16))
#define LED1_TOGGLE         (LED_PORT->ODR  ^= LED1_MASK)

#define LED2_ON             (LED_PORT->BSRR = LED2_MASK)
#define LED2_OFF            (LED_PORT->BSRR = (LED2_MASK << 16))
#define LED2_TOGGLE         (LED_PORT->ODR  ^= LED2_MASK)

#define LED3_ON             (LED_PORT->BSRR = LED3_MASK)
#define LED3_OFF            (LED_PORT->BSRR = (LED3_MASK << 16))
#define LED3_TOGGLE         (LED_PORT->ODR  ^= LED3_MASK)
/** @} */

/**
 * @brief User button
 */
#define BTN_B1_PIN          GPIO_PIN(PORT_A, 0)

/**
 * @brief Initialize board specific hardware, including clock, LEDs and std-IO
 */
void board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */
/** @} */
