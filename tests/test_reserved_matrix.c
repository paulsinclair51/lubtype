/**
 * @file test_reserved_matrix.c
 * @brief Tests for reserved word matrix and related logic in lubtype.h.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include <assert.h>
#include <stddef.h>

#include "../lubtype.h"
#include "test_declarations.h"

/**
 * @brief Create a local ucstr_t from ASCII string.
 */
static uchar_t *make_ustr_local(const char *src, uchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (uchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (uchar_t)(unsigned char)src[i];
    dst[i] = (uchar_t)0;
    return dst;
}

/**
 * @brief Create a local lstr_t from ASCII string.
 */
static lchar_t *make_lstr_local(const char *src, lchar_t *dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (lchar_t *)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (lchar_t)(unsigned char)src[i];
    dst[i] = (lchar_t)0;
    return dst;
}

/**
 * @brief Convert ASCII char to uppercase (for case-insensitive compare).
 */
static char up_ascii_local(char c) {
    return (c >= 'a' && c <= 'z') ? (char)(c - ('a' - 'A')) : c;
}

/**
 * @brief Case-insensitive ASCII string comparison.
 */
static int eq_ascii_ci_local(const char *a, const char *b) {
    size_t i = 0;
    if (!a || !b) return 0;
    while (a[i] && b[i]) {
        if (up_ascii_local(a[i]) != up_ascii_local(b[i])) return 0;
        ++i;
    }
    return a[i] == b[i];
}

/**
 * @brief Simulate Oracle reserved word check for ASCII strings.
 */
static int oracle_isureserved_ascii_local(const char *s) {
    static const char *reserved[] = {
        "SELECT", "INSERT", "UPDATE", "DELETE", "CREATE", "DROP",
        "INTEGER", "VARCHAR", "TIMESTAMP", "COUNT", "SUM", "AVG",
        "ABS", "SQRT", "CASESPECIFIC", "CASE_N", "CHAR2HEXINT",
        "CHARACTER", "CHARACTERS", "CHARACTER_LENGTH", "CHARS",
        "CHAR_LENGTH", "NONE", "NORMALIZE", "NOT", "NOWAIT",
        "NO_BEFORE_JOURNAL", "NO_INLINE", "NO_PRIMARY_INDEX",
        "NO_SCROLL", "NO_UNDO", "PERCENTILE_CONT", "PERCENTILE_DISC",
        "PERCENT_RANK", "ROW", "ROWS", "ROW_NUMBER", "VARBYTE",
        "VARCHAR", "VARGRAPHIC", "VARIANT_TYPE", "VARYING", "VAR_POP",
        "VAR_SAMP", "WORK", "YEAR", "ZONE"
    };
    size_t i;

    if (!s || !*s) return 0;
    for (i = 0; i < sizeof(reserved) / sizeof(reserved[0]); ++i)
        if (eq_ascii_ci_local(s, reserved[i])) return 1;
    return 0;
}

static void test_isureserved_oracle_crosscheck(void) {
    static const char *positives[] = {
        "SELECT", "insert", "UpDaTe", "CHAR_LENGTH", "character_length",
        "CASE_N", "casespecific", "NO_INLINE", "none", "Nowait",
        "PERCENTILE_CONT", "percent_rank", "ROWS", "row_number",
        "VARBYTE", "variant_type", "VAR_POP", "var_samp"
    };
    static const char *negatives[] = {
        "SELECTX", "_SELECT", "CASE__N", "NO-UNDO", "PERCENTRANK",
        "ROW__NUMBER", "VARPOP", "VAR__SAMP", "USERTABLE", "my_table"
    };
    lchar_t lbuf[64];
    uchar_t buf[64];
    size_t i;

    for (i = 0; i < sizeof(positives) / sizeof(positives[0]); ++i) {
        int expected = oracle_isureserved_ascii_local(positives[i]);
        int actual = isureserved(make_ustr_local(positives[i], buf, 64));
        int lactual = islreserved(make_lstr_local(positives[i], lbuf, 64));
        assert(expected == 1);
        assert(actual == expected);
        assert(lactual == expected);
    }

    for (i = 0; i < sizeof(negatives) / sizeof(negatives[0]); ++i) {
        int expected = oracle_isureserved_ascii_local(negatives[i]);
        int actual = isureserved(make_ustr_local(negatives[i], buf, 64));
        int lactual = islreserved(make_lstr_local(negatives[i], lbuf, 64));
        assert(expected == 0);
        assert(actual == expected);
        assert(lactual == expected);
    }
        // Null/empty are invalid-name errors in current API.
        assert(isureserved(NULL) == (int)LUB_NAME_INVALID);
        assert(islreserved(NULL) == (int)LUB_NAME_INVALID);
        buf[0] = 0;
        lbuf[0] = 0;
        assert(isureserved(buf) == (int)LUB_NAME_INVALID);
        assert(islreserved(lbuf) == (int)LUB_NAME_INVALID);
}

static void test_matrix_consistency_cmp_pfx_sfx(void) {
    lchar_t l1[64], l2[64], llhs[64], lrhs[64];
    uchar_t u1[64], u2[64], ulhs[64], urhs[64];

    make_lstr_local("Alpha", l1, 64);
    make_lstr_local("alpha", l2, 64);
    make_ustr_local("Alpha", u1, 64);
    make_ustr_local("alpha", u2, 64);

    assert(llsnncmp(l1, 63, l2, 63) != 0);
    assert(lusnncmp(l1, 63, u2, 63) != 0);
    assert(ulsnncmp(u1, 63, l2, 63) != 0);
    assert(uusnncmp(u1, 63, u2, 63) != 0);

    assert(llsnnCMP(l1, 63, l2, 63) == 0);
    assert(lusnnCMP(l1, 63, u2, 63) == 0);
    assert(ulsnnCMP(u1, 63, l2, 63) == 0);
    assert(uusnnCMP(u1, 63, u2, 63) == 0);

    assert(llsnncmp(l1, LUB_MAX_LSTRLEN, l2, LUB_MAX_LSTRLEN) == llsnncmp(l1, LUB_MAX_LSTRLEN, l2, LUB_MAX_LSTRLEN));
    assert(lusnncmp(l1, LUB_MAX_USTRLEN, u2, LUB_MAX_USTRLEN) == lusnncmp(l1, LUB_MAX_USTRLEN, u2, LUB_MAX_USTRLEN));
    assert(ulsnncmp(u1, LUB_MAX_USTRLEN, l2, LUB_MAX_USTRLEN) == ulsnncmp(u1, LUB_MAX_USTRLEN, l2, LUB_MAX_USTRLEN));
    assert(uusnncmp(u1, LUB_MAX_USTRLEN, u2, LUB_MAX_USTRLEN) == uusnncmp(u1, LUB_MAX_USTRLEN, u2, LUB_MAX_USTRLEN));

    assert(llsnnCMP(l1, LUB_MAX_LSTRLEN, l2, LUB_MAX_LSTRLEN) == llsnnCMP(l1, LUB_MAX_LSTRLEN, l2, LUB_MAX_LSTRLEN));
    assert(lusnnCMP(l1, LUB_MAX_USTRLEN, u2, LUB_MAX_USTRLEN) == lusnnCMP(l1, LUB_MAX_USTRLEN, u2, LUB_MAX_USTRLEN));
    assert(ulsnnCMP(u1, LUB_MAX_USTRLEN, l2, LUB_MAX_USTRLEN) == ulsnnCMP(u1, LUB_MAX_USTRLEN, l2, LUB_MAX_USTRLEN));
    assert(uusnnCMP(u1, LUB_MAX_USTRLEN, u2, LUB_MAX_USTRLEN) == uusnnCMP(u1, LUB_MAX_USTRLEN, u2, LUB_MAX_USTRLEN));

    make_lstr_local("PrefixBodySuffix", llhs, 64);
    make_lstr_local("prefix", lrhs, 64);
    make_ustr_local("PrefixBodySuffix", ulhs, 64);
    make_ustr_local("prefix", urhs, 64);

    assert(llsnnpfxcmp(llhs, 63, lrhs, 63) != 0);
    assert(lusnnpfxcmp(llhs, 63, urhs, 63) != 0);
    assert(ulsnnpfxcmp(ulhs, 63, lrhs, 63) != 0);
    assert(uusnnpfxcmp(ulhs, 63, urhs, 63) != 0);

    assert(llsnnPFXCMP(llhs, 63, lrhs, 63) == 0);
    assert(lusnnPFXCMP(llhs, 63, urhs, 63) == 0);
    assert(ulsnnPFXCMP(ulhs, 63, lrhs, 63) == 0);
    assert(uusnnPFXCMP(ulhs, 63, urhs, 63) == 0);

    make_lstr_local("PrefixBodySuffix", llhs, 64);
    make_lstr_local("suffix", lrhs, 64);
    make_ustr_local("PrefixBodySuffix", ulhs, 64);
    make_ustr_local("suffix", urhs, 64);

    assert(llsnnsfxcmp(llhs, 63, lrhs, 63) != 0);
    assert(lusnnsfxcmp(llhs, 63, urhs, 63) != 0);
    assert(ulsnnsfxcmp(ulhs, 63, lrhs, 63) != 0);
    assert(uusnnsfxcmp(ulhs, 63, urhs, 63) != 0);

    assert(llsnnSFXCMP(llhs, 63, lrhs, 63) == 0);
    assert(lusnnSFXCMP(llhs, 63, urhs, 63) == 0);
    assert(ulsnnSFXCMP(ulhs, 63, lrhs, 63) == 0);
    assert(uusnnSFXCMP(ulhs, 63, urhs, 63) == 0);
}

void run_reserved_matrix_tests(void) {
    test_isureserved_oracle_crosscheck();
    test_matrix_consistency_cmp_pfx_sfx();

}
