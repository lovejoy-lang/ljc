#!/bin/sh

TARGET=ljc_debug CFLAGS=-ggdb OPT=-Og make
gdb --args ./ljc_debug $@
