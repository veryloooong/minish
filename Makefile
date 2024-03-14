# The target
TARGET := minish

# Paths
SRC_PATH := ./src
BUILD_PATH := ./build


# Source files
SRCS := $(wildcard $(SRC_PATH)/*.c)
OBJS := $(SRCS:$(SRC_PATH)/%.c=$(BUILD_PATH)/%.o)

INCLUDE := -Iinclude

# Compiler
CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -std=c11 -O2 $(INCLUDE)

# Build
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Link
$(BUILD_PATH)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean
.PHONY: clean
clean:
	rm -rf $(BUILD_PATH) $(TARGET)