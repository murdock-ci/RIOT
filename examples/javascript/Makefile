# application name
APPLICATION = riot_javascript

# default BOARD environment
BOARD ?= native

BOARD_INSUFFICIENT_MEMORY := airfy-beacon arduino-duemilanove arduino-mega2560 \
                             arduino-uno calliope-mini cc2650stk chronos \
                             maple-mini microbit msb-430 msb-430h nrf51dongle \
                             nrf6310 nucleo-f030 nucleo-f070 nucleo-f072 \
                             nucleo-f103 nucleo-f302 nucleo-f334 nucleo-f410 \
                             nucleo-l053 nucleo-l073 nucleo32-f031 nucleo32-f042 \
                             nucleo32-f303 nucleo32-l031 opencm904 pca10000 \
                             pca10005 qemu-i386 spark-core stm32f0discovery \
                             telosb waspmote-pro weio wsn430-v1_3b wsn430-v1_4 \
                             yunjia-nrf51822 z1 \

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
