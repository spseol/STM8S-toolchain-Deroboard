
# Switch Makefile

MKDIR = mkdir
ifeq ($(OS),Windows_NT)
    uname_S := Windows

	LN = cp
else
    uname_S := $(shell uname -s)

	LN = ln -sf
endif
CP = cp


menu::
	@echo "# Vyber si toolchain!"
	@echo
	@echo "# Pokud chceš debug a nevadí ti větší binárka, zavolej:"
	@echo \"make sdcc\"   ... nebo
	@echo \"make default\"
	@echo
	@echo "# Pokud chceš malé binárky a nepotřebuješ debug zavolej:"
	@echo \"make sdccrm\"
	@echo
	@echo "# Pokud chceš malé binárky i debug a máš 'sdcc-gas' zavolej:"
	@echo \"make sdcc-gas\"

default: sdcc
sdcc:: spl
	$(LN) .make/Makefile-sdcc Makefile || cp .make/Makefile-sdcc Makefile
sdccrm:: spl
	$(LN) .make/Makefile-sdccrm Makefile || cp .make/Makefile-sdccrm Makefile
sdcc-gas:: spl
	$(LN) .make/Makefile-sdcc-gas Makefile || cp .make/Makefile-sdcc-gas Makefile

spl::
	bash .make/spl.sh
