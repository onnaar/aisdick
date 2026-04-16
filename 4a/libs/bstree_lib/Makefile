INCLUDE_DIR = include
BUILD_DIR = build
SOURCE_DIR = source

CC = gcc
TARGET = libbstree.so
CFLAGS += -I$(INCLUDE_DIR) -fPIC -Wall -Wextra -Werror
LDFLAGS = -shared

OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(SOURCE_DIR)/*))

$(TARGET) : $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	rm -f $(TARGET) $(BUILD_DIR)/* 
