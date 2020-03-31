SRC_DIR := src
INC_DIR := include
LIB_DIR := lib
OBJ_DIR := build
BIN_DIR := bin

SRC := $(wildcard $(SRC_DIR)/*.c)
LIB := -l$(wildcard $(LIB_DIR)/*)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BIN := $(notdir $(shell pwd))

INCFLAG := -I${INC_DIR}
LIBFLAG := -L${LIB_DIR}

FLAG := -Wall

all: $(BIN_DIR)/$(BIN)

$(BIN_DIR)/$(BIN): $(OBJ) | $(BIN_DIR)
	$(CC) $(LIBFLAG) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(INCFLAG) $(FLAG) -c $< -o $@

$(OBJ_DIR):
	mkdir $@

$(BIN_DIR):
	mkdir $@

run:
	$(BIN_DIR)/$(BIN)

clean:
	$(RM) -r $(OBJ_DIR)
	$(RM) -r $(BIN_DIR)

.PHONY: all run clean
