/**
 * @file lubtype.h
 * @mainpage Latin/Unicode/Byte API
 * @brief This API header provides robust and portable Latin-8,
 *        Unicode-16, and byte types, plus associated macros, extern function
 *        declarations (protypes), and static inline function
 *        definitions.
 * 
 *        To include definitions of extern functions,
 * 
 *             #define LUB_DEFINITIONS 
 * 
 *        before including this header.
 * 
 *        Functions include classification, character transformation,
 *        length, concatenate, copy, search, compare, fixed/prefix/suffix
 *        compare, transform, replace, format, validation,
 *        split, span, character substitutiion, and count.
 * 
 *        - Variants for Latin/Unicode/Byte <- Latin/Unicode/Byte.
 *        - Variants for explicit/default bounds on source length.
 *        - Explicit bound on target buffer size.
 *        - Explicit or default bound on source length.
 *        - Variants for character case-preserving
 *          uppercase, lowercase mapping.
 *        - Case-sensitive/insensitive character matching.
 * @note Not all variants are provided for all functions due a
 *       variant not being meaningful/useful for a particular function
 *       or the variant being easily implemented by the caller
 *       using a combination of provided variants.
 * @warning Due to the naming conventions used in this API for types, macros,
 *          and functions, it is not expected name conflicts will arise.
 *          If a conflict does arise, it must be resolved by updating
 *          the caller code or the API.
 * @copyright Copyright (c) 2026 paulsinclair51
 * @license SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
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

#if defined(__LUB_STRINGIFY2__) || defined(__LUB_STRINGIFY__) || \
    defined(__LUB_STATIC_ASSERT__)
#error "lubtype.h: A __LUB_STRINGIFY2__, __LUB_STRINGIFY__, " \
       "or __LUB_STATIC_ASSERT__ macro is unexpectedly already defined. " \
       "#undef before including lubtype.h. " \
       "These macros are undefined after their last use " \
       "by this include. After including, define again as needed."
#endif

// Token pasting and stringification helpers (undefined after versioning set).
#define __LUB_PASTE2__(a, b) a##b
#define __LUB_PASTE__(a, b) __LUB_PASTE2__(a, b)
#define __LUB_STRINGIFY2__(x) #x
#define __LUB_STRINGIFY__(x) __LUB_STRINGIFY2__(x)

// STATIC_ASSERT
//
// Usage:
//   LUB_STATIC_ASSERT(sizeof(int) == 4, int_must_be_4_bytes);
//
// Expands (C99 fallback) to:
//   typedef char LUB_STATIC_ASSERT__int_must_be_4_bytes[1];
//
//  Or (if false):
//    typedef char LUB_STATIC_ASSERT__int_must_be_4_bytes[-1];
//
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    /* C11 and later: use the built‑in */
    #define LUB_STATIC_ASSERT(cond, msg) _Static_assert(cond, #msg)
#else
    /* C99 fallback: typedef with negative array size */
    #define LUB_STATIC_ASSERT(cond, msg) \
        typedef char __LUB_PASTE__(LUB_STATIC_ASSERT__, msg)[(cond) ? 1 : -1]
#endif

/**
 * @section Versioning Versioning
 * @defgroup LUBVersioningMacros LUB Versioning Macros
 * @name LUB_VERSION_MAJOR, LUB_VERSION_MINOR, LUB_VERSION_PATCH,
 *       LUB_VERSION, LUB_VERSION_NUM, LUB_VERSION_HEX, 
 *       LUB_VERSION_EQ, LUB_VERSION_AT_LEAST
 * @brief Versioning macros for the LUB API (lubtype.h):
 * 
 *        LUB_VERSION_MAJOR
 *           Major version number, incremented for incompatible API changes.
 *           String form, e.g., "1" for major version 1.
 * 
 *        LUB_VERSION_MINOR
 *           Minor version number, incremented for backward-compatible additions.
 *           String form, e.g., "0" for minor version 0,
 *           or "22" for minor version 22.
 * 
 *        LUB_VERSION_PATCH
 *           Patch version number, incremented for bug fixes or internal improvements.
 *           String form, e.g., "0" for patch version 0,
 *           or "12" for patch version 12.
 * 
 *        LUB_VERSION
 *           String form, e.g., "1.0.0" for major version 1, minor
 *           version 0, patch version 0.
 * 
 *        LUB_VERSION_NUM
 *           Integer form MMmmpp for comparisons, e.g., 10000 for
 *           version 1.0.0, 10200 for version 1.2.0, or 11212 for version 1.12.12.
 * 
 *        LUB_VERSION_HEX
 *           Hexadecimal form 0xMMmmpp for display/debugging, e.g.,
 *           0x010000 for version 1.0.0, 0x010200 for version 1.2.0,
 *           or 0x011212 for version 1.12.12.
 * 
 *        LUB_VERSION_EQ(maj,min,pat)
 *           True if current version is exactly maj.min.pat
 * .
 *        LUB_VERSION_AT_LEAST(maj,min,pat)
 *           True if current version is at least maj.min.pat.
 * @note An error is raised if any of the versioning macros (or
 *       __LUB_STRINGIFY2__, __LUB_STRINGIFY__, __LUB_STATIC_ASSERT__)
 *       are already defined before including lubtype.h.
 * @note Helper macros __LUB_STRINGIFY2__, __LUB_STRINGIFY__,
 *       and __LUB_STATIC_ASSERT__ are undefined after use by
 *       lubtype.h to avoid namespace pollution.
 * @note The naming conventions, error semantics, and safety guarantees
 *       are part of the stable API and will not change without a
 *       major version increment.
 * @{
 */

#if defined(LUB_VERSION_MAJOR) || defined(LUB_VERSION_MINOR) || \
    defined(LUB_VERSION_PATCH) || \
    defined(LUB_VERSION) || \
    defined(LUB_VERSION_NUM) || defined(LUB_VERSION_HEX) || \
    defined(LUB_VERSION_EQ) || defined(LUB_VERSION_AT_LEAST)
#error "lubtype.h: A LUB_VERSION_* macro is unexpectedly " \
       "already defined. #undef before including lubtype.h. " \
       "After including, undef and define again as needed if " \
       "a LUB_VERSION_* definition is not required."
#endif

// Library version major, minor, patch.
#define LUB_VERSION_MAJOR 1
#define LUB_VERSION_MINOR 0
#define LUB_VERSION_PATCH 0

// Library version string in "major.minor.patch" format.
#define LUB_VERSION \
    __LUB_STRINGIFY__(LUB_VERSION_MAJOR) "." \
    __LUB_STRINGIFY__(LUB_VERSION_MINOR) "." \
    __LUB_STRINGIFY__(LUB_VERSION_PATCH)

// Library version as an integer for comparisons.
#define LUB_VERSION_NUM \
    ((int)((LUB_VERSION_MAJOR * 10000) + \
    (LUB_VERSION_MINOR * 100) + (LUB_VERSION_PATCH)))

// Library version encoded as 0xMMmmpp (major, minor, patch) for display/debug.
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
#define __LUB_STATIC_ASSERT__(cond, msg) \
    typedef char static_assert_##msg[(cond) ? 1 : -1]

// Ensure version components are within expected ranges.
__LUB_STATIC_ASSERT__(LUB_VERSION_MAJOR >= 0, major_must_be_nonnegative);
__LUB_STATIC_ASSERT__(LUB_VERSION_MINOR >= 0, minor_must_be_nonnegative);
__LUB_STATIC_ASSERT__(LUB_VERSION_PATCH >= 0, patch_must_be_nonnegative);

// Ensure version components fit in the hex encoding fields.
__LUB_STATIC_ASSERT__(LUB_VERSION_MAJOR <= 255, major_fits_in_hex_field);
__LUB_STATIC_ASSERT__(LUB_VERSION_MINOR <= 255, minor_fits_in_hex_field);
__LUB_STATIC_ASSERT__(LUB_VERSION_PATCH <= 255, patch_fits_in_hex_field);

// Ensure the numeric encoding cannot overflow int.
__LUB_STATIC_ASSERT__(LUB_VERSION_NUM >= 0, version_num_nonnegative);
__LUB_STATIC_ASSERT__(LUB_VERSION_NUM < 100000000, version_num_reasonable);

// Undefine version helper macros to avoid namespace pollution.
#undef __LUB_STRINGIFY2__
#undef __LUB_STRINGIFY__
#undef __LUB_STATIC_ASSERT__
/** @} */

// End of Versioning.

#if defined(LUB_DEFINITIONS)
// This API expects size_t and intptr_t are the same size,
// short is 2 bytes, and int is 4 bytes, and
// wchar_t is 4 bytes. If not, force a compile error (invalid typedef).
typedef char static_assert_unexpected_type_sizes
                 [(sizeof(size_t) == sizeof(intptr_t) &&
                   sizeof(short) == 2 && sizeof(int) == 4 &&
                   sizeof(wchar_t) == 4) ? 1 : -1];
#endif // LUB_DEFINITIONS

/**
 * @section Types Types
 * 
 * @defgroup LatinCharacterType Latin Character Type
 * @name lchar_t
 * @brief Latin-8 character type (1 byte)
 *        Base type: uint8_t, Values: 0-255 (0x00-0xFF).
 * @{
 */

typedef uint8_t lchar_t;
/** @} */

/**
 * @defgroup UnicodeCharacterType Unicode Character Type
 * @name uchar_t
 * @brief UTF-16 Unicode character type (2 bytes)
 *        Base type: uint16_t, Values: 0-65535 (0x0000-0xFFFF).
 * @{
 */

typedef uint16_t uchar_t;  
/** @} */

/**
 * @defgroup ByteType Byte Type
 * @name byte_t
 * @brief Byte type (1 byte)
 *        Base type: uint8_t, Values: 0-255 (0x00-0xFF).
 * @{
 */

typedef uint8_t byte_t;
/** @} */

/**
 * @defgroup CharacterLimits Character Limits
 * @name LUB_MAX_LCHAR, LUB_MAX_UCHAR, LUB_MAX_BYTE
 * @brief Maximum values for lchar_t, uchar_t, and byte_t.
 * @{
 */

#if defined(LUB_MAX_LCHAR) || defined(LUB_MAX_UCHAR) || defined(LUB_MAX_BYTE)
#error "lubtype.h: A character limit LUB_MAX_* "\
       "macro is unexpectedly already defined. " \
       "#undef before including lubtype.h. " \
       "After including, undef and define again as needed ' \
       "if the LUB_MAX_* definition is not required."
#endif

#define LUB_MAX_LCHAR ((size_t)255)
#define LUB_MAX_UCHAR ((size_t)65535)
#define LUB_MAX_BYTE ((size_t)255)
/** @} */

/**
 * @defgroup StringLimits String Limits
 * @name LUB_MAX_LSTRLEN, LUB_MAX_USTRLEN,
 *       LUB_MAX_UNAMELEN, LUB_MAX_UQNAMELEN,
 *       LUB_MAX_LOPTLEN, LUB_MAX_UOPTLEN,
 *       LUB_MAX_BSTRLEN, LUB_MAX_BOPTLEN
 * @brief Maximum number of characters for an lchar_t string,
 *        uchar_t string, name, quoted name, or option (not
 *        including the null terminator)
 *        Maximum number of bytes for a byte_t string or byte option.
 * @note MAX_LNAMELEN and MAX_LQNAMELEN are not provided since
 *       names are Unicode.
 * @{
 */

#if defined(LUB_MAX_LSTRLEN) || defined(LUB_MAX_USTRLEN) || \
    defined(LUB_MAX_UNAMELEN) || defined(LUB_MAX_UQNAMELEN) || \
    defined(LUB_MAX_LOPTLEN) || defined(LUB_MAX_UOPTLEN) || \
    defined(LUB_MAX_BSTRLEN) || defined(LUB_MAX_BOPTLEN)
#error "lubtype.h: A string limit LUB_MAX_* macro " \
       "is unexpectedly already defined. " \
       "#undef before including lubtype.h. " \
       "After including, undef and define again as needed if " \
       "a LUB_MAX_* definition is not required."
#endif

#define LUB_MAX_LSTRLEN    ((size_t)1000000)
#define LUB_MAX_USTRLEN    ((size_t)500000)
#define LUB_MAX_UNAMELEN   ((size_t)128)
#define LUB_MAX_UQNAMELEN  ((size_t)(2*(LUB_MAX_UNAMELEN) + 2))
#define LUB_MAX_LOPTLEN    ((size_t)64000)
#define LUB_MAX_UOPTLEN    ((size_t)32000)
#define LUB_MAX_BSTRLEN    ((size_t)1000000)
#define LUB_MAX_BOPTLEN    ((size_t)64000)
/** @} */

/**
 * @defgroup SpecialReturnValues Special Return Values
 * @name LUB_CMP_GREATER_THAN, LUB_CMP_EQUAL, LUB_CMP_LESS_THAN,
 *       LUB_QUOTEDNAME, LUB_UNQUOTEDNAME
 * @brief Special values for int-returning comparison and name classification.
 * @note For int returning comparison functions:
 * 
 *         LUB_CMP_LESS_THAN (-1) indicates s1 < s2.
 *         LUB_CMP_EQUAL (0) indicates s1 == s2.
 *         LUB_CMP_GREATER_THAN (1) indicates s1 > s2.
 * 
 *       Note that this differs from the standard strcmp
 *       convention of returning an undefined negative
 *       value for s1 < s2 and an undefined positive
 *       value for s1 > s2. Returning specific values
 *       allows for error return values that are between -99 and -2
 *       that are distinct from valid comparison return values.
 * @{
 */

