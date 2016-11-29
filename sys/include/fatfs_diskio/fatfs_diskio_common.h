/*
 * Copyright (C) 2016 Michel Rottleuthner <michel.rottleuthner@haw-hamburg.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     sys_fatfs_diskio
 * @brief
 * @{
 *
 * @brief       Common defines for fatfs low-level diskio defines
 * @author      Michel Rottleuthner <michel.rottleuthner@haw-hamburg.de>
 */

#ifndef FATFS_DISKIO_H
#define FATFS_DISKIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Complete pending write process (needed at _FS_READONLY == 0) */
#define CTRL_SYNC           0   

/* Get media size (needed at _USE_MKFS == 1) */
#define GET_SECTOR_COUNT    1   

/* Get sector size (needed at ) */
#define GET_SECTOR_SIZE     2   

/* Get erase block size (needed at _USE_MKFS == 1) */
#define GET_BLOCK_SIZE      3   

/* Inform device that the data on the block of sectors is 
   no longer used (needed at _USE_TRIM == 1) */
#define CTRL_TRIM           4   

#define RTC_YEAR_OFFSET   1900
#define FATFS_YEAR_OFFSET 1980

#define FIXED_BLOCK_SIZE 512

#ifdef __cplusplus
}
#endif

#endif /* FATFS_DISKIO_H */
/** @} */