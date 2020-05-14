#===============================================================================

TARGET_OS = WINDOWS
#TARGET_OS = UNIX

#===============================================================================
SOURCEDIRS := ./src/
SOURCEDIRS += ./src/core/
ifeq ($(TARGET_OS), WINDOWS)
SOURCEDIRS += ./src/target_specific/windows/
CDEFS = -D _TARGET_WINDOWS_
CPPDEFS = -D _TARGET_WINDOWS_
else
SOURCEDIRS += ./src/target_specific/unix/
CDEFS = -D _TARGET_UNIX_
CPPDEFS = -D _TARGET_UNIX_
endif

INCDIRS := $(SOURCEDIRS)
INCDIRS += ./src/core/include/
INCDIRS += ./src/core/include/interfaces/


OPTIM  = fast

#===============================================================================
TARGETNAME := $(notdir $(CURDIR))
GNU_TOOLS = ""

SRC_EXTENSIONS = c cpp
CONFIG_DIR = CONFIG

PROJ_ROOT := .

OUTDIR  := $(PROJ_ROOT)/OUTPUT
BINDIR  := $(OUTDIR)/bin
OBJDIR  := $(OUTDIR)/obj
LSTDIR  := $(OUTDIR)/lst
OUTDIRS := $(OUTDIR) $(BINDIR) $(OBJDIR) $(LSTDIR)

VPATH +=  $(INCDIRS) $(SOURCEDIRS)

#===============================================================================
.PHONY: all outdirs gccversion clean

#all: outdirs dependencies $(BINDIR)/$(TARGETNAME)
all: outdirs  $(BINDIR)/$(TARGETNAME)

read_id:
	./scripts/read_id.sh
	
read:
	./scripts/read.sh

erase:
	./scripts/erase.sh

write:
	./scripts/write.sh

error:
	./scripts/error.sh

help:
	./scripts/help.sh




include $(PROJ_ROOT)/$(CONFIG_DIR)/build_rules.mk
include $(wildcard $(OBJDIR)/*.d)
