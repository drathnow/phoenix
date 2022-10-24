CPP_SRCS := $(wildcard *.cpp)
C_SRCS := $(wildcard *.c)

OBJS = $(patsubst %.cpp, $(OUT_DIR)/%.o, $(CPP_SRCS))
OBJS += $(patsubst %.c, $(OUT_DIR)/%.o, $(C_SRCS))