#if defined(LUB_CMP_GREATER_THAN) || \
    defined(LUB_CMP_EQUAL) || \
    defined(LUB_CMP_LESS_THAN)(
#error "lubtype.h: A special value LUB_CMP_* " \
       "macro is unexpectedly already defined. " \
       "#undef before including lubtype.h. " \
       "After including, undef and define again as needed if " \
       "a LUB_CMP_* definition is not required."
#endif

#if defined(LUB_QUOTEDNAME) || \
    defined(LUB_UNQUOTEDNAME)
#error "lubtype.h: A special value LUB_QUOTEDNAME " \
       "or LUB_UNQUOTEDNAME macro is unexpectedly already defined. " \
       "#undef before including lubtype.h. " \
       "After including, undef and define again as needed if " \
       "a LUB_*QUOTEDNAME definition is not required."
#endif

//Special values for comparison result.
#define LUB_CMP_GREATER_THAN       (1)
#define LUB_CMP_EQUAL              (0)
#define LUB_CMP_LESS_THAN          (-1)

// Special values for names.
#define LUB_QUOTEDNAME             (1)
#define LUB_UNQUOTEDNAME           (0)

/**
 * @defgroup ErrorReturnValues Error Return Values
 * @name LUB_BAD_PTR
 *       LUB_UNTERMINATED
 *       LUB_INVALID_NAME
 *       LUB_OPT_TOO_LONG
 *       LUB_OPT_INVALID
 *       LUB_OPT_RESERVED
 *       LUB_OVERLAP
 *       LUB_TRUNCATED
 * @brief Reserved error values for size_t, pointer, and
 * int error-returning functions (for use with LUB_*_ERR macros,
 * see @ref ErrorClassificationAndCast).
 *
 * Error values are reserved in the range -99 to -2 for int,
 * (size_t)-99 to (size_t)-2 for size_t, and (void *)-99 to
 * (void *)-2 for pointer types. These values are distinct
 * from valid results.
 * @{
 */

#if defined(LUB_BAD_PTR) || \
    defined(LUB_UNTERMINATED) || \
    defined(LUB_INVALID_NAME) || \
    defined(LUB_OPT_TOO_LONG) || \
    defined(LUB_OPT_INVALID) || \
    defined(LUB_OPT_RESERVED) || \
    defined(LUB_OVERLAP) || \
    defined(LUB_TRUNCATED)
#error "lubtype.h: An error value LUB_* macro is unexpectedly " \
       "already defined. #undef before including lubtype.h. " \
       "After including, undef and define again as needed if " \
       "a LUB_* definition is not required."
#endif

// Error values.
#define LUB_BAD_PTR                (-2)
#define LUB_UNTERMINATED           (-3)
#define LUB_INVALID_NAME           (-4)
#define LUB_OPT_TOO_LONG           (-5)
#define LUB_OPT_INVALID            (-6)
#define LUB_OPT_RESERVED           (-7)
#define LUB_OVERLAP                (-8)
#define LUB_TRUNCATED              (-9)
#define LUB_OPERATION_INVALID      (-99) // Internal error.
// -11 to -98 reserved for future error values.
/** @} */

/**
 * @defgroup ErrorClassificationAndCast Error Classification and Cast
 * @name LUB_PTR_ERR, LUB_SIZE_ERR, LUB_INT_ERR
 * @brief Macros for classifying value as an error value and casting error values to
 *        pointer, size_t, and int types.
 * @param value The value returned by a function to be classified or cast.
 * @param error 0 (any error) or a specific error value.
 * @return The value (if it is error value) or 0 (not an error)
 *         cast to the corresponding type:
 *
 *             LUB_PTR_ERR -> void *
 *             LUB_SIZE_ERR -> size_t
 *             LUB_INT_ERR -> int
 * @example
 *   Examples of using error values and macros:
 * 
 *   * Use the LUB_*_ERR macros to check if a returned value is an error:
 * 
 *     * if (LUB_SIZE_ERR(result, 0)) { error handling }
 *       else { handle non-error result }
 * 
 *     * if (LUB_PTR_ERR(result, 0)) { error handling }
 *       else { handle non-error result }
 * 
 *     * if (LUB_INT_ERR(result, 0)) { error handling }
 *       else { handle non-error result }
 * 
 *   * Use the LUB_*_ERR macros to check if a returned value is a specific error:
 * 
 *     * if (LUB_SIZE_ERR(result, LUB_UNTERMINATED)) { handle unterminated error }
 * 
 *     * if (LUB_PTR_ERR(result, LUB_BAD_PTR)) { handle bad pointer error }
 * 
 *   * Use the LUB_*_ERR macros to cast an error value
 *     from a called function to thr return type
 *.    of the calling function:
 * 
 *     * if (LUB_SIZE_ERR(result, 0)) return (uchar_t *)LUB_PTR_ERR(result, 0);
 *
 *   * For input pointer arguments (e.g., s), encountering an error value
 *     indicates an invalid pointer. In this case, return LUB_BAD_PTR:
 *
 *     * if (LUB_PTR_ERR(s, 0)) return LUB_SIZE_ERR(LUB_BAD_PTR, 0);
 * @{
 */

#if defined(LUB_PTR_ERR) || \
    defined(LUB_SIZE_ERR) || \
    defined(LUB_INT_ERR)
#error "lubtype.h: A LUB_*_ERR macro " \
       "is unexpectedly already defined. " \
       "#undef before including lubtype.h. " \
       "After including, undef and define again as needed if " \
       "a LUB_*_ERR definition is not required."
#endif

#define LUB_PTR_ERR(value, error) \
    ((!(error) && \
      (void *)(intptr_t)(value) >= (void *)(intptr_t)-99 && \
      (void *)(intptr_t)(value) <= (void *)(intptr_t)-2) || \
     ((void *)(intptr_t)(error) >= (void *)(intptr_t)-99 && \
      (void *)(intptr_t)(error) <= (void *)(intptr_t)-2  && \
      (void *)(intptr_t)(value) == (void *)(intptr_t)(error) \
     ) ? (void *)(intptr_t)(value) : (void *)(intptr_t)0)

#define LUB_SIZE_ERR(value, error) \
    ((size_t)(intptr_t)LUB_PTR_ERR(value, error))

#define LUB_INT_ERR(value, error) \
    ((int)(intptr_t)LUB_PTR_ERR(value, error))
/** @} */

/**
 * @section Functions Functions
 */

/**
 * @subsection FunctionNamingConventions Function Naming Conventions
 *
 * {} required. [] optional. <> token. | alternatives.
 * 
 * l = Latin (lchar_t), u = Unicode (uchar_t), b = byte (byte_t)
 *
 * 1. Character Classification:
 *
 *    is<s><kind>
 * 
 *    <s> = l for classify Latin character.
 *          u for classify Unicode character.
 * 
 *    <kind> = alpha, digit, alnum, name1c, namec,
 *             upper, lower, cntrl, print, graph, punct,
 *             blank, space, hexdigit
 *
 *    Note: islname1c and islnamec are not
 *    provided since names are Unicode.
 *
 *    Examples: isualpha, islhexdigit, isunamec
 * 
 * 2. String Classification:
 *
 *    is<s>n<kind>
 * 
 *    <s> = l for classify Latin string.
 *          u for classify Unicode string.
 * 
 *    n = bounded source with an sn parameter for the bound.
 * 
 *    <kind> = RESERVED, QNAME, truncstr, hexdigits
 *
 *    Note: RESERVED and QNAME are case-insensitive.
 * 
 *    Note: islRESERVED and islQNAME are not provided
 *          since names are Unicode.
 * 
 *    Note: isutruncstr is not provided string is always Latin.
 *
 *    Examples: isualpha, islhexdigits, isureserved, tolupper
 *
 * 3. Character <- character case transforms:
 *
 *    <ts>to<case>
 * 
 *    <rs> = result/source types:
 *
 *         ll - Latin result and source character
 *         lu - Latin result and Unicode source character
 *         ul - Unicode result and Latin source character
 *         uu - Unicode result and source character
 * 
 *    <case> = case, lower, upper
 *
 *.   Note: case indicates case-preserving transform.
 *
 *    Examples: ultoupper, lutolower, lutocase
 *
 * 4. int <- character transforms:
 *
 *    i<s>hexdigit
 *
 *    <s> = l for Latin source character.
 *          u for Unicode source character.
 *
 *    Example: iuhexdigit
 *
 * 5. String length functions:
 * 
 *    <s>csnlen
 * 
 *    <s> = l for Latin source string.
 *          u for Unicode source string.
 * 
 *    n = bounded source with an sn
 *        parameter for the bound.
 * 
 *  Examples: lcsnlen, ucsnlen
 * 
 * 6. Other functions:
 *
 *    <ts>s[nn|n]<op>
 *
 *    <ts> = target/source or source/source types:
 *
 *         ll - Latin, Latin
 *         lu - Latin, Unicode
 *         ul - Unicode, Latin
 *         uu - Unicode, Unicode
 *         lb - Latin, Byte
 *         ub - Unicode, Byte
 *         bl - Byte, Latin
 *         bu - Byte, Unicode
 * 
 *   nn = bounded target buffer and bounded source(s) with
 *        tn and sn parameters for the target bound
 *        and source bound(s), respectively.
 *   n = bounded target with tn parameter for the
 *       bound (implicit bound on source) or bounded source (s) with an sn
 *       parameter for the bound depending on the operation.
 * 
 *   <op> = operation:
 *
 *     cat/catc/CATC = concatenate to target.
 *     catq/catqc/CATQC = concatenate to target with quoting.
 *     catqnameq/catqnamec/CATQNAMEQC = concatenate to target
 *                                      with quoting for a name.
 *
 *     cpy/cpyc/CPYC = copy to target.
 *     cpyq/cpyqc/CPYQC = copy to target with quoting.
 *     cpyqname/cpyqnamec/CPYQNAMEC = copy to target with quoting
 *                                    for a name (maximum length of source
 *.                                   is implicitly bounded to 128).
 * 
 *     trim/reverse/pad = copy modified string to target.
 *     replace/REPLACE = character/string replacement.
 * 
 *     cmp/CMP = compare.
 *     fxdcmp//FXDCMP = fixed-length leading substring compare.
 *     pfxcmp/PFXCMP = prefix compare.
 *     sfxcmp/SFXCMP = suffix compare.
 * 
 *     ptrim = return pointer into string to start of trimmed
 *             substring plus length of trimmed substring.
 * 
 *     skip = skip characters.
 *
 *     strm/STRM = character/string search for mth occurrence.
 * 
 *     cnt/CNT = count occurrences of characters or substrings.
 * 
 *     vprintf/printf = format string.
 * 
 *    Examples: lusnncpy, ulsnnCATC, uusnCMP, ulsnnSTRM, uusncatqnamec,
 *              uusnCATQNAMEC, ulsncpyqname, ulsncpyqnamec.
 *
 * @note Operations with case handling
 *
 *   lowercase operation (no 'c') = case-sensitive character comparison or
 *                                  no case mapping
 *   uppercase operation (no 'C') = case-insensitive matching/comparison
 *   lowercase operation with 'c' = case-mapped to lowercase
 *   uppercase operation with 'C' = case-mapped to uppercase
 */

/**
 * @subsection  CommonParameters Common Parameters
 *
 * Commonly used parameters for the functions in this API.
 * Specific functions may use a subset of these parameters,
 * have additional parameters, or have additional details
 * or requirements for these parameters (see specific
 * function declarations and definitions for details).
 * 
 * @param t Pointer to target buffer (destination for operations such as
 *          concatenate, copy, trim, replace, and pad).
 * 
 * @param tn Target buffer size (excluding null terminator).
 *           tn is silently bounded by LUB_MAX_LSTRLEN, LUB_MAX_USTRLEN,
 *           or LUB_MAX_BSTRLEN based on the type of the target string.
 * 
 * @param s Pointer to source string for operations such as concatenate/copy
 *          and string to search for search functions.
 * 
 * @param s1 Pointer to left source string for compare functions.
 * 
 * @param s2 Pointer to right source string for compare or
 *           needle string for search functions.
 * 
 * @param sn Bound on source length (excluding null terminator).
 *           sn itself is silently bounded by LUB_MAX_LSTRLEN, LUB_MAX_USTRLEN,
 *           or LUB_MAX_BSTRLEN based on the type of the source string
 *           and operation. If sn is omitted in a function declaration,
 *           the source is bounded by LUB_MAX_UQNAMELEN.
 *           Note sn is required if the source is a byte string (*byte_t)
 *.          and specifies the length of th source.
 * 
 * @param trunc Pointer to a string that specifies how to handle truncation
 *              when the result would otherwise exceed the
 *              size (tn) of the target buffer.
 * 
 *              The string has the form:
 * 
 *                [<truncate mode>][<truncated replacement string>]
 * 
 *              where <truncate mode> is an optional alphabetic character.
 * 
 *              - If trunc is NULL, truncate mode defaults to "B" and truncated
 *                replacement string defaults to a 0-length string.
 *              - If the first character is alphabetic, it specifies
 *                the truncate mode and is followed by the truncated replacement string.
 *              - If the first character is not alphbetic, truncate
 *                mode defaults to 'B' and trunc is the truncated replacement
 *                string.
 * 
 *             Truncate Mode (explicit or by default):
 * 
 *             - If 'L' or 'l', truncate on
 *               the left with the truncated replacement string added on the left.
 *             - If 'R' or 'r', truncate on the right with the truncated
 *               replacement string added on the right.
 *             - If 'C' or 'c', truncate in
 *               the center with the truncated replacement string added in the center.
 *             - If 'B' or 'b', the result is the truncated replacement string.
 * 
 *             Truncated replacement string:
 * 
 *             - May be a 0-length string.
 *.            - If this string is longer than the target buffer size (tn),
 *.              a 0-length string is used instaead.
 *
 *.            If a result is truncated, error LUB_TRUNCATED is
 *.            returned by the funcion.
 * 
 * @param q Quote character (' or ") to enclose result. Each q character in the
 *          source is doubled in the target buffer).
 * 
 * @param delim Defines a delimiter character to delimit substrings in a 
 *              map string for replacment, s2 string for search functions,
 *              or substrings in a trimmed result.
 * 
 *              For example, if delim is a comma, the string is treated as
 *              a comma separated list of substrings (e.g., "sub1,sub2,sub3").
 *              A substring may be empty (e.g., the second substring is empty
 *              in "sub1,,sub3", the first substring is empty in ",sub2,sub3"
 *              and the last substring is empty in "sub1,sub2,").
 * 
 *              If delim is 0 for a map or s2, no delimiter is used
 *              and the string is treated as a set of characters.
 * 
 *              If delim is 0 for trim, embedded sequences of trim
 *              characters are not replaced with a delimiter.
 * 
 * @param m mth occurrence for matching in replacement and search functions.
 *          m > 0 counts from the beginning of the string (1 means first occurrence).
 *          m == 0 returns NULL for search functions.
 *          m == 0 means replace all occurrences for replace functions.
 *          m < 0 counts from the end of the string (-1 means last
 *                occurrence) when the operation supports reverse search.
 * 
 * @param err_c For Unicode target with Latin source functions only, replacement
 *              character to use when neither the Unicode source 
 *              character nor its uppercased or lowercased value
 *              is a valid Latin character.
 */

/**
 * @defgroup CharacterClassification Character Classification
 * @name isualpha, islalpha
 *       isudigit, isldigit, isualnum, islalnum
 *       isuupper, islupper, isulower, isllower
 *       isucntrl, islcntrl, isuprint, islprint
 *       isugraph, islgraph, isupunct, islpunct
 *       isublank, islblank, isuspace, islspace
 *       isuhexdigit, islhexdigit
 *       isuname1c, isunamec
 * @brief Latin and Unicode character classification functions.
 * @param c Character to classify.
 * @return Non-zero if the character satisfies the condition, otherwise zero.
 * @note islblank and isublank classify whether c is a Latin ' ' character.
 * @note islspace classifies whether c is a Latin whitespace character,
 *       i.e., ' ', '\t', '\v', '\f', '\n', or '\r' character.
 * @note isuspace classifies whether c is a Unicode whitespace character,
 *       which includes Latin whitespace characters plus additional Unicode
 *       whitespace characters (e.g., U+00A0 NO-BREAK SPACE,
 *       U+2003 EM SPACE, etc.).
 * @note isuhexdigit/islhexdigit classifies whether c is a
 *       hexadecimal digit character, i.e., ('0' to '9',
 *       'A' to 'F', or 'a' to 'f').
 * @note isuname1c classifies whether c is valid for the first character
 *       of a Unicode unquoted name, i.e., a Latin alphabetic
 *       or '_' character.
 * @note isunamec classifies whether c is valid in a Unicode unquoted name
 *       other than for the first character in an unquoted name, i.e.,
 *       a Latin alphabetic, numeric, or '_' character.
 * @note islname1c and islnamec are not provided since names are Unicode.
 * @{
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
    {return (iswalnum((wchar_t)c);}
static inline int islalnum(const lchar_t c)
    {return isalnum((unsigned char)c);}
static inline int isuname1c(const uchar_t c)
    {return (size_t)c <= LUB_MAX_LCHAR &&
       (islalpha((lchar_t)c) || (lchar_t)c == '_');
    }
static inline int isunamec(const uchar_t c)
    {return (size_t)c <= LUB_MAX_LCHAR &&
       (islalnum((lchar_t)c) || (lchar_t)c == '_');
    }
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
    {return iswspace((wchar_t)c);}
static inline int islspace(const lchar_t c)
    {return isspace((unsigned char)c);}
static inline int islhexdigit(const lchar_t c)
    {return c >= '0' && c <= '9' ||
            c >= 'a' && c <= 'f' ||
            c >= 'A' && c <= 'F' ? 1 : 0;
    }
static inline int isuhexdigit(const uchar_t c)
    {return c > LUB_MAX_LCHAR ? 0 : islhexdigit((uchar_t)c);}
 /** @} */

 /**
 * @defgroup Preserve/UpperLowerCaseConversion Preserve/Upper/Lower Case Conversion
 * @name lltocase, lutocase, ultocase, uutocase
 *       lltoupper, lutoupper, ultoupper, uutoupper
 *       lltolower, lutolower, ultolower, uutolower
 * @brief Character preserve/uppercase/lowercase conversion.
 * @param c Character to convert.
 * @param err_c For lutocase/lutoupper/lutolower only.
 * @return Preserve/upper/lower case version of c if one defined,
 *         otherwise c.
 * @note For lutocase/lutoupper/lutolower, if converted c is not a valid
 *       Latin chararacter and c is a valid Latin characer,
 *       return c, otherwise return err_c.
 * @{
 */

// To case-preserve conversion.

static inline lchar_t lltocase(const lchar_t c)
    {return c;}
static inline lchar_t lutocase(const uchar_t c, lchar_t err_c)
    {return (size_t)c > LUB_MAX_LCHAR ? err_c : (lchar_t)c;}
static inline uchar_t ultocase(const lchar_t c)
    {return (uchar_t)c;}
static inline uchar_t uutocase(const uchar_t c)
    {return c;}

// To upper case conversion.

static inline lchar_t lltoupper(const lchar_t c)
    {return (lchar_t)toupper((int)c);}
static inline lchar_t lutoupper(const uchar_t c, lchar_t err_c)
    {wint_t wc = towupper((wint_t)c);
     return (wc < 0 || (size_t)wc > LUB_MAX_LCHAR) ?
            (c > LUB_MAX_LCHAR ? (lchar_t)c : err_c) : (lchar_t)wc;
    }
static inline uchar_t ultoupper(const lchar_t c)
    {wint_t wc = towupper((wint_t)c);
     return (wc < 0 || (size_t)wc > LUB_MAX_UCHAR) ?
            (uchar_t)c : (uchar_t)wc;
    }
static inline uchar_t uutoupper(const uchar_t c)
    {wint_t wc = towupper((wint_t)c);
     return (wc < 0 || (size_t)wc > LUB_MAX_UCHAR) ?
            (uchar_t)c : (uchar_t)wc;
    }

// To lower case conversion.

static inline lchar_t lltolower(const lchar_t c)
    {return (lchar_t)tolower((int)c);}
static inline lchar_t lutolower(const uchar_t c, lchar_t err_c)
    {wint_t wc = towlower((wint_t)c);
     return (wc < 0 || (size_t)wc > LUB_MAX_LCHAR) ?
            (c > LUB_MAX_LCHAR ? (lchar_t)c : err_c) : (lchar_t)wc;
    }
static inline uchar_t ultolower(const lchar_t c)
    {return (uchar_t)tolower((int)c);}
static inline uchar_t uutolower(const uchar_t c)
    {wint_t wc = towlower((wint_t)c);
     return (wc < 0 || (size_t)wc > LUB_MAX_UCHAR) ?
            (uchar_t)c : (uchar_t)wc;
    }
 /** @} */

/**
 * @defgroup HexDigitToIntConversion Hex Digit to Int Conversion
 * @name ilhexdigit and iuhexdigit
 * @brief Hex digit character to integer conversion.
 * @param  c Character to convert.
 * @return Value (0-15) of the hex digit if c is a valid hex
 *         digit ('0'-'9', 'a'-'f', 'A'-'F'); otherwise, -1.
 * @{
 */

static inline int iuhexdigit(const uchar_t c)
{   if (c >= '0' && c <= '9') return c - '0';
    else if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    else if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return (int)-1;
}
static inline int ilhexdigit(const lchar_t c)
    {return iuhexdigit((uchar_t)c);}
/** @} */

/**
 * @defgroup StringLength String Length
 * @name lcsnlen, ucsnlen
 * @brief Return the length of a source string (Latin or Unicode).
 * @param s Pointer to the source string.
 * @param sn The maximum length of the string (clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN).
 * @return String length, 0 if s is NULL, or error value.
 * @note Errors:
 *       - (size_t)LUB_UNTERMINATED if s is not null-terminated.
 *       - (size_t)LUB_BAD_PTR if s is an invalid pointer.
 * @{
 */

extern size_t lcsnlen(const lchar_t *s, size_t sn)
#if defined(LUB_DEFINITIONS)
{   if (!s) return (int)0;
    if (LUB_PTR_ERR(s, 0)) return LUB_SIZE_ERR(LUB_BAD_PTR, 0);
    if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;
    size_t k = 0;
    for (; sn && *s; s++, sn--) k++;
    // Return error if null terminator not found at or before s[sn].
    return *s ? LUB_SIZE_ERR(LUB_UNTERMINATED, 0) : k;
}
#else
    ;
#endif

extern size_t ucsnlen(const uchar_t *s, size_t sn)
#if defined(LUB_DEFINITIONS)
{   if (!s) return (int)0;
    if (LUB_PTR_ERR(s, 0)) return LUB_SIZE_ERR(LUB_BAD_PTR, 0);
    if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;
    size_t k = 0;
    for (; sn && *s; s++, sn--) k++;
    // Return error if null terminator not found at or before s[sn].
    return *s ? LUB_SIZE_ERR(LUB_UNTERMINATED, 0) : k;
}
#else
    ;
#endif

/**
 * @defgroup StringClassification String Classification
 * @name isuRESERVED, isuQNAME (case-insensitive)
 *       isltruncstr, isneedlestr
 *       islnhexdigits, isunhexdigits
 * @brief Latin and Unicode string classification.
 * @param s Source string.
 * @param sn Maximum source string length (clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN).
 *           For isuRESERVED and isuQNAME, sn is omitted and sn defaults to LUB_MAX_UNAMELEN.
 *           For isltruncstr, sn is omitted and sn defaults to LUB_MAX_LOPTLEN.
 * @return 1 Condition satisfied.
 *         0 Condition unsatisfied and no error.
 *        <0 Error.
 * @note Errors:
 *       - (int)LUB_BAD_PTR if s is an invalid pointer.
 *       - (int)LUB_UNTERMINATED if s is not null-terminated.
 *       - (int)LUB_INVALID_NAME if s is not a valid name for isuQNAME.
 * @note RESERVED: Classify whether s is a Teradata reserved word.
 * @note QNAME: Classify whether s must be a quoted name.
 *              s must be quoted if the first character is not
 *              a first-name character (see iuname1c), or any subsequent character
 *              is not a name character (see iunamec).
 * @note isltruncstr: Classify whether s is a valid truncated string for use as value
 *                    for as an trunc parameter. A valid string is NULL,
 *                    null-terminated by the bound LUB_MAX_LOPTLEN,
 *                    an empty string, and the first character is 
 *                    not a reserved alphabetic character (see trunc parameter for details).
* @note islneedlestr: Classify whether s is a valid needle string for use as value
 *                    for as an search parameter. A valid string is NULL,
 *                    null-terminated by the bound LUB_MAX_LOPTLEN,
 *                    an empty string, and the first character is 
 *                    not a reserved alphabetic character (see trunc parameter for details).
 * @note The two hexdigits functions classify whether s consists only of hex digit characters
 *       '0' to '9', 'A' to 'F', or 'a' to 'f', s is NULL, or s is empty.
 * @{
 */

extern int isuRESERVED(const uchar_t *s)
#if defined(LUB_DEFINITIONS)
{   if (!s || !*s) return (int)0;
    if (LUB_PTR_ERR(s, 0)) return (int)0;

    // Alphabetically ordered list of uppercased Teradata 
    // reserved words as of Teradata V16.20.
    static const char *td_reserved_words[] = {
        "ABORT", "ABORTSESSION", "ABS", "ACCESS_LOCK", "ACCOUNT", "ACOS",
        "ACOSH", "ADD", "ADD_MONTHS", "ADMIN", "AFTER", "AGGREGATE", "ALIAS",
        "ALL", "ALTER", "AMP", "AND", "ANSIDATE", "ANY", "ARGLPAREN", "AS",
        "ASC", "ASIN", "ASINH", "AT", "ATAN", "ATAN2", "ATANH", "ATOMIC",
        "AUTHORIZATION", "AVE", "AVERAGE", "AVG", "BEFORE", "BEGIN", "BETWEEN",
        "BIGINT", "BINARY", "BLOB", "BOTH", "BT", "BUT", "BY", "BYTE",
        "BYTEINT", "BYTES", "CALL", "CASE", "CASESPECIFIC", "CASE_N", "CAST",
        "CD", "CHAR", "CHAR2HEXINT", "CHARACTER", "CHARACTERS",
        "CHARACTER_LENGTH", "CHARS", "CHAR_LENGTH", "CHECK", "CHECKPOINT",
        "CLASS", "CLOB", "CLOSE", "CLUSTER", "CM", "COALESCE", "COLLATE",
        "COLLECT", "COLUMN", "COMMENT", "COMMIT", "COMPRESS", "CONDITION",
        "CONNECT", "CONSTRAINT", "CONSTRAINTS", "CONSTRUCTOR", "CONTAINS",
        "CONTINUE", "CONVERT_TABLE_HEADER", "CORR", "COS", "COSH", "COUNT",
        "COVAR_POP", "COVAR_SAMP", "CREATE", "CROSS", "CS", "CSUM", "CT",
        "CURRENT", "CURRENT_DATE", "CURRENT_TIME", "CURRENT_TIMESTAMP",
        "CURRENT_USER", "CURSOR", "CV", "CYCLE", "DATA", "DATE", "DAY", "DAYS",
        "DEC", "DECIMAL", "DECLARE", "DEFAULT", "DELETE", "DENSE_RANK", "DEREF",
        "DESCRIBE", "DESCRIPTOR", "DETERMINISTIC", "DIAGNOSTIC", "DISABLED",
        "DISTINCT", "DO", "DOUBLE", "DROP", "DYNAMIC", "EACH", "ELEMENT",
        "ELSE", "ELSEIF", "EQUALS", "ERROR", "ESCAPE", "EVERY", "EXCEPT",
        "EXEC", "EXECUTE", "EXISTS", "EXIT", "EXP", "EXPLAIN", "EXTERNAL",
        "EXTRACT", "FALLBACK", "FALSE", "FASTEXPORT", "FETCH", "FILTER",
        "FIRST", "FIRST_VALUE", "FLOAT", "FLOOR", "FOR", "FOREIGN", "FORMAT",
        "FROM", "FULL", "FUNCTION", "FUSION", "GE", "GET", "GIVE", "GLOBAL",
        "GO", "GOTO", "GRANT", "GRAPHIC", "GROUP", "GROUPING", "GT", "HANDLER",
        "HASH", "HAVING", "HELP", "HOUR", "HOURS", "IDENTITY", "IF", "IN",
        "INDEX", "INDICATOR", "INDICATOR_TYPE", "INNER", "INOUT", "INPUT",
        "INS", "INSENSITIVE", "INSERT", "INT", "INTEGER", "INTEGERDATE",
        "INTERSECT", "INTERVAL", "INTO", "IS", "ITERATE", "JOIN", "JSON",
        "JSON_", "JSON_TABLE", "KURTOSIS", "LANGUAGE", "LARGE", "LATERAL", "LE",
        "LEAD", "LEADING", "LEAVE", "LEFT", "LIKE", "LIKE_ANY", "LIKE_REGEX",
        "LIMIT", "LN", "LOCAL", "LOCALTIME", "LOCALTIMESTAMP", "LOG", "LOG10",
        "LOGGING", "LOGON", "LOWER", "LT", "MACRO", "MAP", "MATCH",
        "MATCH_RECOGNIZE", "MAX", "MEMBER", "MERGE", "METHOD", "MIN", "MINDEX",
        "MINUS", "MINUTE", "MINUTES", "MLINREG", "MLOAD", "MOD", "MODIFIES",
        "MONITOR", "MONTH", "MONTHS", "MSUBSTR", "MULTISET", "NAMED", "NATURAL",
        "NCHAR", "NEW", "NEW_TABLE", "NEXT", "NO", "NONE", "NORMALIZE",
        "NOT", "NOWAIT", "NO_BEFORE_JOURNAL", "NO_INLINE", "NO_PRIMARY_INDEX",
        "NO_SCROLL", "NO_UNDO", "NTH_VALUE", "NTILE", "NULL", "NULLIF",
        "NULLIFZERO", "NUMERIC", "OBJECT", "OCTET_LENGTH", "OF", "OFF",
        "OLD_TABLE", "ON", "ONE", "ONLY", "OPEN", "OR", "ORDER", "ORDINALITY",
        "OUT", "OUTER", "OVER", "OVERLAPS", "OVERLAY", "OVERRIDE", "PARAMETER",
        "PARTITION", "PARTITION#L1", "PARTITION#L2", "PARTITION#L3",
        "PARTITION#L4", "PASSWORD", "PERCENT", "PERCENTILE_CONT",
        "PERCENTILE_DISC", "PERCENT_RANK", "PERIOD", "PERM", "PLACING",
        "PORTION", "POSITION", "POSITION_REGEX", "POWER", "PRECISION",
        "PREPARE", "PRIMARY", "PRIVATE", "PROCEDURE", "PROFILE", "PROTECTED",
        "QUALIFIED", "QUALIFY", "QUANTILE", "RANGE", "RANK", "READS", "REAL",
        "RECURSIVE", "REF", "REFERENCES", "REFERENCING", "REGR_AVGX",
        "REGR_AVGY", "REGR_COUNT", "REGR_INTERCEPT", "REGR_R2", "REGR_SLOPE",
        "REGR_SXX", "REGR_SXY", "REGR_SYY", "RELEASE", "RENAME", "REPEAT",
        "REPLACE", "REPLICATION", "REQUEST", "RESIGNAL", "RESTART", "RESTORE",
        "RESULT", "RETURN", "RETURNS", "REVOKE", "RIGHT", "RIGHTS", "ROLE",
        "ROLLBACK", "ROLLFORWARD", "ROLLUP", "ROW", "ROWS", "ROW_NUMBER",
        "SAMPLE", "SAMPLEID", "SCROLL", "SEARCH", "SECOND", "SECONDS", "SEL",
        "SELECT", "SENSITIVE", "SET", "SETS", "SHOW", "SIGNAL", "SIMILAR",
        "SIN", "SINH", "SKEW", "SMALLINT", "SOME", "SPECIFIC", "SPECIFICTYPE",
        "SQL", "SQLEXCEPTION", "SQLSTATE", "SQLTEXT", "SQLWARNING", "SQRT",
        "SS", "START", "STATS", "STDDEV_POP", "STDDEV_SAMP", "STEPINFO",
        "STORED", "SUBMULTISET", "SUBSCRIBER", "SUBSET", "SUBSTRING",
        "SUBSTRING_REGEX", "SUCCEEDS", "SUM", "SUMMARY", "SUSPEND", "SYSTEM",
        "SYSTEM_TIME", "SYSTEM_USER", "TABLE", "TABLESAMPLE", "THEN", "TIME",
        "TIMESTAMP", "TIMEZONE_HOUR", "TIMEZONE_MINUTE", "TO", "TRAILING",
        "TRANSLATE", "TRANSLATE_CHK", "TRANSLATE_REGEX", "TRANSLATION", "TREAT",
        "TRIGGER", "TRIM", "TRIM_ARRAY", "TRUE", "UESCAPE", "UNBOUNDED",
        "UNICODE", "UNION", "UNIQUE", "UNKNOWN", "UNNEST", "UNTIL_CHANGED",
        "UNTIL_CLOSED", "UPD", "UPDATE", "UPPER", "UPPERCASE", "USER", "USING",
        "VALIDTIME", "VALUE", "VALUES", "VARBYTE",
        "VARCHAR", "VARGRAPHIC", "VARIANT_TYPE", "VARYING", "VAR_POP",
        "VAR_SAMP", "VIEW", "VOLATILE",
        "WAIT", "WHEN", "WHERE", "WHILE", "WIDTH_BUCKET", "WITH", "WITHOUT",
        "WORK", "XMLPLAN", "YEAR", "ZEROIFNULL", "ZONE"
    };

    const size_t td_reserved_words_count =
        sizeof(td_reserved_words) / sizeof(td_reserved_words[0]);
    const size_t td_reserved_word_min_len = (size_t)2;
    const size_t td_reserved_word_max_len = (size_t)20;

    size_t len = ucsnlen(s, td_reserved_word_max_len);
    if (LUB_SIZE_ERR(len, 0) || len < td_reserved_word_min_len)
      return (int)0;

    size_t lo = 0;
    size_t hi = td_reserved_words_count;
    while (lo < hi) {
      const size_t mid = lo + ((hi - lo) / 2);
      const char *kw = td_reserved_words[mid];
      const uchar_t *ss = s;
      int cmp;

      for (; ; kw++, ss++) {
        if (!*kw && !*ss) return (int)1;
        if (*ss > LUB_MAX_LCHAR) return (int)0;
        char c = toupper((unsigned char)*ss);
        if (c < *kw) {cmp = -1; break;}
        if (c > *kw) {cmp = 1; break;}
      }

      if (cmp < 0) hi = mid;
      else lo = mid + 1;
    }

    return (int)0;
}
#else
    ;
#endif

extern int isuQNAME(const uchar_t *s)
#if defined(LUB_DEFINITIONS)
{   if (LUB_PTR_ERR(s, 0)) return LUB_INT_ERR(LUB_BAD_PTR, 0);
    size_t len = ucsnlen(s, LUB_MAX_UNAMELEN);
    if (LUB_SIZE_ERR(len, 0)) return LUB_INT_ERR(len, 0);
    if (!len) return LUB_INT_ERR(LUB_INVALID_NAME, 0);

    // Check if s is all spaces.
    const uchar_t *ss = s;
    size_t llen = len;
    for (; *ss; llen--, ss++) if (*ss != ' ') break;
    if (!llen) return LUB_INT_ERR(LUB_INVALID_NAME, 0);

    if (!isuname1c(*s)) return (int)1;
    for (size_t i = 1; i < len; ++i)
      if (!isunamec(s[i])) return (int)1;

    if (isuRESERVED(s)) return (int)1;

    return (int)0;
}
#else
    ;
#endif

extern int isltruncstr(const lchar_t *s)
#if defined(LUB_DEFINITIONS)
{   if (LUB_PTR_ERR(s, 0))
      return LUB_INT_ERR(LUB_BAD_PTR, 0);
    size_t len = lcsnlen(s, LUB_MAX_LOPTLEN);
    if (LUB_SIZE_ERR(len, 0)) return LUB_INT_ERR(len, 0);
    if (!len) return (int)1;
    int c = toupper((int)*s);
    return strchr("LRC", c) || !isalpha(c) ? (int)1 : (int)0;
}
#else
    ;
#endif

// Hex digit string classification.
extern int islnhexdigits(const lchar_t *s, size_t sn)
#if defined(LUB_DEFINITIONS)
{   if (!s) return (int)1;
    if (LUB_PTR_ERR(s, 0)) return LUB_INT_ERR(LUB_BAD_PTR, 0);
    if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;
    for (; sn && *s; --sn, ++s) if (!islhexdigit(*s)) return (int)0;
    if (*s) return LUB_INT_ERR(LUB_UNTERMINATED, 0);
    return (int)1;
}
#else
    ;
#endif

extern int isunhexdigits(const uchar_t *s, size_t sn)
#if defined(LUB_DEFINITIONS)
{   if (!s) return (int)1;
    if (LUB_PTR_ERR(s, 0)) return LUB_INT_ERR(LUB_BAD_PTR, 0);
    if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;
    for (; sn && *s; --sn, ++s) if (!isuhexdigit(*s)) return (int)0;
    if (*s) return LUB_INT_ERR(LUB_UNTERMINATED, 0);
    return (int)1;
}
#else
    ;
#endif
/** @} */

/**
 * @defgroup Concatenate Concatenate
 * @name llsnncat, lusnncat, ulsnncat, uusnncat (case-preserving)
 *       llsnncatc, lusnncatc, ulsnncatc, uusnncatc (lowercase)
 *       llsnnCATC, lusnnCATC, ulsnnCATC, uusnnCATC (uppercase)
 *       llsnncatq, lusnncatq, ulsnncatq, uusnncatq (quoted string, case-preserving)
 *       llsnncatqc, lusnncatqc, ulsnncatqc, uusnncatqc (quoted string, lowercase)
 *       llsnnCATQC, lusnnCATQC, ulsnnCATQC, uusnnCATQC (quoted string, uppercase)
 *       uusncatname (unquoted/quoted name, case-preserving)
 *       uusncatnamec (unquoted/quoted name, lowercase)
 *       uusnCATNAMEC (unquoted/quoted name, uppercase)
 *       lbsnnCATC, ubsnnCATC (lowercase)
 *       lbsnnCATC, ubsnnCATC (uppercase)
 * @brief Concatenate to target character buffer with options for
 *        case-mapping (preserving, lower, or upper), quoting, and
 *        truncation handling.
 * @note For qname, concatenate source if it is a valid unquoted name (see isuQNAME);
 *       otherwise, if it is valid name, concatenate quoted source. Error if not a valid name.
 * @param t Pointer to the target buffer.
 * @param tn tn is the maximum number of characters for buffer t,
 *           excluding the null terminator; tn is clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * @param s Pointer to source string.
 * @param sn For a character source string, sn is the maximum length of the
 *           string. sn is clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * 
 *           sn is omitted for uusncatname, uusncatnamec, and
 *           uusnCATNAMEC since the source is expected to be Unicode name
 *           and thus sn defaults to LUB_MAX_UNAMELEN (128 characters).
 * 
 *           For a copy of a byte string to a character string, sn 
 *           defines the length of the source string.
 * @param trunc See @ref ParameterName.
 * @param q Quote (' or ") character for quoted string and
 *          quoted name concatenate functions.
 * @param err_c Replacement for out-of-range Unicode characters
 *              (lus functions only).
 * @return t, NULL (if t is null), or an error.
 * 
 *         If t is not null and t is not null-terminated, the target buffer
 *         is set to an empty string (that is, *t = '\0').
 * @note Errors
 *       - LUB_BAD_PTR if t or s is an invalid pointer.
 *       - LUB_UNTERMINATED if t or s is not null-terminated.
 *       - LUB_OPT_TOO_LONG if trunc is too long.
 *       - LUB_OPT_INVALID if trunc is invalid.
 *       - LUB_OPT_RESERVED if trunc is valid except that the first character is a reserved
 *         alphabetic character.
 *       - LUB_OVERLAP if source and target overlap when not allowed.
 *       - LUB_TRUNCATED if truncation occurs.
 * @note For lls and uus, concatenate is overlap-safe (target result is correct
 *       but source may be overridden if there is overlap).
 *       For a concatenate with target and source of different types or quoted,
 *       overlap is not allowed.
 * @{
 */

// Concatenate/copy function and macro helpers.

#if defined(LUB_DEFINITIONS)

static inline void __t_set__
    (char xt, const void *t, const size_t t_len,
     const uchar_t c, const lchar_t err_c
    )
{   if (xt == 'l')
      *((lchar_t *)t + t_len) = (c > LUB_MAX_LCHAR) ?
        (!err_c ? (lchar_t)'?' : err_c) : (lchar_t)c;
    else
      *((uchar_t *)t + t_len) = c;
    return;
}

static void *__concatenate_quoted_helper__
    (const char xt, const char xs,
     const lchar_t q, const char Case,
     void *t, size_t t_len, void *s, size_t s_len,
     const lchar_t *st, const lchar_t err_c
    )
{   // TBD - check for truncation and trunc fit in tn if trunc is not NULL.
    size_t t_inc = (xt == 'l') ? sizeof(lchar_t) : sizeof(uchar_t);
    size_t s_inc = (xs == 'l') ? sizeof(lchar_t) : sizeof(uchar_t);
    if (t_len > 1)
    { __t_set__(xt, t, 0, (uchar_t)q, err_c);
      t += t_inc; t_len -= 2;
      for (; t_len; t_len--, s_len--, t += t_inc, s += s_inc)
      { uchar_t c = (xs == 'l') ? (uchar_t)*(lchar_t *)s : *(uchar_t *)s;
        if (!c) break;
        if (c == (uchar_t)q)
        { if (t_len == 1) break;
          __t_set__(xt, t, 0, (uchar_t)q, err_c);
          t += t_inc;
          t_len--;
        }
        c = (Case == 'C') ? uutoupper(c)
                          : (Case == 'c') ? uutolower(c)
                          : c;
        __t_set__(xt, t, 0, (uchar_t)c, err_c);
      }
      __t_set__(xt, t, 0, (uchar_t)q, err_c);
      t += t_inc;
    }
    __t_set__(xt, t, 0, 0, err_c);
    return  (s_len) ? LUB_PTR_ERR(LUB_TRUNCATED, 0) : t;
}

static void *__target_source_helper__
    (const char xt, const char xs,
     lchar_t q, const char Name, const char Case, 
     void *t, size_t t_len, void *s, size_t s_len,
     const void *trunc,
     const void *trim, const uchar delim,
     const void *pad, const size_t pad_len,
     const lchar_t err_c
    )
{   // Validate Quote, Name, Trim, Pad, Repeat, Replace, and Case.
        if ((q != '\0' && q != '\'' && q != '"') ||
            (q && (trimset || pad || repeat || replace)) ||
                (Name != '\0' && Name != 'N') ||
                (Name == 'N' && q == '\0') ||
                (Case != '\0' && Case != 'c' && Case != 'C'))
            return (void *)LUB_PTR_ERR(LUB_OPT_INVALID, 0);
 
    // Validate trunc and set truncation action and string.
    char trunc_action; const lchar_t *trunc; size_t trunc_len;
    const int st_valid = isltruncstr(st);
    if (LUB_INT_ERR(st_valid, 0)) return LUB_PTR_ERR(st_valid, 0);
    if (!st_valid) return LUB_PTR_ERR(LUB_OPT_RESERVED, 0);
    if (!st) // No truncation action.
    { trunc_action = '\0'; trunc = (lchar_t *)NULL; trunc_len = 0; }
    else // Validate and set truncation action and string.
    { const size_t st_len = lcsnlen(st, LUB_MAX_LOPTLEN);
      if (LUB_SIZE_ERR(st_len, 0)) return LUB_PTR_ERR(st_len, 0);
      if (st_len > tn + 1) return LUB_PTR_ERR(LUB_OPT_TOO_LONG, 0);
      if (isalpha((int)*st)) // Explicit action.
      { trunc_action = (char)toupper((int)*st);
        trunc = trunc + 1;
        trunc_len = st_len - 1;
      }
      else if (st_len > tn)
      { return LUB_PTR_ERR(LUB_OPT_TOO_LONG, 0); }
      else // Default right-truncate action.
      { trunc_action = 'R';
        trunc = st;
        trunc_len = st_len;
      }
    }

    if (!t) return (void *)NULL;

    if 

    if (Name == 'N')
    { if (xt != 'u' || xs != 'u')
        return LUB_PTR_ERR(LUB_OPERATION_INVALID, 0); // Internal error.
      int quoted = isuQNAME((uchar_t *)s);
      if (LUB_INT_ERR(quoted, 0))
        return LUB_PTR_ERR(LUB_INVALID_NAME, 0);
      if (!quoted) q = '\0';
    }

    if (q)
      return __concatenate_quoted_helper__
               (xt, xs, q, Case,
                t, t_len, s, s_len,
                st, err_c);

    // Check for null or empty source string s.
    if (!s_len)
    { // Nothing to concatenate. Null terminate target buffer
      // in case doing a copy (for concatenate, already null-
      // terminated but okay to set it again).
      __t_set__(xt, t, t_len, 0, err_c);
      return t;
    }

    // Check for truncation, accounting for current target length.
    char truncate = (t_len + s_len > tn) ? 1 : 0;
    if (truncate)
    { if (!trunc_action)
      { // NULL action so truncate target buffer to empty string.
        __t_set__(xt, t, 0, 0, err_c);
        return LUB_PTR_ERR(LUB_TRUNCATED, 0);
      }
      if (trunc_action == 'L')
      { // Truncate on the left.
        if (s_len + trunc_len >= tn) {
          // Source plus trunc fill or exceeds target buffer. */
          // Copy right part of source. */
          // Note: do this first to avoid overlap issues. */
          tt = t + trunc_len;
          s += s_len - (tn - trunc_len);
          for (; *s;) *tt++ = *s++;
          *tt = (t_xchar_t)0; /* Null-terminate the result. */
          // Copy trunc to target buffer.
          for (tt = t; *trunc;) *tt++ = *trunc++;
        } 
        // Move existing target chars right, concatenate s,
        // and add trunc on the left.
        size_t t_excess = tn - trunc_len - s_len;
        t_xchar_t *ss = t + t_len - 1;
        tt = ss - t_excess;
        for (; t_excess; t_excess--) *tt-- = *ss--;
        // Copy s to target buffer.
        for (; *s;) *tt-- = *s++;
        *tt = (t_xchar_t)0; // Null-terminate the result.
        // Copy trunc to target buffer.
        for (tt = t; *trunc;) *tt++ = *trunc++;
       } else
       if (trunc_action == 'C')
       { // Truncate in the center.
         size_t
         // Move existing target chars right and add trunc in the center.
         size_t half_trunc = trunc_len / 2;
         size_t half_t = t_len / 2;
         size_t half_s = s_len / 2;
         size_t center_t = half_t + (t_len % 2);
         size_t center_s = half_s + (s_len % 2);
         for (size_t i = half_t; i > 0; --i)
           t[i + half_trunc - 1] = t[i - 1];
         for (size_t i = 0; i < half_trunc; ++i)
           t[i] = trunc[i];
         for (size_t i = center_t, j = center_s; j < s_len; ++i, ++j)
           t[i + half_trunc] = s[j];
         s_len = center_s; // Adjust s_len to reflect truncated source length.
       } else
       if (trunc_action == 'R')
       { // Truncate on the right.
         size_t t_excess = tn - (t_len + trunc_len);
         tt = t + t_len;
         // Copy leading source characters.
         for (; t_excess; t_excess--) *tt++ = *s++;
         // Add trunc on the right after existing target characters.
         for (; trunc_len; trunc_len--) *tt++ = *trunc++;
         *t = (t_xchar_t)0; // Null-terminate the result.
       }
       return (t_xchar_t *)(LUB_PTR_ERR(LUB_TRUNCATED, 0));
     }
     // Overlap-safe: check for overlap and copy direction.
     if (t <= s || t > s + sn)
     { // Safe to concatenate left-to-right, even with overlap.
       tt = t; // Save start of concatenate for null terminator on error.
       for (; sn && *s; sn--, t++, s++) *t = xxtoc;
       // Error  if source string is not null-terminated at or before s[sn].
       if (*s) {*tt = (t_xchar_t)0; return LUB_PTR_ERR(NULL, 0);}
       *t = (t_xchar_t)0; // Null-terminate the result.
     }
     else
     { // Potentially unsafe left-to-right concatenate.
       // Concatenate right-to-left for safe overlap handling.
       sn = s_len;
       sn++; // Include null terminator in concatenate.
       tt = t = t + sn;
       s = s + sn;
       if (t != s) for (; sn; sn--, tt--, s--) *tt = xxtoc;
     }
     return t;

#if defined(__LUB_OP_HELPER__)
#error "lubtype.h: __LUB_OP_HELPER__ macro is unexpectedly " \
       "already defined. #undef before including lubtype.h. " \
       "This macro is undefined after its last use " \
       "by the include. After including, define again as needed."
#endif

#define __LUB_OP_HELPER__(cat, xt, xs, Quote, Name, Case, \
                          t_xchar_t, t_max_xstrlen, t_xscnlen, \
                          s_max_xstrlen, s_xscnlen, Err_c) \
{   if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0) || LUB_PTR_ERR(st, 0)) \
      return (t_xchar_t *)LUB_PTR_ERR(LUB_BAD_PTR, 0); \
    if (tn > t_max_xstrlen) tn = t_max_xstrlen; \
    const size_t t_len = 0; /* For copy. */ \
    if (cat) \
    { t_len = t_xscnlen(t, tn); \
      if (LUB_SIZE_ERR(t_len, 0)) \
      { if (LUB_INT_ERR(t_len, LUB_UNTERMINATED)) \
          *t = (t_xchar_t)0; \
        return (t_xchar_t *)LUB_PTR_ERR(t_len, 0); \
      } \
    } \
    if (sn > s_max_xstrlen) sn = s_max_xstrlen; \
    const size_t s_len = s_xscnlen(s, sn); \
    if (LUB_INT_ERR(s_len, LUB_UNTERMINATED)) \
      return (t_xchar_t *)LUB_PTR_ERR(LUB_UNTERMINATED, 0); \
    return (t_xchar_t *)__target_source_helper__\
           (xt, xs, Quote, Name, Case, \
            (void *)t, t_len, \
            (void *)s, s_len,
            st, Err_c); \
}

