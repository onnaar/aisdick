MAKEFLAGS += --no-print-directory

TARGET = lab3
INPUT_LIB_DIR = libs/input_lib
TABLE_LIB_DIR = libs/table_lib
STACK_LIB_DIR = libs/stack_lib
INCLUDE_DIR = include
SOURCE_DIR = source
BUILD_DIR = build
DATA_DIR = datafiles

CC = gcc
CFLAGS = -I$(INCLUDE_DIR) -I$(INPUT_LIB_DIR)/$(INCLUDE_DIR) -I$(TABLE_LIB_DIR)/$(INCLUDE_DIR) -I$(STACK_LIB_DIR)/$(INCLUDE_DIR) -Wall -Wextra -Werror
LDFLAGS = -L$(INPUT_LIB_DIR) -L$(TABLE_LIB_DIR) -L$(STACK_LIB_DIR) -linput -ltable -lstack -Wl,-rpath,'$$ORIGIN/$(INPUT_LIB_DIR):$$ORIGIN/$(TABLE_LIB_DIR):$$ORIGIN/$(STACK_LIB_DIR)'

INPUT_LIB = $(INPUT_LIB_DIR)/libinput.so
TABLE_LIB = $(TABLE_LIB_DIR)/libtable.so
STACK_LIB = $(STACK_LIB_DIR)/libstack.so
DATA_FILES = $(wildcard $(DATA_DIR)/*)

OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(SOURCE_DIR)/*.c))

$(TARGET) : $(OBJECTS) $(INPUT_LIB) $(TABLE_LIB) $(STACK_LIB) $(DATA_FILES)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(INPUT_LIB) :
	$(MAKE) -C $(INPUT_LIB_DIR)

$(TABLE_LIB) :
	$(MAKE) -C $(TABLE_LIB_DIR)

$(STACK_LIB) :
	$(MAKE) -C $(STACK_LIB_DIR)

$(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(BUILD_DIR)/*.o $(TARGET)
	$(MAKE) -C $(INPUT_LIB_DIR) clean
	$(MAKE) -C $(TABLE_LIB_DIR) clean
	$(MAKE) -C $(STACK_LIB_DIR) clean
