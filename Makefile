CC := gcc
OPT := -O3
WARN := -Wall -Wpedantic -Wextra -Wshadow
LINKS := -lpthread
INCLUDES := -Isrc
CFLAGS := $(WARN) $(OPT) $(INCLUDES)
TARGET := ljc
OBJS := bin.o

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

bin.o:
	$(CC) -c $(CFLAGS) src/bin.c $(LINKS)

clean:
	@echo "Cleaning build."
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean install
