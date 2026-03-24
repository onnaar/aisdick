TARGET = bin/lab2
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
LIB_DIR = lib

PARENT_DIR = ..
CC = gcc

CFLAGS = -fPIC -Wall -Wextra -Werror -I$(INCLUDE_DIR)
SHARED_FLAG = -shared

IMPL ?= list 
TYPE ?= dynamic

LIB_STACK = stack.so
SRC_STACK = stack_list.c
LDFLAGS = -L$(LIB_DIR)

ifeq ($(IMPL), vector)
	LDFLAGS += -linput
	CFLAGS += -DSTACK_VECTOR
	SRC_STACK = stack_vector.c
endif

LIB_NAME = libstack
LIB_TARGET = $(LIB_NAME).so
ifeq ($(TYPE), static)
	LIB_TARGET = $(LIB_NAME).a
endif

.PHONY : all clean 

all : $(LIB_DIR)/$(LIB_TARGET) $(TARGET)

$(LIB_DIR)/$(LIB_NAME).so : $(BUILD_DIR)/stack.o
	$(CC) $(SHARED_FLAG) $< -o $@

$(LIB_DIR)/$(LIB_NAME).a : $(BUILD_DIR)/stack.o
	ar rcs $@ $<

$(BUILD_DIR)/stack.o : $(SRC_DIR)/$(SRC_STACK) $(INCLUDE_DIR)/stack.h
	$(CC) $(CFLAGS) -c $< -o $@

ifeq ($(IMPL), vector)
$(LIB_DIR)/libinput.so : $(BUILD_DIR)/input.o
	$(CC) $(SHARED_FLAG) $< -o $@

LIB_INPUT = $(LIB_DIR)/libinput.so

$(BUILD_DIR)/input.o : $(SRC_DIR)/input.c $(INCLUDE_DIR)/input.h
	$(CC) $(CFLAGS) -c $< -o $@
endif

LDFLAGS += -lstack -Wl,-rpath '$$ORIGIN/$(PARENT_DIR)/$(LIB_DIR)'
SRCS_TASK  = $(filter-out $(SRC_DIR)/stack_vector.c $(SRC_DIR)/stack_list.c $(SRC_DIR)/input.c, $(wildcard $(SRC_DIR)/*))
OBJ_TASK = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS_TASK))

$(TARGET) : $(OBJ_TASK) $(LIB_DIR)/$(LIB_TARGET) $(LIB_INPUT)
	$(CC) $(OBJ_TASK) -o $@ $(LDFLAGS) 

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	rm -f $(TARGET) $(BUILD_DIR)/* $(LIB_DIR)/*  
