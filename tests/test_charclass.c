/**
 * @file test_charclass.c
 * @brief Polymorphic (x) macro tests for character classification
 *        functions in lubtype.h.
 *
 * @note Compiled twice: -DLUB_X_IS_L for Latin, -DLUB_X_IS_U for Unicode.
 *       Each test file needs `static lub_test_result_t test_result;`.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

#include "lubtype_test_declarations.h"

static xchar_t *make_xstr_local_charclass(const char *src, xchar_t *dst, size_t cap) {
	size_t i = 0;
	if (!dst || !cap) return (xchar_t *)NULL;
	for (; src && src[i] && i + 1 < cap; ++i) dst[i] = (xchar_t)(unsigned char)src[i];
	dst[i] = (xchar_t)0;
	return dst;
}

static void test_alpha_alias_mapping(void) {
	static const char *samples[] = {
		"abc", "ABC", "aBc", "123", "a1b", ""
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_charclass(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxalpha(buf[0]) == islalpha(buf[0]));
#else
		LUB_ASSERT(isxalpha(buf[0]) == isualpha(buf[0]));
#endif
	}
}

static void test_digit_alias_mapping(void) {
	static const char *samples[] = {
		"0123456789", "abc", "a1b", ""
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_charclass(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxdigit(buf[0]) == isldigit(buf[0]));
#else
		LUB_ASSERT(isxdigit(buf[0]) == isudigit(buf[0]));
#endif
	}
}

static void test_alnum_alias_mapping(void) {
	static const char *samples[] = {
		"abc", "123", "a1b", "!@#", ""
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_charclass(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxalnum(buf[0]) == islalnum(buf[0]));
#else
		LUB_ASSERT(isxalnum(buf[0]) == isualnum(buf[0]));
#endif
	}
}

static void test_latin_alias_mapping(void) {
	static const char *samples[] = {
		"a", "Z", "5", " ", "\t"
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_charclass(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxlatin(buf[0]) == isllatin(buf[0]));
#else
		LUB_ASSERT(isxlatin(buf[0]) == isulatin(buf[0]));
#endif
	}
}

static void test_name1c_alias_mapping(void) {
	static const char *samples[] = {
		"a", "Z", "_", "5", " ", ""
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_charclass(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxname1c(buf[0]) == islname1c(buf[0]));
#else
		LUB_ASSERT(isxname1c(buf[0]) == isuname1c(buf[0]));
#endif
	}
}

static void test_namec_alias_mapping(void) {
	static const char *samples[] = {
		"a", "Z", "_", "5", " ", ""
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_charclass(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxnamec(buf[0]) == islnamec(buf[0]));
#else
		LUB_ASSERT(isxnamec(buf[0]) == isunamec(buf[0]));
#endif
	}
}

static void test_upper_alias_mapping(void) {
	static const char *samples[] = {
		"A", "Z", "a", "z", "5"
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_charclass(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxupper(buf[0]) == islupper(buf[0]));
#else
		LUB_ASSERT(isxupper(buf[0]) == isuupper(buf[0]));
#endif
	}
}

static void test_lower_alias_mapping(void) {
	static const char *samples[] = {
		"A", "Z", "a", "z", "5"
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_charclass(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxlower(buf[0]) == isllower(buf[0]));
#else
		LUB_ASSERT(isxlower(buf[0]) == isulower(buf[0]));
#endif
	}
}

static void test_cntrl_alias_mapping(void) {
	static const char *samples[] = {
		"\t", "\n", "\r", "a", " "
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_charclass(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxcntrl(buf[0]) == islcntrl(buf[0]));
#else
		LUB_ASSERT(isxcntrl(buf[0]) == isucntrl(buf[0]));
#endif
	}
}

static void test_print_alias_mapping(void) {
	static const char *samples[] = {
		"a", "Z", "5", " ", "!"
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_charclass(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxprint(buf[0]) == islprint(buf[0]));
#else
		LUB_ASSERT(isxprint(buf[0]) == isuprint(buf[0]));
#endif
	}
}

static void test_graph_alias_mapping(void) {
	static const char *samples[] = {
		"a", "Z", "5", "!", " "
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_charclass(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxgraph(buf[0]) == islgraph(buf[0]));
#else
		LUB_ASSERT(isxgraph(buf[0]) == isugraph(buf[0]));
#endif
	}
}

static void test_punct_alias_mapping(void) {
	static const char *samples[] = {
		"!", ".", ",", "a", "5"
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_charclass(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxpunct(buf[0]) == islpunct(buf[0]));
#else
		LUB_ASSERT(isxpunct(buf[0]) == isupunct(buf[0]));
#endif
	}
}

static void test_blank_alias_mapping(void) {
	static const char *samples[] = {
		" ", "\t", "a", "5"
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_charclass(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxblank(buf[0]) == islblank(buf[0]));
#else
		LUB_ASSERT(isxblank(buf[0]) == isublank(buf[0]));
#endif
	}
}

static void test_space_alias_mapping(void) {
	static const char *samples[] = {
		" ", "\t", "\n", "a", "5"
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_charclass(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxspace(buf[0]) == islspace(buf[0]));
#else
		LUB_ASSERT(isxspace(buf[0]) == isuspace(buf[0]));
#endif
	}
}

static void test_hex_alias_mapping(void) {
	static const char *samples[] = {
		"0", "9", "a", "f", "A", "F", "g", "x"
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local_charclass(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxhex(buf[0]) == islhex(buf[0]));
#else
		LUB_ASSERT(isxhex(buf[0]) == isuhex(buf[0]));
#endif
	}
}

/**
 * @brief Run tests for x-macro character classification and
 *        conversion functions.
 *
 * Tests isxalpha, isxupper, isxlower, isxdigit, isxalnum, xxtoupper, xxtolower.
 * Also checks edge cases for null and non-alphabetic input.
 * This file is compiled twice: once with -DLUB_X_IS_L and once
 * with -DLUB_X_IS_U.
 */

