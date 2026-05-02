/**
 * @file lubtype_tests.c
 * @brief Aggregates and runs all lubtype.h regression and unit tests.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#define __LUB_DEFINITIONS__
#include "../lubtype.h"

#include "test_error_edge.c"
void run_error_edge_tests(void);
#include "test_advanced_ops.c"
void run_advanced_ops_tests(void);
#include "test_cmp_search.c"
void run_cmp_search_tests(void);
#include "test_strlen_validation.c"
void run_strlen_validation_tests(void);
#include "test_charclass.c"
void run_charclass_tests(void);
#include "test_fuzz_edge.c"
void run_fuzz_edge_tests(void);
#include "test_skip.c"
void run_skip_tests(void);
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void run_reserved_matrix_tests(void);
void run_search_family_tests(void);
void run_span_count_tests(void);
void run_core_family_tests(void);
void run_type_matrix_tests(void);
void run_utilities_tests(void);

/**
 * @brief Main entry point for all lubtype.h tests.
 *
 * Runs all test modules in sequence and prints a summary message.
 */
int main(void) {
	FILE *report = fopen("lubtype_tests_report.txt", "w");
	if (!report) {
		fprintf(stderr, "[ERROR] Could not open report file for writing.\n");
		return 1;
	}

	// Write header with date/time
	time_t now = time(NULL);
	char timebuf[64];
	strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", localtime(&now));
	fprintf(report, "LUBTYPE TEST SUITE REPORT\n");
	fprintf(report, "Generated: %s\n", timebuf);
	fprintf(report, "----------------------------------------\n");
	fprintf(report, "Test modules executed (in order):\n");
	fprintf(report, "  1. Error/edge cases\n");
	fprintf(report, "  2. Advanced operations\n");
	fprintf(report, "  3. Compare/search\n");
	fprintf(report, "  4. String length/validation\n");
	fprintf(report, "  5. Character classification\n");
	fprintf(report, "  6. Reserved/matrix\n");
	fprintf(report, "  7. Search families\n");
	fprintf(report, "  8. Span/count\n");
	fprintf(report, "  9. Core families\n");
	fprintf(report, " 10. Type matrix\n");
	fprintf(report, " 11. Utilities\n");
	fprintf(report, " 12. Fuzz/edge cases\n");
	fprintf(report, " 13. Skip functions\n");
	fprintf(report, "----------------------------------------\n");

	// Run all test modules
	run_error_edge_tests();
	run_advanced_ops_tests();
	run_cmp_search_tests();
	run_strlen_validation_tests();
	run_charclass_tests();
	run_reserved_matrix_tests();
	run_search_family_tests();
	run_span_count_tests();
	run_core_family_tests();
	run_type_matrix_tests();
	run_utilities_tests();
	run_fuzz_edge_tests();
	run_skip_tests();

	// Write completion message
	fprintf(report, "\nAll tests completed successfully.\n");
	fprintf(report, "No failures detected (assertions would abort execution).\n");
	fclose(report);

	printf("All tests completed. Report written to lubtype_tests_report.txt\n");
	return 0;
}
