/**
 * @file test_utilities.c
 * @brief Tests for utility functions in lubtype.h (reverse/split/length).
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include <assert.h>
#include <stddef.h>

#include "../lubtype.h"

static lcstr_t make_lstr_local(const char *src, lstr_t dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (lcstr_t)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (lchar_t)(unsigned char)src[i];
    dst[i] = (lchar_t)0;
    return (lcstr_t)dst;
}

static ucstr_t make_ustr_local(const char *src, ustr_t dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (ucstr_t)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (uchar_t)(unsigned char)src[i];
    dst[i] = (uchar_t)0;
    return (ucstr_t)dst;
}

static int eq_lstr_ascii_local(lcstr_t s, const char *ascii) {
    size_t i = 0;
    if (!s || !ascii) return 0;
    for (; ascii[i]; ++i)
        if (s[i] != (lchar_t)(unsigned char)ascii[i]) return 0;
    return s[i] == (lchar_t)0;
}

static void test_reverse(void) {
    lchar_t dst[32];
    lchar_t src[32];

    llsnrev(dst, make_lstr_local("hello", src, 32), 32);
    assert(eq_lstr_ascii_local(dst, "olleh"));

    llsnrev(dst, make_lstr_local("a", src, 32), 32);
    assert(eq_lstr_ascii_local(dst, "a"));

    llsnrev(dst, make_lstr_local("", src, 32), 32);
    assert(eq_lstr_ascii_local(dst, ""));
}

static void test_split(void) {
    lchar_t left[32];
    lchar_t right[32];
    lchar_t src[64];
    size_t result;

    result = llsnsplit(left, right, make_lstr_local("key=value", src, 64), '=', 64);
    assert(result == 2);
    assert(eq_lstr_ascii_local(left, "key"));
    assert(eq_lstr_ascii_local(right, "value"));

    result = llsnsplit(left, right, make_lstr_local("nodealim", src, 64), '=', 64);
    assert(result == 1);
    assert(eq_lstr_ascii_local(left, "nodealim"));
    assert(eq_lstr_ascii_local(right, ""));
}

static void test_length_functions(void) {
    lchar_t lstr[32];
    uchar_t ustr[32];
    size_t result;

    result = lcsnlen(make_lstr_local("hello", lstr, 32), 32);
    assert(result == 5);

    result = ucsnlen(make_ustr_local("hello", ustr, 32), 32);
    assert(result == 5);

    result = lcsnlen(make_lstr_local("", lstr, 32), 32);
    assert(result == 0);
}

void run_utilities_tests(void) {
    test_reverse();
    test_split();
    test_length_functions();

}
