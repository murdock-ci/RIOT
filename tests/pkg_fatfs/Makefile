APPLICATION = pkg_fatfs
include ../Makefile.tests_common

FEATURES_REQUIRED += periph_rtc

USEMODULE += shell

USEPKG += fatfs

ifeq ($(BOARD),native)
  DIRS += fatfs_diskio_native
  USEMODULE += fatfs_diskio_native
else
  DIRS += fatfs_diskio_sdcard_spi
  USEMODULE += fatfs_diskio_sdcard_spi
  USEMODULE += sdcard_spi
endif

include $(RIOTBASE)/Makefile.include
