CC := gcc
OPT := -O3
WARN := -Wall -Wpedantic -Wextra -Wshadow
LINKS := -lpthread
INCLUDES := -Isrc
GCC_OPTIONS := -funsigned-char
CFLAGS := $(WARN) $(OPT) $(INCLUDES) $(GCC_OPTIONS)
TARGET := ljc
OBJS := bin.o lexer.o display.o

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

all: clean $(TARGET)
	@printf "\nBuild success.\n"

$(TARGET): $(OBJS)
	$(CC) $(OPT) -o $(TARGET) $(OBJS) $(LINKS)

install: $(TARGET)
	@echo "Installing to $(PREFIX)/bin/$(TARGET)."
	install -d $(PREFIX)/bin
	install -m 755 $(TARGET) $(PREFIX)/bin

bin.o: lexer.o display.o
	$(CC) -c $(CFLAGS) src/bin.c $(LINKS)

lexer.o:
	$(CC) -c $(CFLAGs) src/lovejoy/lexer.c $(LINKS)

display.o:
	$(CC) -c $(CFLAGS) src/lovejoy/display.c $(LINKS)

clean:
	@echo "Cleaning build."
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean install
