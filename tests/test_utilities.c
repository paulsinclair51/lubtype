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
#include "lubtype_test_declarations.h"

static lub_test_result_t test_result;

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

    LUB_ASSERT(llsnnreverse(dst, 32, make_lstr_local("hello", src, 32), 32) != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(dst, "olleh"));

    LUB_ASSERT(llsnnreverse(dst, 32, make_lstr_local("a", src, 32), 32) != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(dst, "a"));

    LUB_ASSERT(llsnnreverse(dst, 32, make_lstr_local("", src, 32), 32) != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(dst, ""));

    LUB_ASSERT(uusnnreverse(udst, 32, make_ustr_local("abc", usrc, 32), 32) != NULL);
    LUB_ASSERT(eq_ustr_ascii_local(udst, "cba"));
}

static void test_trim(void) {
    lchar_t out[33];
    lchar_t src[64];
    lchar_t overlap[32];
    const lchar_t *trim_b = (const lchar_t[]){ 'B', 0 };
    const lchar_t *trim_l = (const lchar_t[]){ 'L', 0 };
    const lchar_t *trunc_r = (const lchar_t[]){ 'R', '.', '.', 0 };
    const lchar_t *trunc_l = (const lchar_t[]){ 'L', '.', '.', 0 };
    const lchar_t *trunc_c = (const lchar_t[]){ 'C', '.', '.', 0 };
    const lchar_t *trunc_b = (const lchar_t[]){ 'B', '.', '.', 0 };

    LUB_ASSERT(llsnntrim(out, 32, make_lstr_local("  foo   bar  ", src, 64), 32,
                     0, trim_b, ' ') != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(out, "foo bar"));

    LUB_ASSERT(llsnntrim(out, 32, make_lstr_local("  hello  ", src, 64), 32,
                     0, trim_l, 0) != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(out, "hello  "));

        LUB_ASSERT((intptr_t)llsnntrim(out, 6,
            make_lstr_local("  abcdefghij  ", src, 64), 64,
            trunc_r, trim_b, 0) == (intptr_t)LUB_TRUNCATED);
        LUB_ASSERT(eq_lstr_ascii_local(out, "ab.."));

        LUB_ASSERT((intptr_t)llsnntrim(out, 6,
            make_lstr_local("  abcdefghij  ", src, 64), 64,
            trunc_l, trim_b, 0) == (intptr_t)LUB_TRUNCATED);
        LUB_ASSERT(eq_lstr_ascii_local(out, "..ij"));

        LUB_ASSERT((intptr_t)llsnntrim(out, 6,
            make_lstr_local("  abcdefghij  ", src, 64), 64,
            trunc_c, trim_b, 0) == (intptr_t)LUB_TRUNCATED);
        LUB_ASSERT(eq_lstr_ascii_local(out, "a..j"));

        LUB_ASSERT((intptr_t)llsnntrim(out, 6,
            make_lstr_local("  abcdefghij  ", src, 64), 64,
            trunc_b, trim_b, 0) == (intptr_t)LUB_TRUNCATED);
        LUB_ASSERT(eq_lstr_ascii_local(out, ".."));

        make_lstr_local("  overlap  ", overlap, 32);
        LUB_ASSERT((intptr_t)llsnntrim(overlap + 1, 10, overlap, 32,
            0, trim_b, 0) == (intptr_t)LUB_OVERLAP);
        LUB_ASSERT(overlap[1] == (lchar_t)0);
}

static void test_pad_and_repeat(void) {
    lchar_t out[17];
    lchar_t src[32];
    uchar_t uout[17];
    uchar_t usrc[32];
    uchar_t usrc_nonlatin[4] = { 'h', 0x0100, 'i', 0 };
    static const lchar_t left_pad[] = {'L', ' ', 0};
    static const lchar_t right_pad[] = {'R', ' ', 0};
    static const lchar_t both_pad[] = {'B', '-', 0};
    static const lchar_t lright_pad_x[] = {'R', 'x', 0};
    static const uchar_t uright_pad[] = {'R', 'x', 0};
    static const uchar_t uboth_pad[] = {'B', '-', 0};

    LUB_ASSERT(llsnnpad(out, 8, make_lstr_local("hi", src, 32), 32, left_pad) != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(out, "      hi"));

    LUB_ASSERT(llsnnpad(out, 8, make_lstr_local("hi", src, 32), 32, right_pad) != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(out, "hi      "));

    LUB_ASSERT(llsnnpad(out, 8, make_lstr_local("hi", src, 32), 32, both_pad) != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(out, "---hi---"));

    LUB_ASSERT(lusnnpad(out, 8, make_ustr_local("hi", usrc, 32), 32,
                    lright_pad_x, 0) != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(out, "hixxxxxx"));

    LUB_ASSERT(lusnnpad(out, 8, usrc_nonlatin, 4, lright_pad_x, '?') != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(out, "h?ixxxxx"));

        LUB_ASSERT(lusnnpad(out, 8, usrc_nonlatin, 4, lright_pad_x, 0) ==
            (lchar_t *)LUB_NON_LATIN_CHAR);
        LUB_ASSERT(out[0] == (lchar_t)0);

    LUB_ASSERT(ulsnnpad(uout, 8, make_lstr_local("hi", src, 32), 32, uright_pad) != NULL);
    LUB_ASSERT(eq_ustr_ascii_local(uout, "hixxxxxx"));

    LUB_ASSERT(uusnnpad(uout, 8, make_ustr_local("hi", usrc, 32), 32, uboth_pad) != NULL);
    LUB_ASSERT(eq_ustr_ascii_local(uout, "---hi---"));

    LUB_ASSERT(llsnnrepeat(out, 16, make_lstr_local("ab", src, 32), 32, 4) != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(out, "abababab"));

    LUB_ASSERT(llsnnrepeat(out, 16, make_lstr_local("ab", src, 32), 32, 0) != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(out, ""));
}

lub_test_result_t run_utilities_tests(void) {
    test_result = (lub_test_result_t){0};
    test_reverse();
    test_trim();
    test_pad_and_repeat();

    return test_result;
}
