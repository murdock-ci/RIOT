/*
 * Copyright (C) 2016 Michel Rottleuthner <michel.rottleuthner@haw-hamburg.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup  tests
 * @{
 *
 * @file
 * @brief    Implementation of common FatFs interface for native and sdcard_spi
 *           Based on low level disk I/O module example for FatFs by ChaN, 2016
 *
 * @author   Michel Rottleuthner <michel.rottleuthner@haw-hamburg.de>
 *
 * @}
 */

#include "fatfs/diskio.h" /* FatFs lower layer API */
#include "fatfs_diskio_common.h"

DWORD get_fattime (void)
{
    struct tm time;

    rtc_get_time(&time);

    /* bit 31:25 Year origin from 1980 (0..127) */
    uint8_t year = time.tm_year + RTC_YEAR_OFFSET - FATFS_YEAR_OFFSET; 
    uint8_t month = time.tm_mon + 1;        /* bit 24:21 month (1..12) */
    uint8_t day_of_month = time.tm_mon + 1; /* bit 20:16 day (1..31) */
    uint8_t hour = time.tm_hour;            /* bit 15:11 hour (0..23) */
    uint8_t minute = time.tm_min;           /* bit 10:5 minute (0..59) */
    uint8_t second = (time.tm_sec / 2);     /* bit 4:0 second/2 (0..29) */

    return year << 25 | month << 21 | day_of_month << 16 | 
           hour << 11 | minute << 5 | second; 
}