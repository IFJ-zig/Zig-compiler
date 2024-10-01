#############################################################
# Makefile
# Projekt: Implementace překladače imperativního jazyka IFJ24
# Tvůrci: Ivo Puchnar, xpuchn02
#############################################################

# Directories
SOURCE_DIR      = src
BUILD_DIR       = build

# Compiler settings
CC              = gcc
CFLAGS          = -std=gnu99 -Wall -Wextra -Werror
LDFLAGS         = -o ifj24.out

# Find source files
SOURCES         := $(shell find $(SOURCE_DIR) -type f -name '*.c')
OBJECTS         := $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# Phony targets
.PHONY: all clean

# Default target
all: $(BUILD_DIR) prekladac

# Create the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build the executable
prekladac: $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) $(LDFLAGS)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Cleanup
clean:
	rm -rf $(BUILD_DIR) ifj24.out
