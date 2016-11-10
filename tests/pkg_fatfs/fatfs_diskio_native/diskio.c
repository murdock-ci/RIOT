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
 * @brief    Implementation of FatFs interface that makes use of a fatfs image 
 *           file instead of hardware to allow FatFs usage/testing on native.
 *           Based on low level disk I/O module example for FatFs by ChaN, 2016
 *
 * @author   Michel Rottleuthner <michel.rottleuthner@haw-hamburg.de>
 *
 * @}
 */
#define ENABLE_DEBUG    (1)
#include "debug.h"
#include "fatfs/diskio.h"		/* FatFs lower layer API */
#include "fatfs/integer.h"

#include <stdio.h>
#include <time.h>
#include "periph_conf.h"
#include "periph/rtc.h"
#include "xtimer.h"
#include <stdlib.h>
#include <errno.h>

/* Complete pending write process (needed at _FS_READONLY == 0) */
#define CTRL_SYNC			0	

/* Get media size (needed at _USE_MKFS == 1) */
#define GET_SECTOR_COUNT	1	

/* Get sector size (needed at ) */
#define GET_SECTOR_SIZE		2	

/* Get erase block size (needed at _USE_MKFS == 1) */
#define GET_BLOCK_SIZE		3	

/* Inform device that the data on the block of sectors is 
   no longer used (needed at _USE_TRIM == 1) */
#define CTRL_TRIM			4	

#define RTC_YEAR_OFFSET   1900
#define FATFS_YEAR_OFFSET 1980

#define FIXED_BLOCK_SIZE 512

bool rtc_init_done = false;

typedef struct {
    const char *image_path;
	FILE *fd;
	bool opened;
} dummy_volume_t;

static dummy_volume_t volume_files[] = {
    {
        .image_path = "../../riot_fatfs_disk.img",
        .fd = NULL,
        .opened  = false
    },
};

static inline dummy_volume_t *get_volume_file(uint32_t idx)
{
	if(idx < sizeof(volume_files) / sizeof(dummy_volume_t)){
		return &volume_files[idx];
	}else{
		return NULL;
	}
}

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

/**
 * @brief           returns the status of the disk
 *
 * @param[in] pdrv  drive number to identify the drive
 *
 * @return          STA_NODISK if no disk exists with the given id
 * @return          0 if disk is initialized
 * @return          STA_NOINIT if disk id exists, but disk isn't initialized 
 */
DSTATUS disk_status (BYTE pdrv)
{
	dummy_volume_t *volume = get_volume_file(pdrv);

	if(volume == NULL){
		return STA_NODISK;
	}
	if(volume->opened){
		return 0;
	}else{
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
DSTATUS disk_initialize (BYTE pdrv)
{
	dummy_volume_t *volume = get_volume_file(pdrv);
	
	if(volume == NULL){
		return STA_NODISK;
	}else if(!volume->opened){
		/* open file for r/w but don't create if it doesn't exist */
		FILE *fd = fopen(volume->image_path, "r+"); 
		DEBUG("fd: %p\n", (void*)fd);
		if(fd == NULL){
			DEBUG("diskio_native.c: disk_initialize: fopen: \
				   errno: 0x%08x\n", errno);
			return STA_NOINIT;
		}else{
			volume->fd = fd;
			volume->opened = true;
			return 0;
		}
	}else{
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
DRESULT disk_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
	dummy_volume_t *volume = get_volume_file(pdrv);

	if(volume != NULL && volume->opened){
		/* set read pointer to secor aquivalent position */
		if(fseek(volume->fd, sector * FIXED_BLOCK_SIZE, SEEK_SET) == 0){
			if(fread(buff, FIXED_BLOCK_SIZE, count, volume->fd) == count){
				return RES_OK;
			}else{
				DEBUG("diskio_native.c: disk_read: fread: \
					   errno: 0x%08x\n", errno);
			}
		}else{
			DEBUG("diskio_native.c: disk_read: fseek: errno: 0x%08x\n", errno);
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
DRESULT disk_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
	dummy_volume_t *volume = get_volume_file(pdrv);

	if(volume != NULL && volume->opened){
		//TODO: f_lseek, f_write, f_flush;
		/* set write pointer to secor aquivalent position */
		if(fseek(volume->fd, sector * FIXED_BLOCK_SIZE, SEEK_SET) == 0){
			if(fwrite(buff, FIXED_BLOCK_SIZE, count, volume->fd) == count){
				if(fflush(volume->fd) == 0){
					return RES_OK;
				}else{
					DEBUG("diskio_native.c: disk_write: fflush: \
						   errno: 0x%08x\n", errno);	
				}
			}else{
				DEBUG("diskio_native.c: disk_write: fwrite: \
					   errno: 0x%08x\n", errno);
			}
		}else{
			DEBUG("diskio_native.c: disk_write: fseek: errno: 0x%08x\n", errno);
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
DRESULT disk_ioctl (
	BYTE pdrv,		/*  */
	BYTE cmd,		/*  */
	void *buff		/* Buffer to send/receive control data */
)
{
	(void) pdrv; /* prevent warning about unused param */
    (void) buff; /* prevent warning about unused param */

	switch(cmd){
		#if(_FS_READONLY == 0)
		case CTRL_SYNC: 
			/* r/w is always finished within r/w-functions of sdcard_spi */
			return RES_OK;
		#endif

		#if(_USE_MKFS == 1)
		case GET_SECTOR_COUNT:

			dummy_volume_t *volume = get_volume_file(pdrv);

			if(volume != NULL && volume->opened){
				struct stat s; 
    			if (stat(volume->image_path, &s) == 0){
        			*(DWORD*)buff = s.st_size / FIXED_BLOCK_SIZE;
        			return RES_OK;
    			}
			}
			return RES_ERROR;
			
        #endif

        #if(_MAX_SS != _MIN_SS)
        case GET_SECTOR_SIZE;
        	*buff = FIXED_BLOCK_SIZE;
        	return RES_OK;
    	#endif

        #if(_USE_MKFS == 1)
    	case GET_BLOCK_SIZE
			*buff = FIXED_BLOCK_SIZE;
        	return RES_OK;   
    	#endif

        #if(_USE_TRIM == 1)
    	case CTRL_TRIM:
    		return RES_OK;
    	#endif
	}

	return RES_PARERR;
}

