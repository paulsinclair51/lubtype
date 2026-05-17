/**
 * @file test_skip.c
 * @brief X-macro tests for xxsnskip in lubtype.h, plus ulsnskip cross-type
 *        tests under LUB_X_IS_U.
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
#include <stddef.h>
#include <stdio.h>

#include "../lubtype.h"
#include "lubtype_test_declarations.h"

static lub_test_result_t test_result;

/* ---- helpers ------------------------------------------------------------ */

static xchar_t *make_xs(const char *src, xchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (xchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (xchar_t)(unsigned char)src[i];
    dst[i] = (xchar_t)0;
    return dst;
}

/* Check that an xchar_t* pointer equals a given ASCII suffix of the original. */
static int xptr_eq(const xchar_t *p, const char *expected) {
    size_t i = 0;
    if (!p || !expected) return 0;
    for (; expected[i]; ++i)
        if (p[i] != (xchar_t)(unsigned char)expected[i]) return 0;
    return p[i] == (xchar_t)0;
}

#if defined(LUB_X_IS_U)
static uchar_t *make_us(const char *src, uchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (uchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (uchar_t)(unsigned char)src[i];
    dst[i] = (uchar_t)0;
    return dst;
}
static int uptr_eq(const uchar_t *p, const char *expected) {
    size_t i = 0;
    if (!p || !expected) return 0;
    for (; expected[i]; ++i)
        if (p[i] != (uchar_t)(unsigned char)expected[i]) return 0;
    return p[i] == (uchar_t)0;
}
/* Check that an lchar_t* pointer equals a given ASCII suffix (for ulsnskip skip-sets). */
static lchar_t *make_ls(const char *src, lchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (lchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (lchar_t)(unsigned char)src[i];
    dst[i] = (lchar_t)0;
    return dst;
}
#endif /* LUB_X_IS_U */

/* ---- llsnskip ----------------------------------------------------------- */

/* ---- xxsnskip ----------------------------------------------------------- */

static void test_xxsnskip_explicit_skipset(void) {
    xchar_t s[32];
    xchar_t sk[8];

    /* Skip leading 'a' and 'b' characters; stop at 'c'. */
    LUB_ASSERT(xptr_eq(xxsnskip(make_xs("aaabbc_rest", s, 32),
                             32, make_xs("ab", sk, 8)), "c_rest"));

    /* First char not in skip-set; pointer unchanged. */
    LUB_ASSERT(xptr_eq(xxsnskip(make_xs("xyz", s, 32),
                             32, make_xs("ab", sk, 8)), "xyz"));

    /* All characters in skip-set; pointer reaches null terminator. */
    {   xchar_t *r = xxsnskip(make_xs("aaa", s, 32), 32, make_xs("a", sk, 8));
        LUB_ASSERT(r && *r == (xchar_t)0);
    }

    /* Empty source; returns pointer to null terminator. */
    {   xchar_t *r = xxsnskip(make_xs("", s, 32), 32, make_xs("ab", sk, 8));
        LUB_ASSERT(r && *r == (xchar_t)0);
    }

    /* Single-character match at start. */
    LUB_ASSERT(xptr_eq(xxsnskip(make_xs("xhello", s, 32),
                             32, make_xs("x", sk, 8)), "hello"));
}

static void test_xxsnskip_default_whitespace(void) {
    xchar_t s[32];

    /* Leading spaces skipped; stops at 'h'. */
    LUB_ASSERT(xptr_eq(xxsnskip(make_xs("   hello", s, 32), 32, NULL), "hello"));

    /* Mixed whitespace (space + tab). */
    {   xchar_t mixed[16];
        mixed[0] = ' '; mixed[1] = '\t'; mixed[2] = 'w'; mixed[3] = 0;
        LUB_ASSERT(xptr_eq(xxsnskip(mixed, 16, NULL), "w"));
    }

    /* Empty skip string "" triggers whitespace default. */
    {   xchar_t sk[4] = {0};
        LUB_ASSERT(xptr_eq(xxsnskip(make_xs("  hi", s, 32), 32, sk), "hi"));
    }

    /* No leading whitespace; pointer unchanged. */
    LUB_ASSERT(xptr_eq(xxsnskip(make_xs("hello", s, 32), 32, NULL), "hello"));

    /* All whitespace; pointer at null terminator. */
    {   xchar_t *r = xxsnskip(make_xs("   ", s, 32), 32, NULL);
        LUB_ASSERT(r && *r == (xchar_t)0);
    }

    /* Empty source with default skip. */
    {   xchar_t *r = xxsnskip(make_xs("", s, 32), 32, NULL);
        LUB_ASSERT(r && *r == (xchar_t)0);
    }
}

static void test_xxsnskip_error_paths(void) {
    /* NULL source returns NULL. */
    LUB_ASSERT(xxsnskip(NULL, 32, NULL) == NULL);
}

/* ---- ulsnskip ----------------------------------------------------------- */

#if defined(LUB_X_IS_U)
static void test_ulsnskip_explicit_skipset(void) {
    uchar_t s[32];
    lchar_t sk[8];

    /* Skip ASCII digits at start. */
    LUB_ASSERT(uptr_eq(ulsnskip(make_us("123abc", s, 32),
                             32, make_ls("123", sk, 8)), "abc"));

    /* No match; pointer unchanged. */
    LUB_ASSERT(uptr_eq(ulsnskip(make_us("abc", s, 32),
                             32, make_ls("123", sk, 8)), "abc"));

    /* All chars skipped; pointer at null terminator. */
    {   uchar_t *r = ulsnskip(make_us("111", s, 32), 32, make_ls("1", sk, 8));
        LUB_ASSERT(r && *r == (uchar_t)0);
    }

    /* Empty source. */
    {   uchar_t *r = ulsnskip(make_us("", s, 32), 32, make_ls("1", sk, 8));
        LUB_ASSERT(r && *r == (uchar_t)0);
    }
}

static void test_ulsnskip_default_whitespace(void) {
    uchar_t s[32];

    /* Leading spaces; stops at 'w'. */
    LUB_ASSERT(uptr_eq(ulsnskip(make_us("  word", s, 32), 32, NULL), "word"));

    /* No leading whitespace. */
    LUB_ASSERT(uptr_eq(ulsnskip(make_us("word", s, 32), 32, NULL), "word"));

    /* All whitespace. */
    {   uchar_t *r = ulsnskip(make_us("   ", s, 32), 32, NULL);
        LUB_ASSERT(r && *r == (uchar_t)0);
    }

    /* Unicode character > LUB_MAX_LCHAR is not treated as whitespace. */
    {   uchar_t hi[4];
        hi[0] = (uchar_t)0x3000; /* ideographic space, > LUB_MAX_LCHAR */
        hi[1] = (uchar_t)'x';
        hi[2] = (uchar_t)0;
        /* 0x3000 > LUB_MAX_LCHAR so the default whitespace branch skips it
         * only if *s <= LUB_MAX_LCHAR — since 0x3000 fails that test the
         * loop should stop immediately, returning the original pointer. */
        uchar_t *r = ulsnskip(hi, 4, NULL);
        LUB_ASSERT(r == hi);
    }
}

static void test_ulsnskip_error_paths(void) {
    /* NULL source returns NULL. */
    LUB_ASSERT(ulsnskip(NULL, 32, NULL) == NULL);
}
#endif /* LUB_X_IS_U */


static void test_null_skip_defaults_whitespace_all_variants(void) {
    xchar_t xs[16];

    LUB_ASSERT(xptr_eq(xxsnskip(make_xs("   a", xs, 16), 16, NULL), "a"));
#if defined(LUB_X_IS_U)
    {
        uchar_t us[16];
        LUB_ASSERT(uptr_eq(ulsnskip(make_us("\t\tb", us, 16), 16, NULL), "b"));
    }
#endif
}

/* ---- skip-set loop correctness ------------------------------------------ */

static void test_skipset_first_char_is_stop(void) {
    /* Verify that when s[0] is NOT in the skip-set, s itself is returned
     * immediately (no off-by-one advance). */
    xchar_t xs[8];

    xchar_t *xp = xxsnskip(make_xs("Xabc", xs, 8), 8,
                            (const xchar_t *)"abc");
    LUB_ASSERT(xp == xs); /* pointer must be the original s, not s+1 */
}

static void test_skipset_consecutive_skip_chars(void) {
    /* Mix of skip and non-skip chars interleaved — only leading run skipped. */
    xchar_t xs[16];
    xchar_t *r = xxsnskip(make_xs("aabXab", xs, 16), 16,
                           (const xchar_t *)"ab");
    LUB_ASSERT(xptr_eq(r, "Xab"));
}

/* ---- sn bound ----------------------------------------------------------- */

static void test_sn_bound_zero(void) {
    /* sn=0: lcsnlen/ucsnlen will find the null terminator at position 0 only
     * if the very first byte is '\0'.  For a non-empty string this is treated
     * as unterminated and an error pointer is returned. */
    xchar_t xs[8];
    make_xs("hello", xs, 8);
    xchar_t *r = xxsnskip(xs, 0, NULL);
    /* Should be an error value (non-NULL error pointer) or NULL. */
    LUB_ASSERT(r == NULL || LUB_PTR_ERR(r, 0));
}

#define SKIP_TESTS_COMMON(X) \
    X(test_xxsnskip_explicit_skipset) \
    X(test_xxsnskip_default_whitespace) \
    X(test_xxsnskip_error_paths) \
    X(test_null_skip_defaults_whitespace_all_variants) \
    X(test_skipset_first_char_is_stop) \
    X(test_skipset_consecutive_skip_chars) \
    X(test_sn_bound_zero)

#if defined(LUB_X_IS_U)
#define SKIP_TESTS_U_ONLY(X) \
    X(test_ulsnskip_explicit_skipset) \
    X(test_ulsnskip_default_whitespace) \
    X(test_ulsnskip_error_paths)
#endif

/* ---- entry point -------------------------------------------------------- */

lub_test_result_t LUB_PASTE(run_skip_tests_, LUB_X)(void) {
    test_result = (lub_test_result_t){0};
    #define RUN_TEST(fn) fn();
    SKIP_TESTS_COMMON(RUN_TEST)
#if defined(LUB_X_IS_U)
    SKIP_TESTS_U_ONLY(RUN_TEST)
#endif
    #undef RUN_TEST

    return test_result;
}
