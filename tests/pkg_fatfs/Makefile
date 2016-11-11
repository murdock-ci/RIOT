APPLICATION = pkg_fatfs
include ../Makefile.tests_common

# import list of provided features
include $(RIOTBASE)/boards/$(BOARD)/Makefile.features

$(foreach f, $(FEATURES_PROVIDED), $(info $(f)))

#if periph_rtc is available use it. Otherwise use static timestamps
ifneq (, $(filter $(periph_rtc), $(FEATURES_PROVIDED)))
   $(info RTC AVAILABLE)
   FEATURES_REQUIRED += periph_rtc
   CFLAGS += -DFATFS_RTC_AVAILABLE
endif

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
