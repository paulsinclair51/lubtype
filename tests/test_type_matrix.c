/**
 * @file test_type_matrix.c
 * @brief Tests for type matrix and type conversion logic in lubtype.h.
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
 * @brief Test cross-type conversions between lstr_t and ustr_t.
 */
static void test_cross_type_conv(void) {
    lchar_t dst_l[32], src_l[32];
    uchar_t dst_u[32], src_u[32];
    // ulsnapp, ulsnappc, ulsnAPPC
    assert(ulsnapp(dst_u, make_lstr_local("abc", src_l, 32), 32) == dst_u);
    assert(eq_ustr_ascii_local(dst_u, "abc"));
    assert(ulsnappc(dst_u, make_lstr_local("DEF", src_l, 32), 32) == dst_u);
    assert(eq_ustr_ascii_local(dst_u, "abcdef"));
    assert(ulsnAPPC(dst_u, make_lstr_local("ghi", src_l, 32), 32) == dst_u);
    assert(eq_ustr_ascii_local(dst_u, "abcdefGHI"));
    // lusnapp, lusnappc, lusnAPPC
    assert(lusnapp(dst_l, make_ustr_local("foo", src_u, 32), 32, '?') == dst_l);
    assert(eq_lstr_ascii_local(dst_l, "foo"));
    assert(lusnappc(dst_l, make_ustr_local("BAR", src_u, 32), 32, '?') == dst_l);
    assert(eq_lstr_ascii_local(dst_l, "foobar"));
    assert(lusnAPPC(dst_l, make_ustr_local("baz", src_u, 32), 32, '?') == dst_l);
    assert(eq_lstr_ascii_local(dst_l, "foobarbaz"));
    // Null-termination on error
    dst_l[0] = 'X'; dst_l[1] = 0;
    assert(lusnapp(NULL, NULL, 0, '?') == NULL);
    assert(dst_l[0] == 'X');
    assert(lusnapp(dst_l, NULL, 0, '?') == dst_l);
    assert(dst_l[0] == 0);
    // Cross-type overlap error
    strcpy((char*)dst_l, "abcdef");
    assert(lusnapp(dst_l, (ucstr_t)dst_l, 6, '?') == NULL);
    assert(dst_l[0] == 0);
    for (int i = 0; i < 6; ++i) dst_u[i] = (uchar_t)('a'+i); dst_u[6]=0;
    assert(ulsnapp(dst_u, (lcstr_t)dst_u, 6) == NULL);
    assert(dst_u[0] == 0);
}

/**
 * @brief Test type matrix core function families.
 */
