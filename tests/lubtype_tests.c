/**
 * @file lubtype_tests.c
 * @brief Main aggregator and runner for lubtype.h regression and unit tests.
 *
 * This module serves as the test suite orchestrator for testing lubtype.h,
 * coordinating execution of test categories defined across separate
 * test modules. Each category tests a distinct feature of lubtype.h
 * (e.g., Character classification, Compare/search).
 * 
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 * 
 * @section usage Usage
 * Use the Makefile or build_lubtype_tests.ps1 to build an executable:
 * 
 * - Linux/macOS: make
 *   (defaults to use Makefile in the current directory)
 * 
 * - Windows PowerShell: .\build_lubtype_tests.ps1
 * 
 *  Run the test suite executable to generate the test report
 *  lubtype_tests_report.txt::
 * 
 * - Linux/macOS: ./lubtype_tests [report_path]
 * 
 * - Windows: .\lubtype_tests.exe [report_path]
 * 
 *  The optional report_path argument allows specifying a
 *  path for the test report file lubtype_tests_report.txt. For example:
 * 
 * - Linux/macOS: ./lubtype_tests /path/to/report.txt
 * 
 * - Windows: .\lubtype_tests.exe C:\path\to\report.txt
 * 
 *  If no report_path is provided, the test report is written to
 *  "lubtype_tests_report.txt" in the same directory as the test executable.
 * 
 * @section report Test Report
 *
 * The test report includes:
 * 
 * - Timestamp of test execution and version of the tested lubtype.h file.
 * 
 * - Per-category pass/fail/exception counts.
 * 
 * - Totals across all categories.
 * 
 * - Overall pass/fail/exception summary.
 * 
 * @note A category (e.g., Character classification) is marked with -x if
 *       the test .c file includes polymorphic (x) macros such as xxsnncmp
 *       and uxsnncmp.
 * 
 *       The tests in the category are run sequentially with
 *       the test .c file compiled with LUB_X_IS_L (x maps to l), then with
 *       LUB_X_IS_U (x maps to u). Their results are aggregated for the report.
 * 
 *       See README.md or lubtype.h documentation for details on polymorphic
 *       (x) macros and how they are used.
 * 
 * @section exception_guarding Exception Guarding
 *
 * The test framework uses a two-level exception guarding approach using
 * sigaction and sigsetjmp/siglongjmp to catch unexpected terminations
 * (e.g., segmentation faults, aborts) during test execution:
 * 
 * - Assert-Level Guarding (in test modules)
 * 
 *     The LUB_ASSERT macro wraps signal handlers around individual
 *     assertions, enabling fine-grained detection exceptions at the
 *     assertion level rather than crashing the entire test suite.
 *     This allows counting pass (assertion true), fail (assertion false),
 *     and exception (assertion did not complete due to signal/fault).
 *
 * - Category-Level Guarding (in this file)
 * 
 *     The run_guarded() wrapper catches unhandled process terminations
 *     (SIGSEGV, SIGABRT, SIGBUS) that occur during entire test module
 *     execution. These are counted as category-level exceptions.
 * 
 * Note: Assert-level guarding is expected to handle most exceptions,
 *       so category-level exceptions are rare but detection is provided
 *       for defensive programming (for instance, if a test module
 *       causes an exception outside of an assertion).
 */

/* Enable POSIX.1-2008 (sigaction, sigsetjmp, siglongjmp, sigjmp_buf). */
#define _POSIX_C_SOURCE 200809L

#include <limits.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../lubtype.h"
#include "lubtype_test_declarations.h"

