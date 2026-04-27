INCLUDE_DIR = include
SOURCE_DIR = source
BUILD_DIR = build
ROOT_PATH = ../..
PROJECT_ROOT_BUILD_DIR = $(ROOT_PATH)/$(BUILD_DIR)
ADDITIONAL_INCLUDE_DIR = -I../stack_lib/$(INCLUDE_DIR) -I../input_lib/$(INCLUDE_DIR)

CC = gcc
LIB = libbstree.so
CFLAGS += -I$(INCLUDE_DIR) $(ADDITIONAL_INCLUDE_DIR) -fPIC -Wall -Wextra -Werror
LDFLAGS = -shared

OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(PROJECT_ROOT_BUILD_DIR)/%.o, $(wildcard $(SOURCE_DIR)/*))

$(PROJECT_ROOT_BUILD_DIR)/$(LIB) : $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(PROJECT_ROOT_BUILD_DIR)/%.o : $(SOURCE_DIR)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	rm -f $(PROJECT_ROOT_BUILD_DIR)/$(LIB) $(OBJECTS) 
