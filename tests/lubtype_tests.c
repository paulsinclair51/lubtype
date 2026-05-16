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

#define RUN_AND_REPORT(idx, label, expr) \
	do { \
		size_t category_count = (size_t)(expr); \
		write_test_category(report, (idx), (label), category_count); \
		total_asserts += category_count; \
	} while (0)

	RUN_AND_REPORT(1, "Error/edge cases", run_error_edge_tests());
	RUN_AND_REPORT(2, "Advanced operations for LUB_X = l and u", run_advanced_ops_tests_l() + run_advanced_ops_tests_u());
	RUN_AND_REPORT(3, "Compare/search for LUB_X = l and u", run_cmp_search_tests_l() + run_cmp_search_tests_u());
	RUN_AND_REPORT(4, "String length/validation", run_strlen_validation_tests());
	RUN_AND_REPORT(5, "Charclass for LUB_X = l and u", run_charclass_tests_l() + run_charclass_tests_u());
	RUN_AND_REPORT(6, "Reserved/matrix", run_reserved_matrix_tests());
	RUN_AND_REPORT(7, "Search families", run_search_family_tests());
	RUN_AND_REPORT(8, "Span/count", run_span_count_tests());
	RUN_AND_REPORT(9, "Core families", run_core_family_tests());
	RUN_AND_REPORT(10, "Type matrix", run_type_matrix_tests());
	RUN_AND_REPORT(11, "Utilities", run_utilities_tests());
	RUN_AND_REPORT(12, "Fuzz/edge cases", run_fuzz_edge_tests());
	RUN_AND_REPORT(13, "Skip functions", run_skip_tests());

#undef RUN_AND_REPORT

	fprintf(report, "----------------------------------------\n");
	fprintf(report, "Total tests executed: %zu\n", total_asserts);
	fprintf(report, "----------------------------------------\n");

	// Write completion message
	fprintf(report, "\nAll tests completed successfully.\n");
	fprintf(report, "No failures detected (assertions would abort execution).\n");
	fclose(report);

	printf("All tests completed. Report written to %s\n", resolved_report_path);
	return 0;
}
