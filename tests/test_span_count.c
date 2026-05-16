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
 * @brief Test count functions using current substring-based APIs.
 */
static void test_count_character(void) {
    lchar_t str[64];
    lchar_t needle[4];

    LUB_ASSERT(llsncnt(make_lstr_local("hello world", str, 64), 64,
                       make_lstr_local("l", needle, 4), 0) == 3);

    LUB_ASSERT(llsncnt(make_lstr_local("hello", str, 64), 64,
                       make_lstr_local("z", needle, 4), 0) == 0);

    LUB_ASSERT(llsncnt(make_lstr_local("a", str, 64), 64,
                       make_lstr_local("a", needle, 4), 0) == 1);

    LUB_ASSERT(llsnCNT(make_lstr_local("HeLLo", str, 64), 64,
                       make_lstr_local("l", needle, 4), 0) == 2);
}

/**
 * @brief Test substring count functions.
 */
static void test_substring_count(void) {
    lchar_t str[64];
    lchar_t needle[16];

    LUB_ASSERT(llsncnt(make_lstr_local("abab", str, 64), 64,
                       make_lstr_local("ab", needle, 16), '|') == 2);

    LUB_ASSERT(llsncnt(make_lstr_local("aaa", str, 64), 64,
                       make_lstr_local("aa", needle, 16), '|') == 2);

    LUB_ASSERT(llsncnt(make_lstr_local("hello", str, 64), 64,
                       make_lstr_local("xyz", needle, 16), '|') == 0);

    LUB_ASSERT(llsnCNT(make_lstr_local("CatDogCat", str, 64), 64,
                       make_lstr_local("cat", needle, 16), '|') == 2);
}

lub_test_result_t run_count_tests(void) {
    test_result = (lub_test_result_t){0};
    test_count_character();
    test_substring_count();

    return test_result;
}
