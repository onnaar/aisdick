MAKEFLAGS += --no-print-directory

CC = gcc
CFLAGS = -Wall -Wextra -Werror

type ?= hash
size ?= 19
win ?= 5
TARGET = gomoku

INCLUDE_DIR = include
BUILD_DIR = build
SOURCE_DIR = source

LDFLAGS = -L$(BUILD_DIR) -linput -Wl,-rpath,'$$ORIGIN/$(BUILD_DIR)' -lncursesw
LIBS_BUILD = $(BUILD_DIR)/libinput.so

COMMON_SOURCES = $(filter-out $(SOURCE_DIR)/hash_adapter.c $(SOURCE_DIR)/tree_adapter.c, $(wildcard $(SOURCE_DIR)/*.c))

ifeq ($(type), hash)
    LIBS_DIR = ../3b/libs
    LDFLAGS += -lhtable
    LIBS_BUILD += $(BUILD_DIR)/libhtable.so
    COMMON_SOURCES += $(SOURCE_DIR)/hash_adapter.c
else
    LIBS_DIR = ../4b/libs
    LDFLAGS += -lscapegoat -lvector -lstack -lm
    LIBS_BUILD += $(BUILD_DIR)/libvector.so $(BUILD_DIR)/libstack.so $(BUILD_DIR)/libscapegoat.so
    COMMON_SOURCES += $(SOURCE_DIR)/tree_adapter.c
endif

LIB_FOLDERS = $(wildcard $(LIBS_DIR)/*_lib)

ALL_INCLUDES := $(wildcard $(LIBS_DIR)/*_lib/$(INCLUDE_DIR))
CFLAGS += -I$(INCLUDE_DIR) $(patsubst %, -I%, $(ALL_INCLUDES))

OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(COMMON_SOURCES))

.PHONY: all clean run build_libs compile

all : $(TARGET)

$(TARGET) : $(LIBS_BUILD) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(LIBS_BUILD): build_libs

build_libs :
	@mkdir -p $(BUILD_DIR)
	@for dir in $(LIB_FOLDERS); do $(MAKE) -C $$dir; done

$(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run : $(TARGET) 
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=ncurses.supp ./$(TARGET) $(size) $(win)

compile :
	$(MAKE) clean
	$(MAKE)

clean :
	rm -f $(TARGET)
	@for dir in $(LIB_FOLDERS); do $(MAKE) -C $$dir clean; done
	rm -f $(BUILD_DIR)/*
