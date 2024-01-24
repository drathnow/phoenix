# Incrdev Jul 15, 2015 change ARM release to O1 from O2

#basic rules for all:
CC    = gcc
CXX   = g++
AR    = ar
AS    = as
LD    = ld
LD_VERBOSE = -Wl,--verbose
STRIP = strip

#Debug target stuff...
ifeq ($(PLT), Debug)
#no define strip binary
DSB = 
DEFINES = -DZTRACE -DZDEV
GCC_FLAGS += -O0 -g3 -Wall -c -fmessage-length=0 -fPIC
CC_FLAGS += $(GCC_FLAGS)
L_FLAGS = 
endif

ifeq ($(PLT), DebugARM)
#
# define cross compiler location.  Note that this value could be overridden by 
# either the platform make definitions or by developers local definitions
#

# define platform specific tools and options
CC    := $(CROSS_COMPILER_BIN_PREFIX)$(CC)
CXX   := $(CROSS_COMPILER_BIN_PREFIX)$(CXX)
AR    := $(CROSS_COMPILER_BIN_PREFIX)$(AR)
AS    := $(CROSS_COMPILER_BIN_PREFIX)$(AS)
LD    := $(CROSS_COMPILER_BIN_PREFIX)$(LD)
STRIP := $(CROSS_COMPILER_BIN_PREFIX)$(STRIP)
#no define strip binary
DSB = 
DEFINES = -DZTRACE -D_ONARM=LPC3250_K51 -DZDEV
GCC_FLAGS = -O0 -g3 -Wall -c -fmessage-length=0  -Wno-psabi -ftrack-macro-expansion=0 -fPIC
CC_FLAGS = $(GCC_FLAGS)
L_FLAGS =
endif

#Release target stuff...
ifeq ($(PLT), Release)
#yes, define strip binary
DSB   := STRIP
DEFINES = -DNDEBUG
GCC_FLAGS= -O2 -Wall -c -fmessage-length=0 -fPIC
CC_FLAGS= $(GCC_FLAGS)
endif

ifeq ($(PLT), ReleaseARM)

# define platform specific tools and options
CC    := $(CROSS_COMPILER_BIN_PREFIX)$(CC)
CXX   := $(CROSS_COMPILER_BIN_PREFIX)$(CXX)
AR    := $(CROSS_COMPILER_BIN_PREFIX)$(AR)
AS    := $(CROSS_COMPILER_BIN_PREFIX)$(AS)
LD    := $(CROSS_COMPILER_BIN_PREFIX)$(LD)
STRIP := $(CROSS_COMPILER_BIN_PREFIX)$(STRIP)
#yes, define strip binary
DSB   := STRIP
DEFINES = -D_ONARM=LPC3250_K51 -DNDEBUG
# Note: Jul 15, 2015 changed from O2 to O1, O2 seems to cause code to segfault on ARM.
GCC_FLAGS = -O0 -Wall -c -fmessage-length=0  -Wno-psabi -ftrack-macro-expansion=0 -fPIC
CC_FLAGS = $(GCC_FLAGS)
endif

