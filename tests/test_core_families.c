/**
 * @file test_core_families.c
 * @brief X-macro tests for core function families in lubtype.h (xx, plus lu/ul
 *        cross-type variants under LUB_X_IS_L).
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
 * @brief Create a local xchar_t string from ASCII text.
 */
static xchar_t *make_xstr_local(const char *src, xchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (xchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (xchar_t)(unsigned char)src[i];
    dst[i] = (xchar_t)0;
    return dst;
}

/**
 * @brief Compare an xchar_t string to ASCII text for equality.
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
 * @brief Create a local lchar_t string from ASCII text (cross-type tests).
 */
static lchar_t *make_lstr_local(const char *src, lchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (lchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (lchar_t)(unsigned char)src[i];
    dst[i] = (lchar_t)0;
    return dst;
}
/**
 * @brief Create a local uchar_t string from ASCII text (cross-type tests).
 */
static uchar_t *make_ustr_local(const char *src, uchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (uchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (uchar_t)(unsigned char)src[i];
    dst[i] = (uchar_t)0;
    return dst;
}
/**
 * @brief Compare a lchar_t string to ASCII text for equality (cross-type tests).
 */
static int eq_lstr_ascii_local(const lchar_t *s, const char *ascii) {
    size_t i = 0;
    if (!s || !ascii) return 0;
    for (; ascii[i]; ++i)
        if (s[i] != (lchar_t)(unsigned char)ascii[i]) return 0;
    return s[i] == (lchar_t)0;
}
/**
 * @brief Compare a uchar_t string to ASCII text for equality (cross-type tests).
 */
static int eq_ustr_ascii_local(const uchar_t *s, const char *ascii) {
    size_t i = 0;
    if (!s || !ascii) return 0;
    for (; ascii[i]; ++i)
        if (s[i] != (uchar_t)(unsigned char)ascii[i]) return 0;
    return s[i] == (uchar_t)0;
}
#endif /* LUB_X_IS_L */

/**
 * @brief Test bounded copy variants (xx same-type, lu/ul cross-type under guard).
 */
static void test_copy_variants(void) {
    xchar_t dst[33], src[33];

    LUB_ASSERT(xxsnncpy(dst, 32, make_xstr_local("hello", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(dst, "hello"));
    LUB_ASSERT(xxsnncpyc(dst, 32, make_xstr_local("HELLO", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(dst, "hello"));
    LUB_ASSERT(xxsnnCPYC(dst, 32, make_xstr_local("hello", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(dst, "HELLO"));

#if defined(LUB_X_IS_L)
    {
        lchar_t dst_l[33], src_l[33];
        uchar_t dst_u[33], src_u[33];

        LUB_ASSERT(lusnncpy(dst_l, 32, make_ustr_local("abc", src_u, 33), 32, NULL, '?') != NULL);
        LUB_ASSERT(eq_lstr_ascii_local(dst_l, "abc"));
        LUB_ASSERT(lusnncpyc(dst_l, 32, make_ustr_local("DEF", src_u, 33), 32, NULL, '?') != NULL);
        LUB_ASSERT(eq_lstr_ascii_local(dst_l, "def"));
        LUB_ASSERT(lusnnCPYC(dst_l, 32, make_ustr_local("ghi", src_u, 33), 32, NULL, '?') != NULL);
        LUB_ASSERT(eq_lstr_ascii_local(dst_l, "GHI"));

        LUB_ASSERT(ulsnncpy(dst_u, 32, make_lstr_local("123", src_l, 33), 32, NULL) != NULL);
        LUB_ASSERT(eq_ustr_ascii_local(dst_u, "123"));
        LUB_ASSERT(ulsnncpyc(dst_u, 32, make_lstr_local("ABC", src_l, 33), 32, NULL) != NULL);
        LUB_ASSERT(eq_ustr_ascii_local(dst_u, "abc"));
    }
#endif /* LUB_X_IS_L */
}

static void test_cat_basic(void) {
    xchar_t dst[33], src[33];

    LUB_ASSERT(xxsnncpy(dst, 32, make_xstr_local("hello", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(xxsnncat(dst, 32, make_xstr_local(" world", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(dst, "hello world"));

    LUB_ASSERT(xxsnncpy(dst, 32, make_xstr_local("foo", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(xxsnncatc(dst, 32, make_xstr_local("BAR", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(dst, "foobar"));

    LUB_ASSERT(xxsnncpy(dst, 32, make_xstr_local("foo", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(xxsnnCATC(dst, 32, make_xstr_local("baz", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_xstr_ascii_local(dst, "fooBAZ"));

#if defined(LUB_X_IS_L)
    {
        lchar_t ldst[33], lsrc[33];
        uchar_t udst[33], usrc[33];

        LUB_ASSERT(llsnncpy(ldst, 32, make_lstr_local("abc", lsrc, 33), 32, NULL) != NULL);
        LUB_ASSERT(lusnncat(ldst, 32, make_ustr_local("def", usrc, 33), 32, NULL, '?') != NULL);
        LUB_ASSERT(eq_lstr_ascii_local(ldst, "abcdef"));

        LUB_ASSERT(llsnncpy(ldst, 32, make_lstr_local("foo", lsrc, 33), 32, NULL) != NULL);
        LUB_ASSERT(lusnncatc(ldst, 32, make_ustr_local("BAR", usrc, 33), 32, NULL, '?') != NULL);
        LUB_ASSERT(eq_lstr_ascii_local(ldst, "foobar"));

        LUB_ASSERT(llsnncpy(ldst, 32, make_lstr_local("foo", lsrc, 33), 32, NULL) != NULL);
        LUB_ASSERT(lusnnCATC(ldst, 32, make_ustr_local("baz", usrc, 33), 32, NULL, '?') != NULL);
        LUB_ASSERT(eq_lstr_ascii_local(ldst, "fooBAZ"));

        LUB_ASSERT(uusnncpy(udst, 32, make_ustr_local("abc", usrc, 33), 32, NULL) != NULL);
        LUB_ASSERT(ulsnncat(udst, 32, make_lstr_local("def", lsrc, 33), 32, NULL) != NULL);
        LUB_ASSERT(eq_ustr_ascii_local(udst, "abcdef"));

        LUB_ASSERT(uusnncpy(udst, 32, make_ustr_local("foo", usrc, 33), 32, NULL) != NULL);
        LUB_ASSERT(ulsnncatc(udst, 32, make_lstr_local("BAR", lsrc, 33), 32, NULL) != NULL);
        LUB_ASSERT(eq_ustr_ascii_local(udst, "foobar"));

        LUB_ASSERT(uusnncpy(udst, 32, make_ustr_local("foo", usrc, 33), 32, NULL) != NULL);
        LUB_ASSERT(ulsnnCATC(udst, 32, make_lstr_local("baz", lsrc, 33), 32, NULL) != NULL);
        LUB_ASSERT(eq_ustr_ascii_local(udst, "fooBAZ"));
    }
#endif /* LUB_X_IS_L */
}

static void test_copy_and_cat_error_behavior(void) {
    xchar_t dst[33], src[33];

    dst[0] = 'X';
    dst[1] = 0;
    LUB_ASSERT(xxsnncpy(NULL, 32, make_xstr_local("abc", src, 33), 32, NULL) == NULL);
    LUB_ASSERT(dst[0] == 'X');

    LUB_ASSERT(xxsnncpy(dst, 32, NULL, 32, NULL) != NULL);
    LUB_ASSERT(dst[0] == 0);

    // Mixed-type overlap behavior is implementation-dependent in current
    // version, so avoid asserting legacy overlap-error outcomes here.
}

#define CORE_FAMILY_TESTS(X) \
    X(test_copy_variants) \
    X(test_cat_basic) \
    X(test_copy_and_cat_error_behavior)

lub_test_result_t LUB_PASTE(run_core_family_tests_, LUB_X)(void) {
    test_result = (lub_test_result_t){0};
    #define RUN_TEST(fn) fn();
    CORE_FAMILY_TESTS(RUN_TEST)
    #undef RUN_TEST
    printf("Core family tests passed for LUB_X=%s.\n", LUB_STRINGIFY(LUB_X));
    return test_result;
}
