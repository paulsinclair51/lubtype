/**
 * @file lubtype_tests.c
 * @brief Aggregates and runs all lubtype.h regression and unit tests.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

/* Enable POSIX.1-2008 (sigaction, sigsetjmp, siglongjmp, sigjmp_buf). */
#define _POSIX_C_SOURCE 200809L

#include "../lubtype.h"
#include "lubtype_test_declarations.h"

#include <setjmp.h>
#include <signal.h>
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

/**
 * @brief Run fn with a signal trap; catch any crash/abort as exception.
 *
 * Installs handlers for SIGSEGV, SIGABRT, and SIGBUS that longjmp back
 * to the guard point, setting exception=1 in the returned result.
 * Handlers are restored to their previous disposition after each call.
 */
static sigjmp_buf  guard_env;
static volatile sig_atomic_t guard_active = 0;

static void guard_signal_handler(int sig) {
	(void)sig;
	if (guard_active) {
		guard_active = 0;
		siglongjmp(guard_env, 1);
	}
}

static lub_test_result_t run_guarded(lub_test_result_t (*fn)(void)) {
	struct sigaction sa, old_segv, old_abrt, old_bus;
	sa.sa_handler = guard_signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGSEGV, &sa, &old_segv);
	sigaction(SIGABRT, &sa, &old_abrt);
	sigaction(SIGBUS,  &sa, &old_bus);

	guard_active = 1;
	lub_test_result_t result = {0, 0, 0};

	if (sigsetjmp(guard_env, 1) == 0) {
		result = fn();
		guard_active = 0;
	} else {
		result = (lub_test_result_t){0, 0, 1};
	}

	sigaction(SIGSEGV, &old_segv, NULL);
	sigaction(SIGABRT, &old_abrt, NULL);
	sigaction(SIGBUS,  &old_bus,  NULL);

	return result;
}

/** @brief Merge two result structs by summing each field. */
static lub_test_result_t merge_results(lub_test_result_t a, lub_test_result_t b) {
	return (lub_test_result_t){
		a.pass + b.pass,
		a.fail + b.fail,
		a.exception + b.exception
	};
}

static void write_test_category(FILE *report, size_t index, const char *label,
                                lub_test_result_t result) {
	fprintf(report,
	        " %2zu. %-35s  pass: %5zu  fail: %5zu  exception: %zu\n",
	        index, label,
	        result.pass, result.fail, result.exception);
}

/**
 * @brief Main entry point for all lubtype.h tests.
 *
 * Runs all test modules in sequence and prints a summary message.
 */
int main(int argc, char **argv) {
	char report_path[1024];
	lub_test_result_t totals = {0, 0, 0};
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

	/* Write header with date/time */
	time_t now = time(NULL);
	char timebuf[64];
	strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", localtime(&now));

	fprintf(report, "LUBTYPE TEST SUITE REPORT\n");
	fprintf(report, "Generated: %s\n", timebuf);
	fprintf(report, "----------------------------------------\n");
	fprintf(report, "Test categories executed (in order):\n");

#define RUN_AND_REPORT(idx, label, result_expr) \
	do { \
		lub_test_result_t _cat = (result_expr); \
		write_test_category(report, (idx), (label), _cat); \
		totals = merge_results(totals, _cat); \
	} while (0)

	RUN_AND_REPORT(1,  "Error/edge cases",
	               run_guarded(run_error_edge_tests));
	RUN_AND_REPORT(2,  "Advanced operations (l and u)",
	               merge_results(run_guarded(run_advanced_ops_tests_l),
	                             run_guarded(run_advanced_ops_tests_u)));
	RUN_AND_REPORT(3,  "Compare/search (l and u)",
	               merge_results(run_guarded(run_cmp_search_tests_l),
	                             run_guarded(run_cmp_search_tests_u)));
	RUN_AND_REPORT(4,  "String length/validation",
	               run_guarded(run_strlen_validation_tests));
	RUN_AND_REPORT(5,  "Charclass (l and u)",
	               merge_results(run_guarded(run_charclass_tests_l),
	                             run_guarded(run_charclass_tests_u)));
	RUN_AND_REPORT(6,  "Reserved/matrix",
	               run_guarded(run_reserved_matrix_tests));
	RUN_AND_REPORT(7,  "Search families",
	               run_guarded(run_search_family_tests));
	RUN_AND_REPORT(8,  "Count",
	               run_guarded(run_count_tests));
	RUN_AND_REPORT(9,  "Core families",
	               run_guarded(run_core_family_tests));
	RUN_AND_REPORT(10, "Type matrix",
	               run_guarded(run_type_matrix_tests));
	RUN_AND_REPORT(11, "Utilities",
	               run_guarded(run_utilities_tests));
	RUN_AND_REPORT(12, "Fuzz/edge cases",
	               run_guarded(run_fuzz_edge_tests));
	RUN_AND_REPORT(13, "Skip functions",
	               run_guarded(run_skip_tests));

#undef RUN_AND_REPORT

	fprintf(report, "----------------------------------------\n");
	fprintf(report, "Totals:  pass: %zu  fail: %zu  exception: %zu\n",
	        totals.pass, totals.fail, totals.exception);
	fprintf(report, "----------------------------------------\n");

	if (totals.fail == 0 && totals.exception == 0) {
		fprintf(report, "\nAll tests passed.\n");
	} else {
		fprintf(report, "\nTest run completed with failures or exceptions.\n");
	}
	fclose(report);

	printf("Tests completed. Report written to %s\n", resolved_report_path);
	return (totals.fail > 0 || totals.exception > 0) ? 1 : 0;
}
