CC=gcc
CFLAGS=-Wall

.PHONY: all clean start test

all: main

main: main.o
	$(CC) $(CFLAGS) main.o -o main

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

start: main
	./main

test: main
	@echo "Running tests..."
	./main
	@echo "Test passed successfully."


	@echo "Checking process termination..."
	@PROCESS_COUNT=$$(ps aux | grep "./main" | grep -v "grep" | wc -l); \
	if [ "$$PROCESS_COUNT" -eq 0 ]; then \
		echo "Process termination check passed"; \
	else \
		echo "Process termination check failed"; \
	fi

clean:
	rm -f main main.o

