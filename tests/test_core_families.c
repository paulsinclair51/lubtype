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
 * @brief Create a local lstr_t from ASCII string.
 */
static lcstr_t make_lstr_local(const char *src, lstr_t dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (lcstr_t)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (lchar_t)(unsigned char)src[i];
    dst[i] = (lchar_t)0;
    return (lcstr_t)dst;
}

/**
 * @brief Create a local ustr_t from ASCII string.
 */
static ucstr_t make_ustr_local(const char *src, ustr_t dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (ucstr_t)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (uchar_t)(unsigned char)src[i];
    dst[i] = (uchar_t)0;
    return (ucstr_t)dst;
}

/**
 * @brief Compare lstr_t to ASCII string for equality.
 */
static int eq_lstr_ascii_local(lcstr_t s, const char *ascii) {
    size_t i = 0;
    if (!s || !ascii) return 0;
    for (; ascii[i]; ++i)
        if (s[i] != (lchar_t)(unsigned char)ascii[i]) return 0;
    return s[i] == (lchar_t)0;
}

/**
 * @brief Compare ustr_t to ASCII string for equality.
 */
static int eq_ustr_ascii_local(ucstr_t s, const char *ascii) {
    size_t i = 0;
    if (!s || !ascii) return 0;
    for (; ascii[i]; ++i)
        if (s[i] != (uchar_t)(unsigned char)ascii[i]) return 0;
    return s[i] == (uchar_t)0;
}

/**
 * @brief Test basic append operations for core families.
 */
static void test_append_basic(void) {
    lchar_t dst[64], src[64];
    uchar_t udst[64], usrc[64];
    lstr_t end;
    ustr_t uend;
    dst[0] = (lchar_t)0;
    udst[0] = 0;
    // llsnapp, llsnappc, llsnAPPC
    end = llsnapp(dst, make_lstr_local("hello", src, 64), 64);
    assert(end != NULL && eq_lstr_ascii_local(dst, "hello"));
    end = llsnappc(dst, make_lstr_local(" world", src, 64), 64);
    assert(end != NULL && eq_lstr_ascii_local(dst, "hello world"));
    end = llsnAPPC(dst, make_lstr_local("!", src, 64), 64);
    assert(end != NULL && eq_lstr_ascii_local(dst, "hello world!"));
    // uusnapp, uusnappc, uusnAPPC
    uend = uusnapp(udst, make_ustr_local("foo", usrc, 64), 64);
    assert(uend != NULL && eq_ustr_ascii_local(udst, "foo"));
    uend = uusnappc(udst, make_ustr_local("BAR", usrc, 64), 64);
    assert(uend != NULL && eq_ustr_ascii_local(udst, "foobar"));
    uend = uusnAPPC(udst, make_ustr_local("baz", usrc, 64), 64);
    assert(uend != NULL && eq_ustr_ascii_local(udst, "foobarbaz"));
    // Cross-type: lusnapp, lusnappc, lusnAPPC
    end = lusnapp(dst, make_ustr_local("abc", usrc, 64), 64, '?');
    assert(end != NULL && eq_lstr_ascii_local(dst, "abc"));
    end = lusnappc(dst, make_ustr_local("DEF", usrc, 64), 64, '?');
    assert(end != NULL && eq_lstr_ascii_local(dst, "abcdef"));
    end = lusnAPPC(dst, make_ustr_local("ghi", usrc, 64), 64, '?');
    assert(end != NULL && eq_lstr_ascii_local(dst, "abcdefghi"));
    // Cross-type: ulsnapp, ulsnappc, ulsnAPPC
    uend = ulsnapp(udst, make_lstr_local("123", src, 64), 64);
    assert(uend != NULL && eq_ustr_ascii_local(udst, "foobarbaz123"));
    uend = ulsnappc(udst, make_lstr_local("ABC", src, 64), 64);
    assert(uend != NULL && eq_ustr_ascii_local(udst, "foobarbaz123abc"));
    uend = ulsnAPPC(udst, make_lstr_local("xyz", src, 64), 64);
    assert(uend != NULL && eq_ustr_ascii_local(udst, "foobarbaz123abcxyz"));
    // Null-termination on error
    dst[0] = 'X'; dst[1] = 0;
    assert(llsnapp(NULL, NULL, 0) == NULL);
    assert(dst[0] == 'X');
    assert(llsnapp(dst, NULL, 0) == dst);
    assert(dst[0] == 0);
    // Cross-type overlap error
    strcpy((char*)dst, "abcdef");
    assert(lusnapp(dst, (ucstr_t)dst, 6, '?') == NULL);
    assert(dst[0] == 0);
    for (int i = 0; i < 6; ++i) udst[i] = (uchar_t)('a'+i); udst[6]=0;
    assert(ulsnapp(udst, (lcstr_t)udst, 6) == NULL);
    assert(udst[0] == 0);
}

