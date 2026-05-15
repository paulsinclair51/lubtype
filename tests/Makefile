# Makefile for lubtype test runner (Linux/macOS)
# Copyright (c) 2026 paulsinclair51
# SPDX-License-Identifier: MIT

CC ?= clang
CFLAGS ?= -std=c11 -Wall -Wextra -O2
CPPFLAGS ?= -I..

LUB_DEFINITIONS_SRC = ../lubdefinitions.c
LUB_DEFINITIONS_OBJ = lubdefinitions.o

TEST_SOURCES = \
	lubtype_tests.c \
	test_error_edge.c \
	test_advanced_ops.c \
	test_strlen_validation.c \
	test_reserved_matrix.c \
	test_search_families.c \
	test_span_count.c \
	test_core_families.c \
	test_type_matrix.c \
	test_utilities.c \
	test_fuzz_edge.c \
	test_skip.c

TEST_OBJECTS = $(TEST_SOURCES:.c=.o)

# x-macro charclass object files (compiled from same source, different defines)
XCHARCLASS_L_OBJ = test_charclass_l.o
XCHARCLASS_U_OBJ = test_charclass_u.o

# x-macro cmp_search object files (compiled from same source, different defines)
XCMPSEARCH_L_OBJ = test_cmp_search_l.o
XCMPSEARCH_U_OBJ = test_cmp_search_u.o

all: lubtype_tests

# Build lubdefinitions.o (provides all API function bodies)
$(LUB_DEFINITIONS_OBJ): $(LUB_DEFINITIONS_SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Pattern rule for all normal .c -> .o
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


# Explicit rules for x-macro charclass object files
$(XCHARCLASS_L_OBJ): test_charclass.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_L -c $< -o $@

$(XCHARCLASS_U_OBJ): test_charclass.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_U -c $< -o $@

# Explicit rules for x-macro cmp_search object files
$(XCMPSEARCH_L_OBJ): test_cmp_search.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_L -c $< -o $@

$(XCMPSEARCH_U_OBJ): test_cmp_search.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_U -c $< -o $@


# Link one test suite executable with all x-macro variants.
lubtype_tests: $(TEST_OBJECTS) $(XCHARCLASS_L_OBJ) $(XCHARCLASS_U_OBJ) $(XCMPSEARCH_L_OBJ) $(XCMPSEARCH_U_OBJ) $(LUB_DEFINITIONS_OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $(TEST_OBJECTS) $(XCHARCLASS_L_OBJ) $(XCHARCLASS_U_OBJ) $(XCMPSEARCH_L_OBJ) $(XCMPSEARCH_U_OBJ) $(LUB_DEFINITIONS_OBJ)
	rm -f $(TEST_OBJECTS) $(XCHARCLASS_L_OBJ) $(XCHARCLASS_U_OBJ) $(XCMPSEARCH_L_OBJ) $(XCMPSEARCH_U_OBJ) $(LUB_DEFINITIONS_OBJ)

run: lubtype_tests
	./lubtype_tests

clean:
	rm -f *.o lubtype_tests

.PHONY: all run clean
