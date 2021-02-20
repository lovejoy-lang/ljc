#!/bin/sh

TEST=false

printf "Run tests.c file instead of bin.c? [Y/n]: "
read -r REPLY
TEST_MODE="$(echo "$REPLY" | awk '{print tolower($0)}')"
[ -z "$TEST_MODE" ] && TEST=true
[ "$TEST_MODE" = y ] && TEST=true

make clean
export TARGET=ljc_debug
export CFLAGS=-ggdb
export OPT=-Og

EXEC="$TARGET"
if $TEST; then
	make clean test
	EXEC="./${EXEC}_test"
else
	make clean
	EXEC="./${EXEC}"
fi

printf "Run \`gdb --args %s %s\`? [y/N]: " "$EXEC" "$*"
read -r REPLY
RUN_GDB="$(echo "$REPLY" | awk '{print tolower($0)}')"
[ -z "$RUN_GDB" ] && RUN_GDB="n"
[ "$RUN_GDB" = y ] && RUN_GDB=true || RUN_GDB=false

$RUN_GDB && gdb --args "$EXEC" "$@"
