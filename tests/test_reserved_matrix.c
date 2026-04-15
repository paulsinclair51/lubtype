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

/**
 * @brief Create a local ucstr_t from ASCII string.
 */
static ucstr_t make_ustr_local(const char *src, ustr_t dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (ucstr_t)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (uchar_t)(unsigned char)src[i];
    dst[i] = (uchar_t)0;
    return (ucstr_t)dst;
}

/**
 * @brief Create a local lstr_t from ASCII string.
 */
static lcstr_t make_lstr_local(const char *src, lstr_t dst, size_t cap) {
    size_t i = 0;
    if (!dst || !cap) return (lcstr_t)NULL;
    for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (lchar_t)(unsigned char)src[i];
    dst[i] = (lchar_t)0;
    return (lcstr_t)dst;
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
    uchar_t buf[64];
    size_t i;

    for (i = 0; i < sizeof(positives) / sizeof(positives[0]); ++i) {
        int expected = oracle_isureserved_ascii_local(positives[i]);
        int actual = isureserved(make_ustr_local(positives[i], buf, 64));
        assert(expected == 1);
        assert(actual == expected);
    }

    for (i = 0; i < sizeof(negatives) / sizeof(negatives[0]); ++i) {
        int expected = oracle_isureserved_ascii_local(negatives[i]);
        int actual = isureserved(make_ustr_local(negatives[i], buf, 64));
        assert(expected == 0);
        assert(actual == expected);
    }
        // Null/empty/error edge cases
        assert(!isureserved(NULL));
        buf[0] = 0;
        assert(!isureserved(buf));
}

static void test_matrix_consistency_cmp_pfx_sfx(void) {
    lchar_t l1[64], l2[64], llhs[64], lrhs[64];
    uchar_t u1[64], u2[64], ulhs[64], urhs[64];

    make_lstr_local("Alpha", l1, 64);
    make_lstr_local("alpha", l2, 64);
    make_ustr_local("Alpha", u1, 64);
    make_ustr_local("alpha", u2, 64);

    assert(llsncmp(l1, l2, 63) != 0);
    assert(lusncmp(l1, u2, 63) != 0);
    assert(ulsncmp(u1, l2, 63) != 0);
    assert(uusncmp(u1, u2, 63) != 0);

    assert(llsnCMP(l1, l2, 63) == 0);
    assert(lusnCMP(l1, u2, 63) == 0);
    assert(ulsnCMP(u1, l2, 63) == 0);
    assert(uusnCMP(u1, u2, 63) == 0);

    assert(llscmp(l1, l2) == llsncmp(l1, l2, MAX_LSTRLEN));
    assert(luscmp(l1, u2) == lusncmp(l1, u2, MAX_USTRLEN));
    assert(ulscmp(u1, l2) == ulsncmp(u1, l2, MAX_USTRLEN));
    assert(uuscmp(u1, u2) == uusncmp(u1, u2, MAX_USTRLEN));

    assert(llsCMP(l1, l2) == llsnCMP(l1, l2, MAX_LSTRLEN));
    assert(lusCMP(l1, u2) == lusnCMP(l1, u2, MAX_USTRLEN));
    assert(ulsCMP(u1, l2) == ulsnCMP(u1, l2, MAX_USTRLEN));
    assert(uusCMP(u1, u2) == uusnCMP(u1, u2, MAX_USTRLEN));

    make_lstr_local("PrefixBodySuffix", llhs, 64);
    make_lstr_local("prefix", lrhs, 64);
    make_ustr_local("PrefixBodySuffix", ulhs, 64);
    make_ustr_local("prefix", urhs, 64);

    assert(llsnpfx(llhs, lrhs, 63) != 0);
    assert(lusnpfx(llhs, urhs, 63) != 0);
    assert(ulsnpfx(ulhs, lrhs, 63) != 0);
    assert(uusnpfx(ulhs, urhs, 63) != 0);

    assert(llsnPFX(llhs, lrhs, 63) == 0);
    assert(lusnPFX(llhs, urhs, 63) == 0);
    assert(ulsnPFX(ulhs, lrhs, 63) == 0);
    assert(uusnPFX(ulhs, urhs, 63) == 0);

    assert(llspfx(llhs, lrhs) == llsnpfx(llhs, lrhs, MAX_LSTRLEN));
    assert(luspfx(llhs, urhs) == lusnpfx(llhs, urhs, MAX_USTRLEN));
    assert(ulspfx(ulhs, lrhs) == ulsnpfx(ulhs, lrhs, MAX_USTRLEN));
    assert(uuspfx(ulhs, urhs) == uusnpfx(ulhs, urhs, MAX_USTRLEN));

    assert(llsPFX(llhs, lrhs) == llsnPFX(llhs, lrhs, MAX_LSTRLEN));
    assert(lusPFX(llhs, urhs) == lusnPFX(llhs, urhs, MAX_USTRLEN));
    assert(ulsPFX(ulhs, lrhs) == ulsnPFX(ulhs, lrhs, MAX_USTRLEN));
    assert(uusPFX(ulhs, urhs) == uusnPFX(ulhs, urhs, MAX_USTRLEN));

    make_lstr_local("PrefixBodySuffix", llhs, 64);
    make_lstr_local("suffix", lrhs, 64);
    make_ustr_local("PrefixBodySuffix", ulhs, 64);
    make_ustr_local("suffix", urhs, 64);

    assert(llsnsfx(llhs, lrhs, 63) != 0);
    assert(lusnsfx(llhs, urhs, 63) != 0);
    assert(ulsnsfx(ulhs, lrhs, 63) != 0);
    assert(uusnsfx(ulhs, urhs, 63) != 0);

    assert(llsnSFX(llhs, lrhs, 63) == 0);
    assert(lusnSFX(llhs, urhs, 63) == 0);
    assert(ulsnSFX(ulhs, lrhs, 63) == 0);
    assert(uusnSFX(ulhs, urhs, 63) == 0);

    assert(llssfx(llhs, lrhs) == llsnsfx(llhs, lrhs, MAX_LSTRLEN));
    assert(lussfx(llhs, urhs) == lusnsfx(llhs, urhs, MAX_USTRLEN));
    assert(ulssfx(ulhs, lrhs) == ulsnsfx(ulhs, lrhs, MAX_USTRLEN));
    assert(uussfx(ulhs, urhs) == uusnsfx(ulhs, urhs, MAX_USTRLEN));

    assert(llsSFX(llhs, lrhs) == llsnSFX(llhs, lrhs, MAX_LSTRLEN));
    assert(lusSFX(llhs, urhs) == lusnSFX(llhs, urhs, MAX_USTRLEN));
    assert(ulsSFX(ulhs, lrhs) == ulsnSFX(ulhs, lrhs, MAX_USTRLEN));
    assert(uusSFX(ulhs, urhs) == uusnSFX(ulhs, urhs, MAX_USTRLEN));
}

void run_reserved_matrix_tests(void) {
    test_isureserved_oracle_crosscheck();
    test_matrix_consistency_cmp_pfx_sfx();

}
