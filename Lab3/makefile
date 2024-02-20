CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic

SRC = main.c morse_filter.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = morse_filter

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)

test: $(EXECUTABLE)
	@echo "Testing Morse Filter program..."
	@./$(EXECUTABLE) < input.txt

.PHONY: all clean test

