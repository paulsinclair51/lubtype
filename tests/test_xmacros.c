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

static void test_alpha_alias_mapping(void) {
	static const char *samples[] = {
		"abc", "ABC", "aBc", "123", "a1b", ""
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local(samples[i], buf, 64);
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
		make_xstr_local(samples[i], buf, 64);
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
		make_xstr_local(samples[i], buf, 64);
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
		make_xstr_local(samples[i], buf, 64);
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
		make_xstr_local(samples[i], buf, 64);
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
		make_xstr_local(samples[i], buf, 64);
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
		make_xstr_local(samples[i], buf, 64);
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
		make_xstr_local(samples[i], buf, 64);
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
		make_xstr_local(samples[i], buf, 64);
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
		make_xstr_local(samples[i], buf, 64);
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
		make_xstr_local(samples[i], buf, 64);
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
		make_xstr_local(samples[i], buf, 64);
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
		make_xstr_local(samples[i], buf, 64);
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
		make_xstr_local(samples[i], buf, 64);
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
		make_xstr_local(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxhex(buf[0]) == islhex(buf[0]));
#else
		LUB_ASSERT(isxhex(buf[0]) == isuhex(buf[0]));
#endif
	}
}

static void test_nlatinstr_alias_mapping(void) {
	static const char *samples[] = {
		"abc", "123abc", "Latin_1", ""
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local(samples[i], buf, 64);
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
		make_xstr_local(samples[i], buf, 64);
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
		make_xstr_local(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxnhexstr(buf, 64) == islnhexstr(buf, 64));
#else
		LUB_ASSERT(isxnhexstr(buf, 64) == isunhexstr(buf, 64));
#endif
	}
}

static void test_truncstr_alias_mapping(void) {
	static const char *samples[] = {
		"", "R", "Rtext", "L", "Ctext", "B", "invalid"
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		if (i >= 6) break;  // Skip invalid ones for this test
		make_xstr_local(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxtruncstr(buf) == isltruncstr(buf));
#else
		LUB_ASSERT(isxtruncstr(buf) == isutruncstr(buf));
#endif
	}
}

static void test_trimstr_alias_mapping(void) {
	static const char *samples[] = {
		"", "L", "L ", "R", "R ", "B", "B ", "L "
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxtrimstr(buf) == isltrimstr(buf));
#else
		LUB_ASSERT(isxtrimstr(buf) == isutrimstr(buf));
#endif
	}
}

static void test_padstr_alias_mapping(void) {
	static const char *samples[] = {
		"", "L", "L ", "R", "R ", "B", "B "
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxpadstr(buf) == islpadstr(buf));
#else
		LUB_ASSERT(isxpadstr(buf) == isupadstr(buf));
#endif
	}
}

static void test_needlestr_alias_mapping(void) {
	static const char *samples[] = {
		"", "needle", "a,b,c", "text"
	};
	xchar_t buf[64];
	size_t i;

	for (i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
		make_xstr_local(samples[i], buf, 64);
#if defined(LUB_X_IS_L)
		LUB_ASSERT(isxneedlestr(buf) == islneedlestr(buf));
#else
		LUB_ASSERT(isxneedlestr(buf) == isuneedlestr(buf));
#endif
	}
}

#define XMACRO_ALIAS_TESTS(X) \
	X(test_reserved_alias_mapping) \
	X(test_qname_alias_mapping) \
	X(test_alpha_alias_mapping) \
	X(test_digit_alias_mapping) \
	X(test_alnum_alias_mapping) \
	X(test_latin_alias_mapping) \
	X(test_name1c_alias_mapping) \
	X(test_namec_alias_mapping) \
	X(test_upper_alias_mapping) \
	X(test_lower_alias_mapping) \
	X(test_cntrl_alias_mapping) \
	X(test_print_alias_mapping) \
	X(test_graph_alias_mapping) \
	X(test_punct_alias_mapping) \
	X(test_blank_alias_mapping) \
	X(test_space_alias_mapping) \
	X(test_hex_alias_mapping) \
	X(test_nlatinstr_alias_mapping) \
	X(test_nalphastr_alias_mapping) \
	X(test_nhexstr_alias_mapping) \
	X(test_truncstr_alias_mapping) \
	X(test_trimstr_alias_mapping) \
	X(test_padstr_alias_mapping) \
	X(test_needlestr_alias_mapping)

lub_test_result_t LUB_PASTE(run_xmacros_tests_, LUB_X)(void) {
	test_result = (lub_test_result_t){0};
#define RUN_TEST(fn) fn();
	XMACRO_ALIAS_TESTS(RUN_TEST)
#undef RUN_TEST

	return test_result;
}
