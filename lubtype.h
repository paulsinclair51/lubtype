/**
 * @file lubtype.h
 */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>

#ifdef __cplusplus
extern "C" {
#endif

// Versioning

// Library version major, minor, patch.
#define LUB_VERSION_MAJOR 1
#define LUB_VERSION_MINOR 0
#define LUB_VERSION_PATCH 0

// Define version helper macros (undefined after versioning set).
#define LUB_STRINGIFY2(x) #x
#define LUB_STRINGIFY(x) LUB_STRINGIFY2(x)

// Library version string in "major.minor.patch" format.
#define LUB_VERSION \
    LUB_STRINGIFY(LUB_VERSION_MAJOR) "." \
    LUB_STRINGIFY(LUB_VERSION_MINOR) "." \
    LUB_STRINGIFY(LUB_VERSION_PATCH)

// Library version as an integer for comparisons.
#define LUB_VERSION_NUM \
    ((int)((LUB_VERSION_MAJOR * 10000) + \
           (LUB_VERSION_MINOR * 100) + \
           (LUB_VERSION_PATCH)))

// Library version encoded as 0xMMmmpp (major, minor, patch)
// for display and debugging.
#define LUB_VERSION_HEX \
    (((LUB_VERSION_MAJOR) << 16) | \
     ((LUB_VERSION_MINOR) << 8) | \
     (LUB_VERSION_PATCH))

// True if the current library version is the specified version.
#define LUB_VERSION_EQ(maj, min, pat) \
    (LUB_VERSION_NUM == ((maj) * 10000 + (min) * 100 + (pat)))

// True if the current library version is at least the specified version.
#define LUB_VERSION_AT_LEAST(maj, min, pat) \
    (LUB_VERSION_NUM >= ((maj) * 10000 + (min) * 100 + (pat)))

// Compile-time validation of version macros (C99).
#define LUB_STATIC_ASSERT(cond, msg) \
    typedef char static_assert_##msg[(cond) ? 1 : -1]

// Ensure version components are within expected ranges.
LUB_STATIC_ASSERT(LUB_VERSION_MAJOR >= 0, major_must_be_nonnegative);
LUB_STATIC_ASSERT(LUB_VERSION_MINOR >= 0, minor_must_be_nonnegative);
LUB_STATIC_ASSERT(LUB_VERSION_PATCH >= 0, patch_must_be_nonnegative);

// Ensure version components fit in the hex encoding fields.
LUB_STATIC_ASSERT(LUB_VERSION_MAJOR <= 255, major_fits_in_hex_field);
LUB_STATIC_ASSERT(LUB_VERSION_MINOR <= 255, minor_fits_in_hex_field);
LUB_STATIC_ASSERT(LUB_VERSION_PATCH <= 255, patch_fits_in_hex_field);

// Ensure the numeric encoding cannot overflow int.
LUB_STATIC_ASSERT(LUB_VERSION_NUM >= 0, version_num_nonnegative);
LUB_STATIC_ASSERT(LUB_VERSION_NUM < 100000000, version_num_reasonable);

// Undefine version helper macros to avoid namespace pollution.
#undef LUB_STRINGIFY2
#undef LUB_STRINGIFY
#undef LUB_STATIC_ASSERT

// End of Versioning.

/**
 *   Unicode <- Latin:   ulsnSTRC, ulsSTRC
 *   Unicode <- Unicode: uusnpbrk, uuspbrk
 *
 * Case-insensitive set-character search (bounded and unbounded):
 *   Latin <- Latin:     llsnPBRKC, llsPBRKC
 *   Latin <- Unicode:   lusnPBRKC, lusPBRKC
 *   Unicode <- Latin:   ulsnPBRKC, ulsPBRKC
 *   Unicode <- Unicode: uusnPBRKC, uusPBRKC
 *
 * mth set-character search (bounded and unbounded):
 *   Latin <- Latin:     llsnpbrkm, llspbrkm
 *   Latin <- Unicode:   lusnpbrkm, luspbrkm
 *   Unicode <- Latin:   ulsnpbrkm, ulspbrkm
 *   Unicode <- Unicode: uusnpbrkm, uuspbrkm
 *
 * Case-insensitive mth set-character search (bounded and unbounded):
 *   Latin <- Latin:     llsnPBRKCM, llsPBRKCM
 *   Latin <- Unicode:   lusnPBRKCM, lusPBRKCM
 *   Unicode <- Latin:   ulsnPBRKCM, ulsPBRKCM
 *   Unicode <- Unicode: uusnPBRKCM, uusPBRKCM
 *
 * Reverse set-character search (bounded and unbounded):
 *   Latin <- Latin:     llsnrpbrk, llsrpbrk
 *   Latin <- Unicode:   lusnrpbrk, lusrpbrk
 *   Unicode <- Latin:   ulsnrpbrk, ulsrpbrk
 *   Unicode <- Unicode: uusnrpbrk, uusrpbrk
 *
 * mth-from-last reverse set-character search (bounded and unbounded):
 *   Latin <- Latin:     llsnrpbrkm, llsrpbrkm
 *   Latin <- Unicode:   lusnrpbrkm, lusrpbrkm
 *   Unicode <- Latin:   ulsnrpbrkm, ulsrpbrkm
 *   Unicode <- Unicode: uusnrpbrkm, uusrpbrkm
 *
 * Case-insensitive reverse set-character search (bounded and unbounded):
 *   Latin <- Latin:     llsnRPBRKC, llsRPBRKC
 *   Latin <- Unicode:   lusnRPBRKC, lusRPBRKC
 *   Unicode <- Latin:   ulsnRPBRKC, ulsRPBRKC
 *   Unicode <- Unicode: uusnRPBRKC, uusRPBRKC
 *
 * Case-insensitive mth-from-last reverse set-character search
 * (bounded and unbounded):
 *   Latin <- Latin:     llsnRPBRKCM, llsRPBRKCM
 *   Latin <- Unicode:   lusnRPBRKCM, lusRPBRKCM
 *   Unicode <- Latin:   ulsnRPBRKCM, ulsRPBRKCM
 *   Unicode <- Unicode: uusnRPBRKCM, uusRPBRKCM
 *
 * Initial-span length in set (bounded and unbounded):
 *   Latin <- Latin:     llsnspn, llsspn
 *   Latin <- Unicode:   lusnspn, lusspn
 *   Unicode <- Latin:   ulsnspn, ulsspn
 *   Unicode <- Unicode: uusnspn, uusspn
 *
 * Case-insensitive initial-span length in set (bounded and unbounded):
 *   Latin <- Latin:     llsnSPNC, llsSPNC
 *   Latin <- Unicode:   lusnSPNC, lusSPNC
 *   Unicode <- Latin:   ulsnSPNC, ulsSPNC
 *   Unicode <- Unicode: uusnSPNC, uusSPNC
 *
 * Initial-span length not in set (bounded and unbounded):
 *   Latin <- Latin:     llsncspn, llscspn
 *   Latin <- Unicode:   lusncspn, luscspn
 *   Unicode <- Latin:   ulsncspn, ulscspn
 *   Unicode <- Unicode: uusncspn, uuscspn
 *
 * Case-insensitive initial-span length not in set (bounded and unbounded):
 *   Latin <- Latin:     llsnCSPNC, llsCSPNC
 *   Latin <- Unicode:   lusnCSPNC, lusCSPNC
 *   Unicode <- Latin:   ulsnCSPNC, ulsCSPNC
 *   Unicode <- Unicode: uusnCSPNC, uusCSPNC
 *
 * Count occurrences of character c in s (bounded and unbounded):
 *   Latin <- Latin:     llsncnt, llscnt
 *   Latin <- Unicode:   lusncnt, luscnt
 *   Unicode <- Latin:   ulsncnt, ulscnt
 *   Unicode <- Unicode: uusncnt, uuscnt
 *
 * Case-insensitive count of occurrences of character c in s
 * (bounded and unbounded):
 *   Latin <- Latin:     llsnCNTC, llsCNTC
 *   Latin <- Unicode:   lusnCNTC, lusCNTC
 *   Unicode <- Latin:   ulsnCNTC, ulsCNTC
 *   Unicode <- Unicode: uusnCNTC, uusCNTC
 *
 * ---------------------------------------------------------------------------
 * SUBSTRING SEARCH FUNCTIONS (return pointer to first match or NULL)
 * ---------------------------------------------------------------------------
 * Case-sensitive substring search (bounded and unbounded):
 *   Latin <- Latin:     llsnstr, llsstr
 *   Latin <- Unicode:   lusnstr, lusstr
 *   Unicode <- Latin:   ulsnstr, ulsstr
 *   Unicode <- Unicode: uusnstr, uusstr
 *
 * Case-insensitive substring search (bounded and unbounded):
 *   Latin <- Latin:     llsnSTRC, llsSTRC
 *   Latin <- Unicode:   lusnSTRC, lusSTRC
 *   Unicode <- Latin:   ulsnSTRC, ulsSTRC
 *   Unicode <- Unicode: uusnSTRC, uusSTRC
 *
 * mth substring search (bounded and unbounded):
 *   Latin <- Latin:     llsnstrm, llsstrm
 *   Latin <- Unicode:   lusnstrm, lusstrm
 *   Unicode <- Latin:   ulsnstrm, ulsstrm
 *   Unicode <- Unicode: uusnstrm, uusstrm
 *
 * Case-insensitive mth substring search (bounded and unbounded):
 *   Latin <- Latin:     llsnSTRCM, llsSTRCM
 *   Latin <- Unicode:   lusnSTRCM, lusSTRCM
 *   Unicode <- Latin:   ulsnSTRCM, ulsSTRCM
 *   Unicode <- Unicode: uusnSTRCM, uusSTRCM
 *
 * Reverse substring search (bounded and unbounded):
 *   Latin <- Latin:     llsnrstr, llsrstr
 *   Latin <- Unicode:   lusnrstr, lusrstr
 *   Unicode <- Latin:   ulsnrstr, ulsrstr
 *   Unicode <- Unicode: uusnrstr, uusrstr
 *
 * Case-insensitive reverse substring search (bounded and unbounded):
 *   Latin <- Latin:     llsnRSTRC, llsRSTRC
 *   Latin <- Unicode:   lusnRSTRC, lusRSTRC
 *   Unicode <- Latin:   ulsnRSTRC, ulsRSTRC
 *   Unicode <- Unicode: uusnRSTRC, uusRSTRC
 *
 * mth-from-last reverse substring search (bounded and unbounded):
 *   Latin <- Latin:     llsnrstrm, llsrstrm
 *   Latin <- Unicode:   lusnrstrm, lusrstrm
 *   Unicode <- Latin:   ulsnrstrm, ulsrstrm
 *   Unicode <- Unicode: uusnrstrm, uusrstrm
 *
 * Case-insensitive mth-from-last reverse substring search
 * (bounded and unbounded):
 *   Latin <- Latin:     llsnRSTRCM, llsRSTRCM
 *   Latin <- Unicode:   lusnRSTRCM, lusRSTRCM
 *   Unicode <- Latin:   ulsnRSTRCM, ulsRSTRCM
 *   Unicode <- Unicode: uusnRSTRCM, uusRSTRCM
 *
 * Count occurrences of s2 in s1 (bounded and unbounded):
 *   Latin <- Latin:     llsnsubcnt, llssubcnt
 *   Latin <- Unicode:   lusnsubcnt, lussubcnt
 *   Unicode <- Latin:   ulsnsubcnt, ulssubcnt
 *   Unicode <- Unicode: uusnsubcnt, uussubcnt
 *
 * Case-insensitive count of occurrences of s2 in s1
 * (bounded and unbounded):
 *   Latin <- Latin:     llsnSUBCNTC, llsSUBCNTC
 *   Latin <- Unicode:   lusnSUBCNTC, lusSUBCNTC
 *   Unicode <- Latin:   ulsnSUBCNTC, ulsSUBCNTC
 *   Unicode <- Unicode: uusnSUBCNTC, uusSUBCNTC
 *
 * @section Examples Examples
 *
 * - Copy (bounded source):
 *   llsncpy(dst_l, src_l, 32);
 *
 * - Search (mth occurrence):
 *   lcstr_t p = llsnstrm("one two two", "two", 32, 2);
 *   // p points at the second "two"
 *
 * - Trim:
 *   llsntrim(dst_l, "  abc  ", 16, 'B', ' ');
 *   // -> "abc"
 *
 * - Split:
 *   llsnsplit(left_l, right_l, "key=value", '=', 32);
 *   // left_l="key", right_l="value"
 *
 * - Replace (bounded source):
 *   llsnreplace(t, 64, "cat and cat", "cat|dog", '|', 64, 2);
 *   // -> "cat and dog"
 *
 *   llsnreplace(t, 64, "x_x_x", "x|y", '|', 64, -1);
 *   // -> "x_x_y"
 *
 *   llsnreplace(t, 64, "ab ba", "a|b|b|a", '|', 64, 0);
 *   // -> "ba ab" (first matching pair wins)
 *
 *   llsnreplace(t, 64, "ab", "a|b|b|a", '|', 64, 1);
 *   // -> NULL (multi-pair map requires m == 0)
 *
 * - Replace (unbounded source):
 *   llsreplace(t, 64, "cat and cat", "cat|dog", '|', 2);
 *   // -> "cat and dog"
 *
 * - Replace, case-insensitive (bounded):
 *   llsnREPLACEC(t, 64, "Hello World", "hello|hi", '|', 64, 0);
 *   // -> "hi World"
 *
 * - Replace, case-insensitive (unbounded):
 *   llsREPLACEC(t, 64, "Cat and CAT", "cat|dog", '|', 0);
 *   // -> "dog and dog"
 *
 * - Reverse:
 *   llsrev(t, "stressed");
 *   // -> "desserts"
 *
 * - Pad (left, right, center):
 *   llspad(t, 6, "cat", '.', 1);
 *   // -> "...cat"
 *   llspad(t, 6, "cat", '.', -1);
 *   // -> "cat..."
 *   llspad(t, 7, "cat", '.', 0);
 *   // -> "..cat.."
 *
 * - Format:
 *   llsnprintf(dst_l, 64, "id=%u", (unsigned)42);
 *   // -> "id=42"
 *
 * @section Versioning Versioning
 * - LUB_VERSION_MAJOR, LUB_VERSION_MINOR, LUB_VERSION_PATCH
 * - LUB_VERSION          - string form, e.g., "1.0.0"
 * - LUB_VERSION_NUM      - integer MMmmpp, for comparisons
 * - LUB_VERSION_HEX      - 0xMMmmpp, for display/debugging
 * - LUB_VERSION_EQ(maj,min,pat)       - exact version check
 * - LUB_VERSION_AT_LEAST(maj,min,pat) - minimum version check
 *
 * Major = incompatible API change; minor = backward-compatible addition;
 * patch = bug fix or internal improvement. All stringify helper macros
 * are `#undef`'d after the versioning block to avoid namespace pollution.
 * The naming contract, error semantics, and safety guarantees are part of
 * the stable API and will not change without a major version increment.
 *
 * @section DesignNotes Design Notes
 * Five core principles guide this API:
 * - Explicitness:   caller supplies all buffers, bounds,
 *                   and substitution chars.
 * - Symmetry:       every operation exists for every encoding direction.
 * - Safety:         explicit bounds, terminator validation, representability
 *                   checks.
 * - Predictability: behavior mirrors familiar C string patterns while making
 *                   bounds and error contracts explicit.
 * - Portability:    C99, fixed-width types, no wchar_t size assumptions.
 *                   No API-managed locale state. However, Unicode
 *                   classification and case conversion use C runtime
 *                   wide-character routines (isw* and tow*) via wchar_t
 *                   casts and, therefore, results are locale- and
 *                   CRT-dependent.
 * - Clarity:        function names encode direction, bound,
 *                   operation, and case.
 *
 * All functions are `static inline`; no dynamic memory is allocated or freed.
 * This keeps the implementation lightweight and enables compiler inlining.
 * All string types are simple pointer aliases; no metadata is stored.
 *
 * Overlapping source/target buffers produce implementation-defined behavior.
 *
 * The API is thread-safe provided threads do not share target buffers without
 * external synchronization. Character classification relies on <ctype.h> and
 * <wctype.h>, which are thread-safe when the locale is not modified.
 *
 * All operations run in O(n) time; no recursion or dynamic allocation occurs.
 *
 * This header was reviewed and refined with assistance from
 * Microsoft Copilot.
 *
 * @section EncodingAssumptions Encoding Assumptions
 * - lchar_t = uint8_t  (Latin-8, values 0-255)
 * - uchar_t = uint16_t (Unicode BMP, values 0-65535)
 * - bchar_t = uint8_t  (raw byte)
 * - wchar_t width is not assumed; all casts include explicit bounds checks.
 * - No normalization, surrogate handling, or multi-code-unit processing.
 * - All operations act on individual characters only.
 *
 * @section GlossaryOfTerms Glossary
 * - lchar_t     - Latin-8 character (uint8_t)
 * - uchar_t     - Unicode-16 character (uint16_t)
 * - bchar_t     - raw byte (uint8_t)
 * - lstr_t      - pointer to null-terminated lchar_t sequence
 * - ustr_t      - pointer to null-terminated uchar_t sequence
 * - bstr_t      - pointer to bchar_t array; length always explicit
 * - MAX_LSTRLEN - maximum Latin characters per operation
 * - MAX_USTRLEN - maximum Unicode characters per operation
 * - MAX_BSTRLEN - maximum bytes per operation
 * - MISSING_STR_TERMINATOR - (size_t)-1; returned on unterminated source
 * - err_c       - substitution char for out-of-range Unicode->Latin conversions
 * - n           - max source characters/bytes; for char strings, stops at null
 * - span length - number of leading characters in s1 that satisfy a
 *                 function-specific condition
 *
 */

/**
 * @name MAX_LCHAR, MAX_UCHAR, MAX_BCHAR, MAX_LSTRLEN, MAX_USTRLEN,
 *       and MAX_BSTRLEN
 * @brief Maximum values for the character and string types.
 */
#undef MAX_LCHAR
#undef MAX_UCHAR
#undef MAX_BCHAR
#undef MAX_LSTRLEN
#undef MAX_USTRLEN
#undef MAX_BSTRLEN
#undef MISSING_STR_TERMINATOR
#define MAX_LCHAR ((size_t)255)
#define MAX_UCHAR ((size_t)65535)
#define MAX_BCHAR ((size_t)255)
#define MAX_LSTRLEN ((size_t)1000000)
#define MAX_USTRLEN ((size_t)500000)
#define MAX_BSTRLEN ((size_t)1000000)

/**
 * @name MISSING_STR_TERMINATOR
 * @brief Returned error value for missing null terminator in a
 * character-source string (bounded explictly or by default).
 * @note This value is chosen to be distinct from any valid string length.
 */
#define MISSING_STR_TERMINATOR ((size_t)-1)

/**
 * @name lchar_t, uchar_t, bchar_t, lstr_t, ustr_t, bstr_t,
 *       lcstr_t, ucstr_t, and bcstr_t
 * @brief Latin and Unicode character and string types, including
 * read-only string aliases for source parameters.
 */
typedef uint8_t lchar_t;         // Latin character
typedef uint16_t uchar_t;        // Unicode character
typedef uint8_t bchar_t;         // Byte
typedef lchar_t *lstr_t;         // Latin string
typedef uchar_t *ustr_t;         // Unicode string
typedef bchar_t *bstr_t;         // Byte string
typedef const lchar_t *lcstr_t;  // Read-only Latin string
typedef const uchar_t *ucstr_t;  // Read-only Unicode string
typedef const bchar_t *bcstr_t;  // Read-only byte string

/**
 * @name isualpha, islalpha, isudigit, isldigit, isualnum, islalnum,
 *       isuname1c, isunamec, isuupper, islupper, isulower, isllower,
 *       isucntrl, islcntrl, isuprint, islprint, isugraph, islgraph,
 *       isupunct, islpunct, isublank, islblank, isuspace, islspace,
 *       isuhexdigit, and islhexdigit
 * @brief Latin and Unicode character classification functions.
 * @param c Character to classify.
 * @return Non-zero if the character satisfies the condition, otherwise zero.
 * @note isuname1c classifies if c is valid for the first character
 *       of an unquoted Unicode identifier.
 * @note isunamec classifies whether c is valid for any character position
 *       other than the first in an unquoted Unicode identifier.
 */
static inline int isualpha(const uchar_t c)
    {return iswalpha((wchar_t)c);}
static inline int islalpha(const lchar_t c)
    {return isalpha((unsigned char)c);}
static inline int isudigit(const uchar_t c)
    {return iswdigit((wchar_t)c);}
static inline int isldigit(const lchar_t c)
    {return isdigit((unsigned char)c);}
static inline int isualnum(const uchar_t c)
    {return iswalnum((wchar_t)c);}
static inline int islalnum(const lchar_t c)
    {return isalnum((unsigned char)c);}
static inline int isuname1c(const uchar_t c)
    // The 1st character for an unquoted Unicode name
    // must be Latin alphabetic or '_'.
    {return c <= MAX_LCHAR &&
            (islalpha((lchar_t)c) || (lchar_t)c == '_');}
static inline int isunamec(const uchar_t c)
    // A character in an unquoted Unicode name
    // must be Latin alphanumeric or '_'.
    {return c <= MAX_LCHAR &&
            (islalnum((lchar_t)c) || (lchar_t)c == '_');}
static inline int isuupper(const uchar_t c)
    {return iswupper((wchar_t)c);}
static inline int islupper(const lchar_t c)
    {return isupper((unsigned char)c);}
static inline int isulower(const uchar_t c)
    {return iswlower((wchar_t)c);}
static inline int isllower(const lchar_t c)
    {return islower((unsigned char)c);}
static inline int isucntrl(const uchar_t c)
    {return iswcntrl((wchar_t)c);}
static inline int islcntrl(const lchar_t c)
    {return iscntrl((unsigned char)c);}
static inline int isuprint(const uchar_t c)
    {return iswprint((wchar_t)c);}
static inline int islprint(const lchar_t c)
    {return isprint((unsigned char)c);}
static inline int isugraph(const uchar_t c)
    {return iswgraph((wchar_t)c);}
static inline int islgraph(const lchar_t c)
    {return isgraph((unsigned char)c);}
static inline int isupunct(const uchar_t c)
    {return iswpunct((wchar_t)c);}
static inline int islpunct(const lchar_t c)
    {return ispunct((unsigned char)c);}
static inline int isublank(const uchar_t c)
    {return iswblank((wchar_t)c);}
static inline int islblank(const lchar_t c)
    {return isblank((unsigned char)c);}
static inline int isuspace(const uchar_t c)
    // ' ', '\t', '\v', '\f', '\n', '\r' plus additional Unicode
    // whitespace characters (e.g., U+00A0 NO-BREAK SPACE,
    // U+2003 EM SPACE, etc.)
    {return iswspace((wchar_t)c);}
static inline int islspace(const lchar_t c)
    // ' ', '\t', '\v', '\f', '\n', or '\r'
    {return isspace((unsigned char)c);}

/**
 * @name tolupper and touupper
 * @brief Uppercase a character.
 * @param c Character to Latin or Unicode uppercase.
 * @return Uppercase version of the character if
 *         one defined, otherwise the original character.
 */
static inline lchar_t tolupper(const lchar_t c)
    {return (lchar_t)toupper((unsigned char)c);}
static inline uchar_t touupper(const uchar_t c) {
    wint_t wc = towupper((wchar_t)c);
    return (wc < 0 || (size_t)wc > MAX_UCHAR) ? (uchar_t)c : (uchar_t)wc;
}

/**
 * @name tollower and toulower
 * @brief Lowercase a character.
 * @param c Character to Latin or Unicode lowercase.
 * @return Lowercase version of the character if
 *         one defined, otherwise the original character.
 */
static inline lchar_t tollower(const lchar_t c)
    {return (lchar_t)tolower((unsigned char)c);}
static inline uchar_t toulower(const uchar_t c) {
    wint_t wc = towlower((wchar_t)c);
    return (wc < 0 || (size_t)wc > MAX_UCHAR) ? (uchar_t)c : (uchar_t)wc;
}

/**
 * @name ilhexdigit and iuhexdigit
 * @brief Character hex digit to integer.
 * @param c Character to convert.
 * @return Integer value (0-15) of the hex digit
 *         if c is a valid hex digit ('0'-'9',
 *         'a'-'f', 'A'-'F'); otherwise, -1.
 */
static inline int iuhexdigit(const uchar_t c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    else if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return (int)-1;
}
static inline int ilhexdigit(const lchar_t c)
    {return iuhexdigit((uchar_t)c);}

/**
 * @name islhexdigit and isuhexdigit
 * @brief Classify as hex digit character.
 * @param c Character to classify.
 * @return Non-zero if the character is a hex digit;
 *         otherwise, zero.
 * @note Applies to all overloads in this family.
 */
static inline int islhexdigit(const lchar_t c)
    {return ilhexdigit(c) >= 0;}
static inline int isuhexdigit(const uchar_t c)
    {return iuhexdigit(c) >= 0;}

/**
 * @name lcsnlen and ucsnlen
 * @brief Return the length of a bounded source string.
 * @param s Source string.
 * @param n Bound (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return String length, or MISSING_STR_TERMINATOR if unterminated within
 * bound.
 */
static inline size_t lcsnlen(lcstr_t s, size_t n) {
    size_t k = 0; if (!s) return k;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    for (; n && *s; s++, n--) k++;
    if (*s)
        return MISSING_STR_TERMINATOR;
    return k;
}
static inline size_t ucsnlen(ucstr_t s, size_t n) {
    size_t k = 0; if (!s) return k;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s; s++, n--) k++;
    if (*s)
        return MISSING_STR_TERMINATOR;
    return k;
}

/**
 * @name lcslen and ucslen
 * @brief Return the length of a source string.
 * @param s Source string (bounded by MAX_LSTRLEN or MAX_USTRLEN).
 * @return String length, or MISSING_STR_TERMINATOR if unterminated within
 * bound.
 */
static inline size_t lcslen(lcstr_t s)
    {return lcsnlen(s, MAX_LSTRLEN);}
static inline size_t ucslen(ucstr_t s)
    {return ucsnlen(s, MAX_USTRLEN);}

/**
 * @name llsnapp, lusnapp, ulsnapp, and uusnapp
 * @brief Append the bounded source string to the target buffer.
 * @param t Append position in the target buffer.
 * @param s Source string.
 * @param n Source bound (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (lusnapp only) Replacement for out-of-range Unicode chars.
 * @return Next append position (null terminator), or NULL on error.
 */
static inline lstr_t llsnapp(lstr_t t, lcstr_t s, size_t n) {
    if (!t)
        return (lstr_t)NULL;
    *t = (lchar_t)0; // Ensure null terminator.
    if (!s) {*t = (lchar_t)0; return t;}
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    for (; n && *s; ++s, ++t, n--) *t = *s;
    if (*s)
        return (lstr_t)NULL;
    *t = (lchar_t)0;
    return t;
}
static inline lstr_t lusnapp(
    lstr_t t, ucstr_t s, size_t n,
    const lchar_t err_c) {
    if (!t)
        return (lstr_t)NULL;
    *t = (lchar_t)0; // Ensure null terminator.
    if (!s) {*t = (lchar_t)0; return t;}
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s; ++s, ++t, n--)
        *t = (size_t)*s <= MAX_LCHAR ? (lchar_t)*s : err_c;
    if (*s)
        return (lstr_t)NULL;
    *t = (lchar_t)0;
    return t;
}
static inline ustr_t ulsnapp(ustr_t t, lcstr_t s, size_t n) {
    if (!t)
        return (ustr_t)NULL;
    *t = (uchar_t)0; // Ensure null terminator.
    if (!s) {*t = (uchar_t)0; return t;}
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s; ++s, ++t, n--) *t = (uchar_t)*s;
    if (*s)
        return (ustr_t)NULL;
    *t = (uchar_t)0;
    return t;
}
static inline ustr_t uusnapp(ustr_t t, ucstr_t s, size_t n) {
    if (!t)
        return (ustr_t)NULL;
    *t = (uchar_t)0; // Ensure null terminator.
    if (!s) {*t = (uchar_t)0; return t;}
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s; ++s, ++t, n--) *t = *s;
    if (*s)
        return (ustr_t)NULL;
    *t = (uchar_t)0;
    return t;
}

/**
 * @name llsapp, lusapp, ulsapp, and uusapp
 * @brief Append the source string to the target buffer.
 * @param t Append position in the target buffer.
 * @param s Source string (bounded by MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (lusapp only) Replacement for out-of-range Unicode chars.
 * @return Next append position (null terminator), or NULL on error.
 */
static inline lstr_t llsapp(lstr_t t, lcstr_t s)
    {return llsnapp(t, s, MAX_LSTRLEN);}
static inline lstr_t lusapp(lstr_t t, ucstr_t s,  const lchar_t err_c)
    {return lusnapp(t, s, MAX_USTRLEN, err_c);}
static inline ustr_t ulsapp(ustr_t t, lcstr_t s)
    {return ulsnapp(t, s, MAX_USTRLEN);}
static inline ustr_t uusapp(ustr_t t, ucstr_t s)
    {return uusnapp(t, s, MAX_USTRLEN);}

/**
 * @name llsncpy, lusncpy, ulsncpy, and uusncpy
 * @brief Copy the bounded source string to the target buffer.
 * @param t Target buffer.
 * @param s Source string.
 * @param n Source bound (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (lusncpy only) Replacement for out-of-range Unicode chars.
 * @return t, or NULL on error.
 * @note Errors include: NULL arguments or unterminated s within bound n.
 */
static inline lstr_t llsncpy(lstr_t t, lcstr_t s, size_t n)
    {if (!llsnapp(t, s, n)) return (lstr_t)NULL; return t;}
static inline lstr_t lusncpy(
    lstr_t t, ucstr_t s, size_t n, const lchar_t err_c)
    {if (!lusnapp(t, s, n, err_c)) return (lstr_t)NULL; return t;}
