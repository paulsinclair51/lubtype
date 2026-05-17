#include "lubtype_test_declarations.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <signal.h>

#include "../lubtype.h"

// Triggers an exception inside an assertion
lub_test_result_t run_count_tests_l(void) {
    raise(SIGSEGV); // Simulate a segfault inside assertion
    return (lub_test_result_t){0, 0, 1};
}
lub_test_result_t run_count_tests_u(void) {
    raise(SIGSEGV);
    return (lub_test_result_t){0, 0, 1};
}