lub_test_result_t LUB_PASTE(run_charclass_tests_, LUB_X)(int inject_faults)
{ test_result = (lub_test_result_t){0};
  // Test: isxalpha
  LUB_ASSERT(isxalpha((xchar_t)'A'));
  LUB_ASSERT(isxalpha((xchar_t)'z'));
  LUB_ASSERT(!isxalpha((xchar_t)'1'));
  // Test: isxupper
  LUB_ASSERT(isxupper((xchar_t)'Z'));
  LUB_ASSERT(!isxupper((xchar_t)'a'));
  // Test: isxlower
  LUB_ASSERT(isxlower((xchar_t)'a'));
  LUB_ASSERT(!isxlower((xchar_t)'Z'));
  // Test: isxdigit
  LUB_ASSERT(isxdigit((xchar_t)'5'));
  LUB_ASSERT(!isxdigit((xchar_t)'A'));
  // Test: isxalnum
  LUB_ASSERT(isxalnum((xchar_t)'9'));
  LUB_ASSERT(isxalnum((xchar_t)'B'));
  LUB_ASSERT(!isxalnum((xchar_t)'!'));
  if (inject_faults) LUB_ASSERT(0 && "Inject assertion fail 1");
  // Additional x-macro character-class aliases.
  LUB_ASSERT(isxblank((xchar_t)' '));
  LUB_ASSERT(!isxblank((xchar_t)'A'));
  LUB_ASSERT(isxcntrl((xchar_t)'\n'));
  LUB_ASSERT(!isxcntrl((xchar_t)'A'));
  LUB_ASSERT(isxprint((xchar_t)'A'));
  LUB_ASSERT(!isxprint((xchar_t)'\n'));
  LUB_ASSERT(isxpunct((xchar_t)'!'));
  LUB_ASSERT(!isxpunct((xchar_t)'A'));
  LUB_ASSERT(isxspace((xchar_t)' '));
  LUB_ASSERT(!isxspace((xchar_t)'A'));
  LUB_ASSERT(isxgraph((xchar_t)'!'));
  LUB_ASSERT(!isxgraph((xchar_t)' '));
  LUB_ASSERT(isxhex((xchar_t)'F'));
  LUB_ASSERT(!isxhex((xchar_t)'G'));
	LUB_ASSERT(ixhex((xchar_t)'f') == 15);
	LUB_ASSERT(ixhex((xchar_t)'g') == -1);
  LUB_ASSERT(isxlatin((xchar_t)'A'));
  LUB_ASSERT(isxname1c((xchar_t)'A'));
  LUB_ASSERT(!isxname1c((xchar_t)'1'));
  LUB_ASSERT(isxnamec((xchar_t)'_'));
  LUB_ASSERT(isxnamec((xchar_t)'1'));
  LUB_ASSERT(!isxnamec((xchar_t)'-'));
  if (inject_faults) LUB_ASSERT(0 && "Inject assertion fail 2");
  // Test: xxtoupper, xxtolower
  LUB_ASSERT(xxtoupper((xchar_t)'a') == (xchar_t)'A');
  LUB_ASSERT(xxtolower((xchar_t)'Z') == (xchar_t)'z');
  // Null/zero edge cases
  LUB_ASSERT(!isxalpha((xchar_t)0));
  LUB_ASSERT(!isxupper((xchar_t)0));
  LUB_ASSERT(!isxlower((xchar_t)0));
  LUB_ASSERT(!isxdigit((xchar_t)0));
  LUB_ASSERT(!isxalnum((xchar_t)0));
  LUB_ASSERT(xxtoupper((xchar_t)0) == (xchar_t)0);
  LUB_ASSERT(xxtolower((xchar_t)0) == (xchar_t)0);
  // Non-alphabetic character passthrough
  LUB_ASSERT(xxtoupper((xchar_t)'!') == (xchar_t)'!');
  LUB_ASSERT(xxtolower((xchar_t)'!') == (xchar_t)'!');

  /* Run x-macro alias mapping tests for character classification */
  test_alpha_alias_mapping();
  test_digit_alias_mapping();
  test_alnum_alias_mapping();
  test_latin_alias_mapping();
  test_name1c_alias_mapping();
  test_namec_alias_mapping();
  test_upper_alias_mapping();
  test_lower_alias_mapping();
  test_cntrl_alias_mapping();
  test_print_alias_mapping();
  test_graph_alias_mapping();
  test_punct_alias_mapping();
  test_blank_alias_mapping();
  test_space_alias_mapping();
  test_hex_alias_mapping();

  return test_result;
}
