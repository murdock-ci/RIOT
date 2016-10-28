MODULE = cpu

DIRS += periph
DIRS += vfs

ifneq (,$(filter netdev_tap,$(USEMODULE)))
	DIRS += netdev_tap
endif

ifneq (,$(filter socket_zep,$(USEMODULE)))
  DIRS += socket_zep
endif

ifneq (,$(filter mtd_native,$(USEMODULE)))
	DIRS += mtd
endif

include $(RIOTBASE)/Makefile.base

INCLUDES = $(NATIVEINCLUDES)
