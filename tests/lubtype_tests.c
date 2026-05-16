/**
 * @file lubtype_tests.c
 * @brief Main aggregator and runner for lubtype.h regression and unit tests.
 *
 * This module serves as the test suite orchestrator for lubtype.h, coordinating
 * execution of 13 test categories defined across separate test modules. Each
 * category tests a distinct feature family of lubtype.h (e.g., advanced
 * operations, character classification, substring searching).
 *
 * @section test_framework Test Framework Architecture
 *
 * The test framework uses a two-level signal guarding approach:
 *
 * **Category-Level Guarding (this file):** The run_guarded() wrapper catches
 * unhandled process terminations (SIGSEGV, SIGABRT, SIGBUS) that occur during
 * entire test module execution. These are recorded as category-level exceptions.
 * Note: Modern assert-level guarding is handled within LUB_ASSERT macro in the
 * individual test files (see lubtype_test_declarations.h), so category-level
 * exceptions are now rare but retained for defensive programming.
 *
 * **Assert-Level Guarding (test modules):** Each LUB_ASSERT macro wraps signal
 * handlers around individual assertions, enabling fine-grained detection of
 * faults (segfaults, aborts) at the assertion level rather than crashing the
 * entire test suite. This allows counting: pass (assertion true), fail
 * (assertion false), and exception (assertion did not complete due to
 * signal/fault).
 *
 * @section reporting Test Report Output
 *
 * After all categories complete, a summary report is written to
 * "lubtype_tests_report.txt" (or a path derived from argv[0]). The report includes:
 * - Timestamp of test execution
 * - Per-category pass/fail/exception counts
 * - Grand totals across all categories
 * - Overall pass/fail summary
 *
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

/**
 * @brief Derive the output report path from the executable's argv[0].
 *
 * Extracts the directory component from argv[0] (if present) and appends
 * "lubtype_tests_report.txt" to place the report alongside the test binary.
 * If argv[0] contains no directory separator or the derived path would exceed
 * buffer capacity, returns the default filename "lubtype_tests_report.txt"
 * (written to the current working directory).
 *
 * @param argv0       The program name/path (typically argv[0] from main)
 * @param buffer      Destination buffer for the result (must not be NULL)
 * @param buffer_size Size of buffer in bytes (must be large enough for
 *                    directory + filename)
 *
 * @return Pointer to the report path (either buffer contents or static
 *         default filename)
 */
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
 *
 * @section context Category-Level Guard
 *
 * This is the category-level guard. Individual assertions within
 * test functions now have their own signal guards (LUB_ASSERT macro in
 * lubtype_test_declarations.h), so category-level exceptions are rare
 * but possible for unguarded faults.
 *
 * @param fn Function pointer to test function returning lub_test_result_t
 *
 * @return Result from fn (if no signal), or {0, 0, 1} if signal caught
 */
static sigjmp_buf  guard_env;
static volatile sig_atomic_t guard_active = 0;

/**
 * @brief Category-level signal handler for test guard.
 *
 * Catches unhandled signals (SIGSEGV, SIGABRT, SIGBUS) during test
 * module execution and siglongjmps back to the guard point
 * (run_guarded). This results in the entire category being marked
 * as exception:1.
 *
 * @note Modern per-assert signal trapping is now done within
 *       LUB_ASSERT macro (in lubtype_test_declarations.h), so
 *       this category-level handler acts as a backstop for
 *       unguarded crashes.
 *
 * @param sig The caught signal number (SIGSEGV, SIGABRT, or SIGBUS)
 */
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

/**
 * @brief Merge two result structs by summing each field.
 *
 * Used to combine results from paired test functions (e.g., uppercase and lowercase
 * variants) into a single category result. For example, Advanced Operations tests
 * both Latin (run_advanced_ops_tests_l) and Unicode (run_advanced_ops_tests_u)
 * variants; their results are merged into one.
 *
 * @param a First result struct
 * @param b Second result struct
 *
 * @return New result with pass/fail/exception sums
 */
static lub_test_result_t merge_results(lub_test_result_t a, lub_test_result_t b) {
	return (lub_test_result_t){
		a.pass + b.pass,
		a.fail + b.fail,
		a.exception + b.exception
	};
}

/**
 * @brief Write a single test category result line to the report.
 *
 * Formats and writes one line of the test report showing category index,
 * label, and pass/fail/exception counts.
 *
 * Format: " %2zu. %-35s  pass: %5zu  fail: %5zu  exception: %zu\n"
 *
 * @param report The open FILE* for the report
 * @param index  Category index (1-13)
 * @param label  Display name of the category (e.g., "Error/edge cases")
 * @param result The lub_test_result_t struct with pass/fail/exception counts
 */
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
 * Initializes the test report file, executes 13 test categories in sequence,
 * accumulates results, and writes a summary report. Each category result is
 * passed through run_guarded() to catch unhandled signals at category level
 * (though most per-assert signal handling is done within test functions).
 *
 * @section report_location Report File Location
 * The report is written to:
 * 1. Directory alongside the executable (if argv[0] contains path) + "lubtype_tests_report.txt", or
 * 2. Current working directory + "lubtype_tests_report.txt" (fallback)
 *
 * @section buffering Unbuffered I/O
 * Report file uses unbuffered I/O to ensure output is written immediately,
 * even if a test assertion triggers process abort/termination.
 *
 * @return 0 if all tests passed (fail=0, exception=0); 1 otherwise
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

/**
 * @def RUN_AND_REPORT(idx, label, result_expr)
 * @brief Macro to execute a test category and record its result.
 *
 * Executes the test expression (result_expr), writes a single report line
 * via write_test_category(), and accumulates the result into totals.
 *
 * @param idx         Category index (1-13, written to report)
 * @param label       Display name of category (written to report)
 * @param result_expr Expression evaluating to lub_test_result_t
 *                    (typically a run_guarded() or merge_results() call)
 */
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
