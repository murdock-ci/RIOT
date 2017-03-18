# application name
APPLICATION = riot_javascript

# default BOARD environment
BOARD ?= native

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

# Set stack size to something (conservatively) enormous
CFLAGS += -DTHREAD_STACKSIZE_MAIN=9092

# Add the shell and some shell commands
USEMODULE += shell
USEMODULE += shell_commands

# Add the package for Jerryscript
USEPKG += jerryscript

include $(CURDIR)/../../Makefile.include
