MAKEFLAGS += --no-print-directory

CC = gcc
CFLAGS = -Wall -Wextra -Werror
TARGET = lab5 

INCLUDE_DIR = include 
BUILD_DIR = build
SOURCE_DIR = source
LIBS_DIR = libs

ALL_INCLUDES := $(wildcard $(LIBS_DIR)/*_lib/$(INCLUDE_DIR))
CFLAGS += -I$(INCLUDE_DIR) $(patsubst %, -I%, $(ALL_INCLUDES))

LDFLAGS = -L$(BUILD_DIR) -lgraph -lqueue -linput -lhtable -lstack -Wl,-rpath,'$$ORIGIN/$(BUILD_DIR)'
OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(SOURCE_DIR)/*.c))

LIB_FOLDERS = $(wildcard $(LIBS_DIR)/*_lib)
LIBS_BUILD = $(BUILD_DIR)/libinput.so $(BUILD_DIR)/libgraph.so $(BUILD_DIR)/libqueue.so $(BUILD_DIR)/libhtable.so $(BUILD_DIR)/libstack.so

.PHONY: all clean run super $(LIB_FOLDERS)

all: $(TARGET)

$(TARGET): $(LIBS_BUILD) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(LIBS_BUILD): $(LIB_FOLDERS)

$(LIB_FOLDERS):
	$(MAKE) -C $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

super:
	$(MAKE) clean
	$(MAKE) run

run: $(TARGET) 
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

clean: 
	rm -f $(TARGET)
	rm -rf $(BUILD_DIR)/*
	@for dir in $(LIB_FOLDERS); do $(MAKE) -C $$dir clean; done
