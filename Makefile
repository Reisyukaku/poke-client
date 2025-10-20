#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

TOPDIR ?= $(CURDIR)
include $(DEVKITPRO)/libnx/switch_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# DATA is a list of directories containing data files
# INCLUDES is a list of directories containing header files
#
# CONFIG_JSON is the filename of the NPDM config file (.json), relative to the project folder.
#   If not set, it attempts to use one of the following (in this order):
#     - <Project name>.json
#     - config.json
#---------------------------------------------------------------------------------
export TARGET		:=	main

#----------------------------- User configuration -----------------------------
# PROGRAM_ID is titleID of the game
# LOAD_KIND_ENUM is the type of hook (0=KIP, 1=ASRTLD, 2=Module)
# BINARY_NAME is the name the hook will be called (named after a subsdk)
# DIST_DIR is the dir the file build will be copied to

PROGRAM_ID 		:= 	0100F43008C44000
LOAD_KIND_ENUM 	:= 	2
BINARY_NAME 	:= 	subsdk9
DIST_DIR 		:= 	$(TOPDIR)/deploy
CONFIG_JSON		:=	$(PROGRAM_ID).json
TCPLOG_IP		:= 	192.168.1.221

#-------------------------- End of user configuration --------------------------

BUILD		:=	build
ROOT_SOURCE	:=	$(TOPDIR)/source
MODULES		:=	$(shell find $(ROOT_SOURCE) -mindepth 1 -maxdepth 1 -type d)
SOURCES		:=	$(foreach module,$(MODULES),$(shell find $(module) -type d))
SOURCES		:= 	$(foreach source,$(SOURCES),$(source:$(TOPDIR)/%=%)/)
GIT_SHA_FETCH := $(shell git rev-parse HEAD | cut -c 1-16)

DATA		:=	data
INCLUDES	:=	include

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fpic -fvisibility=hidden

CFLAGS	:=	-g -Wall -O3 \
			-ffunction-sections \
			-fdata-sections \
			$(ARCH) \
			$(DEFINES)

CFLAGS	+=	$(INCLUDE) -D__SWITCH__ -D__RTLD_6XX__

CFLAGS	+=  -DEXL_LOAD_KIND="Module" \
			-DEXL_LOAD_KIND_ENUM=$(LOAD_KIND_ENUM) \
			-DEXL_PROGRAM_ID=0x$(PROGRAM_ID) \
			-DTCPLOG_IP=\"$(TCPLOG_IP)\" \
			-DGIT_COMMIT_HASH=\"$(GIT_SHA_FETCH)\" \
			-I"$(DEVKITPRO)/libnx/include" \
			-I$(ROOT_SOURCE) $(addprefix -I,$(MODULES))

CXXFLAGS	:= $(CFLAGS) \
				-fno-rtti -fno-exceptions -fno-asynchronous-unwind-tables -fno-unwind-tables -std=gnu++20 

ASFLAGS	:=	-g $(ARCH)

SPEC_NAME := 
ifeq ($(LOAD_KIND_ENUM), 1)
	SPEC_NAME := as_rltd.specs 
else ifeq ($(LOAD_KIND_ENUM), 2)
	SPEC_NAME := module.specs 
endif

LDFLAGS	:=  -specs=$(TOPDIR)/specs/$(SPEC_NAME) -g $(ARCH) -Wl,-Map,$(notdir $*.map) -nostartfiles

LIBS	:=

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES_BIN	:=	$(addsuffix .o,$(BINFILES))
export OFILES_SRC	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES 	:=	$(OFILES_BIN) $(OFILES_SRC)
export HFILES_BIN	:=	$(addsuffix .h,$(subst .,_,$(BINFILES)))

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD) -I$(ROOT_SOURCE)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

ifeq ($(strip $(CONFIG_JSON)),)
	jsons := $(wildcard npdm/*.json)
	ifneq (,$(findstring npdm/$(TARGET).json,$(jsons)))
		export APP_JSON := $(TOPDIR)/npdm/$(PROGRAM_ID).json
	endif
else
	export APP_JSON := $(TOPDIR)/npdm/$(CONFIG_JSON)
endif

.PHONY: $(BUILD) clean all

#---------------------------------------------------------------------------------
all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
	@mkdir -p $(DIST_DIR)
	@mv $(TARGET).nso $(DIST_DIR)/$(BINARY_NAME)
	@mv $(TARGET).npdm $(DIST_DIR)/$(TARGET).npdm
	

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).nso $(TARGET).npdm $(TARGET).elf $(DIST_DIR)
#---------------------------------------------------------------------------------
else
.PHONY:	all 

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------

all	:	$(OUTPUT).nso

$(OUTPUT).nso	:	$(OUTPUT).elf $(OUTPUT).npdm

$(OUTPUT).elf	:	$(OFILES)

$(OFILES_SRC)	: $(HFILES_BIN)

#---------------------------------------------------------------------------------
# you need a rule like this for each extension you use as binary data
#---------------------------------------------------------------------------------
%.bin.o	%_bin.h :	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
