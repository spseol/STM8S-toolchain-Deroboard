######################################################
#         Makefile pro čísté SDCC
######################################################
# makefile for STM8S_StdPeriph_Lib and SDCC compiler
#
# Customized by MightyPork 1/2017
#
# usage:
#   1. if SDCC not in PATH set path -> CC_ROOT
#   2. set correct STM8 device -> DEVICE
#   3. set project paths -> PRJ_ROOT, PRJ_SRC_DIR, PRJ_INC_DIR
#   4. set SPL root path -> SPL_ROOT
#   5. include required SPL modules -> SPL_SOURCE
#
#######

# STM8 device (default is STM8 discovery board)
#DEVICE=STM8S103
#DEVICE=STM8S105
DEVICE=STM8S208
#DEVICE_FLASH=stm8s103f3
#DEVICE_FLASH=stm8s105k4
DEVICE_FLASH=stm8s208rb

STLINK=stlinkv2
STLINK=stlinkv21

F_CPU=16000000


MKDIR = mkdir
ifeq ($(OS),Windows_NT)
    uname_S := Windows

	CC_ROOT = "/c/Program Files/SDCC"
	CC = $(CC_ROOT)/bin/sdcc
	LN = cp
	OPENOCD = openocd -f .make/stm8s-flash.cfg -f interface/stlink.cfg -f target/stm8s.cfg
	PYTHON = python
else
    uname_S := $(shell uname -s)

	CC_ROOT = /usr/
	CC = $(CC_ROOT)/bin/sdcc
	LN = ln -sf
	OPENOCD = openocd -f .make/stm8s-flash.cfg -f interface/stlink-dap.cfg -f target/stm8s.cfg
	PYTHON = python3
endif
CP = cp

# trap handling requires SDCC >=v3.4.3
SKIP_TRAPS = 1

# set compiler path & parameters 
CFLAGS  = -mstm8 -lstm8 --opt-code-size 
CFLAGS  = -mstm8 -lstm8 --opt-code-size --std-sdcc99 --nogcse --all-callee-saves --stack-auto --fverbose-asm --float-reent --no-peep
CFLAGS += -I inc
CFLAGS += -D F_CPU=$(F_CPU)


ifeq ($(IHEX_NODEBUG),)
	FMT_ELF = --debug --out-fmt-elf
endif

# required for some examples for STM8S EVAL board
#CFLAGS += -DUSE_STM8_128_EVAL

# set output folder and target name
OUTPUT_DIR = ./build-$(DEVICE)
TARGET     = $(OUTPUT_DIR)/out

# set project folder and files (all *.c)
PRJ_ROOT    = .
PRJ_SRC_DIR = $(PRJ_ROOT)/src
PRJ_INC_DIR = $(PRJ_ROOT)/inc
# all project sources included by default
PRJ_SOURCE  = $(notdir $(wildcard $(PRJ_SRC_DIR)/*.c))
PRJ_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(PRJ_SOURCE:.c=.rel))

# set SPL paths
#SPL_SRC_DIR = /usr/local/lib/stm8/src/
#SPL_INC_DIR = /usr/local/lib/stm8/inc/
SPL_SRC_DIR = ../SPL/src/
SPL_INC_DIR = ../SPL/inc/
#SPL_SRC_DIR = ../SPL-$(DEVICE)/src/
#SPL_INC_DIR = ../SPL-$(DEVICE)/inc/
# add all library sources used here singly ... or all .c files in SPL src dir
SPL_SOURCE  = stm8s_gpio.c stm8s_clk.c stm8s_tim4.c stm8s_itc.c 
#SPL_SOURCE += stm8s_uart1.c
SPL_SOURCE += stm8s_adc2.c
#SPL_SOURCE += stm8s_flash.c
#SPL_SOURCE += stm8s_tim1.c
#SPL_SOURCE += stm8s_tim2.c
#SPL_SOURCE += stm8s_tim3.c
#SPL_SOURCE  = $(notdir $(wildcard $(SPL_SRC_DIR)/*.c))
SPL_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(SPL_SOURCE:.c=.rel))

# collect all include folders
INCLUDE = -I$(PRJ_INC_DIR) -I$(SPL_INC_DIR) 

# collect all source directories
VPATH=$(PRJ_SRC_DIR):$(SPL_SRC_DIR)


elf:: $(TARGET).elf
ihx:: out.ihx
out.ihx::
	make half-clean
	IHEX_NODEBUG=1 make $(TARGET).ihx
	make half-clean


$(TARGET).elf: $(PRJ_OBJECTS) $(SPL_OBJECTS)
	$(CC) $(CFLAGS) $(FMT_ELF) -o $(TARGET).elf $^
	$(CP) $@ ./out.elf

$(TARGET).ihx: $(PRJ_OBJECTS) $(SPL_OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET).ihx $^
	$(CP) $@ ./out.ihx
	@echo
	@$(PYTHON) .make/sizecal.py ./out.ihx
	@echo

$(OUTPUT_DIR)/%.rel: %.c Makefile | $(OUTPUT_DIR)
	$(CC) $(CFLAGS) $(FMT_ELF) -D$(DEVICE) $(INCLUDE) -DSKIP_TRAPS=$(SKIP_TRAPS) -c $< -o $@

$(OUTPUT_DIR):
	$(MKDIR) -p $(OUTPUT_DIR)

half-clean::
	rm -Rf $(OUTPUT_DIR)
clean: half-clean
	rm -f ./out.ihx
	rm -f ./out.elf

flash: $(TARGET).elf
	$(OPENOCD) -c "init" -c "program_device $(TARGET).elf 0"

flash2: out.ihx
	stm8flash -c $(STLINK) -p $(DEVICE_FLASH) -s flash -w out.ihx

rebuild::
	make clean
	make 

reflash::
	make clean
	make flash

openocd::
	$(OPENOCD) -c "init" -c "reset halt"

debug: $(TARGET).elf
	stm8-gdb --tui $^


switch-sdcc::
	$(LN) .make/Makefile-sdcc     Makefile
switch-sdcc-gas::
	$(LN) .make/Makefile-sdcc-gas Makefile
switch-sdccrm:
	$(LN) .make/Makefile-sdccrm Makefile

spl::
	bash .make/spl.sh

.PHONY: clean debug
