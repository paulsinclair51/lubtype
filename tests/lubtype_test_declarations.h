/**
 * @file lubtype_test_declarations.h
 * @brief Shared test infrastructure: result types, assert macros,
 *        and function declarations.
 *
 * This header provides the test framework foundation used by all
 * lubtype.h test modules:
 *
 * @section result_struct Test Result Structure
 * Each test module returns a lub_test_result_t struct counting:
 * - **pass**: Assertions that evaluated true
 * - **fail**: Assertions that evaluated false
 * - **exception**: Assertions that did not complete (e.g., due to
 *   segfault, abort, SIGBUS)
 *
 * @section assert_level_guarding Assert-Level Signal Guarding
 * The LUB_ASSERT macro wraps each assertion with signal handlers
 * to detect faults at assertion granularity. When an assertion is reached:
 *
 * 1. Install handlers for SIGSEGV (segfault), SIGABRT (abort),
 *    SIGBUS (bus error)
 * 2. Set up a setjmp() recovery point (lubtype_assert_env)
 * 3. Mark guard_active = 1 to indicate guard is armed
 * 4. Evaluate the assertion expression
 * 5. If expression is true, increment pass; if false, increment
 *    fail and print message
 * 6. If a signal is caught during steps 2-5, longjmp back to
 *    recovery point and increment exception
 * 7. Restore original signal handlers
 *
 * This per-assert guarding enables finer-grained exception detection
 * than category-level guarding (which exists in lubtype_tests.c as
 * a defensive backstop).
 *
 * @section why_signal_handling Why Signal Handling?
 * Without per-assert signal guarding, a segfault or abort during
 * any assertion would:
 * - Crash the entire test process
 * - Prevent completion of remaining assertions
 * - Lose count of how many tests passed before the crash
 *
 * With per-assert guarding, faults are caught and counted as
 * exceptions, allowing:
 * - Continued execution of remaining assertions
 * - Accurate reporting of pass/fail/exception per category
 * - Test suite completion even if some assertions encounter faults
 *
 * @section signals_caught Signals Caught
 * - **SIGSEGV**: Segmentation fault (most common; accessing invalid memory)
 * - **SIGABRT**: Abort signal (explicit abort() or assertion failures in libc)
 * - **SIGBUS**: Bus error (unaligned memory access on some architectures)
 *
 * @section per_file_result Per-File Result Tracking
 * Each test module declares a file-scoped static lub_test_result_t:
 *   static lub_test_result_t test_result;
 *
 * The LUB_ASSERT macro modifies this struct. The run_*() function
 * resets it to zero, runs all assertions, and returns the final counts.
 *
 * @section posix_requirement POSIX Requirement
 * This header requires POSIX.1-2008 features (sigaction, setjmp, signal).
 * The including file must define _POSIX_C_SOURCE 200809L
 * before including system headers.
 *
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#ifndef LUBTYPE_TEST_DECLARATIONS_H
#define LUBTYPE_TEST_DECLARATIONS_H

#include <setjmp.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @typedef lub_test_result_t
 * @brief Result counters returned by every run_*() test function.
 *
 * Tracks the outcomes of all assertions within a test category:
 * - **pass**: Number of assertions that evaluated to true
 * - **fail**: Number of assertions that evaluated to false
 *   (evaluated but condition failed)
 * - **exception**: Number of assertions that did not complete
 *   due to signal/fault
 *
 * @note The semantics of "exception" differ from traditional
 *       exception handling: It represents a detected fault
 *       (segfault, abort, bus error) during assertion evaluation,
 *       caught by signal handlers and longjmp recovery. It is NOT
 *       a C++ or runtime exception but a counted fault event.
 *
 * @note If an assertion encounters a signal, the signal's
 *       pass/fail/exception counts are not incremented again at
 *       the category level, as the assert-level guard already captured it.
 */
typedef struct {
	size_t pass;
	size_t fail;
	size_t exception;
} lub_test_result_t;

typedef void (*lubtype_sighandler_t)(int);