/**
 * @name report_path_from_argv0
 * @brief Derive the output report path from the executable's argv[0].
 *
 * Extracts the directory component from argv[0] (if present) and appends
 * "lubtype_tests_report.txt" to place the report alongside the test binary.
 * If argv[0] contains no directory separator or the derived path would exceed
 * buffer capacity, returns the default filename "lubtype_tests_report.txt"
 * (written to the current working directory).
 * 
 * @todo Consider raising error if no directory separator found or buffer_size
 *       is too small to hold the derived path?
 *
 * @param argv0       The program name/path (typically argv[0] from main).
 * @param buffer      Destination buffer for the result (must not be NULL).
 * @param buffer_size Size of buffer in bytes (must be large enough for
 *                    directory + filename).
 *
 * @return Pointer to the report path (either buffer contents or static
 *         default filename).
 */
static const char *report_path_from_argv0(
	const char *argv0, char *buffer, size_t buffer_size)
{
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
 * Defines handlers for SIGSEGV, SIGABRT, and SIGBUS that longjmp back
 * to the guard point, setting exception=1 in the returned result.
 * Handlers are restored to their previous disposition after each call.
 *
 * @section context Category-Level Guard
 *
 * This is the category-level guard.
 *
 * @param fn Function pointer to test function returning lub_test_result_t
 *
 * @return Result from fn (if no signal), or {0, 0, SIZE_MAX} if signal caught
 *         (SIZE_MAX marks category-level exception).
 */
static sigjmp_buf  guard_env;
static volatile sig_atomic_t guard_active = 0;

/**
 * @name guard_signal_handler
 * @brief Category-level signal handler.
 *
 * Catches unhandled signals (SIGSEGV, SIGABRT, SIGBUS) during test
 * module execution and siglongjmps back to the guard point
 * (run_guarded).
 *
 * @param sig The caught signal number (SIGSEGV, SIGABRT, or SIGBUS)
 *            (signal value is ignored; not used in return value logic)
 */
static void guard_signal_handler(int sig) {
	(void)sig;
	if (guard_active) {
		guard_active = 0;
		siglongjmp(guard_env, 1);
	}
}

/**
 * @name run_guarded
 * @brief Category-level test guard.
 *
 * Runs the test module and siglongjmps back to the guard point (run_guarded).
 * If a signal occurs, returns {0, 0, SIZE_MAX} to mark a category-level exception..
 *
 * @param fn Function pointer to test function.
 */
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
		/* Use SIZE_MAX to mark category-level exception. */
		result = (lub_test_result_t){0, 0, SIZE_MAX};
	}

	sigaction(SIGSEGV, &old_segv, NULL);
	sigaction(SIGABRT, &old_abrt, NULL);
	sigaction(SIGBUS,  &old_bus,  NULL);

	return result;
}

/**
 * @name merge_x_results
 * @brief Merge two result structs by adding the two fields.
 *
 * Used to combine results from -x test function runs into a single category result.
 * For example, Advanced Operations tests both Latin (run_advanced_ops_tests_l)
 * and Unicode (run_advanced_ops_tests_u) variants; their results are merged.
 *
 * If either result has a category-level exception (exception == SIZE_MAX),
 * the merged result is marked as a category-level exception (SIZE_MAX).
 * Otherwise, exception counts from the two runs are summed normally.
 *
 * @param a First result struct.
 * @param b Second result struct.
 *
 * @return New result with pass/fail/exception merged for the two runs.
 * 
 * @note If either exception count is SIZE_MAX, the merged count for the
 *       two runs is SIZE_MAX; otherwise, the merged exception count 
 *       is the sum of the exceptions counts for the two runs (which may
 *       be 0 if both are 0).
 */ 
static lub_test_result_t merge_x_results(lub_test_result_t a, lub_test_result_t b) {
	return (lub_test_result_t){
		       a.pass + b.pass,
		       a.fail + b.fail,
		       (a.exception == SIZE_MAX || b.exception == SIZE_MAX) ?
	                SIZE_MAX : a.exception + b.exception
	       };
}

