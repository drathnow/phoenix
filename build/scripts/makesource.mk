CPP_SRCS := $(wildcard *.cpp)
C_SRCS := $(wildcard *.c)
CPP_DEPS := 
C_DEPS := 

OBJS = $(patsubst %.cpp, $(OUT_DIR)/%.o, $(CPP_SRCS))
OBJS += $(patsubst %.c, $(OUT_DIR)/%.o, $(C_SRCS))

CPP_DEPS = $(patsubst %.cpp,$(OUT_DIR)/%.d, $(CPP_SRCS))
C_DEPS = $(patsubst %.cpp,$(OUT_DIR)/%.d, $(C_SRCS))