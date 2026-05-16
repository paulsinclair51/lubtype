/**
 * @file test_cmp_search.c
 * @brief Tests for compare and search functions in lubtype.h.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#if !defined(LUB_X_IS_L) && !defined(LUB_X_IS_U)
#define LUB_X_IS_L
#endif

#include <assert.h>
#include <stdio.h>

#include "lubtype_test_declarations.h"
#include "../lubtype.h"

static size_t test_count = 0;

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

/**
 * @brief Run tests for compare and search functions.
 *
 * Tests current compare and string-search/count helpers.
 * Each block checks a specific function or family for
 * correct behavior and edge cases.
 */
size_t LUB_PASTE(run_cmp_search_tests_, LUB_X)(void)
{ test_count = 0;
  xchar_t s1[16] = {'a','b','c','\0'};
  xchar_t s2[16] = {'a','b','c','\0'};
  xchar_t s3[16] = {'a','b','d','\0'};

  // Compare.
  LUB_ASSERT(xxsnncmp(s1, 3, s2, 3) == 0);
  LUB_ASSERT(xxsnncmp(s1, 3, s3, 3) < 0);

  // Search/count.
  test_search_delim_negative_m();
  test_search_charset_negative_m();
  test_search_case_insensitive();
  test_search_empty_token();
  { const xchar_t hay_count[] = {'a','b','c','a','b','c',0};
    const xchar_t needle_count[] = {'a','b',0};
    LUB_ASSERT(xxsncnt(hay_count, 6, needle_count, '|') == 2);
  }

  printf("Comparison/search tests passed for LUB_X=%s.\n",
         LUB_STRINGIFY(LUB_X));
  return test_count;
}
