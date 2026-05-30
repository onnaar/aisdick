INCLUDE_DIR = include
SOURCE_DIR = source
BUILD_DIR = build
ROOT_PATH = ../../../gomoku
PROJECT_ROOT_BUILD_DIR = $(ROOT_PATH)/$(BUILD_DIR)

CC = gcc
CFLAGS = -I$(INCLUDE_DIR) -fPIC -Wall -Wextra -Werror
LDFLAGS = -shared
LIB = libinput.so

OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(PROJECT_ROOT_BUILD_DIR)/%.o, $(wildcard $(SOURCE_DIR)/*))

$(PROJECT_ROOT_BUILD_DIR)/$(LIB) : $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

$(PROJECT_ROOT_BUILD_DIR)/%.o : $(SOURCE_DIR)/%.c $(INCLUDE_DIR)/*
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(PROJECT_ROOT_BUILD_DIR)/$(LIB) $(OBJECTS) 