static inline ustr_t ulsncpy(ustr_t t, lcstr_t s, size_t n)
    {if (!ulsnapp(t, s, n)) return (ustr_t)NULL; return t;}
static inline ustr_t uusncpy(ustr_t t, ucstr_t s, size_t n)
    {if (!uusnapp(t, s, n)) return (ustr_t)NULL; return t;}

/**
 * @name llscpy, luscpy, ulscpy, and uuscpy
 * @brief Copy the source string to the target buffer.
 * @param t Target buffer.
 * @param s Source string (bounded by MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (luscpy only) Replacement for out-of-range Unicode chars.
 * @return t, or NULL on error.
 * @note Errors include: NULL arguments or unterminated s within its bound.
 */
static inline lstr_t llscpy(lstr_t t, lcstr_t s)
    {if (!llsnapp(t, s, MAX_LSTRLEN)) return (lstr_t)NULL; return t;}
static inline lstr_t luscpy(lstr_t t, ucstr_t s, const lchar_t err_c)
    {if (!lusnapp(t, s, MAX_USTRLEN, err_c)) return (lstr_t)NULL; return t;}
static inline ustr_t ulscpy(ustr_t t, lcstr_t s)
    {if (!ulsnapp(t, s, MAX_USTRLEN)) return (ustr_t)NULL; return t;}
static inline ustr_t uuscpy(ustr_t t, ucstr_t s)
    {if (!uusnapp(t, s, MAX_USTRLEN)) return (ustr_t)NULL; return t;}

/**
 * @name llsncat, lusncat, ulsncat, and uusncat
 * @brief Concatenate the bounded source string to the target buffer.
 * @param t Null-terminated target buffer (bounded by MAX_LSTRLEN or
 * MAX_USTRLEN).
 * @param s Source string.
 * @param n Source bound (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (lusncat only) Replacement for out-of-range Unicode chars.
 * @return t, or NULL on error.
 * @note Errors include: NULL arguments, unterminated t or s within their bounds,
 *       or concatenation would exceed buffer size.
 */
static inline lstr_t llsncat(lstr_t t, lcstr_t s, size_t n) {
     if (!t)
         return (lstr_t)NULL;
     size_t t_len = lcsnlen(t, MAX_LSTRLEN);
     if (t_len == MISSING_STR_TERMINATOR) {
       *t = (lchar_t)0; // Ensure null terminator with error case.
       return (lstr_t)NULL;
     }
     size_t s_len = lcsnlen(s, n);
     if (s_len == MISSING_STR_TERMINATOR ||
         t_len + s_len >= MAX_LSTRLEN) return (lstr_t)NULL;
     llsnapp(t + t_len, s, n); return t;
}
static inline lstr_t lusncat(
    lstr_t t, ucstr_t s, size_t n,
    const lchar_t err_c) {
     if (!t)
         return (lstr_t)NULL;
     size_t t_len = lcsnlen(t, MAX_LSTRLEN);
     if (t_len == MISSING_STR_TERMINATOR) {
       *t = (lchar_t)0; // Ensure null terminator with error case.
       return (lstr_t)NULL;
     }
     size_t s_len = ucsnlen(s, n);
     if (s_len == MISSING_STR_TERMINATOR ||
         t_len + s_len >= MAX_LSTRLEN) return (lstr_t)NULL;
     lusnapp(t + t_len, s, n, err_c); return t;
}
static inline ustr_t ulsncat(ustr_t t, lcstr_t s, size_t n) {
     if (!t)
         return (ustr_t)NULL;
     size_t t_len = ucsnlen(t, MAX_USTRLEN);
     if (t_len == MISSING_STR_TERMINATOR) {
       *t = (uchar_t)0; // Ensure null terminator with error case.
       return (ustr_t)NULL;
     }
     size_t s_len = lcsnlen(s, n);
     if (s_len == MISSING_STR_TERMINATOR ||
         t_len + s_len >= MAX_USTRLEN) return (ustr_t)NULL;
     ulsnapp(t + t_len, s, n); return t;
}
static inline ustr_t uusncat(ustr_t t, ucstr_t s, size_t n) {
    if (!t)
        return (ustr_t)NULL;
    size_t t_len = ucsnlen(t, MAX_USTRLEN);
    if (t_len == MISSING_STR_TERMINATOR) {
      *t = (uchar_t)0; // Ensure null terminator with error case.
      return (ustr_t)NULL;
    }
    size_t s_len = ucsnlen(s, n);
    if (s_len == MISSING_STR_TERMINATOR ||
        t_len + s_len >= MAX_USTRLEN) return (ustr_t)NULL;
    uusnapp(t + t_len, s, n); return t;
}

/**
 * @name llscat, luscat, ulscat, and uuscat
 * @brief Concatenate the source string to the target buffer.
 * @param t Null-terminated target buffer (bounded by MAX_LSTRLEN or
 * MAX_USTRLEN).
 * @param s Source string (bounded by MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (luscat only) Replacement for out-of-range Unicode chars.
 * @return t, or NULL on error.
 * @note Errors include: NULL arguments, unterminated t or s, or concatenation
 *       would exceed buffer size.
 */
static inline lstr_t llscat(lstr_t t, lcstr_t s)
    {return llsncat(t, s, MAX_LSTRLEN);}
static inline lstr_t luscat(lstr_t t, ucstr_t s, const lchar_t err_c)
    {return lusncat(t, s, MAX_USTRLEN, err_c);}
static inline ustr_t ulscat(ustr_t t, lcstr_t s)
    {return ulsncat(t, s, MAX_USTRLEN);}
static inline ustr_t uuscat(ustr_t t, ucstr_t s)
    {return uusncat(t, s, MAX_USTRLEN);}

/**
 * @name llsnAPPC, lusnAPPC, ulsnAPPC, and uusnAPPC
 * @brief Append the uppercased bounded source string to the target buffer.
 * @param t Append position in the target buffer.
 * @param s Source string.
 * @param n Source bound (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (lusnAPPC only) Replacement for out-of-range Unicode chars.
 * @return Next append position (null terminator), or NULL on error.
 * @note Errors include: NULL arguments, unterminated s within bound n,
 *       or result would exceed buffer size.
 */
static inline lstr_t llsnAPPC(lstr_t t, lcstr_t s, size_t n) {
    if (!t)
        return (lstr_t)NULL;
    *t = (lchar_t)0; // Ensure null terminator.
    if (!s) {*t = (lchar_t)0; return t;}
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    for (; n && *s; ++s, ++t, n--) *t = tolupper(*s);
    if (*s)
        return (lstr_t)NULL;
    *t = (lchar_t)0;
    return t;
}
static inline lstr_t lusnAPPC(
    lstr_t t, ucstr_t s, size_t n,
    const lchar_t err_c) {
    if (!t)
        return (lstr_t)NULL;
    *t = (lchar_t)0; // Ensure null terminator.
    if (!s) {*t = (lchar_t)0; return t;}
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s; ++s, ++t, n--) {
        uchar_t up_c = touupper(*s);
        *t = (size_t)up_c > MAX_LCHAR ? err_c : (lchar_t)up_c;
    }
    if (*s)
        return (lstr_t)NULL;
    *t = (lchar_t)0;
    return t;
}
static inline ustr_t ulsnAPPC(ustr_t t, lcstr_t s, size_t n) {
    if (!t)
        return (ustr_t)NULL;
    *t = (uchar_t)0; // Ensure null terminator.
    if (!s) {*t = (uchar_t)0; return t;}
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s; ++s, ++t, n--) *t = (uchar_t)tolupper(*s);
    if (*s)
        return (ustr_t)NULL;
    *t = (uchar_t)0;
    return t;
}
static inline ustr_t uusnAPPC(ustr_t t, ucstr_t s, size_t n) {
    if (!t)
        return (ustr_t)NULL;
    *t = (uchar_t)0; // Ensure null terminator.
    if (!s) {*t = (uchar_t)0; return t;}
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s; ++s, ++t, n--) *t = touupper(*s);
    if (*s)
        return (ustr_t)NULL;
    *t = (uchar_t)0;
    return t;
}

/**
 * @name llsAPPC, lusAPPC, ulsAPPC, and uusAPPC
 * @brief Append the uppercased source string to the target buffer.
 * @param t Append position in the target buffer.
 * @param s Source string (bounded by MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (lusAPPC only) Replacement for out-of-range Unicode chars.
 * @return Next append position (null terminator), or NULL on error.
 * @note Errors include: NULL arguments, unterminated s, or result would exceed
 *       buffer size.
 */
static inline lstr_t llsAPPC(lstr_t t, lcstr_t s)
    {return llsnAPPC(t, s, MAX_LSTRLEN);}
static inline lstr_t lusAPPC(lstr_t t, ucstr_t s,  const lchar_t err_c)
    {return lusnAPPC(t, s, MAX_USTRLEN, err_c);}
static inline ustr_t ulsAPPC(ustr_t t, lcstr_t s)
    {return ulsnAPPC(t, s, MAX_USTRLEN);}
static inline ustr_t uusAPPC(ustr_t t, ucstr_t s)
    {return uusnAPPC(t, s, MAX_USTRLEN);}

/**
 * @name llsnCPYC, lusnCPYC, ulsnCPYC, and uusnCPYC
 * @brief Copy the uppercased bounded source string to the target buffer.
 * @param t Target buffer.
 * @param s Source string.
 * @param n Source bound (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (lusnCPYC only) Replacement for out-of-range Unicode chars.
 * @return t, or NULL on error.
 * @note Errors include: NULL arguments or unterminated s within bound n.
 */
static inline lstr_t llsnCPYC(lstr_t t, lcstr_t s, size_t n)
    {if (!llsnAPPC(t, s, n)) return (lstr_t)NULL; return t;}
static inline lstr_t lusnCPYC(
    lstr_t t, ucstr_t s,  size_t n, const lchar_t err_c)
    {if (!lusnAPPC(t, s, n, err_c)) return (lstr_t)NULL; return t;}
static inline ustr_t ulsnCPYC(ustr_t t, lcstr_t s, size_t n)
    {if (!ulsnAPPC(t, s, n)) return (ustr_t)NULL; return t;}
static inline ustr_t uusnCPYC(ustr_t t, ucstr_t s, size_t n)
    {if (!uusnAPPC(t, s, n)) return (ustr_t)NULL; return t;}

/**
 * @name llsCPYC, lusCPYC, ulsCPYC, and uusCPYC
 * @brief Copy the uppercased source string to the target buffer.
 * @param t Target buffer.
 * @param s Source string (bounded by MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (lusCPYC only) Replacement for out-of-range Unicode chars.
 * @return t, or NULL on error.
 * @note Errors include: NULL arguments or unterminated s within its bound.
 */
static inline lstr_t llsCPYC(lstr_t t, lcstr_t s)
    {if (!llsnAPPC(t, s, MAX_LSTRLEN)) return (lstr_t)NULL; return t;}
static inline lstr_t lusCPYC(lstr_t t, ucstr_t s,  const lchar_t err_c)
    {if (!lusnAPPC(t, s, MAX_USTRLEN, err_c)) return (lstr_t)NULL; return t;}
static inline ustr_t ulsCPYC(ustr_t t, lcstr_t s)
    {if (!ulsnAPPC(t, s, MAX_USTRLEN)) return (ustr_t)NULL; return t;}
static inline ustr_t uusCPYC(ustr_t t, ucstr_t s)
    {if (!uusnAPPC(t, s, MAX_USTRLEN)) return (ustr_t)NULL; return t;}

/**
 * @name llsnCATC, lusnCATC, ulsnCATC, and uusnCATC
 * @brief Concatenate the uppercased bounded source string onto the target
 * buffer.
 * @param t Null-terminated target buffer (bounded by MAX_LSTRLEN or
 * MAX_USTRLEN).
 * @param s Source string.
 * @param n Source bound (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (lusnCATC only) Replacement for out-of-range Unicode chars.
 * @return t, or NULL on error.
 * @note Errors include: NULL arguments, unterminated t or s within their bounds,
 *       or concatenation would exceed buffer size.
 */
static inline lstr_t llsnCATC(lstr_t t, lcstr_t s, size_t n) {
     if (!t)
         return (lstr_t)NULL;
     size_t t_len = lcsnlen(t, MAX_LSTRLEN);
     if (t_len == MISSING_STR_TERMINATOR) {
       *t = (lchar_t)0; // Ensure null terminator with error case.
       return (lstr_t)NULL;
     }
     size_t s_len = lcsnlen(s, n);
     if (s_len == MISSING_STR_TERMINATOR ||
         t_len + s_len >= MAX_LSTRLEN) return (lstr_t)NULL;
     llsnAPPC(t + t_len, s, n); return t;
}
static inline lstr_t lusnCATC(
    lstr_t t, ucstr_t s, size_t n,
    const lchar_t err_c) {
     if (!t)
         return (lstr_t)NULL;
     size_t t_len = lcsnlen(t, MAX_LSTRLEN);
     if (t_len == MISSING_STR_TERMINATOR) {
       *t = (lchar_t)0; // Ensure null terminator with error case.
       return (lstr_t)NULL;
      }
     size_t s_len = ucsnlen(s, n);
     if (s_len == MISSING_STR_TERMINATOR ||
         t_len + s_len >= MAX_LSTRLEN) return (lstr_t)NULL;
     lusnAPPC(t + t_len, s, n, err_c); return t;
}
static inline ustr_t ulsnCATC(ustr_t t, lcstr_t s, size_t n) {
     if (!t)
         return (ustr_t)NULL;
     size_t t_len = ucsnlen(t, MAX_USTRLEN);
     if (t_len == MISSING_STR_TERMINATOR) {
       *t = (uchar_t)0; // Ensure null terminator with error case.
       return (ustr_t)NULL;
     }
     size_t s_len = lcsnlen(s, n);
     if (s_len == MISSING_STR_TERMINATOR ||
         t_len + s_len >= MAX_USTRLEN) return (ustr_t)NULL;
     ulsnAPPC(t + t_len, s, n); return t;
}
static inline ustr_t uusnCATC(ustr_t t, ucstr_t s, size_t n) {
    if (!t)
        return (ustr_t)NULL;
    size_t t_len = ucsnlen(t, MAX_USTRLEN);
    if (t_len == MISSING_STR_TERMINATOR) {
        *t = (uchar_t)0; // Ensure null terminator with error case.
        return (ustr_t)NULL;
    }
    size_t s_len = ucsnlen(s, n);
    if (s_len == MISSING_STR_TERMINATOR ||
        t_len + s_len >= MAX_USTRLEN) return (ustr_t)NULL;
    uusnAPPC(t + t_len, s, n); return t;
}

/**
 * @name llsCATC, lusCATC, ulsCATC, and uusCATC
 * @brief Concatenate the uppercased source string onto the target buffer.
 * @param t Null-terminated target buffer (bounded by MAX_LSTRLEN or
 * MAX_USTRLEN).
 * @param s Source string (bounded by MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (lusCATC only) Replacement for out-of-range Unicode chars.
 * @return t, or NULL on error.
 * @note Errors include: NULL arguments, unterminated t or s, or concatenation
 *       would exceed buffer size.
 */
static inline lstr_t llsCATC(lstr_t t, lcstr_t s)
    {return llsnCATC(t, s, MAX_LSTRLEN);}
static inline lstr_t lusCATC(lstr_t t, ucstr_t s, const lchar_t err_c)
    {return lusnCATC(t, s, MAX_USTRLEN, err_c);}
static inline ustr_t ulsCATC(ustr_t t, lcstr_t s)
    {return ulsnCATC(t, s, MAX_USTRLEN);}
static inline ustr_t uusCATC(ustr_t t, ucstr_t s)
    {return uusnCATC(t, s, MAX_USTRLEN);}

/**
 * @name llsnappc, lusnappc, ulsnappc, and uusnappc
 * @brief Append the lowercased bounded source string to the target buffer.
 * @param t Append position in the target buffer.
 * @param s Source string.
 * @param n Source bound (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (lusnappc only) Replacement for out-of-range Unicode chars.
 * @return Next append position (null terminator), or NULL on error.
 * @note Errors include: NULL arguments, unterminated s within bound n,
 *       or result would exceed buffer size.
 */
static inline lstr_t llsnappc(lstr_t t, lcstr_t s, size_t n) {
    if (!t)
        return (lstr_t)NULL;
    *t = (lchar_t)0; // Ensure null terminator.
    if (!s)
        return t;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    for (; n && *s; ++s, ++t, n--) *t = tollower(*s);
    if (*s)
        return (lstr_t)NULL;
    *t = (lchar_t)0;
    return t;
}
static inline lstr_t lusnappc(
    lstr_t t, ucstr_t s, size_t n,
    const lchar_t err_c) {
    if (!t)
        return (lstr_t)NULL;
    *t = (lchar_t)0; // Ensure null terminator.
    if (!s)
        return t;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s; ++s, ++t, n--) {
        uchar_t lo_c = toulower(*s);
        *t = (size_t)lo_c > MAX_LCHAR ? err_c : (lchar_t)lo_c;
    }
    if (*s)
        return (lstr_t)NULL;
    *t = (lchar_t)0;
    return t;
}
static inline ustr_t ulsnappc(ustr_t t, lcstr_t s, size_t n) {
    if (!t)
        return (ustr_t)NULL;
    *t = (uchar_t)0; // Ensure null terminator.
    if (!s)
        return t;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s; ++s, ++t, n--) *t = (uchar_t)tollower(*s);
    if (*s)
        return (ustr_t)NULL;
    *t = (uchar_t)0;
    return t;
}
static inline ustr_t uusnappc(ustr_t t, ucstr_t s, size_t n) {
    if (!t)
        return (ustr_t)NULL;
    *t = (uchar_t)0; // Ensure null terminator.
    if (!s)
        return t;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s; ++s, ++t, n--) *t = toulower(*s);
    if (*s)
        return (ustr_t)NULL;
    *t = (uchar_t)0;
    return t;
}

/**
 * @name llsappc, lusappc, ulsappc, and uusappc
 * @brief Append the lowercased source string to the target buffer.
 * @param t Append position in the target buffer.
 * @param s Source string (bounded by MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (lusappc only) Replacement for out-of-range Unicode chars.
 * @return Next append position (null terminator), or NULL on error.
 * @note Errors include: NULL arguments, unterminated s, or result would exceed
 *       buffer size.
 */
static inline lstr_t llsappc(lstr_t t, lcstr_t s)
    {return llsnappc(t, s, MAX_LSTRLEN);}
static inline lstr_t lusappc(lstr_t t, ucstr_t s,  const lchar_t err_c)
    {return lusnappc(t, s, MAX_USTRLEN, err_c);}
static inline ustr_t ulsappc(ustr_t t, lcstr_t s)
    {return ulsnappc(t, s, MAX_USTRLEN);}
static inline ustr_t uusappc(ustr_t t, ucstr_t s)
    {return uusnappc(t, s, MAX_USTRLEN);}

/**
 * @name llsncpyc, lusncpyc, ulsncpyc, and uusncpyc
 * @brief Copy the lowercased bounded source string to the target buffer.
 * @param t Target buffer.
 * @param s Source string.
 * @param n Source bound (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (lusncpyc only) Replacement for out-of-range Unicode chars.
 * @return t, or NULL on error.
 * @note Errors include: NULL arguments or unterminated s within bound n.
 */
static inline lstr_t llsncpyc(lstr_t t, lcstr_t s, size_t n)
    {if (!llsnappc(t, s, n)) return (lstr_t)NULL; return t;}
static inline lstr_t lusncpyc(
    lstr_t t, ucstr_t s,  size_t n, const lchar_t err_c)
    {if (!lusnappc(t, s, n, err_c)) return (lstr_t)NULL; return t;}
static inline ustr_t ulsncpyc(ustr_t t, lcstr_t s, size_t n)
    {if (!ulsnappc(t, s, n)) return (ustr_t)NULL; return t;}
static inline ustr_t uusncpyc(ustr_t t, ucstr_t s, size_t n)
    {if (!uusnappc(t, s, n)) return (ustr_t)NULL; return t;}

/**
 * @name llscpyc, luscpyc, ulscpyc, and uuscpyc
 * @brief Copy the lowercased source string to the target buffer.
 * @param t Target buffer.
 * @param s Source string (bounded by MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (luscpyc only) Replacement for out-of-range Unicode chars.
 * @return t, or NULL on error.
 * @note Errors include: NULL arguments or unterminated s within its bound.
 */
static inline lstr_t llscpyc(lstr_t t, lcstr_t s)
    {if (!llsnappc(t, s, MAX_LSTRLEN)) return (lstr_t)NULL; return t;}
static inline lstr_t luscpyc(lstr_t t, ucstr_t s,  const lchar_t err_c)
    {if (!lusnappc(t, s, MAX_USTRLEN, err_c)) return (lstr_t)NULL; return t;}
static inline ustr_t ulscpyc(ustr_t t, lcstr_t s)
    {if (!ulsnappc(t, s, MAX_USTRLEN)) return (ustr_t)NULL; return t;}
static inline ustr_t uuscpyc(ustr_t t, ucstr_t s)
    {if (!uusnappc(t, s, MAX_USTRLEN)) return (ustr_t)NULL; return t;}

/**
 * @name llsncatc, lusncatc, ulsncatc, and uusncatc
 * @brief Concatenate the lowercased bounded source string onto the target
 * buffer.
 * @param t Null-terminated target buffer (bounded by MAX_LSTRLEN or
 * MAX_USTRLEN).
 * @param s Source string.
 * @param n Source bound (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (lusncatc only) Replacement for out-of-range Unicode chars.
 * @return t, or NULL on error.
 * @note Errors include: NULL arguments, unterminated t or s within their bounds,
 *       or concatenation would exceed buffer size.
 */
static inline lstr_t llsncatc(lstr_t t, lcstr_t s, size_t n) {
     if (!t)
         return (lstr_t)NULL;
     size_t t_len = lcsnlen(t, MAX_LSTRLEN);
     if (t_len == MISSING_STR_TERMINATOR) {
       *t = (lchar_t)0; // Ensure null terminator with error case.
       return (lstr_t)NULL;
     }
     size_t s_len = lcsnlen(s, n);
     if (s_len == MISSING_STR_TERMINATOR ||
         t_len + s_len >= MAX_LSTRLEN) return (lstr_t)NULL;
     llsnappc(t + t_len, s, n); return t;
}
static inline lstr_t lusncatc(
    lstr_t t, ucstr_t s, size_t n,
    const lchar_t err_c) {
     if (!t)
         return (lstr_t)NULL;
     size_t t_len = lcsnlen(t, MAX_LSTRLEN);
     if (t_len == MISSING_STR_TERMINATOR) {
       *t = (lchar_t)0; // Ensure null terminator with error case.
       return (lstr_t)NULL;
     }
     size_t s_len = ucsnlen(s, n);
     if (s_len == MISSING_STR_TERMINATOR ||
         t_len + s_len >= MAX_LSTRLEN) return (lstr_t)NULL;
     lusnappc(t + t_len, s, n, err_c); return t;
}
static inline ustr_t ulsncatc(ustr_t t, lcstr_t s, size_t n) {
     if (!t)
         return (ustr_t)NULL;
     size_t t_len = ucsnlen(t, MAX_USTRLEN);
     if (t_len == MISSING_STR_TERMINATOR) {
       *t = (uchar_t)0; // Ensure null terminator with error case.
       return (ustr_t)NULL;
     }
     size_t s_len = lcsnlen(s, n);
     if (s_len == MISSING_STR_TERMINATOR ||
         t_len + s_len >= MAX_USTRLEN) return (ustr_t)NULL;
     ulsnappc(t + t_len, s, n); return t;
}
static inline ustr_t uusncatc(ustr_t t, ucstr_t s, size_t n) {
    if (!t)
        return (ustr_t)NULL;
    size_t t_len = ucsnlen(t, MAX_USTRLEN);
    if (t_len == MISSING_STR_TERMINATOR) {
      *t = (uchar_t)0; // Ensure null terminator with error case.
      return (ustr_t)NULL;
    }
    size_t s_len = ucsnlen(s, n);
    if (s_len == MISSING_STR_TERMINATOR ||
        t_len + s_len >= MAX_USTRLEN) return (ustr_t)NULL;
    uusnappc(t + t_len, s, n); return t;
}

/**
 * @name llscatc, luscatc, ulscatc, and uuscatc
 * @brief Concatenate the lowercased source string onto the target buffer.
 * @param t Null-terminated target buffer (bounded by MAX_LSTRLEN or
 * MAX_USTRLEN).
 * @param s Source string (bounded by MAX_LSTRLEN or MAX_USTRLEN).
 * @param err_c (luscatc only) Replacement for out-of-range Unicode chars.
 * @return t, or NULL on error.
 * @note Errors include: NULL arguments, unterminated t or s, or concatenation
 *       would exceed buffer size.
 */
static inline lstr_t llscatc(lstr_t t, lcstr_t s)
    {return llsncatc(t, s, MAX_LSTRLEN);}
static inline lstr_t luscatc(lstr_t t, ucstr_t s, const lchar_t err_c)
    {return lusncatc(t, s, MAX_USTRLEN, err_c);}
static inline ustr_t ulscatc(ustr_t t, lcstr_t s)
    {return ulsncatc(t, s, MAX_USTRLEN);}
static inline ustr_t uuscatc(ustr_t t, ucstr_t s)
    {return uusncatc(t, s, MAX_USTRLEN);}

/**
 * @name lbsnappc, ubsnappc, lbsnAPPC, and ubsnAPPC
 * @brief Append n bytes from source as hex digits to the target buffer.
 * @param t Append position in the target buffer.
 * @param s Source byte string.
 * @param n Number of bytes to convert (2 hex chars written per byte).
 * @return Next append position (null terminator), or NULL if t is NULL.
 * @note appc functions append lowercase; APPC functions append uppercase.
 * @note n is clamped so the result fits within MAX_LSTRLEN (lb) or MAX_USTRLEN
 * (ub).
 */
static inline lstr_t lbsnappc(lstr_t t, bcstr_t s, size_t n) {
    if (!t)
        return (lstr_t)NULL;
    *t = (lchar_t)0; // Ensure null terminator even if subsequent error occurs.
    if (n > MAX_LSTRLEN >> 1) n = MAX_LSTRLEN >> 1;
    static const lchar_t hexdigit_lc[] = "0123456789abcdef";
    while (n--) {
        const bchar_t c = s ? *s++ : (bchar_t)0;
        *t++ = hexdigit_lc[(c >> 4) & 0xF];
        *t++ = hexdigit_lc[c & 0xF];
    }
    *t = (lchar_t)0;
    return t;
}
static inline ustr_t ubsnappc(ustr_t t, bcstr_t s, size_t n) {
    if (!t)
        return (ustr_t)NULL;
    *t = (uchar_t)0; // Ensure null terminator even if subsequent error occurs.
    if (n > MAX_USTRLEN >> 1) n = MAX_USTRLEN >> 1;
    static const lchar_t hexdigit_lc[] = "0123456789abcdef";
    while (n--) {
        const bchar_t c = s ? *s++ : (bchar_t)0;
        *t++ = (uchar_t)hexdigit_lc[(c >> 4) & 0xF];
        *t++ = (uchar_t)hexdigit_lc[c & 0xF];
    }
    *t = (uchar_t)0;
    return t;
}
static inline lstr_t lbsnAPPC(lstr_t t, bcstr_t s, size_t n) {
    if (!t)
        return (lstr_t)NULL;
    *t = (lchar_t)0; // Ensure null terminator even if subsequent error occurs.
    if (n > MAX_LSTRLEN >> 1) n = MAX_LSTRLEN >> 1;
    static const lchar_t hexdigit_UC[] = "0123456789ABCDEF";
    while (n--) {
        const bchar_t c = s ? *s++ : (bchar_t)0;
        *t++ = hexdigit_UC[(c >> 4) & 0xF];
        *t++ = hexdigit_UC[c & 0xF];
    }
    *t = (lchar_t)0;
    return t;
}
static inline ustr_t ubsnAPPC(ustr_t t, bcstr_t s, size_t n) {
    if (!t)
        return (ustr_t)NULL;
    *t = (uchar_t)0; // Ensure null terminator even if subsequent error occurs.
    if (n > MAX_USTRLEN >> 1) n = MAX_USTRLEN >> 1;
    static const lchar_t hexdigit_UC[] = "0123456789ABCDEF";
    while (n--) {
        const bchar_t c = s ? *s++ : (bchar_t)0;
        *t++ = (uchar_t)hexdigit_UC[(c >> 4) & 0xF];
        *t++ = (uchar_t)hexdigit_UC[c & 0xF];
    }
    *t = (uchar_t)0;
    return t;
}

/**
 * @name lbsncpyc, lbsnCPYC, ubsncpyc, and ubsnCPYC
 * @brief Copy n bytes from source as hex digits into the target buffer.
 * @param t Target buffer.
 * @param s Source byte string.
 * @param n Number of bytes to convert (2 hex chars written per byte).
 * @return t, or NULL if t is NULL.
 * @note cpyc functions copy lowercase; CPYC functions copy uppercase.
 * @note n is clamped so the result fits within MAX_LSTRLEN (lb) or MAX_USTRLEN
 * (ub).
 */
static inline lstr_t lbsncpyc(lstr_t t, bcstr_t s, size_t n)
    {if (!lbsnappc(t, s, n)) return (lstr_t)NULL; return t;}
static inline lstr_t lbsnCPYC(lstr_t t, bcstr_t s, size_t n)
    {if (!lbsnAPPC(t, s, n)) return (lstr_t) NULL; return t;}
static inline ustr_t ubsncpyc(ustr_t t, bcstr_t s, size_t n)
    {if (!ubsnappc(t, s, n)) return (ustr_t)NULL; return t;}
static inline ustr_t ubsnCPYC(ustr_t t, bcstr_t s, size_t n)
    {if (!ubsnAPPC(t, s, n)) return (ustr_t)NULL; return t;}

