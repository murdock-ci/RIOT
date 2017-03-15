/*
 * Copyright (C) 2017 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @defgroup    boards_nucleo144-f429 Nucleo144-F429
 * @ingroup     boards
 * @brief       Board specific files for the nucleo144-f429 board
 * @{
 *
 * @file
 * @brief       Board specific definitions for the nucleo144-f429 board
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 */

#ifndef BOARD_H
#define BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name xtimer configuration
 * @{
 */
#define XTIMER_DEV          TIMER_DEV(0)
#define XTIMER_CHAN         (0)
#define XTIMER_OVERHEAD     (6)
#define XTIMER_BACKOFF      (5)
/** @} */

/**
 * @brief   LED pin definitions and handlers
 * @{
 */
#define LED0_PIN            GPIO_PIN(PORT_B, 0)
#define LED0_MASK           (1 << 0)
#define LED0_ON             (GPIOA->BSSR = LED0_MASK)
#define LED0_OFF            (GPIOA->BSSR = (LED0_MASK << 16))
#define LED0_TOGGLE         (GPIOA->ODR ^= LED0_MASK)

#define LED1_PIN            GPIO_PIN(PORT_B, 7)
#define LED1_MASK           (1 << 7)
#define LED1_OFF            (GPIOA->BSSR = LED1_MASK)
#define LED1_ON             (GPIOA->BSSR = (LED1_MASK << 16))
#define LED1_TOGGLE         (GPIOA->ODR ^= LED1_MASK)

#define LED2_PIN            GPIO_PIN(PORT_B, 14)
#define LED2_MASK           (1 << 14)
#define LED2_ON             (GPIOA->BSSR = LED2_MASK)
#define LED2_OFF            (GPIOA->BSSR = (LED2_MASK << 16))
#define LED2_TOGGLE         (GPIOA->ODR ^= LED2_MASK)
/** @} */

/**
 * @brief   User button
 */
#define BTN_B1_PIN          GPIO_PIN(PORT_C, 13)

/**
 * @brief   Initialize board specific hardware, including clock, LEDs and std-IO
 */
void board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */
/** @} */