#endif // LUB_DEFINITIONS

// Concatenate case-preserving.

extern lchar_t *llsnncat(lchar_t *t, size_t tn,
                         lchar_t *s, size_t sn, const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'l', 'l', (lchar_t)'\0', '\0', '\0',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern lchar_t *lusnncat(lchar_t *t, size_t tn,
                         uchar_t *s, size_t sn, const lchar_t *st,
                         const lchar_t err_c)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'l', 'u', (lchar_t)'\0', '\0', '\0',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, err_c)
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *ulsnncat(uchar_t *t, size_t tn,
                         lchar_t *s, size_t sn,
                         const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'u', 'l', (lchar_t)'\0', '\0', '\0',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *uusnncat(uchar_t *t, size_t tn,
                         uchar_t *s, size_t sn,
                         const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'u', 'u', (lchar_t)'\0', '\0', '\0',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Concatenate lowercase.

extern lchar_t *llsnncatc(lchar_t *t, size_t tn,
                          lchar_t *s, size_t sn,
                          const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'l', 'l', (lchar_t)'\0', '\0', 'c',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern lchar_t *lusnncatc(lchar_t *t, size_t tn,
                          uchar_t *s, size_t sn,
                          const lchar_t *st,
                          const lchar_t err_c)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'u', 'l', (lchar_t)'\0', '\0', 'c',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, err_c)
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *ulsnncatc(uchar_t *t, size_t tn,
                          lchar_t *s, size_t sn,
                          const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'u', 'l', (lchar_t)'\0', '\0', 'c',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *uusnncatc(uchar_t *t, size_t tn,
                          uchar_t *s, size_t sn,
                          const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'u', 'u', (lchar_t)'\0', '\0', 'c',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Concatenate uppercase.

extern lchar_t *llsnnCATC(lchar_t *t, size_t tn,
                          lchar_t *s, size_t sn,
                          const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'l', 'l', (lchar_t)'\0', '\0', 'C',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern lchar_t *lusnnCATC(lchar_t *t, size_t tn,
                          uchar_t *s, size_t sn,
                          const lchar_t *st,
                          const lchar_t err_c)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'l', 'u', (lchar_t)'\0', '\0', 'C',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, err_c)
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *ulsnnCATC(uchar_t *t, size_t tn,
                          lchar_t *s, size_t sn,
                          const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'u', 'l', (lchar_t)'\0', '\0', 'C',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *uusnnCATC(uchar_t *t, size_t tn,
                          const uchar_t *s, size_t sn,
                          const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'u', 'u', (lchar_t)'\0', '\0', 'C',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Concatenate quoted case-preserving.

extern lchar_t *llsnncatq(lchar_t *t, size_t tn,
                          const lchar_t *s, size_t sn,
                          const lchar_t *st, lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'l', 'l', q, '\0', '\0',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern lchar_t *lusnncatq(lchar_t *t, size_t tn,
                          const uchar_t *s, size_t sn,
                          const lchar_t *st, lchar_t q,
                          const lchar_t err_c)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'l', 'u', q, '\0', '\0',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, err_c)
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *ulsnncatq(uchar_t *t, size_t tn,
                          const lchar_t *s, size_t sn,
                          const lchar_t *st, lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'u', 'l', q, '\0', '\0',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *uusnncatq(uchar_t *t, size_t tn,
                          const uchar_t *s, size_t sn,
                          const lchar_t *st, lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'u', 'u', q, '\0', '\0',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Concatenate quoted lowercase.

extern lchar_t *llsnncatqc(lchar_t *t, size_t tn,
                           const lchar_t *s, size_t sn,
                           const lchar_t *st, lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'l', 'l',  q, '\0', '\0',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern lchar_t *lusnncatqc(lchar_t *t, size_t tn,
                           const uchar_t *s, size_t sn,
                           const lchar_t *st, lchar_t q,
                           const lchar_t err_c)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'l', 'u',  q, '\0', '\0',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, err_c)
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *ulsnncatqc(uchar_t *t, size_t tn,
                           const lchar_t *s, size_t sn,
                           const lchar_t *st, lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'u', 'l',  q, '\0', '\0',
                      uchar_t, LUB_MAX_USTRLEN, lcsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *uusnncatqc(uchar_t *t, size_t tn,
                           const uchar_t *s, size_t sn,
                           const lchar_t *st, lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'u', 'u',  q, '\0', '\0',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Concatenate quoted uppercase.

extern lchar_t *llsnnCATQC(lchar_t *t, size_t tn,
                           const lchar_t *s, size_t sn,
                           const lchar_t *st, lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'l', 'l',  q, '\0', 'C',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern lchar_t *lusnnCATQC(lchar_t *t, size_t tn,
                           const uchar_t *s, size_t sn,
                           const lchar_t *st, lchar_t q,
                           const lchar_t err_c)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'l', 'u',  q, '\0', 'C',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, err_c)
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *ulsnnCATQC(uchar_t *t, size_t tn,
                           const lchar_t *s, size_t sn,
                           const lchar_t *st, lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'u', 'l',  q, '\0', 'C',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *uusnnCATQC(uchar_t *t, size_t tn,
                           const uchar_t *s, size_t sn,
                           const lchar_t *st, lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'u', 'u',  q, '\0', 'C',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Concatenate name case-preserving.

extern uchar_t *uusncatname(uchar_t *t, size_t tn,
                            const uchar_t *s,
                            lchar_t q, const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'u', 'u', q, 'N', '\0',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Concatenate name lowercase.

extern uchar_t *uusncatnamec(uchar_t *t, size_t tn,
                             const uchar_t *s,
                             lchar_t q, const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'u', 'u', q, 'N', 'c',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Concatenate name uppercase.

extern uchar_t *uusnCATNAMEC(uchar_t *t, size_t tn,
                             const uchar_t *s,
                             lchar_t q, const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(1, 'u', 'u', q, 'N', 'C',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Concatenate hex digit characters <- bytes function helpers.

#if defined(LUB_DEFINITIONS)
static lchar_t __lbtohexdigit__
  (const byte_t b, int shift, const uint8_t Case)
{   if (shift) shift = 4;
    b = (b >> shift) & 0xF;
    return b < 10 ?
           (lchar_t)(b + '0') :
           (lchar_t)(b - 10 + (Case == 'c' ? 'a' : 'A'));
}

static lchar_t *__lbsnncatc__(lchar_t *t, size_t tn, 
                              const byte_t *s, size_t sn,
                              const lchar_t *st,
                              const uint8_t Case)
{   if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0) || LUB_PTR_ERR(st, 0))
      return (lchar_t *)LUB_PTR_ERR(LUB_BAD_PTR, 0);
    if (!t) return (lchar_t *)NULL;
    if (sn > LUB_MAX_LSTRLEN >> 1) sn = LUB_MAX_LSTRLEN >> 1;
    if (sn > LUB_MAX_BSTRLEN) sn = LUB_MAX_BSTRLEN;
    for (; sn; sn--) {
      // Note: if s is NULL, treat as sn x'00' bytes.
      const byte_t c = s ? *s++ : (byte_t)0;
      *t++ = __lbtohexdigit__(c, 4, Case);
      *t++ = __lbtohexdigit__(c, 0, Case);
    }
    *t = (lchar_t)0;
    return t;
}

static uchar_t *__ubsnncatc__(uchar_t *t, size_t tn,
                              const byte_t *s, size_t sn,
                              const lchar_t *st,
                              const uint8_t Case)
{   if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0) || LUB_PTR_ERR(st, 0))
      return (uchar_t *)LUB_PTR_ERR(LUB_BAD_PTR, 0);
    if (!t) return (uchar_t *)NULL;
    if (sn > LUB_MAX_USTRLEN >> 1) sn = LUB_MAX_USTRLEN >> 1;
    if (sn > LUB_MAX_BSTRLEN) sn = LUB_MAX_BSTRLEN;
    for (; sn; sn--) {
      // Note: if s is NULL, treat as sn x'00' bytes.
      const byte_t c = s ? *s++ : (byte_t)0;
      *t++ = (uchar_t)__lbtohexdigit__(c, 4, Case);
      *t++ = (uchar_t)__lbtohexdigit__(c, 0, Case);
    }
    *t = (uchar_t)0;
    return t;
}
#endif // LUB_DEFINITIONS.

// Concatenate hex digit characters (lowercase) <- bytes.

extern lchar_t *lbsnncatc(lchar_t *t, size_t tn,
                          const byte_t *s, size_t sn,
                          const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    {return __lbsnncatc__(t, tn, s, sn, st, -1);}
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *ubsnncatc(uchar_t *t, size_t tn,
                          const byte_t *s, size_t sn,
                          const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    {return __ubsnncatc__(t, tn, s, sn, st, -1);}
#else
    ;
#endif // LUB_DEFINITIONS

// Concatenate hex digit characters (uppercase) <- bytes.

extern lchar_t *lbsnnCATC(lchar_t *t, size_t tn,
                          const byte_t *s, size_t sn,
                          const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    {return __lbsnncatc__(t, tn, s, sn, st, 1);}
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *ubsnnCATC(uchar_t *t, size_t tn,
                          const byte_t *s, size_t sn,
                          const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    {return __ubsnncatc__(t, tn, s, sn, st, 1);}
#else
    ;
#endif // LUB_DEFINITIONS
/** @} */

/**
 * @defgroup Copy Copy
 * @name llsnncpy, lusnncpy, ulsnncpy, uusnncpy (case-preserving)
 *       llsnncpyc, lusnncpyc, ulsnncpyc, uusnncpyc (lowercase)
 *       llsnnCPYC, lusnnCPYC, ulsnnCPYC, uusnnCPYC (uppercase)
 *       llsnncpyq, lusnncpyq, ulsnncpyq, uusnncpyq (quoted, case-preserving)
 *       llsnncpyqc, lusnncpyqc, ulsnncpyqc, uusnncpyqc (quoted, lowercase)
 *       llsnnCPYQC, lusnnCPYQC, ulsnnCPYQC, uusnnCPYQC (quoted, uppercase)
 *       uusncpyname (unquoted/quoted name, case-preserving)
 *       uusncpynamec (unquoted/quoted lowercase)
 *       uusnCPYNAMEC (unquoted/quoted uppercase)
 *       lbsnncpyc, ubsnncpyc (bytes to hex digit characters, lowercase)
 *       lbsnnCPYC, ubsnnCPYC (bytes to hex digit characters, uppercase)
 *       blsnncpy, busnncpy (hex digit characters to bytes)
 *       bbsnncpy (bytes to bytes)
 * @brief Copy to target character or byte buffer with options for
 *        case-mapping (preserving, lower, or upper), quoting, and
 *        truncation handling.
 * @param t Pointer to target buffer.
 * @param tn For a character target, tn is the maximum number of characters to copy to buffer t,
 *           excluding the null terminator; tn is clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * 
 *           For bbsnncpy, tn is the number of bytes to copy the target
 *           buffer. The target buffer must be at least tn bytes long.
 * @param s Pointer to source string.
 * @param sn For a character source string, sn is the maximum length of the
 *           string. sn is clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * 
 *           For uusncpyname, uusncpynamec, and uusnCPYNAMEC, sn is omitted
 *           since the source must be a valid name and
 *           thus sn defaults to LUB_MAX_UNAMELEN (128 characters).
 * 
 *           For a copy of a byte string, sn is the length of the source
 *           string. Error if sn is greater than LUB_MAX_BSTRLEN.
 * @param q Quote character for quoted string and quoted name copy functions.
 * @param trunc See @ref ParameterName.
 * @param  err_c  Replacement for out-of-range Unicode characters
 *                (lus functions only).
 * @return t, or NULL if an error occurs.
 * 
 *         If t is not null and an error occurs, the target buffer
 *         is set to an empty string for a character target or filled
 *         with sn x'00' bytes for a byte target (truncated if sn > tn
 *         or padded with x'00' bytes to a length of tn).
 * @note Errors:
 *       - t is NULL.
 *       - Unterminated character source at or before s[sn].
 *       - For a byte target, tn exceeds LUB_MAX_BSTRLEN.
 *       - Unexpected overlap when source and target have different types, or when quoted.
 *       - Length of trunc string exceeds 31 or tn if trunc is not NULL.
 * @note Copy is overlap-safe if target and source have the same type and not quoted
 *       (target result is correct but source may be overridden if there is overlap).
 *
 *       For copy with target and source of different types or quoted, overlap is not allowed.
 * @note For concatenate with a byte target and a character source:
 *       - If the source string contains an odd number of hex digits, the last
 *         hex digit is treated as the high nibble of a byte, with an implicit
 *         low nibble of zero.
 *       - If the number of bytes copied into the target buffer is less than tn,
 *         the target buffer is padded with x'00' bytes to a length of tn.
 * @{
 */

// Copy case-preserving.

extern lchar_t *llsnncpy(lchar_t *t, size_t tn, const lchar_t *s, size_t sn,
                                const lchar_t *st)
#if defined(LUB_DEFINITIONS)                              
    __LUB_OP_HELPER__(0, 'l', 'l', (lchar_t)'\0', '\0', '\0',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern lchar_t *lusnncpy(lchar_t *t, size_t tn,  uchar_t *s, size_t sn,
                                const lchar_t *st, const lchar_t err_c)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'l', 'u', (lchar_t)'\0', '\0', '\0',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, err_c)
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *ulsnncpy(uchar_t *t, size_t tn, lchar_t *s, size_t sn,
                                 const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'u', 'l', (lchar_t)'\0', '\0', '\0',
                      lchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *uusnncpy(uchar_t *t, size_t tn,  uchar_t *s, size_t sn,
                                 const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'u', 'u', (lchar_t)'\0', '\0', '\0',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Copy lowercase.

extern lchar_t *llsnncpyc(lchar_t *t, size_t tn,
                          const lchar_t *s, size_t sn,
                          const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'l', 'l', (lchar_t)'\0', '\0', 'c',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern lchar_t *lusnncpyc(lchar_t *t, size_t tn,
                          uchar_t *s, size_t sn,
                          const lchar_t *st, const lchar_t err_c)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'l', 'u', (lchar_t)'\0', '\0', 'c',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, err_c)
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *ulsnncpyc(uchar_t *t, size_t tn,
                          lchar_t *s, size_t sn,
                          const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'u', 'l', (lchar_t)'\0', '\0', 'c',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *uusnncpyc(uchar_t *t, size_t tn,
                          uchar_t *s, size_t sn,
                                 const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'u', 'u', (lchar_t)'\0', '\0', 'c',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Copy uppercase.

extern lchar_t *llsnnCPYC(lchar_t *t, size_t tn,
                                 const lchar_t *s, size_t sn,
                                 const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'l', 'l', (lchar_t)'\0', '\0', 'C',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern lchar_t *lusnnCPYC(lchar_t *t, size_t tn, uchar_t *s, size_t sn,
                                 const lchar_t *st, const lchar_t err_c)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'l', 'u', (lchar_t)'\0', '\0', 'C',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, err_c)
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *ulsnnCPYC(uchar_t *t, size_t tn, uchar_t *s, size_t sn,
                                 const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'u', 'l', (lchar_t)'\0', '\0', 'C',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *uusnnCPYC(uchar_t *t, size_t tn,  uchar_t *s, size_t sn,
                                  const lchar_t *st)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'u', 'u', (lchar_t)'\0', '\0', 'C',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Copy quoted case-preserving.

extern lchar_t *llsnncpyq(lchar_t *t, size_t tn,
                          const lchar_t *s, size_t sn,
                          const lchar_t *st, const lchar_t q)
#if defined(LUB_DEFINITIONS)                              
    __LUB_OP_HELPER__(0, 'l', 'l', q, '\0', '\0',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern lchar_t *lusnncpyq(lchar_t *t, size_t tn,
                          const uchar_t *s, size_t sn,
                          const lchar_t *st, const lchar_t q,
                          const lchar_t err_c)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'l', 'u', q, '\0', '\0',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, err_c)
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *ulsnncpyq(uchar_t *t, size_t tn,
                          const lchar_t *s, size_t sn,
                          const lchar_t *st, const lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'u', 'l', q, '\0', '\0',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *uusnncpyq(uchar_t *t, size_t tn,
                          const uchar_t *s, size_t sn,
                          const lchar_t *st, const lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'u', 'u', q, '\0', '\0',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Copy quoted lowercase.

extern lchar_t *llsnncpyqc(lchar_t *t, size_t tn,
                           const lchar_t *s, size_t sn,
                           const lchar_t *st, const lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'l', 'l', q, '\0', 'c',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern lchar_t *lusnncpyqc(lchar_t *t, size_t tn,
                           const uchar_t *s, size_t sn,
                           const lchar_t *st, const lchar_t q,
                           const lchar_t err_c)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'l', 'u', q, '\0', 'c',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, err_c)
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *ulsnncpyqc(uchar_t *t, size_t tn,
                           const lchar_t *s, size_t sn,
                           const lchar_t *st, const lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'u', 'l', q, '\0', 'c',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *uusnncpyqc(uchar_t *t, size_t tn,
                           const uchar_t *s, size_t sn,
                           const lchar_t *st, const lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'u', 'u', q, '\0', 'c',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Copy quoted uppercase.

extern lchar_t *llsnnCPYQC(lchar_t *t, size_t tn,
                           const lchar_t *s, size_t sn,
                           const lchar_t *st, const lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'l', 'l', q, '\0', 'C',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern lchar_t *lusnnCPYQC(lchar_t *t, size_t tn,
                           const uchar_t *s, size_t sn,
                           const lchar_t *st, const lchar_t q,
                           const lchar_t err_c)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'l', 'u', q, '\0', 'C',
                      lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, err_c)
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *ulsnnCPYQC(uchar_t *t, size_t tn,
                           const lchar_t *s, size_t sn,
                           const lchar_t *st, const lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'u', 'l', q, '\0', 'C',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_LSTRLEN, lcsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *uusnnCPYQC(uchar_t *t, size_t tn,
                           const uchar_t *s, size_t sn,
                           const lchar_t *st, const lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'u', 'u', q, '\0', 'C',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Copy quoted/unquoted name case-preserving.

extern uchar_t *uusncpyname(uchar_t *t, size_t tn,
                            const uchar_t *s, // sn defaults to LUB_MAX_UNAMELEN
                            const lchar_t *st, const lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'u', 'u', q, 'N', '\0',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Copy quoted/unquoted name lowercase.

extern uchar_t *uusncpynamec(uchar_t *t, size_t tn,
                             const uchar_t *s, // sn defaults to LUB_MAX_UNAMELEN
                             const lchar_t *st, const lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'u', 'u', q, 'N', 'c',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

// Copy quoted/unquoted name uppercase.

extern uchar_t *uusnCPYNAMEC(uchar_t *t, size_t tn,
                             const uchar_t *s, // sn defaults to LUB_MAX_UNAMELEN
                             const lchar_t *st, const lchar_t q)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(0, 'u', 'u', q, 'N', 'C',
                      uchar_t, LUB_MAX_USTRLEN, ucsnlen,
                      LUB_MAX_USTRLEN, ucsnlen, (lchar_t)'\0')
#else
    ;
#endif // LUB_DEFINITIONS

#if defined(LUB_DEFINITIONS)
#undef __LUB_OP_HELPER__  // For concatenate/copy functions.
#endif // LUB_DEFINITIONS

// Copy hex digit characters lowercase <- bytes.

static inline lchar_t *lbsnncpyc(lchar_t *t, size_t tn,
                                 const byte_t *s, size_t sn,
                                 const lchar_t *st)
    {if (!t) return (lchar_t *)NULL;
     if (LUB_PTR_ERR(t, 0)) return (lchar_t *)LUB_PTR_ERR(LUB_BAD_PTR, 0);
     *t = (lchar_t)0;
     return lbsnncatc(t, tn, s, sn, st) ? t : (lchar_t *)NULL;}

static inline  uchar_t *ubsnncpyc(uchar_t *t, size_t tn,
                                  const byte_t *s, size_t sn,
                                  const lchar_t *st)
    {if (!t) return (uchar_t *)NULL;
     if (LUB_PTR_ERR(t, 0)) return (uchar_t *)LUB_PTR_ERR(LUB_BAD_PTR, 0);
     *t = (uchar_t)0;
     return ubsnncatc(t, tn, s, sn, st) ? t : (uchar_t *)NULL;}

// Copy hex digit characters uppercase <- bytes.

static inline lchar_t *lbsnnCPYC(lchar_t *t, size_t tn,
                                 const byte_t *s, size_t sn,
                                 const lchar_t *st)
    {if (!t) return (lchar_t *)NULL;
     if (LUB_PTR_ERR(t, 0)) return (lchar_t *)LUB_PTR_ERR(LUB_BAD_PTR, 0);
     *t = (lchar_t)0;
     return lbsnnCATC(t, tn, s, sn, st) ? t : (lchar_t *)NULL;}

static inline  uchar_t *ubsnnCPYC(uchar_t *t, size_t tn,
                                  const byte_t *s, size_t sn,
                                  const lchar_t *st)
    {if (!t) return (uchar_t *)NULL;
     if (LUB_PTR_ERR(t, 0)) return (uchar_t *)LUB_PTR_ERR(LUB_BAD_PTR, 0);
     *t = (uchar_t)0;
     return ubsnnCATC(t, tn, s, sn, st) ? t : (uchar_t *)NULL;}

// Copy bytes <- hex digit characters.

extern byte_t *blsnncpy(byte_t *t, size_t tn, const lchar_t *s, size_t sn)
#if defined(LUB_DEFINITIONS)
{   if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0))
      return (byte_t *)LUB_PTR_ERR(LUB_BAD_PTR, 0); 
    if (!t || !s) return t;
    if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;
    int hi, lo;
    for (; sn && *s; sn--, s++) {
      hi = ilhexdigit(*s);
      if (sn && s[1]) {lo = ilhexdigit(*++s); sn--;} else lo = 0;
      if (hi < 0 || lo < 0) return (*byte_t)0; // Invalid hex digit.
      *t++ = (unsigned char)((hi << 4) | lo);
    }
    if (*s) return (*byte_t)0; // Missing null terminator for source string.
    return t;
}
#else
    ;
#endif // LUB_DEFINITIONS

extern byte_t *busnncpy(byte_t *t, size_t tn, const uchar_t *s)
#if defined(LUB_DEFINITIONS)
{   if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0))
      return (byte_t *)LUB_PTR_ERR(LUB_BAD_PTR, 0); 
    if (!t || !s) return t;
    if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;
    int hi, lo;
    for (; sn && *s; sn--, s++)
    { hi = iuhexdigit(*s);
       lo = 0;
       if (sn && s[1]) {lo = iuhexdigit(*++s); sn--;} else lo = 0;
       if (hi < 0 || lo < 0) return (*byte_t)0; // Invalid hex digit.
       *t++ = (unsigned char)((hi << 4) | lo);
    }
    if (*s) return (*byte_t)0; // Missing null terminator for source string.
    return t;
}
#else
    ;
#endif // LUB_DEFINITIONS

// Copy bytes <- bytes.

extern byte_t *bbsnncpy(byte_t *t, size_t tn, const byte_t *s, size_t sn)
#if defined(LUB_DEFINITIONS)
{   if (!t) return (byte_t *)NULL;
    if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0))
      return (byte_t *)LUB_PTR_ERR(LUB_BAD_PTR, 0);
    if (sn > LUB_MAX_BSTRLEN) return (byte_t *)NULL;
    if (!s) {
      // Copy sn null bytes if s is NULL.
      for (; sn; sn--) *t++ = (byte_t)0;
    } else
    if (t <= s || t >= s + sn) {
      // Safe to copy sn bytes left-to-right, even with overlap.
      for (; sn; sn--) *t++ = *s++;
    } else {
      // Avoid potential overlap with copy sn bytes right-to-left.
      *byte_t tt = (t += sn) - 1;
      s += sn - 1;
      for (; sn; sn--) *tt-- = *s--;
    }
    return t;
}
#else
;
#endif
/** @} */

/**
 * @defgroup Trim Trim
 * @name llsnntrim, ulsnntrim, uusnntrim (case-preserving)
 *       llsnntrimc, ulsnntrimc, uusnntrimc (lowercase)
 *       llsnntrimC, ulsnntrimC, uusnntrimC (uppercase)
 * @brief Trim leading and/or trailing characters that match one of the specified characters
 *        and optionally collapse embedded specified characters.
 * @param t Target buffer for the trimmed string.
 * @param tn Bound for the target buffer (clamped to LUB_MAX_LSTRLEN or
 * @param s Source string to trim.
 * @param sn Bound for the source string (clamped to LUB_MAX_LSTRLEN or
 *           LUB_MAX_USTRLEN).
 * @param trunc SSee @ref ParameterName.
 * @param trim Pointer to a source trim indicator string (optional trim mode
 *             character and trim characters):
 * 
 *             - If NULL, trim defaults to "B".
 *             - If the first character is alphabetic, it specifies
 *               the trim mode and the rest of the characters are the trim characters.
 *             - If the first character is not alphabetic, trim mode defaults
 *               to 'B' and the characters in trim are the trim characters.
 * 
 *             Trim mode (explicit or by default):
 * 
 *             - If 'L' or 'l', trim on the left.
 *             - If 'R' or 'r', trim on the right.
 *             - If 'B' or 'b', trim on both left and right.
 *             - Other alphabetic characters are reserved for future use
 *               and an error occurs if used.
 *
 *             Trim characters (explicit or by default): 
 * 
 *                Characters to trim from the beginning
 *                and/or end of the source string and optionally 
 *                collapse in the string (see delim parameter).
 * 
 *                Matching is case-sensitive (include both cases
 *                as trim characters if desired).
 * 
 *                If no trim characters, defaults to trimming
 *                whitespace characters (as determined by islspace
 *                or isuspace).
 * @param delim Delimiter character for collapsing embedded trim characters.
 *              A null character indicates embedded trim characters
 *              in the source string are not collapsed and trim mode must 
 *              be 'B', 'b', or default to 'B'.
 * @param err_c Replacement for out-of-range Unicode characters (lus functions only).
 * @return Pointer to t, or NULL if an error occurs
 * @note Errors:
 *       - LUB_BAD_PTR if t, s, st, or trim is not a valid pointer (value
 *         is reserved as an error value).
 *       - LUB_UNTERNMAINTED if s, trunc, or trim is not null terminated.
 *       - LUB_OPT_INVALID if invalid trunc or trim value.
 * @note The target buffer must be large enough for the result
 *       characters plus a null-terminator.
 * @{
 */

#if defined(LUB_DEFINITIONS)

#if defined(__LUB_TRIM_LEFT_MODE__) || \
    defined(__LUB_TRIM_RIGHT_MODE__)
#error "lubtype.h: A __LUB_TRIM_*_MODE__ macro is unexpectedly " \
       "already defined. #undef before including lubtype.h. " \
       "These macros are undefined after their last use " \
       "by the include. After including, define again as needed."
#endif

#define __LUB_TRIM_LEFT_MODE__(trim) \
    ((trim) == 'L' || (trim) == 'B' || (trim) == 'l' || (trim) == 'b')
#define __LUB_TRIM_RIGHT_MODE__(trim) \
    ((trim) == 'R' || (trim) == 'B' || (trim) == 'r' || (trim) == 'b')

// Trim helper macro.

#define __LUB_OP_HELPER__(ttype, stype, \
                          max_strlen, snlen_func,
                          space_func, cast) \
    {   if (!t) return (ttype *)NULL; \
        if (LUB_PTR_ERR(t, 0)) return (ttype *)LUB_PTR_ERR(LUB_BAD_PTR, 0); \
        if (!s) {*t = (ttype)0; return t;} \
        if (LUB_PTR_ERR(s, 0)) {*t = (ttype)0; return (ttype *)LUB_PTR_ERR(LUB_BAD_PTR, 0);} \
        if (sn > max_strlen) sn = max_strlen; \
        size_t len = snlen_func(s, sn); \
        if (LUB_SIZE_ERR(len, 0)) {*t = (ttype)0; return (ttype *)LUB_PTR_ERR(len, 0);} \
        size_t start = 0, end = len; \
        if (__LUB_TRIM_LEFT_MODE__(trim)) \
            while (start < end && space_func(s[start])) start++; \
        if (__LUB_TRIM_RIGHT_MODE__(trim)) \
            while (end > start && space_func(s[end - 1])) end--; \
        /* TBD: If trimset is neither NULL nor empty, use it instead of whitespace characters. */ \
        if (delim) { \
            size_t ti = 0; \
            int pending_space = 0; \
            for (size_t i = start; i < end; ++i) { \
                if (space_func(s[i])) { pending_space = 1; } \
                else { if (pending_space) t[ti++] = delim; t[ti++] = cast s[i]; pending_space = 0; } \
            } \
            if (pending_space) t[ti++] = delim; \
            t[ti] = (ttype)0; \
        } else { \
            for (size_t i = start; i < end; ++i) t[i - start] = cast s[i]; \
            t[end - start] = (ttype)0; \
        } \
        return t; \
    }

#endif // LUB_DEFINITIONS for trim mode macros.

// Trim (case-preserving).
extern lchar_t *llsnntrim
    (lchar_t *t, size_t tn,
     const lchar_t *s, size_t sn, const lchar_t st,
     const lchar_t *trimset, const char trim, const lchar_t delim
    )
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(lchar_t, lchar_t,
                      LUB_MAX_LSTRLEN, lcsnlen, islspace, )
#else
;
#endif // LUB_DEFINITIONS for llsntrim.

extern uchar_t *lusnntrim
    (lchar_t *t, size_t tn,
     const uchar_t *s, size_t sn, const lchar_t st,
     const lchar_t *trimset, const char trim, const lchar_t delim,
     const lchar_t err_c
    )
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(lchar_t, uchar_t,
                      LUB_MAX_LSTRLEN, ucsnlen, isuspace, (lchar_t))
#else
;
#endif // LUB_DEFINITIONS for lusnntrim.

extern uchar_t *ulsnntrim
    (uchar_t *t, size_t tn,
     const lchar_t *s, size_t sn, const lchar_t st,
     const lchar_t *trimset, const char trim, const uchar_t delim
    )
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(uchar_t, lchar_t,
                      LUB_MAX_USTRLEN, lcsnlen, islspace, (uchar_t))
#else
;
#endif // LUB_DEFINITIONS for ulsnnbtrim.

extern uchar_t *uusnntrim
    (uchar_t *t, size_t tn, const uchar_t *s, size_t sn,
     const lchar_t st,
     const uchar_t *trimset, const char trim, const uchar_t delim, 
    )
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(uchar_t, uchar_t,
                      LUB_MAX_USTRLEN, ucsnlen, isuspace, )
#else
;
#endif // LUB_DEFINITIONS for uusnntrim.
/** @} */

#if defined(LUB_DEFINITIONS)
#undef __LUB_OP_HELPER__  // For trim functions.
#endif // LUB_DEFINITIONS

/**
 * @defgroup PointerTrim Pointer Trim
 * @name llsnptrim, uussnptrim
 * @brief The trimmed substring of a string according to the trimset and
 *        trim parameters.
 * @param s       Source string.
 * @param sn      Bound for the source string (clamped to LUB_MAX_LSTRLEN or
 * @param trimlen  Pointer to size_t to receive the length of the trimmed portion.
 * @param trim    Trim selector: 'L', 'R', or 'B' (lowercase also accepted).
 *                'L' trims left, 'R' trims right, 'B' trims both.
 * @param trimset If non-NULL, points to a null-terminated string of characters
 *                to trim; if NULL, whitespace is trimmed.
 * @return For left or both trim, pointer into s at the first non-trim character, if any;
 *         otherwise, returns s. For right trim, returns s.
 * 
 *         If the trimmed result is empty (all trim chars), returns pointer to the
 *         null-termintor for s and sets *trimlen to 0.
 * 
 *         If an error occurs, returns NULL with *trimlen set to 0 if trimlen
 *         is non-NULL. 
 * @note Errors:
 *       - trimlen NULL.
 *       - s is NULL.
 *       - s unterminated within the default bound (MAX_LSTRLEN or LUB_MAX_USTRLEN).
 * @note The returned pointer and length can be used to access the trimmed
 *       substring of the string (the subsstring might not be null-terminated).
 * @note lus and uls variants are not supported.
 * @{
 */
extern const lchar_t *llsnptrim(const lchar_t *s, size_t sn,
                        size_t *const trimlen,
                        const char trim, const char *trimset)
#if defined(LUB_DEFINITIONS)
{   if (LUB_PTR_ERR(s, 0) || LUB_PTR_ERR(trimset, 0))
      return LUB_PTR_ERR(LUB_BAD_PTR, 0);
    if (!trimlen) return (lchar_t *)NULL;
        *trimlen = 0;
        if (!s) return (lchar_t *)NULL;
        if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;
        size_t len = lcsnlen(s, sn);
        if (LUB_SIZE_ERR(len, 0)) return LUB_PTR_ERR(len, 0);
        const lchar_t *left = s;
        const lchar_t *right = s + len;
        if (__LUB_TRIM_LEFT_MODE__(trim)) {
            if (trimset) {
                while (left < right && llsnpbrk(left, trimset, 1) == left)
                    ++left;
            } else {
                while (left < right && islspace(*left))
                    ++left;
            }
        }
        if (__LUB_TRIM_RIGHT_MODE__(trim)) {
            if (trimset) {
                while (right > left &&
                    llsnpbrk(right - 1, trimset, 1) == right - 1)
                    --right;
            } else {
                while (right > left && islspace(*(right - 1)))
                    --right;
            }
        }
        *trimlen = (size_t)(right - left);
        return (lchar_t *)left;
}
#else
    ;
#endif // LUB_DEFINITIONS for llsptrim.
extern const uchar_t *uunptrim(const uchar_t *s,  size_t sn,
                               size_t *const trimlen,
                               const char trim, const uchar_t *trimset)
#if defined(LUB_DEFINITIONS)
{
        if (!trimlen) return (uchar_t *)NULL;
        *trimlen = 0;
        if (!s) return (uchar_t *)NULL;
        if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;
        size_t len = ucsnlen(s, sn);
        if (LUB_SIZE_ERR(len, 0)) return (uchar_t *)LUB_PTR_ERR(len, 0);
        const uchar_t *left = s;
        const uchar_t *right = s + len;
        if (__LUB_TRIM_LEFT_MODE__(trim)) {
            if (trimset) {
                while (left < right && uusnpbrk(left, trimset, 1) == left)
                    ++left;
            } else {
                while (left < right && isuspace(*left))
                    ++left;
            }
        }
        if (__LUB_TRIM_RIGHT_MODE__(trim)) {
            if (trimset) {
                while (right > left &&
                    uusnpbrk(right - 1, trimset, 1) == right - 1)
                    --right;
            } else {
                while (right > left && isuspace(*(right - 1)))
                    --right;
            }
        }
        *trimlen = (size_t)(right - left);
        return (uchar_t *)left;
}
#else
    ;
#endif // LUB_DEFINITIONS for uunptrim.
/** @} */

#if defined(LUB_DEFINITIONS)
#undef __LUB_TRIM_LEFT_MODE__
#undef __LUB_TRIM_RIGHT_MODE__
#endif // LUB_DEFINITIONS

/**
 * @defgroup Reverse Reverse
 * @name llsnnreverse, ulsnnreverse, uusnnreverse
 * @brief Reverse of the source string.
 * @param t Target buffer.
 * @param tn Maximum number of characters to write to the target buffer
 *           (excluding null-terminator).
 * @param s Source string.
 * @param sn Bound for the source string (clamped to LUB_MAX_LSTRLEN or
 *           LUB_MAX_USTRLEN) for bounded functions. For default-bounded
 *           functions, sn defaults to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * @return t, or NULL if an error occurs.
 * @note Errors: 
 *       - t is NULL
 *       - Unterminated source string.
 *       - t and s are overlapping.
 * @note The target buffer must be large enough for the result 
 *       characters plus a null-terminator.
 * @note lusrev and ulsrev functions are not provided.
 *       Instead, use lusncpy/luscpy followed by uusnrev/uusrev.
 * @{
 */

#if defined(LUB_DEFINITIONS)
#undef __LUB_OP_HELPER__
#define __LUB_OP_HELPER__(t_t, s_t, xxslen, LUB_MAX_xstrlen) \
{   if (!t) return (t_t)NULL; \
    if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0)) \
      return LUB_PTR_ERR(LUB_BAD_PTR, 0); \
    if (sn > LUB_MAX_xstrlen) sn = LUB_MAX_xstrlen; \
    size_t len = xxslen(s, sn); \
    if (len >= LUB_SIZE_ERRORS) return (t_t)len; \
    if (len == 0) { *t = (t_t)0; return t; } \
    if (t == s) { \
        /* In-place reversal */ \
        size_t i = 0, j = len - 1; \
        while (i < j) { \
            t_t tmp = t[i]; \
            t[i] = t[j]; \
            t[j] = tmp; \
            ++i; --j; \
        } \
        t[len] = (t_t)0; \
        return t; \
    } \
    /* Out-of-place reversal with overlap-safe logic */ \
    t_t *tdst = t; \
    const s_t *ssrc = s; \
    if ((tdst > (t_t*)ssrc) && (tdst < (t_t*)ssrc + len)) { \
        /* Overlap: copy from end to avoid overwrite */ \
        size_t i; \
        for (i = 0; i < len; ++i) \
            tdst[len - 1 - i] = ssrc[i]; \
        tdst[len] = (t_t)0; \
        return t; \
    } \
    /* No overlap or safe direction: normal reverse copy */ \
    t = t + len; \
    *t = (t_t)0; \
    for (; len; len--)  *--t = *s++; \
    return t; \
}
#endif // LUB_DEFINITIONS for reverse macro helper.

// Reverse.
extern lchar_t *llsnnreverse(lchar_t *t, const lchar_t *s, size_t sn)
#if defined(LUB_DEFINITIONS)
    {__LUB_OP_HELPER__(lchar_t *, const lchar_t *, lcsnlen, LUB_MAX_LSTRLEN)}
#else
    ;
#endif // LUB_DEFINITIONS for llsnnreverse.

extern uchar_t *ulsnnreverse(uchar_t *t, const lchar_t *s, size_t sn)
#if defined(LUB_DEFINITIONS)
    {__LUB_OP_HELPER__(uchar_t *, const lchar_t *, lcsnlen, LUB_MAX_LSTRLEN)}
#else
    ;
#endif // LUB_DEFINITIONS for ulsnnreverse.

extern uchar_t *uusnnreverse(uchar_t *t, const uchar_t *s, size_t sn)
#if defined(LUB_DEFINITIONS)
    {__LUB_OP_HELPER__(uchar_t *, constuchar_t *, ucsnlen, LUB_MAX_USTRLEN)}
#else
    ;
#undef __LUB_OP_HELPER__
#endif // LUB_DEFINITIONS for uusnnreverse.
/** @} */

/**
 * @defgroup Pad Pad
 * @name llsnnpad, ulsnnpad, uusnnpad
 * @brief Pad source string to exactly tn characters using left, center, or
 *        right alignment (bounded).
 * @param t Target buffer.
 * @param tn Number of result characters written to t (excluding terminator).
 * @param s Source string.
 * @param n Bound on source string (clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN)
 *          for bounded functions. For default-bounded functions,
 *          n defaults to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * @param pad 'L' or 'l' pad on left (for right-aligned text).
 *            'R' or 'r' pad on right (for left-aligned text).
 *            'B' or 'b' pad on left and right (for center-aligned text).
 *            Any other character is an error.
 * @param pad_c Pad character.
 * @param err_c For lusnnpad and lusnnpad, character used when a
 *              Unicode character cannot be converted to a Latin character
 *              (i.e., Unicode character > LUB_MAX_LCHAR).
 * @return t, NULL (if t is NULL), error. For an error,
 *         *t is set to a null-terminator.
 * @note Errors:
 *       - Invalid arguments
 *       - Unterminated source.
 *       - Source longer than tn.
 * @{
 */
// Pad.
extern lchar_t *llsnnpad(lchar_t *t, size_t tn, const lchar_t *s, size_t sn,
                             char pad, lchar_t pad_c)
#if defined(LUB_DEFINITIONS)
{   if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0))
      return LUB_PTR_ERR(LUB_BAD_PTR, 0);
    if (!t || !s) return (lchar_t *)NULL;
    if (tn > LUB_MAX_LSTRLEN) tn = LUB_MAX_LSTRLEN;
    if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;

    size_t len = lcsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (lchar_t *)len;

    if (len > tn) {
        t[tn] = (lchar_t)0;
        return (lchar_t *)NULL;
    }

    size_t left = 0, right = 0;
    if (len < tn) {
        size_t total = tn - len;
        if (pad == 'L' || pad == 'l') left = total;
        else if (pad == 'R' || pad == 'r') right = total;
        else {
            left = total / 2;
            right = total - left;
        }
    }

    size_t ti = 0;
    for (size_t i = 0; i < left; ++i) t[ti++] = pad_c;
    for (size_t i = 0; i < len; ++i) t[ti++] = s[i];
    for (size_t i = 0; i < right; ++i) t[ti++] = pad_c;
    t[ti] = (lchar_t)0;
    return t;
}
#else
;
#endif // LUB_DEFINITIONS for llsnnpad.

extern uchar_t *ulsnnpad(uchar_t *t, size_t tn, const lchar_t *s, size_t sn,
                             char pad, uchar_t pad_c)
#if defined(LUB_DEFINITIONS)
{   if (!t || !s) return (uchar_t *)NULL;
    if (tn > LUB_MAX_USTRLEN) tn = LUB_MAX_USTRLEN;
    if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;

    size_t len = lcsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (uchar_t *)len;

    if (len > tn) {
        t[tn] = (uchar_t)0;
        return (uchar_t *)NULL;
    }

    size_t left = 0, right = 0;
    if (len < tn) {
        size_t total = tn - len;
        if (pad == 'L' || pad == 'l') left = total;
        else if (pad == 'R' || pad == 'r') right = total;
        else {
            left = total / 2;
            right = total - left;
        }
    }

    size_t ti = 0;
    for (size_t i = 0; i < left; ++i) t[ti++] = pad_c;
    for (size_t i = 0; i < len; ++i) t[ti++] = (uchar_t)s[i];
    for (size_t i = 0; i < right; ++i) t[ti++] = pad_c;
    t[ti] = (uchar_t)0;
    return t;
}
#else
;
#endif // LUB_DEFINITIONS for ulsnnpad.

extern uchar_t *uusnnpad(uchar_t *t, size_t tn, const uchar_t *s, size_t sn,
                         char pad, uchar_t pad_c)
#if defined(LUB_DEFINITIONS)
{   if (!t || !s) return (uchar_t *)NULL;
    if (tn > LUB_MAX_USTRLEN) tn = LUB_MAX_USTRLEN;
    if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;

    size_t len = ucsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (uchar_t *)len;

    if (len > tn) {
        t[tn] = (uchar_t)0;
        return (uchar_t *)NULL;
    }

    size_t left = 0, right = 0;
    if (len < tn) {
        size_t total = tn - len;
        if (pad == 'L' || pad == 'l') left = total;
        else if (pad == 'R' || pad == 'r') right = total;
        else {
            left = total / 2;
            right = total - left;
        }
    }

    size_t ti = 0;
    for (size_t i = 0; i < left; ++i) t[ti++] = pad_c;
    for (size_t i = 0; i < len; ++i) t[ti++] = s[i];
    for (size_t i = 0; i < right; ++i) t[ti++] = pad_c;
    t[ti] = (uchar_t)0;
    return t;
}
#else
;
#endif // LUB_DEFINITIONS for uusnnpad.
/** @} */

/**
 * @defgroup Repeat Repeat
 * @name llsnnrepeat, ulsnnrepeat, uusnnrepeat
 * @brief Repeat source string into target.
 * @param t Target buffer for result.
 * @param tn Maximum number of result characters written to t
 *           (excluding null-terminator).
 *           Clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * @param s Source string.
 * @param sn Bound on source string. Clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * @param times Number of times to repeat source. 0 yields empty string.
 * @return t, null (if t is NULL), or error. For an error,
 *         *t is set to a null-terminator.
 * @note Errors:
 *       - LUB_BAD_PTR if s is an invalid pointer.
 *       - LUB_UNTERMINATED if s is not null-terminated.
 *       - LUB_OVERLAP if target and source buffers overlap in a way
 *         that would cause incorrect results in the target buffer.
 *       - LUB_TRUNCATED if the result cannot fit in the target buffer.
 * @{
 */

extern lchar_t *llsnnrepeat(
    lchar_t *t, size_t tn, const lchar_t *s, size_t sn, size_t times)
#if defined(LUB_DEFINITIONS)
{   if (!t || !s) return (lchar_t *)NULL;
    if (tn > LUB_MAX_LSTRLEN) tn = LUB_MAX_LSTRLEN;
    if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) {
        t[tn] = (lchar_t)0; return (lchar_t *)s_len;
    }

    if (times == 0) {t[0] = (lchar_t)0;  return t;}

    if (s_len && times > tn / s_len) {
        t[tn] = (lchar_t)0; return (lchar_t *)NULL;
    }

    size_t needed = s_len * times;
    if (needed > tn) {
        t[tn] = (lchar_t)0; return (lchar_t *)NULL;
    }

    for (size_t rep = 0; rep < times; ++rep)
        for (size_t i = 0; i < s_len; ++i)
            t[rep * s_len + i] = s[i];

    t[needed] = (lchar_t)0;
    return t;
}
#else
;
#endif

extern uchar_t *ulsnnrepeat(
     uchar_t *t, size_t tn, const lchar_t *s, size_t sn, size_t times)
#if defined(LUB_DEFINITIONS)
{   if (!t || !s) return (uchar_t *)NULL;
    if (tn > LUB_MAX_USTRLEN) tn = LUB_MAX_USTRLEN;
    if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) {
        t[tn] = (uchar_t)0;
        return (uchar_t *)s_len;
    }

    if (times == 0) {
        t[0] = (uchar_t)0;
        return t;
    }

    if (s_len && times > tn / s_len) {
        t[tn] = (uchar_t)0;
        return (uchar_t *)NULL;
    }

    size_t needed = s_len * times;
    if (needed > tn) {
        t[tn] = (uchar_t)0;
        return (uchar_t *)NULL;
    }

    for (size_t rep = 0; rep < times; ++rep)
        for (size_t i = 0; i < s_len; ++i)
            t[rep * s_len + i] = (uchar_t)s[i];

    t[needed] = (uchar_t)0;
    return t;
}
#else
    ;
#endif // LUB_DEFINITIONS for ulsnnrepeat.

extern uchar_t *uusnnrepeat(
     uchar_t *t, size_t tn, const uchar_t *s, size_t sn, size_t times)
#if defined(LUB_DEFINITIONS)
{   if (!t || !s) return (uchar_t *)NULL;
    if (tn > LUB_MAX_USTRLEN) tn = LUB_MAX_USTRLEN;
    if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;

    size_t s_len = ucsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) {
        t[tn] = (uchar_t)0;
        return (uchar_t *)s_len;
    }

    if (times == 0) {
        t[0] = (uchar_t)0;
        return t;
    }

    if (s_len && times > tn / s_len) {
        t[tn] = (uchar_t)0;
        return (uchar_t *)NULL;
    }

    size_t needed = s_len * times;
    if (needed > tn) {
        t[tn] = (uchar_t)0;
        return (uchar_t *)NULL;
    }

    for (size_t rep = 0; rep < times; ++rep)
        for (size_t i = 0; i < s_len; ++i)
            t[rep * s_len + i] = s[i];

    t[needed] = (uchar_t)0;
    return t;
}
#else
    ;
#endif // LUB_DEFINITIONS for uusnnrepeat.

extern uchar_t *uusnnrepeat(
     uchar_t *t, size_t tn, const uchar_t *s, size_t sn, size_t times)
#if defined(LUB_DEFINITIONS)
{    t[tn] = (lchar_t)0; return (uchar_t *)NULL;

    if (times == 0) {
        t[0] = (lchar_t)0;  return (uchar_t *)t;
    }

    if (s_len && times > tn / s_len) {
        t[tn] = (lchar_t)0; return (uchar_t *)NULL;
    }

    size_t needed = s_len * times;
    if (needed > tn) {
        t[tn] = (lchar_t)0; return (lchar_t *)NULL;
    }

    for (size_t rep = 0; rep < times; ++rep)
        for (size_t i = 0; i < s_len; ++i)
            t[rep * s_len + i] = s[i];

    t[needed] = (lchar_t)0;
    return t;
}
#else
    ;
#endif // LUB_DEFINITIONS for uusnnrepeat.

extern uchar_t *ulsnnrepeat(
     uchar_t *t, size_t tn, const lchar_t *s, size_t sn, size_t times)
#if defined(LUB_DEFINITIONS)
{   if (!t || !s) return (uchar_t *)NULL;
    if (tn > LUB_MAX_USTRLEN) tn = LUB_MAX_USTRLEN;
    if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) {
        t[tn] = (uchar_t)0;
        return NULL;
    }

    if (times == 0) {
        t[0] = (uchar_t)0;
        return t;
    }

    if (s_len && times > tn / s_len) {
        t[tn] = (uchar_t)0;
        return (uchar_t *)NULL;
    }

    size_t needed = s_len * times;
    if (needed > tn) {
        t[tn] = (uchar_t)0;
        return (uchar_t *)NULL;
    }

    for (size_t rep = 0; rep < times; ++rep)
        for (size_t i = 0; i < s_len; ++i)
            t[rep * s_len + i] = (uchar_t)s[i];

    t[needed] = (uchar_t)0;
    return t;
}
#else
    ;
#endif // LUB_DEFINITIONS for ulsnnrepeat.

extern uchar_t *uusnnrepeat(
     uchar_t *t, size_t tn, const uchar_t *s, size_t sn, size_t times)
#if defined(LUB_DEFINITIONS)
{   if (!t || !s) return (uchar_t *)NULL;
    if (tn > LUB_MAX_USTRLEN) tn = LUB_MAX_USTRLEN;
    if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;

    size_t s_len = ucsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) {
        t[tn] = (uchar_t)0;
        return NULL;
    }

    if (times == 0) {
        t[0] = (uchar_t)0;
        return t;
    }

    if (s_len && times > tn / s_len) {
        t[tn] = (uchar_t)0;
        return (uchar_t *)NULL;
    }

    size_t needed = s_len * times;
    if (needed > tn) {
        t[tn] = (uchar_t)0;
        return (uchar_t *)NULL;
    }

    for (size_t rep = 0; rep < times; ++rep)
        for (size_t i = 0; i < s_len; ++i)
            t[rep * s_len + i] = s[i];

    t[needed] = (uchar_t)0;
    return t;
}
#else
    ;
#endif // LUB_DEFINITIONS for uusnnrepeat.
/** @} */

/**
 * @defgroup Replace Replace
 * @name llsnnreplace, ulsnnreplace, uusnnreplace (case-sensitive)
 *       llsnnREPLACE, ulsnnREPLACE, uusnnREPLACE (case-insensitive)
 * @brief Replace using a substring delimited needle/replace map
 *        or a needle/replace character map.
 * @param t Pointer to target buffer.
 * @param tn Maximum number of result characters for t
 *           (excluding null-terminator).
 * @param s Pointer to source string.
 * @param sn Maximum number of characters in source string
 *           (excluding null-terminator).
 * @param trunc See @ref ParameterName.
 * @param map Pointer to string of needles and replacements.
 * 
 *   1. If delim is not a null-character, string is encoded as:
 * 
 *         needle1<delim>rep1<delim>needle2<delim>rep2...<null-terminator>
 *
 *      where needle and replace substrings are not null-terminated
 *      but are separated by the delim character.
 * 
 *      A zero-length needle substring does not match any substring
 *      and is effectively ignored along with its corresponding replace substring.
 * 
 *      A zero-length replace substring is allowed (followed by the delimiter
 *      or the map string's null-terminator) and replaces a substring
 *      in the source that matches the preceding needle by
 *      an empty string.
 * 
 *   2. If delim is a null-character, string is encoded as:
 *
 *         needle1rep1needle2rep2...<null-terminator>
 * 
 *      where needles and replacments are single nonnull characters.
 * 
 * @param delim Mapping delimiter. Null character indicates character mapping
 *              otherwise, delimited string mapping.
 * @param m Replacement selector:
 *            0 replaces all matches;
 *           >0 replaces the mth match from start (1-based);
 *           <0 replaces the mth match from end (-1 is last).
 * @return t, NULL (if t is NULL), or error. For an error,
 *         *t is set to a null-terminator.
 * @note Errors: 
 *       - LUB_BAD_PTR if s or map is an invalid pointer.
 *       - LUB_UNTERMINATED if s or map is not null-terminated within their bounds.
 *       - LUB_OPT_INVALID if malformed map syntax (missing delimiter or empty needle)
 *       - LUB_OVERLAP if target and source buffers overlap in a way
 *         that would cause incorrect results in the target buffer.
 *       - LUB_TRUNCATED if the result cannot fit in the target buffer.
 * @note The first matching needle string/character in map wins.
 *       For a string map, list a longer needle before before any of its
 *       prefixes to match to the longer needle (otherwise, the shorter
 *       prefix would match first and the longer needle is ignored).
 * @note Needle matching is case-sensitive (replace) or case-insensitive (REPLACE);
 *       delimiter matching is case-sensitive;
 *       replacement text is written as-is.
 * @note If result would exceed tn characters, returns NULL and writes
 *       terminator at t[tn].
 * @note If map has more than one pair, m must be 0.
 * @note For multi-pair maps, the first matching needle in map order wins.
 *       List longer needles before shorter ones if overlap matters.
 
 * @{
 */

// Replace case-sensitive.
static inline lchar_t *llsnnreplace
       (lchar_t *t, size_t tn, const lchar_t *s, size_t sn,
        const lchar_t *map, lchar_t delim, ptrdiff_t m)
{   if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0) || LUB_PTR_ERR(map, 0))
      return LUB_PTR_ERR(LUB_BAD_PTR, 0);
    if (!t || !s || !map || !delim) return (lchar_t *)NULL;
    if (tn > LUB_MAX_LSTRLEN) tn = LUB_MAX_LSTRLEN;
    if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) return NULL;
    size_t map_len = lcsnlen(map, LUB_MAX_LOPTLEN);
    if (map_len >= LUB_SIZE_ERRORS) return (lchar_t *)NULL;

    const lchar_t *p = map;
    const lchar_t *map_end = map + map_len;

    const lchar_t *first_needle = p;
    while (p < map_end && *p != delim) p++;
    if (p >= map_end) return (lchar_t *)NULL;
    size_t first_needle_len = (size_t)(p - first_needle);
    if (!first_needle_len) return (lchar_t *)NULL;
    p++;

    const lchar_t *first_repl = p;
    while (p < map_end && *p != delim) p++;
    size_t first_repl_len = (size_t)(p - first_repl);

    int multi_pair = (p < map_end);
    if (multi_pair && m) return (lchar_t *)NULL;

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
            if (s_len > tn) {t[tn] = (lchar_t)0; return (lchar_t *)NULL;}
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
                        return (lchar_t *)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j)
                        t[ti + j] = first_repl[j];
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lchar_t *)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j)
                        t[ti + j] = s[si + j];
                    ti += needle_len;
                }
                si += needle_len;
            } else {
                if (ti >= tn) {
                    t[tn] = (lchar_t)0;
                    return (lchar_t *)NULL;
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
            const lchar_t *needle = p;
            while (p < map_end && *p != delim) p++;
            size_t needle_len = (size_t)(p - needle);
            if (!needle_len) return (lchar_t *)NULL;
            p++;

            const lchar_t *repl = p;
            while (p < map_end && *p != delim) p++;
            size_t repl_len = (size_t)(p - repl);

            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (s[si + j] != needle[j]) match = 0;

            if (match) {
                if (ti + repl_len > tn) {
                    t[tn] = (lchar_t)0;
                    return (lchar_t *)NULL;
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
                return (lchar_t *)NULL;
            }
            t[ti++] = s[si++];
        }
    }

    t[ti] = (lchar_t)0;
    return t;
}

extern uchar_t *ulsnnreplace(
     uchar_t *t, size_t tn, const lchar_t *s, size_t sn,
    const lchar_t *map, lchar_t delim, ptrdiff_t m)
{   if (!t || !s || !map || !delim) return (uchar_t *)NULL;
    if (tn > LUB_MAX_USTRLEN) tn = LUB_MAX_USTRLEN;
    if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;

    size_t s_len = ucsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) return NULL;
    size_t map_len = lcsnlen(map, LUB_MAX_LOPTLEN);
    if (map_len >= LUB_SIZE_ERRORS) return NULL;

    const lchar_t *p = map;
    const lchar_t *map_end = map + map_len;

    const lchar_t *first_needle = p;
    while (p < map_end && *p != delim) p++;
    if (p >= map_end) return (uchar_t *)NULL;
    size_t first_needle_len = (size_t)(p - first_needle);
    if (!first_needle_len) return (uchar_t *)NULL;
    p++;

    const lchar_t *first_repl = p;
    while (p < map_end && *p != delim) p++;
    size_t first_repl_len = (size_t)(p - first_repl);

    int multi_pair = (p < map_end);
    if (multi_pair && m) return (uchar_t *)NULL;

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
            if (s_len > tn) {t[tn] = (uchar_t)0; return (uchar_t *)NULL;}
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
                        return (uchar_t *)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j)
                        t[ti + j] = (uchar_t)first_repl[j];
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (uchar_t)0;
                        return (uchar_t *)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j)
                        t[ti + j] = s[si + j];
                    ti += needle_len;
                }
                si += needle_len;
            } else {
                if (ti >= tn) {
                    t[tn] = (uchar_t)0;
                    return (uchar_t *)NULL;
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
            const lchar_t *needle = p;
            while (p < map_end && *p != delim) p++;
            size_t needle_len = (size_t)(p - needle);
            if (!needle_len) return (lchar_t *)NULL;
            p++;

            const lchar_t *repl = p;
            while (p < map_end && *p != delim) p++;
            size_t repl_len = (size_t)(p - repl);

            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (s[si + j] != needle[j]) match = 0;

            if (match) {
                if (ti + repl_len > tn) {
                    t[tn] = (uchar_t)0;
                    return (uchar_t *)NULL;
                }
                for (size_t j = 0; j < repl_len; ++j)
                    t[ti + j] = (uchar_t)repl[j];
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
                return (uchar_t *)NULL;
            }
            t[ti++] = s[si++];
        }
    }

    t[ti] = (uchar_t)0;
    return t;
}

extern uchar_t *uusnnreplace(
     uchar_t *t, size_t tn, const uchar_t *s, size_t sn, const lchar_t *st,
    const uchar_t *map, uchar_t delim, ptrdiff_t m)
{   if (!t || !s || !map || !delim) return (uchar_t *)NULL;
    if (tn > LUB_MAX_USTRLEN) tn = LUB_MAX_USTRLEN;
    if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;

    size_t s_len = ucsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) return NULL;
    size_t map_len = ucsnlen(map, LUB_MAX_UOPTLEN);
    if (map_len >= LUB_SIZE_ERRORS) return NULL;

    const uchar_t *p = map;
    const uchar_t *map_end = map + map_len;

    const uchar_t *first_needle = p;
    while (p < map_end && *p != delim) p++;
    if (p >= map_end) return (uchar_t *)NULL;
    size_t first_needle_len = (size_t)(p - first_needle);
    if (!first_needle_len) return (uchar_t *)NULL;
    p++;

    const uchar_t *first_repl = p;
    while (p < map_end && *p != delim) p++;
    size_t first_repl_len = (size_t)(p - first_repl);

    int multi_pair = (p < map_end);
    if (multi_pair && m) return (uchar_t *)NULL;

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
            if (s_len > tn) {t[tn] = (uchar_t)0; return (uchar_t *)NULL;}
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
                        return (uchar_t *)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j)
                        t[ti + j] = first_repl[j];
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (uchar_t)0;
                        return (uchar_t *)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j)
                        t[ti + j] = s[si + j];
                    ti += needle_len;
                }
                si += needle_len;
            } else {
                if (ti >= tn) {
                    t[tn] = (uchar_t)0;
                    return (uchar_t *)NULL;
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
            const uchar_t *needle = p;
            while (p < map_end && *p != delim) p++;
            size_t needle_len = (size_t)(p - needle);
            if (!needle_len) return (uchar_t *)NULL;
            p++;

            const uchar_t *repl = p;
            while (p < map_end && *p != delim) p++;
            size_t repl_len = (size_t)(p - repl);

            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (s[si + j] != needle[j]) match = 0;

            if (match) {
                if (ti + repl_len > tn) {
                    t[tn] = (uchar_t)0;
                    return (uchar_t *)NULL;
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
                return (uchar_t *)NULL;
            }
            t[ti++] = s[si++];
        }
    }

    t[ti] = (uchar_t)0;
    return t;
}