/**
 * @name lbsncatc, lbsnCATC, ubsncatc, and ubsnCATC
 * @brief Concatenate n bytes from source as hex digits to the target buffer.
 * @param t Null-terminated target buffer.
 * @param s Source byte string.
 * @param n Number of bytes to convert (2 hex chars appended per byte).
 * @return t, or NULL on error.
 * @note Errors include: NULL arguments, unterminated t, or insufficient
 *       remaining capacity for converted output.
 * @note catc functions concatenate lowercase; CATC functions concatenate
 * uppercase.
 * @note n is clamped so the combined result fits within MAX_LSTRLEN (lb) or
 * MAX_USTRLEN (ub).
 */
static inline lstr_t lbsncatc(lstr_t t, bcstr_t s, size_t n) {
     if (!t)
         return (lstr_t)NULL;
     size_t t_len = lcsnlen(t, MAX_LSTRLEN);
     if (t_len == MISSING_STR_TERMINATOR) {
       *t = (lchar_t)0;
    // Ensure empty target with null terminator for error case.
       return (lstr_t)NULL;
     }
     if (n > (MAX_LSTRLEN - t_len - 1) / 2)
         return (lstr_t)NULL;
     lbsnappc(t + t_len, s, n); return t;
}
static inline lstr_t lbsnCATC(lstr_t t, bcstr_t s, size_t n) {
     if (!t)
         return (lstr_t)NULL;
     size_t t_len = lcsnlen(t, MAX_LSTRLEN);
     if (t_len == MISSING_STR_TERMINATOR) {
       *t = (lchar_t)0;
    // Ensure empty target with null terminator for error case.
       return (lstr_t)NULL;
     }
     if (n > (MAX_LSTRLEN - t_len - 1) / 2)
         return (lstr_t)NULL;
     lbsnAPPC(t + t_len, s, n); return t;
}
static inline ustr_t ubsncatc(ustr_t t, bcstr_t s, size_t n) {
     if (!t)
         return (ustr_t)NULL;
     size_t t_len = ucsnlen(t, MAX_USTRLEN);
     if (t_len == MISSING_STR_TERMINATOR) {
       *t = (uchar_t)0;
    // Ensure empty target with null terminator for error case.
       return (ustr_t)NULL;
     }
     if (n > (MAX_USTRLEN - t_len - 1) / 2)
         return (ustr_t)NULL;
     ubsnappc(t + t_len, s, n); return t;
}
static inline ustr_t ubsnCATC(ustr_t t, bcstr_t s, size_t n) {
    if (!t)
        return (ustr_t)NULL;
    size_t t_len = ucsnlen(t, MAX_USTRLEN);
    if (t_len == MISSING_STR_TERMINATOR) {
      *t = (uchar_t)0;
    // Ensure empty target with null terminator for error case.
       return (ustr_t)NULL;
    }
    if (n > (MAX_USTRLEN - t_len - 1) / 2)
        return (ustr_t)NULL;
    ubsnAPPC(t + t_len, s, n); return t;
}

/**
 * @name blsnapp and busnapp
 * @brief Append a bounded hex digit string to a target byte buffer
 *        converting the hex digits to bytes.
 * @param t Pointer to the append position in the target byte buffer.
 * @param s Pointer to the source hex digit string.
 * @param n Bound on the source string.
 * @return Pointer to the next append position in the target buffer.
 *         If s is NULL or the source string is empty, t is returned.
 *         Returns NULL if t is NULL, source string contains other
 *         than hex digits, or s is not null terminated.
 * @note If the source string contains an odd number of hex digits, the last
 *       hex digit is treated as the high nibble of a byte, with an implicit
 *       low nibble of zero.
 * @note If NULL is not returned:
 *         - the number of bytes appended to the target buffer
 *           is the value of the function minus t.
 *         - the number of bytes in the target buffer is
 *           the value of the function minus the
 *           pointer to the target buffer.
 *       Alternatively, determine the number of bytes appended by obtaining
 *       the source string length (up to n) and dividing by 2 and rounding up.
 * @note If NULL is returned, the number of bytes appended to the target buffer
 *       can be determined by checking the source string for where the invalid
 *       character is or if the null terminator is missing.
 * @note For safety, n is bounded so that the resulting hex digit
 *       string length does not exceed MAX_LSTRLEN for blsnapp or
 *       MAX_USTRLEN otherwise.
 * @note The target buffer must have enough space for the result.
 */
static inline bstr_t blsnapp(bstr_t t, lcstr_t s, size_t n)
{   if (!t || !s) return t;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    int hi, lo;
    for (; n && *s; n--, s++) {
        hi = ilhexdigit(*s);
        if (n && s[1]) {lo = ilhexdigit(*++s); n--;} else lo = 0;
        if (hi < 0 || lo < 0) return (bstr_t)0; // Invalid hex digit.
        *t++ = (unsigned char)((hi << 4) | lo);
    }
    if (*s) return (bstr_t)0; // Missing null terminator for source string.
    return t;
}
static inline bstr_t busnapp(bstr_t t, ucstr_t s, size_t n)
{   if (!t || !s) return t;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    int hi, lo;
    for (; n && *s; n--, s++) {
        hi = iuhexdigit(*s);
        lo = 0;
        if (n && s[1]) {lo = iuhexdigit(*++s); n--;} else lo = 0;
        if (hi < 0 || lo < 0) return (bstr_t)0; // Invalid hex digit.
        *t++ = (unsigned char)((hi << 4) | lo);
    }
    if (*s) return (bstr_t)0; // Missing null terminator for source string.
    return t;
}

/**
 * @name blsncpy and busncpy
 * @brief Copy a bounded hex digit string to a target byte buffer
 *        converting the hex digits to bytes.
 * @param t Pointer to the target byte buffer.
 * @param s Pointer to the source hex digit string.
 * @param n Bound on the source string.
 * @return Returns NULL if t is NULL, source string contains other
 *         than hex digits, or s is not null terminated; otherwise, returns t.
 * @note If the source string contains an odd number of hex digits, the last
 *       digit is treated as the high nibble of a byte, with an implicit low
 *       nibble of zero.
 * @note If NULL is not returned, the number of bytes copied to the target
 * buffer
 *       is half the number of characters in the source string (rounded up).
 * If the source pointer is NULL, the source string length is considered zero.
 * @note If NULL is returned, the number of bytes appended to the target buffer
 *       can be determined by checking the source string for where the invalid
 *       character is or if the null terminator is missing.
 * @note For safety, n is bounded by MAX_LSTRLEN for blsncpy or
 *       MAX_USTRLEN otherwise
 * @note The target buffer must have enough space for the result.
 */
static inline bstr_t blsncpy(bstr_t t, lcstr_t s, size_t n)
    {if (!blsnapp(t, s, n)) return (bstr_t)NULL; return t;}
static inline bstr_t busncpy(bstr_t t, ucstr_t s, size_t n)
    {if (!busnapp(t, s, n)) return (bstr_t)NULL; return t;}

/**
 * @name blsapp and busapp
 * @brief Append a hex digit string to a target byte buffer
 *        converting the hex digits to bytes.
 * @param t Pointer to the append position in the target byte buffer.
 * @param s Pointer to the source hex digit string.
 * @return Pointer to the next append position in the target buffer.
 *         If s is NULL or the source string is empty, t is returned.
 *         Returns NULL if t is NULL, source string contains other
 *         than hex digits, or s is not null terminated.
 * @note If the source string contains an odd number of hex digits, the last
 *       hex digit is treated as the high nibble of a byte, with an implicit low
 *       nibble of zero.
 * @note If NULL is not returned:
 *         - the number of bytes appended to the target buffer
 *           is the value of the function minus t.
 *         - the number of bytes in the target buffer is
 *           the value of the function minus the
 *           pointer to the target buffer.
 *       Alternatively, the number of bytes appended is
 *       half the source string length (rounded up).
 * @note If NULL is returned, the number of bytes appended to the target buffer
 *       can be determined by checking the source string for where the invalid
 *       character is or if the null terminator is missing.
 * @note For safety, the source string is bounded by MAX_LSTRLEN
 *       for blsapp or MAX_USTRLEN otherwise.
 * @note The target buffer must have enough space for the result.
 */
static inline bstr_t blsapp(bstr_t t, lcstr_t s)
   {return blsnapp(t, s, MAX_LSTRLEN);}
static inline bstr_t busapp(bstr_t t, ucstr_t s)
   {return busnapp(t, s, MAX_USTRLEN);}

/**
 * @name blscpy and buscpy
 * @brief Copy a hex digit string to a target byte buffer
 *        converting the hex digits to bytes.
 * @param t Pointer to the target byte buffer.
 * @param s Pointer to the source hexadecimal string.
 * @return Pointer to the target buffer.
 *         If s is NULL or the source string is empty, t is returned.
 *         Returns NULL if t is NULL, source string contains other
 *         than hex digits, or s is not null terminated.
 * @note If the source string contains an odd number of hex digits, the last
 *       hex digit is treated as the high nibble of a byte, with an implicit
 *       low nibble of zero.
 * @note If NULL is not returned, the number of bytes
 *       copied is the source string length, which is half
 *       the number of hex digits (rounded up).
 * @note If NULL is returned, the number of bytes appended to the target buffer
 *       can be determined by checking the source string for where the invalid
 *       character is or if the null terminator is missing.
 * @note For safety, the source string is bounded by MAX_LSTRLEN
 *       for blscpy or MAX_USTRLEN otherwise.
 * @note The target buffer must have enough space for the result.
 */
static inline bstr_t blscpy(bstr_t t, lcstr_t s)
    {if (!blsnapp(t, s, MAX_LSTRLEN)) return (bstr_t)NULL; return t;}
static inline bstr_t buscpy(bstr_t t, ucstr_t s)
    {if (!busnapp(t, s, MAX_USTRLEN)) return (bstr_t)NULL; return t;}

/**
 * @name islhexdigits and isuhexdigits
 * @brief Validate that a bounded source string consists
 *        of valid hex digits.
 * @param s Pointer to the source string.
 * @param n Bound on the source string.
 * @return 1 if the source string is valid.
 *         0 if invalid or a null-terminator is not found
 *           for the source string.
 * @note For safety, n is bounded by MAX_LSTRLEN for islhexdigits or
 *       MAX_USTRLEN otherwise.
 */
static inline int islhexdigits(lcstr_t s, size_t n)
{   if (!s) return (int)0;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    for (; n && *s; --n, ++s) if (!islhexdigit(*s)) return (int)0;
    if (*s) return (int)0; // Missing null terminator.
    return (int)1;
}
static inline int isuhexdigits(ucstr_t s, size_t n)
{   if (!s) return (int)0;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s; --n, ++s) if (!isuhexdigit(*s)) return (int)0;
    if (*s) return (int)0; // Missing null terminator.
    return (int)1;
}

/**
 * @name llsncmp, lusncmp, ulsncmp, and uusncmp
 * @brief Case-sensitive bounded comparison of two strings.
 * @param s1 First source string.
 * @param s2 Second source string.
 * @param n Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return -1 (s1 < s2), 0 (equal), 1 (s1 > s2), -2 (unterminated source).
 */
static inline int llsncmp(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    for (; n && *s1 && *s2 && *s1 == *s2; s1++, s2++, n--);
    if (!*s1 && !*s2)
        return (int)0;
    if (n)
        return *s1 > *s2 ? (int)1 : (int)-1;
    return (int)-2; // source not null terminated.
}
static inline int lusncmp(lcstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s1 && *s2 && (uchar_t)*s1 == *s2; s1++, s2++, n--);
    if (!*s1 && !*s2)
        return (int)0;
    if (n)
        return (uchar_t)*s1 > *s2 ? (int)1 : (int)-1;
    return (int)-2; // Source not null terminated.
}
static inline int ulsncmp(ucstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s1 && *s2 && *s1 == *s2; s1++, s2++, n--);
    if (!*s1 && !*s2)
        return (int)0;
    if (n)
        return *s1 > (uchar_t)*s2 ? (int)1 : (int)-1;
    return (int)-2; // Source not null terminated.
}
static inline int uusncmp(ucstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s1 && *s2 && *s1 == *s2; s1++, s2++, n--);
    if (!*s1 && !*s2)
        return (int)0;
    if (n)
        return *s1 > *s2 ? (int)1 : (int)-1;
    return (int)-2; // Source not null terminated.
}

/**
 * @name llscmp, luscmp, ulscmp, and uuscmp
 * @brief Case-sensitive comparison of two strings.
 * @param s1 First source string (bounded by MAX_LSTRLEN or MAX_USTRLEN).
 * @param s2 Second source string (same bound).
 * @return -1 (s1 < s2), 0 (equal), 1 (s1 > s2), -2 (unterminated source).
 */
static inline int llscmp(lcstr_t s1, lcstr_t s2)
    {return llsncmp(s1, s2, MAX_LSTRLEN);}
static inline int luscmp(lcstr_t s1, ucstr_t s2)
    {return lusncmp(s1, s2, MAX_USTRLEN);}
static inline int ulscmp(ucstr_t s1, lcstr_t s2)
    {return ulsncmp(s1, s2, MAX_USTRLEN);}
static inline int uuscmp(ucstr_t s1, ucstr_t s2)
    {return uusncmp(s1, s2, MAX_USTRLEN);}

/**
 * @name llsnCMPC, lusnCMPC, ulsnCMPC, and uusnCMPC
 * @brief Case-insensitive bounded comparison of two strings.
 * @param s1 First source string.
 * @param s2 Second source string.
 * @param n Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return -1 (s1 < s2), 0 (equal), 1 (s1 > s2), -2 (unterminated source).
 */
static inline int llsnCMPC(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    for (; n && *s1 && *s2 && tolupper(*s1) == tolupper(*s2); s1++, s2++, n--);
    if (!*s1 && !*s2)
        return (int)0;
    if (n)
        return tolupper(*s1) > tolupper(*s2) ? (int)1 : (int)-1;
    return (int)-2; // source not null terminated.
}
static inline int lusnCMPC(lcstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s1 && *s2 &&
            touupper((uchar_t)*s1) == touupper(*s2);
            s1++, s2++, n--);
    if (!*s1 && !*s2)
        return (int)0;
    if (n)
        return touupper((uchar_t)*s1) > touupper(*s2) ? (int)1 : (int)-1;
    return (int)-2; // source not null terminated.
}
static inline int ulsnCMPC(ucstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s1 && *s2 &&
            touupper(*s1) == touupper((uchar_t)*s2);
            s1++, s2++, n--);
    if (!*s1 && !*s2)
        return (int)0;
    if (n)
        return touupper(*s1) > touupper((uchar_t)*s2) ? (int)1 : (int)-1;
    return (int)-2; // source not null terminated.
}
static inline int uusnCMPC(ucstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s1 && *s2 && touupper(*s1) == touupper(*s2); s1++, s2++, n--);
    if (!*s1 && !*s2)
        return (int)0;
    if (n)
        return touupper(*s1) > touupper(*s2) ? (int)1 : (int)-1;
    return (int)-2; // source not null terminated.
}

/**
 * @name llsCMPC, lusCMPC, ulsCMPC, and uusCMPC
 * @brief Case-insensitive comparison of two strings.
 * @param s1 First source string (bounded by MAX_LSTRLEN or MAX_USTRLEN).
 * @param s2 Second source string (same bound).
 * @return -1 (s1 < s2), 0 (equal), 1 (s1 > s2), -2 (unterminated source).
 */
static inline int llsCMPC(lcstr_t s1, lcstr_t s2)
    {return llsnCMPC(s1, s2, MAX_LSTRLEN);}
static inline int lusCMPC(lcstr_t s1, ucstr_t s2)
    {return lusnCMPC(s1, s2, MAX_USTRLEN);}
static inline int ulsCMPC(ucstr_t s1, lcstr_t s2)
    {return ulsnCMPC(s1, s2, MAX_USTRLEN);}
static inline int uusCMPC(ucstr_t s1, ucstr_t s2)
    {return uusnCMPC(s1, s2, MAX_USTRLEN);}

/**
 * @name llsnpfx, lusnpfx, ulsnpfx, and uusnpfx
 * @brief Case-sensitive bounded prefix check of s2 against the start of s1.
 * @param s1 Source string to test as the full string.
 * @param s2 Source prefix candidate.
 * @param n Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return 0 if s2 is a prefix of s1, -1/1 on first mismatch ordering,
 *         or -2 on source contract violation.
 */
static inline int llsnpfx(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (int)-2;
    size_t k = s1_len < s2_len ? s1_len : s2_len;
    for (size_t i = 0; i < k; ++i) {
        if (s1[i] != s2[i])
            return s1[i] > s2[i] ? (int)1 : (int)-1;
    }
    if (s2_len <= s1_len)
        return (int)0;
    return (int)-1;
}
static inline int lusnpfx(lcstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (int)-2;
    size_t k = s1_len < s2_len ? s1_len : s2_len;
    for (size_t i = 0; i < k; ++i) {
        uchar_t c1 = (uchar_t)s1[i];
        if (c1 != s2[i])
            return c1 > s2[i] ? (int)1 : (int)-1;
    }
    if (s2_len <= s1_len)
        return (int)0;
    return (int)-1;
}
static inline int ulsnpfx(ucstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (int)-2;
    size_t k = s1_len < s2_len ? s1_len : s2_len;
    for (size_t i = 0; i < k; ++i) {
        uchar_t c2 = (uchar_t)s2[i];
        if (s1[i] != c2)
            return s1[i] > c2 ? (int)1 : (int)-1;
    }
    if (s2_len <= s1_len)
        return (int)0;
    return (int)-1;
}
static inline int uusnpfx(ucstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (int)-2;
    size_t k = s1_len < s2_len ? s1_len : s2_len;
    for (size_t i = 0; i < k; ++i) {
        if (s1[i] != s2[i])
            return s1[i] > s2[i] ? (int)1 : (int)-1;
    }
    if (s2_len <= s1_len)
        return (int)0;
    return (int)-1;
}

/**
 * @name llspfx, luspfx, ulspfx, and uuspfx
 * @brief Case-sensitive prefix check of s2 against the start of s1.
 * @param s1 Source string to test as the full string.
 * @param s2 Source prefix candidate.
 * @note Default bounds are applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 * @return 0 if s2 is a prefix of s1, -1/1 on first mismatch ordering,
 *         or -2 on source contract violation.
 */
static inline int llspfx(lcstr_t s1, lcstr_t s2)
    {return llsnpfx(s1, s2, MAX_LSTRLEN);}
static inline int luspfx(lcstr_t s1, ucstr_t s2)
    {return lusnpfx(s1, s2, MAX_USTRLEN);}
static inline int ulspfx(ucstr_t s1, lcstr_t s2)
    {return ulsnpfx(s1, s2, MAX_USTRLEN);}
static inline int uuspfx(ucstr_t s1, ucstr_t s2)
    {return uusnpfx(s1, s2, MAX_USTRLEN);}

/**
 * @name llsnPFXC, lusnPFXC, ulsnPFXC, and uusnPFXC
 * @brief Case-insensitive bounded prefix check of s2 against the start of s1.
 * @param s1 Source string to test as the full string.
 * @param s2 Source prefix candidate.
 * @param n Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return 0 if s2 is a prefix of s1, -1/1 on first mismatch ordering,
 *         or -2 on source contract violation.
 */
static inline int llsnPFXC(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (int)-2;
    size_t k = s1_len < s2_len ? s1_len : s2_len;
    for (size_t i = 0; i < k; ++i) {
        lchar_t c1 = tolupper(s1[i]);
        lchar_t c2 = tolupper(s2[i]);
        if (c1 != c2)
            return c1 > c2 ? (int)1 : (int)-1;
    }
    if (s2_len <= s1_len)
        return (int)0;
    return (int)-1;
}
static inline int lusnPFXC(lcstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (int)-2;
    size_t k = s1_len < s2_len ? s1_len : s2_len;
    for (size_t i = 0; i < k; ++i) {
        uchar_t c1 = touupper((uchar_t)s1[i]);
        uchar_t c2 = touupper(s2[i]);
        if (c1 != c2)
            return c1 > c2 ? (int)1 : (int)-1;
    }
    if (s2_len <= s1_len)
        return (int)0;
    return (int)-1;
}
static inline int ulsnPFXC(ucstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (int)-2;
    size_t k = s1_len < s2_len ? s1_len : s2_len;
    for (size_t i = 0; i < k; ++i) {
        uchar_t c1 = touupper(s1[i]);
        uchar_t c2 = touupper((uchar_t)s2[i]);
        if (c1 != c2)
            return c1 > c2 ? (int)1 : (int)-1;
    }
    if (s2_len <= s1_len)
        return (int)0;
    return (int)-1;
}
static inline int uusnPFXC(ucstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (int)-2;
    size_t k = s1_len < s2_len ? s1_len : s2_len;
    for (size_t i = 0; i < k; ++i) {
        uchar_t c1 = touupper(s1[i]);
        uchar_t c2 = touupper(s2[i]);
        if (c1 != c2)
            return c1 > c2 ? (int)1 : (int)-1;
    }
    if (s2_len <= s1_len)
        return (int)0;
    return (int)-1;
}

/**
 * @name llsPFXC, lusPFXC, ulsPFXC, and uusPFXC
 * @brief Case-insensitive prefix check of s2 against the start of s1.
 * @param s1 Source string to test as the full string.
 * @param s2 Source prefix candidate.
 * @note Default bounds are applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 * @return 0 if s2 is a prefix of s1, -1/1 on first mismatch ordering,
 *         or -2 on source contract violation.
 */
static inline int llsPFXC(lcstr_t s1, lcstr_t s2)
    {return llsnPFXC(s1, s2, MAX_LSTRLEN);}
static inline int lusPFXC(lcstr_t s1, ucstr_t s2)
    {return lusnPFXC(s1, s2, MAX_USTRLEN);}
static inline int ulsPFXC(ucstr_t s1, lcstr_t s2)
    {return ulsnPFXC(s1, s2, MAX_USTRLEN);}
static inline int uusPFXC(ucstr_t s1, ucstr_t s2)
    {return uusnPFXC(s1, s2, MAX_USTRLEN);}

/**
 * @name llsnsfx, lusnsfx, ulsnsfx, and uusnsfx
 * @brief Case-sensitive bounded suffix check of s2 against the end of s1.
 * @param s1 Source string to test as the full string.
 * @param s2 Source suffix candidate.
 * @param n Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return 0 if s2 is a suffix of s1, -1/1 on first aligned mismatch ordering,
 *         or -2 on source contract violation.
 */
static inline int llsnsfx(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (int)-2;
    if (!s2_len)
        return (int)0;
    if (s2_len > s1_len)
        return (int)-1;
    size_t start = s1_len - s2_len;
    for (size_t i = 0; i < s2_len; ++i) {
        if (s1[start + i] != s2[i])
            return s1[start + i] > s2[i] ? (int)1 : (int)-1;
    }
    return (int)0;
}
static inline int lusnsfx(lcstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (int)-2;
    if (!s2_len)
        return (int)0;
    if (s2_len > s1_len)
        return (int)-1;
    size_t start = s1_len - s2_len;
    for (size_t i = 0; i < s2_len; ++i) {
        uchar_t c1 = (uchar_t)s1[start + i];
        if (c1 != s2[i])
            return c1 > s2[i] ? (int)1 : (int)-1;
    }
    return (int)0;
}
static inline int ulsnsfx(ucstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (int)-2;
    if (!s2_len)
        return (int)0;
    if (s2_len > s1_len)
        return (int)-1;
    size_t start = s1_len - s2_len;
    for (size_t i = 0; i < s2_len; ++i) {
        uchar_t c2 = (uchar_t)s2[i];
        if (s1[start + i] != c2)
            return s1[start + i] > c2 ? (int)1 : (int)-1;
    }
    return (int)0;
}
static inline int uusnsfx(ucstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (int)-2;
    if (!s2_len)
        return (int)0;
    if (s2_len > s1_len)
        return (int)-1;
    size_t start = s1_len - s2_len;
    for (size_t i = 0; i < s2_len; ++i) {
        if (s1[start + i] != s2[i])
            return s1[start + i] > s2[i] ? (int)1 : (int)-1;
    }
    return (int)0;
}

/**
 * @name llssfx, lussfx, ulssfx, and uussfx
 * @brief Case-sensitive suffix check of s2 against the end of s1.
 * @param s1 Source string to test as the full string.
 * @param s2 Source suffix candidate.
 * @note Default bounds are applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 * @return 0 if s2 is a suffix of s1, -1/1 on first aligned mismatch ordering,
 *         or -2 on source contract violation.
 */
static inline int llssfx(lcstr_t s1, lcstr_t s2)
    {return llsnsfx(s1, s2, MAX_LSTRLEN);}
static inline int lussfx(lcstr_t s1, ucstr_t s2)
    {return lusnsfx(s1, s2, MAX_USTRLEN);}
static inline int ulssfx(ucstr_t s1, lcstr_t s2)
    {return ulsnsfx(s1, s2, MAX_USTRLEN);}
static inline int uussfx(ucstr_t s1, ucstr_t s2)
    {return uusnsfx(s1, s2, MAX_USTRLEN);}

/**
 * @name llsnSFXC, lusnSFXC, ulsnSFXC, and uusnSFXC
 * @brief Case-insensitive bounded suffix check of s2 against the end of s1.
 * @param s1 Source string to test as the full string.
 * @param s2 Source suffix candidate.
 * @param n Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return 0 if s2 is a suffix of s1, -1/1 on first aligned mismatch ordering,
 *         or -2 on source contract violation.
 * @note Applies to all overloads in this family.
 */
static inline int llsnSFXC(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (int)-2;
    if (!s2_len)
        return (int)0;
    if (s2_len > s1_len)
        return (int)-1;
    size_t start = s1_len - s2_len;
    for (size_t i = 0; i < s2_len; ++i) {
        lchar_t c1 = tolupper(s1[start + i]);
        lchar_t c2 = tolupper(s2[i]);
        if (c1 != c2)
            return c1 > c2 ? (int)1 : (int)-1;
    }
    return (int)0;
}
static inline int lusnSFXC(lcstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (int)-2;
    if (!s2_len)
        return (int)0;
    if (s2_len > s1_len)
        return (int)-1;
    size_t start = s1_len - s2_len;
    for (size_t i = 0; i < s2_len; ++i) {
        uchar_t c1 = touupper((uchar_t)s1[start + i]);
        uchar_t c2 = touupper(s2[i]);
        if (c1 != c2)
            return c1 > c2 ? (int)1 : (int)-1;
    }
    return (int)0;
}
static inline int ulsnSFXC(ucstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (int)-2;
    if (!s2_len)
        return (int)0;
    if (s2_len > s1_len)
        return (int)-1;
    size_t start = s1_len - s2_len;
    for (size_t i = 0; i < s2_len; ++i) {
        uchar_t c1 = touupper(s1[start + i]);
        uchar_t c2 = touupper((uchar_t)s2[i]);
        if (c1 != c2)
            return c1 > c2 ? (int)1 : (int)-1;
    }
    return (int)0;
}
static inline int uusnSFXC(ucstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (int)-2;
    if (!s2_len)
        return (int)0;
    if (s2_len > s1_len)
        return (int)-1;
    size_t start = s1_len - s2_len;
    for (size_t i = 0; i < s2_len; ++i) {
        uchar_t c1 = touupper(s1[start + i]);
        uchar_t c2 = touupper(s2[i]);
        if (c1 != c2)
            return c1 > c2 ? (int)1 : (int)-1;
    }
    return (int)0;
}

/**
 * @name llsSFXC, lusSFXC, ulsSFXC, and uusSFXC
 * @brief Case-insensitive suffix check of s2 against the end of s1.
 * @param s1 Source string to test as the full string.
 * @param s2 Source suffix candidate.
 * @note Default bounds are applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 * @return 0 if s2 is a suffix of s1, -1/1 on first aligned mismatch ordering,
 *         or -2 on source contract violation.
 */
static inline int llsSFXC(lcstr_t s1, lcstr_t s2)
    {return llsnSFXC(s1, s2, MAX_LSTRLEN);}
static inline int lusSFXC(lcstr_t s1, ucstr_t s2)
    {return lusnSFXC(s1, s2, MAX_USTRLEN);}
static inline int ulsSFXC(ucstr_t s1, lcstr_t s2)
    {return ulsnSFXC(s1, s2, MAX_USTRLEN);}
static inline int uusSFXC(ucstr_t s1, ucstr_t s2)
    {return uusnSFXC(s1, s2, MAX_USTRLEN);}

/**
 * @name llsnchrm, lusnchrm, ulsnchrm, and uusnchrm
 * @brief Bounded case-sensitive search for the mth occurrence of character
 * c in string s.
 * @param s Source string.
 * @param c Character to search for.
 * @param n Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index (1 = first, 2 = second, ...). 0 returns NULL.
 * @return Pointer to mth match in s, or NULL if not found or on source error.
 * @note Applies to all overloads in this family.
 */
