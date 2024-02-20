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
	@./$(EXECUTABLE) < input.txt > output.txt
	@echo "Comparing output with right_result.txt..."
	@if diff output.txt right_result.txt > /dev/null; then \
		echo "Test passed!"; \
	else \
		echo "Test failed!"; \
	fi
	

.PHONY: all clean test
