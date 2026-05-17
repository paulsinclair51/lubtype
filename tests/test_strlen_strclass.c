/**
 * @file test_strlen_strclass.c
 * @brief X-macro tests for string length and classification functions
 *        in lubtype.h.
 *
 * @note Compiled twice: once with -DLUB_X_IS_L for Latin (lchar_t)
 *       and once with -DLUB_X_IS_U for Unicode (uchar_t). Each
 *       compilation produces a separate object file and function:
 *       run_strlen_strclass_tests_l and run_strlen_strclass_tests_u.
 *       Requires `static lub_test_result_t test_result;` at file scope.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#if !defined(LUB_X_IS_L) && !defined(LUB_X_IS_U)
#define LUB_X_IS_L
#endif

#include <assert.h>
#include "../lubtype.h"
#include "lubtype_test_declarations.h"
#include <stdio.h>

static lub_test_result_t test_result;

static xchar_t *make_xstr_local_strlen(const char *src, xchar_t *dst, size_t cap) {
	size_t i = 0;
	if (!dst || !cap) return (xchar_t *)NULL;
	for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (xchar_t)(unsigned char)src[i];
	dst[i] = (xchar_t)0;
	return dst;
}

static void test_nlatinstr_alias_mapping(void) {
	static const char *samples[] = {
		"abc", "123abc", "Latin_1", ""
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_strlen(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxnlatinstr(buf, 64) == islnlatinstr(buf, 64));
#else
		LUB_ASSERT(isxnlatinstr(buf, 64) == isunlatinstr(buf, 64));
#endif
	}
}

static void test_nalphastr_alias_mapping(void) {
	static const char *samples[] = {
		"abc", "abcABC", "123", "abc123"
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_strlen(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxnalphastr(buf, 64) == islnalphastr(buf, 64));
#else
		LUB_ASSERT(isxnalphastr(buf, 64) == isunalphastr(buf, 64));
#endif
	}
}

static void test_nhexstr_alias_mapping(void) {
	static const char *samples[] = {
		"0123456789abcdef", "ABCDEF", "0xg", ""
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_strlen(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxnhexstr(buf, 64) == islnhexstr(buf, 64));
#else
		LUB_ASSERT(isxnhexstr(buf, 64) == isunhexstr(buf, 64));
#endif
	}
}

/**
 * @brief Run tests for string length and classification functions.
 *
 * Tests xcsnlen and string classification helpers for correct
 * behavior and edge cases. Compiled twice for Latin (lchar_t)
 * and Unicode (uchar_t).
 */
lub_test_result_t LUB_PASTE(run_strlen_strclass_tests_, LUB_X)(void) {
    test_result = (lub_test_result_t){0};
    xchar_t xstr[16] = {'a', 'b', 'c', '\0'};
    // Test: bounded length API.
    LUB_ASSERT(xcsnlen(xstr, 100) == 3);
    // Missing terminator within bound yields unterminated error.
    LUB_ASSERT(xcsnlen(xstr, 2) == (size_t)LUB_UNTERMINATED);
    // Null/empty edge cases.
    LUB_ASSERT(xcsnlen(NULL, 10) == 0);
    xstr[0] = 0;
    LUB_ASSERT(xcsnlen(xstr, 100) == 0);

    // String classification helpers.
    xchar_t alpha_x[8] = {'A', 'b', 'C', '\0'};
    LUB_ASSERT(isxnalphastr(alpha_x, 3));
    // Latin chars pass the latin check for both L and U variants.
    xchar_t latin_x[8] = {'A', 'B', 'Z', '\0'};
    LUB_ASSERT(isxnlatinstr(latin_x, 3));

    // Hex-digit string checks.
    xchar_t hex_x[8] = {'A', 'B', 'C', '1', '2', '3', '\0'};
    LUB_ASSERT(isxnhexstr(hex_x, 6));
    // Null/empty edges for hex classifiers.
    LUB_ASSERT(isxnhexstr(NULL, 3));
    hex_x[0] = 0;
    LUB_ASSERT(isxnhexstr(hex_x, 0));

#if defined(LUB_X_IS_U)
    // Unicode-only: reserved name check and non-Latin char test.
    uchar_t not_reserved[8] = {L'A', 0};
    LUB_ASSERT(!isureserved(not_reserved));
    uchar_t non_latin_u[8] = {0x0100, 0};
    LUB_ASSERT(!isunlatinstr(non_latin_u, 1));
#endif

    /* Run x-macro alias mapping tests for string classification */
    test_nlatinstr_alias_mapping();
    test_nalphastr_alias_mapping();
    test_nhexstr_alias_mapping();

    return test_result;
}
