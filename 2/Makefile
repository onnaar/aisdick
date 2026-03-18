CC = gcc
CFLAGS = -fPIC -Wall -Wextra -Werror
LIB_NAME = libstack

IMPL ?= vector
TYPE ?= static 

ifeq ($(IMPL), vector)
    SRC = stack_vector.c
else
    SRC = stack_list.c
endif

ifeq ($(TYPE), dynamic)
    TARGET = $(LIB_NAME).so
    LDFLAGS = -shared
else
    TARGET = $(LIB_NAME).a
endif

.PHONY : all clean

all: $(TARGET)

$(LIB_NAME).so: stack.o
	$(CC) $(LDFLAGS) stack.o -o $@

$(LIB_NAME).a: stack.o
	ar rcs $@ stack.o

stack.o: $(SRC) stack.h
	$(CC) $(CFLAGS) -c $(SRC) -o stack.o

clean:
	rm -f *.o *.so *.a
