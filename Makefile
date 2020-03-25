CC ?= gcc
LD ?= gcc

SRC_DIR := src
INC_DIR := include
OBJ_DIR := build
BIN_DIR := bin

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BIN := main

CFLAGS := -I${INC_DIR}

all: $(BIN_DIR)/$(BIN)

$(BIN_DIR)/$(BIN): $(OBJ) | $(BIN_DIR)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $@

$(BIN_DIR):
	mkdir $@

clean:
	$(RM) -r $(OBJ_DIR)
	$(RM) -r $(BIN_DIR)

.PHONY: all clean
