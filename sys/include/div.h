/*
 * Copyright (C) 2015 Kaspar Schleiser <kaspar@schleiser.de>
 * Copyright (C) 2016 Eistec AB
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @brief     Integer division functions
 *
 * This header provides some integer division functions that can be used
 * to prevent linking in compiler-generated ones, which are often larger.
 *
 * @file
 * @ingroup   sys_util
 * @author    Kaspar Schleiser <kaspar@schleiser.de>
 * @author    Joakim Nohlgård <joakim.nohlgard@eistec.se>
 * @{
 */

#ifndef DIV_H
#define DIV_H

#include <assert.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Approximation of (2**l)/d for d=15625, l=12, 32 bits
 */
#define DIV_H_INV_15625_32     0x431bde83ul

/**
 * @brief Approximation of (2**l)/d for d=15625, l=12, 64 bits
 */
#define DIV_H_INV_15625_64     0x431bde82d7b634dbull

/**
 * @brief Approximation of (2**l)/d for d=625, l=8, 32 bits
 */
#define DIV_H_INV_625_32       0x68db8badul

/**
 * @brief Approximation of (2**l)/d for d=625, l=8, 64 bits
 */
#define DIV_H_INV_625_64       0x68db8bac710cb296ull

/**
 * @brief Approximation of (2**l)/d for d=576, l=0, 32 bits
 */
#define DIV_H_INV_576_32       0x71c71dul

/**
 * @brief Approximation of (2**l)/d for d=576, l=0, 64 bits
 */
#define DIV_H_INV_576_64       0x71c71bff8e38e5ull

/**
 * @brief Required shifts for division by 15625, l above
 */
#define DIV_H_INV_15625_SHIFT  12

/**
 * @brief Required shifts for division by 625, l above
 */
#define DIV_H_INV_625_SHIFT    8

/**
 * @brief Operand for 576/625
 */
#define MUL_576                576ul

/**
 * @brief Operand for 625/576
 */
#define MUL_625                625ul

/**
 * @internal
 * @brief Multiply two 64 bit integers into a 128 bit integer and return the upper half.
 *
 * The implementation only uses 64 bit integers internally, no __int128 support
 * is necessary.
 *
 * @see http://stackoverflow.com/questions/28868367/getting-the-high-part-of-64-bit-integer-multiplication

 * @param[in]   a     operand a
 * @param[in]   b     operand b
 * @return      (((uint128_t)a * b) >> 64)
 */
uint64_t _div_mulhi64(const uint64_t a, const uint64_t b);

/**
 * @brief Integer divide val by 15625, 64 bit version
 *
 * @param[in]   val     dividend
 * @return      (val / 15625)
 */
static inline uint64_t div_u64_by_15625(uint64_t val)
{
    if (val > 16383999997ull) {
        return (_div_mulhi64(DIV_H_INV_15625_64, val) >> DIV_H_INV_15625_SHIFT);
    }
    return (val * DIV_H_INV_15625_32) >> (DIV_H_INV_15625_SHIFT + 32);
}

/**
 * @brief Integer divide val by 625, 64 bit version
 *
 * @param[in]   val     dividend
 * @return      (val / 625)
 */
static inline uint64_t div_u64_by_625(uint64_t val)
{
    if (val > 10485759996ull) {
        return (_div_mulhi64(DIV_H_INV_625_64, val) >> DIV_H_INV_625_SHIFT);
    }
    return (val * DIV_H_INV_625_32) >> (DIV_H_INV_625_SHIFT + 32);
}

/**
 * @brief Integer divide val by 1000000
 *
 * @param[in]   val     dividend
 * @return      (val / 1000000)
 */
static inline uint64_t div_u64_by_1000000(uint64_t val)
{
    return div_u64_by_15625(val) >> 6;
}

/**
 * @brief Divide val by (15625/512)
 *
 * This is used to quantize a 1MHz value to the closest 32768Hz value,
 * e.g., for timers.
 *
 * The algorithm uses the modular multiplicative inverse of 15625 to use only
 * multiplication and bit shifts to perform the division.
 *
 * The result will be equal to the mathematical expression: floor((val * 512) / 15625)
 *
 * @param[in]   val     dividend
 * @return      (val / (15625/512))
 */
static inline uint32_t div_u32_by_15625div512(uint32_t val)
{
    return ((uint64_t)(val) * DIV_H_INV_15625_32) >> (DIV_H_INV_15625_SHIFT + 32 - 9);
}

