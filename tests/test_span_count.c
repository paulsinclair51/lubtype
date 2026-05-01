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
 * @brief Test count functions using current substring-based APIs.
 */
static void test_count_character(void) {
    lchar_t str[64];
    lchar_t needle[4];
    size_t result;

    result = llsncnt(make_lstr_local("hello world", str, 64),
                     make_lstr_local("l", needle, 4), 64, 0);
    assert(result == 3);

    result = llsncnt(make_lstr_local("hello", str, 64),
                     make_lstr_local("z", needle, 4), 64, 0);
    assert(result == 0);

    result = llsncnt(make_lstr_local("a", str, 64),
                     make_lstr_local("a", needle, 4), 64, 0);
    assert(result == 1);

    result = llsnCNT(make_lstr_local("HeLLo", str, 64),
                     make_lstr_local("l", needle, 4), 64, 0);
    assert(result == 2);

    assert(llsncnt(NULL, needle, 10, 0) == (size_t)LUB_PTR_INVALID);
    assert(llsnCNT(str, NULL, 10, 0) == (size_t)LUB_PTR_INVALID);
}

/**
 * @brief Test substring count functions.
 */
static void test_substring_count(void) {
    lchar_t str[64];
    lchar_t needle[16];
    size_t result;

    result = llsncnt(make_lstr_local("abab", str, 64),
                     make_lstr_local("ab", needle, 16), 64, '|');
    assert(result == 2);

    result = llsncnt(make_lstr_local("aaa", str, 64),
                     make_lstr_local("aa", needle, 16), 64, '|');
    assert(result == 2);

    result = llsncnt(make_lstr_local("hello", str, 64),
                     make_lstr_local("xyz", needle, 16), 64, '|');
    assert(result == 0);

    result = llsnCNT(make_lstr_local("CatDogCat", str, 64),
                     make_lstr_local("cat", needle, 16), 64, '|');
    assert(result == 2);
}

void run_span_count_tests(void) {
    test_count_character();
    test_substring_count();

}
