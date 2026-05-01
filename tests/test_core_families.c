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

    assert(llsnncpy(dst_l, 32, make_lstr_local("hello", src_l, 33), 32, NULL) != NULL);
    assert(eq_lstr_ascii_local(dst_l, "hello"));
    assert(llsnncpyc(dst_l, 32, make_lstr_local("HELLO", src_l, 33), 32, NULL) != NULL);
    assert(eq_lstr_ascii_local(dst_l, "hello"));
    assert(llsnnCPYC(dst_l, 32, make_lstr_local("hello", src_l, 33), 32, NULL) != NULL);
    assert(eq_lstr_ascii_local(dst_l, "HELLO"));

    assert(uusnncpy(dst_u, 32, make_ustr_local("foo", src_u, 33), 32, NULL) != NULL);
    assert(eq_ustr_ascii_local(dst_u, "foo"));
    assert(uusnncpyc(dst_u, 32, make_ustr_local("BAR", src_u, 33), 32, NULL) != NULL);
    assert(eq_ustr_ascii_local(dst_u, "bar"));
    assert(uusnnCPYC(dst_u, 32, make_ustr_local("baz", src_u, 33), 32, NULL) != NULL);
    assert(eq_ustr_ascii_local(dst_u, "BAZ"));

    assert(lusnncpy(dst_l, 32, make_ustr_local("abc", src_u, 33), 32, NULL, '?') != NULL);
    assert(eq_lstr_ascii_local(dst_l, "abc"));
    assert(lusnncpyc(dst_l, 32, make_ustr_local("DEF", src_u, 33), 32, NULL, '?') != NULL);
    assert(eq_lstr_ascii_local(dst_l, "def"));
    assert(lusnnCPYC(dst_l, 32, make_ustr_local("ghi", src_u, 33), 32, NULL, '?') != NULL);
    assert(eq_lstr_ascii_local(dst_l, "GHI"));

    assert(ulsnncpy(dst_u, 32, make_lstr_local("123", src_l, 33), 32, NULL) != NULL);
    assert(eq_ustr_ascii_local(dst_u, "123"));
    assert(ulsnncpyc(dst_u, 32, make_lstr_local("ABC", src_l, 33), 32, NULL) != NULL);
    assert(eq_ustr_ascii_local(dst_u, "abc"));
}

static void test_cat_basic(void) {
    lchar_t dst[33], src[33];
    uchar_t udst[33], usrc[33];

    assert(llsnncpy(dst, 32, make_lstr_local("hello", src, 33), 32, NULL) != NULL);
    assert(llsnncat(dst, 32, make_lstr_local(" world", src, 33), 32, NULL) != NULL);
    assert(eq_lstr_ascii_local(dst, "hello world"));

    assert(llsnncpy(dst, 32, make_lstr_local("foo", src, 33), 32, NULL) != NULL);
    assert(llsnncatc(dst, 32, make_lstr_local("BAR", src, 33), 32, NULL) != NULL);
    assert(eq_lstr_ascii_local(dst, "foobar"));

    assert(llsnncpy(dst, 32, make_lstr_local("foo", src, 33), 32, NULL) != NULL);
    assert(llsnnCATC(dst, 32, make_lstr_local("baz", src, 33), 32, NULL) != NULL);
    assert(eq_lstr_ascii_local(dst, "fooBAZ"));

    assert(uusnncpy(udst, 32, make_ustr_local("abc", usrc, 33), 32, NULL) != NULL);
    assert(uusnncat(udst, 32, make_ustr_local("def", usrc, 33), 32, NULL) != NULL);
    assert(eq_ustr_ascii_local(udst, "abcdef"));

    assert(uusnncpy(udst, 32, make_ustr_local("foo", usrc, 33), 32, NULL) != NULL);
    assert(uusnncatc(udst, 32, make_ustr_local("BAR", usrc, 33), 32, NULL) != NULL);
    assert(eq_ustr_ascii_local(udst, "foobar"));

    assert(uusnncpy(udst, 32, make_ustr_local("foo", usrc, 33), 32, NULL) != NULL);
    assert(uusnnCATC(udst, 32, make_ustr_local("baz", usrc, 33), 32, NULL) != NULL);
    assert(eq_ustr_ascii_local(udst, "fooBAZ"));

    assert(llsnncpy(dst, 32, make_lstr_local("abc", src, 33), 32, NULL) != NULL);
    assert(lusnncat(dst, 32, make_ustr_local("def", usrc, 33), 32, NULL, '?') != NULL);
    assert(eq_lstr_ascii_local(dst, "abcdef"));

    assert(llsnncpy(dst, 32, make_lstr_local("foo", src, 33), 32, NULL) != NULL);
    assert(lusnncatc(dst, 32, make_ustr_local("BAR", usrc, 33), 32, NULL, '?') != NULL);
    assert(eq_lstr_ascii_local(dst, "foobar"));

    assert(llsnncpy(dst, 32, make_lstr_local("foo", src, 33), 32, NULL) != NULL);
    assert(lusnnCATC(dst, 32, make_ustr_local("baz", usrc, 33), 32, NULL, '?') != NULL);
    assert(eq_lstr_ascii_local(dst, "fooBAZ"));

    assert(uusnncpy(udst, 32, make_ustr_local("abc", usrc, 33), 32, NULL) != NULL);
    assert(ulsnncat(udst, 32, make_lstr_local("def", src, 33), 32, NULL) != NULL);
    assert(eq_ustr_ascii_local(udst, "abcdef"));

    assert(uusnncpy(udst, 32, make_ustr_local("foo", usrc, 33), 32, NULL) != NULL);
    assert(ulsnncatc(udst, 32, make_lstr_local("BAR", src, 33), 32, NULL) != NULL);
    assert(eq_ustr_ascii_local(udst, "foobar"));

    assert(uusnncpy(udst, 32, make_ustr_local("foo", usrc, 33), 32, NULL) != NULL);
    assert(ulsnnCATC(udst, 32, make_lstr_local("baz", src, 33), 32, NULL) != NULL);
    assert(eq_ustr_ascii_local(udst, "fooBAZ"));
}

static void test_copy_and_cat_error_behavior(void) {
    lchar_t dst[33], src[33];
    uchar_t udst[33];
    size_t i;

    dst[0] = 'X';
    dst[1] = 0;
    assert(llsnncpy(NULL, 32, make_lstr_local("abc", src, 33), 32, NULL) == NULL);
    assert(dst[0] == 'X');

    assert(llsnncpy(dst, 32, NULL, 32, NULL) != NULL);
    assert(dst[0] == 0);

    make_lstr_local("abcdef", dst, 33);
    assert(lusnncat(dst, 32, (uchar_t *)dst, 6, NULL, '?') == NULL);
    assert(dst[0] == 0);

    for (i = 0; i < 6; ++i) udst[i] = (uchar_t)('a' + (int)i);
    udst[6] = 0;
    assert(ulsnncat(udst, 32, (lchar_t *)udst, 6, NULL) == NULL);
    assert(udst[0] == 0);
}

void run_core_family_tests(void) {
    test_copy_variants();
    test_cat_basic();
    test_copy_and_cat_error_behavior();
}
