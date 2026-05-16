/**
 * @file lubtype_test_declarations.h
 * @brief Common test declarations for lubtype tests.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#ifndef LUBTYPE_TEST_DECLARATIONS_H
#define LUBTYPE_TEST_DECLARATIONS_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static inline void lubtype_assert_fail(const char *expr, const char *file, int line) {
	fprintf(stderr, "Assertion failed: %s (%s:%d)\n", expr, file, line);
	abort();
}

#define LUB_ASSERT(expr) \
	do { \
		++test_count; \
		if (!(expr)) { \
			lubtype_assert_fail(#expr, __FILE__, __LINE__); \
		} \
	} while (0)

extern size_t run_error_edge_tests(void);
extern size_t run_advanced_ops_tests_l(void);
extern size_t run_advanced_ops_tests_u(void);
extern size_t run_cmp_search_tests_l(void);
extern size_t run_cmp_search_tests_u(void);
extern size_t run_strlen_validation_tests(void);
extern size_t run_charclass_tests_l(void);
extern size_t run_charclass_tests_u(void);
extern size_t run_reserved_matrix_tests(void);
extern size_t run_search_family_tests(void);
extern size_t run_span_count_tests(void);
extern size_t run_core_family_tests(void);
extern size_t run_type_matrix_tests(void);
extern size_t run_utilities_tests(void);
extern size_t run_fuzz_edge_tests(void);
extern size_t run_skip_tests(void);

#endif