// Replace case-insensitive.
extern lchar_t *llsnnREPLACE(
    lchar_t *t, size_t tn, const lchar_t *s, const lchar_t *map, lchar_t delim, size_t n,
    ptrdiff_t m)
#if defined(LUB_DEFINITIONS)
{   if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0)) \
      return LUB_PTR_ERR(LUB_BAD_PTR, 0); \
    if (!t || !s || !map || !delim) return (lchar_t *)NULL;
    if (tn > LUB_MAX_LSTRLEN) tn = LUB_MAX_LSTRLEN;
    if (n > LUB_MAX_LSTRLEN) n = LUB_MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, n);
    if (s_len >= LUB_SIZE_ERRORS) return NULL;
    size_t map_len = lcsnlen(map, LUB_MAX_LOPTLEN);
    if (map_len >= LUB_SIZE_ERRORS) return NULL;

    const lchar_t *p = map;
    const lchar_t *map_end = map + map_len;

    const lchar_t *first_needle = p;
    while (p < map_end && *p != delim) p++;
    if (p >= map_end) return (lchar_t *)NULL;
    size_t first_needle_len = (size_t)(p - first_needle);
    if (!first_needle_len) return (lchar_t *)NULL;
    p++;

    const lchar_t *first_repl = p;
    while (p < map_end && *p != delim) p++;
    size_t first_repl_len = (size_t)(p - first_repl);

    int multi_pair = (p < map_end);
    if (multi_pair && m) return (lchar_t *)NULL;

    if (!multi_pair) {
        size_t needle_len = first_needle_len, repl_len = first_repl_len;
        size_t matches = 0;
        if (m < 0) {
            for (size_t si = 0; si + needle_len <= s_len;) {
                int match = 1;
                for (size_t j = 0; j < needle_len; ++j)
                    if (lltoupper(s[si + j]) != lltoupper(first_needle[j])) {
                        match = 0;
                        break;
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
            if (s_len > tn) {t[tn] = (lchar_t)0; return (lchar_t *)NULL;}
            for (size_t i = 0; i < s_len; ++i) t[i] = s[i];
            t[s_len] = (lchar_t)0;
            return t;
        }

        size_t si = 0, ti = 0, occ = 0;
        while (si < s_len) {
            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (lltoupper(s[si + j]) != lltoupper(first_needle[j])) match = 0;

            if (match) {
                occ++;
                if (!m || occ == target) {
                    if (ti + repl_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lchar_t *)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j)
                        t[ti + j] = first_repl[j];
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lchar_t *)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j)
                        t[ti + j] = s[si + j];
                    ti += needle_len;
                }
                si += needle_len;
            } else {
                if (ti >= tn) {
                    t[tn] = (lchar_t)0;
                    return (lchar_t *)NULL;
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
            const lchar_t *needle = p;
            while (p < map_end && *p != delim) p++;
            size_t needle_len = (size_t)(p - needle);
            if (!needle_len) return (lchar_t *)NULL;
            p++;

            const lchar_t *repl = p;
            while (p < map_end && *p != delim) p++;
            size_t repl_len = (size_t)(p - repl);

            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (lltoupper(s[si + j]) != lltoupper(needle[j])) match = 0;

            if (match) {
                if (ti + repl_len > tn) {
                    t[tn] = (lchar_t)0;
                    return (lchar_t *)NULL;
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
                return (lchar_t *)NULL;
            }
            t[ti++] = s[si++];
        }
    }

    t[ti] = (lchar_t)0;
    return t;
}
#else
    ;
#endif // LUB_DEFINITIONS for llsnnREPLACE.

extern uchar_t *ulsnnREPLACE(
     uchar_t *t, size_t tn, const uchar_t *s, const lchar_t *map, lchar_t delim, size_t n,
    ptrdiff_t m) {
    if (!t || !s || !map || !delim) return (uchar_t *)NULL;
    if (tn > LUB_MAX_USTRLEN) tn = LUB_MAX_USTRLEN;
    if (n > LUB_MAX_USTRLEN) n = LUB_MAX_USTRLEN;

    size_t s_len = ucsnlen(s, n);
    if (s_len >= LUB_SIZE_ERRORS) return NULL;
    size_t map_len = lcsnlen(map, LUB_MAX_LOPTLEN);
    if (map_len >= LUB_SIZE_ERRORS) return NULL;

    const lchar_t *p = map;
    const lchar_t *map_end = map + map_len;

    const lchar_t *first_needle = p;
    while (p < map_end && *p != delim) p++;
    if (p >= map_end) return (uchar_t *)NULL;
    size_t first_needle_len = (size_t)(p - first_needle);
    if (!first_needle_len) return (uchar_t *)NULL;
    p++;

    const lchar_t *first_repl = p;
    while (p < map_end && *p != delim) p++;
    size_t first_repl_len = (size_t)(p - first_repl);

    int multi_pair = (p < map_end);
    if (multi_pair && m) return (uchar_t *)NULL;

    if (!multi_pair) {
        size_t needle_len = first_needle_len, repl_len = first_repl_len;
        size_t matches = 0;
        if (m < 0) {
            for (size_t si = 0; si + needle_len <= s_len;) {
                int match = 1;
                for (size_t j = 0; j < needle_len; ++j)
                    if (touupper(s[si + j]) != touupper((uchar_t)first_needle[j]))
                    {
                        match = 0;
                        break;
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
            if (s_len > tn) {t[tn] = (uchar_t)0; return (uchar_t *)NULL;}
            for (size_t i = 0; i < s_len; ++i) t[i] = s[i];
            t[s_len] = (uchar_t)0;
            return t;
        }

        size_t si = 0, ti = 0, occ = 0;
        while (si < s_len) {
            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (touupper(s[si + j]) != touupper((uchar_t)first_needle[j]))
                    match = 0;

            if (match) {
                occ++;
                if (!m || occ == target) {
                    if (ti + repl_len > tn) {
                        t[tn] = (uchar_t)0;
                        return (uchar_t *)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j)
                        t[ti + j] = first_repl[j];
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (uchar_t)0;
                        return (uchar_t *)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j) t[ti + j] = s[si + j];
                    ti += needle_len;
                }
                si += needle_len;
            } else {
                if (ti >= tn) {
                    t[tn] = (uchar_t)0;
                    return (uchar_t *)NULL;
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
            const lchar_t *needle = p;
            while (p < map_end && *p != delim) p++;
            size_t needle_len = (size_t)(p - needle);
            if (!needle_len) return (uchar_t *)NULL;
            p++;

            const lchar_t *repl = p;
            while (p < map_end && *p != delim) p++;
            size_t repl_len = (size_t)(p - repl);

            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (touupper(s[si + j]) != touupper((uchar_t)needle[j])) match = 0;

            if (match) {
                if (ti + repl_len > tn) {
                    t[tn] = (uchar_t)0;
                    return (uchar_t *)NULL;
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
                return (uchar_t *)NULL;
            }
            t[ti++] = s[si++];
        }
    }

    t[ti] = (uchar_t)0;
    return t;
}

extern uchar_t *uusnnREPLACE(
     uchar_t *t, size_t tn, const uchar_t *s, size_t sn, const lchar_t *st,
    const uchar_t *map, uchar_t delim, ptrdiff_t m) {
    if (!t || !s || !map || !delim) return (uchar_t *)NULL;
    if (tn > LUB_MAX_USTRLEN) tn = LUB_MAX_USTRLEN;
    if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;

    size_t s_len = ucsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) return NULL;
    size_t map_len = ucsnlen(map, LUB_MAX_UOPTLEN);
    if (map_len >= LUB_SIZE_ERRORS) return NULL;

    const uchar_t *p = map;
    const uchar_t *map_end = map + map_len;

    const uchar_t *first_needle = p;
    while (p < map_end && *p != delim) p++;
    if (p >= map_end) return (uchar_t *)NULL;
    size_t first_needle_len = (size_t)(p - first_needle);
    if (!first_needle_len) return (uchar_t *)NULL;
    p++;

    const uchar_t *first_repl = p;
    while (p < map_end && *p != delim) p++;
    size_t first_repl_len = (size_t)(p - first_repl);

    int multi_pair = (p < map_end);
    if (multi_pair && m) return (uchar_t *)NULL;

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
            if (s_len > tn) {t[tn] = (uchar_t)0; return (uchar_t *)NULL;}
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
                        return (uchar_t *)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j) t[ti + j] = first_repl[j];
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (uchar_t)0;
                        return (uchar_t *)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j) t[ti + j] = s[si + j];
                    ti += needle_len;
                }
                si += needle_len;
            } else {
                if (ti >= tn) {
                    t[tn] = (uchar_t)0;
                    return (uchar_t *)NULL;
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
            const uchar_t *needle = p;
            while (p < map_end && *p != delim) p++;
            size_t needle_len = (size_t)(p - needle);
            if (!needle_len) return (uchar_t *)NULL;
            p++;

            const uchar_t *repl = p;
            while (p < map_end && *p != delim) p++;
            size_t repl_len = (size_t)(p - repl);

            int match = si + needle_len <= s_len;
            for (size_t j = 0; match && j < needle_len; ++j)
                if (touupper(s[si + j]) != touupper(needle[j])) match = 0;

            if (match) {
                if (ti + repl_len > tn) {
                    t[tn] = (uchar_t)0;
                    return (uchar_t *)NULL;
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
                return (uchar_t *)NULL;
            }
            t[ti++] = s[si++];
        }
    }

    t[ti] = (uchar_t)0;
    return t;
}
/** @} */

/**
 * @defgroup PrintfStyleFormatting Printf-style Formatting
 * @name llsnvprintf, llsnprintf
 * @brief Format Latin text into target buffer.
 * @param t Target buffer.
 * @param tn Maximum number of characters for target buffer (excluding terminator).
 * @param fmt Format string.
 * @param ap Variable argument list for llsnvprintf/llsnvprintf.
 * @return Number of chars written (excluding terminator), or -1 on error/truncate.
 * @note This family uses C vsnprintf semantics and treats lchar_t storage as bytes.
 * @{
 */

extern int llsnvprintf(lchar_t *t, size_t tn, const lchar_t *fmt, va_list ap)
#if defined(LUB_DEFINTIIONS)
{   if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(fmt, 0))
      return LUB_INT_ERR(LUB_BAD_PTR, 0);
    if (!t || !fmt || !tn) {
        if (t && tn) *t = (lchar_t)0;
        return -1;
    }

    if (tn > LUB_MAX_LSTRLEN + 1) tn = LUB_MAX_LSTRLEN + 1;

    int rc = vsnprintf((char *)t, tn, (const char *)fmt, ap);
    if (rc < 0 || (size_t)rc >= tn) {
        t[tn - 1] = (lchar_t)0;
        return -1;
    }
    return rc;
}

