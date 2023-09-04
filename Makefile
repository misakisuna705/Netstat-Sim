SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BIN := main

INC_DIR := include
LIB_DIR := lib

INCFLAG := -I${INC_DIR}
LIBFLAG := -L${LIB_DIR}

FLAG := -Wall

all: $(BIN_DIR)/$(BIN)

$(BIN_DIR)/$(BIN): $(OBJ) | $(BIN_DIR)
	$(CC) $(LIBFLAG) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(INCFLAG) $(FLAG) -c $< -o $@

$(OBJ_DIR): $(INC_DIR)
	mkdir $@

$(BIN_DIR): $(LIB_DIR)
	mkdir $@

$(INC_DIR):
	mkdir $@

$(LIB_DIR):
	mkdir $@

run:
	$(BIN_DIR)/$(BIN) $(ARG)

clean:
	$(RM) -r $(OBJ_DIR)
	$(RM) -r $(BIN_DIR)

.PHONY: all run clean
