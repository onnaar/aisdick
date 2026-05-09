MAKEFLAGS += --no-print-directory

INCLUDE_DIR = include 
BUILD_DIR = build
choice ?= s
SOURCE_DIR = source
ifeq ($(choice), t)
	SOURCE_DIR = timing
else ifeq ($(choice), a)
	SOURCE_DIR = additional
endif

LIBS_DIR = libs

INPUT_LIB_DIR = $(LIBS_DIR)/input_lib
BSTREE_LIB_DIR = $(LIBS_DIR)/bstree_lib
STACK_LIB_DIR = $(LIBS_DIR)/stack_lib

TARGET = lab4 
CC = gcc
CFLAGS = -Wall -Wextra -Werror
INPUT_LIB = $(BUILD_DIR)/libinput.so
BSTREE_LIB = $(BUILD_DIR)/libbstree.so
STACK_LIB = $(BUILD_DIR)/libstack.so

CFLAGS += -I$(INCLUDE_DIR) -I$(INPUT_LIB_DIR)/$(INCLUDE_DIR) -I$(BSTREE_LIB_DIR)/$(INCLUDE_DIR) -I$(STACK_LIB_DIR)/$(INCLUDE_DIR)
LDFLAGS = -L$(BUILD_DIR) -lbstree -lstack -linput -Wl,-rpath,'$$ORIGIN/$(BUILD_DIR)'
OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(SOURCE_DIR)/*.c))

LIBS_BUILD = $(BSTREE_LIB) $(STACK_LIB) $(INPUT_LIB)
 
$(TARGET) : $(OBJECTS) $(LIBS_BUILD)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(LIBS_BUILD) :
	$(MAKE) -C $(BSTREE_LIB_DIR)
	$(MAKE) -C $(STACK_LIB_DIR)
	$(MAKE) -C $(INPUT_LIB_DIR)

$(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

super : 
	$(MAKE) clean
	$(MAKE) run

run : $(TARGET) 
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

graphics:
	gnuplot -e "type='all'" plot.gp
	gnuplot -e "type='spec'" plot.gp
	gnuplot -e "type='traversing'" plot.gp

see:
	kitten icat graph*.png

clean : 
	rm -f $(TARGET) *.o
	$(MAKE) -C $(STACK_LIB_DIR) clean 
	$(MAKE) -C $(BSTREE_LIB_DIR) clean 
	$(MAKE) -C $(INPUT_LIB_DIR) clean
	rm -f $(BUILD_DIR)/*
