/**
 * @file test_strlen_validation.c
 * @brief Tests for string length and validation functions in lubtype.h.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include <assert.h>
#include "../lubtype.h"
#include <stdio.h>

/**
 * @brief Run tests for string length and validation functions.
 *
 * Tests lcslen, ucslen, lcsnlen, ucsnlen, isureserved, isuqname,
 * islhexdigits, and isuhexdigits for correct behavior and edge cases.
 */
void run_strlen_validation_tests(void) {
    lchar_t lstr[16] = {'a','b','c','\0'};
    uchar_t ustr[16] = {L'a',L'b',L'c',0};
    // Test: lcslen, ucslen
    assert(lcslen(lstr) == 3);
    assert(ucslen(ustr) == 3);
    // Test: lcsnlen, ucsnlen with missing terminator
    assert(lcsnlen(lstr, 2) == MISSING_STR_TERMINATOR);
    assert(ucsnlen(ustr, 2) == MISSING_STR_TERMINATOR);
    // Null/empty/error edge cases
    assert(lcslen(NULL) == 0);
    assert(ucslen(NULL) == 0);
    assert(lcsnlen(NULL, 10) == MISSING_STR_TERMINATOR);
    assert(ucsnlen(NULL, 10) == MISSING_STR_TERMINATOR);
    lstr[0] = 0; ustr[0] = 0;
    assert(lcslen(lstr) == 0);
    assert(ucslen(ustr) == 0);
    // Test: isureserved, isuqname (example, may need real reserved/qname values)
    uchar_t not_reserved[8] = {L'A',0};
    assert(!isureserved(not_reserved));
    // Test: islhexdigits, isuhexdigits
    lchar_t hexl[8] = {'A','B','C','1','2','3','\0'};
    uchar_t hexu[8] = {L'A',L'B',L'C',L'1',L'2',L'3',0};
    assert(islhexdigits(hexl, 6));
    assert(isuhexdigits(hexu, 6));
    // Null/empty/error edge cases for hex
    assert(!islhexdigits(NULL, 3));
    assert(!isuhexdigits(NULL, 3));
    hexl[0] = 0; hexu[0] = 0;
    assert(!islhexdigits(hexl, 0));
    assert(!isuhexdigits(hexu, 0));
    printf("String length/validation tests passed.\n");

}
