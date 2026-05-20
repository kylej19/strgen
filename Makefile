CC=gcc
CCFLAGS=-Wall -Wextra -std=c99 -g

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

TARGET=$(BIN_DIR)/testpass
SOURCES=$(wildcard $(SRC_DIR)/*.c)
OBJECTS=$(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

#linking
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CCFLAGS) $(OBJECTS) -o $(TARGET)

#compiling
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -rfv $(OBJ_DIR) $(BIN_DIR)

run:
	./$(TARGET)

.PHONY: all clean run