/**
 * @brief Divide val by (625/576)
 *
 * This is used to quantize a 1MHz value to the closest 921600Hz value,
 * e.g., for timers.
 *
 * The algorithm uses the modular multiplicative inverse of 625 to use only
 * multiplication and bit shifts to perform the division.
 *
 * The result will be equal to the mathematical expression: floor((val * 576) / 625)
 *
 * @param[in]   val     dividend
 * @return      (val / (625/576))
 */
static inline uint32_t div_u32_by_625div576(uint32_t val)
{
    return ((uint64_t)(val) * DIV_H_INV_625_32 * MUL_576) >> (DIV_H_INV_625_SHIFT + 32);
}

/**
 * @brief Divide val by (15625/512)
 *
 * This is used to quantize a 1MHz value to the closest 32768Hz value,
 * e.g., for timers.
 *
 * @param[in]   val     dividend
 * @return      (val / (15625/512))
 */
static inline uint64_t div_u64_by_15625div512(uint64_t val)
{
    /*
     * This saves around 1400 bytes of ROM on Cortex-M platforms (both ARMv6 and
     * ARMv7) from avoiding linking against __aeabi_uldivmod and related helpers
     */
    if (val > 16383999997ull) {
        /* this would overflow 2^64 in the multiplication that follows, need to
         * use the long version */
        return (_div_mulhi64(DIV_H_INV_15625_64, val) >> (DIV_H_INV_15625_SHIFT - 9));
    }
    return (val * DIV_H_INV_15625_32) >> (DIV_H_INV_15625_SHIFT + 32 - 9);
}

/**
 * @brief Divide val by (625/576)
 *
 * This is used to quantize a 1MHz value to the closest 921600Hz value,
 * e.g., for timers.
 *
 * @param[in]   val     dividend
 * @return      (val / (625/576))
 */
static inline uint64_t div_u64_by_625div576(uint64_t val)
{
    /*
     * This saves around 1400 bytes of ROM on Cortex-M platforms (both ARMv6 and
     * ARMv7) from avoiding linking against __aeabi_uldivmod and related helpers
     */
    if (val > 10485759996ull) {
        /* this would overflow 2^64 in the multiplication that follows, need to
         * use the long version */
        return (_div_mulhi64(DIV_H_INV_625_64, val) * 576 >> (DIV_H_INV_625_SHIFT));
    }

    return (val * DIV_H_INV_625_32) * MUL_576 >> (DIV_H_INV_625_SHIFT + 32);
}

/**
 * @brief Divide val by (576/625)
 *
 * This is used to quantize a ticks value to the closest 921600 value,
 * e.g., for timers.
 *
 * @param[in]   val     dividend
 * @return      (val / (576/625))
 */
static inline uint32_t div_u64_by_576div625(uint64_t val)
{
    /*
     * This saves around 1400 bytes of ROM on Cortex-M platforms (both ARMv6 and
     * ARMv7) from avoiding linking against __aeabi_uldivmod and related helpers
     */
    if (val > 2473900978176ull) {
        /* this would overflow 2^64 in the multiplication that follows, need to
         * use the long version */
        return (_div_mulhi64(DIV_H_INV_576_64, val) * MUL_625);
    }

    return ((uint64_t)(val) * DIV_H_INV_576_32 * MUL_625) >> 32;
}

/**
 * @brief Divide val by (576/625)
 *
 * This is used to quantize a ticks value to the closest 921600 value,
 * e.g., for timers.
 *
 * The algorithm uses the modular multiplicative inverse of 576 to use only
 * multiplication and bit shifts to perform the division.
 *
 * The result will be equal to the mathematical expression: floor((val * 625) / 576)
 *
 * @param[in]   val     dividend
 * @return      (val / (576/625))
 */
static inline uint32_t div_u32_by_576div625(uint32_t val)
{
    return ((uint64_t)(val) * DIV_H_INV_576_32 * MUL_625) >> 32;
}

/**
 * @brief Integer divide val by 44488
 *
 * @param[in]   val     dividend
 * @return      (val / 44488)
 */
static inline uint32_t div_u32_by_44488(uint32_t val)
{
    return ((uint64_t)val * 0xBC8F1391UL) >> (15 + 32);
}

/**
 * @brief Modulo 44488
 *
 * @param[in]   val     dividend
 * @return      (val % 44488)
 */
static inline uint32_t div_u32_mod_44488(uint32_t val)
{
    return val - (div_u32_by_44488(val)*44488);
}

#ifdef __cplusplus
}
#endif
/** @} */
#endif /* DIV_H */