static inline lcstr_t llsnchrm(
    lcstr_t s, const lchar_t c, size_t n, size_t m) {
    if (!s || !m)
        return (lstr_t)NULL;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    size_t k = 0;
    for (size_t i = 0; i <= len; ++i) {
        if (s[i] == c) {
            if (++k == m) return s + i;
        }
    }
    return (lstr_t)NULL;
}
static inline lcstr_t lusnchrm(
    lcstr_t s, const uchar_t c, size_t n, size_t m) {
    if (!s || !m)
        return (lstr_t)NULL;
    if ((size_t)c > MAX_LCHAR)
        return (lstr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    size_t k = 0;
    lchar_t lc = (lchar_t)c;
    for (size_t i = 0; i <= len; ++i) {
        if (s[i] == lc) {
            if (++k == m) return s + i;
        }
    }
    return (lstr_t)NULL;
}
static inline ucstr_t ulsnchrm(
    ucstr_t s, const lchar_t c, size_t n, size_t m) {
    if (!s || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    size_t k = 0;
    uchar_t uc = (uchar_t)c;
    for (size_t i = 0; i <= len; ++i) {
        if (s[i] == uc) {
            if (++k == m) return s + i;
        }
    }
    return (ustr_t)NULL;
}
static inline ucstr_t uusnchrm(
    ucstr_t s, const uchar_t c, size_t n, size_t m) {
    if (!s || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    size_t k = 0;
    for (size_t i = 0; i <= len; ++i) {
        if (s[i] == c) {
            if (++k == m) return s + i;
        }
    }
    return (ustr_t)NULL;
}

/**
 * @name llsnchr, lusnchr, ulsnchr, and uusnchr
 * @brief Bounded case-sensitive search for the first occurrence of character c
 * in string s.
 * @param s Source string.
 * @param c Character to search for.
 * @param n Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Pointer to first match in s, or NULL if not found or on source error.
 * @note Applies to all overloads in this family.
 */
static inline lcstr_t llsnchr(lcstr_t s, const lchar_t c, size_t n)
    {return llsnchrm(s, c, n, 1);}
static inline lcstr_t lusnchr(lcstr_t s, const uchar_t c, size_t n)
    {return lusnchrm(s, c, n, 1);}
static inline ucstr_t ulsnchr(ucstr_t s, const lchar_t c, size_t n)
    {return ulsnchrm(s, c, n, 1);}
static inline ucstr_t uusnchr(ucstr_t s, const uchar_t c, size_t n)
    {return uusnchrm(s, c, n, 1);}

/**
 * @name llschr, luschr, ulschr, and uuschr
 * @brief Case-sensitive search for the first occurrence of character c
 * in string s.
 * @param s Source string.
 * @param c Character to search for.
 * @return Pointer to first match in s, or NULL if not found or on source error.
 * @note Applies to all overloads in this family.
 */
static inline lcstr_t llschr(lcstr_t s, const lchar_t c)
    {return llsnchr(s, c, MAX_LSTRLEN);}
static inline lcstr_t luschr(lcstr_t s, const uchar_t c)
    {return lusnchr(s, c, MAX_USTRLEN);}
static inline ucstr_t ulschr(ucstr_t s, const lchar_t c)
    {return ulsnchr(s, c, MAX_USTRLEN);}
static inline ucstr_t uuschr(ucstr_t s, const uchar_t c)
    {return uusnchr(s, c, MAX_USTRLEN);}

/**
 * @name llsnCHRCM, lusnCHRCM, ulsnCHRCM, and uusnCHRCM
 * @brief Bounded case-insensitive search for the mth occurrence of character
 * c in s.
 * @param s Source string.
 * @param c Character to search for.
 * @param n Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index (1 = first, 2 = second, ...). 0 returns NULL.
 * @return Pointer to mth match in s, or NULL if not found or on source error.
 * @note Applies to all overloads in this family.
 */
static inline lcstr_t llsnCHRCM(
    lcstr_t s, const lchar_t c, size_t n, size_t m) {
    if (!s || !m)
        return (lstr_t)NULL;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    size_t k = 0;
    lchar_t uc = tolupper(c);
    for (size_t i = 0; i <= len; ++i) {
        if (tolupper(s[i]) == uc) {
            if (++k == m) return s + i;
        }
    }
    return (lstr_t)NULL;
}
static inline lcstr_t lusnCHRCM(
    lcstr_t s, const uchar_t c, size_t n, size_t m) {
    if (!s || !m)
        return (lstr_t)NULL;
    if ((size_t)c > MAX_LCHAR)
        return (lstr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    size_t k = 0;
    lchar_t uc = tolupper((lchar_t)c);
    for (size_t i = 0; i <= len; ++i) {
        if (tolupper(s[i]) == uc) {
            if (++k == m) return s + i;
        }
    }
    return (lstr_t)NULL;
}
static inline ucstr_t ulsnCHRCM(
    ucstr_t s, const lchar_t c, size_t n, size_t m) {
    if (!s || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    size_t k = 0;
    uchar_t uc = touupper((uchar_t)c);
    for (size_t i = 0; i <= len; ++i) {
        if (touupper(s[i]) == uc) {
            if (++k == m) return s + i;
        }
    }
    return (ustr_t)NULL;
}
static inline ucstr_t uusnCHRCM(
    ucstr_t s, const uchar_t c, size_t n, size_t m) {
    if (!s || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    size_t k = 0;
    uchar_t uc = touupper(c);
    for (size_t i = 0; i <= len; ++i) {
        if (touupper(s[i]) == uc) {
            if (++k == m) return s + i;
        }
    }
    return (ustr_t)NULL;
}

/**
 * @name llsnCHRC, lusnCHRC, ulsnCHRC, and uusnCHRC
 * @brief Bounded case-insensitive search for the first occurrence of character
 * c.
 * @param s Source string.
 * @param c Character to search for.
 * @param n Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Pointer to first match in s, or NULL if not found or on source error.
 * @note Applies to all overloads in this family.
 */
static inline lcstr_t llsnCHRC(lcstr_t s, const lchar_t c, size_t n)
    {return llsnCHRCM(s, c, n, 1);}
static inline lcstr_t lusnCHRC(lcstr_t s, const uchar_t c, size_t n)
    {return lusnCHRCM(s, c, n, 1);}
static inline ucstr_t ulsnCHRC(ucstr_t s, const lchar_t c, size_t n)
    {return ulsnCHRCM(s, c, n, 1);}
static inline ucstr_t uusnCHRC(ucstr_t s, const uchar_t c, size_t n)
    {return uusnCHRCM(s, c, n, 1);}

/**
 * @name llsCHRC, lusCHRC, ulsCHRC, and uusCHRC
 * @brief Case-insensitive search for the first occurrence of character c.
 * @param s Source string.
 * @param c Character to search for.
 * @return Pointer to first match in s, or NULL if not found or on source error.
 * @note Applies to all overloads in this family.
 */
static inline lcstr_t llsCHRC(lcstr_t s, const lchar_t c)
    {return llsnCHRC(s, c, MAX_LSTRLEN);}
static inline lcstr_t lusCHRC(lcstr_t s, const uchar_t c)
    {return lusnCHRC(s, c, MAX_USTRLEN);}
static inline ucstr_t ulsCHRC(ucstr_t s, const lchar_t c)
    {return ulsnCHRC(s, c, MAX_USTRLEN);}
static inline ucstr_t uusCHRC(ucstr_t s, const uchar_t c)
    {return uusnCHRC(s, c, MAX_USTRLEN);}


/**
 * @name llschrm, luschrm, ulschrm, and uuschrm
 * @brief Case-sensitive search for the mth occurrence of character c
 * in s.
 * @param s Source string.
 * @param c Character to search for.
 * @param m Occurrence index (1 = first, 2 = second, ...). 0 returns NULL.
 * @note Default bounds applied according to source type.
 * @return Pointer to mth match in s, or NULL if not found or on source error.
 */
static inline lcstr_t llschrm(lcstr_t s, const lchar_t c, size_t m)
    {return llsnchrm(s, c, MAX_LSTRLEN, m);}
static inline lcstr_t luschrm(lcstr_t s, const uchar_t c, size_t m)
    {return lusnchrm(s, c, MAX_USTRLEN, m);}
static inline ucstr_t ulschrm(ucstr_t s, const lchar_t c, size_t m)
    {return ulsnchrm(s, c, MAX_USTRLEN, m);}
static inline ucstr_t uuschrm(ucstr_t s, const uchar_t c, size_t m)
    {return uusnchrm(s, c, MAX_USTRLEN, m);}


/**
 * @name llsCHRCM, lusCHRCM, ulsCHRCM, and uusCHRCM
 * @brief Case-insensitive search for the mth occurrence of character c in s.
 * @param s Source string.
 * @param c Character to search for.
 * @param m Occurrence index (1 = first, 2 = second, ...). 0 returns NULL.
 * @note Default bounds applied according to source type.
 * @return Pointer to mth match in s, or NULL if not found or on source error.
 */
static inline lcstr_t llsCHRCM(lcstr_t s, const lchar_t c, size_t m)
    {return llsnCHRCM(s, c, MAX_LSTRLEN, m);}
static inline lcstr_t lusCHRCM(lcstr_t s, const uchar_t c, size_t m)
    {return lusnCHRCM(s, c, MAX_USTRLEN, m);}
static inline ucstr_t ulsCHRCM(ucstr_t s, const lchar_t c, size_t m)
    {return ulsnCHRCM(s, c, MAX_USTRLEN, m);}
static inline ucstr_t uusCHRCM(ucstr_t s, const uchar_t c, size_t m)
    {return uusnCHRCM(s, c, MAX_USTRLEN, m);}

/**
 * @name llsnrchrm, lusnrchrm, ulsnrchrm, and uusnrchrm
 * @brief Bounded case-sensitive search for the mth-from-last occurrence of
 * character c in string s.
 * @param s Source string.
 * @param c Character to search for.
 * @param n Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index from the end (1 = last, 2 = second-last, ...).
 * @return Pointer to mth-from-last match in s, or NULL if not found or on
 * source error.
 * @note Applies to all overloads in this family.
 */
static inline lcstr_t llsnrchrm(
    lcstr_t s, const lchar_t c, size_t n, size_t m) {
    if (!s || !m)
        return (lstr_t)NULL;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    size_t k = 0;
    for (size_t ii = len + 1; ii; --ii) {
        size_t i = ii - 1;
        if (s[i] == c && ++k == m) return s + i;
    }
    return (lstr_t)NULL;
}
static inline lcstr_t lusnrchrm(
    lcstr_t s, const uchar_t c, size_t n, size_t m) {
    if (!s || !m)
        return (lstr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    if ((size_t)c > MAX_LCHAR)
        return (lstr_t)NULL;
    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    size_t k = 0;
    lchar_t lc = (lchar_t)c;
    for (size_t ii = len + 1; ii; --ii) {
        size_t i = ii - 1;
        if (s[i] == lc && ++k == m) return s + i;
    }
    return (lstr_t)NULL;
}
static inline ucstr_t ulsnrchrm(
    ucstr_t s, const lchar_t c, size_t n, size_t m) {
    if (!s || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    size_t k = 0;
    uchar_t uc = (uchar_t)c;
    for (size_t ii = len + 1; ii; --ii) {
        size_t i = ii - 1;
        if (s[i] == uc && ++k == m) return s + i;
    }
    return (ustr_t)NULL;
}
static inline ucstr_t uusnrchrm(
    ucstr_t s, const uchar_t c, size_t n, size_t m) {
    if (!s || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    size_t k = 0;
    for (size_t ii = len + 1; ii; --ii) {
        size_t i = ii - 1;
        if (s[i] == c && ++k == m) return s + i;
    }
    return (ustr_t)NULL;
}

/**
 * @name llsnrchr, lusnrchr, ulsnrchr, and uusnrchr
 * @brief Bounded case-sensitive search for the last occurrence of character c
 * in string s.
 * @param s Source string.
 * @param c Character to search for.
 * @param n Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Pointer to last match in s, or NULL if not found or on source error.
 * @note Applies to all overloads in this family.
 */
static inline lcstr_t llsnrchr(lcstr_t s, const lchar_t c, size_t n)
    {return llsnrchrm(s, c, n, 1);}
static inline lcstr_t lusnrchr(lcstr_t s, const uchar_t c, size_t n)
    {return lusnrchrm(s, c, n, 1);}
static inline ucstr_t ulsnrchr(ucstr_t s, const lchar_t c, size_t n)
    {return ulsnrchrm(s, c, n, 1);}
static inline ucstr_t uusnrchr(ucstr_t s, const uchar_t c, size_t n)
    {return uusnrchrm(s, c, n, 1);}

/**
 * @name llsrchr, lusrchr, ulsrchr, and uusrchr
 * @brief Case-sensitive search for the last occurrence of character c
 * in string s.
 * @param s Source string.
 * @param c Character to search for.
 * @return Pointer to last match in s, or NULL if not found or on source error.
 * @note Applies to all overloads in this family.
 */
static inline lcstr_t llsrchr(lcstr_t s, const lchar_t c)
    {return llsnrchr(s, c, MAX_LSTRLEN);}
static inline lcstr_t lusrchr(lcstr_t s, const uchar_t c)
    {return lusnrchr(s, c, MAX_USTRLEN);}
static inline ucstr_t ulsrchr(ucstr_t s, const lchar_t c)
    {return ulsnrchr(s, c, MAX_USTRLEN);}
static inline ucstr_t uusrchr(ucstr_t s, const uchar_t c)
    {return uusnrchr(s, c, MAX_USTRLEN);}

/**
 * @name llsrchrm, lusrchrm, ulsrchrm, and uusrchrm
 * @brief Case-sensitive search for the mth-from-last match of character
 * c in string s.
 * @param s Source string.
 * @param c Character to search for.
 * @param m Occurrence index from the end (1 = last, 2 = second-last, ...).
 * @return Pointer to mth-from-last match in s, or NULL if not found or on
 * source error.
 */
static inline lcstr_t llsrchrm(lcstr_t s, const lchar_t c, size_t m)
    {return llsnrchrm(s, c, MAX_LSTRLEN, m);}
static inline lcstr_t lusrchrm(lcstr_t s, const uchar_t c, size_t m)
    {return lusnrchrm(s, c, MAX_USTRLEN, m);}
static inline ucstr_t ulsrchrm(ucstr_t s, const lchar_t c, size_t m)
    {return ulsnrchrm(s, c, MAX_USTRLEN, m);}
static inline ucstr_t uusrchrm(ucstr_t s, const uchar_t c, size_t m)
    {return uusnrchrm(s, c, MAX_USTRLEN, m);}

/**
 * @name llsnRCHRM, lusnRCHRM, ulsnRCHRM, and uusnRCHRM
 * @brief Bounded case-insensitive search for the mth-from-last occurrence of
 * character c in string s.
 * @param s Source string.
 * @param c Character to search for.
 * @param n Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index from the end (1 = last, 2 = second-last, ...).
 * @return Pointer to mth-from-last match in s, or NULL if not found or on
 * source error.
 */
static inline lcstr_t llsnRCHRM(
    lcstr_t s, const lchar_t c, size_t n, size_t m) {
    if (!s || !m)
        return (lstr_t)NULL;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    size_t k = 0;
    lchar_t uc = tolupper(c);
    for (size_t ii = len + 1; ii; --ii) {
        size_t i = ii - 1;
        if (tolupper(s[i]) == uc && ++k == m) return s + i;
    }
    return (lstr_t)NULL;
}
static inline lcstr_t lusnRCHRM(
    lcstr_t s, const uchar_t c, size_t n, size_t m) {
    if (!s || !m)
        return (lstr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    if ((size_t)c > MAX_LCHAR)
        return (lstr_t)NULL;
    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    size_t k = 0;
    lchar_t uc = tolupper((lchar_t)c);
    for (size_t ii = len + 1; ii; --ii) {
        size_t i = ii - 1;
        if (tolupper(s[i]) == uc && ++k == m) return s + i;
    }
    return (lstr_t)NULL;
}
static inline ucstr_t ulsnRCHRM(
    ucstr_t s, const lchar_t c, size_t n, size_t m) {
    if (!s || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    size_t k = 0;
    uchar_t uc = touupper((uchar_t)c);
    for (size_t ii = len + 1; ii; --ii) {
        size_t i = ii - 1;
        if (touupper(s[i]) == uc && ++k == m) return s + i;
    }
    return (ustr_t)NULL;
}
static inline ucstr_t uusnRCHRM(
    ucstr_t s, const uchar_t c, size_t n, size_t m) {
    if (!s || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    size_t k = 0;
    uchar_t uc = touupper(c);
    for (size_t ii = len + 1; ii; --ii) {
        size_t i = ii - 1;
        if (touupper(s[i]) == uc && ++k == m) return s + i;
    }
    return (ustr_t)NULL;
}

/**
 * @name llsnRCHR, lusnRCHR, ulsnRCHR, and uusnRCHR
 * @brief Bounded case-insensitive search for the last occurrence of character
 * c in string s.
 * @param s Source string.
 * @param c Character to search for.
 * @return Pointer to last match in s, or NULL if not found or on source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llsnRCHR(lcstr_t s, const lchar_t c, size_t n)
    {return llsnRCHRM(s, c, n, 1);}
static inline lcstr_t lusnRCHR(lcstr_t s, const uchar_t c, size_t n)
    {return lusnRCHRM(s, c, n, 1);}
static inline ucstr_t ulsnRCHR(ucstr_t s, const lchar_t c, size_t n)
    {return ulsnRCHRM(s, c, n, 1);}
static inline ucstr_t uusnRCHR(ucstr_t s, const uchar_t c, size_t n)
    {return uusnRCHRM(s, c, n, 1);}

/**
 * @name llsRCHR, lusRCHR, ulsRCHR, and uusRCHR
 * @brief Case-insensitive search for the last occurrence of character
 * c in string s.
 * @param s Source string.
 * @param c Character to search for.
 * @return Pointer to last match in s, or NULL if not found or on source error.
 */
static inline lcstr_t llsRCHR(lcstr_t s, const lchar_t c)
    {return llsnRCHR(s, c, MAX_LSTRLEN);}
static inline lcstr_t lusRCHR(lcstr_t s, const uchar_t c)
    {return lusnRCHR(s, c, MAX_USTRLEN);}
static inline ucstr_t ulsRCHR(ucstr_t s, const lchar_t c)
    {return ulsnRCHR(s, c, MAX_USTRLEN);}
static inline ucstr_t uusRCHR(ucstr_t s, const uchar_t c)
    {return uusnRCHR(s, c, MAX_USTRLEN);}

/**
 * @name llsRCHRM, lusRCHRM, ulsRCHRM, and uusRCHRM
 * @brief Case-insensitive search for the mth-from-last occurrence of
 * character c in string s.
 * @param s Source string.
 * @param c Character to search for.
 * @param m Occurrence index from the end (1 = last, 2 = second-last, ...).
 * @return Pointer to mth-from-last match in s, or NULL if not found or on
 * source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llsRCHRM(lcstr_t s, const lchar_t c, size_t m)
    {return llsnRCHRM(s, c, MAX_LSTRLEN, m);}
static inline lcstr_t lusRCHRM(lcstr_t s, const uchar_t c, size_t m)
    {return lusnRCHRM(s, c, MAX_USTRLEN, m);}
static inline ucstr_t ulsRCHRM(ucstr_t s, const lchar_t c, size_t m)
    {return ulsnRCHRM(s, c, MAX_USTRLEN, m);}
static inline ucstr_t uusRCHRM(ucstr_t s, const uchar_t c, size_t m)
    {return uusnRCHRM(s, c, MAX_USTRLEN, m);}

/**
 * @name llsnpbrkm, lusnpbrkm, ulsnpbrkm, and uusnpbrkm
 * @brief Bounded case-sensitive search for the mth character in string s1 that
 * appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @param n Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index (1 = first, 2 = second, ...). 0 returns NULL.
 * @return Pointer to mth matching character in s1, or NULL if not found or
 * source error.
 */
static inline lcstr_t llsnpbrkm(
    lcstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (lstr_t)NULL;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    if (!s1_len || !s2_len)
        return (lstr_t)NULL;
    size_t k = 0;
    for (size_t i = 0; i < s1_len; ++i) {
        for (size_t j = 0; j < s2_len; ++j) {
            if (s1[i] == s2[j]) {
                if (++k == m) return s1 + i;
                break;
            }
        }
    }
    return (lstr_t)NULL;
}
static inline lcstr_t lusnpbrkm(
    lcstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (lstr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    if (!s1_len || !s2_len)
        return (lstr_t)NULL;
    size_t k = 0;
    for (size_t i = 0; i < s1_len; ++i) {
        uchar_t s1c = (uchar_t)s1[i];
        for (size_t j = 0; j < s2_len; ++j) {
            if (s1c == s2[j]) {
                if (++k == m) return s1 + i;
                break;
            }
        }
    }
    return (lstr_t)NULL;
}
static inline ucstr_t ulsnpbrkm(
    ucstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    if (!s1_len || !s2_len)
        return (ustr_t)NULL;
    size_t k = 0;
    for (size_t i = 0; i < s1_len; ++i) {
        for (size_t j = 0; j < s2_len; ++j) {
            if (s1[i] == (uchar_t)s2[j]) {
                if (++k == m) return s1 + i;
                break;
            }
        }
    }
    return (ustr_t)NULL;
}
static inline ucstr_t uusnpbrkm(
    ucstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    if (!s1_len || !s2_len)
        return (ustr_t)NULL;
    size_t k = 0;
    for (size_t i = 0; i < s1_len; ++i) {
        for (size_t j = 0; j < s2_len; ++j) {
            if (s1[i] == s2[j]) {
                if (++k == m) return s1 + i;
                break;
            }
        }
    }
    return (ustr_t)NULL;
}

/**
 * @name llsnpbrk, lusnpbrk, ulsnpbrk, and uusnpbrk
 * @brief Bounded case-sensitive search for the first character in string s1
 * that appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @param n Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Pointer to first matching character in s1, or NULL if no match or
 * source error.
 */
static inline lcstr_t llsnpbrk(lcstr_t s1, lcstr_t s2, size_t n)
    {return llsnpbrkm(s1, s2, n, 1);}
static inline lcstr_t lusnpbrk(lcstr_t s1, ucstr_t s2, size_t n)
    {return lusnpbrkm(s1, s2, n, 1);}
static inline ucstr_t ulsnpbrk(ucstr_t s1, lcstr_t s2, size_t n)
    {return ulsnpbrkm(s1, s2, n, 1);}
static inline ucstr_t uusnpbrk(ucstr_t s1, ucstr_t s2, size_t n)
    {return uusnpbrkm(s1, s2, n, 1);}

/**
 * @name llspbrk, luspbrk, ulspbrk, and uuspbrk
 * @brief Case-sensitive search for the first character in string s1 that
 * appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @return Pointer to first matching character in s1, or NULL if no match or
 * source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llspbrk(lcstr_t s1, lcstr_t s2)
    {return llsnpbrk(s1, s2, MAX_LSTRLEN);}
static inline lcstr_t luspbrk(lcstr_t s1, ucstr_t s2)
    {return lusnpbrk(s1, s2, MAX_USTRLEN);}
static inline ucstr_t ulspbrk(ucstr_t s1, lcstr_t s2)
    {return ulsnpbrk(s1, s2, MAX_USTRLEN);}
static inline ucstr_t uuspbrk(ucstr_t s1, ucstr_t s2)
    {return uusnpbrk(s1, s2, MAX_USTRLEN);}

/**
 * @name llsnPBRKCM, lusnPBRKCM, ulsnPBRKCM, and uusnPBRKCM
 * @brief Bounded case-insensitive search for the mth character in string s1
 * that appears in s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @param n Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index (1 = first, 2 = second, ...). 0 returns NULL.
 * @return Pointer to mth matching character in s1, or NULL if not found or
 * source error.
 */
static inline lcstr_t llsnPBRKCM(
    lcstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (lstr_t)NULL;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    if (!s1_len || !s2_len)
        return (lstr_t)NULL;
    size_t k = 0;
    for (size_t i = 0; i < s1_len; ++i) {
        lchar_t s1c = tolupper(s1[i]);
        for (size_t j = 0; j < s2_len; ++j) {
            if (s1c == tolupper(s2[j])) {
                if (++k == m) return s1 + i;
                break;
            }
        }
    }
    return (lstr_t)NULL;
}
static inline lcstr_t lusnPBRKCM(
    lcstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (lstr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    if (!s1_len || !s2_len)
        return (lstr_t)NULL;
    size_t k = 0;
    for (size_t i = 0; i < s1_len; ++i) {
        uchar_t s1c = touupper((uchar_t)s1[i]);
        for (size_t j = 0; j < s2_len; ++j) {
            if (s1c == touupper(s2[j])) {
                if (++k == m) return s1 + i;
                break;
            }
        }
    }
    return (lstr_t)NULL;
}
static inline ucstr_t ulsnPBRKCM(
    ucstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    if (!s1_len || !s2_len)
        return (ustr_t)NULL;
    size_t k = 0;
    for (size_t i = 0; i < s1_len; ++i) {
        uchar_t s1c = touupper(s1[i]);
        for (size_t j = 0; j < s2_len; ++j) {
            if (s1c == touupper((uchar_t)s2[j])) {
                if (++k == m) return s1 + i;
                break;
            }
        }
    }
    return (ustr_t)NULL;
}
static inline ucstr_t uusnPBRKCM(
    ucstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    if (!s1_len || !s2_len)
        return (ustr_t)NULL;
    size_t k = 0;
    for (size_t i = 0; i < s1_len; ++i) {
        uchar_t s1c = touupper(s1[i]);
        for (size_t j = 0; j < s2_len; ++j) {
            if (s1c == touupper(s2[j])) {
                if (++k == m) return s1 + i;
                break;
            }
        }
    }
    return (ustr_t)NULL;
}

/**
 * @name llsnPBRKC, lusnPBRKC, ulsnPBRKC, and uusnPBRKC
 * @brief Bounded case-insensitive search for the first character in string s1
 * that appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @param n Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Pointer to first matching character in s1, or NULL if no match or
 * source error.
 */
static inline lcstr_t llsnPBRKC(lcstr_t s1, lcstr_t s2, size_t n)
    {return llsnPBRKCM(s1, s2, n, 1);}
static inline lcstr_t lusnPBRKC(lcstr_t s1, ucstr_t s2, size_t n)
    {return lusnPBRKCM(s1, s2, n, 1);}
static inline ucstr_t ulsnPBRKC(ucstr_t s1, lcstr_t s2, size_t n)
    {return ulsnPBRKCM(s1, s2, n, 1);}
static inline ucstr_t uusnPBRKC(ucstr_t s1, ucstr_t s2, size_t n)
    {return uusnPBRKCM(s1, s2, n, 1);}

/**
 * @name llsPBRKC, lusPBRKC, ulsPBRKC, and uusPBRKC
 * @brief Case-insensitive search for the first character in string s1 that
 * appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @return Pointer to first matching character in s1, or NULL if no match or
 * source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llsPBRKC(lcstr_t s1, lcstr_t s2)
    {return llsnPBRKC(s1, s2, MAX_LSTRLEN);}
static inline lcstr_t lusPBRKC(lcstr_t s1, ucstr_t s2)
    {return lusnPBRKC(s1, s2, MAX_USTRLEN);}
static inline ucstr_t ulsPBRKC(ucstr_t s1, lcstr_t s2)
    {return ulsnPBRKC(s1, s2, MAX_USTRLEN);}
static inline ucstr_t uusPBRKC(ucstr_t s1, ucstr_t s2)
    {return uusnPBRKC(s1, s2, MAX_USTRLEN);}


/**
 * @name llspbrkm, luspbrkm, ulspbrkm, and uuspbrkm
 * @brief Case-sensitive search for the mth character in string s1 that
 * appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @param m Occurrence index (1 = first, 2 = second, ...). 0 returns NULL.
 * @return Pointer to mth matching character in s1, or NULL if not found or
 * source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llspbrkm(lcstr_t s1, lcstr_t s2, size_t m)
    {return llsnpbrkm(s1, s2, MAX_LSTRLEN, m);}
static inline lcstr_t luspbrkm(lcstr_t s1, ucstr_t s2, size_t m)
    {return lusnpbrkm(s1, s2, MAX_USTRLEN, m);}
static inline ucstr_t ulspbrkm(ucstr_t s1, lcstr_t s2, size_t m)
    {return ulsnpbrkm(s1, s2, MAX_USTRLEN, m);}
static inline ucstr_t uuspbrkm(ucstr_t s1, ucstr_t s2, size_t m)
    {return uusnpbrkm(s1, s2, MAX_USTRLEN, m);}


/**
 * @name llsPBRKCM, lusPBRKCM, ulsPBRKCM, and uusPBRKCM
 * @brief Case-insensitive search for the mth character in string s1 that
 * appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @param m Occurrence index (1 = first, 2 = second, ...). 0 returns NULL.
 * @return Pointer to mth matching character in s1, or NULL if not found or
 * source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llsPBRKCM(lcstr_t s1, lcstr_t s2, size_t m)
    {return llsnPBRKCM(s1, s2, MAX_LSTRLEN, m);}
static inline lcstr_t lusPBRKCM(lcstr_t s1, ucstr_t s2, size_t m)
    {return lusnPBRKCM(s1, s2, MAX_USTRLEN, m);}
static inline ucstr_t ulsPBRKCM(ucstr_t s1, lcstr_t s2, size_t m)
    {return ulsnPBRKCM(s1, s2, MAX_USTRLEN, m);}
static inline ucstr_t uusPBRKCM(ucstr_t s1, ucstr_t s2, size_t m)
    {return uusnPBRKCM(s1, s2, MAX_USTRLEN, m);}

/**
 * @name llsnrpbrkm, lusnrpbrkm, ulsnrpbrkm, and uusnrpbrkm
 * @brief Bounded case-sensitive search for the mth-from-last character in
 * string s1 that appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @param n Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index from the end (1 = last, 2 = second-last, ...).
 * @return Pointer to mth-from-last matching character in s1, or NULL if no
 * match or source error.
 */
static inline lcstr_t llsnrpbrkm(
    lcstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (lstr_t)NULL;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    if (!s1_len || !s2_len)
        return (lstr_t)NULL;
    size_t k = 0;
    for (size_t i = s1_len; i > 0; --i) {
        size_t idx = i - 1;
        for (size_t j = 0; j < s2_len; ++j) {
            if (s1[idx] == s2[j]) {
                if (++k == m) return s1 + idx;
                break;
            }
        }
    }
    return (lstr_t)NULL;
}
static inline lcstr_t lusnrpbrkm(
    lcstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (lstr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    if (!s1_len || !s2_len)
        return (lstr_t)NULL;
    size_t k = 0;
    for (size_t i = s1_len; i > 0; --i) {
        size_t idx = i - 1;
        uchar_t s1c = (uchar_t)s1[idx];
        for (size_t j = 0; j < s2_len; ++j) {
            if (s1c == s2[j]) {
                if (++k == m) return s1 + idx;
                break;
            }
        }
    }
    return (lstr_t)NULL;
}
static inline ucstr_t ulsnrpbrkm(
    ucstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    if (!s1_len || !s2_len)
        return (ustr_t)NULL;
    size_t k = 0;
    for (size_t i = s1_len; i > 0; --i) {
        size_t idx = i - 1;
        for (size_t j = 0; j < s2_len; ++j) {
            if (s1[idx] == (uchar_t)s2[j]) {
                if (++k == m) return s1 + idx;
                break;
            }
        }
    }
    return (ustr_t)NULL;
}
static inline ucstr_t uusnrpbrkm(
    ucstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    if (!s1_len || !s2_len)
        return (ustr_t)NULL;
    size_t k = 0;
    for (size_t i = s1_len; i > 0; --i) {
        size_t idx = i - 1;
        for (size_t j = 0; j < s2_len; ++j) {
            if (s1[idx] == s2[j]) {
                if (++k == m) return s1 + idx;
                break;
            }
        }
    }
    return (ustr_t)NULL;
}

/**
 * @name llsnrpbrk, lusnrpbrk, ulsnrpbrk, and uusnrpbrk
 * @brief Bounded case-sensitive search for the last character in string s1 that
 * appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @param n Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Pointer to last matching character in s1, or NULL if no match or
 * source error.
 */
static inline lcstr_t llsnrpbrk(lcstr_t s1, lcstr_t s2, size_t n)
    {return llsnrpbrkm(s1, s2, n, 1);}
static inline lcstr_t lusnrpbrk(lcstr_t s1, ucstr_t s2, size_t n)
    {return lusnrpbrkm(s1, s2, n, 1);}
static inline ucstr_t ulsnrpbrk(ucstr_t s1, lcstr_t s2, size_t n)
    {return ulsnrpbrkm(s1, s2, n, 1);}
static inline ucstr_t uusnrpbrk(ucstr_t s1, ucstr_t s2, size_t n)
    {return uusnrpbrkm(s1, s2, n, 1);}

/**
 * @name llsrpbrk, lusrpbrk, ulsrpbrk, and uusrpbrk
 * @brief Case-sensitive search for the last character in string s1 that
 * appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @return Pointer to last matching character in s1, or NULL if no match or
 * source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llsrpbrk(lcstr_t s1, lcstr_t s2)
    {return llsnrpbrk(s1, s2, MAX_LSTRLEN);}
static inline lcstr_t lusrpbrk(lcstr_t s1, ucstr_t s2)
    {return lusnrpbrk(s1, s2, MAX_USTRLEN);}
static inline ucstr_t ulsrpbrk(ucstr_t s1, lcstr_t s2)
    {return ulsnrpbrk(s1, s2, MAX_USTRLEN);}
static inline ucstr_t uusrpbrk(ucstr_t s1, ucstr_t s2)
    {return uusnrpbrk(s1, s2, MAX_USTRLEN);}

/**
 * @name llsrpbrkm, lusrpbrkm, ulsrpbrkm, and uusrpbrkm
 * @brief Case-sensitive search for the mth-from-last character in s1 that
 * appears in s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @param m Occurrence index from the end (1 = last, 2 = second-last, ...).
 * @return Pointer to mth-from-last matching character in s1, or NULL if no
 * match or source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llsrpbrkm(lcstr_t s1, lcstr_t s2, size_t m)
    {return llsnrpbrkm(s1, s2, MAX_LSTRLEN, m);}
static inline lcstr_t lusrpbrkm(lcstr_t s1, ucstr_t s2, size_t m)
    {return lusnrpbrkm(s1, s2, MAX_USTRLEN, m);}
static inline ucstr_t ulsrpbrkm(ucstr_t s1, lcstr_t s2, size_t m)
    {return ulsnrpbrkm(s1, s2, MAX_USTRLEN, m);}
static inline ucstr_t uusrpbrkm(ucstr_t s1, ucstr_t s2, size_t m)
    {return uusnrpbrkm(s1, s2, MAX_USTRLEN, m);}

/**
 * @name llsnRPBRKCM, lusnRPBRKCM, ulsnRPBRKCM, and uusnRPBRKCM
 * @brief Bounded case-insensitive search for the mth-from-last character in
 * string s1 that appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @param n Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index from the end (1 = last, 2 = second-last, ...).
 * @return Pointer to mth-from-last matching character in s1, or NULL if no
 * match or source error.
 */
static inline lcstr_t llsnRPBRKCM(
    lcstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (lstr_t)NULL;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    if (!s1_len || !s2_len)
        return (lstr_t)NULL;
    size_t k = 0;
    for (size_t i = s1_len; i > 0; --i) {
        size_t idx = i - 1;
        lchar_t s1c = tolupper(s1[idx]);
        for (size_t j = 0; j < s2_len; ++j) {
            if (s1c == tolupper(s2[j])) {
                if (++k == m) return s1 + idx;
                break;
            }
        }
    }
    return (lstr_t)NULL;
}
static inline lcstr_t lusnRPBRKCM(
    lcstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (lstr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    if (!s1_len || !s2_len)
        return (lstr_t)NULL;
    size_t k = 0;
    for (size_t i = s1_len; i > 0; --i) {
        size_t idx = i - 1;
        uchar_t s1c = touupper((uchar_t)s1[idx]);
        for (size_t j = 0; j < s2_len; ++j) {
            if (s1c == touupper(s2[j])) {
                if (++k == m) return s1 + idx;
                break;
            }
        }
    }
    return (lstr_t)NULL;
}
static inline ucstr_t ulsnRPBRKCM(
    ucstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    if (!s1_len || !s2_len)
        return (ustr_t)NULL;
    size_t k = 0;
    for (size_t i = s1_len; i > 0; --i) {
        size_t idx = i - 1;
        uchar_t s1c = touupper(s1[idx]);
        for (size_t j = 0; j < s2_len; ++j) {
            if (s1c == touupper((uchar_t)s2[j])) {
                if (++k == m) return s1 + idx;
                break;
            }
        }
    }
    return (ustr_t)NULL;
}
static inline ucstr_t uusnRPBRKCM(
    ucstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    if (!s1_len || !s2_len)
        return (ustr_t)NULL;
    size_t k = 0;
    for (size_t i = s1_len; i > 0; --i) {
        size_t idx = i - 1;
        uchar_t s1c = touupper(s1[idx]);
        for (size_t j = 0; j < s2_len; ++j) {
            if (s1c == touupper(s2[j])) {
                if (++k == m) return s1 + idx;
                break;
            }
        }
    }
    return (ustr_t)NULL;
}

/**
 * @name llsnRPBRKC, lusnRPBRKC, ulsnRPBRKC, and uusnRPBRKC
 * @brief Bounded case-insensitive search for the last character in
 * string s1 that appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @param n Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Pointer to last matching character in s1, or NULL if no match or
 * source error.
 */
static inline lcstr_t llsnRPBRKC(lcstr_t s1, lcstr_t s2, size_t n)
    {return llsnRPBRKCM(s1, s2, n, 1);}
static inline lcstr_t lusnRPBRKC(lcstr_t s1, ucstr_t s2, size_t n)
    {return lusnRPBRKCM(s1, s2, n, 1);}
static inline ucstr_t ulsnRPBRKC(ucstr_t s1, lcstr_t s2, size_t n)
    {return ulsnRPBRKCM(s1, s2, n, 1);}
static inline ucstr_t uusnRPBRKC(ucstr_t s1, ucstr_t s2, size_t n)
    {return uusnRPBRKCM(s1, s2, n, 1);}

/**
 * @name llsRPBRKC, lusRPBRKC, ulsRPBRKC, and uusRPBRKC
 * @brief Case-insensitive search for the last character in string s1 that
 * appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @return Pointer to last matching character in s1, or NULL if no match or
 * source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llsRPBRKC(lcstr_t s1, lcstr_t s2)
    {return llsnRPBRKC(s1, s2, MAX_LSTRLEN);}
static inline lcstr_t lusRPBRKC(lcstr_t s1, ucstr_t s2)
    {return lusnRPBRKC(s1, s2, MAX_USTRLEN);}
static inline ucstr_t ulsRPBRKC(ucstr_t s1, lcstr_t s2)
    {return ulsnRPBRKC(s1, s2, MAX_USTRLEN);}
static inline ucstr_t uusRPBRKC(ucstr_t s1, ucstr_t s2)
    {return uusnRPBRKC(s1, s2, MAX_USTRLEN);}

/**
 * @name llsRPBRKCM, lusRPBRKCM, ulsRPBRKCM, and uusRPBRKCM
 * @brief Case-insensitive search for the mth-from-last character in string s1
 * that appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @param m Occurrence index from the end (1 = last, 2 = second-last, ...).
 * @return Pointer to mth-from-last matching character in s1, or NULL if no
 * match or source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llsRPBRKCM(lcstr_t s1, lcstr_t s2, size_t m)
    {return llsnRPBRKCM(s1, s2, MAX_LSTRLEN, m);}
static inline lcstr_t lusRPBRKCM(lcstr_t s1, ucstr_t s2, size_t m)
    {return lusnRPBRKCM(s1, s2, MAX_USTRLEN, m);}
static inline ucstr_t ulsRPBRKCM(ucstr_t s1, lcstr_t s2, size_t m)
    {return ulsnRPBRKCM(s1, s2, MAX_USTRLEN, m);}
static inline ucstr_t uusRPBRKCM(ucstr_t s1, ucstr_t s2, size_t m)
    {return uusnRPBRKCM(s1, s2, MAX_USTRLEN, m);}

/**
 * @name llsnspn, lusnspn, ulsnspn, and uusnspn
 * @brief Bounded case-sensitive initial-span length where characters in
 * string s1 are all in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string.
 * @param n Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Span length, or MISSING_STR_TERMINATOR on source error.
 */
static inline size_t llsnspn(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s1_len ||
        !s2_len)
        return (size_t)0;
    for (size_t i = 0; i < s1_len; ++i) {
        int in_set = 0;
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1[i] == s2[k]) {in_set = 1; break;}
        }
        if (!in_set)
            return i;
    }
    return s1_len;
}
static inline size_t lusnspn(lcstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s1_len ||
        !s2_len)
        return (size_t)0;
    for (size_t i = 0; i < s1_len; ++i) {
        int in_set = 0;
        uchar_t s1c = (uchar_t)s1[i];
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1c == s2[k]) {in_set = 1; break;}
        }
        if (!in_set)
            return i;
    }
    return s1_len;
}
static inline size_t ulsnspn(ucstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s1_len ||
        !s2_len)
        return (size_t)0;
    for (size_t i = 0; i < s1_len; ++i) {
        int in_set = 0;
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1[i] == (uchar_t)s2[k]) {in_set = 1; break;}
        }
        if (!in_set)
            return i;
    }
    return s1_len;
}
static inline size_t uusnspn(ucstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s1_len ||
        !s2_len)
        return (size_t)0;
    for (size_t i = 0; i < s1_len; ++i) {
        int in_set = 0;
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1[i] == s2[k]) {in_set = 1; break;}
        }
        if (!in_set)
            return i;
    }
    return s1_len;
}

/**
 * @name llsspn, lusspn, ulsspn, and uusspn
 * @brief Case-sensitive initial-span length where characters in
 * string s1 are all in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string.
 * @return Span length, or MISSING_STR_TERMINATOR on source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline size_t llsspn(lcstr_t s1, lcstr_t s2)
    {return llsnspn(s1, s2, MAX_LSTRLEN);}
static inline size_t lusspn(lcstr_t s1, ucstr_t s2)
    {return lusnspn(s1, s2, MAX_USTRLEN);}
static inline size_t ulsspn(ucstr_t s1, lcstr_t s2)
    {return ulsnspn(s1, s2, MAX_USTRLEN);}
static inline size_t uusspn(ucstr_t s1, ucstr_t s2)
    {return uusnspn(s1, s2, MAX_USTRLEN);}

/**
 * @name llsnSPNC, lusnSPNC, ulsnSPNC, and uusnSPNC
 * @brief Bounded case-insensitive initial-span length where characters in
 * string s1 are all in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string.
 * @param n Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Span length, or MISSING_STR_TERMINATOR on source error.
 */
static inline size_t llsnSPNC(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s1_len ||
        !s2_len)
        return (size_t)0;
    for (size_t i = 0; i < s1_len; ++i) {
        int in_set = 0;
        lchar_t s1c = tolupper(s1[i]);
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1c == tolupper(s2[k])) {in_set = 1; break;}
        }
        if (!in_set)
            return i;
    }
    return s1_len;
}
static inline size_t lusnSPNC(lcstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s1_len ||
        !s2_len)
        return (size_t)0;
    for (size_t i = 0; i < s1_len; ++i) {
        int in_set = 0;
        uchar_t s1c = touupper((uchar_t)s1[i]);
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1c == touupper(s2[k])) {in_set = 1; break;}
        }
        if (!in_set)
            return i;
    }
    return s1_len;
}
static inline size_t ulsnSPNC(ucstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s1_len ||
        !s2_len)
        return (size_t)0;
    for (size_t i = 0; i < s1_len; ++i) {
        int in_set = 0;
        uchar_t s1c = touupper(s1[i]);
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1c == touupper((uchar_t)s2[k])) {in_set = 1; break;}
        }
        if (!in_set)
            return i;
    }
    return s1_len;
}
static inline size_t uusnSPNC(ucstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s1_len ||
        !s2_len)
        return (size_t)0;
    for (size_t i = 0; i < s1_len; ++i) {
        int in_set = 0;
        uchar_t s1c = touupper(s1[i]);
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1c == touupper(s2[k])) {in_set = 1; break;}
        }
        if (!in_set)
            return i;
    }
    return s1_len;
}

