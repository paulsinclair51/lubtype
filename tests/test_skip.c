/**
 * @file test_skip.c
 * @brief Tests for llsnskip, ulsnskip, and uusnskip in lubtype.h.
 *
 * @note Requires `static lub_test_result_t test_result;` at file scope.
 *       that LUB_ASSERT macros modify to track pass/fail/exception counts.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include <assert.h>
#include <stddef.h>

#include "../lubtype.h"
#include "lubtype_test_declarations.h"

static lub_test_result_t test_result;

/* ---- helpers ------------------------------------------------------------ */

static lchar_t *make_ls(const char *src, lchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (lchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (lchar_t)(unsigned char)src[i];
    dst[i] = (lchar_t)0;
    return dst;
}

static uchar_t *make_us(const char *src, uchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (uchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (uchar_t)(unsigned char)src[i];
    dst[i] = (uchar_t)0;
    return dst;
}

/* Check that a lchar_t* pointer equals a given ASCII suffix of the original. */
static int lptr_eq(const lchar_t *p, const char *expected) {
    size_t i = 0;
    if (!p || !expected) return 0;
    for (; expected[i]; ++i)
        if (p[i] != (lchar_t)(unsigned char)expected[i]) return 0;
    return p[i] == (lchar_t)0;
}

static int uptr_eq(const uchar_t *p, const char *expected) {
    size_t i = 0;
    if (!p || !expected) return 0;
    for (; expected[i]; ++i)
        if (p[i] != (uchar_t)(unsigned char)expected[i]) return 0;
    return p[i] == (uchar_t)0;
}

/* ---- llsnskip ----------------------------------------------------------- */

static void test_llsnskip_explicit_skipset(void) {
    lchar_t s[32];
    lchar_t sk[8];

    /* Skip leading 'a' and 'b' characters; stop at 'c'. */
    LUB_ASSERT(lptr_eq(llsnskip(make_ls("aaabbc_rest", s, 32),
                             32, make_ls("ab", sk, 8)), "c_rest"));

    /* First char not in skip-set; pointer unchanged. */
    LUB_ASSERT(lptr_eq(llsnskip(make_ls("xyz", s, 32),
                             32, make_ls("ab", sk, 8)), "xyz"));

    /* All characters in skip-set; pointer reaches null terminator. */
    {   lchar_t *r = llsnskip(make_ls("aaa", s, 32), 32, make_ls("a", sk, 8));
        LUB_ASSERT(r && *r == (lchar_t)0);
    }

    /* Empty source; returns pointer to null terminator. */
    {   lchar_t *r = llsnskip(make_ls("", s, 32), 32, make_ls("ab", sk, 8));
        LUB_ASSERT(r && *r == (lchar_t)0);
    }

    /* Single-character match at start. */
    LUB_ASSERT(lptr_eq(llsnskip(make_ls("xhello", s, 32),
                             32, make_ls("x", sk, 8)), "hello"));
}

static void test_llsnskip_default_whitespace(void) {
    lchar_t s[32];

    /* Leading spaces skipped; stops at 'h'. */
    LUB_ASSERT(lptr_eq(llsnskip(make_ls("   hello", s, 32), 32, NULL), "hello"));

    /* Mixed whitespace (space + tab). */
    {   lchar_t mixed[16];
        mixed[0] = ' '; mixed[1] = '\t'; mixed[2] = 'w'; mixed[3] = 0;
        LUB_ASSERT(lptr_eq(llsnskip(mixed, 16, NULL), "w"));
    }

    /* Empty skip string "" triggers whitespace default. */
    {   lchar_t sk[4] = {0};
        LUB_ASSERT(lptr_eq(llsnskip(make_ls("  hi", s, 32), 32, sk), "hi"));
    }

    /* No leading whitespace; pointer unchanged. */
    LUB_ASSERT(lptr_eq(llsnskip(make_ls("hello", s, 32), 32, NULL), "hello"));

    /* All whitespace; pointer at null terminator. */
    {   lchar_t *r = llsnskip(make_ls("   ", s, 32), 32, NULL);
        LUB_ASSERT(r && *r == (lchar_t)0);
    }

    /* Empty source with default skip. */
    {   lchar_t *r = llsnskip(make_ls("", s, 32), 32, NULL);
        LUB_ASSERT(r && *r == (lchar_t)0);
    }
}

static void test_llsnskip_error_paths(void) {
    /* NULL source returns NULL. */
    LUB_ASSERT(llsnskip(NULL, 32, NULL) == NULL);
}

/* ---- ulsnskip ----------------------------------------------------------- */

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

/* ---- uusnskip ----------------------------------------------------------- */

static void test_uusnskip_explicit_skipset(void) {
    uchar_t s[32];
    uchar_t sk[8];

    /* Skip punctuation; stop at letter. */
    LUB_ASSERT(uptr_eq(uusnskip(make_us("...,text", s, 32),
                             32, make_us(".,", sk, 8)), "text"));

    /* No chars from skip-set at start. */
    LUB_ASSERT(uptr_eq(uusnskip(make_us("text", s, 32),
                             32, make_us(".,", sk, 8)), "text"));

    /* All chars in skip-set. */
    {   uchar_t *r = uusnskip(make_us("...", s, 32), 32, make_us(".", sk, 8));
        LUB_ASSERT(r && *r == (uchar_t)0);
    }

    /* Empty source. */
    {   uchar_t *r = uusnskip(make_us("", s, 32), 32, make_us(".", sk, 8));
        LUB_ASSERT(r && *r == (uchar_t)0);
    }
}

static void test_uusnskip_default_whitespace(void) {
    uchar_t s[32];

    /* Leading spaces. */
    LUB_ASSERT(uptr_eq(uusnskip(make_us("  data", s, 32), 32, NULL), "data"));

    /* No whitespace. */
    LUB_ASSERT(uptr_eq(uusnskip(make_us("data", s, 32), 32, NULL), "data"));

    /* All whitespace. */
    {   uchar_t *r = uusnskip(make_us("   ", s, 32), 32, NULL);
        LUB_ASSERT(r && *r == (uchar_t)0);
    }

    /* Empty source. */
    {   uchar_t *r = uusnskip(make_us("", s, 32), 32, NULL);
        LUB_ASSERT(r && *r == (uchar_t)0);
    }
}

static void test_uusnskip_error_paths(void) {
    /* NULL source returns NULL. */
    LUB_ASSERT(uusnskip(NULL, 32, NULL) == NULL);
}

static void test_null_skip_defaults_whitespace_all_variants(void) {
    lchar_t ls[16];
    uchar_t us[16];

    LUB_ASSERT(lptr_eq(llsnskip(make_ls("   a", ls, 16), 16, NULL), "a"));
    LUB_ASSERT(uptr_eq(ulsnskip(make_us("\t\tb", us, 16), 16, NULL), "b"));
    LUB_ASSERT(uptr_eq(uusnskip(make_us("  c", us, 16), 16, NULL), "c"));
}

/* ---- skip-set loop correctness ------------------------------------------ */

static void test_skipset_first_char_is_stop(void) {
    /* Verify that when s[0] is NOT in the skip-set, s itself is returned
     * immediately (no off-by-one advance). */
    lchar_t ls[8];
    uchar_t us[8];

    lchar_t *lp = llsnskip(make_ls("Xabc", ls, 8), 8,
                            (const lchar_t *)"abc");
    LUB_ASSERT(lp == ls); /* pointer must be the original s, not s+1 */

    uchar_t *up = uusnskip(make_us("Xabc", us, 8), 8,
                            (const uchar_t *)"abc");
    LUB_ASSERT(up == us);
}

static void test_skipset_consecutive_skip_chars(void) {
    /* Mix of skip and non-skip chars interleaved — only leading run skipped. */
    lchar_t ls[16];
    lchar_t *r = llsnskip(make_ls("aabXab", ls, 16), 16,
                           (const lchar_t *)"ab");
    LUB_ASSERT(lptr_eq(r, "Xab"));
}

/* ---- sn bound ----------------------------------------------------------- */

static void test_sn_bound_zero(void) {
    /* sn=0: lcsnlen/ucsnlen will find the null terminator at position 0 only
     * if the very first byte is '\0'.  For a non-empty string this is treated
     * as unterminated and an error pointer is returned. */
    lchar_t ls[8];
    make_ls("hello", ls, 8);
    lchar_t *r = llsnskip(ls, 0, NULL);
    /* Should be an error value (non-NULL error pointer) or NULL. */
    LUB_ASSERT(r == NULL || LUB_PTR_ERR(r, 0));
}

/* ---- entry point -------------------------------------------------------- */

lub_test_result_t run_skip_tests(void) {
    test_result = (lub_test_result_t){0};
    test_llsnskip_explicit_skipset();
    test_llsnskip_default_whitespace();
    test_llsnskip_error_paths();

    test_ulsnskip_explicit_skipset();
    test_ulsnskip_default_whitespace();
    test_ulsnskip_error_paths();

    test_uusnskip_explicit_skipset();
    test_uusnskip_default_whitespace();
    test_uusnskip_error_paths();

    test_null_skip_defaults_whitespace_all_variants();
    test_skipset_first_char_is_stop();
    test_skipset_consecutive_skip_chars();
    test_sn_bound_zero();

    return test_result;
}
