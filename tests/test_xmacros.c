/**
 * @file test_xmacros.c
 * @brief X-macro alias mapping tests (isx* -> isl* and isu*).
 *
 * @note Compiled twice: -DLUB_X_IS_L for Latin, -DLUB_X_IS_U for Unicode.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include "lubtype_test_declarations.h"

static xchar_t *make_xstr_local(const char *src, xchar_t *dst, size_t cap) {
	size_t i = 0;
	if (!dst || !cap) return (xchar_t *)NULL;
	for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (xchar_t)(unsigned char)src[i];
	dst[i] = (xchar_t)0;
	return dst;
}

static void test_reserved_alias_mapping(void) {
	static const char *samples[] = {
		"SELECT", "insert", "VAR_POP", "SELECTX", "my_table"
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local(samples[i], buf, 64);
		LUB_ASSERT(isxreserved(buf) == (i < 3 ? 1 : 0));
	}

	make_xstr_local("", buf, 64);
	LUB_ASSERT(LUB_INT_ERR(isxreserved(buf), LUB_NAME_INVALID));
	LUB_ASSERT(LUB_INT_ERR(isxreserved(NULL), LUB_NAME_INVALID));
}

static void test_qname_alias_mapping(void) {
	static const char *samples[] = {
		"name", "Name_1", "SELECT", "1bad", "my-name"
	};
	static const int expected[] = { 1, 1, 1, 1, 0 };
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local(samples[i], buf, 64);
		LUB_ASSERT(isxqname(buf) == expected[i]);
	}

	make_xstr_local("", buf, 64);
	LUB_ASSERT(LUB_INT_ERR(isxqname(buf), LUB_NAME_INVALID));
	LUB_ASSERT(LUB_INT_ERR(isxqname(NULL), LUB_NAME_INVALID));
}

#define XMACRO_ALIAS_TESTS(X) \
	X(test_reserved_alias_mapping) \
	X(test_qname_alias_mapping)

lub_test_result_t LUB_PASTE(run_xmacros_tests_, LUB_X)(int inject_faults) {
	test_result = (lub_test_result_t){0};
#define RUN_TEST(fn) fn();
	XMACRO_ALIAS_TESTS(RUN_TEST)
#undef RUN_TEST

	return test_result;
}