extern int llsnprintf(lchar_t *t, size_t tn, const lchar_t *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int rc = llsnvprintf(t, tn, fmt, ap);
    va_end(ap);
    return rc;
}
/** @} */

 /**
 * @defgroup Compare Compare
 * @name llsncmp, lusncmp, ulsncmp, uusncmp (case-sensitive)
 *       llsnCMP, lusnCMP, ulsnCMP, uusnCMP (case-insensitive)
 *       bbsncmp
 * @brief Comparison of two strings.
 * @param s1 First source string.
 * @param s2 Second source string.
 * @param sn Bound on both strings (clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN) for
 *           bounded functions only, Default-bounded functions use LUB_MAX_LSTRLEN
 *           or LUB_MAX_USTRLEN.
 * @return -1 (s1 < s2), 0 (equal), 1 (s1 > s2), or error
 * @note Errors:
 *       - LUB_BAD_PTR if s1 or s2 is an invalid pointer.
 *       - LUB_UNTERMINATED if s1 or s2 is not null-terminated.
 * @{
 */

#if defined(LUB_DEFINITIONS)
// Compare macro helper.
#undef __LUB_OP_HELPER__
#define __LUB_OP_HELPER__(s1_max_xstrlen, s2_max_xstrlen, \
                          s1_c, s2_c, s1_C, s2_C) \
{   if (LUB_PTR_ERR(s1, 0) || LUB_PTR_ERR(s2, 0)) \
      return LUB_INT_ERR(LUB_BAD_PTR, 0); \
    if (!s1 && !s2) return LUB_CMP_EQUAL; \
    if (!s1) \
      return (!s2 || !*s2) ? LUB_CMP_EQUAL : LUB_CMP_LESS_THAN; \
    if (!s2) \
      return (!*s1) ? LUB_CMP_EQUAL : LUB_CMP_GREATER_THAN; \
    size_t s1_sn = sn > s1_max_xstrlen ? s1_max_xstrlen : sn; \
    size_t s2_sn = sn > s2_max_xstrlen ? s2_max_xstrlen : sn; \
    for (; \
         s1_sn && *s1 && s2_sn && *s2 &&; \
         s1_sn--, s2_sn--, s1++, s2++) { \
      int c1 = s1_C >= 0 && s1_C <= MAX_UCHAR ? \
               (int)s1_C : (int)s1_c; \
      int c2 = s2_C >= 0 && s2_C <= MAX_UCHAR ? \
               (int)s2_C : (int)s2_c; \
      if (c1 != c2) break; \
      } \
    if ((!s1_sn && *s1) || (!s2_sn && *s2)) \
      return LUB_INT_ERR(LUB_UNTERMINATED, 0); \
    if (!*s1 && !*s2) return LUB_CMP_EQUAL; \
    return c1 < c2 ? \
           LUB_CMP_LESS_THAN : LUB_CMP_GREATER_THAN; \  
}
#endif // LUB_DEFINITIONS

