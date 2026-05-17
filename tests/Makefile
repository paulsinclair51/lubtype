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
	test_reserved_matrix.c \
	test_type_matrix.c \
	test_fuzz_edge.c \
	test_charclass_fail.c \
	test_count_cex.c \
	test_skip_aex.c

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

# X-macro strlen_strclass object files (compiled from same source, different defines)
XSTRLENSTRCLASS_L_OBJ = test_strlen_strclass_l.o
XSTRLENSTRCLASS_U_OBJ = test_strlen_strclass_u.o

# X-macro count object files (compiled from same source, different defines)
XCOUNT_L_OBJ = test_count_l.o
XCOUNT_U_OBJ = test_count_u.o

# X-macro search_families object files (compiled from same source, different defines)
XSEARCHFAMILY_L_OBJ = test_search_families_l.o
XSEARCHFAMILY_U_OBJ = test_search_families_u.o

# X-macro core_families object files (compiled from same source, different defines)
XCOREFAMILY_L_OBJ = test_core_families_l.o
XCOREFAMILY_U_OBJ = test_core_families_u.o

# X-macro utilities object files (compiled from same source, different defines)
XUTILITIES_L_OBJ = test_utilities_l.o
XUTILITIES_U_OBJ = test_utilities_u.o

# X-macro skip object files (compiled from same source, different defines)
XSKIP_L_OBJ = test_skip_l.o
XSKIP_U_OBJ = test_skip_u.o

# X-macro alias coverage object files (compiled from same source, different defines)
XXMACROS_L_OBJ = test_xmacros_l.o
XXMACROS_U_OBJ = test_xmacros_u.o

# All x-macro objects combined
X_OBJECTS = $(XERROREDGE_L_OBJ) $(XERROREDGE_U_OBJ) \
            $(XADVANCEDOPS_L_OBJ) $(XADVANCEDOPS_U_OBJ) \
            $(XCHARCLASS_L_OBJ) $(XCHARCLASS_U_OBJ) \
            $(XCMPSEARCH_L_OBJ) $(XCMPSEARCH_U_OBJ) \
			$(XSTRLENSTRCLASS_L_OBJ) $(XSTRLENSTRCLASS_U_OBJ) \
			$(XCOUNT_L_OBJ) $(XCOUNT_U_OBJ) \
			$(XSEARCHFAMILY_L_OBJ) $(XSEARCHFAMILY_U_OBJ) \
			$(XCOREFAMILY_L_OBJ) $(XCOREFAMILY_U_OBJ) \
			$(XUTILITIES_L_OBJ) $(XUTILITIES_U_OBJ) \
			$(XSKIP_L_OBJ) $(XSKIP_U_OBJ) \
			$(XXMACROS_L_OBJ) $(XXMACROS_U_OBJ)

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

# Explicit rules for x-macro count object files
$(XCOUNT_L_OBJ): test_count.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_L -c $< -o $@

$(XCOUNT_U_OBJ): test_count.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_U -c $< -o $@

# Explicit rules for x-macro search_families object files
$(XSEARCHFAMILY_L_OBJ): test_search_families.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_L -c $< -o $@

$(XSEARCHFAMILY_U_OBJ): test_search_families.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_U -c $< -o $@

# Explicit rules for x-macro core_families object files
$(XCOREFAMILY_L_OBJ): test_core_families.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_L -c $< -o $@

$(XCOREFAMILY_U_OBJ): test_core_families.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_U -c $< -o $@

# Explicit rules for x-macro utilities object files
$(XUTILITIES_L_OBJ): test_utilities.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_L -c $< -o $@

$(XUTILITIES_U_OBJ): test_utilities.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_U -c $< -o $@

# Explicit rules for x-macro skip object files
$(XSKIP_L_OBJ): test_skip.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_L -c $< -o $@

$(XSKIP_U_OBJ): test_skip.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_U -c $< -o $@

# Explicit rules for x-macro alias coverage object files
$(XXMACROS_L_OBJ): test_xmacros.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_L -c $< -o $@

$(XXMACROS_U_OBJ): test_xmacros.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_U -c $< -o $@

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

# Explicit rules for x-macro strlen_strclass object files
$(XSTRLENSTRCLASS_L_OBJ): test_strlen_strclass.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -DLUB_X_IS_L -c $< -o $@

$(XSTRLENSTRCLASS_U_OBJ): test_strlen_strclass.c
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
