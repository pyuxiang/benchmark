# CS2040C AY19/20 Sem 2 Lab Group B08
# Peh Yu Xiang, National University of Singapore
# 2020-04-25
#
# Makefile for automated compilation and testing
# Essentially a wrapper to make.bat on Windows, or runs UNIX shell
# commands instead for macOS and Linux platforms.
#
# To sniff actual process resources: https://stackoverflow.com/q/63166
# To limit stack size: https://stackoverflow.com/q/2275550
# - macOS, Linux: #include <sys/resource.h>... setrlimit in source
# - Windows: -Wl,--stack=<size_in_bytes> as g++ flags

SRC_DIR:=implementations
TEST_DIR:=tester
TARGET:=main

CLASSES:=$(notdir $(basename $(wildcard $(SRC_DIR)/*.cpp)))
TESTS:=$(addprefix $(TEST_DIR)/test-, $(addsuffix .cpp, $(CLASSES)))
FILES:=$(filter-out release, $(wildcard *))

EMPTY:=
SPACE:=$(EMPTY) $(EMPTY)
COMMA:=,
CATCH_FLAGS=-x 5 $(subst $(SPACE),$(COMMA),$(strip $(addprefix [, $(addsuffix ], $(CLASSES)))))
CCFLAGS:=-std=c++11 -Wall -I%CATCH_SINGLE_INCLUDE% # consider -Wl,--stack=NNN

.PHONY: all build run clean release
.SILENT: all build run clean release

ifeq ($(OS),Windows_NT)
all:main
	make.bat
build:
	make.bat build
run:
	make.bat run
clean:
	make.bat clean
release:
	make.bat release
else
all:
	make build --no-print-directory
	make run --no-print-directory
build:
ifeq ("$(wildcard $(TEST_DIR)/catch-unix.o)","")
	echo "[make] Compiling test framework..."
	g++ -c $(TEST_DIR)/catch.cpp -o $(TEST_DIR)/catch-unix.o
endif
	echo "[make] Compiling tests for:" $(CLASSES)
	g++ $(CCFLAGS) -o $(TARGET) $(TESTS) $(TEST_DIR)/catch-unix.o
	echo [make] Success, compiled to $(TARGET)
run:
ifneq ("$(wildcard $(TARGET))","")
	echo "[make] Running: \"./$(TARGET) $(CATCH_FLAGS)\""
	./$(TARGET) $(CATCH_FLAGS)
else
	echo "[make] Error, $(TARGET) does not exist"
endif
clean:
	rm -f $(TARGET)
release:
ifneq ("$(wildcard $(TEST_DIR)/catch-unix.o)","")
	rm -f $(TEST_DIR)/catch-unix.o
endif
ifneq ("$(wildcard $(TEST_DIR)/catch-windows.o)","")
	rm -f $(TEST_DIR)/catch-windows.o
endif
	tar -cvf release/tester-$(shell date +'%Y%m%d').tar $(FILES)
endif
