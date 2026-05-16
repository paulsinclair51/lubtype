/**
 * @file test_advanced_ops.c
 * @brief Tests for advanced string operations in lubtype.h
 *        (trim/rev/pad/replace).
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#if !defined(LUB_X_IS_L) && !defined(LUB_X_IS_U)
#define LUB_X_IS_L
#endif

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "lubtype_test_declarations.h"
#include "../lubtype.h"

static lub_test_result_t test_result;

/**
 * @brief Run tests for advanced string operations
 *        (trim, reverse, pad, replace,
 *        split).
 */

lub_test_result_t LUB_PASTE(run_advanced_ops_tests_, LUB_X)(void)
{ test_result = (lub_test_result_t){0};
  xchar_t xsrc[32] = {' ','a','b','c',' ',0};
  xchar_t xtrim_custom_src[32] = {'x','x','a','b','c','x',0};
  lchar_t xtrim_custom_opt[3] = {'L','x',0};
  lchar_t xtrim_invalid_opt[3] = {'Q','x',0};
  xchar_t xout[32] = {0};
#if defined(LUB_X_IS_L)
  uchar_t x_trim_nonlatin_opt[3] = {'B', 0x0120, 0};
  uchar_t usrc[32] = {0x20, 'a', 'b', 'c', 0x20, 0};
  uchar_t utrim_custom_src[32] = {'x','a','b','c','x','x',0};
  uchar_t utrim_opt[2] = {'B', 0};
  uchar_t utrim_custom_opt[3] = {'R','x',0};
#endif

  size_t len = 0;

  const xchar_t *xtrim = xlsnptrim(xsrc, 32, (const lchar_t *)"B", &len);
  LUB_ASSERT(xtrim == xsrc + 1 && len == 3);

  xtrim = xlsnptrim(xtrim_custom_src, 32, xtrim_custom_opt, &len);
  LUB_ASSERT(xtrim == xtrim_custom_src + 2 && len == 4);

  xtrim = xlsnptrim(xtrim_custom_src, 32, xtrim_invalid_opt, &len);
  LUB_ASSERT(LUB_PTR_ERR(xtrim, LUB_OPT_RESERVED));
  LUB_ASSERT(len == 0);

#if defined(LUB_X_IS_L)
  xtrim = xusnptrim(xsrc, 32, x_trim_nonlatin_opt, &len);
  LUB_ASSERT(xtrim == xsrc && len == 5);

  const uchar_t *ultrim = uusnptrim(usrc, 32, NULL, &len);
  LUB_ASSERT(ultrim == usrc + 1 && len == 3);

  const uchar_t *utrim = uusnptrim(usrc, 32, utrim_opt, &len);
  LUB_ASSERT(utrim == usrc + 1 && len == 3);

  utrim = uusnptrim(utrim_custom_src, 32, utrim_custom_opt, &len);
  LUB_ASSERT(utrim == utrim_custom_src && len == 4);
#endif

  LUB_ASSERT(xxsnnreverse(xout, 32, xsrc, 32) != NULL);
  LUB_ASSERT(xout[0] == ' ' && xout[1] == 'c' && xout[2] == 'b' && xout[3] == 'a');

  LUB_ASSERT(xlsnnpad(xout, 8, (const xchar_t *)"abc", 3,
                  (const xchar_t[]){'R', 'x', 0}) != NULL);
  { const xchar_t expected_pad[9] = {'a','b','c','x','x','x','x','x',0};
    LUB_ASSERT(xlsnncmp(xout, 8, expected_pad, 8) == 0);
  }

#if defined(LUB_X_IS_L)
  LUB_ASSERT(xlsnnreplace(xout, 16, (const xchar_t *)"aabbcc", 6,
                      (const xchar_t *)"b|z", '|', 0) != NULL);
  { const xchar_t expected_replace[7] = {'a','a','z','z','c','c',0};
    LUB_ASSERT(xlsnncmp(xout, 6, expected_replace, 6) == 0);
  }
#endif

  printf("Advanced operation tests passed for LUB_X=%s.\n",
         LUB_STRINGIFY(LUB_X));
  return test_result;
}
