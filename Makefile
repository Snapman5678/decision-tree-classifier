# Define the compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Define the directories
SRC_DIR = .
CLASSIFICATION_DIR = classification

# Define the source files and the object files
SRCS = $(SRC_DIR)/main.c $(CLASSIFICATION_DIR)/decision_tree_classifier.c
OBJS = $(SRCS:.c=.o)

# Define the output executable
TARGET = main

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -I$(CLASSIFICATION_DIR) -c $< -o $@

# Clean rule
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets
.PHONY: all clean
