PKG_NAME=minmea
PKG_URL=https://github.com/cloudyourcar/minmea
PKG_VERSION=48ba91275c57838150979eb2820b6e10f5a894b2
PKG_LICENSE=WTFPL

.PHONY: all

all: git-download
	@cp Makefile.${PKG_NAME} $(PKG_BUILDDIR)/Makefile
	"$(MAKE)" -C $(PKG_BUILDDIR)

include $(RIOTBASE)/pkg/pkg.mk
