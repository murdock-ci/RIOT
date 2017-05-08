PKG_NAME     = fatfs
PKG_URL      = elm-chan.org/fsw/ff/arc
PKG_VERSION  = ff12b
PKG_EXT      = zip
PKG_BUILDDIR = $(BINDIRBASE)/pkg/$(BOARD)/$(PKG_NAME)
MODULE_MAKEFILE := $(CURDIR)/Makefile.module
DOWNLOAD_PATH = $(RIOTBASE)/pkg/$(PKG_NAME)

# import list of provided features
include $(RIOTBASE)/boards/$(BOARD)/Makefile.features

ifneq ($(RIOTBASE),)
include $(RIOTBASE)/Makefile.base
endif

.PHONY: all clean patch

all: $(PKG_BUILDDIR)/.patched

ffconf_patches += $(CURDIR)/patches/enable_use_lable.patch
ffconf_patches += $(CURDIR)/patches/set_code_page_to_ascii.patch
ffconf_patches += $(CURDIR)/patches/enable_use_mkfs.patch
ffconf_patches += $(CURDIR)/patches/enable_fs_tiny.patch

#if periph_rtc is available use it. Otherwise use static timestamps
ifeq (, $(filter $(periph_rtc), $(FEATURES_PROVIDED)))
ffconf_patches += $(CURDIR)/patches/disable_rtc.patch
FEATURES_REQUIRED += periph_rtc
endif

$(PKG_BUILDDIR)/.patched: $(PKG_BUILDDIR)/.prepared
	$(AD)$(foreach p, $(ffconf_patches), patch $(PKG_BUILDDIR)/ffconf.h < $(p);)
	$(AD)patch $(PKG_BUILDDIR)/integer.h < $(CURDIR)/patches/remove_int_type.patch
	$(AD)patch $(PKG_BUILDDIR)/ff.c < $(CURDIR)/patches/fix_for_guards.patch
	$(AD)touch $@

$(PKG_BUILDDIR)/.prepared: $(PKG_BUILDDIR)/.extracted
	$(AD)mv $(PKG_BUILDDIR)/src/* $(PKG_BUILDDIR)/
	$(AD)rm -rf $(PKG_BUILDDIR)/src
	$(AD)rm -rf $(PKG_BUILDDIR)/doc

#this file is only a dummy that should be replaced by a compatible implementation
	$(AD)rm $(PKG_BUILDDIR)/diskio.c
	$(AD)cp $(MODULE_MAKEFILE) $(PKG_BUILDDIR)/Makefile
	$(AD)cp $(MODULE_MAKEFILE) $(PKG_BUILDDIR)/option
	$(AD)touch $@

$(PKG_BUILDDIR)/.extracted: $(DOWNLOAD_PATH)/$(PKG_NAME)-$(PKG_VERSION).$(PKG_EXT)
	$(AD)mkdir -p $(@D)
	$(AD)cd $(@D) && $(UNZIP_HERE) $(<)
	$(AD)touch $@

$(DOWNLOAD_PATH)/$(PKG_NAME)-$(PKG_VERSION).$(PKG_EXT):
	$(AD)mkdir -p $(@D)
	$(AD)$(DOWNLOAD_TO_FILE) $@ $(PKG_URL)/$(PKG_VERSION).$(PKG_EXT)

clean::
	$(AD)rm -rf $(PKG_BUILDDIR)

distclean::
	$(AD)rm -f $(DOWNLOAD_PATH)/$(PKG_NAME)-$(PKG_VERSION).$(PKG_EXT)
