CC ?= gcc
AR ?= ar
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c11 -Iinclude -I.
LDFLAGS ?=

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
LIB_DIR := $(BUILD_DIR)/lib

SRC_DIR := src
TEST_DIR := test

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
SRC_OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/src/%.o,$(SRC_FILES))

TEST_SOURCES := $(wildcard $(TEST_DIR)/test_*.c)
TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/test/%.o,$(TEST_SOURCES))
TEST_BINS := $(patsubst $(TEST_DIR)/%.c,$(BIN_DIR)/%,$(TEST_SOURCES))

LIB_NAME := libadt.a
LIB_PATH := $(LIB_DIR)/$(LIB_NAME)

.PHONY: all lib tests test clean directories test_stack test_queue test_list test_tree test_hash_table

all: lib tests

directories:
	@mkdir -p $(OBJ_DIR)/src $(OBJ_DIR)/test $(BIN_DIR) $(LIB_DIR)

lib: $(LIB_PATH)

$(LIB_PATH): $(SRC_OBJECTS) | directories
	$(AR) rcs $@ $^

$(OBJ_DIR)/src/%.o: $(SRC_DIR)/%.c | directories
	$(CC) $(CFLAGS) -c $< -o $@

tests: lib $(TEST_BINS)

test: tests

$(OBJ_DIR)/test/%.o: $(TEST_DIR)/%.c | directories
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/test_%: $(OBJ_DIR)/test/test_%.o $(LIB_PATH) | directories
	$(CC) $< -L$(LIB_DIR) -ladt $(LDFLAGS) -o $@

test_stack: $(BIN_DIR)/test_stack

test_queue: $(BIN_DIR)/test_queue

test_list: $(BIN_DIR)/test_list

test_tree: $(BIN_DIR)/test_tree

test_hash_table: $(BIN_DIR)/test_hash_table

clean:
	rm -rf $(BUILD_DIR)
