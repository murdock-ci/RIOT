APPLICATION = pkg_fatfs
include ../Makefile.tests_common

USEMODULE += shell

ifeq ($(BOARD),native)
  USEMODULE += fatfs_diskio_native
else
  USEMODULE += fatfs_diskio_sdcard_spi
  TEST_SDCARD_SPI  ?= SPI_1
  TEST_SDCARD_CS   ?= GPIO_PIN\(0,14\)
  CFLAGS += -DTEST_SDCARD_SPI=$(TEST_SDCARD_SPI)
  CFLAGS += -DTEST_SDCARD_CS=$(TEST_SDCARD_CS)
endif

USEPKG += fatfs

include $(RIOTBASE)/Makefile.include
