#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "lubtype.h"

static lcstr_t make_lstr(const char *src, lstr_t dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (lcstr_t)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (lchar_t)(unsigned char)src[i];
    dst[i] = (lchar_t)0;
    return (lcstr_t)dst;
}

static ucstr_t make_ustr(const char *src, ustr_t dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (ucstr_t)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (uchar_t)(unsigned char)src[i];
    dst[i] = (uchar_t)0;
    return (ucstr_t)dst;
}

static int eq_lstr_ascii(lcstr_t s, const char *ascii) {
    size_t i = 0;
    if (!s || !ascii) return 0;
    for (; ascii[i]; ++i)
        if (s[i] != (lchar_t)(unsigned char)ascii[i]) return 0;
    return s[i] == (lchar_t)0;
}

static int eq_ustr_ascii(ucstr_t s, const char *ascii) {
    size_t i = 0;
    if (!s || !ascii) return 0;
    for (; ascii[i]; ++i)
        if (s[i] != (uchar_t)(unsigned char)ascii[i]) return 0;
    return s[i] == (uchar_t)0;
}

static void test_trim_collapse_edges(void) {
    lchar_t out[64];
    lchar_t src[64];

    assert(llsntrim(out, make_lstr("  alpha   beta  ", src, 64), 63, 'R', '_') == out);
    assert(eq_lstr_ascii(out, "_alpha_beta"));

    assert(llsntrim(out, make_lstr("  alpha   beta  ", src, 64), 63, 'L', '_') == out);
    assert(eq_lstr_ascii(out, "alpha_beta_"));

    assert(llsntrim(out, make_lstr("  alpha   beta  ", src, 64), 63, 'B', '_') == out);
    assert(eq_lstr_ascii(out, "alpha_beta"));
}

static void test_replace_map_and_m_semantics(void) {
    lchar_t out[64];
    lchar_t src[64];
    lchar_t map[64];

    assert(llsnreplace(
               out, 63,
               make_lstr("foo foo foo", src, 64),
               make_lstr("foo|bar", map, 64),
               '|', 63, 2) == out);
    assert(eq_lstr_ascii(out, "foo bar foo"));

    assert(llsnreplace(
               out, 63,
               make_lstr("foo foo foo", src, 64),
               make_lstr("foo|bar", map, 64),
               '|', 63, -1) == out);
    assert(eq_lstr_ascii(out, "foo foo bar"));

    assert(llsnreplace(
               out, 63,
               make_lstr("cat", src, 64),
               make_lstr("catdog", map, 64),
               '|', 63, 0) == NULL);
    assert(llsnreplace(
               out, 63,
               make_lstr("cat foo", src, 64),
               make_lstr("cat|dog|foo|bar", map, 64),
               '|', 63, 1) == NULL);
}

static void test_replace_map_order_overlap(void) {
    lchar_t out[64];
    lchar_t src[64];
    lchar_t map[64];

    assert(llsnreplace(
               out, 63,
               make_lstr("ababa", src, 64),
               make_lstr("ab|X|a|Y", map, 64),
               '|', 63, 0) == out);
    assert(eq_lstr_ascii(out, "XXY"));
}

static void test_pad_m_sign_semantics(void) {
    lchar_t out[16];
    lchar_t src[16];

    assert(llsnpad(out, 6, make_lstr("xy", src, 16), 8, '.', 1) == out);
    assert(eq_lstr_ascii(out, "....xy"));

    assert(llsnpad(out, 6, make_lstr("xy", src, 16), 8, '.', -1) == out);
    assert(eq_lstr_ascii(out, "xy...."));

    assert(llsnpad(out, 6, make_lstr("xy", src, 16), 8, '.', 0) == out);
    assert(eq_lstr_ascii(out, "..xy.."));
}

static void test_repeat_overflow_guard(void) {
    lchar_t out[16];
    lchar_t src[16];

    assert(llsnrepeat(out, 10, make_lstr("ab", src, 16), 8, (size_t)-1) == NULL);
    assert(out[10] == (lchar_t)0);
}

static void test_sub_basic_and_mismatch(void) {
    lchar_t out_l[32];
    uchar_t out_u[32];
    lchar_t src_l[32];
    lchar_t from_l[32];
    lchar_t to_l[32];
    uchar_t src_u[32];

    assert(llsnsub(
               out_l, 31,
               make_lstr("abc cab", src_l, 32),
               16,
               make_lstr("abc", from_l, 32),
               make_lstr("xyz", to_l, 32)) == out_l);
    assert(eq_lstr_ascii(out_l, "xyz zxy"));

    assert(lusnsub(
               out_u, 31,
               make_ustr("abc", src_u, 32),
               8,
               make_lstr("abc", from_l, 32),
               make_lstr("XYZ", to_l, 32)) == out_u);
    assert(eq_ustr_ascii(out_u, "XYZ"));

    assert(llsnsub(
               out_l, 31,
               make_lstr("abc", src_l, 32),
               8,
               make_lstr("ab", from_l, 32),
               make_lstr("XYZ", to_l, 32)) == NULL);
}

int main(void) {
    test_trim_collapse_edges();
    test_replace_map_and_m_semantics();
    test_replace_map_order_overlap();
    test_pad_m_sign_semantics();
    test_repeat_overflow_guard();
    test_sub_basic_and_mismatch();

    puts("All lubtype regression tests passed.");
    return 0;
}
