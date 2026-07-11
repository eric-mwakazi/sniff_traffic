CC = gcc
CFLAGS = -Wall -Wextra -pedantic
SRC = main.c
OUT_DIR = outputs
TARGET = $(OUT_DIR)/main

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
