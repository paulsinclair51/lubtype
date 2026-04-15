/**
 * @file test_charclass.c
 * @brief Tests for character classification functions in lubtype.h.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include <assert.h>
#include "../lubtype.h"
#include <stdio.h>

/**
 * @brief Run tests for character classification and conversion functions.
 *
 * Tests isualpha, islalpha, isuupper, islupper, isulower, isllower,
 * isudigit, isldigit, isualnum, islalnum, touupper, tolupper, toulower, tollower.
 * Also checks edge cases for null and non-alphabetic input.
 */
void run_charclass_tests(void) {
    // Test: isualpha, islalpha
    assert(isualpha(L'A') && isualpha(L'z'));
    assert(islalpha('A') && islalpha('z'));
    assert(!isualpha(L'1'));
    assert(!islalpha('1'));
    // Test: isuupper, islupper
    assert(isuupper(L'Z'));
    assert(!isuupper(L'a'));
    assert(islupper('Z'));
    assert(!islupper('a'));
    // Test: isulower, isllower
    assert(isulower(L'a'));
    assert(!isulower(L'Z'));
    assert(isllower('a'));
    assert(!isllower('Z'));
    // Test: isudigit, isldigit
    assert(isudigit(L'5'));
    assert(!isudigit(L'A'));
    assert(isldigit('5'));
    assert(!isldigit('A'));
    // Test: isualnum, islalnum
    assert(isualnum(L'9'));
    assert(isualnum(L'B'));
    assert(!isualnum(L'!'));
    assert(islalnum('9'));
    assert(islalnum('B'));
    assert(!islalnum('!'));
    // Test: uutoupper, lltoupper, uutolower, lltolower
    assert(uutoupper(L'a') == L'A');
    assert(lltoupper('a') == 'A');
    assert(uutolower(L'Z') == L'z');
    assert(lltolower('Z') == 'z');
    // Null/empty/error edge cases
    assert(!isualpha(0));
    assert(!islalpha(0));
    assert(!isuupper(0));
    assert(!islupper(0));
    assert(!isulower(0));
    assert(!isllower(0));
    assert(!isudigit(0));
    assert(!isldigit(0));
    assert(!isualnum(0));
    assert(!islalnum(0));
    assert(touupper(0) == 0);
    assert(lltoupper(0) == 0);
    assert(uutolower(0) == 0);
    assert(lltolower(0) == 0);
    assert(touupper(L'!') == L'!');
    assert(tolower('!') == '!');
    printf("Character classification/conversion tests passed.\n");

}
