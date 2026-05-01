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

static lchar_t *make_lstr_local(const char *src, lchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (lchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (lchar_t)(unsigned char)src[i];
    dst[i] = (lchar_t)0;
    return dst;
}

static uchar_t *make_ustr_local(const char *src, uchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (uchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (uchar_t)(unsigned char)src[i];
    dst[i] = (uchar_t)0;
    return dst;
}

static int eq_lstr_ascii_local(const lchar_t *s, const char *ascii) {
    size_t i = 0;
    if (!s || !ascii) return 0;
    for (; ascii[i]; ++i)
        if (s[i] != (lchar_t)(unsigned char)ascii[i]) return 0;
    return s[i] == (lchar_t)0;
}

static int eq_ustr_ascii_local(const uchar_t *s, const char *ascii) {
    size_t i = 0;
    if (!s || !ascii) return 0;
    for (; ascii[i]; ++i)
        if (s[i] != (uchar_t)(unsigned char)ascii[i]) return 0;
    return s[i] == (uchar_t)0;
}

static void test_reverse(void) {
    lchar_t dst[33];
    lchar_t src[32];
    uchar_t udst[33];
    uchar_t usrc[32];

    assert(llsnnreverse(dst, make_lstr_local("hello", src, 32), 32) != NULL);
    assert(eq_lstr_ascii_local(dst, "olleh"));

    assert(llsnnreverse(dst, make_lstr_local("a", src, 32), 32) != NULL);
    assert(eq_lstr_ascii_local(dst, "a"));

    assert(llsnnreverse(dst, make_lstr_local("", src, 32), 32) != NULL);
    assert(eq_lstr_ascii_local(dst, ""));

    assert(uusnnreverse(udst, make_ustr_local("abc", usrc, 32), 32) != NULL);
    assert(eq_ustr_ascii_local(udst, "cba"));
}

static void test_trim(void) {
    lchar_t out[33];
    lchar_t src[64];

    assert(llsnntrim(out, 32, make_lstr_local("  foo   bar  ", src, 64), 32,
                     0, NULL, 'B', ' ') != NULL);
    assert(eq_lstr_ascii_local(out, "foo bar"));

    assert(llsnntrim(out, 32, make_lstr_local("  hello  ", src, 64), 32,
                     0, NULL, 'L', 0) != NULL);
    assert(eq_lstr_ascii_local(out, "hello  "));
}

static void test_pad_and_repeat(void) {
    lchar_t out[17];
    lchar_t src[32];

    assert(llsnnpad(out, 8, make_lstr_local("hi", src, 32), 32, 'L', ' ') != NULL);
    assert(eq_lstr_ascii_local(out, "      hi"));

    assert(llsnnpad(out, 8, make_lstr_local("hi", src, 32), 32, 'R', ' ') != NULL);
    assert(eq_lstr_ascii_local(out, "hi      "));

    assert(llsnnpad(out, 8, make_lstr_local("hi", src, 32), 32, 'B', '-') != NULL);
    assert(eq_lstr_ascii_local(out, "---hi---"));

    assert(llsnnrepeat(out, 16, make_lstr_local("ab", src, 32), 32, 4) != NULL);
    assert(eq_lstr_ascii_local(out, "abababab"));

    assert(llsnnrepeat(out, 16, make_lstr_local("ab", src, 32), 32, 0) != NULL);
    assert(eq_lstr_ascii_local(out, ""));
}

void run_utilities_tests(void) {
    test_reverse();
    test_trim();
    test_pad_and_repeat();

}
