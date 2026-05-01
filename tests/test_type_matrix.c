/**
 * @file test_type_matrix.c
 * @brief Tests for cross-type bounded (n/nn) APIs in lubtype.h.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include <assert.h>
#include <stddef.h>

#include "../lubtype.h"

static const lchar_t *make_lstr_local(const char *src, lchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (const lchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (lchar_t)(unsigned char)src[i];
    dst[i] = (lchar_t)0;
    return (const lchar_t *)dst;
}

static const uchar_t *make_ustr_local(const char *src, uchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (const uchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (uchar_t)(unsigned char)src[i];
    dst[i] = (uchar_t)0;
    return (const uchar_t *)dst;
}

static int eq_lstr_ascii_local(const lchar_t *s, const char *ascii) {
    size_t i = 0;
    if (!s || !ascii) return 0;
    for (; ascii[i]; ++i) {
        if (s[i] != (lchar_t)(unsigned char)ascii[i]) return 0;
    }
    return s[i] == (lchar_t)0;
}

static int eq_ustr_ascii_local(const uchar_t *s, const char *ascii) {
    size_t i = 0;
    if (!s || !ascii) return 0;
    for (; ascii[i]; ++i) {
        if (s[i] != (uchar_t)(unsigned char)ascii[i]) return 0;
    }
    return s[i] == (uchar_t)0;
}

static void test_cross_type_copy_and_cat_matrix(void) {
    lchar_t ldst[33], lsrc[33];
    uchar_t udst[33], usrc[33];

    assert(llsnncpy(ldst, 32, (lchar_t *)make_lstr_local("ab", lsrc, 33), 32, NULL) != NULL);
    assert(eq_lstr_ascii_local(ldst, "ab"));
    assert(llsnncat(ldst, 32, (lchar_t *)make_lstr_local("cd", lsrc, 33), 32, NULL) != NULL);
    assert(eq_lstr_ascii_local(ldst, "abcd"));

    assert(uusnncpy(udst, 32, (uchar_t *)make_ustr_local("ab", usrc, 33), 32, NULL) != NULL);
    assert(eq_ustr_ascii_local(udst, "ab"));
    assert(uusnncat(udst, 32, (uchar_t *)make_ustr_local("cd", usrc, 33), 32, NULL) != NULL);
    assert(eq_ustr_ascii_local(udst, "abcd"));

    assert(lusnncpy(ldst, 32, (uchar_t *)make_ustr_local("xy", usrc, 33), 32, NULL, '?') != NULL);
    assert(eq_lstr_ascii_local(ldst, "xy"));
    assert(lusnncat(ldst, 32, (uchar_t *)make_ustr_local("zz", usrc, 33), 32, NULL, '?') != NULL);
    assert(eq_lstr_ascii_local(ldst, "xyzz"));

    assert(ulsnncpy(udst, 32, (lchar_t *)make_lstr_local("mn", lsrc, 33), 32, NULL) != NULL);
    assert(eq_ustr_ascii_local(udst, "mn"));
    assert(ulsnncat(udst, 32, (lchar_t *)make_lstr_local("op", lsrc, 33), 32, NULL) != NULL);
    assert(eq_ustr_ascii_local(udst, "mnop"));
}

static void test_compare_matrix(void) {
    lchar_t l1[32], l2[32];
    uchar_t u1[32], u2[32];

    assert(llsncmp(make_lstr_local("Match", l1, 32), make_lstr_local("Match", l2, 32), 32) == 0);
    assert(lusncmp(make_lstr_local("Match", l1, 32), (const uchar_t *)make_ustr_local("Match", u1, 32), 32) == 0);
    assert(ulsncmp((const uchar_t *)make_ustr_local("Match", u1, 32), make_lstr_local("Match", l2, 32), 32) == 0);
    assert(uusncmp((const uchar_t *)make_ustr_local("Match", u1, 32), (const uchar_t *)make_ustr_local("Match", u2, 32), 32) == 0);

    assert(llsnCMP(make_lstr_local("MiXeD", l1, 32), make_lstr_local("mixed", l2, 32), 32) == 0);
    assert(lusnCMP(make_lstr_local("MiXeD", l1, 32), (const uchar_t *)make_ustr_local("mixed", u1, 32), 32) == 0);
    assert(ulsnCMP((const uchar_t *)make_ustr_local("MiXeD", u1, 32), make_lstr_local("mixed", l2, 32), 32) == 0);
    assert(uusnCMP((const uchar_t *)make_ustr_local("MiXeD", u1, 32), (const uchar_t *)make_ustr_local("mixed", u2, 32), 32) == 0);
}

static void test_prefix_suffix_cmp_matrix(void) {
    lchar_t l1[32], l2[32];
    uchar_t u1[32], u2[32];

    assert(llsnpfxcmp(make_lstr_local("prefix", l1, 32), make_lstr_local("pre", l2, 32), 32) == 0);
    assert(lusnpfxcmp(make_lstr_local("prefix", l1, 32), (const uchar_t *)make_ustr_local("pre", u1, 32), 32) == 0);
    assert(ulsnpfxcmp((const uchar_t *)make_ustr_local("prefix", u1, 32), make_lstr_local("pre", l2, 32), 32) == 0);
    assert(uusnpfxcmp((const uchar_t *)make_ustr_local("prefix", u1, 32), (const uchar_t *)make_ustr_local("pre", u2, 32), 32) == 0);

    assert(llsnsfxcmp(make_lstr_local("suffix", l1, 32), make_lstr_local("fix", l2, 32), 32) == 0);
    assert(lusnsfxcmp(make_lstr_local("suffix", l1, 32), (const uchar_t *)make_ustr_local("fix", u1, 32), 32) == 0);
    assert(ulsnsfxcmp((const uchar_t *)make_ustr_local("suffix", u1, 32), make_lstr_local("fix", l2, 32), 32) == 0);
    assert(uusnsfxcmp((const uchar_t *)make_ustr_local("suffix", u1, 32), (const uchar_t *)make_ustr_local("fix", u2, 32), 32) == 0);
}

void run_type_matrix_tests(void) {
    test_cross_type_copy_and_cat_matrix();
    test_compare_matrix();
    test_prefix_suffix_cmp_matrix();
}
