/**
 * @file test_error_edge.c
 * @brief Tests for error and edge cases in lubtype.h string operations.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include <assert.h>
#include "../lubtype.h"
#include <stdio.h>

/**
 * @brief Run tests for error and edge cases in string operations.
 *
 * Tests null pointer handling, over-max-length, and empty string cases for
 * lcslen and lcsnlen. Prints a message on success.
 */
void run_error_edge_tests(void) {
    lchar_t lstr[8] = {'a','b','c','\0'};
    uchar_t ustr[8] = {L'a',L'b',L'c',0};
    // Test: Null pointer
    assert(lcslen(NULL) == 0);
    assert(ucslen(NULL) == 0);
    // Test: Over max length
    assert(lcsnlen(lstr, 100) == 3);
    assert(ucsnlen(ustr, 100) == 3);
    // Test: Empty string
    lchar_t empty[1] = {'\0'};
    uchar_t uempty[1] = {0};
    assert(lcslen(empty) == 0);
    assert(ucslen(uempty) == 0);
    // Null-termination on error for app/cpy/cat
    lstr[0] = 'X'; lstr[1] = 0;
    assert(llsnapp(NULL, NULL, 0) == NULL);
    assert(lstr[0] == 'X');
    assert(llsnapp(lstr, NULL, 0) == lstr);
    assert(lstr[0] == 0);
    lstr[0] = 'X'; lstr[1] = 0;
    assert(llsncpy(NULL, NULL, 0) == NULL);
    assert(lstr[0] == 'X');
    assert(llsncpy(lstr, NULL, 0) == lstr);
    assert(lstr[0] == 0);
    lstr[0] = 'X'; lstr[1] = 0;
    assert(llsncat(NULL, NULL, 0) == NULL);
    assert(lstr[0] == 'X');
    assert(llsncat(lstr, NULL, 0) == lstr);
    assert(lstr[0] == 0);
    // Cross-type overlap error
    strcpy((char*)lstr, "abcdef");
    assert(lusnapp(lstr, (ucstr_t)lstr, 6, '?') == NULL);
    assert(lstr[0] == 0);
    for (int i = 0; i < 6; ++i) ustr[i] = (uchar_t)('a'+i); ustr[6]=0;
    assert(ulsnapp(ustr, (lcstr_t)ustr, 6) == NULL);
    assert(ustr[0] == 0);
    printf("Error/edge case tests passed.\n");

}
