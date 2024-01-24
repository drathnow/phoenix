#makedef.mk
#
# This file defines common definitions for all ZIOS makefiles.  It should be the 
# file included in all makefiles that are under the ZIOS build system.
#
#
# 1st check that PLT defined and one of recognized platforms...
#
ifeq ($(PLT),)
  $(error PLT not defined by make invocation, use PLT=<platform> on make cmd line)
endif

#
# These are the platform specific definitions.
#
ifeq ("$(wildcard ~/.zdev/platform.mk)","")
ifeq ("$(wildcard /usr/local/zdev/platform.mk)","")
  $(error Platform make definition file was not found.  Looking for /usr/local/zdev/platform.mk or ~/.zdev/platform.mk)
else
-include /usr/local/zdev/platform.mk
endif
else
-include ~/.zdev/platform.mk
endif

#
# Next pull in user/developer specific defintions
# 
-include ~/.zdev/localdev.mk

INCLUDES = -I"." -I"../AppIncludes"

ifeq ($(PLT),Debug)
LIB_PATH = -L/usr/local/lib
INCLUDES = -I/usr/local/include
else
ifeq ($(PLT),DebugARM)
INCLUDES += $(ARM_INCLUDES)
LIB_PATH += $(ARM_LIB_PATH)
else
ifeq ($(PLT),Release)
LIB_PATH = -L/usr/local/lib
INCLUDES = -I/usr/local/include
else
ifeq ($(PLT),ReleaseARM)
INCLUDES += $(ARM_INCLUDES)
LIB_PATH += $(ARM_LIB_PATH)
else
  $(error PLT unknown, please use one of Debug,DebugARM,Release,ReleaseARM)
endif
endif
endif
endif

OUT_DIR = ./$(PLT)
SHARED_DIR = ../sharedlibs/$(PLT)