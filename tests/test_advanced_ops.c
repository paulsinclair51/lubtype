/**
 * @file test_advanced_ops.c
 * @brief Tests for advanced string operations in lubtype.h (trim/rev/pad/replace).
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include <assert.h>
#include "../lubtype.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief Run tests for advanced string operations (trim, reverse, pad, replace, split).
 */
void run_advanced_ops_tests(void) {
        // --- llsptrim/uuptrim edge cases ---
        // Empty string
        lchar_t empty[1] = {'\0'};
        size_t len;
        const lchar_t *view = llsptrim(empty, &len, 'B', NULL);
        assert(view == empty && len == 0);
        // No trim needed
        lchar_t noneed[8] = {'a','b','c','\0'};
        view = llsptrim(noneed, &len, 'B', NULL);
        assert(view == noneed && len == 3);
        // Trimset with only one char
        lchar_t onechar[8] = {'x','a','x','b','x','\0'};
        view = llsptrim(onechar, &len, 'B', "x");
        assert(view == onechar + 1 && len == 3);
        // Trimset with non-ASCII
        lchar_t nonascii[8] = {(lchar_t)0xA0,'a','b',(lchar_t)0xA0,'\0'};
        view = llsptrim(nonascii, &len, 'B', (const char*)"\xA0");
        assert(view == nonascii + 1 && len == 2);
        // NULL out_len
        assert(llsptrim(noneed, NULL, 'B', NULL) == NULL);
        // NULL s
        assert(llsptrim(NULL, &len, 'B', NULL) == NULL);
        // Unterminated string (simulate by omitting null, but lcsnlen will clamp)
        lchar_t unterm[4] = {'a','b','c','d'};
        view = llsptrim(unterm, &len, 'B', NULL);
        // Should not crash, but may return NULL due to missing terminator
        // (behavior depends on lcsnlen implementation)

        // Unicode: Empty string
        uchar_t uempty[1] = {0};
        size_t ulen;
        const uchar_t *uview = uuptrim(uempty, &ulen, 'B', NULL);
        assert(uview == uempty && ulen == 0);
        // No trim needed
        uchar_t unoneed[8] = {'a','b','c',0};
        uview = uuptrim(unoneed, &ulen, 'B', NULL);
        assert(uview == unoneed && ulen == 3);
        // Trimset with only one char
        uchar_t uonechar[8] = {'x','a','x','b','x',0};
        uview = uuptrim(uonechar, &ulen, 'B', (const uchar_t*)"x");
        assert(uview == uonechar + 1 && ulen == 3);
        // Trimset with non-ASCII
        uchar_t unonascii[8] = {0xA0,'a','b',0xA0,0};
        uview = uuptrim(unonascii, &ulen, 'B', (const uchar_t*)"\xA0");
        assert(uview == unonascii + 1 && ulen == 2);
        // NULL out_len
        assert(uuptrim(unoneed, NULL, 'B', NULL) == NULL);
        // NULL s
        assert(uuptrim(NULL, &ulen, 'B', NULL) == NULL);
        // Unterminated string (simulate by omitting null, but ucsnlen will clamp)
        uchar_t uunterm[4] = {'a','b','c','d'};
        uview = uuptrim(uunterm, &ulen, 'B', NULL);
        // Should not crash, but may return NULL due to missing terminator
        // (behavior depends on ucsnlen implementation)
    lchar_t lstr[32] = {' ','a','b','c',' ','\0'};
    lchar_t out[32];
    // Test: trim (both sides, delim = ' ')
    llsntrim(out, lstr, 32, 'B', ' ');
    assert(strcmp((char*)out, "abc") == 0);
    // --- llsptrim tests ---
    size_t len;
    // Both sides, whitespace
    const lchar_t *view = llsptrim(lstr, &len, 'B', NULL);
    assert(view == lstr + 1 && len == 3);
    assert(strncmp((char*)view, "abc", len) == 0);
    // Left only, whitespace
    view = llsptrim(lstr, &len, 'L', NULL);
    assert(view == lstr + 1 && len == 4);
    assert(strncmp((char*)view, "abc ", len) == 0);
    // Right only, whitespace
    view = llsptrim(lstr, &len, 'R', NULL);
    assert(view == lstr && len == 4);
    assert(strncmp((char*)view, " abc", len) == 0);
    // All whitespace
    lchar_t allws[8] = {' ','\t','\n','\r','\v','\f',' ','\0'};
    view = llsptrim(allws, &len, 'B', NULL);
    assert(view == allws && len == 0);
    // Custom trimset (trim 'a' and ' ')
    lchar_t custom[8] = {'a',' ','b','a','c','a',' ','\0'};
    view = llsptrim(custom, &len, 'B', "a ");
    assert(view == custom + 2 && len == 2);
    assert(strncmp((char*)view, "ba", len) == 0);

    // --- uuptrim tests ---
    uchar_t ustr[32] = {0x20, 'a', 'b', 'c', 0x20, 0};
    // Both sides, whitespace
    size_t ulen;
    const uchar_t *uview = uuptrim(ustr, &ulen, 'B', NULL);
    assert(uview == ustr + 1 && ulen == 3);
    // Left only, whitespace
    uview = uuptrim(ustr, &ulen, 'L', NULL);
    assert(uview == ustr + 1 && ulen == 4);
    // Right only, whitespace
    uview = uuptrim(ustr, &ulen, 'R', NULL);
    assert(uview == ustr && ulen == 4);
    // All whitespace
    uchar_t uallws[8] = {0x20, 0x09, 0x0A, 0x0D, 0x0B, 0x0C, 0x20, 0};
    uview = uuptrim(uallws, &ulen, 'B', NULL);
    assert(uview == uallws && ulen == 0);
    // Custom trimset (trim 'a' and space)
    uchar_t ucustom[8] = {'a',' ', 'b','a','c','a',' ',0};
    uview = uuptrim(ucustom, &ulen, 'B', (const uchar_t*)"a ");
    assert(uview == ucustom + 2 && ulen == 2);
    printf("llsptrim/uuptrim tests passed.\n");
    // Test: reverse
    llsnrev(out, lstr, 32);
    assert(strcmp((char*)out, " cba ") == 0);
    // Test: pad (pad right, m = -1)
    llsnpad(out, 8, (lchar_t*)"abc", 3, 'x', -1);
    assert(strncmp((char*)out, "abcxxxxx", 8) == 0);
    // Test: replace (replace 'b' with 'z', single char map)
    llsnreplace(out, 6, (lchar_t*)"aabbcc", (lchar_t*)"b|z", '|', 6, 0);
    assert(strcmp((char*)out, "aazzcc") == 0);
    // Test: split
    lchar_t left[16], right[16];
    llsnsplit(left, right, (lchar_t*)"key=value", '=', 16);
    assert(strcmp((char*)left, "key") == 0);
    assert(strcmp((char*)right, "value") == 0);
    printf("Advanced operation tests passed.\n");

}
