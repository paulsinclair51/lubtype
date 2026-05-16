/**
 * @file test_advanced_ops.c
 * @brief X-macro tests for advanced string operations in lubtype.h
 *        (trim/reverse/pad/replace).
 *
 * @note Compiled twice: -DLUB_X_IS_L for Latin, -DLUB_X_IS_U for Unicode.
 *       Each test file needs `static lub_test_result_t test_result;`.
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

#if defined(LUB_X_IS_L)
#define XLTOCASE_CONCRETE(c) lltocase((c))
#define XLTOLOWER_CONCRETE(c) lltolower((c))
#define XLTOUPPER_CONCRETE(c) lltoupper((c))
#define XUTOCASE_CONCRETE(c, lrep) lutocase((c), (lrep))
#define XUTOLOWER_CONCRETE(c, lrep) lutolower((c), (lrep))
#define XUTOUPPER_CONCRETE(c, lrep) lutoupper((c), (lrep))
#define XXTOCASE_CONCRETE(c) lltocase((c))
#else
#define XLTOCASE_CONCRETE(c) ultocase((c))
#define XLTOLOWER_CONCRETE(c) ultolower((c))
#define XLTOUPPER_CONCRETE(c) ultoupper((c))
#define XUTOCASE_CONCRETE(c, lrep) uutocase((c))
#define XUTOLOWER_CONCRETE(c, lrep) uutolower((c))
#define XUTOUPPER_CONCRETE(c, lrep) uutoupper((c))
#define XXTOCASE_CONCRETE(c) uutocase((c))
#endif

/**
 * @brief Run tests for advanced string operations
 *        (trim, reverse, pad, replace).
 */

static void test_replace_family(void)
{
  xchar_t out[64] = {0};

  /* xxsnnreplace: verify m semantics (nth, last, all). */
  {
    const xchar_t src[] = {'a','a','a',0};
    const xchar_t map[] = {'a','|','x',0};
    const xchar_t expected_m2[] = {'a','x','a',0};
    const xchar_t expected_last[] = {'a','a','x',0};
    const xchar_t expected_all[] = {'x','x','x',0};

    LUB_ASSERT(xxsnnreplace(out, 64, src, 3, map, '|', 2) != NULL);
    LUB_ASSERT(xxsnncmp(out, 3, expected_m2, 3) == 0);

    LUB_ASSERT(xxsnnreplace(out, 64, src, 3, map, '|', -1) != NULL);
    LUB_ASSERT(xxsnncmp(out, 3, expected_last, 3) == 0);

    LUB_ASSERT(xxsnnreplace(out, 64, src, 3, map, '|', 0) != NULL);
    LUB_ASSERT(xxsnncmp(out, 3, expected_all, 3) == 0);
  }

  /* Multi-pair map requires m == 0; m != 0 should fail. */
  {
    const xchar_t src[] = {'a','b','a',0};
    const xchar_t map_multi[] = {'a','|','x','|','b','|','y',0};
    const xchar_t expected_all[] = {'x','y','x',0};

    LUB_ASSERT(xxsnnreplace(out, 64, src, 3, map_multi, '|', 1) == NULL);
    LUB_ASSERT(xxsnnreplace(out, 64, src, 3, map_multi, '|', 0) != NULL);
    LUB_ASSERT(xxsnncmp(out, 3, expected_all, 3) == 0);
  }
}

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

  {
    xchar_t xc = (xchar_t)'a';
    xchar_t xu = (xchar_t)'A';
    LUB_ASSERT(xltocase(xc) == XLTOCASE_CONCRETE(xc));
    LUB_ASSERT(xltolower(xu) == XLTOLOWER_CONCRETE(xu));
    LUB_ASSERT(xltoupper(xc) == XLTOUPPER_CONCRETE(xc));
    LUB_ASSERT(xutocase(xu, (xchar_t)'?') == XUTOCASE_CONCRETE(xu, (xchar_t)'?'));
    LUB_ASSERT(xutolower(xu, (xchar_t)'?') == XUTOLOWER_CONCRETE(xu, (xchar_t)'?'));
    LUB_ASSERT(xutoupper(xc, (xchar_t)'?') == XUTOUPPER_CONCRETE(xc, (xchar_t)'?'));
    LUB_ASSERT(xxtocase(xc) == XXTOCASE_CONCRETE(xc));
  }

  LUB_ASSERT(xlsnnpad(xout, 8, (const xchar_t *)"abc", 3,
                  (const xchar_t[]){'R', 'x', 0}) != NULL);
  { const xchar_t expected_pad[9] = {'a','b','c','x','x','x','x','x',0};
    LUB_ASSERT(xlsnncmp(xout, 8, expected_pad, 8) == 0);
  }

  test_replace_family();

  printf("Advanced operation tests passed for LUB_X=%s.\n",
         LUB_STRINGIFY(LUB_X));
  return test_result;
}
