CC ?= gcc
OPT ?= -O3
WARN := -Wall -Wpedantic -Wextra -Wshadow
LINKS :=
INCLUDES := -Isrc
OPTIONS := -funsigned-char
CFLAGS += $(WARN) $(OPTIONS) $(OPT) $(INCLUDES)
TARGET ?= ljc
MAIN := bin.o
OBJS := common.o utf.o operators.o lexer.o display.o

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

all: clean $(TARGET)
	@printf "\nBuild success: \`$(TARGET)\`.\n"

test: clean $(OBJS) tests.o
	$(CC) $(OPT) -o $(TARGET)_test $(OBJS) tests.o $(LINKS)
	@printf "\nBuild for tests succeeded: \`$(TARGET)_test\`.\n"

$(TARGET): $(OBJS) $(MAIN)
	$(CC) $(OPT) -o $(TARGET) $(OBJS) $(MAIN) $(LINKS)

install: $(TARGET)
	@echo "Installing to $(PREFIX)/bin/$(TARGET)."
	install -d $(PREFIX)/bin
	install -m 755 $(TARGET) $(PREFIX)/bin

bin.o: common.o lexer.o display.o
	$(CC) $(CFLAGS) -c src/bin.c $(LINKS)

tests.o: common.o lexer.o display.o utf.o
	$(CC) $(CFLAGS) -c src/tests.c $(LINKS)

common.o:
	$(CC) $(CFLAGS) -c src/lovejoy/common.c $(LINKS)

utf.o: common.o
	$(CC) $(CFLAGS) -c src/lovejoy/utf.c $(LINKS)

lexer.o:
	$(CC) $(CFLAGS) -c src/lovejoy/lexer.c $(LINKS)

display.o:
	$(CC) $(CFLAGS) -c src/lovejoy/display.c $(LINKS)

operators.o:
	$(CC) $(CFLAGS) -c src/lovejoy/operators.c $(LINKS)

clean:
	@echo "Cleaning build."
	rm -f $(TARGET) $(OBJS) $(MAIN)

.PHONY: all clean install