/**
 * @name llsSPNC, lusSPNC, ulsSPNC, and uusSPNC
 * @brief Case-insensitive initial-span length where characters
 * in string s1 are all in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string.
 * @return Span length, or MISSING_STR_TERMINATOR on source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline size_t llsSPNC(lcstr_t s1, lcstr_t s2)
    {return llsnSPNC(s1, s2, MAX_LSTRLEN);}
static inline size_t lusSPNC(lcstr_t s1, ucstr_t s2)
    {return lusnSPNC(s1, s2, MAX_USTRLEN);}
static inline size_t ulsSPNC(ucstr_t s1, lcstr_t s2)
    {return ulsnSPNC(s1, s2, MAX_USTRLEN);}
static inline size_t uusSPNC(ucstr_t s1, ucstr_t s2)
    {return uusnSPNC(s1, s2, MAX_USTRLEN);}

/**
 * @name llsncspn, lusncspn, ulsncspn, and uusncspn
 * @brief Bounded case-sensitive initial-span length where characters
 * in string s1 are not in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string.
 * @param n Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Span length, or MISSING_STR_TERMINATOR on source error.
 */
static inline size_t llsncspn(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s1_len ||
        !s2_len)
        return s1_len;
    for (size_t i = 0; i < s1_len; ++i) {
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1[i] == s2[k])
                return i;
        }
    }
    return s1_len;
}
static inline size_t lusncspn(lcstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s1_len ||
        !s2_len)
        return s1_len;
    for (size_t i = 0; i < s1_len; ++i) {
        uchar_t s1c = (uchar_t)s1[i];
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1c == s2[k])
                return i;
        }
    }
    return s1_len;
}
static inline size_t ulsncspn(ucstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s1_len ||
        !s2_len)
        return s1_len;
    for (size_t i = 0; i < s1_len; ++i) {
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1[i] == (uchar_t)s2[k])
                return i;
        }
    }
    return s1_len;
}
static inline size_t uusncspn(ucstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s1_len ||
        !s2_len)
        return s1_len;
    for (size_t i = 0; i < s1_len; ++i) {
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1[i] == s2[k])
                return i;
        }
    }
    return s1_len;
}

/**
 * @name llscspn, luscspn, ulscspn, and uuscspn
 * @brief Case-sensitive initial-span length where characters in string s1
 * are not in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string.
 * @return Span length, or MISSING_STR_TERMINATOR on source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline size_t llscspn(lcstr_t s1, lcstr_t s2)
    {return llsncspn(s1, s2, MAX_LSTRLEN);}
static inline size_t luscspn(lcstr_t s1, ucstr_t s2)
    {return lusncspn(s1, s2, MAX_USTRLEN);}
static inline size_t ulscspn(ucstr_t s1, lcstr_t s2)
    {return ulsncspn(s1, s2, MAX_USTRLEN);}
static inline size_t uuscspn(ucstr_t s1, ucstr_t s2)
    {return uusncspn(s1, s2, MAX_USTRLEN);}

/**
 * @name llsnCSPNC, lusnCSPNC, ulsnCSPNC, and uusnCSPNC
 * @brief Bounded case-insensitive initial-span length where characters in
 * string s1 are not in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string.
 * @param n Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Span length, or MISSING_STR_TERMINATOR on source error.
 */
static inline size_t llsnCSPNC(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s1_len ||
        !s2_len)
        return s1_len;
    for (size_t i = 0; i < s1_len; ++i) {
        lchar_t s1c = tolupper(s1[i]);
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1c == tolupper(s2[k]))
                return i;
        }
    }
    return s1_len;
}
static inline size_t lusnCSPNC(lcstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s1_len ||
        !s2_len)
        return s1_len;
    for (size_t i = 0; i < s1_len; ++i) {
        uchar_t s1c = touupper((uchar_t)s1[i]);
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1c == touupper(s2[k]))
                return i;
        }
    }
    return s1_len;
}
static inline size_t ulsnCSPNC(ucstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s1_len ||
        !s2_len)
        return s1_len;
    for (size_t i = 0; i < s1_len; ++i) {
        uchar_t s1c = touupper(s1[i]);
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1c == touupper((uchar_t)s2[k]))
                return i;
        }
    }
    return s1_len;
}
static inline size_t uusnCSPNC(ucstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s1_len ||
        !s2_len)
        return s1_len;
    for (size_t i = 0; i < s1_len; ++i) {
        uchar_t s1c = touupper(s1[i]);
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1c == touupper(s2[k]))
                return i;
        }
    }
    return s1_len;
}

/**
 * @name llsCSPNC, lusCSPNC, ulsCSPNC, and uusCSPNC
 * @brief Case-insensitive initial-span length where characters
 * in string s1 are not in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string.
 * @return Span length, or MISSING_STR_TERMINATOR on source error.
 * @note Default source bounds applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline size_t llsCSPNC(lcstr_t s1, lcstr_t s2)
    {return llsnCSPNC(s1, s2, MAX_LSTRLEN);}
static inline size_t lusCSPNC(lcstr_t s1, ucstr_t s2)
    {return lusnCSPNC(s1, s2, MAX_USTRLEN);}
static inline size_t ulsCSPNC(ucstr_t s1, lcstr_t s2)
    {return ulsnCSPNC(s1, s2, MAX_USTRLEN);}
static inline size_t uusCSPNC(ucstr_t s1, ucstr_t s2)
    {return uusnCSPNC(s1, s2, MAX_USTRLEN);}

/**
 * @name llsncnt, lusncnt, ulsncnt, and uusncnt
 * @brief Bounded case-sensitive count of matches of character c in string s.
 * @param s Source string.
 * @param c Character to count.
 * @param n Bound on source clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Number of matches in s, or MISSING_STR_TERMINATOR on source error.
 * @note The null terminator is not included in the count.
 */
static inline size_t llsncnt(lcstr_t s, const lchar_t c, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    size_t k = 0;
    for (size_t i = 0; i < len; ++i) if (s[i] == c) ++k;
    return k;
}
static inline size_t lusncnt(lcstr_t s, const uchar_t c, size_t n) {
    if ((size_t)c > MAX_LCHAR) return (size_t)0;
    return llsncnt(s, (lchar_t)c, n > MAX_USTRLEN ? MAX_USTRLEN : n);
}
static inline size_t ulsncnt(ucstr_t s, const lchar_t c, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    size_t k = 0;
    uchar_t uc = (uchar_t)c;
    for (size_t i = 0; i < len; ++i) if (s[i] == uc) ++k;
    return k;
}
static inline size_t uusncnt(ucstr_t s, const uchar_t c, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    size_t k = 0;
    for (size_t i = 0; i < len; ++i) if (s[i] == c) ++k;
    return k;
}

/**
 * @name llscnt, luscnt, ulscnt, and uuscnt
 * @brief Case-sensitive count of matches of character c in string s.
 * @param s Source string.
 * @param c Character to count.
 * @return Number of matches in s, or MISSING_STR_TERMINATOR on source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline size_t llscnt(lcstr_t s, const lchar_t c)
    {return llsncnt(s, c, MAX_LSTRLEN);}
static inline size_t luscnt(lcstr_t s, const uchar_t c)
    {return lusncnt(s, c, MAX_USTRLEN);}
static inline size_t ulscnt(ucstr_t s, const lchar_t c)
    {return ulsncnt(s, c, MAX_USTRLEN);}
static inline size_t uuscnt(ucstr_t s, const uchar_t c)
    {return uusncnt(s, c, MAX_USTRLEN);}

/**
 * @name llsnCNTC, lusnCNTC, ulsnCNTC, and uusnCNTC
 * @brief Bounded case-insensitive count of matches of character c in string s.
 * @param s Source string.
 * @param c Character to count.
 * @param n Bound on source (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Count of matches in s, or MISSING_STR_TERMINATOR on source error.
 * @note The null terminator is not included in the count.
 */
static inline size_t llsnCNTC(lcstr_t s, const lchar_t c, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    size_t k = 0;
    lchar_t uc = tolupper(c);
    for (size_t i = 0; i < len; ++i) if (tolupper(s[i]) == uc) ++k;
    return k;
}
static inline size_t lusnCNTC(lcstr_t s, const uchar_t c, size_t n) {
    if ((size_t)c > MAX_LCHAR) return (size_t)0;
    return llsnCNTC(s, (lchar_t)c, n > MAX_USTRLEN ? MAX_USTRLEN : n);
}
static inline size_t ulsnCNTC(ucstr_t s, const lchar_t c, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    size_t k = 0;
    uchar_t uc = touupper((uchar_t)c);
    for (size_t i = 0; i < len; ++i) if (touupper(s[i]) == uc) ++k;
    return k;
}
static inline size_t uusnCNTC(ucstr_t s, const uchar_t c, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    size_t k = 0;
    uchar_t uc = touupper(c);
    for (size_t i = 0; i < len; ++i) if (touupper(s[i]) == uc) ++k;
    return k;
}

/**
 * @name llsCNTC, lusCNTC, ulsCNTC, and uusCNTC
 * @brief Case-insensitive count of matches of character c in string s.
 * @param s Source string.
 * @param c Character to count.
 * @return Number of matches, or MISSING_STR_TERMINATOR on source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline size_t llsCNTC(lcstr_t s, const lchar_t c)
    {return llsnCNTC(s, c, MAX_LSTRLEN);}
static inline size_t lusCNTC(lcstr_t s, const uchar_t c)
    {return lusnCNTC(s, c, MAX_USTRLEN);}
static inline size_t ulsCNTC(ucstr_t s, const lchar_t c)
    {return ulsnCNTC(s, c, MAX_USTRLEN);}
static inline size_t uusCNTC(ucstr_t s, const uchar_t c)
    {return uusnCNTC(s, c, MAX_USTRLEN);}

/**
 * @name llsnstrm, lusnstrm, ulsnstrm, and uusnstrm
 * @brief Bounded case-sensitive search for the mth occurrence of
 * string s2 in string s1.
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param n Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index (1 = first, 2 = second, ...). 0 returns NULL.
 * @return Pointer to mth match in s1, or NULL if not found or on
 * source error.
 */
static inline lcstr_t llsnstrm(
    lcstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (lstr_t)NULL;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    if (!s2_len)
        return m == 1 ? s1 : (lstr_t)NULL;
    if (s2_len > s1_len)
        return (lstr_t)NULL;
    size_t count = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && s1[i + k] == s2[k]; ++k);
        if (k == s2_len) {
            if (++count == m) return s1 + i;
        }
    }
    return (lstr_t)NULL;
}
static inline lcstr_t lusnstrm(
    lcstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (lstr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    if (!s2_len)
        return m == 1 ? s1 : (lstr_t)NULL;
    if (s2_len > s1_len)
        return (lstr_t)NULL;
    size_t count = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && (uchar_t)s1[i + k] == s2[k]; ++k);
        if (k == s2_len) {
            if (++count == m) return s1 + i;
        }
    }
    return (lstr_t)NULL;
}
static inline ucstr_t ulsnstrm(
    ucstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    if (!s2_len)
        return m == 1 ? s1 : (ustr_t)NULL;
    if (s2_len > s1_len)
        return (ustr_t)NULL;
    size_t count = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && s1[i + k] == (uchar_t)s2[k]; ++k);
        if (k == s2_len) {
            if (++count == m) return s1 + i;
        }
    }
    return (ustr_t)NULL;
}
static inline ucstr_t uusnstrm(
    ucstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    if (!s2_len)
        return m == 1 ? s1 : (ustr_t)NULL;
    if (s2_len > s1_len)
        return (ustr_t)NULL;
    size_t count = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && s1[i + k] == s2[k]; ++k);
        if (k == s2_len) {
            if (++count == m) return s1 + i;
        }
    }
    return (ustr_t)NULL;
}

/**
 * @name llsnstr, lusnstr, ulsnstr, and uusnstr
 * @brief Bounded case-sensitive search of string s2 in string s1.
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param n Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Pointer to first match in s1, or NULL if not found or on source
 * error.
 */
static inline lcstr_t llsnstr(lcstr_t s1, lcstr_t s2, size_t n)
    {return llsnstrm(s1, s2, n, 1);}
static inline lcstr_t lusnstr(lcstr_t s1, ucstr_t s2, size_t n)
    {return lusnstrm(s1, s2, n, 1);}
static inline ucstr_t ulsnstr(ucstr_t s1, lcstr_t s2, size_t n)
    {return ulsnstrm(s1, s2, n, 1);}
static inline ucstr_t uusnstr(ucstr_t s1, ucstr_t s2, size_t n)
    {return uusnstrm(s1, s2, n, 1);}

/**
 * @name llsstr, lusstr, ulsstr, and uusstr
 * @brief Case-sensitive search of string s2 in string s1.
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @return Pointer to first match in s1, or NULL if not found or on source
 * error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llsstr(lcstr_t s1, lcstr_t s2)
    {return llsnstr(s1, s2, MAX_LSTRLEN);}
static inline lcstr_t lusstr(lcstr_t s1, ucstr_t s2)
    {return lusnstr(s1, s2, MAX_USTRLEN);}
static inline ucstr_t ulsstr(ucstr_t s1, lcstr_t s2)
    {return ulsnstr(s1, s2, MAX_USTRLEN);}
static inline ucstr_t uusstr(ucstr_t s1, ucstr_t s2)
    {return uusnstr(s1, s2, MAX_USTRLEN);}

/**
 * @name llsnSTRCM, lusnSTRCM, ulsnSTRCM, and uusnSTRCM
 * @brief Bounded case-insensitive search for the mth occurrence of string s2
 * in string s1.
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param n Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index (1 = first, 2 = second, ...). 0 returns NULL.
 * @return Pointer to mth match in s1, or NULL if not found or on source
 * error.
 */
static inline lcstr_t llsnSTRCM(
    lcstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (lstr_t)NULL;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    if (!s2_len)
        return m == 1 ? s1 : (lstr_t)NULL;
    if (s2_len > s1_len)
        return (lstr_t)NULL;
    size_t count = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && tolupper(s1[i + k]) == tolupper(s2[k]); ++k);
        if (k == s2_len) {
            if (++count == m) return s1 + i;
        }
    }
    return (lstr_t)NULL;
}
static inline lcstr_t lusnSTRCM(
    lcstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (lstr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    if (!s2_len)
        return m == 1 ? s1 : (lstr_t)NULL;
    if (s2_len > s1_len)
        return (lstr_t)NULL;
    size_t count = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len &&
             touupper((uchar_t)s1[i + k]) == touupper(s2[k]); ++k);
        if (k == s2_len) {
            if (++count == m) return s1 + i;
        }
    }
    return (lstr_t)NULL;
}
static inline ucstr_t ulsnSTRCM(
    ucstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    if (!s2_len)
        return m == 1 ? s1 : (ustr_t)NULL;
    if (s2_len > s1_len)
        return (ustr_t)NULL;
    size_t count = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len &&
             touupper(s1[i + k]) == touupper((uchar_t)s2[k]); ++k);
        if (k == s2_len) {
            if (++count == m) return s1 + i;
        }
    }
    return (ustr_t)NULL;
}
static inline ucstr_t uusnSTRCM(
    ucstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR ||
        s2_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    if (!s2_len)
        return m == 1 ? s1 : (ustr_t)NULL;
    if (s2_len > s1_len)
        return (ustr_t)NULL;
    size_t count = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && touupper(s1[i + k]) == touupper(s2[k]); ++k);
        if (k == s2_len) {
            if (++count == m) return s1 + i;
        }
    }
    return (ustr_t)NULL;
}

/**
 * @name llsnSTRC, lusnSTRC, ulsnSTRC, and uusnSTRC
 * @brief Bounded case-insensitive search of string s2 in string s1.
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param n Bound on s1/s2  (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Pointer to first match in s1, or NULL if not found or on
 * source error.
 */
static inline lcstr_t llsnSTRC(lcstr_t s1, lcstr_t s2, size_t n)
    {return llsnSTRCM(s1, s2, n, 1);}
static inline lcstr_t lusnSTRC(lcstr_t s1, ucstr_t s2, size_t n)
    {return lusnSTRCM(s1, s2, n, 1);}
static inline ucstr_t ulsnSTRC(ucstr_t s1, lcstr_t s2, size_t n)
    {return ulsnSTRCM(s1, s2, n, 1);}
static inline ucstr_t uusnSTRC(ucstr_t s1, ucstr_t s2, size_t n)
    {return uusnSTRCM(s1, s2, n, 1);}