static jmp_buf lubtype_assert_env;
static volatile sig_atomic_t lubtype_assert_guard_active = 0;

/**
 * @brief Per-assert signal handler.
 *
 * Called when SIGSEGV, SIGABRT, or SIGBUS is caught during an assertion.
 * If the guard is armed (lubtype_assert_guard_active == 1), disarms it and
 * longjmps back to the setjmp recovery point in the LUB_ASSERT macro, causing
 * the exception counter to increment.
 *
 * @param sig The caught signal number (SIGSEGV, SIGABRT, or SIGBUS)
 */
static inline void lubtype_assert_signal_handler(int sig) {
	if (lubtype_assert_guard_active) {
		lubtype_assert_guard_active = 0;
		longjmp(lubtype_assert_env, sig ? sig : 1);
	}
}

/**
 * @brief Print assertion failure message to stderr.
 *
 * Called by LUB_ASSERT when an expression evaluates to false. Outputs the
 * expression string, file name, and line number for debugging.
 *
 * @param expr The assertion expression as a string (via #expr macro)
 * @param file The source file name (via __FILE__)
 * @param line The source line number (via __LINE__)
 */
static inline void lubtype_assert_fail(const char *expr, const char *file, int line) {
	fprintf(stderr, "Assertion failed: %s (%s:%d)\n", expr, file, line);
}

/**
 * @def LUB_ASSERT(expr)
 * @brief Assert macro with per-assert signal guarding and fault detection.
 *
 * @section usage Usage
 * Use LUB_ASSERT() like a standard assert, wrapping test expressions:
 *   LUB_ASSERT(function_result == expected_value);
 *   LUB_ASSERT(ptr != NULL);
 *   LUB_ASSERT(some_condition);
 *
 * Each assertion is guarded and one of three outcomes is recorded:
 * - **pass**: expr evaluates to true
 * - **fail**: expr evaluates to false
 * - **exception**: expr evaluation is interrupted by signal
 *   (segfault, abort, bus error)
 *
 * @section implementation Implementation Details
 * The macro performs the following steps:
 *
 * 1. **Install signal handlers**: Sets up handlers for SIGSEGV, SIGABRT, SIGBUS
 *    to catch faults during expression evaluation.
 *
 * 2. **Set recovery point**: Calls setjmp() to establish a point
 *    to return to if a signal is caught. Returns 0 on initial
 *    call, non-zero if longjmped from handler.
 *
 * 3. **Arm guard**: Sets lubtype_assert_guard_active = 1 to
 *    indicate handlers are active.
 *
 * 4. **Evaluate expression**: If setjmp returned 0 (normal path):
 *    - Evaluate expr
 *    - If true, increment test_result.pass
 *    - If false, increment test_result.fail and call lubtype_assert_fail()
 *    - Disarm guard (lubtype_assert_guard_active = 0)
 *
 * 5. **Exception path**: If setjmp returned non-zero (signal was caught):
 *    - Skip expression evaluation
 *    - Increment test_result.exception
 *    - Guard is already disarmed by signal handler
 *
 * 6. **Restore handlers**: Restore original signal handlers for
 *    SIGSEGV, SIGABRT, SIGBUS.
 *
 * @section signal_flow Signal Handling Flow
 * If a signal occurs during expression evaluation (step 4):
 * 1. Signal handler (lubtype_assert_signal_handler) is invoked
 * 2. Handler checks if guard is armed (lubtype_assert_guard_active == 1)
 * 3. If armed, disarms guard and calls longjmp(lubtype_assert_env, sig)
 * 4. longjmp returns to setjmp (step 2), which now returns sig (non-zero)
 * 5. Macro detects non-zero return and increments exception counter
 * 6. Original handlers are restored and control returns to caller
 *
 * @section per_file_requirement Per-File Requirement
 * Each test file using LUB_ASSERT must declare:
 *   static lub_test_result_t test_result;
 * at file scope. The macro assumes this variable exists and modifies it.
 *
 * @section example Example Test Function
 * @code
 * static lub_test_result_t test_result;
 *
 * static void test_my_function(void) {
 *     int result = my_function(42);
 *     LUB_ASSERT(result == expected_value);
 *     LUB_ASSERT(result > 0);
 *     // If my_function segfaults, it's caught:
 *     LUB_ASSERT(my_function_that_might_crash() == 0);
 * }
 *
 * lub_test_result_t run_my_tests(void) {
 *     test_result = (lub_test_result_t){0};
 *     test_my_function();
 *     return test_result;  // e.g., {pass: 2, fail: 0, exception: 1}
 * }
 * @endcode
 */