static void test_type_matrix_core_families(void) {
    lchar_t latin1[32], latin2[32], latin3[32];
    uchar_t uni1[32], uni2[32], uni3[32];
    lcstr_t hit_l;
    ucstr_t hit_u;

    assert(llscpy(latin1, make_lstr_local("abc", latin2, 32)) == latin1);
    assert(eq_lstr_ascii_local(latin1, "abc"));

    assert(luscpy(latin1, make_ustr_local("abc", uni1, 32), '?') == latin1);
    assert(eq_lstr_ascii_local(latin1, "abc"));

    assert(ulscpy(uni2, make_lstr_local("abc", latin2, 32)) == uni2);
    assert(eq_ustr_ascii_local(uni2, "abc"));

    assert(uuscpy(uni3, make_ustr_local("abc", uni1, 32)) == uni3);
    assert(eq_ustr_ascii_local(uni3, "abc"));

    assert(llscpy(latin1, make_lstr_local("ab", latin2, 32)) == latin1);
    assert(llscat(latin1, make_lstr_local("cd", latin3, 32)) == latin1);
    assert(eq_lstr_ascii_local(latin1, "abcd"));

    assert(luscpy(latin1, make_ustr_local("ab", uni1, 32), '?') == latin1);
    assert(luscat(latin1, make_ustr_local("cd", uni2, 32), '?') == latin1);
    assert(eq_lstr_ascii_local(latin1, "abcd"));

    assert(ulscpy(uni2, make_lstr_local("ab", latin2, 32)) == uni2);
    assert(ulscat(uni2, make_lstr_local("cd", latin3, 32)) == uni2);
    assert(eq_ustr_ascii_local(uni2, "abcd"));

    assert(uuscpy(uni3, make_ustr_local("ab", uni1, 32)) == uni3);
    assert(uuscat(uni3, make_ustr_local("cd", uni2, 32)) == uni3);
    assert(eq_ustr_ascii_local(uni3, "abcd"));

    assert(llscmp(make_lstr_local("Match", latin1, 32), make_lstr_local("Match", latin2, 32)) == 0);
    assert(luscmp(make_lstr_local("Match", latin1, 32), make_ustr_local("Match", uni1, 32)) == 0);
    assert(ulscmp(make_ustr_local("Match", uni2, 32), make_lstr_local("Match", latin2, 32)) == 0);
    assert(uuscmp(make_ustr_local("Match", uni2, 32), make_ustr_local("Match", uni3, 32)) == 0);

    assert(llsCMP(make_lstr_local("MiXeD", latin1, 32), make_lstr_local("mixed", latin2, 32)) == 0);
    assert(lusCMP(make_lstr_local("MiXeD", latin1, 32), make_ustr_local("mixed", uni1, 32)) == 0);
    assert(ulsCMP(make_ustr_local("MiXeD", uni2, 32), make_lstr_local("mixed", latin2, 32)) == 0);
    assert(uusCMP(make_ustr_local("MiXeD", uni2, 32), make_ustr_local("mixed", uni3, 32)) == 0);

    hit_l = llschr(make_lstr_local("abc", latin1, 32), 'b');
    assert(hit_l != NULL && (size_t)(hit_l - latin1) == 1);

    hit_l = luschr(make_lstr_local("abc", latin1, 32), (uchar_t)'b');
    assert(hit_l != NULL && (size_t)(hit_l - latin1) == 1);

    hit_u = ulschr(make_ustr_local("abc", uni1, 32), 'b');
    assert(hit_u != NULL && (size_t)(hit_u - uni1) == 1);

    hit_u = uuschr(make_ustr_local("abc", uni1, 32), (uchar_t)'b');
    assert(hit_u != NULL && (size_t)(hit_u - uni1) == 1);

    hit_l = llsstr(make_lstr_local("xxabyy", latin1, 32), make_lstr_local("ab", latin2, 32));
    assert(hit_l != NULL && (size_t)(hit_l - latin1) == 2);

    hit_l = lusstr(make_lstr_local("xxabyy", latin1, 32), make_ustr_local("ab", uni1, 32));
    assert(hit_l != NULL && (size_t)(hit_l - latin1) == 2);

    hit_u = ulsstr(make_ustr_local("xxabyy", uni2, 32), make_lstr_local("ab", latin2, 32));
    assert(hit_u != NULL && (size_t)(hit_u - uni2) == 2);

    hit_u = uusstr(make_ustr_local("xxabyy", uni2, 32), make_ustr_local("ab", uni3, 32));
    assert(hit_u != NULL && (size_t)(hit_u - uni2) == 2);
}

