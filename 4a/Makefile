MAKEFLAGS += --no-print-directory

INPUT_LIB_DIR = libs/input_lib
BSTREE_LIB_DIR = libs/bstree_lib
INCLUDE_DIR = include 
BUILD_DIR = build
SOURCE_DIR = source


TARGET = sosi
CC = gcc
CFLAGS = -Wall -Wextra -Werror
INPUT_LIB = $(INPUT_LIB_DIR)/libinput.so
BSTREE_LIB = $(BSTREE_LIB_DIR)/libbstree.so

CFLAGS += -I$(INPUT_LIB_DIR)/$(INCLUDE_DIR) -I$(BSTREE_LIB_DIR)/$(INCLUDE_DIR)
LDFLAGS = -L$(INPUT_LIB_DIR) -L$(BSTREE_LIB_DIR) -linput -lbstree -Wl,-rpath,'$$ORIGIN/$(INPUT_LIB_DIR):$$ORIGIN/$(BSTREE_LIB_DIR)'
OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(SOURCE_DIR)/*.c))

$(TARGET) : $(OBJECTS) $(INPUT_LIB) $(BSTREE_LIB)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(INPUT_LIB) :
	$(MAKE) -C $(INPUT_LIB_DIR)

$(BSTREE_LIB) :
	$(MAKE) -C $(BSTREE_LIB_DIR)

$(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	rm -f $(TARGET) *.o
	$(MAKE) -C $(INPUT_LIB_DIR) clean
	$(MAKE) -C $(BSTREE_LIB_DIR) clean
