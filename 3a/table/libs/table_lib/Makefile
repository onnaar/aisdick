INCLUDE_DIR = include
SRC_DIR = source
BUILD_DIR = build

CC = gcc
CFLAGS = -I$(INCLUDE_DIR) -fPIC -Wall -Wextra -Werror 
CFLAGS += -I../../libs/input_lib/include
CFLAGS += -I../../libs/stack_lib/include
LDFLAGS = -shared

LIB = libtable.so

OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(SRC_DIR)/*.c))

$(LIB) : $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c $(INCLUDE_DIR)/*
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(BUILD_DIR)/* *.so