static void test_wrapper_equivalence_core_families(void) {
    lchar_t dst_a[32], dst_b[32], src_l[32], src2_l[32], hay_l[32], needle_l[16];
    uchar_t dst_u1[32], dst_u2[32], src_u[32], src2_u[32], hay_u[32], needle_u[16];
    lstr_t end_a, end_b;
    lcstr_t hit_l1, hit_l2;
    ucstr_t hit_u1, hit_u2;

    dst_a[0] = (lchar_t)0;
    dst_b[0] = (lchar_t)0;
    end_a = llsapp(dst_a, make_lstr_local("abc", src_l, 32));
    end_b = llsnapp(dst_b, make_lstr_local("abc", src_l, 32), MAX_LSTRLEN);
    assert(end_a != NULL && end_b != NULL);
    assert(eq_lstr_ascii_local(dst_a, "abc") && eq_lstr_ascii_local(dst_b, "abc"));
    assert((size_t)(end_a - dst_a) == (size_t)(end_b - dst_b));

    assert(llscpy(dst_a, make_lstr_local("abc", src_l, 32)) == dst_a);
    assert(llsncpy(dst_b, make_lstr_local("abc", src_l, 32), MAX_LSTRLEN) == dst_b);
    assert(eq_lstr_ascii_local(dst_a, "abc") && eq_lstr_ascii_local(dst_b, "abc"));

    assert(llscat(dst_a, make_lstr_local("xy", src2_l, 32)) == dst_a);
    assert(llsncat(dst_b, make_lstr_local("xy", src2_l, 32), MAX_LSTRLEN) == dst_b);
    assert(eq_lstr_ascii_local(dst_a, "abcxy") && eq_lstr_ascii_local(dst_b, "abcxy"));

    assert(ulscpy(dst_u1, make_lstr_local("abc", src_l, 32)) == dst_u1);
    assert(ulsncpy(dst_u2, make_lstr_local("abc", src_l, 32), MAX_USTRLEN) == dst_u2);
    assert(eq_ustr_ascii_local(dst_u1, "abc") && eq_ustr_ascii_local(dst_u2, "abc"));

    assert(llscmp(make_lstr_local("same", src_l, 32), make_lstr_local("same", src2_l, 32)) ==
           llsncmp(make_lstr_local("same", src_l, 32), make_lstr_local("same", src2_l, 32), MAX_LSTRLEN));

    assert(uuscmp(make_ustr_local("same", src_u, 32), make_ustr_local("same", src2_u, 32)) ==
           uusncmp(make_ustr_local("same", src_u, 32), make_ustr_local("same", src2_u, 32), MAX_USTRLEN));

    hit_l1 = llschr(make_lstr_local("abc", hay_l, 32), 'b');
    hit_l2 = llsnchr(make_lstr_local("abc", hay_l, 32), 'b', MAX_LSTRLEN);
    assert(hit_l1 != NULL && hit_l2 != NULL);
    assert((size_t)(hit_l1 - hay_l) == (size_t)(hit_l2 - hay_l));

    hit_u1 = uuschr(make_ustr_local("abc", hay_u, 32), (uchar_t)'b');
    hit_u2 = uusnchr(make_ustr_local("abc", hay_u, 32), (uchar_t)'b', MAX_USTRLEN);
    assert(hit_u1 != NULL && hit_u2 != NULL);
    assert((size_t)(hit_u1 - hay_u) == (size_t)(hit_u2 - hay_u));

    hit_l1 = llsstr(make_lstr_local("xxabyy", hay_l, 32), make_lstr_local("ab", needle_l, 16));
    hit_l2 = llsnstr(make_lstr_local("xxabyy", hay_l, 32), make_lstr_local("ab", needle_l, 16), MAX_LSTRLEN);
    assert(hit_l1 != NULL && hit_l2 != NULL);
    assert((size_t)(hit_l1 - hay_l) == (size_t)(hit_l2 - hay_l));

    hit_u1 = uusstr(make_ustr_local("xxabyy", hay_u, 32), make_ustr_local("ab", needle_u, 16));
    hit_u2 = uusnstr(make_ustr_local("xxabyy", hay_u, 32), make_ustr_local("ab", needle_u, 16), MAX_USTRLEN);
    assert(hit_u1 != NULL && hit_u2 != NULL);
    assert((size_t)(hit_u1 - hay_u) == (size_t)(hit_u2 - hay_u));
}

