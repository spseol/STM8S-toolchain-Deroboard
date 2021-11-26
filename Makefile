
# Switch Makefile

MKDIR = mkdir
ifeq ($(OS),Windows_NT)
    uname_S := Windows

	CC_ROOT = "/c/Program Files/SDCC"
	CC = $(CC_ROOT)/bin/sdcc
	LN = cp
	OPENOCD = openocd -f .make/stm8s-flash.cfg -f interface/stlink.cfg -f target/stm8s.cfg
else
    uname_S := $(shell uname -s)

	CC_ROOT = /usr/
	CC = $(CC_ROOT)/bin/sdcc
	LN = ln -sf
	OPENOCD = openocd -f .make/stm8s-flash.cfg -f interface/stlink-dap.cfg -f target/stm8s.cfg
endif
CP = cp


menu::
	@echo "# Vyber si toolchain!"
	@echo
	@echo "# Pokud chceš debug a nevadí ti větší binárka, zavolej:"
	@echo "make switch-sdcc "
	@echo
	@echo "# Pokud chceš malé binárky a nepotřebuješ debug zavolej:"
	@echo "make switch-sdccrm"
	@echo
	@echo "# Pokud chceš malé binárky i debug a máš 'sdcc-gas' zavolej:"
	@echo "make switch-sdcc-gas"

default: switch-sdcc
switch-sdcc::
	$(LN) .make/Makefile-sdcc     Makefile
switch-sdcc-gas::
	$(LN) .make/Makefile-sdcc-gas Makefile
switch-sdccrm:
	$(LN) .make/Makefile-sdccrm Makefile

spl::
	bash .make/spl.sh
