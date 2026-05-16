/**
 * @file test_xmacros.c
 * @brief X-macro alias mapping tests (isx* -> isl* and isu*).
 *
 * @note Compiled twice: -DLUB_X_IS_L for Latin, -DLUB_X_IS_U for Unicode.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#if !defined(LUB_X_IS_L) && !defined(LUB_X_IS_U)
#define LUB_X_IS_L
#endif

#include <stddef.h>

#include "../lubtype.h"
#include "lubtype_test_declarations.h"

static lub_test_result_t test_result;

static xchar_t *make_xstr_local(const char *src, xchar_t *dst, size_t cap) {
	size_t i = 0;
	if (!dst || !cap) return (xchar_t *)NULL;
	for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (xchar_t)(unsigned char)src[i];
	dst[i] = (xchar_t)0;
	return dst;
}

#if defined(LUB_X_IS_L)
#define RESERVED_CONCRETE(s) islreserved((const lchar_t *)(s))
#define QNAME_CONCRETE(s) islqname((const lchar_t *)(s))
#else
#define RESERVED_CONCRETE(s) isureserved((const uchar_t *)(s))
#define QNAME_CONCRETE(s) isuqname((const uchar_t *)(s))
#endif

static void test_reserved_alias_mapping(void) {
	static const char *samples[] = {
		"SELECT", "insert", "VAR_POP", "SELECTX", "my_table", ""
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local(samples[i], buf, 64);
		LUB_ASSERT(isxreserved(buf) == RESERVED_CONCRETE(buf));
	}

	LUB_ASSERT(isxreserved(NULL) == RESERVED_CONCRETE(NULL));
}

static void test_qname_alias_mapping(void) {
	static const char *samples[] = {
		"name", "Name_1", "SELECT", "1bad", "my-name", ""
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local(samples[i], buf, 64);
		LUB_ASSERT(isxqname(buf) == QNAME_CONCRETE(buf));
	}

	LUB_ASSERT(isxqname(NULL) == QNAME_CONCRETE(NULL));
}

#define XMACRO_ALIAS_TESTS(X) \
	X(test_reserved_alias_mapping) \
	X(test_qname_alias_mapping)

lub_test_result_t LUB_PASTE(run_xmacros_tests_, LUB_X)(void) {
	test_result = (lub_test_result_t){0};
#define RUN_TEST(fn) fn();
	XMACRO_ALIAS_TESTS(RUN_TEST)
#undef RUN_TEST

	return test_result;
}