#define LUB_ASSERT(expr) \
	do { \
		/* Install signal handlers for SIGSEGV, SIGABRT, SIGBUS */ \
		lubtype_sighandler_t _old_segv = signal(SIGSEGV, lubtype_assert_signal_handler); \
		lubtype_sighandler_t _old_abrt = signal(SIGABRT, lubtype_assert_signal_handler); \
		lubtype_sighandler_t _old_bus = signal(SIGBUS, lubtype_assert_signal_handler); \
		/* setjmp: returns 0 on initial call, non-zero signal on longjmp */ \
		int _jump_code = setjmp(lubtype_assert_env); \
		/* Normal path (_jump_code == 0): evaluate expression */ \
		if (_jump_code == 0) { \
			lubtype_assert_guard_active = 1; \
			if (expr) { \
				++test_result.pass; \
			} else { \
				++test_result.fail; \
				lubtype_assert_fail(#expr, __FILE__, __LINE__); \
			} \
			lubtype_assert_guard_active = 0; \
		/* Signal path (_jump_code != 0): expression was interrupted */ \
		} else { \
			++test_result.exception; \
		} \
		/* Restore original signal handlers */ \
		signal(SIGSEGV, _old_segv); \
		signal(SIGABRT, _old_abrt); \
		signal(SIGBUS, _old_bus); \
	} while (0)

/**
 * @name Test Function Declarations
 * @brief Extern declarations for all test module runner functions.
 *
 * Each function has the signature:
 *   lub_test_result_t run_*_tests[_l|_u](void)
 *
 * Functions suffixed with _l test Latin (lchar_t) variants,
 * _u for Unicode (uchar_t).
 * Functions without suffix test character-encoding-neutral APIs.
 *
 * @note The result of each function is a struct with pass/fail/exception counts
 *       from all assertions executed within that module.
 * @{
 */
extern lub_test_result_t run_error_edge_tests_l(void);
extern lub_test_result_t run_error_edge_tests_u(void);
extern lub_test_result_t run_advanced_ops_tests_l(void);
extern lub_test_result_t run_advanced_ops_tests_u(void);
extern lub_test_result_t run_cmp_search_tests_l(void);
extern lub_test_result_t run_cmp_search_tests_u(void);
extern lub_test_result_t run_strlen_strclass_tests_l(void);
extern lub_test_result_t run_strlen_strclass_tests_u(void);
extern lub_test_result_t run_charclass_tests_l(void);
extern lub_test_result_t run_charclass_tests_u(void);
extern lub_test_result_t run_reserved_matrix_tests(void);
extern lub_test_result_t run_search_family_tests_l(void);
extern lub_test_result_t run_search_family_tests_u(void);
extern lub_test_result_t run_count_tests_l(void);
extern lub_test_result_t run_count_tests_u(void);
extern lub_test_result_t run_core_family_tests_l(void);
extern lub_test_result_t run_core_family_tests_u(void);
extern lub_test_result_t run_type_matrix_tests(void);
extern lub_test_result_t run_utilities_tests_l(void);
extern lub_test_result_t run_utilities_tests_u(void);
extern lub_test_result_t run_fuzz_edge_tests(void);
extern lub_test_result_t run_skip_tests_l(void);
extern lub_test_result_t run_skip_tests_u(void);
extern lub_test_result_t run_xmacros_tests_l(void);
extern lub_test_result_t run_xmacros_tests_u(void);
/** @} */

#endif
