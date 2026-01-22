CC ?= cc
AR ?= ar
ARFLAGS ?= rcs
CFLAGS ?= -std=c99 -Wall -Wextra -Wpedantic -Iinclude
SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)
TESTS_SRC := $(wildcard tests/*.c)
TESTS_BIN := $(TESTS_SRC:.c=)

.PHONY: all examples tests test clean

all: libalgokit.a

examples: libalgokit.a
	$(MAKE) -C examples

tests: libalgokit.a $(TESTS_BIN)

test: tests
	@set -e; for t in $(TESTS_BIN); do echo "Running $$t"; $$t; done
	$(MAKE) -C examples test

tests/%: tests/%.c libalgokit.a
	$(CC) $(CFLAGS) $< -L. -lalgokit -o $@

libalgokit.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) libalgokit.a $(TESTS_BIN)
	$(MAKE) -C examples clean
