/**
 * @file test_search_families.c
 * @brief X-macro tests for search and compare function families in lubtype.h.
 *
 * @note Compiled twice: -DLUB_X_IS_L for Latin, -DLUB_X_IS_U for Unicode.
 *       Each test file needs `static lub_test_result_t test_result;`.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include "lubtype_test_declarations.h"

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
 * @brief Test search functions using single-character needle strings.
 */
static void test_chr_search_with_m_semantics(void) {
    xchar_t haystack[64];
    xchar_t needle[4];
    xchar_t *result;

    result = xxsnstrm(make_xstr_local("hello world", haystack, 64),
                      64, make_xstr_local("l", needle, 4), 0, 1);
    LUB_ASSERT(result != NULL && *result == 'l');

    result = xxsnstrm(make_xstr_local("hello world", haystack, 64),
                      64, make_xstr_local("l", needle, 4), 0, 2);
    LUB_ASSERT(result != NULL && *result == 'l');

    result = xxsnstrm(make_xstr_local("hello world", haystack, 64),
                      64, make_xstr_local("l", needle, 4), 0, 0);
    LUB_ASSERT(result == NULL);

    result = xxsnstrm(make_xstr_local("hello", haystack, 64),
                      64, make_xstr_local("z", needle, 4), 0, 1);
    LUB_ASSERT(result == NULL);

    result = xxsnstrm(make_xstr_local("hello world", haystack, 64),
                      64, make_xstr_local("l", needle, 4), 0, -1);
    LUB_ASSERT(result != NULL && *result == 'l');

    result = xxsnstrm(make_xstr_local("hello world", haystack, 64),
                      64, make_xstr_local("l", needle, 4), 0, -2);
    LUB_ASSERT(result != NULL && *result == 'l');
}

/**
 * @brief Test character search (case-insensitive).
 */
static void test_chr_search_case_insensitive(void) {
    xchar_t haystack[64];
    xchar_t needle[4];
    xchar_t *result;

    result = xxsnSTRM(make_xstr_local("Hello World", haystack, 64),
                      64, make_xstr_local("h", needle, 4), 0, 1);
    LUB_ASSERT(result != NULL);

    result = xxsnSTRM(make_xstr_local("Hello World", haystack, 64),
                      64, make_xstr_local("h", needle, 4), 0, -1);
    LUB_ASSERT(result != NULL);
}

/**
 * @brief Test string search with m semantics (mth occurrence).
 */
static void test_str_search_with_m_semantics(void) {
    xchar_t haystack[64];
    xchar_t needle[16];
    xchar_t *result;

    result = xxsnstrm(make_xstr_local("cat dog cat", haystack, 64),
                      64, make_xstr_local("cat", needle, 16), '|', 1);
    LUB_ASSERT(result != NULL);

    result = xxsnstrm(make_xstr_local("cat dog cat", haystack, 64),
                      64, make_xstr_local("cat", needle, 16), '|', 2);
    LUB_ASSERT(result != NULL);

    result = xxsnstrm(make_xstr_local("cat dog cat", haystack, 64),
                      64, make_xstr_local("xyz", needle, 16), '|', 1);
    LUB_ASSERT(result == NULL);

    result = xxsnstrm(make_xstr_local("cat dog cat", haystack, 64),
                      64, make_xstr_local("cat", needle, 16), '|', -1);
    LUB_ASSERT(result != NULL);
}

/**
 * @brief Test string search (case-insensitive).
 */
static void test_str_search_case_insensitive(void) {
    xchar_t haystack[64];
    xchar_t needle[16];
    xchar_t *result;

    result = xxsnSTRM(make_xstr_local("CAT dog cat", haystack, 64),
                      64, make_xstr_local("cat", needle, 16), '|', 1);
    LUB_ASSERT(result != NULL);

    result = xxsnSTRM(make_xstr_local("CAT dog CAT", haystack, 64),
                      64, make_xstr_local("cat", needle, 16), '|', 2);
    LUB_ASSERT(result != NULL);
}

static void test_cmp_basic_and_case_insensitive(void) {
    xchar_t s1[64];
    xchar_t s2[64];
    int cmp_result;

    cmp_result = xxsnncmp(make_xstr_local("hello", s1, 64), 64,
                          make_xstr_local("hello", s2, 64), 64);
    LUB_ASSERT(cmp_result == 0);

    cmp_result = xxsnncmp(make_xstr_local("apple", s1, 64), 64,
                          make_xstr_local("banana", s2, 64), 64);
    LUB_ASSERT(cmp_result < 0);

    cmp_result = xxsnncmp(make_xstr_local("zebra", s1, 64), 64,
                          make_xstr_local("apple", s2, 64), 64);
    LUB_ASSERT(cmp_result > 0);

    cmp_result = xxsnnCMP(make_xstr_local("Hello", s1, 64), 64,
                          make_xstr_local("HELLO", s2, 64), 64);
    LUB_ASSERT(cmp_result == 0);

    cmp_result = xxsnnCMP(make_xstr_local("hello", s1, 64), 64,
                          make_xstr_local("world", s2, 64), 64);
    LUB_ASSERT(cmp_result != 0);
}

static void test_pfx_and_sfx_matching(void) {
    xchar_t str[64];
    xchar_t pfx[64];
    xchar_t sfx[64];
    int result;

    result = xxsnnpfxcmp(make_xstr_local("hello world", str, 64), 64,
                         make_xstr_local("hello", pfx, 64), 64);
    LUB_ASSERT(result == 0);

    result = xxsnnpfxcmp(make_xstr_local("hello world", str, 64), 64,
                         make_xstr_local("world", pfx, 64), 64);
    LUB_ASSERT(result != 0);

    result = xxsnnsfxcmp(make_xstr_local("hello world", str, 64), 64,
                         make_xstr_local("world", sfx, 64), 64);
    LUB_ASSERT(result == 0);

    result = xxsnnsfxcmp(make_xstr_local("hello world", str, 64), 64,
                         make_xstr_local("hello", sfx, 64), 64);
    LUB_ASSERT(result != 0);

    result = xxsnnPFXCMP(make_xstr_local("Hello World", str, 64), 64,
                         make_xstr_local("HELLO", pfx, 64), 64);
    LUB_ASSERT(result == 0);

    result = xxsnnSFXCMP(make_xstr_local("Hello World", str, 64), 64,
                         make_xstr_local("WORLD", sfx, 64), 64);
    LUB_ASSERT(result == 0);
}

#define SEARCH_FAMILY_TESTS(X) \
    X(test_chr_search_with_m_semantics) \
    X(test_chr_search_case_insensitive) \
    X(test_str_search_with_m_semantics) \
    X(test_str_search_case_insensitive) \
    X(test_cmp_basic_and_case_insensitive) \
    X(test_pfx_and_sfx_matching)

lub_test_result_t LUB_PASTE(run_search_family_tests_, LUB_X)(int inject_faults) {
    test_result = (lub_test_result_t){0};
    #define RUN_TEST(fn) fn();
    SEARCH_FAMILY_TESTS(RUN_TEST)
    #undef RUN_TEST

    return test_result;
}