/**
 * @name write_test_category
 * @brief Write a single test category result line to the report.
 *
 * Formats and writes one line of the test report showing category index,
 * category label, and pass/fail/exception counts. An exception count
 * equal to SIZE_MAX displays as * to indicate a category-level exception.
 *
 * @param report The open FILE* for the report.
 * @param index  Category index (1-14).
 * @param label  Display category label (e.g., "Error/edge cases").
 * @param count Pass/fail/exception counts.
 */
static void write_test_category(FILE *report, size_t index, const char *label,
								lub_test_result_t count) {
	if (!count.fail && !count.exception) {
	  fprintf(report, " %2zu. %-40s  %4zu\n",
		              index, label, count.pass);
	} else if (!count.fail) {
		if (count.exception == SIZE_MAX)
			fprintf(report, " %2zu. %-40s  %4zu           *\n",
			                index, label, count.pass);
		else
			fprintf(report, " %2zu. %-40s  %4zu        %4zu\n",
			                index, label, count.pass, count.exception);
	} else if (!count.exception) {
		fprintf(report, " %2zu. %-40s  %4zu  %4zu\n",
		                index, label, count.pass, count.fail);
	} else {
		if (count.exception == SIZE_MAX)
			fprintf(report, " %2zu. %-40s  %4zu  %4zu     *\n",
			            index, label, count.pass, count.fail);
		else
			fprintf(report, " %2zu. %-40s  %4zu  %4zu  %4zu\n",
			                index, label, count.pass, count.fail, count.exception);
	}
}

/**
 * @def RUN_AND_REPORT
 * @brief Macro to execute a test category and record its result.
 *
 * Executes the test expression (result_expr), writes a single report line
 * via write_test_category(), and accumulates the result into total.
 *
 * @param idx         Category index (1-14, written to report)
 * @param label       Display name of category (written to report)
 * @param result_expr Expression evaluating to lub_test_result_t
 *                    (typically a run_guarded() or merge_results() call)
 */
#define RUN_AND_REPORT(idx, label, result_expr) \
	do { \
		lub_test_result_t _cat = (result_expr); \
		write_test_category(report, (idx), (label), _cat); \
        total = (lub_test_result_t){ \
		            total.pass + _cat.pass, \
		            total.fail + _cat.fail, \
		            total.exception + \
					    (_cat.exception == SIZE_MAX ? 1 : _cat.exception) \
		   	    }; \
		if (_cat.exception == SIZE_MAX) ++cat_exceptions; \
	} while (0)

/**
 * @name main
 * @brief Main entry point for lubtype.h tests.
 *
 * Initializes the test report file, executes test categories in sequence,
 * accumulates results, and writes a summary report. Each category result is
 * passed through run_guarded() to catch unhandled signals at category level).
 * 
 * @param argc Argument count.
 * @param argv Argument vector (argv[0] used for report path derivation).
 * @return 0 if all tests passed (fail=0, exception=0); 1 otherwise.
 * 
 * @note Unbuffered I/O writes to the report file are used to ensure
 *       output is written immediately, in case a following test
 *       assertion triggers an exception.
 */
