APPLICATION = gnrc_sock_dns
include ../Makefile.tests_common

RIOTBASE ?= $(CURDIR)/../..

BOARD_INSUFFICIENT_MEMORY := nucleo32-f042 nucleo32-f031

USEMODULE += sock_dns
USEMODULE += gnrc_sock_udp
USEMODULE += gnrc_ipv6_default
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif

USEMODULE += shell_commands

CFLAGS += -DDEVELHELP

include $(RIOTBASE)/Makefile.include
