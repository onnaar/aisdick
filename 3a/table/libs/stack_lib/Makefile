INCLUDE_DIR = include
SOURCE_DIR = source
BUILD_DIR = build

CC = gcc
CFLAGS = -Iinclude -fPIC -Wall -Wextra -Werror
CFLAGS += -I../../libs/table_lib/include
LDFLAGS = -shared
LIB = libstack.so

OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(SOURCE_DIR)/*))

$(LIB) : $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.c $(INCLUDE_DIR)/*
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(BUILD_DIR)/* *.so
