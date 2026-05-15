/**
 * @file test_declarations.h
 * @brief Shared declarations for all lubtype test modules.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#ifndef LUBTYPE_TEST_DECLARATIONS_H
#define LUBTYPE_TEST_DECLARATIONS_H

extern void run_error_edge_tests(void);
extern void run_advanced_ops_tests(void);
extern void run_cmp_search_tests(void);
extern void run_strlen_validation_tests(void);
extern void run_lcharclass_tests(void);
extern void run_ucharclass_tests(void);
extern void run_reserved_matrix_tests(void);
extern void run_search_family_tests(void);
extern void run_span_count_tests(void);
extern void run_core_family_tests(void);
extern void run_type_matrix_tests(void);
extern void run_utilities_tests(void);
extern void run_fuzz_edge_tests(void);
extern void run_skip_tests(void);

#endif