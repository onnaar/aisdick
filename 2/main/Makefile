TARGET = lab2
CC = gcc
PARENT_DIR = ..
IMPL ?= list 
TYPE ?= dynamic
CFLAGS = -I$(PARENT_DIR) -Wall -Wextra -Werror
LDFLAGS = -L$(PARENT_DIR) -lstack -Wl,-rpath,'$$ORIGIN/$(PARENT_DIR)'
OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c))
STACK_OBJ = $(PARENT_DIR)/stack.o

$(TARGET) : $(OBJECTS)
	$(MAKE) -C $(PARENT_DIR) all IMPL=$(IMPL) TYPE=$(TYPE)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o : %.c *.h 
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(TARGET) $(OBJECTS) 
	$(MAKE) -C $(PARENT_DIR) clean
