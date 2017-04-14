/*
 * Copyright (C) 2017 Eistec AB
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     cpu_cortexm_common
 * @{
 *
 * @file
 * @brief       Bit access macros for Cortex-M based CPUs
 *
 * @author      Joakim Nohlgård <joakim.nohlgard@eistec.se>
 */

#ifndef BITBAND_H
#define BITBAND_H

#include <stdint.h>
#include "cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !BITBAND_MACROS_PROVIDED

#ifndef CPU_HAS_BITBAND
#if (__CORTEX_M >= 3)
#define CPU_HAS_BITBAND 1
#else
#define CPU_HAS_BITBAND 0
#endif

#if CPU_HAS_BITBAND || DOXYGEN
/* Cortex-M3 and higher provide a bitband address space for atomically accessing
 * single bits of registers (and sometimes RAM) */
/**
 * @name Bit band macros
 * @{
 */
/* Generic bit band conversion routine */
/**
 * @brief Convert bit band region address and bit number to bit band alias address
 *
 * @param[in] addr base address in non bit banded memory
 * @param[in] bit  bit number within the word
 *
 * @return Address of the bit within the bit band memory region
 */
#define BITBAND_ADDR(addr, bit) ((((uint32_t) (addr)) & 0xF0000000u) + 0x2000000 + ((((uint32_t) (addr)) & 0xFFFFF) << 5) + ((bit) << 2))

/**
 * @brief Set a single bit in the 32 bit word pointed to by @p ptr
 *
 * This is the same as:
 *
 * @code{c}
 *   *((uint32_t *)ptr) |= (1 << bit);
 * @endcode
 *
 * There is a read-modify-write cycle occurring within the core, but this cycle
 * is atomic and can not be disrupted by IRQs
 *
 * @param[in]  ptr pointer to target word
 * @param[in]  bit bit number within the word
 */
#define BIT_SET32(ptr, bit) (*((volatile uint32_t *) BITBAND_ADDR((ptr), (bit))) = 1)

/**
 * @brief Set a single bit in the 16 bit word pointed to by @p ptr
 *
 * This is the same as:
 *
 * @code{c}
 *   *((uint16_t *)ptr) |= (1 << bit);
 * @endcode
 *
 * There is a read-modify-write cycle occurring within the core, but this cycle
 * is atomic and can not be disrupted by IRQs
 *
 * @param[in]  ptr pointer to target word
 * @param[in]  bit bit number within the word
 */
#define BIT_SET16(ptr, bit) (*((volatile uint16_t *) BITBAND_ADDR((ptr), (bit))) = 1)

/**
 * @brief Set a single bit in the 8 bit byte pointed to by @p ptr
 *
 * This is the same as:
 *
 * @code{c}
 *   *((uint8_t *)ptr) |= (1 << bit);
 * @endcode
 *
 * There is a read-modify-write cycle occurring within the core, but this cycle
 * is atomic and can not be disrupted by IRQs
 *
 * @param[in]  ptr pointer to target word
 * @param[in]  bit bit number within the word
 */
#define BIT_SET8(ptr, bit) (*((volatile uint8_t *) BITBAND_ADDR((ptr), (bit))) = 1)

/**
 * @brief Clear a single bit in the 32 bit word pointed to by @p ptr
 *
 * This is the same as:
 *
 * @code{c}
 *   *((uint32_t *)ptr) &= ~(1 << bit);
 * @endcode
 *
 * There is a read-modify-write cycle occurring within the core, but this cycle
 * is atomic and can not be disrupted by IRQs
 *
 * @param[in]  ptr pointer to target word
 * @param[in]  bit bit number within the word
 */
#define BIT_CLR32(ptr, bit) (*((volatile uint32_t *) BITBAND_ADDR((ptr), (bit))) = 0)

/**
 * @brief Clear a single bit in the 16 bit word pointed to by @p ptr
 *
 * This is the same as:
 *
 * @code{c}
 *   *((uint16_t *)ptr) &= ~(1 << bit);
 * @endcode
 *
 * There is a read-modify-write cycle occurring within the core, but this cycle
 * is atomic and can not be disrupted by IRQs
 *
 * @param[in]  ptr pointer to target word
 * @param[in]  bit bit number within the word
 */
#define BIT_CLR16(ptr, bit) (*((volatile uint16_t *) BITBAND_ADDR((ptr), (bit))) = 0)

/**
 * @brief Clear a single bit in the 8 bit byte pointed to by @p ptr
 *
 * This is the same as:
 *
 * @code{c}
 *   *((uint8_t *)ptr) &= ~(1 << bit);
 * @endcode
 *
 * There is a read-modify-write cycle occurring within the core, but this cycle
 * is atomic and can not be disrupted by IRQs
 *
 * @param[in]  ptr pointer to target word
 * @param[in]  bit bit number within the word
 */
#define BIT_CLR8(ptr, bit) (*((volatile uint8_t *) BITBAND_ADDR((ptr), (bit))) = 0)

/** @} */

#else /* CPU_HAS_BITBAND */
/* CPU does not have bitbanding, fall back to plain C */
#define BIT_SET32(ptr, bit) (*((volatile uint32_t *)(ptr)) |=  (1 << (bit)))
#define BIT_SET16(ptr, bit) (*((volatile uint16_t *)(ptr)) |=  (1 << (bit)))
#define BIT_SET8(ptr, bit)  (*((volatile  uint8_t *)(ptr)) |=  (1 << (bit)))
#define BIT_CLR32(ptr, bit) (*((volatile uint32_t *)(ptr)) &= ~(1 << (bit)))
#define BIT_CLR16(ptr, bit) (*((volatile uint16_t *)(ptr)) &= ~(1 << (bit)))
#define BIT_CLR8(ptr, bit)  (*((volatile  uint8_t *)(ptr)) &= ~(1 << (bit)))

#endif
#endif /* !HAVE_BITBAND_MACROS */

#endif /* !BITBAND_MACROS_PROVIDED */

#ifdef __cplusplus
}
#endif

#endif /* VECTORS_DEFAULT_H */
/** @} */
