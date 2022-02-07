
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
	@echo "# Detaily v REAME a na adrese "
	@echo "# https://chytrosti.marrek.cz/vyvoj-stm8-s-opensource-nastroji.html"
	@echo
	@echo "# Pokud chces debug a nevadi ti vetsi binarka:"
	@echo \"make sdcc\" "     ... nebo"
	@echo \"make default\"
	@echo
	@echo "# Pokud chces male binarky a nepotrebujes debug:"
	@echo \"make sdccrm\"
	@echo
	@echo "# Pokud chces male binarky i debug a mas 'SDCC-gas' zavolej:"
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
