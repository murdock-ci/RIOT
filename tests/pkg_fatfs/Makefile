APPLICATION = pkg_fatfs
include ../Makefile.tests_common

# import list of provided features
include $(RIOTBASE)/boards/$(BOARD)/Makefile.features

USEMODULE += shell

ifeq ($(BOARD),native)
  USEMODULE += fatfs_diskio_native
else
  USEMODULE += fatfs_diskio_sdcard_spi
endif

USEPKG += fatfs

include $(RIOTBASE)/Makefile.include