static void test_pbrk_occurrence_matrix(void) {
    lchar_t hay_l[32], set_l[16];
    uchar_t hay_u[32], set_u[16];
    lcstr_t hit_l;
    ucstr_t hit_u;

    hit_l = llsnpbrkm(make_lstr_local("a1b2c3", hay_l, 32), make_lstr_local("123", set_l, 16), 32, 1);
    assert(hit_l != NULL && (size_t)(hit_l - hay_l) == 1 && *hit_l == '1');

    hit_l = llsnpbrkm(make_lstr_local("a1b2c3", hay_l, 32), make_lstr_local("123", set_l, 16), 32, 2);
    assert(hit_l != NULL && (size_t)(hit_l - hay_l) == 3 && *hit_l == '2');

    hit_l = llsnpbrkm(make_lstr_local("a1b2c3", hay_l, 32), make_lstr_local("123", set_l, 16), 32, 0);
    assert(hit_l == NULL);

    hit_l = llsnpbrkm(make_lstr_local("abc", hay_l, 32), make_lstr_local("123", set_l, 16), 32, 1);
    assert(hit_l == NULL);

    hit_l = llsnPBRKM(make_lstr_local("aAbBcC", hay_l, 32), make_lstr_local("c", set_l, 16), 32, 1);
    assert(hit_l != NULL && (size_t)(hit_l - hay_l) == 4);

    hit_l = llsnPBRKM(make_lstr_local("aAbBcC", hay_l, 32), make_lstr_local("c", set_l, 16), 32, 2);
    assert(hit_l != NULL && (size_t)(hit_l - hay_l) == 5);

    hit_l = llsnrpbrkm(make_lstr_local("a1b2c3", hay_l, 32), make_lstr_local("123", set_l, 16), 32, 1);
    assert(hit_l != NULL && (size_t)(hit_l - hay_l) == 5 && *hit_l == '3');

    hit_l = llsnrpbrkm(make_lstr_local("a1b2c3", hay_l, 32), make_lstr_local("123", set_l, 16), 32, 2);
    assert(hit_l != NULL && (size_t)(hit_l - hay_l) == 3 && *hit_l == '2');

    hit_l = llsnRPBRKM(make_lstr_local("aAbBcC", hay_l, 32), make_lstr_local("a", set_l, 16), 32, 1);
    assert(hit_l != NULL && (size_t)(hit_l - hay_l) == 1);

    hit_l = llsnRPBRKM(make_lstr_local("aAbBcC", hay_l, 32), make_lstr_local("a", set_l, 16), 32, 2);
    assert(hit_l != NULL && (size_t)(hit_l - hay_l) == 0);

    hit_l = lusnpbrkm(make_lstr_local("a1b2", hay_l, 32), make_ustr_local("12", set_u, 16), 32, 2);
    assert(hit_l != NULL && (size_t)(hit_l - hay_l) == 3);

    hit_u = ulsnpbrkm(make_ustr_local("a1b2", hay_u, 32), make_lstr_local("12", set_l, 16), 32, 2);
    assert(hit_u != NULL && (size_t)(hit_u - hay_u) == 3);

    hit_u = uusnRPBRKM(make_ustr_local("aAbB", hay_u, 32), make_ustr_local("b", set_u, 16), 32, 2);
    assert(hit_u != NULL && (size_t)(hit_u - hay_u) == 2);
}

