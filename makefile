# MakeFile 2.2 
# Created by Víctor Garrido, 2019
# Features: Takes all the source files automatically and compiles into a user defined executable
# Usage: Just change the variable EXECUTABLE with the name you want to give it

MAKE_DIR = $(PWD)
ROOT_DIR := $(MAKE_DIR)

# Compiler to use
CC = gcc

# Compiler flags . The -I aparrently looks the header everywhere in directories
CFLAGS = -Wall -Wextra -g -I.

# Linker Flags
LDFLAGS = -lpthread

SUBDIRS := $(wildcard */.)

# Source .c files: Takes all .c files in the source folder
#SOURCES=$(wildcard *.c)
SOURCES := $(shell find $(ROOT_DIR) -type f -name "*.c")

# All the object files (.o) created from their correspondant .c
OBJECTS=$(SOURCES:.c=.o)

# Executable name
EXECUTABLE = trinity

# In order to delete de .o generated it needs to create de .o and do the executable target --> End of compilation
all: $(SOURCES) $(EXECUTABLE)
	rm -f *.o *~

# In order to do the executable target it needs to create de .o
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	rm $(TARGET) $(OBJECTS)