/**
 * @name llsSTRC, lusSTRC, ulsSTRC, and uusSTRC
 * @brief Case-insensitive search of s2 in s1.
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @return Pointer to first match in s1, or NULL if not found or on source
 * error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llsSTRC(lcstr_t s1, lcstr_t s2)
    {return llsnSTRC(s1, s2, MAX_LSTRLEN);}
static inline lcstr_t lusSTRC(lcstr_t s1, ucstr_t s2)
    {return lusnSTRC(s1, s2, MAX_USTRLEN);}
static inline ucstr_t ulsSTRC(ucstr_t s1, lcstr_t s2)
    {return ulsnSTRC(s1, s2, MAX_USTRLEN);}
static inline ucstr_t uusSTRC(ucstr_t s1, ucstr_t s2)
    {return uusnSTRC(s1, s2, MAX_USTRLEN);}


/**
 * @name llsstrm, lusstrm, ulsstrm, and uusstrm
 * @brief Case-sensitive search for the mth match of string s2 in string s1.
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param m Occurrence index (1 = first, 2 = second, ...). 0 returns NULL.
 * @return Pointer to mth match in s1, or NULL if not found or on
 * source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llsstrm(lcstr_t s1, lcstr_t s2, size_t m)
    {return llsnstrm(s1, s2, MAX_LSTRLEN, m);}
static inline lcstr_t lusstrm(lcstr_t s1, ucstr_t s2, size_t m)
    {return lusnstrm(s1, s2, MAX_USTRLEN, m);}
static inline ucstr_t ulsstrm(ucstr_t s1, lcstr_t s2, size_t m)
    {return ulsnstrm(s1, s2, MAX_USTRLEN, m);}
static inline ucstr_t uusstrm(ucstr_t s1, ucstr_t s2, size_t m)
    {return uusnstrm(s1, s2, MAX_USTRLEN, m);}


/**
 * @name llsSTRCM, lusSTRCM, ulsSTRCM, and uusSTRCM
 * @brief Case-insensitive search for the mth match of string s2 in string s1.
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param m Occurrence index (1 = first, 2 = second, ...). 0 returns NULL.
 * @return Pointer to mth match in s1, or NULL if not found or on
 * source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llsSTRCM(lcstr_t s1, lcstr_t s2, size_t m)
    {return llsnSTRCM(s1, s2, MAX_LSTRLEN, m);}
static inline lcstr_t lusSTRCM(lcstr_t s1, ucstr_t s2, size_t m)
    {return lusnSTRCM(s1, s2, MAX_USTRLEN, m);}
static inline ucstr_t ulsSTRCM(ucstr_t s1, lcstr_t s2, size_t m)
    {return ulsnSTRCM(s1, s2, MAX_USTRLEN, m);}
static inline ucstr_t uusSTRCM(ucstr_t s1, ucstr_t s2, size_t m)
    {return uusnSTRCM(s1, s2, MAX_USTRLEN, m);}

/**
 * @name llsnrstrm, lusnrstrm, ulsnrstrm, and uusnrstrm
 * @brief Bounded case-sensitive reverse search for the mth-from-last
 * match of string s2 in string s1.
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param n Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index from the end (1 = last, 2 = second-last, ...).
 * @return Pointer to mth-from-last match in s1, or NULL if not found or source
 * error.
 */
static inline lcstr_t llsnrstrm(
    lcstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!m)
        return (lstr_t)NULL;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    if (!s2_len)
        return m == 1 ? (s1 ? (s1 + s1_len) : (lstr_t)NULL) : (lstr_t)NULL;
    if (s2_len > s1_len)
        return (lstr_t)NULL;
    size_t count = 0;
    for (size_t ii = s1_len - s2_len + 1; ii; --ii) {
        size_t i = ii - 1;
        size_t k = 0;
        for (; k < s2_len && s1[i + k] == s2[k]; ++k);
        if (k == s2_len && ++count == m)
            return s1 + i;
    }
    return (lstr_t)NULL;
}
static inline lcstr_t lusnrstrm(
    lcstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!m)
        return (lstr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    if (!s2_len)
        return m == 1 ? (s1 ? (s1 + s1_len) : (lstr_t)NULL) : (lstr_t)NULL;
    if (s2_len > s1_len)
        return (lstr_t)NULL;
    size_t count = 0;
    for (size_t ii = s1_len - s2_len + 1; ii; --ii) {
        size_t i = ii - 1;
        size_t k = 0;
        for (; k < s2_len && (uchar_t)s1[i + k] == s2[k]; ++k);
        if (k == s2_len && ++count == m)
            return s1 + i;
    }
    return (lstr_t)NULL;
}
static inline ucstr_t ulsnrstrm(
    ucstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    if (!s2_len)
        return m == 1 ? (s1 ? (s1 + s1_len) : (ustr_t)NULL) : (ustr_t)NULL;
    if (s2_len > s1_len)
        return (ustr_t)NULL;
    size_t count = 0;
    for (size_t ii = s1_len - s2_len + 1; ii; --ii) {
        size_t i = ii - 1;
        size_t k = 0;
        for (; k < s2_len && s1[i + k] == (uchar_t)s2[k]; ++k);
        if (k == s2_len && ++count == m)
            return s1 + i;
    }
    return (ustr_t)NULL;
}
static inline ucstr_t uusnrstrm(
    ucstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    if (!s2_len)
        return m == 1 ? (s1 ? (s1 + s1_len) : (ustr_t)NULL) : (ustr_t)NULL;
    if (s2_len > s1_len)
        return (ustr_t)NULL;
    size_t count = 0;
    for (size_t ii = s1_len - s2_len + 1; ii; --ii) {
        size_t i = ii - 1;
        size_t k = 0;
        for (; k < s2_len && s1[i + k] == s2[k]; ++k);
        if (k == s2_len && ++count == m)
            return s1 + i;
    }
    return (ustr_t)NULL;
}

/**
 * @name llsnrstr, lusnrstr, ulsnrstr, and uusnrstr
 * @brief Bounded case-sensitive reverse search of string s2 in string s1
 * (last match).
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @return Pointer to last match in s1, or NULL if not found or source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llsnrstr(lcstr_t s1, lcstr_t s2, size_t n)
    {return llsnrstrm(s1, s2, n, 1);}
static inline lcstr_t lusnrstr(lcstr_t s1, ucstr_t s2, size_t n)
    {return lusnrstrm(s1, s2, n, 1);}
static inline ucstr_t ulsnrstr(ucstr_t s1, lcstr_t s2, size_t n)
    {return ulsnrstrm(s1, s2, n, 1);}
static inline ucstr_t uusnrstr(ucstr_t s1, ucstr_t s2, size_t n)
    {return uusnrstrm(s1, s2, n, 1);}

/**
 * @name llsrstr, lusrstr, ulsrstr, and uusrstr
 * @brief Case-sensitive reverse search of string s2 in string s1
 * (last match).
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @return Pointer to last match in s1, or NULL if not found or source
 * error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llsrstr(lcstr_t s1, lcstr_t s2)
    {return llsnrstr(s1, s2, MAX_LSTRLEN);}
static inline lcstr_t lusrstr(lcstr_t s1, ucstr_t s2)
    {return lusnrstr(s1, s2, MAX_USTRLEN);}
static inline ucstr_t ulsrstr(ucstr_t s1, lcstr_t s2)
    {return ulsnrstr(s1, s2, MAX_USTRLEN);}
static inline ucstr_t uusrstr(ucstr_t s1, ucstr_t s2)
    {return uusnrstr(s1, s2, MAX_USTRLEN);}

/**
 * @name llsrstrm, lusrstrm, ulsrstrm, and uusrstrm
 * @brief Case-sensitive reverse search for the mth-from-last
 * match of string s2 in string s1.
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param m Occurrence index from the end (1 = last, 2 = second-last, ...).
 * @return Pointer to mth-from-last match in s1, or NULL if not found or source
 * error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 * error.
 */
static inline lcstr_t llsrstrm(lcstr_t s1, lcstr_t s2, size_t m)
    {return llsnrstrm(s1, s2, MAX_LSTRLEN, m);}
static inline lcstr_t lusrstrm(lcstr_t s1, ucstr_t s2, size_t m)
    {return lusnrstrm(s1, s2, MAX_USTRLEN, m);}
static inline ucstr_t ulsrstrm(ucstr_t s1, lcstr_t s2, size_t m)
    {return ulsnrstrm(s1, s2, MAX_USTRLEN, m);}
static inline ucstr_t uusrstrm(ucstr_t s1, ucstr_t s2, size_t m)
    {return uusnrstrm(s1, s2, MAX_USTRLEN, m);}

/**
 * @name llsnRSTRCM, lusnRSTRCM, ulsnRSTRCM, and uusnRSTRCM
 * @brief Bounded case-insensitive reverse search for the mth-from-last
 * match of string s2 in string s1.
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param n Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index from the end (1 = last, 2 = second-last, ...).
 * @return Pointer to mth-from-last match in s1, or NULL if not found or source
 * error.
 */
static inline lcstr_t llsnRSTRCM(
    lcstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!m)
        return (lstr_t)NULL;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    if (!s2_len)
        return m == 1 ? (s1 ? (s1 + s1_len) : (lstr_t)NULL) : (lstr_t)NULL;
    if (s2_len > s1_len)
        return (lstr_t)NULL;
    size_t count = 0;
    for (size_t ii = s1_len - s2_len + 1; ii; --ii) {
        size_t i = ii - 1;
        size_t k = 0;
        for (; k < s2_len &&
               tolupper(s1[i + k]) == tolupper(s2[k]); ++k);
        if (k == s2_len && ++count == m)
            return s1 + i;
    }
    return (lstr_t)NULL;
}
static inline lcstr_t lusnRSTRCM(
    lcstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!m)
        return (lstr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;
    if (!s2_len)
        return m == 1 ? (s1 ? (s1 + s1_len) : (lstr_t)NULL) : (lstr_t)NULL;
    if (s2_len > s1_len)
        return (lstr_t)NULL;
    size_t count = 0;
    for (size_t ii = s1_len - s2_len + 1; ii; --ii) {
        size_t i = ii - 1;
        size_t k = 0;
        for (; k < s2_len &&
               touupper((uchar_t)s1[i + k]) == touupper(s2[k]); ++k);
        if (k == s2_len && ++count == m)
            return s1 + i;
    }
    return (lstr_t)NULL;
}
static inline ucstr_t ulsnRSTRCM(
    ucstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    if (!s2_len)
        return m == 1 ? (s1 ? (s1 + s1_len) : (ustr_t)NULL) : (ustr_t)NULL;
    if (s2_len > s1_len)
        return (ustr_t)NULL;
    size_t count = 0;
    for (size_t ii = s1_len - s2_len + 1; ii; --ii) {
        size_t i = ii - 1;
        size_t k = 0;
        for (; k < s2_len &&
               touupper(s1[i + k]) == touupper((uchar_t)s2[k]); ++k);
        if (k == s2_len && ++count == m)
            return s1 + i;
    }
    return (ustr_t)NULL;
}
static inline ucstr_t uusnRSTRCM(
    ucstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!m)
        return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;
    if (!s2_len)
        return m == 1 ? (s1 ? (s1 + s1_len) : (ustr_t)NULL) : (ustr_t)NULL;
    if (s2_len > s1_len)
        return (ustr_t)NULL;
    size_t count = 0;
    for (size_t ii = s1_len - s2_len + 1; ii; --ii) {
        size_t i = ii - 1;
        size_t k = 0;
        for (; k < s2_len && touupper(s1[i + k]) == touupper(s2[k]); ++k);
        if (k == s2_len && ++count == m)
            return s1 + i;
    }
    return (ustr_t)NULL;
}

/**
 * @name llsnRSTRC, lusnRSTRC, ulsnRSTRC, and uusnRSTRC
 * @brief Bounded case-insensitive reverse search of string s2
 * in string s1 (last match).
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param n Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Pointer to last match in s1, or NULL if not
 * found or source error.
 */
static inline lcstr_t llsnRSTRC(lcstr_t s1, lcstr_t s2, size_t n)
    {return llsnRSTRCM(s1, s2, n, 1);}
static inline lcstr_t lusnRSTRC(lcstr_t s1, ucstr_t s2, size_t n)
    {return lusnRSTRCM(s1, s2, n, 1);}
static inline ucstr_t ulsnRSTRC(ucstr_t s1, lcstr_t s2, size_t n)
    {return ulsnRSTRCM(s1, s2, n, 1);}
static inline ucstr_t uusnRSTRC(ucstr_t s1, ucstr_t s2, size_t n)
    {return uusnRSTRCM(s1, s2, n, 1);}

/**
 * @name llsRSTRC, lusRSTRC, ulsRSTRC, and uusRSTRC
 * @brief Case-insensitive reverse search of string s2 in
 * string s1 (last match).
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @return Pointer to last match, or NULL if not
 * found or source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llsRSTRC(lcstr_t s1, lcstr_t s2)
    {return llsnRSTRC(s1, s2, MAX_LSTRLEN);}
static inline lcstr_t lusRSTRC(lcstr_t s1, ucstr_t s2)
    {return lusnRSTRC(s1, s2, MAX_USTRLEN);}
static inline ucstr_t ulsRSTRC(ucstr_t s1, lcstr_t s2)
    {return ulsnRSTRC(s1, s2, MAX_USTRLEN);}
static inline ucstr_t uusRSTRC(ucstr_t s1, ucstr_t s2)
    {return uusnRSTRC(s1, s2, MAX_USTRLEN);}

/**
 * @name llsRSTRCM, lusRSTRCM, ulsRSTRCM, and uusRSTRCM
 * @brief Case-insensitive reverse search for the mth-from-last occurrence of
 * string s2 in string s1.
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param m Occurrence index from the end (1 = last, 2 = second-last, ...).
 * @return Pointer to mth-from-last match in s1, or NULL if not found or source
 * error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline lcstr_t llsRSTRCM(lcstr_t s1, lcstr_t s2, size_t m)
    {return llsnRSTRCM(s1, s2, MAX_LSTRLEN, m);}
static inline lcstr_t lusRSTRCM(lcstr_t s1, ucstr_t s2, size_t m)
    {return lusnRSTRCM(s1, s2, MAX_USTRLEN, m);}
static inline ucstr_t ulsRSTRCM(ucstr_t s1, lcstr_t s2, size_t m)
    {return ulsnRSTRCM(s1, s2, MAX_USTRLEN, m);}
static inline ucstr_t uusRSTRCM(ucstr_t s1, ucstr_t s2, size_t m)
    {return uusnRSTRCM(s1, s2, MAX_USTRLEN, m);}

/**
 * @name llsnsubcnt, lusnsubcnt, ulsnsubcnt, and uusnsubcnt
 * @brief Bounded count of case-sensitive matches of string s2 in string s1
 * (overlapping matches are counted).
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param n Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Count of matches, or MISSING_STR_TERMINATOR on source error.
 * @note If s2 is empty, returns 0.
 */
static inline size_t llsnsubcnt(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s2_len || s2_len > s1_len)
        return (size_t)0;
    size_t c = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && s1[i + k] == s2[k]; ++k);
        if (k == s2_len) ++c;
    }
    return c;
}
static inline size_t lusnsubcnt(lcstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s2_len || s2_len > s1_len)
        return (size_t)0;
    size_t c = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && (uchar_t)s1[i + k] == s2[k]; ++k);
        if (k == s2_len) ++c;
    }
    return c;
}
static inline size_t ulsnsubcnt(ucstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s2_len || s2_len > s1_len)
        return (size_t)0;
    size_t c = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && s1[i + k] == (uchar_t)s2[k]; ++k);
        if (k == s2_len) ++c;
    }
    return c;
}
static inline size_t uusnsubcnt(ucstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s2_len || s2_len > s1_len)
        return (size_t)0;
    size_t c = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && s1[i + k] == s2[k]; ++k);
        if (k == s2_len) ++c;
    }
    return c;
}

/**
 * @name llssubcnt, lussubcnt, ulssubcnt, and uussubcnt
 * @brief Count of case-sensitive matches of string s2 in string s1
 * (overlapping matches are counted).
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @return Count of matches, or MISSING_STR_TERMINATOR on source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline size_t llssubcnt(lcstr_t s1, lcstr_t s2)
    {return llsnsubcnt(s1, s2, MAX_LSTRLEN);}
static inline size_t lussubcnt(lcstr_t s1, ucstr_t s2)
    {return lusnsubcnt(s1, s2, MAX_USTRLEN);}
static inline size_t ulssubcnt(ucstr_t s1, lcstr_t s2)
    {return ulsnsubcnt(s1, s2, MAX_USTRLEN);}
static inline size_t uussubcnt(ucstr_t s1, ucstr_t s2)
    {return uusnsubcnt(s1, s2, MAX_USTRLEN);}

/**
 * @name llsnSUBCNTC, lusnSUBCNTC, ulsnSUBCNTC, and uusnSUBCNTC
 * @brief Bounded case-insensitive count of matches of string s2 in string s1
 * (overlapping matches are counted).
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param n Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Count of matches, or MISSING_STR_TERMINATOR on source error.
 * @note If s2 is empty, returns 0.
 */
static inline size_t llsnSUBCNTC(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s2_len || s2_len > s1_len)
        return (size_t)0;
    size_t c = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len &&
               tolupper(s1[i + k]) == tolupper(s2[k]); ++k);
        if (k == s2_len) ++c;
    }
    return c;
}
static inline size_t lusnSUBCNTC(lcstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s2_len || s2_len > s1_len)
        return (size_t)0;
    size_t c = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len &&
               touupper((uchar_t)s1[i + k]) == touupper(s2[k]); ++k);
        if (k == s2_len) ++c;
    }
    return c;
}
static inline size_t ulsnSUBCNTC(ucstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = lcsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s2_len || s2_len > s1_len)
        return (size_t)0;
    size_t c = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len &&
               touupper(s1[i + k]) == touupper((uchar_t)s2[k]); ++k);
        if (k == s2_len) ++c;
    }
    return c;
}
static inline size_t uusnSUBCNTC(ucstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    size_t s2_len = ucsnlen(s2, n);
    if (s1_len == MISSING_STR_TERMINATOR || s2_len == MISSING_STR_TERMINATOR)
        return MISSING_STR_TERMINATOR;
    if (!s2_len || s2_len > s1_len)
        return (size_t)0;
    size_t c = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && touupper(s1[i + k]) == touupper(s2[k]); ++k);
        if (k == s2_len) ++c;
    }
    return c;
}

/**
 * @name llsSUBCNTC, lusSUBCNTC, ulsSUBCNTC, and uusSUBCNTC
 * @brief Case-insensitive count of matches of string s2 in string s1
 * (overlapping matches are counted).
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @return Count of matches, or MISSING_STR_TERMINATOR on source error.
 * @note Default source bound applied according to source type
 *       (MAX_LSTRLEN or MAX_USTRLEN).
 */
static inline size_t llsSUBCNTC(lcstr_t s1, lcstr_t s2)
    {return llsnSUBCNTC(s1, s2, MAX_LSTRLEN);}
static inline size_t lusSUBCNTC(lcstr_t s1, ucstr_t s2)
    {return lusnSUBCNTC(s1, s2, MAX_USTRLEN);}
static inline size_t ulsSUBCNTC(ucstr_t s1, lcstr_t s2)
    {return ulsnSUBCNTC(s1, s2, MAX_USTRLEN);}
static inline size_t uusSUBCNTC(ucstr_t s1, ucstr_t s2)
    {return uusnSUBCNTC(s1, s2, MAX_USTRLEN);}

#define LUB_TRIM_LEFT_MODE(trim) \
    ((trim) == 'L' || (trim) == 'B' || (trim) == 'l' || (trim) == 'b')
#define LUB_TRIM_RIGHT_MODE(trim) \
    ((trim) == 'R' || (trim) == 'B' || (trim) == 'r' || (trim) == 'b')

/**
 * @name llsntrim, lusntrim, ulsntrim, uusntrim
 * @brief Trim whitespace from source string to target (bounded).
 * @param t Target buffer.
 * @param s Source string.
 * @param n Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param trim Trim selector: 'L', 'R', or 'B' (lowercase also accepted).
 * @param delim If non-null, collapse each embedded whitespace sequence
 *        to this delimiter; if null, no collapsing is performed.
 * @return t, or NULL on error.
 * @note Errors include: NULL target t or unterminated source s within bound n.
 * @note 'L' removes leading whitespace, 'R' removes trailing whitespace,
 *       and 'B' removes both before optional collapsing.
 */
static inline lstr_t llsntrim(
    lstr_t t, lcstr_t s, size_t n, char trim, lchar_t delim) {
    if (!t) return (lstr_t)NULL;
    if (!s) {*t = (lchar_t)0; return t;}
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    
    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR) return (lstr_t)NULL;

    size_t start = 0, end = len;
    if (LUB_TRIM_LEFT_MODE(trim))
        while (start < end && islspace(s[start])) start++;
    if (LUB_TRIM_RIGHT_MODE(trim))
        while (end > start && islspace(s[end - 1])) end--;

    if (delim) {
        size_t ti = 0;
        int pending_space = 0;
        for (size_t i = start; i < end; ++i) {
            if (islspace(s[i])) {
                pending_space = 1;
            } else {
                if (pending_space) t[ti++] = delim;
                t[ti++] = s[i];
                pending_space = 0;
            }
        }
        if (pending_space) t[ti++] = delim;
        t[ti] = (lchar_t)0;
    } else {
        for (size_t i = start; i < end; ++i) t[i - start] = s[i];
        t[end - start] = (lchar_t)0;
    }
    return t;
}
static inline lstr_t lusntrim(
    lstr_t t, ucstr_t s, size_t n, char trim, lchar_t delim) {
    if (!t) return (lstr_t)NULL;
    if (!s) {*t = (lchar_t)0; return t;}
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    
    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR) return (lstr_t)NULL;

    size_t start = 0, end = len;
    if (LUB_TRIM_LEFT_MODE(trim))
        while (start < end && isuspace(s[start])) start++;
    if (LUB_TRIM_RIGHT_MODE(trim))
        while (end > start && isuspace(s[end - 1])) end--;

    if (delim) {
        size_t ti = 0;
        int pending_space = 0;
        for (size_t i = start; i < end; ++i) {
            if (isuspace(s[i])) {
                pending_space = 1;
            } else {
                if (pending_space) t[ti++] = delim;
                t[ti++] = (size_t)s[i] <= MAX_LCHAR ? (lchar_t)s[i] : '?';
                pending_space = 0;
            }
        }
        if (pending_space) t[ti++] = delim;
        t[ti] = (lchar_t)0;
    } else {
        for (size_t i = start; i < end; ++i)
            t[i - start] = (size_t)s[i] <= MAX_LCHAR ? (lchar_t)s[i] : '?';
        t[end - start] = (lchar_t)0;
    }
    return t;
}
static inline ustr_t ulsntrim(
    ustr_t t, lcstr_t s, size_t n, char trim, uchar_t delim) {
    if (!t) return (ustr_t)NULL;
    if (!s) {*t = (uchar_t)0; return t;}
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    
    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR) return (ustr_t)NULL;

    size_t start = 0, end = len;
    if (LUB_TRIM_LEFT_MODE(trim))
        while (start < end && islspace(s[start])) start++;
    if (LUB_TRIM_RIGHT_MODE(trim))
        while (end > start && islspace(s[end - 1])) end--;

    if (delim) {
        size_t ti = 0;
        int pending_space = 0;
        for (size_t i = start; i < end; ++i) {
            if (islspace(s[i])) {
                pending_space = 1;
            } else {
                if (pending_space) t[ti++] = delim;
                t[ti++] = (uchar_t)s[i];
                pending_space = 0;
            }
        }
        if (pending_space) t[ti++] = delim;
        t[ti] = (uchar_t)0;
    } else {
        for (size_t i = start; i < end; ++i) t[i - start] = (uchar_t)s[i];
        t[end - start] = (uchar_t)0;
    }
    return t;
}
static inline ustr_t uusntrim(
    ustr_t t, ucstr_t s, size_t n, char trim, uchar_t delim) {
    if (!t) return (ustr_t)NULL;
    if (!s) {*t = (uchar_t)0; return t;}
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    
    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR) return (ustr_t)NULL;

    size_t start = 0, end = len;
    if (LUB_TRIM_LEFT_MODE(trim))
        while (start < end && isuspace(s[start])) start++;
    if (LUB_TRIM_RIGHT_MODE(trim))
        while (end > start && isuspace(s[end - 1])) end--;

    if (delim) {
        size_t ti = 0;
        int pending_space = 0;
        for (size_t i = start; i < end; ++i) {
            if (isuspace(s[i])) {
                pending_space = 1;
            } else {
                if (pending_space) t[ti++] = delim;
                t[ti++] = s[i];
                pending_space = 0;
            }
        }
        if (pending_space) t[ti++] = delim;
        t[ti] = (uchar_t)0;
    } else {
        for (size_t i = start; i < end; ++i) t[i - start] = s[i];
        t[end - start] = (uchar_t)0;
    }
    return t;
}

/**
 * @name llstrim, lustrim, ulstrim, uustrim
 * @brief Trim whitespace (unbounded).
 * @param t Target buffer.
 * @param s Source string (bounded by MAX_LSTRLEN or MAX_USTRLEN).
 * @param trim Trim selector: 'L', 'R', or 'B' (lowercase also accepted).
 * @param delim If non-null, collapse each embedded whitespace sequence in the
 *        trimmed span to this delimiter; if null, no collapsing is performed.
 * @return t, or NULL on error.
 * @note Errors include: NULL target t or unterminated source s within its
 *       default bound.
 */
static inline lstr_t llstrim(lstr_t t, lcstr_t s, char trim, lchar_t delim)
    {return llsntrim(t, s, MAX_LSTRLEN, trim, delim);}
static inline lstr_t lustrim(lstr_t t, ucstr_t s, char trim, lchar_t delim)
    {return lusntrim(t, s, MAX_USTRLEN, trim, delim);}
static inline ustr_t ulstrim(ustr_t t, lcstr_t s, char trim, uchar_t delim)
    {return ulsntrim(t, s, MAX_USTRLEN, trim, delim);}
static inline ustr_t uustrim(ustr_t t, ucstr_t s, char trim, uchar_t delim)
    {return uusntrim(t, s, MAX_USTRLEN, trim, delim);}

#undef LUB_TRIM_LEFT_MODE
#undef LUB_TRIM_RIGHT_MODE

/**
 * @name llsnrev, lusnrev, ulsnrev, uusnrev
 * @brief Reverse source string into target (bounded).
 * @param t Target buffer for result.
 * @param s Source string.
 * @param n Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return t, or NULL on error.
 * @note Errors include: invalid arguments, unterminated source within n,
 *       and out-of-range Unicode to Latin conversion (replaced with '?').
 */
static inline lstr_t llsnrev(lstr_t t, lcstr_t s, size_t n) {
    if (!t || !s) return (lstr_t)NULL;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;

    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR) return (lstr_t)NULL;

    for (size_t i = 0; i < len; ++i) t[i] = s[len - 1 - i];
    t[len] = (lchar_t)0;
    return t;
}
static inline lstr_t lusnrev(lstr_t t, ucstr_t s, size_t n) {
    if (!t || !s) return (lstr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR) return (lstr_t)NULL;

    for (size_t i = 0; i < len; ++i) {
        uchar_t c = s[len - 1 - i];
        t[i] = (size_t)c <= MAX_LCHAR ? (lchar_t)c : '?';
    }
    t[len] = (lchar_t)0;
    return t;
}
static inline ustr_t ulsnrev(ustr_t t, lcstr_t s, size_t n) {
    if (!t || !s) return (ustr_t)NULL;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;

    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR) return (ustr_t)NULL;

    for (size_t i = 0; i < len; ++i) t[i] = (uchar_t)s[len - 1 - i];
    t[len] = (uchar_t)0;
    return t;
}
static inline ustr_t uusnrev(ustr_t t, ucstr_t s, size_t n) {
    if (!t || !s) return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR) return (ustr_t)NULL;

    for (size_t i = 0; i < len; ++i) t[i] = s[len - 1 - i];
    t[len] = (uchar_t)0;
    return t;
}

/**
 * @name llsrev, lusrev, ulsrev, uusrev
 * @brief Reverse source string into target (unbounded source).
 * @param t Target buffer for result.
 * @param s Source string.
 * @return t, or NULL on error.
 * @note Errors include: invalid arguments, unterminated source within its
 *       default bound, and out-of-range Unicode to Latin conversion
 *       (replaced with '?').
 */
static inline lstr_t llsrev(lstr_t t, lcstr_t s)
    {return llsnrev(t, s, MAX_LSTRLEN);}
static inline lstr_t lusrev(lstr_t t, ucstr_t s)
    {return lusnrev(t, s, MAX_USTRLEN);}
static inline ustr_t ulsrev(ustr_t t, lcstr_t s)
    {return ulsnrev(t, s, MAX_LSTRLEN);}
static inline ustr_t uusrev(ustr_t t, ucstr_t s)
    {return uusnrev(t, s, MAX_USTRLEN);}

/**
 * @name llsnpad, lusnpad, ulsnpad, uusnpad
 * @brief Pad source string to exactly tn characters using left, center, or
 *        right alignment (bounded).
 * @param t Target buffer for result.
 * @param tn Number of result characters written to t (excluding terminator).
 * @param s Source string.
 * @param n Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param pad Pad character written into target.
 * @param m Alignment selector: 0 centers, >0 pads on left (right-aligned
 *        text), <0 pads on right (left-aligned text).
 * @return t, or NULL on error.
 * @note Errors include: invalid arguments, unterminated source within n,
 *       and source longer than tn.
 */
