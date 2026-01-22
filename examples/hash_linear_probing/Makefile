CC ?= cc
CFLAGS ?= -std=c99 -Wall -Wextra -Wpedantic -I../../include
LDFLAGS ?= -L../..
LDLIBS ?= -lalgokit

BIN := hash_linear_probing

.PHONY: all clean test

all: $(BIN)

test: $(BIN)
	./$(BIN)

$(BIN): main.c
	$(CC) $(CFLAGS) $< $(LDFLAGS) $(LDLIBS) -o $@

clean:
	rm -f $(BIN)