// compare case-sensitive.
extern int llsncmp(const lchar_t *s1, const lchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(LUB_MAX_LSTRLEN, LUB_MAX_LSTRLEN,
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned char)(*s2),
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned char)(*s2))
#else
    ;
#endif // LUB_DEFINITIONS

extern int lusncmp(const lchar_t *s1, const uchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(LUB_MAX_LSTRLEN, LUB_MAX_USTRLEN,
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned short)(*s2),
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned short)(*s2))
#else
    ;
#endif // LUB_DEFINITIONS

extern int ulsncmp(const uchar_t *s1, const lchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(LUB_MAX_USTRLEN, LUB_MAX_LSTRLEN,
                      (wint_t)(unsigned short)(*s1),
                      (wint_t)(unsigned char)(*s2),
                      (wint_t)(unsigned short)(*s1),
                      (wint_t)(unsigned char)(*s2))
#else
    ;
#endif // LUB_DEFINITIONS

extern int uusncmp(const uchar_t *s1, const uchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(LUB_MAX_USTRLEN, LUB_MAX_USTRLEN,
                      (wint_t)(unsigned short)(*s1),
                      (wint_t)(unsigned short)(*s2),
                      (wint_t)(unsigned short)(*s1),
                      (wint_t)(unsigned short)(*s2))
#else
    ;
#endif // LUB_DEFINITIONS

// compare case-insensitive.
extern int llsnCMP(const lchar_t *s1, const lchar_t *s2, size_t n)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(LUB_MAX_LSTRLEN, LUB_MAX_LSTRLEN,
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned short)(*s2),
                      (wint_t)toupper((int)(unsigned char)(*s1)),
                      (wint_t)towupper((wint_t)(unsigned short)(*s2)))
#else
    ;
#endif // LUB_DEFINITIONS

extern int lusnCMP(const lchar_t *s1, const uchar_t *s2, size_t n)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(LUB_MAX_LSTRLEN, LUB_MAX_USTRLEN,
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned short)(*s2),
                      (wint_t)toupper((int)(unsigned char)(*s1)),
                      (wint_t)towupper((wint_t)(unsigned short)(*s2)))
