# Makefile for lubtype test runner (Linux/macOS)
# Copyright (c) 2026 paulsinclair51
# SPDX-License-Identifier: MIT

CC ?= clang
CFLAGS ?= -std=c11 -Wall -Wextra -O2
CPPFLAGS ?= -I..

TEST_SOURCES = \
	lubtype_tests.c \
	test_error_edge.c \
	test_advanced_ops.c \
	test_cmp_search.c \
	test_strlen_validation.c \
	test_charclass.c \
	test_reserved_matrix.c \
	test_search_families.c \
	test_span_count.c \
	test_core_families.c \
	test_type_matrix.c \
	test_utilities.c \
	test_fuzz_edge.c \
	test_skip.c

all: lubtype_tests

lubtype_tests: $(TEST_SOURCES)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $(TEST_SOURCES)

run: lubtype_tests
	./lubtype_tests

clean:
	rm -f lubtype_tests lubtype_tests_report.txt

.PHONY: all run clean
