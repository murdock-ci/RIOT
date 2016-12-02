APPLICATION = pkg_fatfs
include ../Makefile.tests_common

USEMODULE += shell

BOARD ?= native

ifeq ($(BOARD),native)
  USEMODULE += fatfs_diskio_native
  FATFS_DISKIO_NATIVE_DEFAULT_FILE ?= \"riot_fatfs_disk.img\"
else
  USEMODULE += fatfs_diskio_sdcard_spi  
  NUM_OF_SD_CARDS ?= 1
  SDCARD_SPI_CONFIGS ?= { { .spi_dev = SPI_1, .cs_pin = GPIO_PIN(0,14) } }
  CFLAGS += "-DSDCARD_SPI_CONFIGS=$(SDCARD_SPI_CONFIGS)"
  CFLAGS += "-DNUM_OF_SD_CARDS=$(NUM_OF_SD_CARDS)"
endif

USEPKG += fatfs

include $(RIOTBASE)/Makefile.include
