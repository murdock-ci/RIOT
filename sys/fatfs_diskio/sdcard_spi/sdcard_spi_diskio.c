/*
 * Copyright (C) 2016 Michel Rottleuthner <michel.rottleuthner@haw-hamburg.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup  sys_fatfs_diskio
 * @{
 *
 * @file
 * @brief    Implementation of fatfs interface that supports sdcard_spi driver
 *           based on low level disk I/O module example for FatFs by ChaN, 2016
 *
 * @author   Michel Rottleuthner <michel.rottleuthner@haw-hamburg.de>
 *
 * @}
 */
#include "fatfs/diskio.h"       /* FatFs lower layer API */
#include "fatfs_diskio_common.h"
#include "fatfs/integer.h"
#include "sdcard_spi.h"

#include <stdio.h>
#include <time.h>
#include "periph_conf.h"
#include "periph/rtc.h"
#include "xtimer.h"
#include "debug.h"

extern sd_card_t cards[NUM_OF_SD_CARDS];

static inline sd_card_t *get_sd_card(int idx)
{
    if (idx < NUM_OF_SD_CARDS) {
        return &(cards[idx]);
    }
    else {
        return NULL;
    }
}

/**
 * @brief           returns the status of the disk
 *
 * @param[in] pdrv  drive number to identify the drive
 *
 * @return          STA_NODISK if no disk exists with the given id
 * @return          0 if disk is initialized
 * @return          STA_NOINIT if disk id exists, but disk isn't initialized
 */
DSTATUS disk_status(BYTE pdrv)
{
    sd_card_t *card = get_sd_card(pdrv);

    if (card == NULL) {
        return STA_NODISK;
    }
    if (card->init_done) {
        return 0;
    }
    else {
        return STA_NOINIT;
    }
}

/**
 * @brief           initializes the disk
 *
 * @param[in] pdrv  drive number to identify the drive
 *
 * @return          STA_NODISK if no disk exists with the given id
 * @return          0 if disk was initialized sucessfully
 * @return          STA_NOINIT if disk id exists, but couldn't be initialized
 */
DSTATUS disk_initialize(BYTE pdrv)
{
    sd_card_t *card = get_sd_card(pdrv);

    if (card == NULL) {
        return STA_NODISK;
    }
    else if (sdcard_spi_init(card)) {
        return 0;
    }
    else {
        return STA_NOINIT;
    }
}

/**
 * @brief              reads sectors from disk
 *
 * @param[in]  pdrv    drive number to identify the drive
 * @param[out] buff    Data buffer to store read data
 * @param[in]  sector  Start sector in LBA
 * @param[in]  count   Number of sectors to read
 *
 * @return             RES_OK if no error occurred
 * @return             RES_NOTRDY if data wasn't read completely
 */
DRESULT disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
    sd_card_t *card = get_sd_card(pdrv);

    if ((card != NULL) && card->init_done) {
        sd_rw_response_t state;
        int read = sdcard_spi_read_blocks(card, sector, (char *)buff,
                                          SD_HC_BLOCK_SIZE, count, &state);
        if (read == count) {
            return RES_OK;
        }
        else {
            printf("disk_read: sdcard_spi_read_blocks: ERROR:%d\n", state);
        }
    }

    return RES_NOTRDY;
}

/**
 * @brief              writes sectors to disk
 *
 * @param[in]  pdrv    Physical drive nmuber to identify the drive
 * @param[in]  buff    Data to be written
 * @param[in]  sector  Start sector in LBA
 * @param[in]  count   Number of sectors to write
 *
 * @return             RES_OK if no error occurred
 * @return             RES_NOTRDY if data wasn't written completely
 */
DRESULT disk_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
    sd_card_t *card = get_sd_card(pdrv);

    if ((card != NULL) && card->init_done) {
        sd_rw_response_t state;
        int written = sdcard_spi_write_blocks(card, sector, (char *)buff,
                                              SD_HC_BLOCK_SIZE, count, &state);
        if (written == count) {
            return RES_OK;
        }
        else {
            printf("disk_write: sdcard_spi_write_blocks: ERROR:%d\n", state);
        }
    }
    return RES_NOTRDY;
}

/**
 * @brief                  perform miscellaneous low-level control functions
 *
 * @param[in]      pdrv    Physical drive nmuber (0..)
 * @param[in out]  cmd     Control code
 * @param[in]      sector  Buffer to send/receive control data
 *
 * @return                 RES_OK if no error occurred
 * @return                 RES_ERROR if an error occurred
 * @return                 RES_PARERR if an error occurred
 */
DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    switch (cmd) {
        #if (_FS_READONLY == 0)
        case CTRL_SYNC:
            /* r/w is always finished within r/w-functions of sdcard_spi */
            return RES_OK;
        #endif

        #if (_USE_MKFS == 1)
        case GET_SECTOR_COUNT:
            sd_card_t card = get_sd_card(pdrv);
            if ((card != NULL) && get_sd_card(pdrv)->init_done) {
                *(DWORD *)buff = sdcard_spi_get_sector_count(card);
                return RES_OK;
            }
            else {
                return RES_ERROR;
            }

        #endif

        #if (_MAX_SS != _MIN_SS)
        case GET_SECTOR_SIZE;
            *buff = SD_HC_BLOCK_SIZE;
            return RES_OK;
        #endif

        #if (_USE_MKFS == 1)
        case GET_BLOCK_SIZE
            *buff = SD_HC_BLOCK_SIZE;
            return RES_OK;
        #endif

        #if (_USE_TRIM == 1)
        case CTRL_TRIM:
            return RES_OK;
        #endif
    }

    return RES_PARERR;
}
