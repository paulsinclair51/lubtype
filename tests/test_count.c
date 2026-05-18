/**
 * @file test_count.c
 * @brief X-macro tests for character counting functions in lubtype.h.
 *
 * @note Compiled twice: once with -DLUB_X_IS_L for Latin (lchar_t)
 *       and once with -DLUB_X_IS_U for Unicode (uchar_t). Each
 *       compilation produces a separate object file and function:
 *       run_count_tests_l and run_count_tests_u.
 *       Requires `static lub_test_result_t test_result;` at file scope.
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

lub_test_result_t LUB_PASTE(run_count_tests_, LUB_X)(int inject_faults)
{ test_result = (lub_test_result_t){0};
  // Test count functions using single-character needle strings.
  { xchar_t str[64];
    xchar_t needle[4];
    LUB_ASSERT(xxsncnt(make_xstr_local("hello world", str, 64), 64,
                       make_xstr_local("l", needle, 4), 0) == 3);
    LUB_ASSERT(xxsncnt(make_xstr_local("hello", str, 64), 64,
                       make_xstr_local("z", needle, 4), 0) == 0);
    LUB_ASSERT(xxsncnt(make_xstr_local("a", str, 64), 64,
                       make_xstr_local("a", needle, 4), 0) == 1);
    LUB_ASSERT(xxsnCNT(make_xstr_local("HeLLo", str, 64), 64,
                     make_xstr_local("l", needle, 4), 0) == 2);
  }
  if (inject_faults)
  {  /* Assert-level guard: NULL dereference inside LUB_ASSERT triggers SIGSEGV
      * which is caught by the per-assert signal handler and counted as an
      * exception. */
    LUB_ASSERT(xxsncnt((const xchar_t *)NULL, 64, (const xchar_t *)NULL, 0) > 0);
  }
  // Test substring count functions.
  { xchar_t str[64];
    xchar_t needle[16];

    LUB_ASSERT(xxsncnt(make_xstr_local("abab", str, 64), 64,
                       make_xstr_local("ab", needle, 16), '|') == 2);

    LUB_ASSERT(xxsncnt(make_xstr_local("aaa", str, 64), 64,
                       make_xstr_local("aa", needle, 16), '|') == 2);

    LUB_ASSERT(xxsncnt(make_xstr_local("hello", str, 64), 64,
                       make_xstr_local("xyz", needle, 16), '|') == 0);

    LUB_ASSERT(xxsnCNT(make_xstr_local("CatDogCat", str, 64), 64,
                       make_xstr_local("cat", needle, 16), '|') == 2);
  }

  return test_result;
}