static inline lstr_t llsnpad(
    lstr_t t, size_t tn, lcstr_t s, size_t n, lchar_t pad, ptrdiff_t m) {
    if (!t || !s) return (lstr_t)NULL;
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;

    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR) return (lstr_t)NULL;

    if (len > tn) {
        t[tn] = (lchar_t)0;
        return (lstr_t)NULL;
    }

    size_t left = 0, right = 0;
    if (len < tn) {
        size_t total = tn - len;
        if (m > 0) left = total;
        else if (m < 0) right = total;
        else {
            left = total / 2;
            right = total - left;
        }
    }

    size_t ti = 0;
    for (size_t i = 0; i < left; ++i) t[ti++] = pad;
    for (size_t i = 0; i < len; ++i) t[ti++] = s[i];
    for (size_t i = 0; i < right; ++i) t[ti++] = pad;
    t[ti] = (lchar_t)0;
    return t;
}
static inline lstr_t lusnpad(
    lstr_t t, size_t tn, ucstr_t s, size_t n, lchar_t pad, ptrdiff_t m) {
    if (!t || !s) return (lstr_t)NULL;
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR) return (lstr_t)NULL;

    if (len > tn) {
        t[tn] = (lchar_t)0;
        return (lstr_t)NULL;
    }

    size_t left = 0, right = 0;
    if (len < tn) {
        size_t total = tn - len;
        if (m > 0) left = total;
        else if (m < 0) right = total;
        else {
            left = total / 2;
            right = total - left;
        }
    }

    size_t ti = 0;
    for (size_t i = 0; i < left; ++i) t[ti++] = pad;
    for (size_t i = 0; i < len; ++i)
        t[ti++] = (size_t)s[i] <= MAX_LCHAR ? (lchar_t)s[i] : '?';
    for (size_t i = 0; i < right; ++i) t[ti++] = pad;
    t[ti] = (lchar_t)0;
    return t;
}
static inline ustr_t ulsnpad(
    ustr_t t, size_t tn, lcstr_t s, size_t n, uchar_t pad, ptrdiff_t m) {
    if (!t || !s) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;

    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR) return (ustr_t)NULL;

    if (len > tn) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    size_t left = 0, right = 0;
    if (len < tn) {
        size_t total = tn - len;
        if (m > 0) left = total;
        else if (m < 0) right = total;
        else {
            left = total / 2;
            right = total - left;
        }
    }

    size_t ti = 0;
    for (size_t i = 0; i < left; ++i) t[ti++] = pad;
    for (size_t i = 0; i < len; ++i) t[ti++] = (uchar_t)s[i];
    for (size_t i = 0; i < right; ++i) t[ti++] = pad;
    t[ti] = (uchar_t)0;
    return t;
}
static inline ustr_t uusnpad(
    ustr_t t, size_t tn, ucstr_t s, size_t n, uchar_t pad, ptrdiff_t m) {
    if (!t || !s) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR) return (ustr_t)NULL;

    if (len > tn) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    size_t left = 0, right = 0;
    if (len < tn) {
        size_t total = tn - len;
        if (m > 0) left = total;
        else if (m < 0) right = total;
        else {
            left = total / 2;
            right = total - left;
        }
    }

    size_t ti = 0;
    for (size_t i = 0; i < left; ++i) t[ti++] = pad;
    for (size_t i = 0; i < len; ++i) t[ti++] = s[i];
    for (size_t i = 0; i < right; ++i) t[ti++] = pad;
    t[ti] = (uchar_t)0;
    return t;
}

/**
 * @name llspad, luspad, ulspad, uuspad
 * @brief Pad source string to exactly tn characters using left, center, or
 *        right alignment (unbounded source).
 * @param t Target buffer for result.
 * @param tn Number of result characters written to t (excluding terminator).
 * @param s Source string.
 * @param pad Pad character written into target.
 * @param m Alignment selector: 0 centers, >0 pads on left (right-aligned
 *        text), <0 pads on right (left-aligned text).
 * @return t, or NULL on error.
 * @note Errors include: invalid arguments, unterminated source within its
 *       default bound, and source longer than tn.
 */
static inline lstr_t llspad(
    lstr_t t, size_t tn, lcstr_t s, lchar_t pad, ptrdiff_t m)
    {return llsnpad(t, tn, s, MAX_LSTRLEN, pad, m);}
static inline lstr_t luspad(
    lstr_t t, size_t tn, ucstr_t s, lchar_t pad, ptrdiff_t m)
    {return lusnpad(t, tn, s, MAX_USTRLEN, pad, m);}
static inline ustr_t ulspad(
    ustr_t t, size_t tn, lcstr_t s, uchar_t pad, ptrdiff_t m)
    {return ulsnpad(t, tn, s, MAX_LSTRLEN, pad, m);}
static inline ustr_t uuspad(
    ustr_t t, size_t tn, ucstr_t s, uchar_t pad, ptrdiff_t m)
    {return uusnpad(t, tn, s, MAX_USTRLEN, pad, m);}

/**
 * @name llsnreplace, lusnreplace, ulsnreplace, uusnreplace
 * @brief Case-sensitive replace using a delimiter-encoded map.
 * @param t Target buffer for result.
 * @param tn Maximum number of result characters written to t
 *        (excluding terminator).
 * @param s Source string.
 * @param map Mapping string encoded as
 *        needle1<delim>repl1<delim>needle2<delim>repl2...
 * @param delim Mapping delimiter. Must not be the null character.
 * @param n Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Replacement selector:
 *        0 replaces all matches;
 *        >0 replaces the mth match from start (1-based);
 *        <0 replaces the mth match from end (-1 is last).
 * @return t, or NULL on error.
 * @note Errors include: invalid arguments, unterminated s or map within their
 *       bounds, malformed map syntax (missing delimiter or empty needle),
 *       multi-pair maps used with m != 0, and output longer than tn.
 * @note If result would exceed tn characters, returns NULL and writes
 *       terminator at t[tn].
 * @note If map has more than one pair, m must be 0.
 * @note For multi-pair maps, the first matching needle in map order wins.
 *       List longer needles before shorter ones if overlap matters.
 */
static inline lstr_t llsnreplace(
    lstr_t t, size_t tn, lcstr_t s, lcstr_t map, lchar_t delim, size_t n,
    ptrdiff_t m) {
    if (!t || !s || !map || !delim) return (lstr_t)NULL;
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, n);
    size_t map_len = lcsnlen(map, MAX_LSTRLEN);
    if (s_len == MISSING_STR_TERMINATOR || map_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;

    lcstr_t p = map;
    lcstr_t map_end = map + map_len;

    lcstr_t first_needle = p;
    while (p < map_end && *p != delim) p++;
    if (p >= map_end) return (lstr_t)NULL;
    size_t first_needle_len = (size_t)(p - first_needle);
    if (!first_needle_len) return (lstr_t)NULL;
    p++;

    lcstr_t first_repl = p;
    while (p < map_end && *p != delim) p++;
    size_t first_repl_len = (size_t)(p - first_repl);

    int multi_pair = (p < map_end);
    if (multi_pair && m) return (lstr_t)NULL;

    if (!multi_pair) {
        size_t needle_len = first_needle_len, repl_len = first_repl_len;
        size_t matches = 0;
        if (m < 0) {
            for (size_t si = 0; si + needle_len <= s_len;) {
                int match = 1;
                for (size_t j = 0; j < needle_len; ++j)
                    if (s[si + j] != first_needle[j]) {match = 0; break;}
                if (match) {matches++; si += needle_len;} else si++;
            }
        }

        size_t target = 0;
        if (m > 0) target = (size_t)m;
        else if (m < 0) {
            ptrdiff_t idx = (ptrdiff_t)matches + m + 1;
            if (idx > 0) target = (size_t)idx;
        }
        if (m < 0 && (!target || target > matches)) {
            if (s_len > tn) {t[tn] = (lchar_t)0; return (lstr_t)NULL;}
            for (size_t i = 0; i < s_len; ++i) t[i] = s[i];
            t[s_len] = (lchar_t)0;
            return t;
        }

        size_t si = 0, ti = 0, occ = 0;
        while (si < s_len) {
            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (s[si + j] != first_needle[j]) match = 0;

            if (match) {
                occ++;
                if (!m || occ == target) {
                    if (ti + repl_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lstr_t)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j) t[ti + j] = first_repl[j];
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lstr_t)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j) t[ti + j] = s[si + j];
                    ti += needle_len;
                }
                si += needle_len;
            } else {
                if (ti >= tn) {
                    t[tn] = (lchar_t)0;
                    return (lstr_t)NULL;
                }
                t[ti++] = s[si++];
            }
        }
        t[ti] = (lchar_t)0;
        return t;
    }

    size_t si = 0, ti = 0;
    while (si < s_len) {
        int matched = 0;
        p = map;
        while (p < map_end) {
            lcstr_t needle = p;
            while (p < map_end && *p != delim) p++;
            size_t needle_len = (size_t)(p - needle);
            if (!needle_len) return (lstr_t)NULL;
            p++;

            lcstr_t repl = p;
            while (p < map_end && *p != delim) p++;
            size_t repl_len = (size_t)(p - repl);

            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (s[si + j] != needle[j]) match = 0;

            if (match) {
                if (ti + repl_len > tn) {
                    t[tn] = (lchar_t)0;
                    return (lstr_t)NULL;
                }
                for (size_t j = 0; j < repl_len; ++j) t[ti + j] = repl[j];
                ti += repl_len;
                si += needle_len;
                matched = 1;
                break;
            }

            if (p < map_end) p++;
        }

        if (!matched) {
            if (ti >= tn) {
                t[tn] = (lchar_t)0;
                return (lstr_t)NULL;
            }
            t[ti++] = s[si++];
        }
    }

    t[ti] = (lchar_t)0;
    return t;
}
static inline lstr_t lusnreplace(
    lstr_t t, size_t tn, lcstr_t s, ucstr_t map, uchar_t delim, size_t n,
    ptrdiff_t m) {
    if (!t || !s || !map || !delim) return (lstr_t)NULL;
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, n);
    size_t map_len = ucsnlen(map, MAX_USTRLEN);
    if (s_len == MISSING_STR_TERMINATOR || map_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;

    ucstr_t p = map;
    ucstr_t map_end = map + map_len;

    ucstr_t first_needle = p;
    while (p < map_end && *p != delim) p++;
    if (p >= map_end) return (lstr_t)NULL;
    size_t first_needle_len = (size_t)(p - first_needle);
    if (!first_needle_len) return (lstr_t)NULL;
    p++;

    ucstr_t first_repl = p;
    while (p < map_end && *p != delim) p++;
    size_t first_repl_len = (size_t)(p - first_repl);

    int multi_pair = (p < map_end);
    if (multi_pair && m) return (lstr_t)NULL;

    if (!multi_pair) {
        size_t needle_len = first_needle_len, repl_len = first_repl_len;
        size_t matches = 0;
        if (m < 0) {
            for (size_t si = 0; si + needle_len <= s_len;) {
                int match = 1;
                for (size_t j = 0; j < needle_len; ++j) {
                    if ((size_t)first_needle[j] > MAX_LCHAR ||
                        s[si + j] != (lchar_t)first_needle[j]) {match = 0; break;}
                }
                if (match) {matches++; si += needle_len;} else si++;
            }
        }

        size_t target = 0;
        if (m > 0) target = (size_t)m;
        else if (m < 0) {
            ptrdiff_t idx = (ptrdiff_t)matches + m + 1;
            if (idx > 0) target = (size_t)idx;
        }
        if (m < 0 && (!target || target > matches)) {
            if (s_len > tn) {t[tn] = (lchar_t)0; return (lstr_t)NULL;}
            for (size_t i = 0; i < s_len; ++i) t[i] = s[i];
            t[s_len] = (lchar_t)0;
            return t;
        }

        size_t si = 0, ti = 0, occ = 0;
        while (si < s_len) {
            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if ((size_t)first_needle[j] > MAX_LCHAR ||
                    s[si + j] != (lchar_t)first_needle[j]) match = 0;

            if (match) {
                occ++;
                if (!m || occ == target) {
                    if (ti + repl_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lstr_t)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j)
                        t[ti + j] = (size_t)first_repl[j] <= MAX_LCHAR ? (lchar_t)first_repl[j] : '?';
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lstr_t)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j) t[ti + j] = s[si + j];
                    ti += needle_len;
                }
                si += needle_len;
            } else {
                if (ti >= tn) {
                    t[tn] = (lchar_t)0;
                    return (lstr_t)NULL;
                }
                t[ti++] = s[si++];
            }
        }
        t[ti] = (lchar_t)0;
        return t;
    }

    size_t si = 0, ti = 0;
    while (si < s_len) {
        int matched = 0;
        p = map;
        while (p < map_end) {
            ucstr_t needle = p;
            while (p < map_end && *p != delim) p++;
            size_t needle_len = (size_t)(p - needle);
            if (!needle_len) return (lstr_t)NULL;
            p++;

            ucstr_t repl = p;
            while (p < map_end && *p != delim) p++;
            size_t repl_len = (size_t)(p - repl);

            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if ((size_t)needle[j] > MAX_LCHAR ||
                    s[si + j] != (lchar_t)needle[j]) match = 0;

            if (match) {
                if (ti + repl_len > tn) {
                    t[tn] = (lchar_t)0;
                    return (lstr_t)NULL;
                }
                for (size_t j = 0; j < repl_len; ++j)
                    t[ti + j] = (size_t)repl[j] <= MAX_LCHAR ? (lchar_t)repl[j] : '?';
                ti += repl_len;
                si += needle_len;
                matched = 1;
                break;
            }

            if (p < map_end) p++;
        }

        if (!matched) {
            if (ti >= tn) {
                t[tn] = (lchar_t)0;
                return (lstr_t)NULL;
            }
            t[ti++] = s[si++];
        }
    }

    t[ti] = (lchar_t)0;
    return t;
}
static inline ustr_t ulsnreplace(
    ustr_t t, size_t tn, ucstr_t s, lcstr_t map, lchar_t delim, size_t n,
    ptrdiff_t m) {
    if (!t || !s || !map || !delim) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t s_len = ucsnlen(s, n);
    size_t map_len = lcsnlen(map, MAX_LSTRLEN);
    if (s_len == MISSING_STR_TERMINATOR || map_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;

    lcstr_t p = map;
    lcstr_t map_end = map + map_len;

    lcstr_t first_needle = p;
    while (p < map_end && *p != delim) p++;
    if (p >= map_end) return (ustr_t)NULL;
    size_t first_needle_len = (size_t)(p - first_needle);
    if (!first_needle_len) return (ustr_t)NULL;
    p++;

    lcstr_t first_repl = p;
    while (p < map_end && *p != delim) p++;
    size_t first_repl_len = (size_t)(p - first_repl);

    int multi_pair = (p < map_end);
    if (multi_pair && m) return (ustr_t)NULL;

    if (!multi_pair) {
        size_t needle_len = first_needle_len, repl_len = first_repl_len;
        size_t matches = 0;
        if (m < 0) {
            for (size_t si = 0; si + needle_len <= s_len;) {
                int match = 1;
                for (size_t j = 0; j < needle_len; ++j)
                    if (s[si + j] != (uchar_t)first_needle[j]) {match = 0; break;}
                if (match) {matches++; si += needle_len;} else si++;
            }
        }

        size_t target = 0;
        if (m > 0) target = (size_t)m;
        else if (m < 0) {
            ptrdiff_t idx = (ptrdiff_t)matches + m + 1;
            if (idx > 0) target = (size_t)idx;
        }
        if (m < 0 && (!target || target > matches)) {
            if (s_len > tn) {t[tn] = (uchar_t)0; return (ustr_t)NULL;}
            for (size_t i = 0; i < s_len; ++i) t[i] = s[i];
            t[s_len] = (uchar_t)0;
            return t;
        }

        size_t si = 0, ti = 0, occ = 0;
        while (si < s_len) {
            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (s[si + j] != (uchar_t)first_needle[j]) match = 0;

            if (match) {
                occ++;
                if (!m || occ == target) {
                    if (ti + repl_len > tn) {
                        t[tn] = (uchar_t)0;
                        return (ustr_t)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j) t[ti + j] = (uchar_t)first_repl[j];
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (uchar_t)0;
                        return (ustr_t)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j) t[ti + j] = s[si + j];
                    ti += needle_len;
                }
                si += needle_len;
            } else {
                if (ti >= tn) {
                    t[tn] = (uchar_t)0;
                    return (ustr_t)NULL;
                }
                t[ti++] = s[si++];
            }
        }
        t[ti] = (uchar_t)0;
        return t;
    }

    size_t si = 0, ti = 0;
    while (si < s_len) {
        int matched = 0;
        p = map;
        while (p < map_end) {
            lcstr_t needle = p;
            while (p < map_end && *p != delim) p++;
            size_t needle_len = (size_t)(p - needle);
            if (!needle_len) return (ustr_t)NULL;
            p++;

            lcstr_t repl = p;
            while (p < map_end && *p != delim) p++;
            size_t repl_len = (size_t)(p - repl);

            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (s[si + j] != (uchar_t)needle[j]) match = 0;

            if (match) {
                if (ti + repl_len > tn) {
                    t[tn] = (uchar_t)0;
                    return (ustr_t)NULL;
                }
                for (size_t j = 0; j < repl_len; ++j) t[ti + j] = (uchar_t)repl[j];
                ti += repl_len;
                si += needle_len;
                matched = 1;
                break;
            }

            if (p < map_end) p++;
        }

        if (!matched) {
            if (ti >= tn) {
                t[tn] = (uchar_t)0;
                return (ustr_t)NULL;
            }
            t[ti++] = s[si++];
        }
    }

    t[ti] = (uchar_t)0;
    return t;
}
static inline ustr_t uusnreplace(
    ustr_t t, size_t tn, ucstr_t s, ucstr_t map, uchar_t delim, size_t n,
    ptrdiff_t m) {
    if (!t || !s || !map || !delim) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t s_len = ucsnlen(s, n);
    size_t map_len = ucsnlen(map, MAX_USTRLEN);
    if (s_len == MISSING_STR_TERMINATOR || map_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;

    ucstr_t p = map;
    ucstr_t map_end = map + map_len;

    ucstr_t first_needle = p;
    while (p < map_end && *p != delim) p++;
    if (p >= map_end) return (ustr_t)NULL;
    size_t first_needle_len = (size_t)(p - first_needle);
    if (!first_needle_len) return (ustr_t)NULL;
    p++;

    ucstr_t first_repl = p;
    while (p < map_end && *p != delim) p++;
    size_t first_repl_len = (size_t)(p - first_repl);

    int multi_pair = (p < map_end);
    if (multi_pair && m) return (ustr_t)NULL;

    if (!multi_pair) {
        size_t needle_len = first_needle_len, repl_len = first_repl_len;
        size_t matches = 0;
        if (m < 0) {
            for (size_t si = 0; si + needle_len <= s_len;) {
                int match = 1;
                for (size_t j = 0; j < needle_len; ++j)
                    if (s[si + j] != first_needle[j]) {match = 0; break;}
                if (match) {matches++; si += needle_len;} else si++;
            }
        }

        size_t target = 0;
        if (m > 0) target = (size_t)m;
        else if (m < 0) {
            ptrdiff_t idx = (ptrdiff_t)matches + m + 1;
            if (idx > 0) target = (size_t)idx;
        }
        if (m < 0 && (!target || target > matches)) {
            if (s_len > tn) {t[tn] = (uchar_t)0; return (ustr_t)NULL;}
            for (size_t i = 0; i < s_len; ++i) t[i] = s[i];
            t[s_len] = (uchar_t)0;
            return t;
        }

        size_t si = 0, ti = 0, occ = 0;
        while (si < s_len) {
            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (s[si + j] != first_needle[j]) match = 0;

            if (match) {
                occ++;
                if (!m || occ == target) {
                    if (ti + repl_len > tn) {
                        t[tn] = (uchar_t)0;
                        return (ustr_t)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j) t[ti + j] = first_repl[j];
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (uchar_t)0;
                        return (ustr_t)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j) t[ti + j] = s[si + j];
                    ti += needle_len;
                }
                si += needle_len;
            } else {
                if (ti >= tn) {
                    t[tn] = (uchar_t)0;
                    return (ustr_t)NULL;
                }
                t[ti++] = s[si++];
            }
        }
        t[ti] = (uchar_t)0;
        return t;
    }

    size_t si = 0, ti = 0;
    while (si < s_len) {
        int matched = 0;
        p = map;
        while (p < map_end) {
            ucstr_t needle = p;
            while (p < map_end && *p != delim) p++;
            size_t needle_len = (size_t)(p - needle);
            if (!needle_len) return (ustr_t)NULL;
            p++;

            ucstr_t repl = p;
            while (p < map_end && *p != delim) p++;
            size_t repl_len = (size_t)(p - repl);

            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (s[si + j] != needle[j]) match = 0;

            if (match) {
                if (ti + repl_len > tn) {
                    t[tn] = (uchar_t)0;
                    return (ustr_t)NULL;
                }
                for (size_t j = 0; j < repl_len; ++j) t[ti + j] = repl[j];
                ti += repl_len;
                si += needle_len;
                matched = 1;
                break;
            }

            if (p < map_end) p++;
        }

        if (!matched) {
            if (ti >= tn) {
                t[tn] = (uchar_t)0;
                return (ustr_t)NULL;
            }
            t[ti++] = s[si++];
        }
    }

    t[ti] = (uchar_t)0;
    return t;
}

/**
 * @name llsreplace, lusreplace, ulsreplace, uusreplace
 * @brief Case-sensitive replace using a delimiter-encoded map (unbounded).
 * @param t Target buffer for result.
 * @param tn Maximum number of result characters written to t
 *        (excluding terminator).
 * @param s Source string.
 * @param map Mapping string encoded as needle/repl pairs.
 * @param delim Mapping delimiter. Must not be the null character.
 * @param m Replacement selector (see *snreplace forms).
 * @return t, or NULL on error.
 * @note Errors include: invalid arguments, unterminated s or map within their
 *       bounds, malformed map syntax (missing delimiter or empty needle),
 *       multi-pair maps used with m != 0, and output longer than tn.
 * @note s is bounded by MAX_LSTRLEN (ll, lu) or MAX_USTRLEN (ul, uu);
 *       map is bounded by its character type's maximum independently.
 * @note If result would exceed tn characters, returns NULL and writes
 *       terminator at t[tn].
 * @note If map has more than one pair, m must be 0.
 * @note For multi-pair maps, the first matching needle in map order wins.
 *       List longer needles before shorter ones if overlap matters.
 */
static inline lstr_t llsreplace(
    lstr_t t, size_t tn, lcstr_t s, lcstr_t map, lchar_t delim, ptrdiff_t m) {
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    return llsnreplace(t, tn, s, map, delim, MAX_LSTRLEN, m);
}
static inline lstr_t lusreplace(
    lstr_t t, size_t tn, lcstr_t s, ucstr_t map, uchar_t delim, ptrdiff_t m) {
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    return lusnreplace(t, tn, s, map, delim, MAX_LSTRLEN, m);
}
static inline ustr_t ulsreplace(
    ustr_t t, size_t tn, ucstr_t s, lcstr_t map, lchar_t delim, ptrdiff_t m) {
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    return ulsnreplace(t, tn, s, map, delim, MAX_USTRLEN, m);
}
static inline ustr_t uusreplace(
    ustr_t t, size_t tn, ucstr_t s, ucstr_t map, uchar_t delim, ptrdiff_t m) {
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    return uusnreplace(t, tn, s, map, delim, MAX_USTRLEN, m);
}

/**
 * @name llsnREPLACEC, lusnREPLACEC, ulsnREPLACEC, uusnREPLACEC
 * @brief Case-insensitive replace using a delimiter-encoded map.
 * @param t Target buffer for result.
 * @param tn Maximum number of result characters written to t
 *        (excluding terminator).
 * @param s Source string.
 * @param map Mapping string encoded as
 *        needle1<delim>repl1<delim>needle2<delim>repl2...
 * @param delim Mapping delimiter. Must not be the null character.
 * @param n Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Replacement selector:
 *        0 replaces all matches;
 *        >0 replaces the mth match from start (1-based);
 *        <0 replaces the mth match from end (-1 is last).
 * @return t, or NULL on error.
 * @note Errors include: invalid arguments, unterminated s or map within their
 *       bounds, malformed map syntax (missing delimiter or empty needle),
 *       multi-pair maps used with m != 0, and output longer than tn.
 * @note Needle matching is case-insensitive; delimiter matching is case-sensitive;
 *       replacement text is written as-is.
 * @note If result would exceed tn characters, returns NULL and writes
 *       terminator at t[tn].
 * @note If map has more than one pair, m must be 0.
 * @note For multi-pair maps, the first matching needle in map order wins.
 *       List longer needles before shorter ones if overlap matters.
 */
