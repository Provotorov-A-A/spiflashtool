#===============================================================================
SOURCEDIRS := ./src/
SOURCEDIRS += ./src/core/
INCDIRS := $(SOURCEDIRS)
INCDIRS += ./src/core/include/

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

include $(PROJ_ROOT)/$(CONFIG_DIR)/build_rules.mk
include $(wildcard $(OBJDIR)/*.d)