static void test_copy_variants(void) {
    lchar_t dst_l[32], src_l[32];
    uchar_t dst_u[32], src_u[32];
    // llsncpy, llsncpyc, llsnCPYC
    assert(llsncpy(dst_l, make_lstr_local("hello", src_l, 32), 32) == dst_l);
    assert(eq_lstr_ascii_local(dst_l, "hello"));
    assert(llsncpyc(dst_l, make_lstr_local("HELLO", src_l, 32), 32) == dst_l);
    assert(eq_lstr_ascii_local(dst_l, "hello"));
    assert(llsnCPYC(dst_l, make_lstr_local("hello", src_l, 32), 32) == dst_l);
    assert(eq_lstr_ascii_local(dst_l, "HELLO"));
    // uusncpy, uusncpyc, uusnCPYC
    assert(uusncpy(dst_u, make_ustr_local("foo", src_u, 32), 32) == dst_u);
    assert(eq_ustr_ascii_local(dst_u, "foo"));
    assert(uusncpyc(dst_u, make_ustr_local("BAR", src_u, 32), 32) == dst_u);
    assert(eq_ustr_ascii_local(dst_u, "bar"));
    assert(uusnCPYC(dst_u, make_ustr_local("baz", src_u, 32), 32) == dst_u);
    assert(eq_ustr_ascii_local(dst_u, "BAZ"));
    // Cross-type: lusncpy, lusncpyc, lusnCPYC
    assert(lusncpy(dst_l, make_ustr_local("abc", src_u, 32), 32, '?') == dst_l);
    assert(eq_lstr_ascii_local(dst_l, "abc"));
    assert(lusncpyc(dst_l, make_ustr_local("DEF", src_u, 32), 32, '?') == dst_l);
    assert(eq_lstr_ascii_local(dst_l, "def"));
    assert(lusnCPYC(dst_l, make_ustr_local("ghi", src_u, 32), 32, '?') == dst_l);
    assert(eq_lstr_ascii_local(dst_l, "GHI"));
    // Cross-type: ulsncpy, ulsncpyc, ulsnCPYC
    assert(ulsncpy(dst_u, make_lstr_local("123", src_l, 32), 32) == dst_u);
    assert(eq_ustr_ascii_local(dst_u, "123"));
    assert(ulsncpyc(dst_u, make_lstr_local("ABC", src_l, 32), 32) == dst_u);
    assert(eq_ustr_ascii_local(dst_u, "abc"));
    assert(ulsnCPYC(dst_u, make_lstr_local("xyz", src_l, 32), 32) == dst_u);
    assert(eq_ustr_ascii_local(dst_u, "XYZ"));
}

static void test_cat_basic(void) {
    lchar_t dst[32], src[32];
    uchar_t udst[32], usrc[32];
    // llsncat, llsncatc, llsnCATC
    llsncpy(dst, make_lstr_local("hello", src, 32), 32);
    assert(llsncat(dst, make_lstr_local(" world", src, 32), 32) == dst);
    assert(eq_lstr_ascii_local(dst, "hello world"));
    llsncpy(dst, make_lstr_local("foo", src, 32), 32);
    assert(llsncatc(dst, make_lstr_local("BAR", src, 32), 32) == dst);
    assert(eq_lstr_ascii_local(dst, "foobar"));
    llsncpy(dst, make_lstr_local("foo", src, 32), 32);
    assert(llsnCATC(dst, make_lstr_local("baz", src, 32), 32) == dst);
    assert(eq_lstr_ascii_local(dst, "fooBAZ"));
    // uusncat, uusncatc, uusnCATC
    uusncpy(udst, make_ustr_local("abc", usrc, 32), 32);
    assert(uusncat(udst, make_ustr_local("def", usrc, 32), 32) == udst);
    assert(eq_ustr_ascii_local(udst, "abcdef"));
    uusncpy(udst, make_ustr_local("foo", usrc, 32), 32);
    assert(uusncatc(udst, make_ustr_local("BAR", usrc, 32), 32) == udst);
    assert(eq_ustr_ascii_local(udst, "foobar"));
    uusncpy(udst, make_ustr_local("foo", usrc, 32), 32);
    assert(uusnCATC(udst, make_ustr_local("baz", usrc, 32), 32) == udst);
    assert(eq_ustr_ascii_local(udst, "fooBAZ"));
    // Cross-type: lusncat, lusncatc, lusnCATC
    llsncpy(dst, make_lstr_local("abc", src, 32), 32);
    assert(lusncat(dst, make_ustr_local("def", usrc, 32), 32, '?') == dst);
    assert(eq_lstr_ascii_local(dst, "abcdef"));
    llsncpy(dst, make_lstr_local("foo", src, 32), 32);
    assert(lusncatc(dst, make_ustr_local("BAR", usrc, 32), 32, '?') == dst);
    assert(eq_lstr_ascii_local(dst, "foobar"));
    llsncpy(dst, make_lstr_local("foo", src, 32), 32);
    assert(lusnCATC(dst, make_ustr_local("baz", usrc, 32), 32, '?') == dst);
    assert(eq_lstr_ascii_local(dst, "fooBAZ"));
    // Cross-type: ulsncat, ulsncatc, ulsnCATC
    uusncpy(udst, make_ustr_local("abc", usrc, 32), 32);
    assert(ulsncat(udst, make_lstr_local("def", src, 32), 32) == udst);
    assert(eq_ustr_ascii_local(udst, "abcdef"));
    uusncpy(udst, make_ustr_local("foo", usrc, 32), 32);
    assert(ulsncatc(udst, make_lstr_local("BAR", src, 32), 32) == udst);
    assert(eq_ustr_ascii_local(udst, "foobar"));
    uusncpy(udst, make_ustr_local("foo", usrc, 32), 32);
    assert(ulsnCATC(udst, make_lstr_local("baz", src, 32), 32) == udst);
    assert(eq_ustr_ascii_local(udst, "fooBAZ"));
}

void run_core_family_tests(void) {
    test_append_basic();
    test_copy_variants();
    test_cat_basic();
}
