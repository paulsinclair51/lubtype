/**
 * @file test_span_count.c
 * @brief Tests for span and count functions in lubtype.h.
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
 * @brief Test span and complement span functions.
 */
static void test_span_and_complement_span(void) {
    lchar_t str[64], set[64];
    size_t result;
    // Normal cases
    result = llsnspn(make_lstr_local("abcdef", str, 64), make_lstr_local("abc", set, 64), 64);
    assert(result == 3);
    result = llsnspn(make_lstr_local("abc", str, 64), make_lstr_local("abc", set, 64), 64);
    assert(result == 3);
    result = llsnspn(make_lstr_local("def", str, 64), make_lstr_local("abc", set, 64), 64);
    assert(result == 0);
    result = llsncspn(make_lstr_local("xyz123", str, 64), make_lstr_local("0123456789", set, 64), 64);
    assert(result == 3);
    result = llsncspn(make_lstr_local("xyz", str, 64), make_lstr_local("0123456789", set, 64), 64);
    assert(result == 3);
    result = llsnSPN(make_lstr_local("ABCdef", str, 64), make_lstr_local("abc", set, 64), 64);
    assert(result == 3);
    result = llsnCSPN(make_lstr_local("XYZ123", str, 64), make_lstr_local("0123456789", set, 64), 64);
    assert(result == 3);
    // Null/empty/error edge cases
    assert(llsnspn(NULL, set, 10) == 0);
    assert(llsnspn(str, NULL, 10) == 0);
    str[0] = 0; set[0] = 0;
    assert(llsnspn(str, set, 10) == 0);
    assert(llsncspn(NULL, set, 10) == 0);
    assert(llsncspn(str, NULL, 10) == 0);
    str[0] = 0; set[0] = 0;
    assert(llsncspn(str, set, 10) == 0);
}

/**
 * @brief Test count functions for specific characters.
 */
static void test_count_character(void) {
    lchar_t str[64];
    size_t result;

    result = llsncnt(make_lstr_local("hello world", str, 64), 'l', 64);
    assert(result == 3);

    result = llsncnt(make_lstr_local("hello", str, 64), 'z', 64);
    assert(result == 0);

    result = llsncnt(make_lstr_local("a", str, 64), 'a', 64);
    assert(result == 1);

    result = llsnCNT(make_lstr_local("HeLLo", str, 64), 'l', 64);
    assert(result == 2);
}

/**
 * @brief Test substring count functions.
 */
static void test_substring_count(void) {
    lchar_t str[64];
    lchar_t needle[16];
    size_t result;

    result = llsnsubcnt(make_lstr_local("abab", str, 64),
                        make_lstr_local("ab", needle, 16), 64);
    assert(result == 2);

    result = llsnsubcnt(make_lstr_local("aaa", str, 64),
                        make_lstr_local("aa", needle, 16), 64);
    assert(result == 2);

    result = llsnsubcnt(make_lstr_local("hello", str, 64),
                        make_lstr_local("xyz", needle, 16), 64);
    assert(result == 0);

    result = llsnSUBCNT(make_lstr_local("CatDogCat", str, 64),
                        make_lstr_local("cat", needle, 16), 64);
    assert(result == 2);
}

void run_span_count_tests(void) {
    test_span_and_complement_span();
    test_count_character();
    test_substring_count();

}
