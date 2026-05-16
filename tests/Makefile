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
	test_strlen_validation.c \
	test_reserved_matrix.c \
	test_search_families.c \
	test_count.c \
	test_core_families.c \
	test_type_matrix.c \
	test_utilities.c \
	test_fuzz_edge.c \
	test_skip.c

TEST_OBJECTS = $(TEST_SOURCES:.c=.o)

# X-macro error_edge object files (compiled from same source, different defines)
XERROREDGE_L_OBJ = test_error_edge_l.o
XERROREDGE_U_OBJ = test_error_edge_u.o

# X-macro advanced ops object files (compiled from same source, different defines)
XADVANCEDOPS_L_OBJ = test_advanced_ops_l.o
XADVANCEDOPS_U_OBJ = test_advanced_ops_u.o

# X-macro charclass object files (compiled from same source, different defines)
XCHARCLASS_L_OBJ = test_charclass_l.o
XCHARCLASS_U_OBJ = test_charclass_u.o

# X-macro cmp_search object files (compiled from same source, different defines)
XCMPSEARCH_L_OBJ = test_cmp_search_l.o
XCMPSEARCH_U_OBJ = test_cmp_search_u.o

# All x-macro objects combined
X_OBJECTS = $(XERROREDGE_L_OBJ) $(XERROREDGE_U_OBJ) $(XADVANCEDOPS_L_OBJ) $(XADVANCEDOPS_U_OBJ) $(XCHARCLASS_L_OBJ) $(XCHARCLASS_U_OBJ) $(XCMPSEARCH_L_OBJ) $(XCMPSEARCH_U_OBJ)

all: lubtype_tests

# Build lubdefinitions.o (provides all API function bodies)
$(LUB_DEFINITIONS_OBJ): $(LUB_DEFINITIONS_SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Pattern rule for all normal .c -> .o
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Explicit rules for x-macro error_edge object files
$(XERROREDGE_L_OBJ): test_error_edge.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_L -c $< -o $@

$(XERROREDGE_U_OBJ): test_error_edge.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_U -c $< -o $@

# Explicit rules for x-macro advanced ops object files
$(XADVANCEDOPS_L_OBJ): test_advanced_ops.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_L -c $< -o $@

$(XADVANCEDOPS_U_OBJ): test_advanced_ops.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_U -c $< -o $@

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
lubtype_tests: $(TEST_OBJECTS) $(X_OBJECTS) $(LUB_DEFINITIONS_OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $(TEST_OBJECTS) $(X_OBJECTS) $(LUB_DEFINITIONS_OBJ)
	rm -f $(TEST_OBJECTS) $(X_OBJECTS) $(LUB_DEFINITIONS_OBJ)

run: lubtype_tests
	./lubtype_tests

clean:
	rm -f *.o lubtype_tests

.PHONY: all run clean