#else
    ;
#endif // LUB_DEFINITIONS

extern int ulsnCMP(const uchar_t *s1, const lchar_t *s2, size_t n)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(LUB_MAX_USTRLEN, LUB_MAX_LSTRLEN,
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned short)(*s2),
                      (wint_t)towupper((wint_t)(unsigned short)(*s1)),
                      (wint_t)toupper((int)(unsigned char)(*s2))))
#else
    ;
#endif // LUB_DEFINITIONS

extern int uusnCMP(const uchar_t *s1, const uchar_t *s2, size_t n)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(LUB_MAX_USTRLEN, LUB_MAX_USTRLEN,
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned short)(*s2),
                      (wint_t)towupper((wint_t)(unsigned short)(*s1)),
                      (wint_t)towupper((wint_t)(unsigned short)(*s2)))
#else
    ;
#endif // LUB_DEFINITIONS

#if defined(LUB_DEFINITIONS)
#undef __LUB_OP_HELPER__
#endif // LUB_DEFINITIONS

/**
 *TBD: byte to byte compare
 * @{
 */

/** @} */

/**
 * @defgroup FixedLengthLeadingSubstringCompare Fixed-Length Leading Substring Compare
 * @name llsnfxdcmp, lusnfxdcmp, ulsnfxdcmp, uusnfxdcmp (case-sensitive)
 *       llsnFXDCMP, lusnFXDCMP, ulsnFXDCMP, uusnFXDCMP (case-insensitive)
 * @brief Compare of sn-length leading substrings of two source strings.
 * @param s1 First source string.
 * @param s2 Second source string.
 * @param sn Maximum number of characters in the leading substrings to compare.
 *           Clamped to LUB_MAX_LSTRLEN if ll, otherwise LUB_MAX_USTRLEN.
 * @return 0 if all sn characters are equal and both substrings are at least sn long,
 *        -1 if leading substring of s1 < leading substring of s2,
 *         1 if leading substring of s1 > leading substring of s2.
 * @note Errors:
 *       - LUB_BAD_PTR if s1 or s2 is an invalid pointer.
 * @note The leading substring may be shorter than sn if a null
 *       terminator is encountered.
 * @note s1 and s2 are not checked for null-termination beyond sn.
 * @note Stops comparing at null terminator in either string;
 *       if either string is shorter than sn,
 *       returns 1 or -1 (short string is less if all compared chars match).
 * @note Returns 0 if sn == 0. If s1 or s2 is NULL, it is treated as a 
 *       zero-length string.
 * @{
 */

#if defined(LUB_DEFINITIONS)
// Fixed-length leading substring compare macro helper.
#undef __LUB_OP_HELPER__
#define __LUB_OP_HELPER__(max_xstrlen \
                          s1_c, s2_c, s1_C, s2_C) \
{   if (LUB_PTR_ERR(s1, 0) || LUB_PTR_ERR(s2, 0)) \
      return LUB_INT_ERR(LUB_BAD_PTR, 0); \
    if (!s1 && !s2) return LUB_CMP_EQUAL; \
    if (!s1) return (!s2 || !*s2) ? LUB_CMP_EQUAL : LUB_CMP_LESS_THAN; \
    if (!s2) return (!*s1) ? LUB_CMP_EQUAL : LUB_CMP_GREATER_THAN; \
    if (!*s1 && !*s2) return LUB_CMP_EQUAL; \
    if (*s1 && !*s2) return LUB_CMP_GREATER_THAN; \
    if (!*s1 && *s2) return LUB_CMP_LESS_THAN; \
    sn = sn > max_xstrlen ? max_xstrlen : sn; \
    for (; sn; sn--, s1++, s2++) { \
      int c1 = s1_C >= 0 && s1_C <= MAX_UCHAR ? (int)s1_C : (int)s1_c; \
      int c2 = s2_C >= 0 && s2_C <= MAX_UCHAR ? (int)s2_C : (int)s2_c; \
      if (!c1 || !c2) { \
        if (c1 == c2) break; \
        return c1 < c2 ? \
               LUB_CMP_LESS_THAN : LUB_CMP_GREATER_THAN; \
      } \
    if (c1 != c2) \
      return c1 < c2 ? \
             LUB_CMP_LESS_THAN : LUB_CMP_GREATER_THAN; \
    } \
    if (sn == 0) return LUB_CMP_EQUAL; \
    if (c1 == c2) return LUB_CMP_EQUAL; \
    return c1 < c2 ? \  
           LUB_CMP_LESS_THAN : LUB_CMP_GREATER_THAN; \                     
}
#endif // LUB_DEFINITIONS

// Fixed-length compare case-sensitive.
extern int llsnfxdcmp(const lchar_t *s1, const lchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(LUB_MAX_LSTRLEN,
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned char)(*s2),
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned char)(*s2))
#else
    ;
#endif // LUB_DEFINITIONS

extern int lusnfxdcmp(const lchar_t *s1, const uchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(LUB_MAX_USTRLEN,
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned short)(*s2),
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned short)(*s2))
#else
    ;
#endif // LUB_DEFINITIONS

