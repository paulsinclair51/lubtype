#include "lubtype_test_declarations.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <signal.h>

#include "../lubtype.h"

// Triggers a category-level exception (outside assertion)
lub_test_result_t run_skip_tests_l(void) {
    raise(SIGABRT); // Simulate abort outside assertion
    return (lub_test_result_t){0, 0, 1};
}
lub_test_result_t run_skip_tests_u(void) {
    raise(SIGABRT);
    return (lub_test_result_t){0, 0, 1};
}
