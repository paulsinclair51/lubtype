/**
 * @file test_cmp_search.c
 * @brief X-macro tests for string comparison and search functions
 *        in lubtype.h.
 *
 * @note Compiled twice: -DLUB_X_IS_L for Latin, -DLUB_X_IS_U for Unicode.
 *       Each test file needs `static lub_test_result_t test_result;`.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include "lubtype_test_declarations.h"

static void test_search_delim_negative_m(void)
{ const xchar_t hay[] = {'a','b','a','b','a',0};
  const xchar_t needle[] = {'a','b','a',0};
  LUB_ASSERT(xxsnstrm(hay, 16, needle, '|', -1) == hay + 2);
  LUB_ASSERT(xxsnSTRM(hay, 16, needle, '|', -1) == hay + 2);
}

static void test_search_charset_negative_m(void)
{ const xchar_t hay[] = {'a','b','a','c','a','d',0};
  const xchar_t set[] = {'b','d',0};
  LUB_ASSERT(xxsnstrm(hay, 16, set, 0, -1) == hay + 5);
  LUB_ASSERT(xxsnSTRM(hay, 16, set, 0, -1) == hay + 5);
}

static void test_search_case_insensitive(void)
{ const xchar_t hay[] = {'a','B','a','b','A',0};
  const xchar_t needle[] = {'A','B','A',0};
  LUB_ASSERT(xxsnSTRM(hay, 16, needle, '|', -1) == hay + 2);
}

static void test_search_empty_token(void)
{ const xchar_t empty[] = {0};
  const xchar_t one[] = {'a',0};
  const xchar_t one_delim[] = {'|',0};
  LUB_ASSERT(xxsnstrm(empty, 4, one_delim, '|', 1) == empty);
  LUB_ASSERT(xxsnstrm(one, 4, empty, 0, 1) == NULL);
  LUB_ASSERT(xxsnstrm(one, 4, empty, 0, 1) == NULL);
}

static void test_search_m_zero_returns_null(void)
{ const xchar_t hay[] = {'a','b','a','b','a',0};
  const xchar_t needle_delim[] = {'a','b',0};
  const xchar_t needle_set[] = {'a','b',0};
  LUB_ASSERT(xxsnstrm(hay, 16, needle_delim, '|', 0) == NULL);
  LUB_ASSERT(xxsnSTRM(hay, 16, needle_delim, '|', 0) == NULL);
  LUB_ASSERT(xxsnstrm(hay, 16, needle_set, 0, 0) == NULL);
  LUB_ASSERT(xxsnSTRM(hay, 16, needle_set, 0, 0) == NULL);
}

static void test_search_forward_and_reverse_indexing(void)
{ const xchar_t hay[] = {'a','b','a','b','a',0};
  const xchar_t needle[] = {'a','b','a',0};
  LUB_ASSERT(xxsnstrm(hay, 16, needle, '|', 1) == hay);
  LUB_ASSERT(xxsnstrm(hay, 16, needle, '|', 2) == hay + 2);
  LUB_ASSERT(xxsnstrm(hay, 16, needle, '|', -1) == hay + 2);
  LUB_ASSERT(xxsnstrm(hay, 16, needle, '|', -2) == hay);
  LUB_ASSERT(xxsnstrm(hay, 16, needle, '|', 3) == NULL);
}

static void test_search_case_sensitive_vs_insensitive(void)
{ const xchar_t hay[] = {'A','b','a','B','a',0};
  const xchar_t needle[] = {'a','b','a',0};
  LUB_ASSERT(xxsnstrm(hay, 16, needle, '|', 1) == NULL);
  LUB_ASSERT(xxsnSTRM(hay, 16, needle, '|', 1) == hay);
}

static void test_count_case_sensitive_vs_insensitive(void)
{ const xchar_t hay[] = {'A','B','a','b',0};
  const xchar_t needle[] = {'a','b',0};
  LUB_ASSERT(xxsncnt(hay, 16, needle, '|') == 1);
  LUB_ASSERT(xxsnCNT(hay, 16, needle, '|') > xxsncnt(hay, 16, needle, '|'));
}

static void test_compare_fixed_prefix_suffix_boundaries(void)
{ const xchar_t s1[] = {'A','b','c','d',0};
  const xchar_t s2[] = {'a','b','c','x',0};
  LUB_ASSERT(xxsnnfxdcmp(s1, 3, s2, 3) < 0);
  LUB_ASSERT(xxsnnFXDCMP(s1, 3, s2, 3) == 0);
  LUB_ASSERT(xxsnnpfxcmp(s1, 4, (const xchar_t[]){'A','b',0}, 2) == 0);
  LUB_ASSERT(xxsnnpfxcmp(s1, 4, (const xchar_t[]){'a','b',0}, 2) != 0);
  LUB_ASSERT(xxsnnPFXCMP(s1, 4, (const xchar_t[]){'a','b',0}, 2) == 0);
  LUB_ASSERT(xxsnnsfxcmp(s1, 4, (const xchar_t[]){'c','d',0}, 2) == 0);
  LUB_ASSERT(xxsnnsfxcmp(s1, 4, (const xchar_t[]){'C','D',0}, 2) != 0);
  LUB_ASSERT(xxsnnSFXCMP(s1, 4, (const xchar_t[]){'C','D',0}, 2) == 0);
}

#define CMP_SEARCH_TESTS(X) \
  X(test_search_delim_negative_m) \
  X(test_search_charset_negative_m) \
  X(test_search_case_insensitive) \
  X(test_search_empty_token) \
  X(test_search_m_zero_returns_null) \
  X(test_search_forward_and_reverse_indexing) \
  X(test_search_case_sensitive_vs_insensitive) \
  X(test_count_case_sensitive_vs_insensitive) \
  X(test_compare_fixed_prefix_suffix_boundaries)

/**
 * @brief Run tests for compare and search functions.
 *
 * Tests current compare and string-search/count helpers.
 * Each block checks a specific function or family for
 * correct behavior and edge cases.
 */
lub_test_result_t LUB_PASTE(run_cmp_search_tests_, LUB_X)(int inject_faults)
{ test_result = (lub_test_result_t){0};
  xchar_t s1[16] = {'a','b','c','\0'};
  xchar_t s2[16] = {'a','b','c','\0'};
  xchar_t s3[16] = {'a','b','d','\0'};

  // Compare.
  LUB_ASSERT(xxsnncmp(s1, 3, s2, 3) == 0);
  LUB_ASSERT(xxsnncmp(s1, 3, s3, 3) < 0);

  // Search/count.
  #define RUN_TEST(fn) fn();
  CMP_SEARCH_TESTS(RUN_TEST)
  #undef RUN_TEST
  { const xchar_t hay_count[] = {'a','b','c','a','b','c',0};
    const xchar_t needle_count[] = {'a','b',0};
    LUB_ASSERT(xxsncnt(hay_count, 6, needle_count, '|') == 2);
  }

  return test_result;
}