extern int ulsnfxdcmp(const uchar_t *s1, const lchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(LUB_MAX_USTRLEN,
                      (wint_t)(unsigned short)(*s1),
                      (wint_t)(unsigned char)(*s2),
                      (wint_t)(unsigned short)(*s1),
                      (wint_t)(unsigned char)(*s2))
#else
    ;
#endif // LUB_DEFINITIONS

extern int uusnfxdcmp(const uchar_t *s1, const uchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(LUB_MAX_USTRLEN,
                    (wint_t)(unsigned short)(*s1),
                      (wint_t)(unsigned short)(*s2),
                      (wint_t)(unsigned short)(*s1),
                      (wint_t)(unsigned short)(*s2))
#else
    ;
#endif // LUB_DEFINITIONS

// Fixed-length compare case-insensitive.
extern int llsnFXDCMP(const lchar_t *s1, const lchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(LUB_MAX_LSTRLEN,
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned char)(*s2),
                      (wint_t)toupper((int)(unsigned char)(*s1)),
                      (wint_t)toupper((int)(unsigned char)(*s2))
#else
    ;
#endif // LUB_DEFINITIONS

extern int lusnFXDCMP(const lchar_t *s1, const uchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(LUB_MAX_USTRLEN,
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned short)(*s2),
                      (wint_t)toupper((int)(unsigned char)(*s1)),
                      (wint_t)towupper((wint_t)(unsigned short)(*s2)))
#else
    ;
#endif // LUB_DEFINITIONS

extern int ulsnFXDCMP(const uchar_t *s1, const lchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(LUB_MAX_USTRLEN,
                      (wint_t)(unsigned char)(*s2),
                      (wint_t)towupper((wint_t)(unsigned short)(*s1)),
                      (wint_t)toupper((wint_t)(unsigned char)(*s2)))
#else
    ;
#endif // LUB_DEFINITIONS

extern int uusnFXDCMP(const uchar_t *s1, const uchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(LUB_MAX_USTRLEN,
                      (wint_t)(unsigned short)(*s1),
                      (wint_t)(unsigned short)(*s2),
                      (wint_t)towupper((wint_t)(unsigned short)(*s1)),
                      (wint_t)towupper((wint_t)(unsigned short)(*s2)))
#else
    ;
#endif // LUB_DEFINITIONS

#if defined(LUB_DEFINITIONS)
#undef __LUB_OP_HELPER__
#endif // LUB_DEFINITIONS
/** @} */

/**
 * @defgroup PrefixCompare Prefix Compare
 * @name llsnpfxcmp, lusnpfxcmp, ulsnpfxcmp, and uusnpfxcmp (case-sensitive)
 *       llsnPFXCMP, lusnPFXCMP, ulsnPFXCMP, and uusnPFXCMP (case-insensitive)
 * @brief Check if s2 is a prefix of s1.
 * @param s1 Source string check.
 * @param s2 Prefix string.
 * @param sn Bound on s1 and s2.
 *           sn clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * @return 0 if s2 is a prefix of s1,
 *         LUB_CMP_LESS_THAN (-1) if s1 is less than s2.
 *         LUB_CMP_GREATER_THAN (1) if s1 is greater than s2.
 *         Or error.
 * @note Errors:
 *      - LUB_BAD_PTR if s1 or s2 is an invalid pointer.
 *      - LUB_UNTERMINATED if s1 or s2 is not null-terminated.
 * @{
 */

#if defined(LUB_DEFINITIONS)
// Prefix compare macro helper.
#undef __LUB_OP_HELPER__
#define __LUB_OP_HELPER__(s1_XCSNLEN, s2_XCSNLEN, \
                          s1_MAX_XSTRLEN, s2_MAX_XSTRLEN, \
                          s1_c, s2_c, s1_C, s2_C) \
{   if (LUB_PTR_ERR(s1, 0) || LUB_PTR_ERR(s2, 0)) \
      return LUB_INT_ERR(LUB_BAD_PTR, 0); \
    size_t s1_len = s1_XCSNLEN(s1, sn > s1_MAX_XSTRLEN ?
                                   s1_MAX_XSTRLEN : sn); \
    if (s1_len >= LUB_SIZE_ERRORS) return LUB_INT_ERR(s1_len, 0); \
    size_t s2_len = s2_XCSNLEN(s2, sn > s2_MAX_XSTRLEN ?
                                   s2_MAX_XSTRLEN : sn); \
    if (s2_len >= LUB_SIZE_ERRORS) return LUB_INT_ERR(s2_len, 0); \
    size_t k = s1_len < s2_len ? s1_len : s2_len; \
    for (size_t i = 0; i < k; ++i) { \
      int c1 = s1_C >= 0 && s1_C <= MAX_UCHAR ? (int)s1_C : (int)s1_c; \
      int c2 = s2_C >= 0 && s2_C <= MAX_UCHAR ? (int)s2_C : (int)s2_c; \
      if (c1 != c2) \
        return c1 > c2 ? LUB_CMP_GREATER_THAN : LUB_CMP_LESS_THAN; \
    } \
    if (s2_len <= s1_len) return LUB_CMP_EQUAL; \
    return LUB_CMP_LESS_THAN; \
}
#endif // LUB_DEFINITIONS

// Prefix check (case-sensitive).
extern int llsnpfxcmp(const lchar_t *s1, const lchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(lcsnlen, lcsnlen,
                      LUB_MAX_LSTRLEN, LUB_MAX_LSTRLEN,
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned char)(*s2),
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned char)(*s2))
#else
    ;
#endif // LUB_DEFINITIONS

extern int lusnpfxcmp(const lchar_t *s1, const uchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(lcsnlen, ucsnlen,
                      LUB_MAX_LSTRLEN, LUB_MAX_USTRLEN,
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned short)(*s2),
                      (wint_t)(unsigned char)(*s1),
                      (wint_t)(unsigned short)(*s2)) 
#else
    ;
#endif // LUB_DEFINITIONS

extern int ulsnpfxcmp(const uchar_t *s1, const lchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
    __LUB_OP_HELPER__(ucsnlen, lcsnlen,
                      LUB_MAX_USTRLEN, LUB_MAX_LSTRLEN,
                      (wint_t)(unsigned short)(*s1),
                      (wint_t)(unsigned char)(*s2),
                      (wint_t)(unsigned short)(*s1),
                      (wint_t)(unsigned char)(*s2)) 
#else
    ;
#endif // LUB_DEFINITIONS

extern int uusnpfxcmp(const uchar_t *s1, const uchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
   __LUB_OP_HELPER__(ucsnlen, ucsnlen,
                     LUB_MAX_USTRLEN, LUB_MAX_USTRLEN,
                     (wint_t)(unsigned short)(*s1),
                     (wint_t)(unsigned short)(*s2),
                     (wint_t)(unsigned short)(*s1),
                     (wint_t)(unsigned short)(*s2))
#else
    ;
#endif // LUB_DEFINITIONS

// Prefix check (case-insensitive).
extern int llsnPFXCMP(const lchar_t *s1, const lchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
   __LUB_OP_HELPER__(lcsnlen, lcsnlen,
                     LUB_MAX_LSTRLEN, LUB_MAX_LSTRLEN,
                     (wint_t)(unsigned char)(*s1),
                     (wint_t)(unsigned char)(*s2),
                     (wint_t)toupper((int)(unsigned char)(*s1)),
                     (wint_t)toupper((int)(unsigned char)(*s2)), 1)
#else
    ;
#endif // LUB_DEFINITIONS

extern int lusnPFXCMP(const lchar_t *s1, const uchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
   __LUB_OP_HELPER__(lcsnlen, ucsnlen,
                     LUB_MAX_LSTRLEN, LUB_MAX_USTRLEN,
                     (wint_t)(unsigned char)(*s1),
                     (wint_t)(unsigned short)(*s2),
                     (wint_t)toupper((int)(unsigned char)(*s1)),
                     (wint_t)towupper((wint_t)(unsigned short)(*s2)))
#else
    ;
#endif // LUB_DEFINITIONS

extern int ulsnPFXCMP(const uchar_t *s1, const lchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
   __LUB_OP_HELPER__(ucsnlen, lcsnlen,
                     LUB_MAX_USTRLEN, LUB_MAX_LSTRLEN,
                     (wint_t)(unsigned short)(*s1),
                     (wint_t)(unsigned char)(*s2),
                     (wint_t)towupper((wint_t)(unsigned short)(*s1)),
                     (wint_t)toupper((int)(unsigned char)(*s2)))
#else
    ;
#endif // LUB_DEFINITIONS

extern int uusnPFXCMP(const uchar_t *s1, const uchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
   __LUB_OP_HELPER__(ucsnlen, ucsnlen,
                     LUB_MAX_USTRLEN, LUB_MAX_USTRLEN,
                     (wint_t)(unsigned short)(*s1),
                     (wint_t)(unsigned short)(*s2),
                     (wint_t)towupper((wint_t)(unsigned short)(*s1)),
                     (wint_t)towupper((wint_t)(unsigned short)(*s2)))
#else
    ;
#endif // LUB_DEFINITIONS

#if defined(LUB_DEFINITIONS)
#undef __LUB_OP_HELPER__
#endif // LUB_DEFINITIONS

/** @} */

/**
 * @defgroup SuffixCompare Suffix Compare
 * @name llsnsfxcmp, lusnsfxcmp, ulsnsfxcmp, and uusnsfxcmp (case-sensitive)
 *       llsnSFXCMP, lusnSFXCMP, ulsnSFXCMP, and uusnSFXCMP (case-insensitive)
 * @brief Check if s2 is a suffix of s1.
 * @param s1 Source string.
 * @param s2 Suffix string.
 * @param sn Bound on s1 and s2.
 *           sn clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * @return 0 if s2 is a suffix of s1,
 *         LUB_CMP_LESS_THAN (-1) if the suffix of s1 is less than s2.
 *         LUB_CMP_GREATER_THAN (1) if the suffix of s1 is greater than s2.
 *         Or error.
 * @note Errors:
 *      - LUB_BAD_PTR if s1 or s2 is an invalid pointer.
 *      - LUB_UNTERMINATED if s1 or s2 is not null-terminated.
 * @{
 */

#if defined(LUB_DEFINITIONS)
// Suffix compare macro helper.
#undef __LUB_OP_HELPER__
#define __LUB_OP_HELPER__(s1_XCSNLEN, s2_XCSNLEN, \
                          s1_MAX_XSTRLEN, s2_MAX_XSTRLEN, \
                          s1_c, s2_c, s1_C, s2_C) \
{   if (LUB_PTR_ERR(s1, 0) || LUB_PTR_ERR(s2, 0)) \
      return LUB_INT_ERR(LUB_BAD_PTR, 0); \
    size_t s1_len = s1_XCSNLEN(s1, sn > s1_MAX_XSTRLEN ?
                                   s1_MAX_XSTRLEN : sn); \
    if (s1_len >= LUB_SIZE_ERRORS) return LUB_INT_ERR(s1_len, 0); \
    size_t s2_len = s2_XCSNLEN(s2, sn > s2_MAX_XSTRLEN ?
                                   s2_MAX_XSTRLEN : sn); \
    if (s2_len >= LUB_SIZE_ERRORS) return LUB_INT_ERR(s2_len, 0); \
    if (!s2_len) return LUB_CMP_EQUAL; \
    if (s2_len > s1_len) return LUB_CMP_LESS_THAN; \
    size_t start = s1_len - s2_len; \
    for (size_t i = 0; i < s2_len; ++i) { \
      int c1 = s1_C >= 0 && s1_C <= MAX_UCHAR ? (int)s1_C : (int)s1_c; \
      int c2 = s2_C >= 0 && s2_C <= MAX_UCHAR ? (int)s2_C : (int)s2_c; \
      if (c1 != c2) \
        return c1 > c2 ? LUB_CMP_GREATER_THAN : LUB_CMP_LESS_THAN; \
    } \
    return LUB_CMP_EQUAL; \
}
#endif // LUB_DEFINITIONS

// Suffix check (case-sensitive).
extern int llsnsfxcmp(const lchar_t *s1, const lchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
   __LUB_OP_HELPER__(lcsnlen, lcsnlen,
                     LUB_MAX_LSTRLEN, LUB_MAX_LSTRLEN,
                     (wint_t)(unsigned char)(*s1),
                     (wint_t)(unsigned char)(*s2),
                     (wint_t)(unsigned char)(*s1),
                     (wint_t)(unsigned char)(*s2))
#else
    ;
#endif // LUB_DEFINITIONS

extern int lusnsfxcmp(const lchar_t *s1, const uchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
   __LUB_OP_HELPER__(lcsnlen, ucsnlen,
                     LUB_MAX_LSTRLEN, LUB_MAX_USTRLEN,
                     (wint_t)(unsigned char)(*s1),
                     (wint_t)(unsigned short)(*s2),
                     (wint_t)(unsigned char)(*s1),
                     (wint_t)(unsigned short)(*s2))
#else
    ;
#endif // LUB_DEFINITIONS

extern int ulsnsfxcmp(const uchar_t *s1, const lchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
   __LUB_OP_HELPER__(ucsnlen, lcsnlen,
                     LUB_MAX_USTRLEN, LUB_MAX_LSTRLEN,
                     (wint_t)(unsigned short)(*s1),
                     (wint_t)(unsigned char)(*s2),
                     (wint_t)(unsigned short)(*s1),
                     (wint_t)(unsigned char)(*s2))
#else
    ;
#endif // LUB_DEFINITIONS

extern int uusnsfxcmp(const uchar_t *s1, const uchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
   __LUB_OP_HELPER__(ucsnlen, ucsnlen,
                     LUB_MAX_USTRLEN, LUB_MAX_USTRLEN,
                     (wint_t)(unsigned short)(*s1),
                     (wint_t)(unsigned short)(*s2),
                     (wint_t)(int)(unsigned short)(*s1),
                     (wint_t)(int)(unsigned short)(*s2))
#else
    ;
#endif // LUB_DEFINITIONS

// Suffix check (case-insensitive).
extern int llsnSFXCMP(const lchar_t *s1, const lchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
   __LUB_OP_HELPER__(lcsnlen, lcsnlen,
                     LUB_MAX_LSTRLEN, LUB_MAX_LSTRLEN,
                     (wint_t)(unsigned char)(*s1),
                     (wint_t)(unsigned char)(*s2),
                     (wint_t)toupper((int)(unsigned char)(*s1)),
                     (wint_t)toupper((int)(unsigned char)(*s2)))
#else
    ;
#endif // LUB_DEFINITIONS

extern int lusnSFXCMP(const lchar_t *s1, const uchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
   __LUB_OP_HELPER__(lcsnlen, ucsnlen,
                     LUB_MAX_LSTRLEN, LUB_MAX_USTRLEN,
                     (wint_t)(unsigned char)(*s1),
                     (wint_t)(unsigned short)(*s2),
                     (wint_t)toupper((int)(unsigned char)(*s1)),
                     (wint_t)towupper((wint_t)(unsigned short)(*s2)))
#else
    ;
#endif // LUB_DEFINITIONS

extern int ulsnSFXCMP(const uchar_t *s1, const lchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
   __LUB_OP_HELPER__(ucsnlen, lcsnlen,
                     LUB_MAX_USTRLEN, LUB_MAX_LSTRLEN,
                     (wint_t)(unsigned short)(*s1),
                     (wint_t)(unsigned char)(*s2),
                     (wint_t)towupper((wint_t)(unsigned short)(*s1)),
                     (wint_t)toupper((int)(unsigned char)(*s2)))
#else
    ;
#endif // LUB_DEFINITIONS

extern int uusnSFXCMP(const uchar_t *s1, const uchar_t *s2, size_t sn)
#if defined(LUB_DEFINITIONS)
   __LUB_OP_HELPER__(ucsnlen, ucsnlen,
                     LUB_MAX_USTRLEN, LUB_MAX_USTRLEN,
                     (wint_t)(unsigned short)(*s1),
                     (wint_t)(unsigned short)(*s2),
                     (wint_t)towupper((wint_t)(unsigned short)(*s1)),
                     (wint_t)towupper((wint_t)(unsigned short)(*s2)))
#else
    ;
#endif // LUB_DEFINITIONS

#if defined(LUB_DEFINITIONS)
#undef __LUB_OP_HELPER__
#endif // LUB_DEFINITIONS
/** @} */

/**
 * @defgroup StringSearch String Search
 * @name llsnstrm, uusnstrm (case-sensitive)
 *       llsnSTRM, uusnSTRM (case-insensitive)
 * @brief Search for the mth occurrence of string s2 in string s1.
 * @param s1 Haystack string.
 * @param s2 Strings of needle substrings separated by delim or 
 *           string of needle characters.
 * 
 *           A zero-length needle substring is treated as a match
 *           for a zero-length string s1 if m is -1 or 1, otherwise,
 *           there is no match for this needle substring.
 * 
 *           If delim is a null character and s2 is zero-length,
 *           there is no match to s1.
 * @param sn Bound on s1 and s2 (clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN).
 * @param delim Delimiter character for needle substrings. If null character, indicates
 *              s2 is a string of needle characters.
 * @param m Occurrence index (1 = first, 2 = second, ...). 0 returns NULL.
 *          mth occurrence for matching. m > 0 counts from the
 *          beginning of the string (1 means first occurrence).
 *          m == 0 returns NULL. m < 0 counts from the end of the string
 *          (-1 means last occurrence) when the operation supports reverse selection.
 * @return Pointer to mth match in s1, NULL if not found, or error.
 * @note Errors:
 *       - LUB_BAD_PTR if s1 or s2 is an invalid pointer.
 *       - LUB_UNTERMINATED if s1 or s2 is not null-terminated.
 * * @{
 */

 // String search (case-sensitive).

extern lchar_t *llsnstrm(
    const lchar_t *s1, const lchar_t *const s2, size_t sn,
    const lchar_t delim, int m)
#if defined(LUB_DEFINITIONS)
{   if (LUB_PTR_ERR(s1, 0) || LUB_PTR_ERR(s2, 0))
        return LUB_INT_ERR(LUB_BAD_PTR, 0);
    if (!s1 || !s2 || !m) return (lchar_t *)NULL;
    if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (lchar_t *)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (lchar_t *)s2_len;
    if (!s2_len)
      return m == 1 ? (lchar_t *)s1 : (lchar_t *)NULL;
    if (s2_len > s1_len) return (lchar_t *)NULL;


    // If delim is null character, treat s2 as a set of needle characters (like strpbrk)
    if (delim == 0) {
        if (m > 0) {
            size_t count = 0;
            const lchar_t *p = s1, *end = s1 + s1_len;
            for (; p < end; ++p) {
                const lchar_t *q = s2, *qend = s2 + s2_len;
                for (; q < qend; ++q) {
                    if (*p == *q) {
                        if (++count == (size_t)m)
                            return (lchar_t *)p;
                        break;
                    }
                }
            }
            return (lchar_t *)NULL;
        } else if (m < 0) {
            size_t match_count = 0;
            const lchar_t *positions[LUB_MAX_LSTRLEN > 1024 ? 1024 : LUB_MAX_LSTRLEN];
            const lchar_t *p = s1, *end = s1 + s1_len;
            for (; p < end; ++p) {
                const lchar_t *q = s2, *qend = s2 + s2_len;
                for (; q < qend; ++q) {
                    if (*p == *q) {
                        if (match_count < sizeof(positions)/sizeof(positions[0]))
                            positions[match_count++] = p;
                        break;
                    }
                }
            }
            size_t abs_m = (size_t)(-m);
            if (match_count >= abs_m)
                return (lchar_t *)positions[match_count - abs_m];
            return (lchar_t *)NULL;
        } else {
            return (lchar_t *)NULL;
        }
    }

    if (m > 0) {
        size_t count = 0;
        const lchar_t *p = s1, *end = s1 + s1_len - s2_len + 1;
        for (; p < end; ++p) {
            const lchar_t *q = s2, *qend = s2 + s2_len;
            const lchar_t *pp = p;
            while (q < qend && *pp == *q) {
                ++pp; ++q;
            }
            if (q == qend) {
                if (++count == (size_t)m) return (lchar_t *)p;
            }
        }
        return (lchar_t *)NULL;
    } else if (m < 0) {
        size_t match_count = 0;
        const lchar_t *positions[LUB_MAX_LSTRLEN > 1024 ? 1024 : LUB_MAX_LSTRLEN];
        const lchar_t *p = s1, *end = s1 + s1_len - s2_len + 1;
        for (; p < end; ++p) {
            const lchar_t *q = s2, *qend = s2 + s2_len;
            const lchar_t *pp = p;
            while (q < qend && *pp == *q) {
                ++pp; ++q;
            }
            if (q == qend) {
                if (match_count < sizeof(positions)/sizeof(positions[0]))
                    positions[match_count++] = p;
            }
        }
        size_t abs_m = (size_t)(-m);
        if (match_count >= abs_m)
            return (lchar_t *)positions[match_count - abs_m];
        return (lchar_t *)NULL;
    } else {
        return (lchar_t *)NULL;
    }
}
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *ulsnstrm(
    const uchar_t *s1, const lchar_t *const s2, const size_t sn,
    const uchar_t delim, const int m)
#if defined(LUB_DEFINITIONS)
{   if (!s1 || !s2 || !m) return (uchar_t *)NULL;
    if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (uchar_t *)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (uchar_t *)s2_len;
    if (!s2_len) return m == 1 ? s1 : (uchar_t *)NULL;
    if (s2_len > s1_len) return (uchar_t *)NULL;
    size_t count = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && s1[i + k] == (uchar_t)s2[k]; ++k);
        if (k == s2_len) {
            if (++count == m) return s1 + i;
        }
    }
    return (uchar_t *)NULL;
}
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *uusnstrm(
    const uchar_t *const s1, const uchar_t *const s2, size_t sn,
    const uchar_t delim, const int m)
#if defined(LUB_DEFINITIONS)
{   if (!s1 || !s2 || !m) return (uchar_t *)NULL;
    if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (uchar_t *)s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (uchar_t *)s2_len;
    if (!s2_len) return m == 1 ? s1 : (uchar_t *)NULL;
    if (s2_len > s1_len) return (uchar_t *)NULL;
    size_t count = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && s1[i + k] == s2[k]; ++k);
        if (k == s2_len) {
            if (++count == m) return s1 + i;
        }
    }
    return (uchar_t *)NULL;
}
#else
    ;
#endif // LUB_DEFINITIONS

// String search (case-insensitive).

extern lchar_t *llsnSTRM(
    const lchar_t *s1, const lchar_t *const s2, const size_t sn,
    const uchar_t delim, const int m)
#if defined(LUB_DEFINITIONS)
{   if (!s1 || !s2 || !m)
        return (lchar_t *)NULL;
    if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (lchar_t *)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (lchar_t *)s2_len;
    if (!s2_len) return m == 1 ? s1 : (lchar_t *)NULL;
    if (s2_len > s1_len) return (lchar_t *)NULL;
    size_t count = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && tolupper(s1[i + k]) == tolupper(s2[k]); ++k);
        if (k == s2_len) {
            if (++count == m) return s1 + i;
        }
    }
    return (lchar_t *)NULL;
}
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *ulsnSTRM(
    const uchar_t *s1, const lchar_t *const s2, const size_t sn,
    const uchar_t delim, const int m)
#if defined(LUB_DEFINITIONS)
{   if (!s1 || !s2 || !m) return (uchar_t *)NULL;
    if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (uchar_t *)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (uchar_t *)s2_len;
    if (!s2_len) return m == 1 ? s1 : (uchar_t *)NULL;
    if (s2_len > s1_len) return (uchar_t *)NULL;
    size_t count = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len &&
             touupper(s1[i + k]) == touupper((uchar_t)s2[k]); ++k);
        if (k == s2_len) {
            if (++count == m) return s1 + i;
        }
    }
    return (uchar_t *)NULL;
}
#else
    ;
#endif // LUB_DEFINITIONS

extern uchar_t *uusnSTRM(
    const uchar_t *s1, const uchar_t *const s2, const size_t sn,
    const uchar_t delim, const int m)
#if defined(LUB_DEFINITIONS)
{   if (!s1 || !s2 || !m) return (uchar_t *)NULL;
    if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (uchar_t *)s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (uchar_t *)s2_len;
    if (!s2_len) return m == 1 ? s1 : (uchar_t *)NULL;
    if (s2_len > s1_len) return (uchar_t *)NULL;
    size_t count = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && touupper(s1[i + k]) == touupper(s2[k]); ++k);
        if (k == s2_len) {
            if (++count == m) return s1 + i;
        }
    }
    return (uchar_t *)NULL;
}
#else
    ;
#endif // LUB_DEFINITIONS
/** @} */

/**
 * @defgroup Count Count
 * @name llsncnt, ulsncnt, uusncnt (case-sensitive)
 *       llsnCNT, ulsnCNT, uusnCNT (case-insensitive)
 * @brief Count of matches of string s2 in string s1
 *        (for substrings, overlapping matches are counted).
 * @param s1 Haystack string.
 * @param s2 Strings of needle substrings separated by delim or 
 *           string of needle characters.
 * @param sn Bound on s1 and s2 (clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN).
 * @param delim Delimiter character for needle substrings. If null, indicates
 *              s2 is a string of needle characters.
 * @return Count of matches, or error.
 * @note Errors;
 *       - LUB_BAD_PTR if s1 or s2 is an invalid pointer.
 *       - LUB_UNTERMINATED if s1 or s2 is not null-terminated.
 * @note If s2 is empty, returns 0.
 * @{
 */

// Count of matches of string s2 in string s1 (case-sensitive).

extern size_t llsncnt(const lchar_t *s1, const lchar_t *const s2, size_t sn,
                      const lchar_t delim)
#if defined(LUB_DEFINITIONS)
{   if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s1_len >= LUB_SIZE_ERRORS) s2_len;
    if (!s2_len || s2_len > s1_len) return (size_t)0;
    size_t c = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && s1[i + k] == s2[k]; ++k);
        if (k == s2_len) ++c;
    }
    return c;
}
#else
    ;
#endif // LUB_DEFINITIONS

extern size_t ulsncnt(const uchar_t *s1, const lchar_t *const s2, size_t sn,
                      const lchar_t delim)
#if defined(LUB_DEFINITIONS)
{   if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) s2_len;
    if (!s2_len || s2_len > s1_len) return (size_t)0;
    size_t c = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len && s1[i + k] == (uchar_t)s2[k]; ++k);
        if (k == s2_len) ++c;
    }
    return c;
}
#else
    ;
#endif // LUB_DEFINITIONS

extern size_t uusncnt(const uchar_t *s1, const uchar_t *const s2, size_t sn,
                      const uchar_t delim)
#if defined(LUB_DEFINITIONS)
{   if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) s2_len;
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
#else
    ;
#endif // LUB_DEFINITIONS

//n Count of matches of string s2 in string s1 (case-insensitive).

extern size_t llsnCNT(const lchar_t *s1, const lchar_t *s2, size_t sn,
                      const lchar_t delim)
#if defined(LUB_DEFINITIONS)
{   if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    if (!s2_len || s2_len > s1_len) return (size_t)0;
    size_t c = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len &&
               tolupper(s1[i + k]) == tolupper(s2[k]); ++k);
        if (k == s2_len) ++c;
    }
    return c;
}
#else
    ;
#endif // LUB_DEFINITIONS

extern size_t ulsnCNT(const uchar_t *s1, const lchar_t *const s2, size_t sn,
                      const lchar_t delim)
#if defined(LUB_DEFINITIONS)
{   if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    if (!s2_len || s2_len > s1_len) return (size_t)0;
    size_t c = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len &&
               touupper(s1[i + k]) == touupper((uchar_t)s2[k]); ++k);
        if (k == s2_len) ++c;
    }
    return c;
}
#else
    ;
#endif // LUB_DEFINITIONS

extern size_t uusnCNT(const uchar_t *s1, const uchar_t *const s2, size_t sn,
                      const uchar_t delim)
#if defined(LUB_DEFINITIONS)
{   if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
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
#else
    ;
#endif // LUB_DEFINITIONS
/** @} */

/**
 * @section Examples Examples
 *
 * - Copy (bounded source):
 *   llsncpy(dst_l, src_l, 32);
 *
 * - Search (mth occurrence):
 *   const lchar_t *p = llsnstrm("one two two", "two", 32, 2);
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
 *   llsnREPLACE(t, 64, "Hello World", "hello|hi", '|', 64, 0);
 *   // -> "hi World"
 *
 * - Replace, case-insensitive (unbounded):
 *   llsREPLACE(t, 64, "Cat and CAT", "cat|dog", '|', 0);
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
 * @section DesignNotes Design Notes
 * Six principles guide this API:
 * - Symmetry:       Operation exists for every encoding direction
 *                   except where explicitly noted. For example,
 *                   Lating (l) <- Unicoce (u) ivariants are not provided
 *                   comparison search functions.
 * - Clarity:        Function names encode direction/type, bound, operation,
 *                   and case (sensitive/insensitive, preserving/lowercase/uppercase).
 * - Safety:         Explicit/default bounds, terminator validation,
 *                   representability checks, error checking.
 * - Predictability: Behavior mirrors familiar C string patterns while
 *                   making bounds, defined (instead of undefined or
 *                   implementation-defined) return values, error
 *                   values.
 * - Portability:    Fixed-width types, no wchar_t size assumptions.
 *                   No API-managed locale state. However, Unicode
 *                   classification and case conversion use C runtime
 *                   wide-character routines (isw* and tow*) via wchar_t
 *                   casts and, therefore, results are locale- and
 *                   CRT-dependent.
 * - Compatibility:  Usable in both C and C++ projects with compatibility
 *                   with C89/C90 (ANSI C) compilers. No C99/C11 features
 *                   are used.
 *
 * @warning Unicode classification and case conversion use the C runtime's
 * wide-character routines (isw*, tow*), which are locale- and CRT-dependent.
 * Results may differ across platforms or locale settings.
 *
 * @note This API does not perform Unicode normalization or surrogate pair
 * handling; all operations are on individual code units.
 *
 * @note For functions that return NULL on error, the output buffer is always
 * null-terminated on error if possible, to help prevent buffer overreads.
 *
 * @note Some search and replace functions (e.g., substring search, multi-pair
 * replace) may have worst-case O(n*m) complexity, where n is the length of the
 * input and m is the pattern or map size. Most other operations are O(n).
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
 * This header was reviewed and refined with assistance from Microsoft Copilot.
 *
 * @section EncodingAssumptions Encoding Assumptions
 * - lchar_t = uint8_t  (Latin-8, values 1-255, 0 reserved for null terminator)
 * - uchar_t = uint16_t (Unicode BMP, values 1-65535, 0 reserved for null terminator)
 * - byte_t = uint8_t  (raw byte, values x'00'-x'FF', no null terminator semantics)
 * - wchar_t width is not assumed; all casts include explicit bounds checks.
 * - No normalization, surrogate handling, or multi-code-unit processing.
 * - All operations act on individual characters only.
 *
 * @section GlossaryOfTerms Glossary
 * lchar_t
 *     Latin-8 character (uint8_t). Values 1-255.
 *     0 is reserved for null terminator.
 * 
 * uchar_t
 *     Unicode-16 character (uint16_t). Values 1-65535.
 *     0 is reserved for null terminator.
 * 
 * byte_t
 *     Raw byte (uint8_t). Values x'00'-x'FF'. No null terminator semantics.
 * 
 * lchar_t *
 *     Pointer to a null-terminated string (lchar_t *) of Latin characters (lchar_t).
 * 
 * uchar_t *
 *     Pointer to a null-terminated string (uchar_t *) of Unicode characters (uchar_t).
 * 
 * byte_t *
 *     Pointer to a string (*byte_t) of bytes (byte_t). A specific
 *     length must be provided for byte strings, as they do not
 *     have null terminator semantics.
 * 
 * LUB_MAX_LSTRLEN
 *     Maximum number of characters in a Latin string (lchar_t *).
 * 
 * LUB_MAX_USTRLEN
 *     Maximum number of characters in a Unicode string (uchar_t *).
 * 
 * LUB_MAX_BSTRLEN
 *     Maximum number of bytes in a byte string (*byte_t).
 * 
 * err_c
 *     Substitution char for out-of-range Unicode->Latin conversions.
 * 
 * sn
 *     Maximum source characters/bytes; for char strings, stops at null.
 * 
 * span length
 *     Number of leading characters in s1 that satisfy a function-specific condition.
 * 
 * tn
 *    Maximum target characters excluding null-terminator or
 *    target buffer byte length.
 * 
 * unterminated
 *     A character string that does not have a null terminator at or before
 *     the specified or default bound.
 */

#ifdef __cplusplus
}
#endif

// End of lubtype.h
