CC ?= gcc

TARGET ?= ljc
CDIR ?= src/lovejoy
ODIR ?= o
DDIR ?= d
OBJS := $(patsubst $(CDIR)/%.c,$(ODIR)/%.o,$(wildcard $(CDIR)/*.c))
DEPS := $(patsubst $(CDIR)/%.c,$(DDIR)/%.d,$(wildcard $(CDIR)/*.c))

OPT ?= -O3
WARN := -Wall -Wpedantic -Wextra -Wshadow
LINKS :=
INCLUDES := -Isrc
OPTIONS := -funsigned-char -std=gnu17
DEPFLAGS = -MT $@ -MMD -MP -MF $(DDIR)/$(*F).d
CFLAGS += $(DEPFLAGS) $(WARN) $(OPTIONS) $(OPT) $(INCLUDES)

ifeq ($(CC),clang)
	WARN += -Wno-gnu-zero-variadic-macro-arguments -Wno-language-extension-token
	OPTIONS += -fms-extensions
endif
ifeq ($(CC),gcc)
	OPTIONS += -fplan9-extensions
endif
ifeq ($(OPT),-O0)
	OPTIONS += -finline-functions
endif

MAIN := $(ODIR)/bin.o
TESTS_MAIN := $(ODIR)/tests.o

NOW := $(shell date +%s.%N)  # Eager.
END  = $(shell date +%s.%N)  # Lazy.
TIME = $(shell dc -e "20 k $(END) $(NOW) - 1000 * p")

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

bold := $(shell tput bold)
dim := $(shell tput dim)
r := $(shell tput sgr0)

begin_command := @printf "$(dim)> "
end_command := @printf "$(r)"

all: pre-build $(TARGET)
	$(eval T := $(TIME))
	@printf "$(bold)\nBuild success:$(r) \`$(TARGET)\`.\n"
	@printf "$(bold)Took:$(r) %g ms.\n" "$(T)"

clean:
	@echo "[~] Cleaning last build."
	$(begin_command)
	rm -f ./$(TARGET) ./$(TARGET)_test
	$(end_command)
	$(begin_command)
	rm -fr ./$(ODIR)/ ./$(DDIR)/
	$(end_command)

$(ODIR):
	@mkdir $@
	@echo "[~] Making object directory: $@/"

$(DDIR):
	@mkdir $@
	@echo "[~] Making dependency directory: $@/"

pre-build: $(ODIR) $(DDIR)

test: pre-build $(ODIR)/tests.o
	$(eval T := $(TIME))
	$(begin_command)
	$(CC) $(OPT) -o $(TARGET)_test $(OBJS) $(TESTS_MAIN) $(LINKS)
	$(end_command)
	@printf "$(bold)\nBuild for tests succeeded:$(r) \`$(TARGET)_test\`.\n"
	@printf "$(bold)Took:$(r) %g ms.\n" "$(T)"

$(TARGET): $(MAIN)
	@echo "$(bold)Building target.$(r)"
	$(begin_command)
	$(CC) $(OPT) -o $(TARGET) $(OBJS) $(MAIN) $(LINKS)
	$(end_command)

install: $(TARGET)
	@echo "Installing to $(PREFIX)/bin/$(TARGET)."
	$(begin_command)
	install -d $(PREFIX)/bin
	$(end_command)
	$(begin_command)
	install -m 755 $(TARGET) $(PREFIX)/bin
	$(end_command)

$(MAIN): src/bin.c $(OBJS)
	@echo "$(bold)Building main.$(r)"
	$(begin_command)
	$(CC) $(CFLAGS) -c src/bin.c -o $@ $(LINKS)
	$(end_command)

$(ODIR)/tests.o: src/tests.c $(OBJS)
	@echo "$(bold)Building tests.$(r)"
	$(begin_command)
	$(CC) $(CFLAGS) -c src/tests.c -o $@ $(LINKS)
	$(end_command)

$(ODIR)/%.o: $(CDIR)/%.c $(DDIR)/%.d
	@printf "$(bold)Building object file.$(r) %25s  ->  %13s\n" "$<" "$@"
	@$(CC) $(CFLAGS) -c $< -o $@ $(LINKS)

$(DEPS):

include $(wildcard $(DEPS))

.PHONY: all test pre-build clean install