int main(int argc, char **argv) {
	char report_path[1024];
	lub_test_result_t total = {0, 0, 0};
	int cat_exceptions = 0;

	const char *resolved_report_path = (argc > 0 && argv && argv[0])
		? report_path_from_argv0(argv[0], report_path, sizeof(report_path))
		: "lubtype_tests_report.txt";

	FILE *report = fopen(resolved_report_path, "w");
	if (!report) {
		fprintf(stderr, "[ERROR] Could not open report file for writing.\n");
		return 1;
	}

	/* Use unbuffered I/O to keep report output on disk even if a
	   subsequent test assertion causes an exception.
	*/
	(void)setvbuf(report, NULL, _IONBF, 0);

	/* Write header with version and date/time. */
	time_t now = time(NULL);
	char timebuf[64];
	strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", localtime(&now));

	fprintf(report, "Test Suite Report for lubtype.h Version %s, %s\n\n",
		            LUB_VERSION, timebuf);
	fprintf(report, "     Test Categories                           Pass  Fail  Exception\n");
	fprintf(report, "--------------------------------------------------------------------\n");

	RUN_AND_REPORT(1, "Utilities -x",
	               merge_x_results(run_guarded(run_utilities_tests_l),
	                               run_guarded(run_utilities_tests_u)));
	RUN_AND_REPORT(2, "Type matrix",
	               run_guarded(run_type_matrix_tests));
	RUN_AND_REPORT(3, "Character classification -x",
	               merge_x_results(run_guarded(run_charclass_tests_l),
	                               run_guarded(run_charclass_tests_u)));
	RUN_AND_REPORT(4, "String length and classification -x",
	               merge_x_results(run_guarded(run_strlen_strclass_tests_l),
	                             run_guarded(run_strlen_strclass_tests_u)));
	RUN_AND_REPORT(5, "Reserved/matrix",
	               run_guarded(run_reserved_matrix_tests));
	RUN_AND_REPORT(6, "Compare/search -x",
	               merge_x_results(run_guarded(run_cmp_search_tests_l),
	                               run_guarded(run_cmp_search_tests_u)));
	RUN_AND_REPORT(7, "Search -x",
	               merge_x_results(run_guarded(run_search_family_tests_l),
	                               run_guarded(run_search_family_tests_u)));
	RUN_AND_REPORT(8, "Count -x",
	               merge_x_results(run_guarded(run_count_tests_l),
	                               run_guarded(run_count_tests_u)));
	RUN_AND_REPORT(9, "Skip -x",
	               merge_x_results(run_guarded(run_skip_tests_l),
	                               run_guarded(run_skip_tests_u)));
	RUN_AND_REPORT(10, "Core -x",
	               merge_x_results(run_guarded(run_core_family_tests_l),
	                               run_guarded(run_core_family_tests_u)));
	RUN_AND_REPORT(11, "Advanced -x",
	               merge_x_results(run_guarded(run_advanced_ops_tests_l),
	                               run_guarded(run_advanced_ops_tests_u)));
	RUN_AND_REPORT(12, "Miscellaneous x-macros -x",
	               merge_x_results(run_guarded(run_xmacros_tests_l),
	                               run_guarded(run_xmacros_tests_u)));
	RUN_AND_REPORT(13, "Error/edge cases -x",
	               merge_x_results(run_guarded(run_error_edge_tests_l),
	                               run_guarded(run_error_edge_tests_u)));
	RUN_AND_REPORT(14, "Fuzz/edge cases",
	               run_guarded(run_fuzz_edge_tests));

	fprintf(report, "------------------------------------------------------------------\n");
	if (!total.fail && !total.exception) {
        fprintf(report, "                                        Total  %4zu",
			            total.pass);
	} else if (!total.fail) {
		fprintf(report, "                                        Total  %4zu        %4zu",
			            total.pass, total.exception);
	} else if (!total.exception) {
		fprintf(report, "                                        Total  %4zu  %4zu",
			            total.pass, total.fail);
	} else {
		fprintf(report, "                                        Total  %4zu  %4zu  %4zu",
			            total.pass, total.fail, total.exception);
	}								
	fprintf(report, "\n\n-x: run tests for category with l (Latin) and then u (Unicode)\n");
	if (cat_exceptions) {
	    fprintf(report, " *: category-level exception (counts as 1 in exception total).\n");
	}
	if (!total.fail && !total.exception) {
		fprintf(report, "\nAll tests passed.\n");
	} else if (!total.fail) {
		fprintf(report, "\nTest run completed with exceptions.\n");
	} else if (!total.exception) {
		fprintf(report, "\nTest run completed with failures.\n");
	} else {
		fprintf(report, "\nTest run completed with failures and exceptions.\n");
	}
	fclose(report);

	return (total.fail > 0 || total.exception > 0) ? 1 : 0;
}

#undef RUN_AND_REPORT

/* End of lubtype_tests.c */