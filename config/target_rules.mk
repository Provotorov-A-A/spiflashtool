ifndef _TARGET_RULES_MK_
unexport _TARGET_RULES_MK_ = 1

#CSTANDARD   = -std=gnu11
#CPPSTANDARD = -std=gnu++11

COMMONFLAGS  = -Wall
COMMONFLAGS += -O$(OPTIM)
#COMMONFLAGS += -g$(DEBUG)
COMMONFLAGS += $(patsubst %,-I%,$(patsubst %/,%,$(INCDIRS)))
#COMMONFLAGS += -MD

#===============================================================================
CFLAGS  = $(CDEFS)
#CFLAGS += -Wstrict-prototypes
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += $(CDEFS)
CFLAGS += $(CSTANDARD) 
#CFLAGS += -Wa,-ahlmsdc=$(LSTDIR)/$(notdir $(<:.c=.lst))

#===============================================================================
CPPFLAGS  = $(CPPDEFS)
CPPFLAGS  += -lstdc++
#CPPFLAGS += -fno-exceptions -fno-rtti
#CPPFLAGS += -Wextra -Wundef -Wcast-align
#CPPFLAGS += -Wa,-adhlns=$(LSTDIR)/$(notdir $(<:.cpp=.lst))
CPPFLAGS += -ffunction-sections -fdata-sections

#===============================================================================
ALL_CFLAGS   = $(COMMONFLAGS) $(CFLAGS)
ALL_CPPFLAGS = $(COMMONFLAGS) $(CPPFLAGS) -x c++

#===============================================================================
LDFLAGS = -Wl,-Map="$(BINDIR)/$(TARGETNAME).map"
LDFLAGS += -Wl,--gc-sections

#===============================================================================
ELFSIZE = $(SIZE) $(BINDIR)/$(TARGETNAME).elf -B -d --common

%.hex: %.elf
	@$(OCP) --output-target=ihex $< $@

%.bin: %.elf
	@$(OCP) --output-target=binary $< $@



#===============================================================================
endif #ifndef _TARGET_RULES_MK_