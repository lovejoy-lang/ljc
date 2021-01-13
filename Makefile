CC ?= gcc
OPT ?= -O3
WARN := -Wall -Wpedantic -Wextra -Wshadow
LINKS :=
INCLUDES := -Isrc
OPTIONS := -funsigned-char
CFLAGS += $(WARN) $(OPTIONS) $(OPT) $(INCLUDES)
TARGET ?= ljc
CDIR ?= src/lovejoy
ODIR ?= o
OBJS := $(patsubst $(CDIR)/%.c,$(ODIR)/%.o,$(wildcard $(CDIR)/*.c))
MAIN := $(ODIR)/bin.o
TESTS_MAIN := $(ODIR)/tests.o

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

all: pre-build $(TARGET)
	@printf "\nBuild success: \`$(TARGET)\`.\n"

clean:
	@echo "Cleaning last build."
	rm -f ./$(TARGET) ./$(TARGET)_test
	rm -fr ./$(ODIR)/

pre-build: clean
	@echo "Making object directory."
	@mkdir $(ODIR)

test: pre-build $(ODIR)/tests.o
	$(CC) $(OPT) -o $(TARGET)_test $(OBJS) $(TESTS_MAIN) $(LINKS)
	@printf "\nBuild for tests succeeded: \`$(TARGET)_test\`.\n"

$(TARGET): $(MAIN)
	@echo "Building target."
	$(CC) $(OPT) -o $(TARGET) $(OBJS) $(MAIN) $(LINKS)

install: $(TARGET)
	@echo "Installing to $(PREFIX)/bin/$(TARGET)."
	install -d $(PREFIX)/bin
	install -m 755 $(TARGET) $(PREFIX)/bin

$(MAIN): $(OBJS)
	@echo "Building main."
	$(CC) $(CFLAGS) -c src/bin.c -o $@ $(LINKS)

$(ODIR)/tests.o: $(OBJS)
	@echo "Building tests."
	$(CC) $(CFLAGS) -c src/tests.c -o $@ $(LINKS)

$(ODIR)/%.o: $(CDIR)/%.c
	@echo "Building object file: $< -> $@."
	@$(CC) $(CFLAGS) -c $< -o $@ $(LINKS)



.PHONY: all test pre-build clean install
