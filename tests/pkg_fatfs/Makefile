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
  
  ifeq ($(BOARD), samr21-xpro)
    SDCARD_SPI_CONFIGS ?= { { .spi_dev = SPI_1, \
                              .cs_pin = GPIO_PIN(0,14), \
                              .clk_pin = GPIO_PIN(1,23), \
                              .mosi_pin = GPIO_PIN(1,22), \
                              .miso_pin = GPIO_PIN(1,2), \
                              .power_pin = GPIO_UNDEF } }
  else ifeq ($(BOARD), remote-revb)
    SDCARD_SPI_CONFIGS ?= { { .spi_dev = SPI_1, \
                              .cs_pin = GPIO_PIN(0,7), \
                              .clk_pin = GPIO_PIN(2,4), \
                              .mosi_pin = GPIO_PIN(2,5), \
                              .miso_pin = GPIO_PIN(2,6), \
                              .power_pin = GPIO_PIN(0,6), \
                              .power_pin_act_high = false } }
  endif

  CFLAGS += "-DSDCARD_SPI_CONFIGS=$(SDCARD_SPI_CONFIGS)"
  CFLAGS += "-DNUM_OF_SD_CARDS=$(NUM_OF_SD_CARDS)"
endif

USEPKG += fatfs

include $(RIOTBASE)/Makefile.include