static void test_wrapper_equivalence_set_and_count_families(void) {
    lchar_t s1_l[32], s2_l[32];
    uchar_t s1_u[32], s2_u[32];

    assert(llspfx(make_lstr_local("prefix", s1_l, 32), make_lstr_local("pre", s2_l, 32)) ==
        llsnpfx(make_lstr_local("prefix", s1_l, 32), make_lstr_local("pre", s2_l, 32), MAX_LSTRLEN));
    assert(uuspfx(make_ustr_local("prefix", s1_u, 32), make_ustr_local("pre", s2_u, 32)) ==
        uusnpfx(make_ustr_local("prefix", s1_u, 32), make_ustr_local("pre", s2_u, 32), MAX_USTRLEN));

    assert(llsPFX(make_lstr_local("PrEfIx", s1_l, 32), make_lstr_local("pre", s2_l, 32)) ==
        llsnPFX(make_lstr_local("PrEfIx", s1_l, 32), make_lstr_local("pre", s2_l, 32), MAX_LSTRLEN));
    assert(uusPFX(make_ustr_local("PrEfIx", s1_u, 32), make_ustr_local("pre", s2_u, 32)) ==
        uusnPFX(make_ustr_local("PrEfIx", s1_u, 32), make_ustr_local("pre", s2_u, 32), MAX_USTRLEN));

    assert(llssfx(make_lstr_local("suffix", s1_l, 32), make_lstr_local("fix", s2_l, 32)) ==
        llsnsfx(make_lstr_local("suffix", s1_l, 32), make_lstr_local("fix", s2_l, 32), MAX_LSTRLEN));
    assert(uussfx(make_ustr_local("suffix", s1_u, 32), make_ustr_local("fix", s2_u, 32)) ==
        uusnsfx(make_ustr_local("suffix", s1_u, 32), make_ustr_local("fix", s2_u, 32), MAX_USTRLEN));

    assert(llsSFX(make_lstr_local("SuFfIx", s1_l, 32), make_lstr_local("fix", s2_l, 32)) ==
        llsnSFX(make_lstr_local("SuFfIx", s1_l, 32), make_lstr_local("fix", s2_l, 32), MAX_LSTRLEN));
    assert(uusSFX(make_ustr_local("SuFfIx", s1_u, 32), make_ustr_local("fix", s2_u, 32)) ==
        uusnSFX(make_ustr_local("SuFfIx", s1_u, 32), make_ustr_local("fix", s2_u, 32), MAX_USTRLEN));

    assert(llsspn(make_lstr_local("aaab", s1_l, 32), make_lstr_local("a", s2_l, 32)) ==
        llsnspn(make_lstr_local("aaab", s1_l, 32), make_lstr_local("a", s2_l, 32), MAX_LSTRLEN));
    assert(uusspn(make_ustr_local("aaab", s1_u, 32), make_ustr_local("a", s2_u, 32)) ==
        uusnspn(make_ustr_local("aaab", s1_u, 32), make_ustr_local("a", s2_u, 32), MAX_USTRLEN));

    assert(llsSPN(make_lstr_local("AaAb", s1_l, 32), make_lstr_local("a", s2_l, 32)) ==
        llsnSPN(make_lstr_local("AaAb", s1_l, 32), make_lstr_local("a", s2_l, 32), MAX_LSTRLEN));
    assert(uusSPN(make_ustr_local("AaAb", s1_u, 32), make_ustr_local("a", s2_u, 32)) ==
        uusnSPN(make_ustr_local("AaAb", s1_u, 32), make_ustr_local("a", s2_u, 32), MAX_USTRLEN));

    assert(llscspn(make_lstr_local("xyz1", s1_l, 32), make_lstr_local("123", s2_l, 32)) ==
        llsncspn(make_lstr_local("xyz1", s1_l, 32), make_lstr_local("123", s2_l, 32), MAX_LSTRLEN));
    assert(uuscspn(make_ustr_local("xyz1", s1_u, 32), make_ustr_local("123", s2_u, 32)) ==
        uusncspn(make_ustr_local("xyz1", s1_u, 32), make_ustr_local("123", s2_u, 32), MAX_USTRLEN));

    assert(llsCSPN(make_lstr_local("xYzA", s1_l, 32), make_lstr_local("a", s2_l, 32)) ==
        llsnCSPN(make_lstr_local("xYzA", s1_l, 32), make_lstr_local("a", s2_l, 32), MAX_LSTRLEN));
    assert(uusCSPN(make_ustr_local("xYzA", s1_u, 32), make_ustr_local("a", s2_u, 32)) ==
        uusnCSPN(make_ustr_local("xYzA", s1_u, 32), make_ustr_local("a", s2_u, 32), MAX_USTRLEN));

    assert(llscnt(make_lstr_local("hello", s1_l, 32), 'l') ==
        llsncnt(make_lstr_local("hello", s1_l, 32), 'l', MAX_LSTRLEN));
    assert(uuscnt(make_ustr_local("hello", s1_u, 32), (uchar_t)'l') ==
        uusncnt(make_ustr_local("hello", s1_u, 32), (uchar_t)'l', MAX_USTRLEN));

    assert(llsCNT(make_lstr_local("HeLLo", s1_l, 32), 'l') ==
        llsnCNT(make_lstr_local("HeLLo", s1_l, 32), 'l', MAX_LSTRLEN));
    assert(uusCNT(make_ustr_local("HeLLo", s1_u, 32), (uchar_t)'l') ==
        uusnCNT(make_ustr_local("HeLLo", s1_u, 32), (uchar_t)'l', MAX_USTRLEN));

    assert(llssubcnt(make_lstr_local("ababab", s1_l, 32), make_lstr_local("ab", s2_l, 32)) ==
        llsnsubcnt(make_lstr_local("ababab", s1_l, 32), make_lstr_local("ab", s2_l, 32), MAX_LSTRLEN));
    assert(uussubcnt(make_ustr_local("ababab", s1_u, 32), make_ustr_local("ab", s2_u, 32)) ==
        uusnsubcnt(make_ustr_local("ababab", s1_u, 32), make_ustr_local("ab", s2_u, 32), MAX_USTRLEN));

    assert(llsSUBCNT(make_lstr_local("AbAbAb", s1_l, 32), make_lstr_local("ab", s2_l, 32)) ==
        llsnSUBCNT(make_lstr_local("AbAbAb", s1_l, 32), make_lstr_local("ab", s2_l, 32), MAX_LSTRLEN));
    assert(uusSUBCNT(make_ustr_local("AbAbAb", s1_u, 32), make_ustr_local("ab", s2_u, 32)) ==
        uusnSUBCNT(make_ustr_local("AbAbAb", s1_u, 32), make_ustr_local("ab", s2_u, 32), MAX_USTRLEN));
}

void run_type_matrix_tests(void) {
    test_cross_type_conv();
    test_type_matrix_core_families();
    test_wrapper_equivalence_core_families();
    test_pbrk_occurrence_matrix();
    test_wrapper_equivalence_set_and_count_families();
}

