APPLICATION = pkg_fatfs
include ../Makefile.tests_common

USEMODULE += shell

BOARD ?= native

ifeq ($(BOARD),native)
  USEMODULE += fatfs_diskio_native
  FATFS_DISKIO_NATIVE_DEFAULT_FILE ?= \"riot_fatfs_disk.img\"
else
  USEMODULE += fatfs_diskio_sdcard_spi
  USEMODULE += auto_init_storage
endif

USEPKG += fatfs

include $(RIOTBASE)/Makefile.include
