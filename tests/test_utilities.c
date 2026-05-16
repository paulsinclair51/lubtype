/**
 * @file test_utilities.c
 * @brief X-macro tests for utility functions in lubtype.h
 *        (reverse/trim/pad/repeat), with cross-type variants under LUB_X_IS_L.
 *
 * @note Compiled twice: -DLUB_X_IS_L for Latin, -DLUB_X_IS_U for Unicode.
 *       Each test file needs `static lub_test_result_t test_result;`.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#if !defined(LUB_X_IS_L) && !defined(LUB_X_IS_U)
#define LUB_X_IS_L
#endif

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "../lubtype.h"
#include "lubtype_test_declarations.h"

static lub_test_result_t test_result;

/**
 * @brief Helper: Create a local xchar_t string from ASCII C string.
 */
static xchar_t *make_xstr_local(const char *src, xchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (xchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (xchar_t)(unsigned char)src[i];
    dst[i] = (xchar_t)0;
    return dst;
}

/**
 * @brief Helper: Compare xchar_t string with ASCII C string.
 */
static int eq_xstr_ascii_local(const xchar_t *s, const char *ascii) {
    size_t i = 0;
    if (!s || !ascii) return 0;
    for (; ascii[i]; ++i)
        if (s[i] != (xchar_t)(unsigned char)ascii[i]) return 0;
    return s[i] == (xchar_t)0;
}

#if defined(LUB_X_IS_L)
/**
 * @brief Helper: Create a local lchar_t string from ASCII C string (cross-type).
 */
static lchar_t *make_lstr_local(const char *src, lchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (lchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (lchar_t)(unsigned char)src[i];
    dst[i] = (lchar_t)0;
    return dst;
}
/**
 * @brief Helper: Create a local uchar_t string from ASCII C string (cross-type).
 */
static uchar_t *make_ustr_local(const char *src, uchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (uchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (uchar_t)(unsigned char)src[i];
    dst[i] = (uchar_t)0;
    return dst;
}
/**
 * @brief Helper: Compare lchar_t string with ASCII C string (cross-type).
 */
static int eq_lstr_ascii_local(const lchar_t *s, const char *ascii) {
    size_t i = 0;
    if (!s || !ascii) return 0;
    for (; ascii[i]; ++i)
        if (s[i] != (lchar_t)(unsigned char)ascii[i]) return 0;
    return s[i] == (lchar_t)0;
}
/**
 * @brief Helper: Compare uchar_t string with ASCII C string (cross-type).
 */
static int eq_ustr_ascii_local(const uchar_t *s, const char *ascii) {
    size_t i = 0;
    if (!s || !ascii) return 0;
    for (; ascii[i]; ++i)
        if (s[i] != (uchar_t)(unsigned char)ascii[i]) return 0;
    return s[i] == (uchar_t)0;
}
#endif /* LUB_X_IS_L */

static void test_reverse(void) {
    xchar_t dst[33];
    xchar_t src[32];

    LUB_ASSERT(xxsnnreverse(dst, 32, make_xstr_local("hello", src, 32), 32) != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(dst, "olleh"));

    LUB_ASSERT(xxsnnreverse(dst, 32, make_xstr_local("a", src, 32), 32) != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(dst, "a"));

    LUB_ASSERT(xxsnnreverse(dst, 32, make_xstr_local("", src, 32), 32) != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(dst, ""));

    LUB_ASSERT(xxsnnreverse(dst, 32, make_xstr_local("abc", src, 32), 32) != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(dst, "cba"));
}

static void test_trim(void) {
    xchar_t out[33];
    xchar_t src[64];
    xchar_t overlap[32];
    const xchar_t *trim_b = (const xchar_t[]){ 'B', 0 };
    const xchar_t *trim_l = (const xchar_t[]){ 'L', 0 };
    const xchar_t *trunc_r = (const xchar_t[]){ 'R', '.', '.', 0 };
    const xchar_t *trunc_l = (const xchar_t[]){ 'L', '.', '.', 0 };
    const xchar_t *trunc_c = (const xchar_t[]){ 'C', '.', '.', 0 };
    const xchar_t *trunc_b = (const xchar_t[]){ 'B', '.', '.', 0 };

    LUB_ASSERT(xxsnntrim(out, 32, make_xstr_local("  foo   bar  ", src, 64), 32,
                     0, trim_b, ' ') != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(out, "foo bar"));

    LUB_ASSERT(xxsnntrim(out, 32, make_xstr_local("  hello  ", src, 64), 32,
                     0, trim_l, 0) != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(out, "hello  "));

        LUB_ASSERT((intptr_t)xxsnntrim(out, 6,
            make_xstr_local("  abcdefghij  ", src, 64), 64,
            trunc_r, trim_b, 0) == (intptr_t)LUB_TRUNCATED);
        LUB_ASSERT(eq_xstr_ascii_local(out, "ab.."));

        LUB_ASSERT((intptr_t)xxsnntrim(out, 6,
            make_xstr_local("  abcdefghij  ", src, 64), 64,
            trunc_l, trim_b, 0) == (intptr_t)LUB_TRUNCATED);
        LUB_ASSERT(eq_xstr_ascii_local(out, "..ij"));

        LUB_ASSERT((intptr_t)xxsnntrim(out, 6,
            make_xstr_local("  abcdefghij  ", src, 64), 64,
            trunc_c, trim_b, 0) == (intptr_t)LUB_TRUNCATED);
        LUB_ASSERT(eq_xstr_ascii_local(out, "a..j"));

        LUB_ASSERT((intptr_t)xxsnntrim(out, 6,
            make_xstr_local("  abcdefghij  ", src, 64), 64,
            trunc_b, trim_b, 0) == (intptr_t)LUB_TRUNCATED);
        LUB_ASSERT(eq_xstr_ascii_local(out, ".."));

        make_xstr_local("  overlap  ", overlap, 32);
        LUB_ASSERT((intptr_t)xxsnntrim(overlap + 1, 10, overlap, 32,
            0, trim_b, 0) == (intptr_t)LUB_OVERLAP);
        LUB_ASSERT(overlap[1] == (xchar_t)0);
}

static void test_pad_and_repeat(void) {
    xchar_t xout[17];
    xchar_t xsrc[32];
    static const xchar_t xleft_pad[] = {'L', ' ', 0};
    static const xchar_t xright_pad[] = {'R', ' ', 0};
    static const xchar_t xboth_pad[] = {'B', '-', 0};

    LUB_ASSERT(xxsnnpad(xout, 8, make_xstr_local("hi", xsrc, 32), 32, xleft_pad) != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(xout, "      hi"));

    LUB_ASSERT(xxsnnpad(xout, 8, make_xstr_local("hi", xsrc, 32), 32, xright_pad) != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(xout, "hi      "));

    LUB_ASSERT(xxsnnpad(xout, 8, make_xstr_local("hi", xsrc, 32), 32, xboth_pad) != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(xout, "---hi---"));

    LUB_ASSERT(xxsnnrepeat(xout, 16, make_xstr_local("ab", xsrc, 32), 32, 4) != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(xout, "abababab"));

    LUB_ASSERT(xxsnnrepeat(xout, 16, make_xstr_local("ab", xsrc, 32), 32, 0) != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(xout, ""));

#if defined(LUB_X_IS_L)
    {
        lchar_t out[17];
        lchar_t src[32];
        uchar_t uout[17];
        uchar_t usrc[32];
        uchar_t usrc_nonlatin[4] = { 'h', 0x0100, 'i', 0 };
        static const lchar_t lright_pad_x[] = {'R', 'x', 0};
        static const uchar_t uright_pad[] = {'R', 'x', 0};

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
    }
#endif /* LUB_X_IS_L */
}

lub_test_result_t LUB_PASTE(run_utilities_tests_, LUB_X)(void) {
    test_result = (lub_test_result_t){0};
    test_reverse();
    test_trim();
    test_pad_and_repeat();

    printf("Utilities tests passed for LUB_X=%s.\n", LUB_STRINGIFY(LUB_X));
    return test_result;
}
