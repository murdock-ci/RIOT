MODULE = fatfs_diskio_native

INCLUDES += -I$(RIOTBASE)/sys/include/fatfs_diskio

FATFS_DISKIO_NATIVE_DEFAULT_FILE ?= \"riot_fatfs_disk.img\"

CFLAGS += -DFATFS_DISKIO_NATIVE_DEFAULT_FILE=$(FATFS_DISKIO_NATIVE_DEFAULT_FILE)

include $(RIOTBASE)/Makefile.base
