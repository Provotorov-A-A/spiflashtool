ifndef _BUILD_RULES_MK_
unexport _BUILD_RULES_MK_ = 1

include $(PROJ_ROOT)/$(CONFIG_DIR)/target_rules.mk
#===============================================================================

CC    = $(GNU_TOOLS)gcc
CPP   = $(GNU_TOOLS)g++
LD    = $(GNU_TOOLS)ld
AR    = $(GNU_TOOLS)ar
AS    = $(GNU_TOOLS)as
OCP   = $(GNU_TOOLS)objcopy
RL    = $(GNU_TOOLS)ranlib
OD    = $(GNU_TOOLS)objdump
SIZE  = $(GNU_TOOLS)size


	
#===============================================================================
#Process all files with specified extensions
SEARCH_DIR_SOURCES = $(foreach EXT,$(SRC_EXTENSIONS),$(wildcard $(1)*.$(EXT)))
SOURCES := $(foreach DIR,$(SOURCEDIRS),$(call SEARCH_DIR_SOURCES,$(DIR)))
OBJECTS = $(addprefix $(OBJDIR)/,$(addsuffix .o, $(notdir $(basename $(1)))))

$(BINDIR)/$(TARGETNAME): $(call OBJECTS,$(SOURCES))
	@echo ... linking:     $(notdir $@)
	@$(CPP)  $(LDFLAGS) $^ $(LIBSFLAGS) -o $@

$(OBJDIR)/%.o: %.s
	@echo ... assembling: $<
	@$(CPP) -c $(ALL_ASFLAGS) $< -o $@ 

$(OBJDIR)/%.o: %.c
	@echo ... compiling: $<
	@$(CPP) -c $(ALL_CFLAGS) $< -o $@

$(OBJDIR)/%.o: %.cpp
	@echo ... compiling: $<
	@$(CPP) -c $(ALL_CPPFLAGS) $< -o $@

#===============================================================================
gccversion:
	@$(CC) --version

outdirs:
ifeq "" "$(wildcard $(OUTDIR) )"
	@mkdir -p $(OUTDIRS)
endif

clean:
ifneq "" "$(wildcard $(OUTDIR) )"
	@rm -f -r $(OUTDIR)
endif

endif #ifndef _BUILD_RULES_MK_
