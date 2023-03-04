SHELL := /bin/bash
#MAKEFLAGS := --jobs=1
MAKEFLAGS := --jobs=$(shell nproc --ignore 1)
MAKEFLAGS += --output-sync=target
MAKEFLAGS += --load-average=$(shell nproc)
######################################################
#         Makefile for sdccrm
######################################################
#
#### STM8 device LowerCase
#
#DEVICE_FLASH=stm8s003k3
DEVICE_FLASH=stm8s103f3
#DEVICE_FLASH=stm8s105c6
#DEVICE_FLASH=stm8s208rb

### STlink version for falsh2 targer (stm8flash program)
STLINK=stlink
#STLINK=stlinkv2
#STLINK=stlinkv21

F_CPU=16000000

ifeq ($(OS),Windows_NT)
	CC_ROOT = "/c/Program Files/SDCC"
	STVP_ROOT = "/c/Program Files (x86)/STMicroelectronics/st_toolset/stvp"
else
	#CC_ROOT = /usr
	#CC_ROOT = /usr/local
	CC_ROOT = /usr/local/stow/sdcc-4.2.0
endif
######################################################################################
CC = $(CC_ROOT)/bin/sdcc

#DEVICE=STM8S103
DEVICE=$(shell bash .make/device $(DEVICE_FLASH))
DEVICE_FLASH_STVP=$(shell bash .make/device_stvp $(DEVICE_FLASH))

MKDIR = mkdir
CP = cp

ifeq ($(findstring 2019, $(shell openocd --version 2>&1)), 2019)
    interface = interface/stlink.cfg
else
    interface = interface/stlink-dap.cfg
endif
ifeq ($(findstring 003, $(DEVICE)), 003)
	target = target/stm8s003.cfg
else ifeq ($(findstring 103, $(DEVICE)), 103)
	target = target/stm8s103.cfg
else ifeq ($(findstring 105, $(DEVICE)), 105)
	target = target/stm8s105.cfg
else
	target = target/stm8s.cfg
endif
OPENOCD = openocd -f .make/stm8s-flash.cfg -f $(interface) -f $(target)


ifeq ($(OS),Windows_NT)
    uname_S := Windows

	LN = cp
	PYTHON = python
	STVP = $(STVP_ROOT)/STVP_CmdLine.exe
else
    uname_S := $(shell uname -s)

	LN = ln -sf
	PYTHON = python3
endif

# trap handling requires SDCC >=v3.4.3
SKIP_TRAPS = 1

# set compiler path & parameters 
CFLAGS  = -mstm8 -lstm8 --opt-code-size 
CFLAGS  = -mstm8 -lstm8 --opt-code-size --std-sdcc99 --nogcse --all-callee-saves --stack-auto --fverbose-asm --float-reent --no-peep
CFLAGS += -I inc
CFLAGS += -D F_CPU=$(F_CPU)

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
PRJ_INCS  = $(wildcard $(PRJ_INC_DIR)/*.h)
PRJ_ASSMS := $(addprefix $(OUTPUT_DIR)/, $(PRJ_SOURCE:.c=.asm))
PRJ_ASSRM := $(addprefix $(OUTPUT_DIR)/, $(PRJ_SOURCE:.c=.asmrm))

# set SPL paths
#SPL_SRC_DIR = /usr/local/lib/stm8/src/
#SPL_INC_DIR = /usr/local/lib/stm8/inc/
#SPL_SRC_DIR = ../SPL/src/
#SPL_INC_DIR = ../SPL/inc/
SPL_SRC_DIR = ../SPL-$(DEVICE)/src/
SPL_INC_DIR = ../SPL-$(DEVICE)/inc/
# add all library sources used here singly ... or all .c files in SPL src dir
SPL_SOURCE  = stm8s_gpio.c stm8s_clk.c stm8s_tim4.c stm8s_itc.c 
SPL_SOURCE += stm8s_adc1.c
SPL_SOURCE += stm8s_exti.c
SPL_SOURCE += stm8s_flash.c
#SPL_SOURCE += stm8s_i2c.c
#SPL_SOURCE += stm8s_spi.c
SPL_SOURCE += stm8s_tim1.c
SPL_SOURCE += stm8s_tim2.c
SPL_SOURCE += stm8s_uart1.c
#SPL_SOURCE  = $(notdir $(wildcard $(SPL_SRC_DIR)/*.c))
SPL_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(SPL_SOURCE:.c=.rel))
SPL_ASSMS := $(addprefix $(OUTPUT_DIR)/, $(SPL_SOURCE:.c=.asm))
SPL_ASSRM := $(addprefix $(OUTPUT_DIR)/, $(SPL_SOURCE:.c=.asmrm))

# collect all include folders
INCLUDE = -I$(PRJ_INC_DIR) -I$(SPL_INC_DIR) 

# collect all source directories
VPATH=$(PRJ_SRC_DIR):$(SPL_SRC_DIR)


ihx:: $(TARGET).ihx

$(TARGET).ihx: $(PRJ_ASSMS) $(SPL_ASSMS)
	@echo asm2obj
	bash .make/asm2obj $^
	@echo linking 
	$(CC) $(CFLAGS) -o $(TARGET).ihx $(PRJ_OBJECTS) $(SPL_OBJECTS) >.make/linker_output 2>&1 || rm -f $(TARGET).ihx
	+bash .make/reference2exclude .make/linker_output
	$(LN) $@ ./out.ihx
	@echo 
	@echo MemSize:
	@$(PYTHON) .make/sizecal.py ./out.ihx
	@echo 


$(OUTPUT_DIR)/%.asm: %.c Makefile exclude_reference $(PRJ_INCS) | $(OUTPUT_DIR)
	+$(CC) $(CFLAGS) -D$(DEVICE) $(INCLUDE) -DSKIP_TRAPS=$(SKIP_TRAPS) -c $< -o $@ 2>&1 | $(PYTHON) .make/recolor.py; exit $${PIPESTATUS[0]}

$(OUTPUT_DIR):
	$(MKDIR) -p $(OUTPUT_DIR)

clean::
	rm -f ./out.ihx
	rm -f ./out.elf
	rm -Rf $(OUTPUT_DIR)

flash: $(TARGET).ihx
ifeq ($(OS),Windows_NT)
	+$(STVP) -BoardName=ST-LINK -Device=$(DEVICE_FLASH_STVP) -Port=USB -ProgMode=SWIM -no_loop -no_log -FileProg=$(TARGET).ihx
else
	+$(OPENOCD) -c "init" -c "program_device $(TARGET).ihx 0"
endif

flash2: $(TARGET).ihx
	+stm8flash -c $(STLINK) -p $(DEVICE_FLASH) -s flash -w $(TARGET).ihx

rebuild::
	$(MAKE) clean
	$(MAKE) ihx

reflash::
	$(MAKE) clean
	$(MAKE) flash

switch-sdcc-gas::
	$(LN) .make/Makefile-sdcc-gas Makefile || $(CP) .make/Makefile-sdcc-gas Makefile

spl::
	bash .make/spl.sh

.PHONY: clean
