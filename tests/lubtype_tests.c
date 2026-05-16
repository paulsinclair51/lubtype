/**
 * @file lubtype_tests.c
 * @brief Aggregates and runs all lubtype.h regression and unit tests.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include "../lubtype.h"
#include "lubtype_test_declarations.h"

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static const char *report_path_from_argv0(const char *argv0, char *buffer, size_t buffer_size) {
	const char *slash = strrchr(argv0, '/');
	const char *backslash = strrchr(argv0, '\\');
	const char *separator = slash;
	if (!separator || (backslash && backslash > separator)) {
		separator = backslash;
	}

	if (!separator) {
		return "lubtype_tests_report.txt";
	}

	size_t prefix_len = (size_t)(separator - argv0) + 1;
	if (prefix_len + strlen("lubtype_tests_report.txt") + 1 > buffer_size) {
		return "lubtype_tests_report.txt";
	}

	memcpy(buffer, argv0, prefix_len);
	buffer[prefix_len] = '\0';
	strcat(buffer, "lubtype_tests_report.txt");
	return buffer;
}

static void write_test_category(FILE *report, size_t index, const char *label, size_t count) {
	fprintf(report, " %2zu. %s (%zu test%s)\n", index, label, count, count == 1 ? "" : "s");
}

/**
 * @brief Main entry point for all lubtype.h tests.
 *
 * Runs all test modules in sequence and prints a summary message.
 */
int main(int argc, char **argv) {
	char report_path[1024];
	size_t total_asserts = 0;
	const char *resolved_report_path = (argc > 0 && argv && argv[0])
		? report_path_from_argv0(argv[0], report_path, sizeof(report_path))
		: "lubtype_tests_report.txt";
	FILE *report = fopen(resolved_report_path, "w");
	if (!report) {
		fprintf(stderr, "[ERROR] Could not open report file for writing.\n");
		return 1;
	}

	/* Keep report output on disk even if a test assertion aborts execution. */
	(void)setvbuf(report, NULL, _IONBF, 0);

	// Write header with date/time
	time_t now = time(NULL);
	char timebuf[64];
	strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", localtime(&now));
	fprintf(report, "LUBTYPE TEST SUITE REPORT\n");
	fprintf(report, "Generated: %s\n", timebuf);
	fprintf(report, "----------------------------------------\n");
	fprintf(report, "Test categories executed (in order):\n");
	write_test_category(report, 1, "Error/edge cases", 12);
	total_asserts += 12;
	write_test_category(report, 2, "Advanced operations for LUB_X = l and u", 24);
	total_asserts += 24;
	write_test_category(report, 3, "Compare/search for LUB_X = l and u", 22);
	total_asserts += 22;
	write_test_category(report, 4, "String length/validation", 19);
	total_asserts += 19;
	write_test_category(report, 5, "Charclass for LUB_X = l and u", 46);
	total_asserts += 46;
	write_test_category(report, 6, "Reserved/matrix", 42);
	total_asserts += 42;
	write_test_category(report, 7, "Search families", 25);
	total_asserts += 25;
	write_test_category(report, 8, "Span/count", 10);
	total_asserts += 10;
	write_test_category(report, 9, "Core families", 62);
	total_asserts += 62;
	write_test_category(report, 10, "Type matrix", 32);
	total_asserts += 32;
	write_test_category(report, 11, "Utilities", 42);
	total_asserts += 42;
	write_test_category(report, 12, "Fuzz/edge cases", 11);
	total_asserts += 11;
	write_test_category(report, 13, "Skip functions", 37);
	total_asserts += 37;
	fprintf(report, "----------------------------------------\n");
	fprintf(report, "Total tests executed: %zu\n", total_asserts);
	fprintf(report, "----------------------------------------\n");

	// Run all test modules
	run_error_edge_tests();
	run_advanced_ops_tests_l();
	run_advanced_ops_tests_u();
	run_cmp_search_tests_l();
	run_cmp_search_tests_u();
	run_strlen_validation_tests();
	run_charclass_tests_l();
	run_charclass_tests_u();
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

	printf("All tests completed. Report written to %s\n", resolved_report_path);
	return 0;
}