static inline lstr_t llsnREPLACEC(
    lstr_t t, size_t tn, lcstr_t s, lcstr_t map, lchar_t delim, size_t n,
    ptrdiff_t m) {
    if (!t || !s || !map || !delim) return (lstr_t)NULL;
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, n);
    size_t map_len = lcsnlen(map, MAX_LSTRLEN);
    if (s_len == MISSING_STR_TERMINATOR || map_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;

    lcstr_t p = map;
    lcstr_t map_end = map + map_len;

    lcstr_t first_needle = p;
    while (p < map_end && *p != delim) p++;
    if (p >= map_end) return (lstr_t)NULL;
    size_t first_needle_len = (size_t)(p - first_needle);
    if (!first_needle_len) return (lstr_t)NULL;
    p++;

    lcstr_t first_repl = p;
    while (p < map_end && *p != delim) p++;
    size_t first_repl_len = (size_t)(p - first_repl);

    int multi_pair = (p < map_end);
    if (multi_pair && m) return (lstr_t)NULL;

    if (!multi_pair) {
        size_t needle_len = first_needle_len, repl_len = first_repl_len;
        size_t matches = 0;
        if (m < 0) {
            for (size_t si = 0; si + needle_len <= s_len;) {
                int match = 1;
                for (size_t j = 0; j < needle_len; ++j)
                    if (tolupper(s[si + j]) != tolupper(first_needle[j])) {match = 0; break;}
                if (match) {matches++; si += needle_len;} else si++;
            }
        }

        size_t target = 0;
        if (m > 0) target = (size_t)m;
        else if (m < 0) {
            ptrdiff_t idx = (ptrdiff_t)matches + m + 1;
            if (idx > 0) target = (size_t)idx;
        }
        if (m < 0 && (!target || target > matches)) {
            if (s_len > tn) {t[tn] = (lchar_t)0; return (lstr_t)NULL;}
            for (size_t i = 0; i < s_len; ++i) t[i] = s[i];
            t[s_len] = (lchar_t)0;
            return t;
        }

        size_t si = 0, ti = 0, occ = 0;
        while (si < s_len) {
            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (tolupper(s[si + j]) != tolupper(first_needle[j])) match = 0;

            if (match) {
                occ++;
                if (!m || occ == target) {
                    if (ti + repl_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lstr_t)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j) t[ti + j] = first_repl[j];
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lstr_t)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j) t[ti + j] = s[si + j];
                    ti += needle_len;
                }
                si += needle_len;
            } else {
                if (ti >= tn) {
                    t[tn] = (lchar_t)0;
                    return (lstr_t)NULL;
                }
                t[ti++] = s[si++];
            }
        }
        t[ti] = (lchar_t)0;
        return t;
    }

    size_t si = 0, ti = 0;
    while (si < s_len) {
        int matched = 0;
        p = map;
        while (p < map_end) {
            lcstr_t needle = p;
            while (p < map_end && *p != delim) p++;
            size_t needle_len = (size_t)(p - needle);
            if (!needle_len) return (lstr_t)NULL;
            p++;

            lcstr_t repl = p;
            while (p < map_end && *p != delim) p++;
            size_t repl_len = (size_t)(p - repl);

            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (tolupper(s[si + j]) != tolupper(needle[j])) match = 0;

            if (match) {
                if (ti + repl_len > tn) {
                    t[tn] = (lchar_t)0;
                    return (lstr_t)NULL;
                }
                for (size_t j = 0; j < repl_len; ++j) t[ti + j] = repl[j];
                ti += repl_len;
                si += needle_len;
                matched = 1;
                break;
            }

            if (p < map_end) p++;
        }

        if (!matched) {
            if (ti >= tn) {
                t[tn] = (lchar_t)0;
                return (lstr_t)NULL;
            }
            t[ti++] = s[si++];
        }
    }

    t[ti] = (lchar_t)0;
    return t;
}
static inline lstr_t lusnREPLACEC(
    lstr_t t, size_t tn, lcstr_t s, ucstr_t map, uchar_t delim, size_t n,
    ptrdiff_t m) {
    if (!t || !s || !map || !delim) return (lstr_t)NULL;
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, n);
    size_t map_len = ucsnlen(map, MAX_USTRLEN);
    if (s_len == MISSING_STR_TERMINATOR || map_len == MISSING_STR_TERMINATOR)
        return (lstr_t)NULL;

    ucstr_t p = map;
    ucstr_t map_end = map + map_len;

    ucstr_t first_needle = p;
    while (p < map_end && *p != delim) p++;
    if (p >= map_end) return (lstr_t)NULL;
    size_t first_needle_len = (size_t)(p - first_needle);
    if (!first_needle_len) return (lstr_t)NULL;
    p++;

    ucstr_t first_repl = p;
    while (p < map_end && *p != delim) p++;
    size_t first_repl_len = (size_t)(p - first_repl);

    int multi_pair = (p < map_end);
    if (multi_pair && m) return (lstr_t)NULL;

    if (!multi_pair) {
        size_t needle_len = first_needle_len, repl_len = first_repl_len;
        size_t matches = 0;
        if (m < 0) {
            for (size_t si = 0; si + needle_len <= s_len;) {
                int match = 1;
                for (size_t j = 0; j < needle_len; ++j) {
                    if ((size_t)first_needle[j] > MAX_LCHAR ||
                        tolupper(s[si + j]) != tolupper((lchar_t)first_needle[j])) {match = 0; break;}
                }
                if (match) {matches++; si += needle_len;} else si++;
            }
        }

        size_t target = 0;
        if (m > 0) target = (size_t)m;
        else if (m < 0) {
            ptrdiff_t idx = (ptrdiff_t)matches + m + 1;
            if (idx > 0) target = (size_t)idx;
        }
        if (m < 0 && (!target || target > matches)) {
            if (s_len > tn) {t[tn] = (lchar_t)0; return (lstr_t)NULL;}
            for (size_t i = 0; i < s_len; ++i) t[i] = s[i];
            t[s_len] = (lchar_t)0;
            return t;
        }

        size_t si = 0, ti = 0, occ = 0;
        while (si < s_len) {
            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if ((size_t)first_needle[j] > MAX_LCHAR ||
                    tolupper(s[si + j]) != tolupper((lchar_t)first_needle[j])) match = 0;

            if (match) {
                occ++;
                if (!m || occ == target) {
                    if (ti + repl_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lstr_t)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j)
                        t[ti + j] = (size_t)first_repl[j] <= MAX_LCHAR ? (lchar_t)first_repl[j] : '?';
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lstr_t)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j) t[ti + j] = s[si + j];
                    ti += needle_len;
                }
                si += needle_len;
            } else {
                if (ti >= tn) {
                    t[tn] = (lchar_t)0;
                    return (lstr_t)NULL;
                }
                t[ti++] = s[si++];
            }
        }
        t[ti] = (lchar_t)0;
        return t;
    }

    size_t si = 0, ti = 0;
    while (si < s_len) {
        int matched = 0;
        p = map;
        while (p < map_end) {
            ucstr_t needle = p;
            while (p < map_end && *p != delim) p++;
            size_t needle_len = (size_t)(p - needle);
            if (!needle_len) return (lstr_t)NULL;
            p++;

            ucstr_t repl = p;
            while (p < map_end && *p != delim) p++;
            size_t repl_len = (size_t)(p - repl);

            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if ((size_t)needle[j] > MAX_LCHAR ||
                    tolupper(s[si + j]) != tolupper((lchar_t)needle[j])) match = 0;

            if (match) {
                if (ti + repl_len > tn) {
                    t[tn] = (lchar_t)0;
                    return (lstr_t)NULL;
                }
                for (size_t j = 0; j < repl_len; ++j)
                    t[ti + j] = (size_t)repl[j] <= MAX_LCHAR ? (lchar_t)repl[j] : '?';
                ti += repl_len;
                si += needle_len;
                matched = 1;
                break;
            }

            if (p < map_end) p++;
        }

        if (!matched) {
            if (ti >= tn) {
                t[tn] = (lchar_t)0;
                return (lstr_t)NULL;
            }
            t[ti++] = s[si++];
        }
    }

    t[ti] = (lchar_t)0;
    return t;
}
static inline ustr_t ulsnREPLACEC(
    ustr_t t, size_t tn, ucstr_t s, lcstr_t map, lchar_t delim, size_t n,
    ptrdiff_t m) {
    if (!t || !s || !map || !delim) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t s_len = ucsnlen(s, n);
    size_t map_len = lcsnlen(map, MAX_LSTRLEN);
    if (s_len == MISSING_STR_TERMINATOR || map_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;

    lcstr_t p = map;
    lcstr_t map_end = map + map_len;

    lcstr_t first_needle = p;
    while (p < map_end && *p != delim) p++;
    if (p >= map_end) return (ustr_t)NULL;
    size_t first_needle_len = (size_t)(p - first_needle);
    if (!first_needle_len) return (ustr_t)NULL;
    p++;

    lcstr_t first_repl = p;
    while (p < map_end && *p != delim) p++;
    size_t first_repl_len = (size_t)(p - first_repl);

    int multi_pair = (p < map_end);
    if (multi_pair && m) return (ustr_t)NULL;

    if (!multi_pair) {
        size_t needle_len = first_needle_len, repl_len = first_repl_len;
        size_t matches = 0;
        if (m < 0) {
            for (size_t si = 0; si + needle_len <= s_len;) {
                int match = 1;
                for (size_t j = 0; j < needle_len; ++j)
                    if (touupper(s[si + j]) != touupper((uchar_t)first_needle[j])) {match = 0; break;}
                if (match) {matches++; si += needle_len;} else si++;
            }
        }

        size_t target = 0;
        if (m > 0) target = (size_t)m;
        else if (m < 0) {
            ptrdiff_t idx = (ptrdiff_t)matches + m + 1;
            if (idx > 0) target = (size_t)idx;
        }
        if (m < 0 && (!target || target > matches)) {
            if (s_len > tn) {t[tn] = (uchar_t)0; return (ustr_t)NULL;}
            for (size_t i = 0; i < s_len; ++i) t[i] = s[i];
            t[s_len] = (uchar_t)0;
            return t;
        }

        size_t si = 0, ti = 0, occ = 0;
        while (si < s_len) {
            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (touupper(s[si + j]) != touupper((uchar_t)first_needle[j])) match = 0;

            if (match) {
                occ++;
                if (!m || occ == target) {
                    if (ti + repl_len > tn) {
                        t[tn] = (uchar_t)0;
                        return (ustr_t)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j) t[ti + j] = (uchar_t)first_repl[j];
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (uchar_t)0;
                        return (ustr_t)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j) t[ti + j] = s[si + j];
                    ti += needle_len;
                }
                si += needle_len;
            } else {
                if (ti >= tn) {
                    t[tn] = (uchar_t)0;
                    return (ustr_t)NULL;
                }
                t[ti++] = s[si++];
            }
        }
        t[ti] = (uchar_t)0;
        return t;
    }

    size_t si = 0, ti = 0;
    while (si < s_len) {
        int matched = 0;
        p = map;
        while (p < map_end) {
            lcstr_t needle = p;
            while (p < map_end && *p != delim) p++;
            size_t needle_len = (size_t)(p - needle);
            if (!needle_len) return (ustr_t)NULL;
            p++;

            lcstr_t repl = p;
            while (p < map_end && *p != delim) p++;
            size_t repl_len = (size_t)(p - repl);

            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (touupper(s[si + j]) != touupper((uchar_t)needle[j])) match = 0;

            if (match) {
                if (ti + repl_len > tn) {
                    t[tn] = (uchar_t)0;
                    return (ustr_t)NULL;
                }
                for (size_t j = 0; j < repl_len; ++j) t[ti + j] = (uchar_t)repl[j];
                ti += repl_len;
                si += needle_len;
                matched = 1;
                break;
            }

            if (p < map_end) p++;
        }

        if (!matched) {
            if (ti >= tn) {
                t[tn] = (uchar_t)0;
                return (ustr_t)NULL;
            }
            t[ti++] = s[si++];
        }
    }

    t[ti] = (uchar_t)0;
    return t;
}
static inline ustr_t uusnREPLACEC(
    ustr_t t, size_t tn, ucstr_t s, ucstr_t map, uchar_t delim, size_t n,
    ptrdiff_t m) {
    if (!t || !s || !map || !delim) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t s_len = ucsnlen(s, n);
    size_t map_len = ucsnlen(map, MAX_USTRLEN);
    if (s_len == MISSING_STR_TERMINATOR || map_len == MISSING_STR_TERMINATOR)
        return (ustr_t)NULL;

    ucstr_t p = map;
    ucstr_t map_end = map + map_len;

    ucstr_t first_needle = p;
    while (p < map_end && *p != delim) p++;
    if (p >= map_end) return (ustr_t)NULL;
    size_t first_needle_len = (size_t)(p - first_needle);
    if (!first_needle_len) return (ustr_t)NULL;
    p++;

    ucstr_t first_repl = p;
    while (p < map_end && *p != delim) p++;
    size_t first_repl_len = (size_t)(p - first_repl);

    int multi_pair = (p < map_end);
    if (multi_pair && m) return (ustr_t)NULL;

    if (!multi_pair) {
        size_t needle_len = first_needle_len, repl_len = first_repl_len;
        size_t matches = 0;
        if (m < 0) {
            for (size_t si = 0; si + needle_len <= s_len;) {
                int match = 1;
                for (size_t j = 0; j < needle_len; ++j)
                    if (touupper(s[si + j]) != touupper(first_needle[j])) {match = 0; break;}
                if (match) {matches++; si += needle_len;} else si++;
            }
        }

        size_t target = 0;
        if (m > 0) target = (size_t)m;
        else if (m < 0) {
            ptrdiff_t idx = (ptrdiff_t)matches + m + 1;
            if (idx > 0) target = (size_t)idx;
        }
        if (m < 0 && (!target || target > matches)) {
            if (s_len > tn) {t[tn] = (uchar_t)0; return (ustr_t)NULL;}
            for (size_t i = 0; i < s_len; ++i) t[i] = s[i];
            t[s_len] = (uchar_t)0;
            return t;
        }

        size_t si = 0, ti = 0, occ = 0;
        while (si < s_len) {
            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (touupper(s[si + j]) != touupper(first_needle[j])) match = 0;

            if (match) {
                occ++;
                if (!m || occ == target) {
                    if (ti + repl_len > tn) {
                        t[tn] = (uchar_t)0;
                        return (ustr_t)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j) t[ti + j] = first_repl[j];
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (uchar_t)0;
                        return (ustr_t)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j) t[ti + j] = s[si + j];
                    ti += needle_len;
                }
                si += needle_len;
            } else {
                if (ti >= tn) {
                    t[tn] = (uchar_t)0;
                    return (ustr_t)NULL;
                }
                t[ti++] = s[si++];
            }
        }
        t[ti] = (uchar_t)0;
        return t;
    }

    size_t si = 0, ti = 0;
    while (si < s_len) {
        int matched = 0;
        p = map;
        while (p < map_end) {
            ucstr_t needle = p;
            while (p < map_end && *p != delim) p++;
            size_t needle_len = (size_t)(p - needle);
            if (!needle_len) return (ustr_t)NULL;
            p++;

            ucstr_t repl = p;
            while (p < map_end && *p != delim) p++;
            size_t repl_len = (size_t)(p - repl);

            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (touupper(s[si + j]) != touupper(needle[j])) match = 0;

            if (match) {
                if (ti + repl_len > tn) {
                    t[tn] = (uchar_t)0;
                    return (ustr_t)NULL;
                }
                for (size_t j = 0; j < repl_len; ++j) t[ti + j] = repl[j];
                ti += repl_len;
                si += needle_len;
                matched = 1;
                break;
            }

            if (p < map_end) p++;
        }

        if (!matched) {
            if (ti >= tn) {
                t[tn] = (uchar_t)0;
                return (ustr_t)NULL;
            }
            t[ti++] = s[si++];
        }
    }

    t[ti] = (uchar_t)0;
    return t;
}

/**
 * @name llsREPLACEC, lusREPLACEC, ulsREPLACEC, uusREPLACEC
 * @brief Case-insensitive replace using a delimiter-encoded map (unbounded).
 * @param t Target buffer for result.
 * @param tn Maximum number of result characters written to t
 *        (excluding terminator).
 * @param s Source string.
 * @param map Mapping string encoded as needle/repl pairs.
 * @param delim Mapping delimiter. Must not be the null character.
 * @param m Replacement selector (see *snREPLACEC forms).
 * @return t, or NULL on error.
 * @note Errors include: invalid arguments, unterminated s or map within their
 *       bounds, malformed map syntax (missing delimiter or empty needle),
 *       multi-pair maps used with m != 0, and output longer than tn.
 * @note Needle matching is case-insensitive; delimiter matching is case-sensitive;
 *       replacement text is written as-is.
 * @note s is bounded by MAX_LSTRLEN (ll, lu) or MAX_USTRLEN (ul, uu);
 *       map is bounded by its character type's maximum independently.
 * @note If result would exceed tn characters, returns NULL and writes
 *       terminator at t[tn].
 * @note If map has more than one pair, m must be 0.
 * @note For multi-pair maps, the first matching needle in map order wins.
 *       List longer needles before shorter ones if overlap matters.
 */
static inline lstr_t llsREPLACEC(
    lstr_t t, size_t tn, lcstr_t s, lcstr_t map, lchar_t delim, ptrdiff_t m) {
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    return llsnREPLACEC(t, tn, s, map, delim, MAX_LSTRLEN, m);
}
static inline lstr_t lusREPLACEC(
    lstr_t t, size_t tn, lcstr_t s, ucstr_t map, uchar_t delim, ptrdiff_t m) {
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    return lusnREPLACEC(t, tn, s, map, delim, MAX_LSTRLEN, m);
}
static inline ustr_t ulsREPLACEC(
    ustr_t t, size_t tn, ucstr_t s, lcstr_t map, lchar_t delim, ptrdiff_t m) {
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    return ulsnREPLACEC(t, tn, s, map, delim, MAX_USTRLEN, m);
}
static inline ustr_t uusREPLACEC(
    ustr_t t, size_t tn, ucstr_t s, ucstr_t map, uchar_t delim, ptrdiff_t m) {
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    return uusnREPLACEC(t, tn, s, map, delim, MAX_USTRLEN, m);
}

/**
 * @name llsnrepeat, lusnrepeat, ulsnrepeat, uusnrepeat
 * @brief Repeat source string into target (bounded).
 * @param t Target buffer for result.
 * @param tn Maximum number of result characters written to t
 *        (excluding terminator).
 * @param s Source string.
 * @param n Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param times Number of times to repeat source. 0 yields empty string.
 * @return t, or NULL on error.
 * @note Errors include: NULL arguments, unterminated s within bound n,
 *       or result exceeding tn characters.
 * @note On error, returns NULL and writes terminator at t[tn].
 */
static inline lstr_t llsnrepeat(
    lstr_t t, size_t tn, lcstr_t s, size_t n, size_t times) {
    if (!t || !s) return (lstr_t)NULL;
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, n);
    if (s_len == MISSING_STR_TERMINATOR) {
        t[tn] = (lchar_t)0;
        return (lstr_t)NULL;
    }

    if (times == 0) {
        t[0] = (lchar_t)0;
        return t;
    }

    if (s_len && times > tn / s_len) {
        t[tn] = (lchar_t)0;
        return (lstr_t)NULL;
    }

    size_t needed = s_len * times;
    if (needed > tn) {
        t[tn] = (lchar_t)0;
        return (lstr_t)NULL;
    }

    for (size_t rep = 0; rep < times; ++rep)
        for (size_t i = 0; i < s_len; ++i)
            t[rep * s_len + i] = s[i];

    t[needed] = (lchar_t)0;
    return t;
}
static inline ustr_t lusnrepeat(
    ustr_t t, size_t tn, ucstr_t s, size_t n, size_t times) {
    if (!t || !s) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t s_len = ucsnlen(s, n);
    if (s_len == MISSING_STR_TERMINATOR) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    if (times == 0) {
        t[0] = (uchar_t)0;
        return t;
    }

    if (s_len && times > tn / s_len) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    size_t needed = s_len * times;
    if (needed > tn) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    for (size_t rep = 0; rep < times; ++rep)
        for (size_t i = 0; i < s_len; ++i)
            t[rep * s_len + i] = s[i];

    t[needed] = (uchar_t)0;
    return t;
}
static inline ustr_t ulsnrepeat(
    ustr_t t, size_t tn, lcstr_t s, size_t n, size_t times) {
    if (!t || !s) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, n);
    if (s_len == MISSING_STR_TERMINATOR) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    if (times == 0) {
        t[0] = (uchar_t)0;
        return t;
    }

    if (s_len && times > tn / s_len) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    size_t needed = s_len * times;
    if (needed > tn) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    for (size_t rep = 0; rep < times; ++rep)
        for (size_t i = 0; i < s_len; ++i)
            t[rep * s_len + i] = (uchar_t)s[i];

    t[needed] = (uchar_t)0;
    return t;
}
static inline ustr_t uusnrepeat(
    ustr_t t, size_t tn, ucstr_t s, size_t n, size_t times) {
    if (!t || !s) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t s_len = ucsnlen(s, n);
    if (s_len == MISSING_STR_TERMINATOR) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    if (times == 0) {
        t[0] = (uchar_t)0;
        return t;
    }

    if (s_len && times > tn / s_len) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    size_t needed = s_len * times;
    if (needed > tn) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    for (size_t rep = 0; rep < times; ++rep)
        for (size_t i = 0; i < s_len; ++i)
            t[rep * s_len + i] = s[i];

    t[needed] = (uchar_t)0;
    return t;
}

/**
 * @name llsrepeat, lusrepeat, ulsrepeat, uusrepeat
 * @brief Repeat source string into target (unbounded).
 * @param t Target buffer for result.
 * @param tn Maximum number of result characters written to t
 *        (excluding terminator).
 * @param s Source string.
 * @param times Number of times to repeat source. 0 yields empty string.
 * @return t, or NULL on error.
 * @note Errors include: NULL arguments, unterminated s within its bound,
 *       or result exceeding tn characters.
 * @note s is bounded by MAX_LSTRLEN (ll, lu) or MAX_USTRLEN (ul, uu).
 * @note On error, returns NULL and writes
 *       terminator at t[tn].
 */
static inline lstr_t llsrepeat(
    lstr_t t, size_t tn, lcstr_t s, size_t times) {
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    return llsnrepeat(t, tn, s, MAX_LSTRLEN, times);
}
static inline ustr_t lusrepeat(
    ustr_t t, size_t tn, ucstr_t s, size_t times) {
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    return lusnrepeat(t, tn, s, MAX_USTRLEN, times);
}
static inline ustr_t ulsrepeat(
    ustr_t t, size_t tn, lcstr_t s, size_t times) {
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    return ulsnrepeat(t, tn, s, MAX_LSTRLEN, times);
}
static inline ustr_t uusrepeat(
    ustr_t t, size_t tn, ucstr_t s, size_t times) {
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    return uusnrepeat(t, tn, s, MAX_USTRLEN, times);
}

/**
 * @name llsnsub, lusnsub, ulsnsub, uusnsub
 * @brief Character substitution (transliterate) source into target (bounded).
 * @param t Target buffer for result.
 * @param tn Maximum number of result characters written to t
 *        (excluding terminator).
 * @param s Source string.
 * @param n Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param from Character set specifying which characters to replace.
 * @param to Replacement character set. Must be same length as from.
 *        If to is shorter than from or NULL, the function returns NULL.
 * @return t, or NULL on error.
 * @note Each character in source that appears in from (at position i) is
 *       replaced with to[i]. Characters not in from are copied as-is.
 * @note Errors include: NULL arguments, from and to having different lengths,
 *       unterminated s, from, or to within their bounds, or result exceeding
 *       tn characters.
 * @note On error, returns NULL and writes terminator at t[tn].
 */
static inline lstr_t llsnsub(
    lstr_t t, size_t tn, lcstr_t s, size_t n, lcstr_t from, lcstr_t to) {
    if (!t || !s || !from || !to) return (lstr_t)NULL;
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, n);
    if (s_len == MISSING_STR_TERMINATOR) {
        t[tn] = (lchar_t)0;
        return (lstr_t)NULL;
    }

    size_t from_len = lcslen(from);
    size_t to_len = lcslen(to);
    if (from_len == MISSING_STR_TERMINATOR ||
        to_len == MISSING_STR_TERMINATOR ||
        from_len != to_len) {
        t[tn] = (lchar_t)0;
        return (lstr_t)NULL;
    }

    if (s_len > tn) {
        t[tn] = (lchar_t)0;
        return (lstr_t)NULL;
    }

    for (size_t i = 0; i < s_len; ++i) {
        lchar_t c = s[i];
        int found = 0;
        for (size_t j = 0; j < from_len; ++j) {
            if (c == from[j]) {
                t[i] = to[j];
                found = 1;
                break;
            }
        }
        if (!found) t[i] = c;
    }

    t[s_len] = (lchar_t)0;
    return t;
}
static inline ustr_t lusnsub(
    ustr_t t, size_t tn, ucstr_t s, size_t n, lcstr_t from, lcstr_t to) {
    if (!t || !s || !from || !to) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t s_len = ucsnlen(s, n);
    if (s_len == MISSING_STR_TERMINATOR) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    size_t from_len = lcslen(from);
    size_t to_len = lcslen(to);
    if (from_len == MISSING_STR_TERMINATOR ||
        to_len == MISSING_STR_TERMINATOR ||
        from_len != to_len) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    if (s_len > tn) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    for (size_t i = 0; i < s_len; ++i) {
        uchar_t c = s[i];
        int found = 0;
        for (size_t j = 0; j < from_len; ++j) {
            if (c == (uchar_t)from[j]) {
                t[i] = (uchar_t)to[j];
                found = 1;
                break;
            }
        }
        if (!found) t[i] = c;
    }

    t[s_len] = (uchar_t)0;
    return t;
}
static inline ustr_t ulsnsub(
    ustr_t t, size_t tn, lcstr_t s, size_t n, ucstr_t from, ucstr_t to) {
    if (!t || !s || !from || !to) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, n);
    if (s_len == MISSING_STR_TERMINATOR) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    size_t from_len = ucslen(from);
    size_t to_len = ucslen(to);
    if (from_len == MISSING_STR_TERMINATOR ||
        to_len == MISSING_STR_TERMINATOR ||
        from_len != to_len) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    if (s_len > tn) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    for (size_t i = 0; i < s_len; ++i) {
        uchar_t c = (uchar_t)s[i];
        int found = 0;
        for (size_t j = 0; j < from_len; ++j) {
            if (c == from[j]) {
                t[i] = to[j];
                found = 1;
                break;
            }
        }
        if (!found) t[i] = c;
    }

    t[s_len] = (uchar_t)0;
    return t;
}
static inline ustr_t uusnsub(
    ustr_t t, size_t tn, ucstr_t s, size_t n, ucstr_t from, ucstr_t to) {
    if (!t || !s || !from || !to) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t s_len = ucsnlen(s, n);
    if (s_len == MISSING_STR_TERMINATOR) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    size_t from_len = ucslen(from);
    size_t to_len = ucslen(to);
    if (from_len == MISSING_STR_TERMINATOR ||
        to_len == MISSING_STR_TERMINATOR ||
        from_len != to_len) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    if (s_len > tn) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
    }

    for (size_t i = 0; i < s_len; ++i) {
        uchar_t c = s[i];
        int found = 0;
        for (size_t j = 0; j < from_len; ++j) {
            if (c == from[j]) {
                t[i] = to[j];
                found = 1;
                break;
            }
        }
        if (!found) t[i] = c;
    }

    t[s_len] = (uchar_t)0;
    return t;
}

/**
 * @name llssub, lussub, ulssub, uussub
 * @brief Character substitution (transliterate) source into target (unbounded).
 * @param t Target buffer for result.
 * @param tn Maximum number of result characters written to t
 *        (excluding terminator).
 * @param s Source string.
 * @param from Character set specifying which characters to replace.
 * @param to Replacement character set. Must be same length as from.
 * @return t, or NULL on error.
 * @note Each character in source that appears in from (at position i) is
 *       replaced with to[i]. Characters not in from are copied as-is.
 * @note Errors include: NULL arguments, from and to having different lengths,
 *       unterminated s, from, or to, or result exceeding tn characters.
 * @note s is bounded by MAX_LSTRLEN (ll, lu) or MAX_USTRLEN (ul, uu).
 * @note On error, returns NULL and writes terminator at t[tn].
 */
static inline lstr_t llssub(
    lstr_t t, size_t tn, lcstr_t s, lcstr_t from, lcstr_t to) {
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    return llsnsub(t, tn, s, MAX_LSTRLEN, from, to);
}
static inline ustr_t lussub(
    ustr_t t, size_t tn, ucstr_t s, lcstr_t from, lcstr_t to) {
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    return lusnsub(t, tn, s, MAX_USTRLEN, from, to);
}
static inline ustr_t ulssub(
    ustr_t t, size_t tn, lcstr_t s, ucstr_t from, ucstr_t to) {
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    return ulsnsub(t, tn, s, MAX_LSTRLEN, from, to);
}
static inline ustr_t uussub(
    ustr_t t, size_t tn, ucstr_t s, ucstr_t from, ucstr_t to) {
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    return uusnsub(t, tn, s, MAX_USTRLEN, from, to);
}

/**
 * @name llsnsplit, lusnsplit, ulsnsplit, uusnsplit
 * @brief Split source at first delimiter into left and right outputs (bounded).
 * @param left Output buffer for part before delimiter.
 * @param right Output buffer for part after delimiter.
 * @param s Source string.
 * @param delim Delimiter character.
 * @param n Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return 2 if delimiter found, 1 if not found (right becomes empty), 0 on error.
 */
static inline size_t llsnsplit(
    lstr_t left, lstr_t right, lcstr_t s, lchar_t delim, size_t n) {
    if (!left || !right || !s) return 0;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;

    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR) return 0;

    size_t split = len;
    for (size_t i = 0; i < len; ++i) {
        if (s[i] == delim) {split = i; break;}
    }

    for (size_t i = 0; i < split; ++i) left[i] = s[i];
    left[split] = (lchar_t)0;

    if (split < len) {
        size_t rlen = len - split - 1;
        for (size_t i = 0; i < rlen; ++i) right[i] = s[split + 1 + i];
        right[rlen] = (lchar_t)0;
        return 2;
    }

    right[0] = (lchar_t)0;
    return 1;
}
static inline size_t lusnsplit(
    lstr_t left, lstr_t right, lcstr_t s, uchar_t delim, size_t n) {
    if (!left || !right || !s) return 0;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;

    size_t len = lcsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR) return 0;

    size_t split = len;
    if ((size_t)delim <= MAX_LCHAR) {
        lchar_t d = (lchar_t)delim;
        for (size_t i = 0; i < len; ++i) {
            if (s[i] == d) {split = i; break;}
        }
    }

    for (size_t i = 0; i < split; ++i) left[i] = s[i];
    left[split] = (lchar_t)0;

    if (split < len) {
        size_t rlen = len - split - 1;
        for (size_t i = 0; i < rlen; ++i) right[i] = s[split + 1 + i];
        right[rlen] = (lchar_t)0;
        return 2;
    }

    right[0] = (lchar_t)0;
    return 1;
}
static inline size_t ulsnsplit(
    ustr_t left, ustr_t right, ucstr_t s, lchar_t delim, size_t n) {
    if (!left || !right || !s) return 0;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR) return 0;

    size_t split = len;
    uchar_t d = (uchar_t)delim;
    for (size_t i = 0; i < len; ++i) {
        if (s[i] == d) {split = i; break;}
    }

    for (size_t i = 0; i < split; ++i) left[i] = s[i];
    left[split] = (uchar_t)0;

    if (split < len) {
        size_t rlen = len - split - 1;
        for (size_t i = 0; i < rlen; ++i) right[i] = s[split + 1 + i];
        right[rlen] = (uchar_t)0;
        return 2;
    }

    right[0] = (uchar_t)0;
    return 1;
}
static inline size_t uusnsplit(
    ustr_t left, ustr_t right, ucstr_t s, uchar_t delim, size_t n) {
    if (!left || !right || !s) return 0;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t len = ucsnlen(s, n);
    if (len == MISSING_STR_TERMINATOR) return 0;

    size_t split = len;
    for (size_t i = 0; i < len; ++i) {
        if (s[i] == delim) {split = i; break;}
    }

    for (size_t i = 0; i < split; ++i) left[i] = s[i];
    left[split] = (uchar_t)0;

    if (split < len) {
        size_t rlen = len - split - 1;
        for (size_t i = 0; i < rlen; ++i) right[i] = s[split + 1 + i];
        right[rlen] = (uchar_t)0;
        return 2;
    }

    right[0] = (uchar_t)0;
    return 1;
}

/**
 * @name llsplit, lusplit, ulsplit, uusplit
 * @brief Split source at first delimiter (unbounded).
 * @param left Output buffer for part before delimiter.
 * @param right Output buffer for part after delimiter.
 * @param s Source string (bounded by MAX_LSTRLEN or MAX_USTRLEN).
 * @param delim Delimiter character.
 * @return 2 if delimiter found, 1 if not found, 0 on error.
 */
static inline size_t llsplit(lstr_t left, lstr_t right, lcstr_t s, lchar_t delim)
    {return llsnsplit(left, right, s, delim, MAX_LSTRLEN);}
static inline size_t lusplit(lstr_t left, lstr_t right, lcstr_t s, uchar_t delim)
    {return lusnsplit(left, right, s, delim, MAX_LSTRLEN);}
static inline size_t ulsplit(ustr_t left, ustr_t right, ucstr_t s, lchar_t delim)
    {return ulsnsplit(left, right, s, delim, MAX_USTRLEN);}
static inline size_t uusplit(ustr_t left, ustr_t right, ucstr_t s, uchar_t delim)
    {return uusnsplit(left, right, s, delim, MAX_USTRLEN);}

/**
 * @name llvsnprintf, llsnprintf, llvsprintf, llsprintf
 * @brief Format Latin text into target buffer.
 * @param t Target buffer.
 * @param tn Target capacity in bytes, including terminator.
 * @param fmt Format string.
 * @param ap Variable argument list for llvsnprintf/llvsprintf.
 * @return Number of chars written (excluding terminator), or -1 on error/truncate.
 * @note This family uses C vsnprintf semantics and treats lchar_t storage as bytes.
 */
static inline int llvsnprintf(lstr_t t, size_t tn, lcstr_t fmt, va_list ap) {
    if (!t || !fmt || !tn) {
        if (t && tn) *t = (lchar_t)0;
        return -1;
    }

    if (tn > MAX_LSTRLEN + 1) tn = MAX_LSTRLEN + 1;

    int rc = vsnprintf((char *)t, tn, (const char *)fmt, ap);
    if (rc < 0 || (size_t)rc >= tn) {
        t[tn - 1] = (lchar_t)0;
        return -1;
    }
    return rc;
}
static inline int llsnprintf(lstr_t t, size_t tn, lcstr_t fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int rc = llvsnprintf(t, tn, fmt, ap);
    va_end(ap);
    return rc;
}
static inline int llvsprintf(lstr_t t, lcstr_t fmt, va_list ap)
    {return llvsnprintf(t, MAX_LSTRLEN + 1, fmt, ap);}
static inline int llsprintf(lstr_t t, lcstr_t fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int rc = llvsnprintf(t, MAX_LSTRLEN + 1, fmt, ap);
    va_end(ap);
    return rc;
}

#ifdef __cplusplus
}
#endif

// End of lubtype.h