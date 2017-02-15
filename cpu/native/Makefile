MODULE = cpu

DIRS += periph

ifneq (,$(filter netdev_tap,$(USEMODULE)))
	DIRS += netdev_tap
endif

include $(RIOTBASE)/Makefile.base

INCLUDES = $(NATIVEINCLUDES)
