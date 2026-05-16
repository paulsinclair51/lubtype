/**
 * @file test_core_families.c
 * @brief Tests for core function families in lubtype.h (ll/lu/ul/uu).
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include <assert.h>
#include <stddef.h>

#include "../lubtype.h"
#include "lubtype_test_declarations.h"

static lub_test_result_t test_result;

/**
 * @brief Create a local Latin string from ASCII text.
 */
static lchar_t *make_lstr_local(const char *src, lchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (lchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (lchar_t)(unsigned char)src[i];
    dst[i] = (lchar_t)0;
    return dst;
}

/**
 * @brief Create a local Unicode string from ASCII text.
 */
static uchar_t *make_ustr_local(const char *src, uchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (uchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (uchar_t)(unsigned char)src[i];
    dst[i] = (uchar_t)0;
    return dst;
}

/**
 * @brief Compare a Latin string to ASCII text for equality.
 */
static int eq_lstr_ascii_local(const lchar_t *s, const char *ascii) {
    size_t i = 0;
    if (!s || !ascii) return 0;
    for (; ascii[i]; ++i)
        if (s[i] != (lchar_t)(unsigned char)ascii[i]) return 0;
    return s[i] == (lchar_t)0;
}

/**
 * @brief Compare a Unicode string to ASCII text for equality.
 */
static int eq_ustr_ascii_local(const uchar_t *s, const char *ascii) {
    size_t i = 0;
    if (!s || !ascii) return 0;
    for (; ascii[i]; ++i)
        if (s[i] != (uchar_t)(unsigned char)ascii[i]) return 0;
    return s[i] == (uchar_t)0;
}

/**
 * @brief Test bounded copy variants for core families.
 */
static void test_copy_variants(void) {
    lchar_t dst_l[33], src_l[33];
    uchar_t dst_u[33], src_u[33];

    LUB_ASSERT(llsnncpy(dst_l, 32, make_lstr_local("hello", src_l, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(dst_l, "hello"));
    LUB_ASSERT(llsnncpyc(dst_l, 32, make_lstr_local("HELLO", src_l, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(dst_l, "hello"));
    LUB_ASSERT(llsnnCPYC(dst_l, 32, make_lstr_local("hello", src_l, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(dst_l, "HELLO"));

    LUB_ASSERT(uusnncpy(dst_u, 32, make_ustr_local("foo", src_u, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_ustr_ascii_local(dst_u, "foo"));
    LUB_ASSERT(uusnncpyc(dst_u, 32, make_ustr_local("BAR", src_u, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_ustr_ascii_local(dst_u, "bar"));
    LUB_ASSERT(uusnnCPYC(dst_u, 32, make_ustr_local("baz", src_u, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_ustr_ascii_local(dst_u, "BAZ"));

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

static void test_cat_basic(void) {
    lchar_t dst[33], src[33];
    uchar_t udst[33], usrc[33];

    LUB_ASSERT(llsnncpy(dst, 32, make_lstr_local("hello", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(llsnncat(dst, 32, make_lstr_local(" world", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(dst, "hello world"));

    LUB_ASSERT(llsnncpy(dst, 32, make_lstr_local("foo", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(llsnncatc(dst, 32, make_lstr_local("BAR", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(dst, "foobar"));

    LUB_ASSERT(llsnncpy(dst, 32, make_lstr_local("foo", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(llsnnCATC(dst, 32, make_lstr_local("baz", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(dst, "fooBAZ"));

    LUB_ASSERT(uusnncpy(udst, 32, make_ustr_local("abc", usrc, 33), 32, NULL) != NULL);
    LUB_ASSERT(uusnncat(udst, 32, make_ustr_local("def", usrc, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_ustr_ascii_local(udst, "abcdef"));

    LUB_ASSERT(uusnncpy(udst, 32, make_ustr_local("foo", usrc, 33), 32, NULL) != NULL);
    LUB_ASSERT(uusnncatc(udst, 32, make_ustr_local("BAR", usrc, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_ustr_ascii_local(udst, "foobar"));

    LUB_ASSERT(uusnncpy(udst, 32, make_ustr_local("foo", usrc, 33), 32, NULL) != NULL);
    LUB_ASSERT(uusnnCATC(udst, 32, make_ustr_local("baz", usrc, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_ustr_ascii_local(udst, "fooBAZ"));

    LUB_ASSERT(llsnncpy(dst, 32, make_lstr_local("abc", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(lusnncat(dst, 32, make_ustr_local("def", usrc, 33), 32, NULL, '?') != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(dst, "abcdef"));

    LUB_ASSERT(llsnncpy(dst, 32, make_lstr_local("foo", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(lusnncatc(dst, 32, make_ustr_local("BAR", usrc, 33), 32, NULL, '?') != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(dst, "foobar"));

    LUB_ASSERT(llsnncpy(dst, 32, make_lstr_local("foo", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(lusnnCATC(dst, 32, make_ustr_local("baz", usrc, 33), 32, NULL, '?') != NULL);
    LUB_ASSERT(eq_lstr_ascii_local(dst, "fooBAZ"));

    LUB_ASSERT(uusnncpy(udst, 32, make_ustr_local("abc", usrc, 33), 32, NULL) != NULL);
    LUB_ASSERT(ulsnncat(udst, 32, make_lstr_local("def", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_ustr_ascii_local(udst, "abcdef"));

    LUB_ASSERT(uusnncpy(udst, 32, make_ustr_local("foo", usrc, 33), 32, NULL) != NULL);
    LUB_ASSERT(ulsnncatc(udst, 32, make_lstr_local("BAR", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_ustr_ascii_local(udst, "foobar"));

    LUB_ASSERT(uusnncpy(udst, 32, make_ustr_local("foo", usrc, 33), 32, NULL) != NULL);
    LUB_ASSERT(ulsnnCATC(udst, 32, make_lstr_local("baz", src, 33), 32, NULL) != NULL);
    LUB_ASSERT(eq_ustr_ascii_local(udst, "fooBAZ"));
}

static void test_copy_and_cat_error_behavior(void) {
    lchar_t dst[33], src[33];

    dst[0] = 'X';
    dst[1] = 0;
    LUB_ASSERT(llsnncpy(NULL, 32, make_lstr_local("abc", src, 33), 32, NULL) == NULL);
    LUB_ASSERT(dst[0] == 'X');

    LUB_ASSERT(llsnncpy(dst, 32, NULL, 32, NULL) != NULL);
    LUB_ASSERT(dst[0] == 0);

    // Mixed-type overlap behavior is implementation-dependent in current
    // version, so avoid asserting legacy overlap-error outcomes here.
}

lub_test_result_t run_core_family_tests(void) {
    test_result = (lub_test_result_t){0};
    test_copy_variants();
    test_cat_basic();
    test_copy_and_cat_error_behavior();
    return test_result;
}
