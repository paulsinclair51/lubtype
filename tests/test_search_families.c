/**
 * @file test_search_families.c
 * @brief Tests for search function families in lubtype.h.
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
 * @brief Test current search functions using single-character needle strings.
 */
static void test_chr_search_with_m_semantics(void) {
    lchar_t haystack[64];
    lchar_t needle[4];
    lchar_t *result;

    result = llsnstrm(make_lstr_local("hello world", haystack, 64),
                      make_lstr_local("l", needle, 4), 64, 0, 1);
    assert(result != NULL && *result == 'l');

    result = llsnstrm(make_lstr_local("hello world", haystack, 64),
                      make_lstr_local("l", needle, 4), 64, 0, 2);
    assert(result != NULL && *result == 'l');

    result = llsnstrm(make_lstr_local("hello world", haystack, 64),
                      make_lstr_local("l", needle, 4), 64, 0, 0);
    assert(result == NULL);

    result = llsnstrm(make_lstr_local("hello", haystack, 64),
                      make_lstr_local("z", needle, 4), 64, 0, 1);
    assert(result == NULL);

    result = llsnstrm(make_lstr_local("hello world", haystack, 64),
                      make_lstr_local("l", needle, 4), 64, 0, -1);
    assert(result != NULL && *result == 'l');

    result = llsnstrm(make_lstr_local("hello world", haystack, 64),
                      make_lstr_local("l", needle, 4), 64, 0, -2);
    assert(result != NULL && *result == 'l');
}

/**
 * @brief Test character search (case-insensitive).
 */
static void test_chr_search_case_insensitive(void) {
    lchar_t haystack[64];
    lchar_t needle[4];
    lchar_t *result;

    result = llsnSTRM(make_lstr_local("Hello World", haystack, 64),
                      make_lstr_local("h", needle, 4), 64, 0, 1);
    assert(result != NULL);

    result = llsnSTRM(make_lstr_local("Hello World", haystack, 64),
                      make_lstr_local("h", needle, 4), 64, 0, -1);
    assert(result != NULL);
}

/**
 * @brief Test string search with m semantics (mth occurrence).
 */
static void test_str_search_with_m_semantics(void) {
    lchar_t haystack[64];
    lchar_t needle[16];
    lchar_t *result;

    result = llsnstrm(make_lstr_local("cat dog cat", haystack, 64),
                      make_lstr_local("cat", needle, 16), 64, '|', 1);
    assert(result != NULL);

    result = llsnstrm(make_lstr_local("cat dog cat", haystack, 64),
                      make_lstr_local("cat", needle, 16), 64, '|', 2);
    assert(result != NULL);

    result = llsnstrm(make_lstr_local("cat dog cat", haystack, 64),
                      make_lstr_local("xyz", needle, 16), 64, '|', 1);
    assert(result == NULL);

    result = llsnstrm(make_lstr_local("cat dog cat", haystack, 64),
                      make_lstr_local("cat", needle, 16), 64, '|', -1);
    assert(result != NULL);
}

/**
 * @brief Test string search (case-insensitive).
 */
static void test_str_search_case_insensitive(void) {
    lchar_t haystack[64];
    lchar_t needle[16];
    lchar_t *result;

    result = llsnSTRM(make_lstr_local("CAT dog cat", haystack, 64),
                      make_lstr_local("cat", needle, 16), 64, '|', 1);
    assert(result != NULL);

    result = llsnSTRM(make_lstr_local("CAT dog CAT", haystack, 64),
                      make_lstr_local("cat", needle, 16), 64, '|', 2);
    assert(result != NULL);
}

static void test_cmp_basic_and_case_insensitive(void) {
    lchar_t s1[64];
    lchar_t s2[64];
    int cmp_result;

    cmp_result = llsncmp(make_lstr_local("hello", s1, 64),
                         make_lstr_local("hello", s2, 64), 64);
    assert(cmp_result == 0);

    cmp_result = llsncmp(make_lstr_local("apple", s1, 64),
                         make_lstr_local("banana", s2, 64), 64);
    assert(cmp_result < 0);

    cmp_result = llsncmp(make_lstr_local("zebra", s1, 64),
                         make_lstr_local("apple", s2, 64), 64);
    assert(cmp_result > 0);

    cmp_result = llsnCMP(make_lstr_local("Hello", s1, 64),
                         make_lstr_local("HELLO", s2, 64), 64);
    assert(cmp_result == 0);

    cmp_result = llsnCMP(make_lstr_local("hello", s1, 64),
                         make_lstr_local("world", s2, 64), 64);
    assert(cmp_result != 0);
}

static void test_pfx_and_sfx_matching(void) {
    lchar_t str[64];
    lchar_t pfx[64];
    lchar_t sfx[64];
    int result;

    result = llsnpfxcmp(make_lstr_local("hello world", str, 64),
                        make_lstr_local("hello", pfx, 64), 64);
    assert(result == 0);

    result = llsnpfxcmp(make_lstr_local("hello world", str, 64),
                        make_lstr_local("world", pfx, 64), 64);
    assert(result != 0);

    result = llsnsfxcmp(make_lstr_local("hello world", str, 64),
                        make_lstr_local("world", sfx, 64), 64);
    assert(result == 0);

    result = llsnsfxcmp(make_lstr_local("hello world", str, 64),
                        make_lstr_local("hello", sfx, 64), 64);
    assert(result != 0);

    result = llsnPFXCMP(make_lstr_local("Hello World", str, 64),
                        make_lstr_local("HELLO", pfx, 64), 64);
    assert(result == 0);

    result = llsnSFXCMP(make_lstr_local("Hello World", str, 64),
                        make_lstr_local("WORLD", sfx, 64), 64);
    assert(result == 0);
}

void run_search_family_tests(void) {
    test_chr_search_with_m_semantics();
    test_chr_search_case_insensitive();
    test_str_search_with_m_semantics();
    test_str_search_case_insensitive();
    test_cmp_basic_and_case_insensitive();
    test_pfx_and_sfx_matching();

}
