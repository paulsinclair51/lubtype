#include "lubtype_test_declarations.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <signal.h>

#include "../lubtype.h"

// Always triggers an assertion failure
lub_test_result_t run_charclass_tests_l(void) {
    LUB_ASSERT(0 && "Intentional assertion fail (Latin)");
    return (lub_test_result_t){0, 1, 0};
}
lub_test_result_t run_charclass_tests_u(void) {
    LUB_ASSERT(0 && "Intentional assertion fail (Unicode)");
    return (lub_test_result_t){0, 1, 0};
}
