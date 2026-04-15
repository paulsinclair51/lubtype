/**
 * @file lubtype.h
 * @mainpage Latin/Unicode/Byte API
 * @brief This API header provides robust and portable Latin-8,
 *        Unicode-16, and byte types, macros, extern function
 *        declarations (protypes), and static inline function
 *        definitions. To include definitions of extern functions,
 *        #define LUB_DEFINITIONS before including this header.
 * 
 *        Functions for classification, character conversion,
 *        length, concatenate, copy, search, compare, mem compare,
 *        prefix, suffix, transform, replace, format, validation,
 *        split, span, character substitutiion, and count functions.
 * 
 *        - Variants for Latin/Unicode/Byte <- Latin/Unicode/Byte.
 *        - Variants for explicit/default bounds on source length.
 *        - Explicit bounds on target buffer when required.
 *        - Variants for character case-preserving and mapping to
 *          uppercase/lowercase characters.
 *        - Case-sensitive/insensitive character matching.
 * 
 * @note Not all variants are provided for all functions due a
 *       variant not being meaningful/useful for a particular function
 *       or the variant being easily implemented by the caller
 *       using a combination of provided variants.
 * 
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

/**
 * @section Versioning Versioning
 * @defgroup LUBVersioningMacros LUB Versioning Macros
 * @name LUB_VERSION_MAJOR, LUB_VERSION_MINOR, LUB_VERSION_PATCH,
 *       LUB_VERSION, LUB_VERSION_NUM, LUB_VERSION_HEX, 
 *       LUB_VERSION_EQ, LUB_VERSION_AT_LEAST
 * @brief Versioning macros for the LUB API (lubtype.h)
 *        LUB_VERSION_MAJOR
 *           Major version number, incremented for incompatible API changes.
 *           String form, e.g., "1" for major version 1.
 *        LUB_VERSION_MINOR
 *           Minor version number, incremented for backward-compatible additions.
 *           String form, e.g., "0" for minor version 0,
 *           or "22" for minor version 22.
 *        LUB_VERSION_PATCH
 *           Patch version number, incremented for bug fixes or internal improvements.
 *           String form, e.g., "0" for patch version 0,
 *           or "12" for patch version 12.
 *        LUB_VERSION
 *           String form, e.g., "1.0.0" for major version 1, minor version 0, patch version 0.
 *        LUB_VERSION_NUM
 *           Integer form MMmmpp for comparisons, e.g., 10000 for
 *           version 1.0.0, 10200 for version 1.2.0, or 11212 for version 1.12.12.
 *        LUB_VERSION_HEX
 *           Hexadecimal form 0xMMmmpp for display/debugging, e.g.,
 *           0x010000 for version 1.0.0, 0x010200 for version 1.2.0,
 *           or 0x011212 for version 1.12.12.
 *        LUB_VERSION_EQ(maj,min,pat)
 *           True if current version is exactly maj.min.pat.
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
#error "lubtype.h: One or more LUB_VERSION_* macros are already defined. " \
       "#undef before including lubtype.h. " \
       "After including, define again as needed if the LUB definition is " \
       "not required."
#endif
#if defined(__LUB_STRINGIFY2__) || defined(__LUB_STRINGIFY__) || \
    defined(__LUB_STATIC_ASSERT__)
#error "lubtype.h: One or more of __LUB_STRINGIFY2__, __LUB_STRINGIFY__, " \
       "or __LUB_STATIC_ASSERT__ macros are already defined. " \
       "#undef before including lubtype.h. " \
       "The macro is undefined after its last use " \
       "by the include. After including, define again as needed."
#endif

// Library version major, minor, patch.
#define LUB_VERSION_MAJOR 1
#define LUB_VERSION_MINOR 0
#define LUB_VERSION_PATCH 0

// Define version helper macros (undefined after versioning set).
#define __LUB_STRINGIFY2__(x) #x
#define __LUB_STRINGIFY__(x) __LUB_STRINGIFY2__(x)

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
#define LUB_STATIC_ASSERT(cond, msg) \
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

/**
 * @section Types Types
 * 
 * @defgroup LatinCharacterTypes Latin Character Types (1 byte)
 * @name lchar_t, lcchar_t
 * @brief Latin-8 character types
 *        Base type: uint8_t, Values: 0-255 (0x00-0xFF).
 * @{
 */
typedef uint8_t lchar_t;        // Modifiable
typedef const lchar_t lcchar_t; // Constant
/** @} */

/**
 * @defgroup UnicodeCharacterTypes Unicode Character Types (2 bytes)
 * @name uchar_t, ucchar_t
 * @brief 2-byte Unicode character types
 *        Base type: uint16_t, Values: 0-65535 (0x0000-0xFFFF).
 * @{
 */
typedef uint16_t uchar_t;       // Modifiable
typedef const uchar_t ucchar_t; // Constant
/** @} */

/**
 * @defgroup ByteTypes Byte Types
 * @name bchar_t, bcchar_t
 * @brief Byte types (1 byte)
 *        Base type: uint8_t, Values: 0-255 (0x00-0xFF).
 * @{
 */
typedef uint8_t bchar_t;        // Modifiable
typedef const bchar_t bcchar_t; // Constant
/** @} */

/**
 * @defgroup LatinStringTypes Latin String Types
 * @name lstr_t, lcstr_t
 * @brief Latin-8 string types
 *        Base type: array of lchar_t
 *        Null-terminated string with length bounded by MAX_LSTRLEN.
 */
typedef lchar_t *lstr_t;        // Modifiable
typedef const lchar_t *lcstr_t; // Constant
/** @} */

/**
 * @defgroup UnicodeStringTypes Unicode String Types
 * @name ustr_t, ucstr_t
 * @brief Unicode string types
 *        Base type: array of uchar_t
 *        Null-terminated string with length bounded by MAX_USTRLEN.
 */
typedef uchar_t *ustr_t;        // Modifiable
typedef const uchar_t *ucstr_t; // Constant
/** @} */

/**
 * @defgroup ByteStringTypes Byte String Types
 * @name bstr_t, bcstr_t
 * @brief Byte string type
 *        Base type: array of bchar_t
 *        Null-terminated string with length bounded by MAX_BSTRLEN.
 * @{
 */
typedef bchar_t *bstr_t;        // Modifiable
typedef const bchar_t *bcstr_t; // Constant
/** @} */

/**
 * @defgroup CharacterLimits Character Limits
 * @name LUB_MAX_LCHAR, LUB_MAX_UCHAR, LUB_MAX_BCHAR
 * @brief Maximum values for lchar_t, uchar_t, and bchar_t.
 * @{
 */
#if defined(LUB_MAX_LCHAR) || defined(LUB_MAX_UCHAR) || defined(LUB_MAX_BCHAR)
#error "lubtype.h: One or more character limit LUB_MAX_* macros are already defined. " \
       "#undef before including lubtype.h. " \
       "After including, define again as needed if the LUB definition is " \
       "not required."
#endif

#define LUB_MAX_LCHAR ((size_t)255)
#define LUB_MAX_UCHAR ((size_t)65535)
#define LUB_MAX_BCHAR ((size_t)255)
/** @} */

/**
 * @defgroup StringLimits String Limits
 * @name LUB_MAX_LSTRLEN, LUB_MAX_USTRLEN,
 *       LUB_MAX_UNAMELEN, LUB_MAX_UQNAMELEN,
 *       LUB_MAX_LOPTLEN, LUB_MAX_UOPTLEN,
 *       LUB_MAX_BSTRLEN, LUB_MAX_BOPTLEN
 * @brief Maximum string lengths for lstr_t/lcstr_t, ustr_t/ucstr_t,
 *        name, quoted name, st parameter, and
 *        bstr_t/bcstr_t.
 * @{
 */
#if defined(LUB_MAX_LSTRLEN) || defined(LUB_MAX_USTRLEN) || \
    defined(LUB_MAX_UNAMELEN) || defined(LUB_MAX_UQNAMELEN) || \
    defined(LUB_MAX_LOPTLEN) || defined(LUB_MAX_UOPTLEN) || \
    defined(LUB_MAX_BSTRLEN) || defined(LUB_MAX_BOPTLEN)
#error "lubtype.h: One or more string limit LUB_MAX_* macros are already defined. " \
       "#undef before including lubtype.h. " \
       "After including, define again as needed if the LUB definition is " \
       "not required."
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
 * @defgroup SpecialAndErrorReturnValues Special and Error Return Values
 * @name LUB_CMP_GREATER_THAN, LUB_CMP_EQUAL, LUB_CMP_LESS_THAN,
 *       LUB_MISSING_TERMINATOR, LUB_OPTRESERVED,
 *       LUB_OPTLEN_EXCEEDS_TN,
 *       LUB_SIZE_ERRORS, LUB_PTR_ERRORS, LUB_INT_ERRORS.
 * @brief Special and error returned values for size_t,
 *        pointer, and int returning functions.
 * @note Values are chosen to be distinct from any valid returned size_t,
 *       pointer, or int value.
 * @note Cast a special or error value to size_t for size_t returning functions, 
 *       to pointer type of function (or void *) for pointer returning
 *       functions, and int for int returning functions.
 * @note LUB_SIZE_ERRORS is a general error value
 *       that can be used to check for errors for a function that returns size_t.
 *       An error is indicated when:
 * 
 *           returned_value >= (size_t)LUB_SIZE_ERRORS
 * 
 * @note LUB_PTR_ERRORS is a general error value
 *       that can be used to check for errors for a function that returns a pointer.
 *       An error is indicated when:
 * 
 *            returned_value >= (void*)LUB_PTR_ERRORS
 * 
 * @note LUB_INT_ERRORS is a general error value
 *       that can be used to check for errors for a function that returns int.
 *       An error is indicated when:
 * 
 *           returned_value <= (int)LUB_INT_ERRORS
 * 
 * @note For pointer returning functions with a target parameter t, NULL is returned
 *       when t is NULL.
 * @note For int returning comparison functions, LUB_CMP_LESS_THAN (-1) is used to indicate less-than,
 *       LUB_CMP_EQUAL (0) for equal, and LUB_CMP_GREATER_THAN (1) for greater-than.
 * @{
 */

#if defined(LUB_CMP_GREATER_THAN) || defined(LUB_CMP_EQUAL) || defined(LUB_CMP_LESS_THAN) || \
    defined(LUB_QUOTEDNAME) || defined(LUB_UNQUOTEDNAME)
#error "lubtype.h: One or more special value LUB_CMP_*, LUB_QUOTEDNAME, " \
       "or LUB_UNQUOTEDNAME macros are already defined. " \
       "#undef before including lubtype.h. " \
       "After including, define again as needed if the LUB definition is " \
       "not required."
#endif
#if defined(LUB_MISSING_TERMINATOR) || defined(LUB_INVALID_OPT) || \
    defined(LUB_OPTLEN_EXCEEDS_TN) || defined(LUB_INVALID_NAME) || \
    defined(LUB_OVERLAP) || defined(LUB_TRUNCATION) || \
    defined(LUB_SIZE_ERRORS) || defined(LUB_PTR_ERRORS) || \
    defined(LUB_INT_ERRORS)
#error "lubtype.h: One or more error value LUB_* macros " \
       "are already defined. #undef before including lubtype.h. " \
       "After including, define again as needed if the LUB definition is " \
       "not required."
#endif

//Special values for comparison result.
#define LUB_CMP_GREATER_THAN       ((int)1)
#define LUB_CMP_EQUAL              ((int)0)
#define LUB_CMP_LESS_THAN          ((int)-1)
// Special values for names.
#define LUB_QUOTEDNAME             ((int)1)
#define LUB_UNQUOTEDNAME           ((int)0)

// Error values.
#define LUB_INT_ERRORS             ((int)-2)

#define LUB_MISSING_TERMINATOR     (-2)
#define LUB_INVALID_OPT            (-3)
#define LUB_OPTLEN_EXCEEDS_TN       (-4)
#define LUB_INVALID_NAME           (-5)
#define LUB_OVERLAP                (-6)
#define LUB_TRUNCATION             (-7)

#define LUB_SIZE_ERRORS            (size_t(-99))
#define LUB_PTR_ERRORS             ((void*)(-99))
/** @} */

/**
 * @section Functions Functions
 * @subsection FunctionNamingConventions Function Naming Conventions
 *   l = Latin-8 char/string
 *   u = Unicode-16 char/string
 *   b = Byte
 *   ll = Latin <- Latin,
 *   lu = Latin <- Unicode
 *   ul = Unicode <- Latin
 *   uu = Unicode <- Unicode
 *   lb = Latin <- byte
 *   ub = Unicode <- byte
 *   bl = byte <- Latin
 *   bu = byte <- Unicode
 *
 *   nn = bounded target buffer and bounded source(s) with explicit tn and sn parameters
 *        for target bound and source bound(s), respectively.
 *   n = bounded target or source(s) with explicit tn for a target bound or
 *       explicit sn parameter for the source bound(s) depending on the operation.
 *
 *   lowercase operation (no 'c') = case-sensitive character comparison or
 *                                  no case mapping
 *   uppercase operation (no 'C') = case-insensitive matching/comparison
 *   lowercase operation with 'c' = case-mapped to lowercase
 *   uppercase operation with 'C' = case-mapped to uppercase
 *
 *   cat/catc/CATC = concatenate to target (return pointer to target)
 *   cpy/cpyc/CPYC = copy to target (return pointer to target)
 *
 *   cmp/CMP = compare (return int <0, 0, >0)
 *   pfx/PFX = prefix compare (return int <0, 0, >0)
 *   sfx/SFX = suffix compare (return int <0, 0, >0)
 *
 *   chr/CHR/rchr/RCHR = character search (return pointer to match or NULL)
 *   str/STR/rstr/RSTR = substring search (return pointer to match or NULL)
 *
 *   Other operations: trim/rev/pad/replace/sub/split/printf pbrk/PBRK/rpbrk/RPBRK
 *   spn/SPN/cspn/CSPN subcnt/SUBCNT
 * 
 *   m = mth occurrence for search functions with explicit m parameter.
 *       See m parameter in following subsection.
 * 
 *   M = same as m with explicit m parameter for case-insensitive matching
 *       (uppercase operation).
 * 
 * @subsection ParameterNames Parameter Names

 * @brief Commonly used parameters for the functions in this API.
 *        Specific functions may use a subset of these parameters
 *        or additional parameters (see specific function declarations
 *        and definitions for details).
 * 
 * @param t target buffer (destination for operations such as
 *          concatenate/copy operations).
 * 
 * @param tn target buffer size.
 * 
 * @param s source string for operations such as concatenate/copy
 *          and string to search for search functions.
 * 
 * @param s1 left source string for compare functions.
 * 
 * @param s2 right source string for compare functions.
 * 
 * @param c character (for character search functions, or character to
 *          replace in replace functions).
 * 
 * @param sn source bound. sn itself is bounded by LUB_MAX_LSTRLEN, LUB_MAX_USTRLEN,
 *           or LUB_MAX_BSTRLEN based on the type of the source string
 *           and operation. If sn is omitted in a function declaration,
 *           the source is bounded by LUB_MAX_UQNAMELEN.
 *           based on the type of the source string. Note n is required if the
 *           source is a byte string (bstr_t).
 * @param st Pointer to a source truncated indicator string.
 *           Null indicates an error if truncation occurs (i.e., length of
 *           concatenated string would exceed tn).
 *           Otherwise, if truncation occurs:
 *            - If the first character in st is 'L' or 'l', truncate on the
 *              left with string st+1 added on the left.
 *            - If the first character in st is 'R' or 'r', truncate on the
 *              right with string st+1 added on  the right.
 *            - If the first character in st is 'C' or 'c', truncate in
 *              the center with string st+1 added in the center.
 *            - If the first character is not an alphabetic character,
 *              truncate on the right with string st added on the right.
 *            - Other alphabetic character indicate an error (reserved
 *              for future use).
 *           Note that st or st+1 may be empty.
 * @note q quote character (' or ""),
 *
 * @param m mth occurrence for matching.
 *          m > 0 counts from the beginning of the string (1 means first occurrence).
 *          m == 0 returns NULL for search functions.
 *          m == 0 means replace all occurrences for replace functions.
 *          m < 0 counts from the end of the string (-1 means last
 *          occurrence) when the operation supports reverse selection.
 * 
 * @param err_c For ul functions only, character to return when
 *              neither the Unicode source character nor its converted
 *              value (if uppercasing/lowercasing) is a valid Latin character.
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
 * @note islname1c and islanmec are not provided since names are Unicode.
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
    {return iswalnum((wchar_t)c);}
static inline int islalnum(const lchar_t c)
    {return isalnum((unsigned char)c);}
static inline int isuname1c(const uchar_t c)
    {return c <= LUB_MAX_LCHAR &&
       (islalpha((lchar_t)c) || (lchar_t)c == '_');
    }
static inline int isunamec(const uchar_t c)
    {return c <= LUB_MAX_LCHAR &&
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
 * @defgroup UppderLowerCaseConversion Upper/Lower Case Conversion
 * @name lltoupper, lutoupper, ultoupper, uutoupper
 *       lltolower, lutolower, ultolower, uutolower
 * @brief Character uppercase/lowercase conversion.
 * @param c Character to convert.
 * @param err_c For lutoupper/lutolower only.
 * @return Upper/lower case version of c if one defined,
 *         otherwise c.
 * @note For lutoupper/lutolower, if converted c is not a valid
 *       Latin chararacter and c is a valid Latin characer,
 *       return c, otherwise return err_c.
 * @{
 */
// Upper case conversion.
static inline lchar_t lltopper(const lchar_t c)
    {return (lchar_t)toupper((unsigned char)c);}
static inline lchar_t lutoupper(const uchar_t c, lchar_t err_c)
    {wint_t wc = towupper((wchar_t)c);
     return (wc < 0 || (size_t)wc > LUB_MAX_LCHAR) ?
            (c > LUB_MAX_LCHAR ? (lchar_t)c : err_c) : (lchar_t)wc;
    }
static inline uchar_t ultoupper(const lchar_t c)
    {wint_t wc = towupper((wchar_t)c);
     return (wc < 0 || (size_t)wc > LUB_MAX_UCHAR) ?
            (uchar_t)c : (uchar_t)wc;
    }
static inline uchar_t uutoupper(const uchar_t c)
    {wint_t wc = towupper((wchar_t)c);
     return (wc < 0 || (size_t)wc > LUB_MAX_UCHAR) ?
            (uchar_t)c : (uchar_t)wc;
    }

// Lower case conversion.
static inline lchar_t lltolower(const lchar_t c)
    {return (lchar_t)tolower((unsigned char)c);}
static inline lchar_t lutolower(const uchar_t c, lchar_t err_c)
    {wint_t wc = towlower((wchar_t)c);
     return (wc < 0 || (size_t)wc > LUB_MAX_LCHAR) ?
            (c > LUB_MAX_LCHAR ? (lchar_t)c : err_c) : (lchar_t)wc;
    }
static inline uchar_t ultolower(const lchar_t c)
    {return (uchar_t)tolower((unsigned char)c);}
static inline uchar_t uutolower(const uchar_t c)
    {wint_t wc = towlower((wchar_t)c);
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
 * @param n The maximum length of the string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return String length, 0 if s is NULL, or error value.
 * @note Errors:
 *       - LUB_MISSING_TERMINATOR
 * @{
 */

static inline size_t lcsnlen(lcstr_t s, size_t n) {
    size_t k = 0;
    if (!s) return k;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    for (; n && *s; s++, n--) k++;
    // Return error if null terminator not found at or before s[n].
    return *s ? (size_t)LUB_MISSING_TERMINATOR : k;
}
static inline size_t ucsnlen(ucstr_t s, size_t n) {
    size_t k = 0;
    if (!s) return k;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s; s++, n--) k++;
    // Return error if null terminator not found at or before s[n].
    return *s ? (size_t)LUB_MISSING_TERMINATOR : k;
}

/**
 * @defgroup StringClassification String Classification
 * @name isuRESERVED, isuQNAME (case-insensitive)
 *       isltruncstr,
 *       islnhexdigits, isunhexdigits
 * @brief Latin and Unicode string classification.
 * @param s Source string.
 * @param sn Maximum source string length (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 *           For isuRESERVED and isuQNAME, sn is omitted and sn defaults to MAX_UNAMELEN.
 *           For isltruncstr, sn is omitted and sn defaults to MAX_LOPTLEN.
 * @return 1 Condition satisfied.
 *         0 Condition unsatisfied and no error.
 *        <0 Error.
 * @note Errors:
 *       - LUB_MISSING_TERMINATOR when s is not null-terminated by the bound sn.
 *       - LUB_INVALID_NAME when s is not a valid name for isuQNAME.
 * @note RESERVED: Classify whether s is a Teradata reserved word.
 * @note QNAME: Classify whether s must be a quoted name.
 *              s must be quoted if the first character is not
 *              a first-name character (see iuname1c), or any subsequent character
 *              is not a name character (see iunamec).
 * @note isltruncstr: Classify whether s is a valid truncated string for use as value
 *                    for as an st parameter. A valid string is NULL,
 *                    null-terminated by the bound MAX_LOPTLEN,
 *                    an empty string, and the first character is 
 *                    not a reserved alphabetic character (see st parameter for details).
 * @note The two hexdigits functions classify whether s consists only of hex digit characters
 *       '0' to '9', 'A' to 'F', or 'a' to 'f'.
 * @{
 */

extern int isuRESERVED(ucstr_t s)
#if defined(LUB_DEFINITIONS)
{   if (!s || !*s) return (int)0;

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
    if (len >= LUB_SIZE_ERRORS || len < td_reserved_word_min_len)
      return (int)0;

    size_t lo = 0;
    size_t hi = td_reserved_words_count;
    while (lo < hi) {
      const size_t mid = lo + ((hi - lo) / 2);
      const char *kw = td_reserved_words[mid];
      ucstr_t ss = s;
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

extern int isuQNAME(ucstr_t s)
#if defined(LUB_DEFINITIONS)
{   size_t len = ucsnlen(s, MAX_UNAMELEN);
    if (len >= LUB_SIZE_ERRORS) return (int)len;
    if (len == 0) return (int)LUB_INVALID_NAME;

    // Check if s is all spaces.
    ucstr_t ss = s;
    size_t llen = len;
    for (; *ss; llen--, ss++) {
        if (*ss != ' ') break;
    }
    if (llen == 0) return (int)LUB_INVALID_NAME;

    if (!isuname1c(*s)) return (int)1;
    for (size_t i = 1; i < len; ++i)
        if (!isunamec(s[i])) return (int)1;

    if (isuRESERVED(s)) return (int)1;

    return (int)0;
}
#else
;
#endif

extern int isltruncstr(lcstr_t s)
#if defined(LUB_DEFINITIONS)
{   size_t len = lcsnlen(s, MAX_LOPTLEN);
    if (len >= LUB_SIZE_ERRORS) return (int)len;
    if (len == 0) return (int)1;
    int c = toupper((int)*s);
    return strchr("LRC", c) || !isalpha(c) ? (int)1 : (int)0;
}
#else
;
#endif

// Hex digit string classification.
extern int islnhexdigits(lcstr_t s, size_t sn)
#if defined(LUB_DEFINITIONS)
{   if (!s) return (int)0;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    for (; sn && *s; --sn, ++s) if (!islhexdigit(*s)) return (int)0;
    if (*s) return (int)0; // Missing null terminator.
    return (int)1;
}
#else
;
#endif

extern int isunhexdigits(ucstr_t s, size_t sn)
#if defined(LUB_DEFINITIONS)
{   if (!s) return (int)0;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    for (; sn && *s; --sn, ++s) if (!isuhexdigit(*s)) return (int)0;
    if (*s) return (int)0; // Missing null terminator.
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
 * @param tn tn is the maximum number of characters to copy to buffer t,
 *           not including the null terminator; tn is clamped to MAX_LSTRLEN or MAX_USTRLEN.
 * @param s Pointer to source string.
 * @param sn For a character source string, sn is the maximum length of the
 *           string. sn is clamped to MAX_LSTRLEN or MAX_USTRLEN.
 * 
 *           sn is omitted for uusncatname, uusncatnamec, and
 *           uusnCATNAMEC since the source is expected to be Unicode name
 *           and thus sn defaults to MAX_UNAMELEN (128 characters).
 * 
 *           For a copy of a byte string to a character string, sn 
 *           defines the length of the source string.
 * @param st Pointer to a source truncated indicator string.
 *           Null indicates an error if truncation occurs (i.e., length of
 *           concatenated string would exceed tn).
 *           Otherwise, if truncation occurs:
 *            - If the first character in st is 'L' or 'l', truncate on the
 *              left with st+1 added on the left.
 *            - If the first character in st is 'R' or 'r', truncate on the
 *              right with st+1 added on  the right.
 *            - If the first character in st is 'C' or 'c', truncate in
 *              the center with st+1 added in the center.
 *            - If the first character is not an alphabetic character,
 *              truncate on the right with st added on the right.
 *            - Other alphabetic character indicate an error (reserved
 *              for future use).
 *           Note that st+1 may be empty.
 * @param q Quote character for quoted string and quoted name concatenate functions.
 * @param err_c  Replacement for out-of-range Unicode characters
 *               (lus functions only).
 * @return t, or NULL if an error occurs.
 * 
 *         If t is not null and an error occurs, the target buffer
 *         is set to an empty string.
 * @note Errors
 *       - t is null.
 *       - tn is too large (greater than MAX_LSTRLEN or MAX_USTRLEN).
 *       - Source string needs to be truncated but st is null or length of st
 *         (not including optional left/right/center indicator) exceeds tn.
 *       - Unterminated character source string at or before s[sn].
 *       - Unexpected overlap when source and target have different types, or when quoted.
 * @note For lls and uus, concatenate is overlap-safe (target result is correct
 *       but source may be overridden if there is overlap).
 *       For a concatenate with target and source of different types or quoted,
 *       overlap is not allowed.
 * @{
 */

// Concatenate function and macro helpers.
#if defined(LUB_DEFINITIONS)

static void *parse_args(void *t, size_t t_len,
                       lcstr_t st,
                       char *trunc_action, lcstr **trunc, size_t *trunc_len)
{   if (!t) return (void *)NULL;
    if (t_len >= LUB_SIZE_ERRORS) {*t = (lchar_t)0; return (void *)t)t_len;}
    if (isltruncst(st) != 1) return (void *)LUB_INVALID_OPT;
    if (!st) return (lstr_t *)NULL;
    if (!st) {*trunc_action = '\0'; *trunc = (lstr_t)NULL; *trunc_len = 0;}
    else
      {size_t st_len = lcsnlen(st, tn + 1);
       if (st_len >= LUB_SIZE_ERRORS) return (void *)LUB_INVALID_OPT;
       if (isalpha((int)*st))
         {*trunc_action = *st; *trunc = st + 1; *trunc_len = st_len - 1;}
       else if (st_len > tn) return (void *)LUB_INVALID_OPT;
       else {*trunc_action = 'R'; *trunc = st; *trunc_len = st_len;}
      }
    return (void *)t;
}

#if defined(__lub_op_helper__)
#error "lubtype.h: __lub_op_helper__ macro is already defined. " \
       "#undef before including lubtype.h. The macro is undefined after its last use " \
       "by the include. After including, define again as needed."
#endif

#define __lub_op_helper__(t_vstr_t, t_vchar_t, t_maxlen, t_vscnlen, s_maxlen, s_vscnlen, vvtoc) \
     char trunc_action; lcstr_t trunc; size_t trunc_len; \
     if (tn > t_maxlen) tn = t_maxlen; \
     const size_t t_len = t_vscnlen(t, tn); \
     t_vstr_t tt = (t_vstr_t)parse_args((void *)t, t_len, st, &trunc_action, &trunc, &trunc_len); \
     if (!t || tt > (t_vstr_t)LUB_PTR_ERRORS) return (t_vstr_t)tt; \
     if (!s) return t; \
     if (sn > s_maxlen) sn = s_maxlen; \
     size_t s_len = s_vscnlen(s, sn); \
     if (s_len >= LUB_SIZE_ERRORS) return (t_vstr_t)s_len; \
     /* Check for truncation, accounting for current target length. */ \
    if (t_len + s_len > tn)
      {if (!trunc) return (t_vstr_t)LUB_TRUNCATION; \
         if (trunc_len >= tn) return (t_vstr_t)LUB_INVALID_OPT; \
         if (trunc_len + t_len > tn) return (t_vstr_t)LUB_TRUNCATION; \
         switch (trunc_action) { \
            case 'L': case 'l': \
              /* Truncate on the left. Move existing target chars right and add trunc on the left. */ \
              for (size_t i = t_len; i > 0; --i) t[i + trunc_len - 1] = t[i - 1]; \
              for (size_t i = 0; i < trunc_len; ++i) t[i] = trunc[i]; \
              break; \
            case 'C': case 'c': \
              /* Truncate in the center. Move existing target chars right and add trunc in the center. */ \
              {size_t half_trunc = trunc_len / 2; \
                size_t half_t = t_len / 2; \
                size_t half_s = s_len / 2; \
                size_t center_t = half_t + (t_len % 2); \
                size_t center_s = half_s + (s_len % 2); \
                for (size_t i = half_t; i > 0; --i) t[i + half_trunc - 1] = t[i - 1]; \
                for (size_t i = 0; i < half_trunc; ++i) t[i] = trunc[i]; \
                for (size_t i = center_t, j = center_s; j < s_len; ++i, ++j) t[i + half_trunc] = s[j]; \
                s_len = center_s; /* Adjust s_len to reflect truncated source length. */ \
              } \
              break; \
            case 'R': case 'r': default: \
              /* Truncate on the right. Add trunc on the right after existing target chars. */ \
              for (size_t i = 0; i < trunc_len; ++i) t[t_len + i] = trunc[i]; \
              break; \
         } \
         tn -= trunc_len; \
         if (s_len > tn - t_len) s_len = tn - t_len;\
         /* Note that if st is not null, then trunc is not null and thus */ \
         /* trunc_len is greater than 0. */ \
      } \
     /* Overlap-safe: check for overlap and copy direction. */ \
     if (t <= s || t > s + sn) { \
         /* Safe to concatenate left-to-right, even with overlap. */ \
         tt = t; /* Save start of concatenate for null terminator on error. */ \
         for (; sn && *s; sn--, t++, s++) *t = vvtoc; \
         /* Error if source string is not null-terminated at or before s[sn]. */ \
         if (*s) {*tt = (t_vchar_t)0; return (t_vstr_t)NULL;} \
         *t = (t_vchar_t)0; /* Null-terminate the result. */ \
     } else { \
         /* Potentially unsafe left-to-right concatenate. */ \
         /* Concatenate right-to-left for safe overlap handling. */ \
         sn = s_len; \
         sn++; /* Include null terminator in concatenate. */ \
         tt = t = t + sn; \
         s = s + sn; \
         if (t != s) for (; sn; sn--, tt--, s--) *tt = vvtoc; \
     } \
     return t;

static inline lchar_t lutoc (uchar_t c, lchar_t err_c)
     {return c > LUB_MAX_LCHAR ? err_c : (lchar_t)c;}

#endif // LUB_DEFINITIONS

// Concatenate (case-preserving)
extern lstr_t llsnncat(lstr_t t, size_t tn,  lstr_t s, size_t sn, const lcstr_t st)
    #if defined(LUB_DEFINITIONS)
    {__lub_op_helper__(lstr_t, lchar_t, MAX_LSTRLEN, lcsnlen, MAX_LSTRLEN, lcsnlen, *s)}
    #else
    ;
    #endif // LUB_DEFINITIONS
extern lstr_t lusnncat(lstr_t t, size_t tn, ustr_t s, size_t sn, const lcstr_t st,
                      const lchar_t err_c)
    #if defined(LUB_DEFINITIONS)
    {__lub_op_helper__(lstr_t, lchar_t, MAX_USTRLEN, ucsnlen, MAX_USTRLEN, ucsnlen, lutoc(*s, err_c))}
    #else
    ;
    #endif // LUB_DEFINITIONS
extern ustr_t ulsnncat(ustr_t t, size_t tn, lstr_t s, size_t sn, const lcstr_t st)
    #if defined(LUB_DEFINITIONS)
    {__lub_op_helper__(ustr_t, uchar_t, MAX_USTRLEN, lcsnlen, MAX_USTRLEN, lcsnlen, (uchar_t)*s)}
    #else
    ;
    #endif // LUB_DEFINITIONS
extern ustr_t uusnncat(ustr_t t, size_t tn, ustr_t s, size_t sn, const lcstr_t st)
    #if defined(LUB_DEFINITIONS)
    {__lub_op_helper__(ustr_t, uchar_t, MAX_USTRLEN, ucsnlen, MAX_USTRLEN, ucsnlen, *s) }
    #else
    ;
    #endif // LUB_DEFINITIONS

// Concatenate (lowercase).
extern lstr_t llsnncatc(lstr_t t, size_t tn, lstr_t s, size_t sn,
                        lcstr_t st)
    #if defined(LUB_DEFINITIONS)
    {__lub_op_helper__(lstr_t, lchar_t, MAX_LSTRLEN, lcsnlen, MAX_LSTRLEN, lcsnlen, lltolower(*s)) }
    #else
    ;
    #endif // LUB_DEFINITIONS
extern lstr_t lusnncatc(lstr_t t, size_t tn, ustr_t s, size_t sn,
                        lcstr_t st, lcchar_t err_c)
    #if defined(LUB_DEFINITIONS)
    {__lub_op_helper__(lstr_t, lchar_t, MAX_USTRLEN, ucsnlen, MAX_USTRLEN, ucsnlen, lutolower(*s, err_c)) }
    #else
    ;
    #endif // LUB_DEFINITIONS
extern ustr_t ulsnncatc(ustr_t t, size_t tn, lstr_t s, size_t sn,
                        lcstr_t st)
    #if defined(LUB_DEFINITIONS)
    {__lub_op_helper__(ustr_t, uchar_t, MAX_USTRLEN, lcsnlen, MAX_USTRLEN, lcsnlen, ultolower(*s)) }
    #else
    ;
    #endif // LUB_DEFINITIONS
extern ustr_t uusnncatc(ustr_t t, size_t tn, ustr_t s, size_t sn,
                      lcstr_t st)
    #if defined(LUB_DEFINITIONS)
    {__lub_op_helper__(ustr_t, uchar_t, MAX_USTRLEN, ucsnlen, MAX_USTRLEN, ucsnlen, uutolower(*s)) }
    #else
    ;
    #endif // LUB_DEFINITIONS

// Concatenate (uppercase).
extern lstr_t llsnnCATC(lstr_t t, size_t tn, lstr_t s, size_t sn,
                        lcstr_t st)
    #if defined(LUB_DEFINITIONS)
    {__lub_op_helper__(lstr_t, lchar_t, MAX_LSTRLEN, lcsnlen, MAX_LSTRLEN, lcsnlen,
                       lltoupper(*s)) }
    #else
    ;
    #endif // LUB_DEFINITIONS
extern lstr_t lusnnCATC(lstr_t t, size_t tn, ustr_t s, size_t sn,
                       lcstr_t st, lcchar_t err_c)
    #if defined(LUB_DEFINITIONS)
    {__lub_op_helper__(lstr_t, lchar_t, MAX_USTRLEN, ucsnlen, MAX_USTRLEN, ucsnlen,
                       lutoupper(*s, err_c)) }
    #else
    ;
    #endif // LUB_DEFINITIONS
extern ustr_t ulsnnCATC(ustr_t t, size_t tn, lstr_t s, size_t sn,
                        lcstr_t st)
    #if defined(LUB_DEFINITIONS)
     {__lub_op_helper__(ustr_t, uchar_t, MAX_USTRLEN, lcsnlen, MAX_USTRLEN, lcsnlen,
                        ultoupper(*s)) }
    #else
    ;
    #endif // LUB_DEFINITIONS
extern ustr_t uusnnCATC(ustr_t t, size_t tn, ucstr_t s, size_t sn,
                        lcstr_t st)
    #if defined(LUB_DEFINITIONS)
     {__lub_op_helper__(ustr_t, uchar_t, MAX_USTRLEN, ucsnlen, MAX_USTRLEN, ucsnlen,
                        uutoupper(*s, err_c)) }
    #else
    ;
    #endif // LUB_DEFINITIONS

// Concatenate quoted macro helper.
#if defined(LUB_DEFINITIONS)
#undef __lub_op_helper__
#define __lub_op_helper__(t_vstr_t, t_vchar_t, s_vchar_t, s_maxlen, s_vcslen, vvtoc) \
        if (!t || (q != '\'' && q != '"')) return (t_vstr_t)NULL; \
        /* TBD - check for truncation and st fit in tn if st is not NULL. */ \
        size_t len = s_vcslen(s, s_maxlen); \
        if (len >= LUB_SIZE_ERRORS) return (t_vstr_t)len; \
        if (!len) return (t_vstr_t)LUB_TRUNCATION; \
        *t++ = (t_vchar_t)q; \
        for (; len; len--, t++, s++) { \
            s_vchar_t c = *s; \
            if (c == (s_vchar_t)q) *t++ = (t_vchar_t)q; \
            *t = vvtoc; \
        } \
        *t++ = (t_vchar_t)q; \
        *t = (t_vchar_t)0; \
        return t;
#endif // LUB_DEFINITIONS

// Concatenate quoted (case-preserving).
extern lstr_t llsnncatq(lstr_t t, size_t tn, lcstr_t s, size_t sn,
                        lcstr_t st, lchar_t q)
    #if defined(LUB_DEFINITIONS)
    {return __lub_op_helper__(lstr_t, lchar_t, lchar_t, MAX_LSTRLEN, lcsnlen, *s); }
    #else
    ;
    #endif // LUB_DEFINITIONS
extern lstr_t lusnncatq(lstr_t t, size_t tn, ucstr_t s, size_t sn,
                        lcstr_t st, lchar_t q, const lchar_t err_c)
    #if defined(LUB_DEFINITIONS)
    {return __lub_op_helper__(lstr_t, lchar_t, uchar_t, MAX_USTRLEN, ucsnlen, lutoc(*s, err_c)); }
    #else
    ;
    #endif // LUB_DEFINITIONS
extern ustr_t ulsnncatq(ustr_t t, size_t tn, lcstr_t s, size_t sn,
                        lcstr_t st, lchar_t q)
    #if defined(LUB_DEFINITIONS)
    {return __lub_op_helper__(ustr_t, uchar_t, lchar_t, MAX_USTRLEN, lcsnlen, (uchar_t)*s); }
    #else
    ;
    #endif // LUB_DEFINITIONS
extern ustr_t uusnncatq(ustr_t t, size_t tn, ucstr_t s, size_t sn,
                        lcstr_t st, lchar_t q)
    #if defined(LUB_DEFINITIONS)
    {return __lub_op_helper__(ustr_t, uchar_t, uchar_t, MAX_USTRLEN, ucsnlen, *s);  }
    #else
    ;
    #endif // LUB_DEFINITIONS

// Concatenate quoted (lowercase).
extern lstr_t llsnncatqc(lstr_t t, size_t tn, lcstr_t s, size_t sn,
                         lcstr_t st, lchar_t q)
    #if defined(LUB_DEFINITIONS)
    {return __lub_op_helper__(lstr_t, lchar_t, lchar_t, MAX_LSTRLEN, lcsnlen, lltolower(*s)); }
    #else
    ;
    #endif // LUB_DEFINITIONS
extern lstr_t lusnncatqc(lstr_t t, size_t tn, ucstr_t s, size_t sn,
                         lcstr_t st, lchar_t q, const lchar_t err_c)
    #if defined(LUB_DEFINITIONS)
    {return __lub_op_helper__(lstr_t, lchar_t, uchar_t, MAX_USTRLEN, ucsnlen, lutolower(*s, err_c)); }
    #else
    ;
    #endif // LUB_DEFINITIONS
extern ustr_t ulsnncatqc(ustr_t t, size_t tn, lcstr_t s, size_t sn,
                         lcstr_t st, lchar_t q)
    #if defined(LUB_DEFINITIONS)
    {return __lub_op_helper__(ustr_t, uchar_t, lchar_t, MAX_USTRLEN, lcsnlen, lutolower(*s)); }
    #else
    ;
    #endif // LUB_DEFINITIONS
extern ustr_t uusnncatqc(ustr_t t, size_t tn, ucstr_t s, size_t sn,
                         lcstr_t st, lchar_t q)
    #if defined(LUB_DEFINITIONS)
    {return __lub_op_helper__(ustr_t, uchar_t, uchar_t, MAX_USTRLEN, ucsnlen, uutolower(*s)); }
    #else
    ;
    #endif // LUB_DEFINITIONS

// Concatenate quoted (uppercase).
extern lstr_t llsnnCATQC(lstr_t t, size_t tn, lcstr_t s, size_t sn,
                         lcstr_t st, lchar_t q)
    #if defined(LUB_DEFINITIONS)
    {return __lub_op_helper__(lstr_t, lchar_t, lchar_t, MAX_LSTRLEN, lcsnlen, lltoupper(*s));}
    #else
    ;
    #endif // LUB_DEFINITIONS
extern lstr_t lusnnCATQC(lstr_t t, size_t tn, ucstr_t s, size_t sn,
                         lcstr_t st, lchar_t q, const lchar_t err_c)
    #if defined(LUB_DEFINITIONS)
    {return __lub_op_helper__(lstr_t, lchar_t, uchar_t, MAX_USTRLEN, ucsnlen, lutoupper(*s, err_c));}
    #else
    ;
    #endif // LUB_DEFINITIONS
extern ustr_t ulsnnCATQC(ustr_t t, size_t tn, lcstr_t s, size_t sn,
                         lcstr_t st, lchar_t q)
    #if defined(LUB_DEFINITIONS)
    {return __lub_op_helper__(ustr_t, uchar_t, lchar_t, MAX_USTRLEN, lcsnlen, ultoupper(*s)); }
    #else
    ;
    #endif // LUB_DEFINITIONS
extern ustr_t uusnnCATQC(ustr_t t, size_t tn, ucstr_t s, size_t sn,
                         lcstr_t st, lchar_t q)
    #if defined(LUB_DEFINITIONS)
    {return __lub_op_helper__(ustr_t, uchar_t, uchar_t, MAX_USTRLEN, ucsnlen, uutoupper(*s)); }
    #else
    ;
    #endif // LUB_DEFINITIONS

#if defined(LUB_DEFINITIONS)
#undef __lub_op_helper__
#endif // LUB_DEFINITIONS

// Concatenate name function helper.
#if defined(LUB_DEFINITIONS)
static ustr_t __uuscatname__(ustr_t t, size_t tn, ucstr_t s,
                             lchar_t q, lcstr_t st, const uint8_t Case)
{   const size_t MAX_STLEN = 10; // Limit for st string length.
    if (!t) return (ustr_t)NULL;
    *t = (uchar_t)0; // Null-terminate the result in case of error.
    if (q != '\'' && q != '"') return (ustr_t)NULL;
    int quotes_required = isuQNAME(s);
    if (quotes_required < 0) return (ustr_t)NULL;
    if (quotes_required) tn -= 2; // Account for required quotes..
    size_t st_len = lcsnlen(st, MAX_STLEN);
    if (st_len < 0 || st_len > MAXSTRLEN ||
        st_len > tn) return (ustr_t)NULL; // s truncated string too long.
    size_t s_len = ucslen(s);
    if (quotes_required) {
      // Check for truncation of quoted name.
      size_t q_cnt = uuscnt(s, (uchar_t)q); // Count quote occurrences.
      if (s_len + q_cnt + 2 > tn) {
        if (!st_len) return (ustr_t)NULL; // No st string to indicate truncation.
        s_len = tn - st_len - q_cnt; // Truncate s to fit st in tn.
        // Avoid truncating in the middle of a doubled quote.
        if (s[s_len - 1] == (uchar_t)q) s_len--; 
      } else {
        st_len = 0; // No truncation, so st is not needed.
      }
      *t++ = (uchar_t)q;  // Opening quote.
    } else {
      // Quotes not required. Check for truncation of unquoted name.
      if (s_len > tn) {
        if (!st_len) return (ustr_t)NULL; // No st string to indicate truncation.
        s_len = tn - st_len; // Truncate s to fit st in tn.
      } else {
        st_len = 0; // No truncation, so st is not needed.
      }
    }
    if (Case < 0)
      for (; s_len; --s_len, t++, s++) {
        if (*s == (uchar_t)q) *t++ = *s;
        *t = uutolower(*s);
      }
    else if (Case > 0)
      for (; s_len; --s_len, t++, s++) {
        if (*s == (uchar_t)q) *t++ = *s;
        *t = uutoupper(*s);
       }
    else
      for (; s_len; --s_len, t++, s++) {
        if (*s == (uchar_t)q) *t++ = *s;
        *t = *s;
      }
    // Concatenate st if there was truncation (st may have 0 length).
    for (; st_len; --st_len, t++, st++) *t = (uchar_t)*st;
    if (quotes_required) *t++ = (uchar_t)q; // Closing quote.
    *t = (uchar_t)0; // Null-terminate the result.
}
#endif // LUB_DEFINITIONS for concatenate name function helper.

// Concatenate name (case-preserving/lowercase/uppercase).
extern ustr_t uusncatname(ustr_t t, size_t tn, ucstr_t s, lchar_t q, lcstr_t st)
#if defined(LUB_DEFINITIONS)
    {return __uuscatname__(t, tn, s, q, st, 0);}
#else
    ;
#endif // LUB_DEFINITIONS
extern ustr_t uusncatnamec(ustr_t t, size_t tn, ucstr_t s, lchar_t q, lcstr_t st)
#if defined(LUB_DEFINITIONS)
    {return __uuscatname__(t, tn, s, q, st, -1);}
#else
    ;
#endif // LUB_DEFINITIONS
extern ustr_t uusnCATNAMEC(ustr_t t, size_t tn, ucstr_t s, lchar_t q, lcstr_t st)
#if defined(LUB_DEFINITIONS)
    {return __uuscatname__(t, tn, s, q, st, 1);}
#else
    ;
#endif // LUB_DEFINITIONS

// Concatenate hex digit characters <- bytes function helpers.
#if defined(LUB_DEFINITIONS)
static lchar_t __lbtohexdigit__(const byte_t b, int shift, const uint8_t Case)
{   if (shift) shift = 4;
    b = (b >> shift) & 0xF;
    return b < 10 ?
          (lchar_t)(b + '0') :
          (lchar_t)(b - 10 + (Case < 0 ? 'a' : 'A'));
}
// TBD: combine the next two functions as single a macro.
static lstr_t __lbsnncatc__(lstr_t t, size_t tn, bcstr_t s, size_t sn,
                            lcstr_t st, const uint8_t Case)
{  if (!t) return (lstr_t)NULL;
    if (sn > MAX_LSTRLEN >> 1) sn = MAX_LSTRLEN >> 1;
    if (sn > MAX_BSTRLEN) sn = MAX_BSTRLEN;
    for (; sn; sn--) {
      // Note: if s is NULL, treat as sn x'00' bytes.
      const bchar_t c = s ? *s++ : (bchar_t)0;
      *t++ = __lbtohexdigit__(c, 4, Case);
      *t++ = __lbtohexdigit__(c, 0, Case);
    }
    *t = (lchar_t)0;
    return t;
}
static ustr_t __ubsnncatc__(ustr_t t, size_t tn, bcstr_t s, size_t sn,
                            lcstr_t st, const uint8_t Case)
{   if (!t) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN >> 1) sn = MAX_USTRLEN >> 1;
    if (sn > MAX_BSTRLEN) sn = MAX_BSTRLEN;
    for (; sn; sn--) {
      // Note: if s is NULL, treat as sn x'00' bytes.
      const bchar_t c = s ? *s++ : (bchar_t)0;
      *t++ = (uchar_t)__lbtohexdigit__(c, 4, Case);
      *t++ = (uchar_t)__lbtohexdigit__(c, 0, Case);
    }
    *t = (uchar_t)0;
    return t;
}
#endif // LUB_DEFINITIONS.

// Concatenate hex digit characters <- bytes (lowercase).
extern lstr_t lbsnncatc(lstr_t t, size_t tn, bcstr_t s, size_t sn,
                               lcstr_t st)
#if defined(LUB_DEFINITIONS)
    {return __lbsnncatc__(t, tn, s, sn, st, -1);}
#else
    ;
#endif // LUB_DEFINITIONS
extern ustr_t ubsnncatc(ustr_t t, size_t tn, bcstr_t s, size_t sn,
                              lcstr_t st)
#if defined(LUB_DEFINITIONS)
    {return __ubsnncatc__(t, tn, s, sn, st, -1);}
#else
    ;
#endif // LUB_DEFINITIONS

// Concatenate hex digit characters <- bytes (uppercase).
extern lstr_t lbsnnCATC(lstr_t t, size_t tn, bcstr_t s, size_t sn,
                               lcstr_t st)
#if defined(LUB_DEFINITIONS)
    {return __lbsnncatc__(t, tn, s, sn, st, 1);}
#else
    ;
#endif // LUB_DEFINITIONS
extern ustr_t ubsnnCATC(ustr_t t, size_t tn, bcstr_t s, size_t sn,
                               lcstr_t st)
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
 *           not including the null terminator; tn is clamped to MAX_LSTRLEN or MAX_USTRLEN.
 * 
 *           For bbsnncpy, tn is the number of bytes to copy the target
 *           buffer. The target buffer must be at least tn bytes long.
 * @param s Pointer to source string.
 * @param sn For a character source string, sn is the maximum length of the
 *           string. sn is clamped to MAX_LSTRLEN or MAX_USTRLEN.
 * 
 *           For uusncpyname, uusncpynamec, and uusnCPYNAMEC, sn is omitted
 *           since the source must be a valid name and
 *           thus sn defaults to MAX_UNAMELEN (128 characters).
 * 
 *           For a copy of a byte string, sn is the length of the source
 *           string. Error if sn is greater than MAX_BSTRLEN.
 * @param q Quote character for quoted string and quoted name copy functions.
 * @param st Pointer to a source truncated indicator string.
 *           Null indicates an error if source string would be truncated.
 *           Otherwise, source is truncated:
 *            - If the first character in st is 'L', truncate on the left with st+1 added on
 *              the left.
 *            - If the first character in st is 'R', truncate on the right with st+1 added on
 *              the right.
 *            - If the first character in st is 'C', truncate in
 *              the center with st+1 added in the center.
 *            - Otherwise, truncate on the right with st added on the right.
 *           Note that st+1 may be empty.
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
 *       - For a byte target, tn exceeds MAX_BSTRLEN.
 *       - Unexpected overlap when source and target have different types, or when quoted.
 *       - Length of st string exceeds 31 or tn if st is not NULL.
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

// Copy (case-preserving).
static inline lstr_t llsnncpy(lstr_t t, size_t tn, lcstr_t s, size_t sn,
                              lcstr_t st)
    {if (!t) *t = (uchar_t)0;
     return llsnncat(t, s, n) ? t : (lstr_t)NULL;}
static inline lstr_t lusnncpy(lstr_t t, size_t tn, ustr_t s, size_t sn,
                              lcstr_t st, const lchar_t err_c)
     {if (!t) *t = (uchar_t)0;
     return lusnncat(t, s, n, err_c) ? t : (lstr_t)NULL;}
static inline ustr_t ulsnncpy(ustr_t t, lstr_t s, size_t n)
    {if (!t) *t = (uchar_t)0;
     return ulsnncat(t, s, n) ? t : (ustr_t)NULL;}
static inline ustr_t uusnncpy(ustr_t t, ustr_t s, size_t n)
    {if (!t) *t = (uchar_t)0;
     return uusnncat(t, s, n) ? t : (ustr_t)NULL;}

// Copy (lowercase).
static inline lstr_t llsnncpyc(lstr_t t, size_t tn, lcstr_t s, size_t sn,
                              lcstr_t st)
    {if (!t) *t = (uchar_t)0;
     return llsnncatc(t, tn, s, sn, st) ? t : (lstr_t)NULL;}
static inline lstr_t lusnncpyc(lstr_t t, size_t tn, ustr_t s, size_t sn,
                              lcstr_t st, const lchar_t err_c)
    {if (!t) *t = (uchar_t)0;
     return lusnncatc(t, tn, s, sn, st, err_c) ? t : (lstr_t)NULL;}
static inline ustr_t ulsnncpyc(ustr_t t, size_t tn, lstr_t s, size_t sn,
                              lcstr_t st)
    {if (!t) *t = (uchar_t)0;
     return ulsnncatc(t, tn, s, sn, st) ? t : (ustr_t)NULL;}
static inline ustr_t uusnncpyc(ustr_t t, size_t tn, ustr_t s, size_t sn,
                              lcstr_t st)
    {if (!t) *t = (uchar_t)0;
     return uusnncatc(t, tn, s, sn, st) ? t : (ustr_t)NULL;}

// Copy (uppercase).
static inline lstr_t llsnnCPYC(lstr_t t, size_t tn, lcstr_t s, size_t sn,
                              lcstr_t st)
    {return llsnnCATC(t, tn, s, sn, st) ? t : (lstr_t)NULL;}
static inline lstr_t lusnnCPYC(lstr_t t, size_t tn, ustr_t s, size_t sn,
                              lcstr_t st, const lchar_t err_c)
    {return lusnnCATC(t, tn, s, sn, st, err_c) ? t : (lstr_t)NULL;}
static inline ustr_t ulsnnCPYC(ustr_t t, size_t tn, lcstr_t s, size_t sn,
                              lcstr_t st)
    {return ulsnnCATC(t, tn, s, sn, st) ? t : (ustr_t)NULL;}
static inline ustr_t uusnnCPYC(ustr_t t, size_t tn, ustr_t s, size_t sn,
                              lcstr_t st)
    {return uusnnCATC(t, tn, s, sn, st) ? t : (ustr_t)NULL;}

// Copy quoted (case-preserving).
static inline lstr_t llsnncpyq(lstr_t t, size_t tn, lcstr_t s, size_t n,
                              lcstr_t st, lcchar_t q)
    {return llsnnCATQ(t, tn, s, sn, st, q) ? t : (lstr_t)NULL;}
static inline lstr_t lusnncpyq(lstr_t t, size_t tn, ucstr_t s, size_t n,
                              lcstr_t st, lcchar_t q)
    {return lusnnCATQ(t, tn, s, sn, st, q) ? t : (lstr_t)NULL;}
static inline ustr_t ulsnncpyq(ustr_t t, size_t tn, lcstr_t s, size_t n,
                              lcstr_t st, lcchar_t q)
    {return ulsnnCATQ(t, tn, s, sn, st, q) ? t : (ustr_t)NULL;}
static inline ustr_t uusnncpyq(ustr_t t, size_t tn, ucstr_t s, size_t n,
                              lcstr_t st, lcchar_t q)
    {return uusnnCATQ(t, tn, s, sn, st, q) ? t : (ustr_t)NULL;}

// Copy quoted (lowercase).
static inline lstr_t llsnncpyqc(lstr_t t, size_t tn, lcstr_t s, size_t n,
                              lcstr_t st, lcchar_t q)
    {if (!t) *t = (uchar_t)0;
     return llsnncatqc(t, tn, s, sn, st, q) ? t : (lstr_t)NULL;}
static inline lstr_t lusnncpyqc(lstr_t t, size_t tn, ucstr_t s, size_t n,
                              lcstr_t st, lcchar_t q, const lchar_t err_c)
    {if (!t) *t = (uchar_t)0;
     return lusnncatqc(t, tn, s, sn, st, q, err_c) ? t : (lstr_t)NULL;}
static inline ustr_t ulsnncpyqc(ustr_t t, size_t tn, lcstr_t s, size_t n,
                              lcstr_t st, lcchar_t q)
    {if (!t) *t = (uchar_t)0;
     return ulsnncatqc(t, tn, s, sn, st, q) ? t : (ustr_t)NULL;}
static inline ustr_t uusnncpyqc(ustr_t t, size_t tn, ucstr_t s, size_t n,
                                lcstr_t st, lcchar_t q)
    {if (!t) *t = (uchar_t)0;
     return uusnncatqc(t, tn, s, sn, st, q) ? t : (ustr_t)NULL;}

// Copy quoted/unquoted name (case-preserving).
static inline ustr_t uusncpyname(ustr_t t, size_t tn, ucstr_t s,
                                 lcstr_t st, lcchar_t q)
    {if (!t) *t = (uchar_t)0;
     return uusncatname(t, tn, s, st, q) ? t : (ustr_t)NULL;}

// Copy quoted/unquoted name (lowercase).
static inline ustr_t uusncpynamec(ustr_t t, size_t tn, ucstr_t s,
                                  lcstr_t st, lcchar_t q)
    {if (!t) *t = (uchar_t)0;
     return uusncatnamec(t, tn, s, st, q) ? t : (ustr_t)NULL;}

// Copy quoted/unquoted name (uppercase).
static inline ustr_t uusnCPYNAMEC(ustr_t t, size_t tn, ucstr_t s,
                                  lcstr_t st, lcchar_t q)
    {if (!t) *t = (uchar_t)0;
     return uusnCATNAMEC(t, tn, s, st, q) ? t : (ustr_t)NULL;}

// Copy hex digit characters <- bytes (lowercase).
static inline lstr_t lbsnncpyc(lstr_t t, bcstr_t s, size_t n, lcstr_t st)
    {if (!t) *t = (uchar_t)0;
     return lbsnnCATC(t, tn, s, sn, st) ? t : (lstr_t)NULL;}
static inline ustr_t ubsnncpyc(ustr_t t, bcstr_t s, size_t n, lcstr_t st)
    {if (!t) *t = (uchar_t)0;
     return ubsnnCATC(t, tn, s, sn, st) ? t : (ustr_t)NULL;}

// Copy hex digit characters <- bytes (uppercase).,
static inline lstr_t lbsnnCPYC(lstr_t t, bcstr_t s, size_t n, lcstr_t st)
    {if (!t) *t = (uchar_t)0;
     return lbsnnCATC(t, tn, s, sn, st) ? t : (lstr_t) NULL;}
static inline ustr_t ubsnnCPYC(ustr_t t, bcstr_t s, size_t n, lcstr_t st)
    {if (!t) *t = (uchar_t)0;
     return ubsnnCATC(t, tn, s, sn, st) ? t : (ustr_t)NULL;}

// Copy bytes <- hex digit characters.
extern bstr_t blsnncpy(bstr_t t, size_t tn, lcstr_t s, size_t sn,
                              lcstr_t st)
#if defined(LUB_DEFINITIONS)
{   if (!t || !s) return t;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    int hi, lo;
    for (; sn && *s; sn--, s++) {
        hi = ilhexdigit(*s);
        if (sn && s[1]) {lo = ilhexdigit(*++s); sn--;} else lo = 0;
        if (hi < 0 || lo < 0) return (bstr_t)0; // Invalid hex digit.
        *t++ = (unsigned char)((hi << 4) | lo);
    }
    if (*s) return (bstr_t)0; // Missing null terminator for source string.
    return t;
}
#else
    ;
#endif // LUB_DEFINITIONS
extern bstr_t busnncpy(bstr_t t, size_t tn, ucstr_t s, size_t sn,
                     lcstr_t st)
#if defined(LUB_DEFINITIONS)
{   if (!t || !s) return t;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    int hi, lo;
    for (; sn && *s; sn--, s++) {
        hi = iuhexdigit(*s);
        lo = 0;
        if (sn && s[1]) {lo = iuhexdigit(*++s); sn--;} else lo = 0;
        if (hi < 0 || lo < 0) return (bstr_t)0; // Invalid hex digit.
        *t++ = (unsigned char)((hi << 4) | lo);
    }
    if (*s) return (bstr_t)0; // Missing null terminator for source string.
    return t;
}
#else
    ;
#endif // LUB_DEFINITIONS

// Copy bytes <- bytes.
extern bstr_t bbsnncpy(bstr_t t, size_t tn, bcstr_t s, size_t sn,
                       bcstr_t st)
#if defined(LUB_DEFINITIONS)
 {  if (!t) return (bstr_t)NULL;
    if (sn > MAX_BSTRLEN) return (bstr_t)NULL;
    if (!s) {
      // Copy sn null bytes if s is NULL.
      for (; sn; sn--) *t++ = (byte_t)0;
    } else
    if (t <= s || t >= s + sn) {
      // Safe to copy sn bytes left-to-right, even with overlap.
      for (; sn; sn--) *t++ = *s++;
    } else {
      // Avoid potential overlap with copy sn bytes right-to-left.
      bstr_t tt = (t += sn) - 1;
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
 * @name llsntrim, ulsntrim, uusntrim (bounded, case-sensitive)
 *       llstrim, ulstrim, uustrim (default-bounded, case-sensitive)
 * @brief Trim leading and/or trailing characters that match one of the specified characters
 *        and optionally collapse embedded specified characters.
 * @param t Target buffer for the trimmed string.
 * @param s Source string to trim.
 * @param n Bound for the source string (clamped to MAX_LSTRLEN or
 *          MAX_USTRLEN) for bounded functions. For default-bounded
 *          functions, n defaults to MAX_LSTRLEN or MAX_USTRLEN.
 * @param trimset String of characters to trim from the beginning
 *                and/or end of the source string and optionally 
 *                collapse in the string (see delim parameter).
 * 
 *                Matching is case-sensitive (include both cases
 *                in trimset if desired).
 * 
 *                 If trimset is NULL or empty, defaults to trimming
 *                 whitespace characters (as determined by islspace
 *                 or isuspace).
 * @param trim 'L' or 'l': Trim characters in trimset from
 *                         the left (beginning) of the string.
 *             'R' or 'r': Trim characters in trimset from
 *                         the right (end) of the string.
 *             'B' or 'b': Trim characters in trimset from
 *                         both ends of the string.
 *             Any other character: error.
 * @param delim If not null-terminator ('\0'), collapse each sequence of one or more trimset
 *              characters in the middle of the string with this single delim character.
 *              If null-terminator ('\0'), do not collapse trimset characters in the middle of the string.
 * @return Pointer to t, or NULL if an error occurs
 * @note Errors:
 *       - Unterminated string s.
 *       - Unterminated string trimset.
 *       - Invalid trim mode (trim not 'L', 'R', or 'B' in either case).
 * @note The target buffer must be large enough for the result
 *       characters plus a null-terminator.
 * @note lusntrim and lustrim functions are not provided. Instead,
 *       use uusntrim/uustrim followed by lusncpy/luscpy.
 * @{
 */

#if defined(LUB_DEFINITIONS)
#undef LUB_TRIM_LEFT_MODE
#undef LUB_TRIM_RIGHT_MODE
#define LUB_TRIM_LEFT_MODE(trim) \
    ((trim) == 'L' || (trim) == 'B' || (trim) == 'l' || (trim) == 'b')
#define LUB_TRIM_RIGHT_MODE(trim) \
    ((trim) == 'R' || (trim) == 'B' || (trim) == 'r' || (trim) == 'b')
#endif // LUB_DEFINITIONS for trim mode macros.

// Trim (case-sensitive).
extern lstr_t llsnntrim(lstr_t t, size_t tn, lcstr_t s, size_t sn,
                       lcstr_t trimset, const char trim, lcchar_t delim)
#if defined(LUB_DEFINITIONS)
{   if (!t) return (lstr_t)NULL;
    if (!s) {*t = (lchar_t)0; return t;}

    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t len = lcsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (lstr_t)NULL;

    size_t start = 0, end = len;
    if (LUB_TRIM_LEFT_MODE(trim))
        while (start < end && islspace(s[start])) start++;
    if (LUB_TRIM_RIGHT_MODE(trim))
        while (end > start && islspace(s[end - 1])) end--;

    // TBD: If trimset is neither NULL nor empty, use it instead of whitespace characters.    
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
#else
;
#endif // LUB_DEFINITIONS for llsntrim.
extern ustr_t ulsnntrim(ustr_t t, size_t tn, lcstr_t s, size_t sn,
                       lcchar_t trimset, const char trim, ucchar_t delim)
#if defined(LUB_DEFINITIONS)
{   if (!t) return (ustr_t)NULL;
    if (!s) {*t = (uchar_t)0; return t;}

    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t len = lcsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (ustr_t)len;

    size_t start = 0, end = len;
    if (LUB_TRIM_LEFT_MODE(trim))
        while (start < end && islspace(s[start])) start++;
    if (LUB_TRIM_RIGHT_MODE(trim))
        while (end > start && islspace(s[end - 1])) end--;

    // TBD: If trimset is neither NULL nor empty, use it instead of whitespace characters.
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
#else
;
#endif // LUB_DEFINITIONS for ulsnnbtrim.
extern ustr_t uusnntrim(ustr_t t, size_t tn, ucstr_t s, size_t sn,
                       ucchar_t trimset, const char trim, ucchar_t delim)
#if defined(LUB_DEFINITIONS)
{   if (!t) return (ustr_t)NULL;
    if (!s) {*t = (uchar_t)0; return t;}

    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t len = ucsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (ustr_t)len;

    size_t start = 0, end = len;
    if (LUB_TRIM_LEFT_MODE(trim))
        while (start < end && isuspace(s[start])) start++;
    if (LUB_TRIM_RIGHT_MODE(trim))
        while (end > start && isuspace(s[end - 1])) end--;

    // TBD: If trimset is neither NULL nor empty, use it instead of whitespace characters.
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
#else
;
#endif // LUB_DEFINITIONS for uusnntrim.
/** @} */

/**
 * @defgroup PointerTrim Pointer Trim
 * @name llsptrim, uuptrim
 * @brief The trimmed substring of a string according to the trimset and
 *        trim parameters.
 * @param s       Source string.
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
 *       - s unterminated within the default bound (MAX_LSTRLEN or MAX_USTRLEN).
 * @note The returned pointer and length can be used to access the trimmed
 *       substring of the string (the subsstring might not be null-terminated).
 * @note lus and uls variants are not supported.
 * @{
 */
extern lcstr_t llsptrim(lcstr_t s, size_t *const trimlen,
                        const char trim, const char *trimset)
#if defined(LUB_DEFINITIONS)
    {   if (!trimlen) return NULL;
        *trimlen = 0;
        if (!s) return (lstr_t)NULL;
        size_t len = lcsnlen(s, MAX_LSTRLEN);
        if (len >= LUB_SIZE_ERRORS) return (lstr_t)len;
        lcstr_t left = s;
        lcstr_t right = s + len;
        if (LUB_TRIM_LEFT_MODE(trim)) {
            if (trimset) {
                while (left < right && llsnpbrk(left, trimset, 1) == left)
                    ++left;
            } else {
                while (left < right && islspace(*left))
                    ++left;
            }
        }
        if (LUB_TRIM_RIGHT_MODE(trim)) {
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
        return (lstr_t)left;
}
#else
    ;
#endif // LUB_DEFINITIONS for llsptrim.
extern ucstr_t uuptrim(ucstr_t s, size_t *const trimlen,
                              const char trim, const uchar_t *trimset)
#if defined(LUB_DEFINITIONS)
{
        if (!trimlen) return (ustr_t)NULL;
        *trimlen = 0;
        if (!s) return (ustr_t)NULL;
        size_t len = ucsnlen(s, MAX_USTRLEN);
        if (len >= LUB_SIZE_ERRORS) return (ustr_t)len;
        ucstr_t left = s;
        ucstr_t right = s + len;
        if (LUB_TRIM_LEFT_MODE(trim)) {
            if (trimset) {
                while (left < right && uusnpbrk(left, trimset, 1) == left)
                    ++left;
            } else {
                while (left < right && isuspace(*left))
                    ++left;
            }
        }
        if (LUB_TRIM_RIGHT_MODE(trim)) {
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
        return (ustr_t)left;
}
#else
    ;
#endif // LUB_DEFINITIONS for uuptrim.
/** @} */

#if defined(LUB_DEFINITIONS)
#undef LUB_TRIM_LEFT_MODE
#undef LUB_TRIM_RIGHT_MODE
#endif // LUB_DEFINITIONS

/**
 * @defgroup Reverse Reverse
 * @name llsnrev, uusnrev
 * @brief Reverse of the source string.
 * @param t Target buffer.
 * @param tn Maximum number of characters to write to the target buffer
 *           (excluding null-terminator).
 * @param s Source string.
 * @param sn Bound for the source string (clamped to MAX_LSTRLEN or
 *           MAX_USTRLEN) for bounded functions. For default-bounded
 *           functions, sn defaults to MAX_LSTRLEN or MAX_USTRLEN.
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
#undef __lub_reverse__
#define __lub_reverse__(t_t, s_t, vvslen, maxlen) \
    if (!t) return (t_t)NULL; \
    if (sn > maxlen) sn = maxlen; \
    size_t len = vvslen(s, sn); \
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
    } else { \
        /* No overlap or safe direction: normal reverse copy */ \
        t = t + len; \
        *t = (t_t)0; \
        for (; len; len--)  *--t = *s++; \
        return t; \
    }
#endif

// Reverse.
extern lstr_t llsnnrev(lstr_t t, lcstr_t s, size_t sn)
#if defined(LUB_DEFINITIONS)
    {__lub_reverse__(lstr_t, lcstr_t, lcsnlen, MAX_LSTRLEN)}
#else
    ;
#endif // LUB_DEFINITIONS for llsnnrev.
extern ustr_t uusnnrev(ustr_t t, ucstr_t s, size_t sn)
#if defined(LUB_DEFINITIONS)
    {__lub_reverse__(ustr_t, ucstr_t, ucsnlen, MAX_USTRLEN)}
#else
    ;
#undef __lub_reverse__
#endif // LUB_DEFINITIONS for uusnnrev.
/** @} */

/**
 * @defgroup Pad Pad
 * @name llsnnpad, lusnnpad, ulsnnpad, uusnnpad
 * @brief Pad source string to exactly tn characters using left, center, or
 *        right alignment (bounded).
 * @param t Target buffer.
 * @param tn Number of result characters written to t (excluding terminator).
 * @param s Source string.
 * @param n Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN)
 *          for bounded functions. For default-bounded functions,
 *          n defaults to MAX_LSTRLEN or MAX_USTRLEN.
 * @param pad 'L' or 'l' pad on left (for right-aligned text).
 *            'R' or 'r' pad on right (for left-aligned text).
 *            'B' or 'b' pad on left and right (for center-aligned text).
 *            Any other character is an error.
 * @param pad_c Pad character.
 * @param err_c For lusnnpad and lusnnpad, character used when a
 *              Unicode character cannot be converted to a Latin character
 *              (i.e., Unicode character > LUB_MAX_LCHAR).
 * @return t, or NULL on error.
 * @note Errors:
 *       - Invalid arguments
 *       - Unterminated source.
 *       - Source longer than tn.
 * @{
 */
// Pad.
extern lstr_t llsnnpad(lstr_t t, size_t tn, lcstr_t s, size_t sn,
                             char pad, lchar_t pad_c);
#if defined(LUB_DEFINITIONS)
{   if (!t || !s) return (lstr_t)NULL;
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;

    size_t len = lcsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (lstr_t)len;

    if (len > tn) {
        t[tn] = (lchar_t)0;
        return (lstr_t)NULL;
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
extern lstr_t lusnnpad(lstr_t t, size_t tn, ucstr_t s, size_t sn,
                      char pad, lchar_t pad_c, const lchar_t err_c)
#if defined(LUB_DEFINITIONS)
{   if (!t || !s) return (lstr_t)NULL;
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;

    size_t len = ucsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (lstr_t)len;

    if (len > tn) {
        t[tn] = (lchar_t)0;
        return (lstr_t)NULL;
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
    for (size_t i = 0; i < len; ++i)
        t[ti++] = (size_t)s[i] <= LUB_MAX_LCHAR ? (lchar_t)s[i] : err_c;
    for (size_t i = 0; i < right; ++i) t[ti++] = pad_c;
    t[ti] = (lchar_t)0;
    return t;
}
#else
;
#endif // LUB_DEFINITIONS for lusnnpad.
extern ustr_t ulsnnpad(ustr_t t, size_t tn, lcstr_t s, size_t sn,
                             char pad, uchar_t pad_c)
#if defined(LUB_DEFINITIONS)
{   if (!t || !s) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;

    size_t len = lcsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (ustr_t)len;

    if (len > tn) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
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
extern ustr_t uusnnpad(ustr_t t, size_t tn, ucstr_t s, size_t sn,
                             char pad, uchar_t pad_c)
#if defined(LUB_DEFINITIONS)
{   if (!t || !s) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;

    size_t len = ucsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (ustr_t)len;

    if (len > tn) {
        t[tn] = (uchar_t)0;
        return (ustr_t)NULL;
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
 * @name llsnnrepeat, lusnnrepeat, ulsnnrepeat, uusnnrepeat
 * @brief Repeat source string into target.
 * @param t Target buffer for result.
 * @param tn Maximum number of result characters written to t
 *           (excluding terminator).
 * @param s Source string.
 * @param sn Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param times Number of times to repeat source. 0 yields empty string.
 * @return t, or NULL on error.
 * @note Errors:
 *       - NULL arguments
 *       - LUB_MISSING_TERMINATOR
 *       - result exceeding tn characters
 * @note On error, returns NULL and writes terminator at t[tn].
 * @{
 */
static inline lstr_t llsnnrepeat(
    lstr_t t, size_t tn, lcstr_t s, size_t sn, size_t times) {
    if (!t || !s) return (lstr_t)NULL;
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) {
        t[tn] = (lchar_t)0; return (lstr_t)s_len;
    }

    if (times == 0) {
        t[0] = (lchar_t)0;  return t;
    }

    if (s_len && times > tn / s_len) {
        t[tn] = (lchar_t)0; return (lstr_t)NULL;
    }

    size_t needed = s_len * times;
    if (needed > tn) {
        t[tn] = (lchar_t)0; return (lstr_t)NULL;
    }

    for (size_t rep = 0; rep < times; ++rep)
        for (size_t i = 0; i < s_len; ++i)
            t[rep * s_len + i] = s[i];

    t[needed] = (lchar_t)0;
    return t;
}
static inline ustr_t lusnnrepeat(
    ustr_t t, size_t tn, ucstr_t s, size_t sn, size_t times) {
    if (!t || !s) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;

    size_t s_len = ucsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS)
       {t[tn] = (uchar_t)0; return (ustr_t)s_len;}

    if (times == 0)
       {t[0] = (uchar_t)0; return t;}

    if (s_len && times > tn / s_len)
      {t[tn] = (uchar_t)0; return (ustr_t)NULL;}

    size_t needed = s_len * times;
    if (needed > tn)
      {t[tn] = (uchar_t)0; return (ustr_t)NULL;}

    for (size_t rep = 0; rep < times; ++rep)
        for (size_t i = 0; i < s_len; ++i)
            t[rep * s_len + i] = s[i];

    t[needed] = (uchar_t)0;
    return t;
}
static inline ustr_t ulsnnrepeat(
    ustr_t t, size_t tn, lcstr_t s, size_t sn, size_t times) {
    if (!t || !s) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) {
        t[tn] = (uchar_t)0;
        return (ustr_t)s_len;
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
static inline ustr_t uusnnrepeat(
    ustr_t t, size_t tn, ucstr_t s, size_t sn, size_t times) {
    if (!t || !s) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;

    size_t s_len = ucsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) {
        t[tn] = (uchar_t)0;
        return (ustr_t)s_len;
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
/** @} */

/**
 * @defgroup ReplaceI ReplaceI
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
 * @note Needle matching is case-insensitive; delimiter matching is
 * case-sensitive;
 *       replacement text is written as-is.
 * @note If result would exceed tn characters, returns NULL and writes
 *       terminator at t[tn].
 * @note If map has more than one pair, m must be 0.
 * @note For multi-pair maps, the first matching needle in map order wins.
 *       List longer needles before shorter ones if overlap matters.
 
 * @defgroup Replace Replace
 * @name llsnnreplace, lusnnreplace, ulsnnreplace, uusnnreplace (case-sensitive)
 *       llsnnREPLACE, lusnnREPLACE, ulsnnREPLACE, uusnnREPLACE (case-insensitive)
 * @brief Replace using a delimiter-encoded map.
 * @param t Target buffer for result.
 * @param tn Size of target buffer (excluding terminator).
 *           Clamped to MAX_LSTRLEN or MAX_USTRLEN.
 * @param s Source string.
 * @param sn Maximum number of characters in source string.
 *           Clamped to MAX_LSTRLEN or MAX_USTRLEN.
 * @param st Truncated string indicator.
 * @param map Mapping string encoded as
 *            needle1<delim>rep1<delim>needle2<delim>rep2...
 * @param delim Mapping delimiter. Must not be null character.
 * @param m Replacement selector:
 *            0 replaces all matches;
 *           >0 replaces the mth match from start (1-based);
 *           <0 replaces the mth match from end (-1 is last).
 * @return t, or NULL on error.
 * @note Errors: 
 *       - invalid arguments
 *       - unterminated s or map within their bounds
 *       - malformed map syntax (missing delimiter or empty needle)
 *       - multi-pair maps used with m != 0
 *       - output longer than tn characters
 * @note If result would exceed tn characters, returns NULL and writes
 *       terminator at t[tn].
 * @note If map has more than one pair, m must be 0.
 * @note For multi-pair maps, the first matching needle in map order wins.
 *       List longer needles before shorter ones if overlap matters.
 * @{
 */

// Replace (case-sensitive).
static inline lstr_t llsnnreplace
       (lstr_t t, size_t tn, lcstr_t s, size_t sn,
        lcstr_t map, lchar_t delim, ptrdiff_t m)
{   if (!t || !s || !map || !delim) return (lstr_t)NULL;
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) return (lstr_t)s_len;
    size_t map_len = lcsnlen(map, MAX_LMAPLEN);
    if (map_len >= LUB_SIZE_ERRORS) return (lstr_t)map_len;

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
                    for (size_t j = 0; j < repl_len; ++j)
                        t[ti + j] = first_repl[j];
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lstr_t)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j)
                        t[ti + j] = s[si + j];
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
static inline lstr_t lusnnreplace(
    lstr_t t, size_t tn, lcstr_t s, size_t sn,
    ucstr_t map, uchar_t delim, ptrdiff_t m)
{   if (!t || !s || !map || !delim) return (lstr_t)NULL;
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) return (lstr_t)s_len;
    size_t map_len = ucsnlen(map, MAX_UMAPLEN);
    if (map_len >= LUB_SIZE_ERRORS) return (lstr_t)map_len;

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
                    if ((size_t)first_needle[j] > LUB_MAX_LCHAR ||
                        s[si + j] != (lchar_t)first_needle[j]) {
                            match = 0;
                            break;
                        }
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
                if ((size_t)first_needle[j] > LUB_MAX_LCHAR ||
                    s[si + j] != (lchar_t)first_needle[j]) match = 0;

            if (match) {
                occ++;
                if (!m || occ == target) {
                    if (ti + repl_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lstr_t)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j)
                        t[ti + j] = (size_t)first_repl[j] <= LUB_MAX_LCHAR ?
                                    (lchar_t)first_repl[j] : '?';
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lstr_t)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j)
                        t[ti + j] = s[si + j];
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
                if ((size_t)needle[j] > LUB_MAX_LCHAR ||
                    s[si + j] != (lchar_t)needle[j]) match = 0;

            if (match) {
                if (ti + repl_len > tn) {
                    t[tn] = (lchar_t)0;
                    return (lstr_t)NULL;
                }
                for (size_t j = 0; j < repl_len; ++j)
                    t[ti + j] = (size_t)repl[j] <= LUB_MAX_LCHAR ?
                                (lchar_t)repl[j] : '?';
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
static inline ustr_t ulsnnreplace(
    ustr_t t, size_t tn, ucstr_t s, size_t sn,
    lcstr_t map, lchar_t delim, ptrdiff_t m)
{   if (!t || !s || !map || !delim) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;

    size_t s_len = ucsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) return (ustr_t)s_len;
    size_t map_len = lcsnlen(map, MAX_LMAPLEN);
    if (map_len >= LUB_SIZE_ERRORS) return (ustr_t)map_len;

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
                    if (s[si + j] != (uchar_t)first_needle[j]) {
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
                    for (size_t j = 0; j < repl_len; ++j)
                        t[ti + j] = (uchar_t)first_repl[j];
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (uchar_t)0;
                        return (ustr_t)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j)
                        t[ti + j] = s[si + j];
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
                return (ustr_t)NULL;
            }
            t[ti++] = s[si++];
        }
    }

    t[ti] = (uchar_t)0;
    return t;
}
static inline ustr_t uusnnreplace(
    ustr_t t, size_t tn, ucstr_t s, size_t sn, lcstr_t st,
    ucstr_t map, uchar_t delim, ptrdiff_t m)
{   if (!t || !s || !map || !delim) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;

    size_t s_len = ucsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) return (ustr_t)s_len;
    size_t map_len = ucsnlen(map, MAX_UMAPLEN);
    if (map_len >= LUB_SIZE_ERRORS) return (ustr_t)map_len;

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
                    for (size_t j = 0; j < repl_len; ++j)
                        t[ti + j] = first_repl[j];
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (uchar_t)0;
                        return (ustr_t)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j)
                        t[ti + j] = s[si + j];
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

// Replace (case-insensitive).
static inline lstr_t llsnREPLACE(
    lstr_t t, size_t tn, lcstr_t s, lcstr_t map, lchar_t delim, size_t n,
    ptrdiff_t m) {
    if (!t || !s || !map || !delim) return (lstr_t)NULL;
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, n);
    if (s_len >= LUB_SIZE_ERRORS) return (lstr_t)s_len;
    size_t map_len = lcsnlen(map, MAX_LMAPLEN);
    if (map_len >= LUB_SIZE_ERRORS) return (lstr_t)map_len;

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
            if (s_len > tn) {t[tn] = (lchar_t)0; return (lstr_t)NULL;}
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
                        return (lstr_t)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j)
                        t[ti + j] = first_repl[j];
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lstr_t)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j)
                        t[ti + j] = s[si + j];
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
                if (lltoupper(s[si + j]) != lltoupper(needle[j])) match = 0;

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
static inline lstr_t lusnREPLACE(
    lstr_t t, size_t tn, lcstr_t s, ucstr_t map, uchar_t delim, size_t n,
    ptrdiff_t m) {
    if (!t || !s || !map || !delim) return (lstr_t)NULL;
    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, n);
    if (s_len >= LUB_SIZE_ERRORS) return (lstr_t)s_len;
    size_t map_len = ucsnlen(map, MAX_UMAPLEN);
    if (map_len >= LUB_SIZE_ERRORS) return (lstr_t)map_len;

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
                    if ((size_t)first_needle[j] > LUB_MAX_LCHAR ||
                        lltoupper(s[si + j]) != lltoupper((lchar_t)first_needle[j]))
                    {
                        match = 0;
                        break;
                        }
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
                if ((size_t)first_needle[j] > LUB_MAX_LCHAR ||
                    lltoupper(s[si + j]) != lltoupper((lchar_t)first_needle[j]))
                        match = 0;

            if (match) {
                occ++;
                if (!m || occ == target) {
                    if (ti + repl_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lstr_t)NULL;
                    }
                    for (size_t j = 0; j < repl_len; ++j)
                        t[ti + j] = (size_t)first_repl[j] <= LUB_MAX_LCHAR ?
                                    (lchar_t)first_repl[j] : '?';
                    ti += repl_len;
                } else {
                    if (ti + needle_len > tn) {
                        t[tn] = (lchar_t)0;
                        return (lstr_t)NULL;
                    }
                    for (size_t j = 0; j < needle_len; ++j)
                        t[ti + j] = s[si + j];
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
                if ((size_t)needle[j] > LUB_MAX_LCHAR ||
                    lltoupper(s[si + j]) != lltoupper((lchar_t)needle[j]))
                        match = 0;

            if (match) {
                if (ti + repl_len > tn) {
                    t[tn] = (lchar_t)0;
                    return (lstr_t)NULL;
                }
                for (size_t j = 0; j < repl_len; ++j)
                    t[ti + j] = (size_t)repl[j] <= LUB_MAX_LCHAR ?
                                (lchar_t)repl[j] : '?';
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
static inline ustr_t ulsnREPLACE(
    ustr_t t, size_t tn, ucstr_t s, lcstr_t map, lchar_t delim, size_t n,
    ptrdiff_t m) {
    if (!t || !s || !map || !delim) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t s_len = ucsnlen(s, n);
    if (s_len >= LUB_SIZE_ERRORS) return (ustr_t)s_len;
    size_t map_len = lcsnlen(map, MAX_LMAPLEN);
    if (map_len >= LUB_SIZE_ERRORS) return (ustr_t)map_len;

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
            if (s_len > tn) {t[tn] = (uchar_t)0; return (ustr_t)NULL;}
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
static inline ustr_t uusnnREPLACE(
    ustr_t t, size_t tn, ucstr_t s, size_t sn, lcstr_t st,
    ucstr_t map, uchar_t delim, ptrdiff_t m) {
    if (!t || !s || !map || !delim) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;

    size_t s_len = ucsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) return (ustr_t)s_len;
    size_t map_len = ucsnlen(map, MAX_UMAPLEN);
    if (map_len >= LUB_SIZE_ERRORS) return (ustr_t)map_len;

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
/** @} */

/** 
 * @defgroup CharacterSubstitution Character Substitution
 * @name llsnsub, lusnsub, ulsnsub, uusnsub (bounded, case-sensitive)
 *       llssub, lussub, ulssub, uussub (default-bounded, case-sensitive)
 *       llsnSUB, lusnSUB, ulsnSUB, uusnSUB (bounded, case-insensitive)
 *       llsSUB, lusSUB, ulsSUB, uusSUB (default-bounded, case-insensitive)
 * @{
 */
/**
 * @name llsnnsub, lusnnsub, ulsnnsub, uusnnsub (case-sensitive)
 * @brief Character substitution of source into target.
 * @param t Target buffer.
 * @param tn Maximum number of result characters written to t
 *           (excluding terminator).
 * @param s Source string.
 * @param sn Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param from Character set specifying which characters to replace.
 * @param to Replacement character set. Must be same length as from.
 *        If to is shorter than from or NULL, the function returns NULL.
 * @return t, or NULL on error.
 * @note Each character in source that appears in from (at position i) is
 *       replaced with to[i]. Characters not in from are copied as-is.
 * @note Errors: 
 *       - NULL arguments, from and to having different lengths,
 *       - unterminated s, from, or to
 *       - result exceeding tn characters.
 * @note On error, returns NULL and writes terminator at t[tn].
 * @{
 */

// Character substitution (case-sensitive).
static inline lstr_t llsnnsub(
    lstr_t t, size_t tn, lcstr_t s, size_t sn, lcstr_t st,
    lcstr_t from, lcstr_t to)
{   if (!t) return (lstr_t)NULL;
    *t = (lchar_t)0; // Null-terminate in case of error.
    if (!s || !*s) return t;
    // TBD: treat null as empty string.
    if (!from || !to) return (lstr_t)NULL;

    if (tn > MAX_LSTRLEN) tn = MAX_LSTRLEN;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS)
      {t[tn] = (lchar_t)0; return (lstr_t)s_len;}

    size_t from_len = lcsnlen(from, sn);
    if (from_len >= LUB_SIZE_ERRORS) {t[tn] = (lchar_t)0; return (lstr_t)from_len;}
    size_t to_len = lcslen(to);
    if (to_len >= LUB_SIZE_ERRORS) {t[tn] = (lchar_t)0; return (lstr_t)to_len;}
    if (from_len != to_len) {
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
static inline ustr_t lusnnsub(
    ustr_t t, size_t tn, ucstr_t s, size_t sn,
    lcstr_t from, lcstr_t to)
{   if (!t) return (ustr_t)NULL;
    *t = (uchar_t)0; // Null-terminate in case of error.
    if (!s || !from || !to) return (ustr_t)NULL;
    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;

    size_t s_len = ucsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) return (ustr_t)s_len;

    size_t from_len = lcslen(from);
    if (from_len >= LUB_SIZE_ERRORS) return (ustr_t)from_len;

    size_t to_len = lcslen(to);
    if (to_len >= LUB_SIZE_ERRORS) return (ustr_t)to_len;

    if (s_len > tn) return (ustr_t)NULL;

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
static inline ustr_t ulsnnsub(
    ustr_t t, size_t tn, lcstr_t s, size_t sn, ucstr_t from, ucstr_t to)
 {  if (!t) return (ustr_t)NULL;
    *t = (uchar_t)0; // Null-terminate in case of error.
    if (!s) return t;
    if (!from || !to) return (ustr_t)NULL;

    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) return (ustr_t)s_len;

    size_t from_len = ucsnlen(from, sn);
    if (from_len >= LUB_SIZE_ERRORS) return (ustr_t)from_len;

    size_t to_len = ucsnlen(to, sn);
    if (to_len >= LUB_SIZE_ERRORS) return (ustr_t)to_len;

    if (from_len != to_len) return (ustr_t)LUB_OVERLAP;

    if (s_len > tn) return (ustr_t)NULL;

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
static inline ustr_t uusnnsub(
    ustr_t t, size_t tn, ucstr_t s, size_t sn, ucstr_t from, ucstr_t to) {
    if (!t) return (ustr_t)NULL;
    *t = (uchar_t)0; // Null-terminate in case of error.
    if (!s || !from || !to) return (ustr_t)NULL;

    if (tn > MAX_USTRLEN) tn = MAX_USTRLEN;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;

    size_t s_len = ucsnlen(s, sn);
    if (s_len >= LUB_SIZE_ERRORS) {
        t[tn] = (uchar_t)0; return (ustr_t)s_len;
    }

    size_t from_len = ucsnlen(from, sn);
    if (from_len >= LUB_SIZE_ERRORS) {
        t[tn] = (uchar_t)0; return (ustr_t)from_len;
    }
    size_t to_len = ucsnlen(to, sn);
    if (to_len >= LUB_SIZE_ERRORS) {
        t[tn] = (uchar_t)0; return (ustr_t)to_len;
    }
    if (from_len != to_len) return (ustr_t)LUB_OVERLAP;

    if (s_len > tn) return (ustr_t)NULL;

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
/** @} */

/**
 * @defgroup Split Split
 * @name llsnsplit, lusnsplit, ulsnsplit, uusnsplit
 * @brief Split source at first delimiter into left and right outputs (bounded).
 * @param left Output buffer for part before delimiter.
 * @param right Output buffer for part after delimiter.
 * @param s Source string.
 * @param delim Delimiter character.
 * @param sn Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return 2 if delimiter found, 1 if not found (right becomes empty),
 *         or error.
 * @note Errors:
 *       - LUB_MISSING_TERMINATOR
 *       - NULL left or right arguments
 *       - LUB_TRUNCATION
 * @{
 */
static inline size_t llsnnnsplit(
    lstr_t left, lstr_t right, lcstr_t s, lchar_t delim, size_t sn) {
    if (!left || !right || !s) return 0;

    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t len = lcsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (size_t)len;

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
static inline size_t lusnsnplit(
    lstr_t left, lstr_t right, lcstr_t s, uchar_t delim, size_t n) {
    if (!left || !right || !s) return 0;

    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t len = lcsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (size_t)len;

    size_t split = len;
    if ((size_t)delim <= LUB_MAX_LCHAR) {
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
static inline size_t ulsnnsplit(
    ustr_t left, ustr_t right, ucstr_t s, lchar_t delim, size_t n) {
    if (!left || !right || !s) return 0;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t len = ucsnlen(s, n);
    if (len >= LUB_SIZE_ERRORS) return 0;

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
static inline size_t uusnnsplit(
    ustr_t left, ustr_t right, ucstr_t s, uchar_t delim, size_t n) {
    if (!left || !right || !s) return 0;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;

    size_t len = ucsnlen(s, n);
    if (len >= LUB_SIZE_ERRORS) return 0;

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
/** @} */

/**
 * @defgroup PrintfStyleFormatting Printf-style Formatting
 * @name llsnvprintf, llsnprintf
 * @brief Format Latin text into target buffer.
 * @param t Target buffer.
 * @param tn Maximum number of characters for target buffer (not including terminator).
 * @param fmt Format string.
 * @param ap Variable argument list for llsnvprintf/llsnvprintf.
 * @return Number of chars written (excluding terminator), or -1 on error/truncate.
 * @note This family uses C vsnprintf semantics and treats lchar_t storage as bytes.
 * @{
 */
static inline int llsnvprintf(lstr_t t, size_t tn, lcstr_t fmt, va_list ap) {
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
    int rc = llsnvprintf(t, tn, fmt, ap);
    va_end(ap);
    return rc;
}
/** @} */

/**
 * @defgroup Compare Compare
 * @name llsncmp, lusncmp, ulsncmp, uusncmp (bounded, case-sensitive)
 *       llsnCMP, lusnCMP, ulsnCMP, uusnCMP (bounded, case-insensitive)
 *       bbsncmp
 * @return int -1, 0, or 1. If less than -1, error.
 * @note Errors:
 *       - Unternerminated character source string.
 * @{
 */

/** @} */

 /**
 * @defgroup Compare Compare
 * @name llsncmp, lusncmp, ulsncmp, uusncmp (case-sensitive)
 *       llsnCMP, lusnCMP, ulsnCMP, uusnCMP (case-insensitive)
 *       bbsncmp
 * @brief Comparison of two strings.
 * @param s1 First source string.
 * @param s2 Second source string.
 * @param sn Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN) for
 *           bounded functions only, Default-bounded functions use MAX_LSTRLEN
 *           or MAX_USTRLEN.
 * @return -1 (s1 < s2), 0 (equal), 1 (s1 > s2), or error
 * @note Errors:
 *       - LUB_MISSING_TERMINATOR
 * @{
 */

// case-sensitive compare.
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

// case-insensitive compare.
static inline int llsnCMP(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    for (; n && *s1 && *s2 && lltoupper(*s1) == lltoupper(*s2); s1++, s2++, n--);
    if (!*s1 && !*s2)
        return (int)0;
    if (n)
        return lltoupper(*s1) > lltoupper(*s2) ? (int)1 : (int)-1;
    return (int)-2; // source not null terminated.
}
static inline int lusnCMP(lcstr_t s1, ucstr_t s2, size_t n) {
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
static inline int ulsnCMP(ucstr_t s1, lcstr_t s2, size_t n) {
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
static inline int uusnCMP(ucstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n && *s1 && *s2 && touupper(*s1) == touupper(*s2); s1++, s2++, n--);
    if (!*s1 && !*s2)
        return (int)0;
    if (n)
        return touupper(*s1) > touupper(*s2) ? (int)1 : (int)-1;
    return (int)-2; // source not null terminated.
}

// TBD: byte to byte compare
/** @} */

/**
 * @defgroup MemCompare Memory Compare
 * @name llsnmemcmp, lusnmemcmp, ulsnmemcmp, uusnmemcmp (case-sensitive)
 *       llsnMEMCMP, lusnMEMCMP, ulsnMEMCMP, uusnMEMCMP (case-insensitive)
 * @brief Compare of sn characters of two strings (stops at null terminator).
 * @param s1 First source string.
 * @param s2 Second source string.
 * @param sn Number of characters to compare.
 * @return   0 if all sn characters are equal and both strings are at least sn long,
 *           <0 if s1 < s2, >0 if s1 > s2 (by first differing char or null).
 * @note     sn is clamped to MAX_LSTRLEN or MAX_USTRLEN.
 * @note     Stops at null terminator in either string; if either string is shorter than sn,
 *           returns nonzero (short string is less if all compared chars match).
 * @note     Returns 0 if sn == 0. If s1 or s2 is NULL, returns INT_MIN.
 * @note     Uses pointer arithmetic, not array indexing.
 * @{
 */
// case-sensitive memory compare.
static inline int llsnmemcmp(const lcstr_t s1, const lcstr_t s2, size_t sn) {
    if (!s1 || !s2) return (int)0x80000000; /* INT_MIN */
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    for (; sn > 0; --sn, ++s1, ++s2) {
        if (!*s1 || !*s2) {
            if (*s1 == *s2) break;
            return (int)((unsigned char)*s1 - (unsigned char)*s2);
        }
        if (*s1 != *s2)
            return (int)((unsigned char)*s1 - (unsigned char)*s2);
    }
    if (n == 0) return 0;
    if (*s1 == *s2) return 0;
    return (int)((unsigned char)*s1 - (unsigned char)*s2);
}
static inline int lusnmemcmp(const lcstr_t s1, const ucstr_t s2, size_t n) {
    if (!s1 || !s2) return (int)0x80000000; /* INT_MIN */
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    for (; n > 0; --n, ++s1, ++s2) {
        if (!*s1 || !*s2) {
            if (*s1 == *s2) break;
            return (int)((unsigned char)*s1 - (unsigned short)*s2);
        }
        if ((unsigned char)*s1 != (unsigned short)*s2)
            return (int)((unsigned char)*s1 - (unsigned short)*s2);
    }
    if (n == 0) return 0;
    if (*s1 == *s2) return 0;
    return (int)((unsigned char)*s1 - (unsigned short)*s2);
}
static inline int ulsnmemcmp(const ucstr_t s1, const lcstr_t s2, size_t n) {
    if (!s1 || !s2) return (int)0x80000000; /* INT_MIN */
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n > 0; --n, ++s1, ++s2) {
        if (!*s1 || !*s2) {
            if (*s1 == *s2) break;
            return (int)((unsigned short)*s1 - (unsigned char)*s2);
        }
        if ((unsigned short)*s1 != (unsigned char)*s2)
            return (int)((unsigned short)*s1 - (unsigned char)*s2);
    }
    if (n == 0) return 0;
    if (*s1 == *s2) return 0;
    return (int)((unsigned short)*s1 - (unsigned char)*s2);
}
static inline int uusnmemcmp(const ucstr_t s1, const ucstr_t s2, size_t n) {
    if (!s1 || !s2) return (int)0x80000000; /* INT_MIN */
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n > 0; --n, ++s1, ++s2) {
        if (!*s1 || !*s2) {
            if (*s1 == *s2) break;
            return (int)((unsigned short)*s1 - (unsigned short)*s2);
        }
        if (*s1 != *s2)
            return (int)((unsigned short)*s1 - (unsigned short)*s2);
    }
    if (n == 0) return 0;
    if (*s1 == *s2) return 0;
    return (int)((unsigned short)*s1 - (unsigned short)*s2);
}
// case-insensitive memory compare.
static inline int llsnMEMCMP(const lcstr_t s1, const lcstr_t s2, size_t n) {
    if (!s1 || !s2) return (int)0x80000000; /* INT_MIN */
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    for (; n > 0; --n, ++s1, ++s2) {
        unsigned char c1 = (unsigned char)*s1;
        unsigned char c2 = (unsigned char)*s2;
        if (!c1 || !c2) {
            if (c1 == c2) break;
            return (int)(tolower(c1) - tolower(c2));
        }
        if (tolower(c1) != tolower(c2))
            return (int)(tolower(c1) - tolower(c2));
    }
    if (n == 0) return 0;
    if (*s1 == *s2) return 0;
    return (int)(tolower((unsigned char)*s1) - tolower((unsigned char)*s2));
}
static inline int lusnMEMCMP(const lcstr_t s1, const ucstr_t s2, size_t n) {
    if (!s1 || !s2) return (int)0x80000000; /* INT_MIN */
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    for (; n > 0; --n, ++s1, ++s2) {
        unsigned char c1 = (unsigned char)*s1;
        unsigned short c2 = (unsigned short)*s2;
        if (!c1 || !c2) {
            if (c1 == c2) break;
            return (int)(tolower(c1) - towlower(c2));
        }
        if (tolower(c1) != towlower(c2))
            return (int)(tolower(c1) - towlower(c2));
    }
    if (n == 0) return 0;
    if (*s1 == *s2) return 0;
    return (int)(tolower((unsigned char)*s1) - towlower((unsigned short)*s2));
}
static inline int ulsnMEMCMP(const ucstr_t s1, const lcstr_t s2, size_t n) {
    if (!s1 || !s2) return (int)0x80000000; /* INT_MIN */
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n > 0; --n, ++s1, ++s2) {
        unsigned short c1 = (unsigned short)*s1;
        unsigned char c2 = (unsigned char)*s2;
        if (!c1 || !c2) {
            if (c1 == c2) break;
            return (int)(towlower(c1) - tolower(c2));
        }
        if (towlower(c1) != tolower(c2))
            return (int)(towlower(c1) - tolower(c2));
    }
    if (n == 0) return 0;
    if (*s1 == *s2) return 0;
    return (int)(towlower((unsigned short)*s1) - tolower((unsigned char)*s2));
}
static inline int uusnMEMCMP(const ucstr_t s1, const ucstr_t s2, size_t n) {
    if (!s1 || !s2) return (int)0x80000000; /* INT_MIN */
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    for (; n > 0; --n, ++s1, ++s2) {
        unsigned short c1 = (unsigned short)*s1;
        unsigned short c2 = (unsigned short)*s2;
        if (!c1 || !c2) {
            if (c1 == c2) break;
            return (int)(towlower(c1) - towlower(c2));
        }
        if (towlower(c1) != towlower(c2))
            return (int)(towlower(c1) - towlower(c2));
    }
    if (n == 0) return 0;
    if (*s1 == *s2) return 0;
    return (int)(towlower((unsigned short)*s1) - towlower((unsigned short)*s2));
}
/** @} */

/**
 * @defgroup PrefixCompare Prefix Compare
 * @name llsnpfx, lusnpfx, ulsnpfx, and uusnpfx (case-sensitive)
 *       llsnPFX, lusnPFX, ulsnPFX, and uusnPFX (case-insensitive)
 * @brief Prefix check of s2 against the start of s1.
 * @param s1 Source string to test as the full string.
 * @param s2 Source prefix candidate.
 * @param n Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return 0 if s2 is a prefix of s1, -1/1 on first mismatch ordering,
 *         or -2 on source contract violation.
 * @{
 */

// case-sensitive prefix check.
static inline int llsnpfx(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = lcsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
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
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = ucsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
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
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = lcsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
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
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = ucsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
    size_t k = s1_len < s2_len ? s1_len : s2_len;
    for (size_t i = 0; i < k; ++i) {
        if (s1[i] != s2[i])
            return s1[i] > s2[i] ? (int)1 : (int)-1;
    }
    if (s2_len <= s1_len)
        return (int)0;
    return (int)-1;
}

// case-insensitive prefix check.
static inline int llsnPFX(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = lcsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
    size_t k = s1_len < s2_len ? s1_len : s2_len;
    for (size_t i = 0; i < k; ++i) {
        lchar_t c1 = lltoupper(s1[i]);
        lchar_t c2 = lltoupper(s2[i]);
        if (c1 != c2)
            return c1 > c2 ? (int)1 : (int)-1;
    }
    if (s2_len <= s1_len)
        return (int)0;
    return (int)-1;
}
static inline int lusnPFX(lcstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = ucsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
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
static inline int ulsnPFX(ucstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = lcsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
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
static inline int uusnPFX(ucstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = ucsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
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
/** @} */

/**
 * @defgroup SuffixCompare Suffix Compare
 * @name llsnsfx, lusnsfx, ulsnsfx, and uusnsfx (case-sensitive)
 *       llsnSFX, lusnSFX, ulsnSFX, and uusnSFX (case-insensitive)
 * @brief Case-sensitive suffix check of s2 against the end of s1.
 * @param s1 Source string to test as the full string.
 * @param s2 Source suffix candidate.
 * @param n Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return 0 if s2 is a suffix of s1, -1/1 on first aligned mismatch ordering,
 *         or -2 on source contract violation.
 * @{
 */
// case-sensitive suffix check.
static inline int llsnsfx(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = lcsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
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
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = ucsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
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
    if (s1_len == MISSING_TERMINATOR ||
        s2_len == MISSING_TERMINATOR)
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
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = ucsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
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

// case-insensitive suffix check.
static inline int llsnSFX(lcstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_LSTRLEN) n = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = lcsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
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
static inline int lusnSFX(lcstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = ucsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
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
static inline int ulsnSFX(ucstr_t s1, lcstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = lcsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
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
static inline int uusnSFX(ucstr_t s1, ucstr_t s2, size_t n) {
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = ucsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
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
/** @} */

/**
 * @defgroup SetCharacterSearch Set-Character Search
 * @name llsnpbrkm, lusnpbrkm, ulsnpbrkm, uusnpbrkm (case-sensitive, mth character)
 *       llsnpBRKM, lusnpBRKM, ulsnpBRKM, uusnpBRKM (case-insensitive, mth character)
 * @brief Search for the mth character in string s1 that
 *        appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @param sn Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index (1 = first, 2 = second, ...). 0 returns NULL.
 * @return Pointer to mth matching character in s1, or NULL if not found or
 *         source error.
 * @note Errors;
 * k     - LUB_MISSING_TERMINATOR if s1 or s2 is not null-terminated by the specified bound.
 * @{
 */

 // Set-character search (case-sensitive)
static inline lcstr_t llsnpbrkm(
    lcstr_t s1, lcstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (lstr_t)NULL;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
    if (!s1_len || !s2_len) return (lstr_t)NULL;
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
    lcstr_t s1, ucstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (lstr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
    if (!s1_len || !s2_len) return (lstr_t)NULL;
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
    ucstr_t s1, lcstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
    if (!s1_len || !s2_len) return (ustr_t)NULL;
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
    ucstr_t s1, ucstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (int)s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (int)s2_len;
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

// Set-character search (case-insensitive) 
static inline lcstr_t llsnPBRKM(
    lcstr_t s1, lcstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (lstr_t)NULL;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (lstr_t)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (lstr_t)s2_len;
    if (!s1_len || !s2_len) return (lstr_t)NULL;
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
static inline lcstr_t lusnPBRKM(
    lcstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m) return (lstr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, n);
    if (s1_len >= LUB_SIZE_ERRORS) return (lstr_t)s1_len;
    size_t s2_len = ucsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (lstr_t)s2_len;
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
static inline ucstr_t ulsnPBRKM(
    ucstr_t s1, lcstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m) return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    if (s1_len >= LUB_SIZE_ERRORS) return (ustr_t)s1_len;
    size_t s2_len = lcsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (ustr_t)s2_len;
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
static inline ucstr_t uusnPBRKM(
    ucstr_t s1, ucstr_t s2, size_t n, size_t m) {
    if (!s1 || !s2 || !m) return (ustr_t)NULL;
    if (n > MAX_USTRLEN) n = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, n);
    if (s1_len >= LUB_SIZE_ERRORS) return (ustr_t)s1_len;
    size_t s2_len = ucsnlen(s2, n);
    if (s2_len >= LUB_SIZE_ERRORS) return (ustr_t)s2_len;
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
 * @name llsnrpbrkm, lusnrpbrkm, ulsnrpbrkm, and uusnrpbrkm
 * @brief case-sensitive search for the mth-from-last character in
 * string s1 that appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @param sn Bound on both strings (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index from the end (1 = last, 2 = second-last, ...).
 * @return Pointer to mth-from-last matching character in s1, or NULL if no
 * match or source error.
 */
// Set-character search (case-insensitive) from the end.
static inline lcstr_t llsnrpbrkm(
    lcstr_t s1, lcstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (lstr_t)NULL;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (ustr_t)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (ustr_t)s2_len;
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
    lcstr_t s1, ucstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (lstr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (lstr_t)s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (lstr_t)s2_len;
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
    ucstr_t s1, lcstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (ustr_t)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (ustr_t)s2_len;
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
    ucstr_t s1, ucstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (ustr_t)s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (ustr_t)s2_len;
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
 * @name llsnRPBRKM, lusnRPBRKM, ulsnRPBRKM, and uusnRPBRKM
 * @brief Bounded case-insensitive search for the mth-from-last character in
 * string s1 that appears in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string to match against.
 * @param n Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index from the end (1 = last, 2 = second-last, ...).
 * @return Pointer to mth-from-last matching character in s1, or NULL if no
 * match or source error.
 */

// Set-character search (case-insensitive) from the end.
static inline lcstr_t llsnRPBRKM(
    lcstr_t s1, lcstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (lstr_t)NULL;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (lstr_t)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (lstr_t)s2_len;
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
static inline lcstr_t lusnRPBRKM(
    lcstr_t s1, ucstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (lstr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (lstr_t)s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (lstr_t)s2_len;
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
static inline ucstr_t ulsnRPBRKM(
    ucstr_t s1, lcstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    size_t s2_len = lcsnlen(s2, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (ustr_t)s1_len;
    if (s2_len >= LUB_SIZE_ERRORS) return (ustr_t)s2_len;
    size_t k = 0;
    for (size_t i = s1_len; i > 0; --i) {
        size_t idx = i - 1;
        uchar_t s1c = touupper(s1[idx]);

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
static inline ucstr_t uusnRPBRKM(
    ucstr_t s1, ucstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (ustr_t)s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (ustr_t)s2_len;
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
 * @defgroup CharacterSearch Character Search

 * @name llsnchrm, lusnchrm, ulsnchrm, uusnchrm (case-sensitive)
 *       llsnCHRM, lusnCHRM, ulsnCHRM, uusnCHRM (case-insensitive)
 * @brief Search for the mth (first/last) occurrence of character c in string s.
 *        If m > 0, searches forward for the mth occurrence; if m < 0,
 *        searches backward for the |m|th-from-last occurrence.
 *        If m == 0, returns NULL.
 * @param s Source string.
 * @param c Character to search for.
 * @param sn Bound on source string (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index: m > 0 for forward (1 = first, 2 = second, ...),
 *          m < 0 for reverse (−1 = last, −2 = second-last, ...). 0 returns NULL.
 * @return  Pointer to the mth (or |m|th-from-last) match in s, or NULL
 *          if not found or on source error.
 * @note Errors:
 *       - LUB_MISSING_TERMINATOR if s is not null-terminated by the specified bound.
 * @{
 */

//  mth character search (case-sensitive).
static inline lcstr_t llsnchrm(
    lcstr_t s, const lchar_t c, size_t sn, int m) {
    if (!s || !m) return (lstr_t)NULL;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t len = lcsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (lstr_t)len;
    if (m > 0) {
        size_t k = 0;
        lcstr_t p;
        for (p = s; p <= s + len; ++p) {
            if (*p == c) {
                if (++k == (size_t)m) return p;
            }
        }
    } else if (m < 0) {
        size_t k = 0;
        lcstr_t p;
        for (p = s + len; p >= s; --p) {
            if (*p == c && ++k == (size_t)(-m)) return p;
            if (p == s) break;
        }
    }
    return (lstr_t)NULL;
}
static inline lcstr_t lusnchrm(
    lcstr_t s, const uchar_t c, size_t sn, int m) {
    if (!s || !m) return (lstr_t)NULL;
    if ((size_t)c > LUB_MAX_LCHAR) return (lstr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t len = lcsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (lstr_t)len;
    lchar_t lc = (lchar_t)c;
    if (m > 0) {
        size_t k = 0;
        lcstr_t p;
        for (p = s; p <= s + len; ++p) {
            if (*p == lc) {
                if (++k == (size_t)m) return p;
            }
        }
    } else if (m < 0) {
        size_t k = 0;
        lcstr_t p;
        for (p = s + len; p >= s; --p) {
            if (*p == lc && ++k == (size_t)(-m)) return p;
            if (p == s) break;
        }
    }
    return (lstr_t)NULL;
}
static inline ucstr_t ulsnchrm(
    ucstr_t s, const lchar_t c, size_t sn, int m) {
    if (!s || !m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t len = ucsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (ustr_t)len;
    uchar_t uc = (uchar_t)c;
    if (m > 0) {
        size_t k = 0;
        ucstr_t p;
        for (p = s; p <= s + len; ++p) {
            if (*p == uc) {
                if (++k == (size_t)m) return p;
            }
        }
    } else if (m < 0) {
        size_t k = 0;
        ucstr_t p;
        for (p = s + len; p >= s; --p) {
            if (*p == uc && ++k == (size_t)(-m)) return p;
            if (p == s) break;
        }
    }
    return (ustr_t)NULL;
}
static inline ucstr_t uusnchrm(
    ucstr_t s, const uchar_t c, size_t sn, int m) {
    if (!s || !m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t len = ucsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (ustr_t)len;
    if (m > 0) {
        size_t k = 0;
        ucstr_t p;
        for (p = s; p <= s + len; ++p) {
            if (*p == c) {
                if (++k == (size_t)m) return p;
            }
        }
    } else if (m < 0) {
        size_t k = 0;
        ucstr_t p;
        for (p = s + len; p >= s; --p) {
            if (*p == c && ++k == (size_t)(-m)) return p;
            if (p == s) break;
        }
    }
    return (ustr_t)NULL;
}

//  mth character search (case-insensitive).
static inline lcstr_t llsnCHRM(
    lcstr_t s, const lchar_t c, size_t sn, int m) {
    if (!s || !m) return (lstr_t)NULL;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t len = lcsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (lstr_t)len;
    lchar_t uc = tolupper(c);
    if (m > 0) {
        size_t k = 0;
        lcstr_t p;
        for (p = s; p <= s + len; ++p) {
            if (tolupper(*p) == uc) {
                if (++k == (size_t)m) return p;
            }
        }
    } else if (m < 0) {
        size_t k = 0;
        lcstr_t p;
        for (p = s + len; p >= s; --p) {
            if (tolupper(*p) == uc && ++k == (size_t)(-m)) return p;
            if (p == s) break;
        }
    }
    return (lstr_t)NULL;
}
static inline lcstr_t lusnCHRM(
    lcstr_t s, const uchar_t c, size_t sn, int m) {
    if (!s || !m) return (lstr_t)NULL;
    if ((size_t)c > LUB_MAX_LCHAR) return (lstr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t len = lcsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (lstr_t)len;
    lchar_t uc = tolupper((lchar_t)c);
    if (m > 0) {
        size_t k = 0;
        lcstr_t p;
        for (p = s; p <= s + len; ++p) {
            if (tolupper(*p) == uc) {
                if (++k == (size_t)m) return p;
            }
        }
    } else if (m < 0) {
        size_t k = 0;
        lcstr_t p;
        for (p = s + len; p >= s; --p) {
            if (tolupper(*p) == uc && ++k == (size_t)(-m)) return p;
            if (p == s) break;
        }
    }
    return (lstr_t)NULL;
}
static inline ucstr_t ulsnCHRM(
    ucstr_t s, const lchar_t c, size_t sn, int m) {
    if (!s || !m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t len = ucsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (ustr_t)len;
    uchar_t uc = touupper((uchar_t)c);
    if (m > 0) {
        size_t k = 0;
        ucstr_t p;
        for (p = s; p <= s + len; ++p) {
            if (touupper(*p) == uc) {
                if (++k == (size_t)m) return p;
            }
        }
    } else if (m < 0) {
        size_t k = 0;
        ucstr_t p;
        for (p = s + len; p >= s; --p) {
            if (touupper(*p) == uc && ++k == (size_t)(-m)) return p;
            if (p == s) break;
        }
    }
    return (ustr_t)NULL;
}
static inline ucstr_t uusnCHRM(
    ucstr_t s, const uchar_t c, size_t sn, int m) {
    if (!s || !m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t len = ucsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return (ustr_t)len;
    uchar_t uc = touupper(c);
    if (m > 0) {
        size_t k = 0;
        ucstr_t p;
        for (p = s; p <= s + len; ++p) {
            if (touupper(*p) == uc) {
                if (++k == (size_t)m) return p;
            }
        }
    } else if (m < 0) {
        size_t k = 0;
        ucstr_t p;
        for (p = s + len; p >= s; --p) {
            if (touupper(*p) == uc && ++k == (size_t)(-m)) return p;
            if (p == s) break;
        }
    }
    return (ustr_t)NULL;
}
/** @} */

 * @defgroup StringSpan String Span
 * @name llsnspn, lusnspn, ulsnspn, uusnspn (case-sensitive)
 *       llsnSPN, lusnSPN, ulsnSPN, uusnSPN (case-insensitive)
 * @brief Initial-span length where characters in
 *        string s1 are all in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string.
 * @param sn Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Span length, or MISSING_TERMINATOR on source error.
 * @note Errors:
 *       - LUB_MISSING_TERMINATOR if s1 or s2 is not null-terminated by the specified bound.
 * @{
 */

// String span (case-sensitive).
static inline size_t llsnspn(lcstr_t s1, lcstr_t s2, size_t sn) {
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    size_t i, k;
    for (i = 0; i < s1_len; ++i) {
        int in_set = 0;
        for (k = 0; k < s2_len; ++k) {
            if (s1[i] == s2[k]) {in_set = 1; break;}
        }
        if (!in_set)
            return i;
    }
    return s1_len;
}
static inline size_t lusnspn(lcstr_t s1, ucstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    size_t i, k;
    for (i = 0; i < s1_len; ++i) {
        int in_set = 0;
        uchar_t s1c = (uchar_t)s1[i];
        for (k = 0; k < s2_len; ++k) {
            if (s1c == s2[k]) {in_set = 1; break;}
        }
        if (!in_set)
            return i;
    }
    return s1_len;
}
static inline size_t ulsnspn(ucstr_t s1, lcstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    size_t i, k;
    for (i = 0; i < s1_len; ++i) {
        int in_set = 0;
        for (k = 0; k < s2_len; ++k) {
            if (s1[i] == (uchar_t)s2[k]) {in_set = 1; break;}
        }
        if (!in_set)
            return i;
    }
    return s1_len;
}
static inline size_t uusnspn(ucstr_t s1, ucstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    size_t i, k;
    for (i = 0; i < s1_len; ++i) {
        int in_set = 0;
        for (k = 0; k < s2_len; ++k) {
            if (s1[i] == s2[k]) {in_set = 1; break;}
        }
        if (!in_set)
            return i;
    }
    return s1_len;
}

// String span (case-insensitive)
static inline size_t llsnSPN(lcstr_t s1, lcstr_t s2, size_t sn) {
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    size_t i, k;
    for (i = 0; i < s1_len; ++i) {
        int in_set = 0;
        lchar_t s1c = tolupper(s1[i]);
        for (k = 0; k < s2_len; ++k) {
            if (s1c == tolupper(s2[k])) {in_set = 1; break;}
        }
        if (!in_set)
            return i;
    }
    return s1_len;
}
static inline size_t lusnSPN(lcstr_t s1, ucstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    size_t i, k;
    for (i = 0; i < s1_len; ++i) {
        int in_set = 0;
        uchar_t s1c = touupper((uchar_t)s1[i]);
        for (k = 0; k < s2_len; ++k) {
            if (s1c == touupper(s2[k])) {in_set = 1; break;}
        }
        if (!in_set)
            return i;
    }
    return s1_len;
}
static inline size_t ulsnSPN(ucstr_t s1, lcstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    size_t i, k;
    for (i = 0; i < s1_len; ++i) {
        int in_set = 0;
        uchar_t s1c = touupper(s1[i]);
        for (k = 0; k < s2_len; ++k) {
            if (s1c == touupper((uchar_t)s2[k])) {in_set = 1; break;}
        }
        if (!in_set)
            return i;
    }
    return s1_len;
}
static inline size_t uusnSPN(ucstr_t s1, ucstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    size_t i, k;
    for (i = 0; i < s1_len; ++i) {
        int in_set = 0;
        uchar_t s1c = touupper(s1[i]);
        for (k = 0; k < s2_len; ++k) {
            if (s1c == touupper(s2[k])) {in_set = 1; break;}
        }
        if (!in_set)
            return i;
    }
    return s1_len;
}
/** @} */

/**
 * @defgroup CharacterSpan Character Span
 * @name llsncspn, lusncspn, ulsncspn, uusncspn (case-sensitive)
 *       llsnCSPN, lusnCSPN, ulsnCSPN, uusnCSPN (case-insenstive)
 * @brief Initial-span length where characters
 *        in string s1 are not in string s2.
 * @param s1 Source string to scan.
 * @param s2 Character-set string. String of characters to match for the search.
 * @param sn Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Span length, or error.
 * @note Errors:
 *      - LUB_MISSING_TERMINATOR if s1 or s2 is not null-terminated by the specified bound.
  * @{
 */

// Character span (case-sensitive).
static inline size_t llsncspn(lcstr_t s1, lcstr_t s2, size_t sn) {
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    for (size_t i = 0; i < s1_len; ++i) {
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1[i] == s2[k])
                return i;
        }
    }
    return s1_len;
}
static inline size_t lusncspn(lcstr_t s1, ucstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    for (size_t i = 0; i < s1_len; ++i) {
        uchar_t s1c = (uchar_t)s1[i];
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1c == s2[k])
                return i;
        }
    }
    return s1_len;
}
static inline size_t ulsncspn(ucstr_t s1, lcstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    for (size_t i = 0; i < s1_len; ++i) {
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1[i] == (uchar_t)s2[k])
                return i;
        }
    }
    return s1_len;
}
static inline size_t uusncspn(ucstr_t s1, ucstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    for (size_t i = 0; i < s1_len; ++i) {
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1[i] == s2[k])
                return i;
        }
    }
    return s1_len;
}
/* @}*/

// Character span (case-insensitive).
static inline size_t llsnCSPN(lcstr_t s1, lcstr_t s2, size_t sn) {
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    for (size_t i = 0; i < s1_len; ++i) {
        lchar_t s1c = tolupper(s1[i]);
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1c == tolupper(s2[k]))
                return i;
        }
    }
    return s1_len;
}
static inline size_t lusnCSPN(lcstr_t s1, ucstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    for (size_t i = 0; i < s1_len; ++i) {
        uchar_t s1c = touupper((uchar_t)s1[i]);
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1c == touupper(s2[k]))
                return i;
        }
    }
    return s1_len;
}
static inline size_t ulsnCSPN(ucstr_t s1, lcstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    for (size_t i = 0; i < s1_len; ++i) {
        uchar_t s1c = touupper(s1[i]);
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1c == touupper((uchar_t)s2[k]))
                return i;
        }
    }
    return s1_len;
}
static inline size_t uusnCSPN(ucstr_t s1, ucstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
        return MISSING_TERMINATOR;
    for (size_t i = 0; i < s1_len; ++i) {
        uchar_t s1c = touupper(s1[i]);
        for (size_t k = 0; k < s2_len; ++k) {
            if (s1c == touupper(s2[k]))
                return i;
        }
    }
    return s1_len;
}
/** @} */

/**
 * @defgroup Count Count
 * @name llsncnt, lusncnt, ulsncnt, uusncnt
 *       llsnCNT, lusnCNT, ulsnCNT, uusnCNT
 * @brief Count characters in a string based on a set of characters.
 * @param s String to search.
 * @param sn Bound for the source string (clamped to MAX_LSTRLEN or
 *          MAX_USTRLEN) for bounded functions. For default-bounded
 *          functions, sn defaults to MAX_LSTRLEN or MAX_USTRLEN.
 * @param set String of characters to match for the search. Must be
 *            null-terminated and have length within the specified or default bound.
 * @return Number of characters in the span, or -1 if an error occurs.
 * @note For functions with 'SPN' in the name, character matching is case-sensitive.
 *       For functions with 'SPNM' in the name, character matching is case-insensitive
 * @note Count is the number of characters in s that match any character
 *       in set.
 * @note For functions with 'CSPN' in the name, the span includes characters in s that do not match any character in set.
 *       For functions with 'CNT' in the name, the count is the number of characters
 *       in s that do not match any character in set.

 * @name llsncnt, lusncnt, ulsncnt, and uusncnt
 * @brief Bounded case-sensitive count of matches of character c in string s.
 * @param s Source string.
 * @param c Character to count.
 * @param n Bound on source clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Number of matches in s, or MISSING_TERMINATOR on source error.
 * @note The null terminator is not included in the count.
 * @{
 */

// Count of matches of character c in string s (case-sensitive).
static inline size_t llsncnt(lcstr_t s, const lchar_t c, size_t sn) {
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t len = lcsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return len;
    size_t k = 0;
    for (size_t i = 0; i < len; ++i) if (s[i] == c) ++k;
    return k;
}
static inline size_t lusncnt(lcstr_t s, const uchar_t c, size_t sn) {
    if ((size_t)c > LUB_MAX_LCHAR) return (size_t)0;
    return llsncnt(s, (lchar_t)c, sn > MAX_USTRLEN ? MAX_USTRLEN : sn);
}
static inline size_t ulsncnt(ucstr_t s, const lchar_t c, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t len = ucsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return len;
    size_t k = 0;
    uchar_t uc = (uchar_t)c;
    for (size_t i = 0; i < len; ++i) if (s[i] == uc) ++k;
    return k;
}
static inline size_t uusncnt(ucstr_t s, const uchar_t c, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t len = ucsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return len;
    size_t k = 0;
    for (size_t i = 0; i < len; ++i) if (s[i] == c) ++k;
    return k;
}

// Count of matches of character c in string s (case-insensitive).
static inline size_t llsnCNT(lcstr_t s, const lchar_t c, size_t sn) {
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t len = lcsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return len;
    size_t k = 0;
    lchar_t uc = tolupper(c);
    for (size_t i = 0; i < len; ++i) if (tolupper(s[i]) == uc) ++k;
    return k;
}
static inline size_t lusnCNT(lcstr_t s, const uchar_t c, size_t sn) {
    if ((size_t)c > LUB_MAX_LCHAR) return (size_t)0;
    return llsnCNT(s, (lchar_t)c, sn > MAX_USTRLEN ? MAX_USTRLEN : sn);
}
static inline size_t ulsnCNT(ucstr_t s, const lchar_t c, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t len = ucsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return len;
    size_t k = 0;
    uchar_t uc = touupper((uchar_t)c);
    for (size_t i = 0; i < len; ++i) if (touupper(s[i]) == uc) ++k;
    return k;
}
static inline size_t uusnCNT(ucstr_t s, const uchar_t c, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t len = ucsnlen(s, sn);
    if (len >= LUB_SIZE_ERRORS) return len;
    size_t k = 0;
    uchar_t uc = touupper(c);
    for (size_t i = 0; i < len; ++i) if (touupper(s[i]) == uc) ++k;
    return k;
}
/** @} */

/**
 * @defgroup SubstringSearchFunctions Substring Search
 * @name llsnstrm, lusnstrm, ulsnstrm, uusnstrm (case-sensitive)
 *       llsnSTRM, lusnSTRM, ulsnSTRM, uusnSTRM (case-insensitive)
 * @brief Search for the mth occurrence of string s2 in string s1.
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param sn Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index (1 = first, 2 = second, ...). 0 returns NULL.
 *          mth occurrence for matching. m > 0 counts from the
 *          beginning of the string (1 means first occurrence).
 *          m == 0 returns NULL. m < 0 counts from the end of the string
 *          (-1 means last occurrence) when the operation supports reverse selection.
 * @return Pointer to mth match in s1, NULL if not found,  or error.
 * @note Errors:
 *       - LUB_MISSING_TERMINATOR if s1 or s2 is not null-terminated by the specified bound.
 * * @{
 */

 // Substring search (case-sensitive).
static inline lcstr_t llsnstrm(
    lcstr_t s1, lcstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (lstr_t)NULL;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (lstr_t)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (lstr_t)s2_len;
    if (!s2_len)
        return m == 1 ? s1 : (lstr_t)NULL;
    if (s2_len > s1_len) return (lstr_t)NULL;
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
    lcstr_t s1, ucstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (lstr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (lstr_t)s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (lstr_t)s2_len;
    if (!s2_len) return m == 1 ? s1 : (lstr_t)NULL;
    if (s2_len > s1_len) return (lstr_t)NULL;
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
    ucstr_t s1, lcstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (ustr_t)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (ustr_t)s2_len;
    if (!s2_len) return m == 1 ? s1 : (ustr_t)NULL;
    if (s2_len > s1_len) return (ustr_t)NULL;
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
    ucstr_t s1, ucstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (ustr_t)s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (ustr_t)s2_len;
    if (!s2_len) return m == 1 ? s1 : (ustr_t)NULL;
    if (s2_len > s1_len) return (ustr_t)NULL;
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

// Substring search (case-insensitive).
static inline lcstr_t llsnSTRM(
    lcstr_t s1, lcstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m)
        return (lstr_t)NULL;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (lstr_t)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (lstr_t)s2_len;
    if (!s2_len) return m == 1 ? s1 : (lstr_t)NULL;
    if (s2_len > s1_len) return (lstr_t)NULL;
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
static inline lcstr_t lusnSTRM(
    lcstr_t s1, ucstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (lstr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (lstr_t)s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (lstr_t)s2_len;
    if (!s2_len) return m == 1 ? s1 : (lstr_t)NULL;
    if (s2_len > s1_len) return (lstr_t)NULL;
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
static inline ucstr_t ulsnSTRM(
    ucstr_t s1, lcstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (ustr_t)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (ustr_t)s2_len;
    if (!s2_len) return m == 1 ? s1 : (ustr_t)NULL;
    if (s2_len > s1_len) return (ustr_t)NULL;
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
static inline ucstr_t uusnSTRM(
    ucstr_t s1, ucstr_t s2, size_t sn, size_t m) {
    if (!s1 || !s2 || !m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (ustr_t)s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (ustr_t)s2_len;
    if (!s2_len) return m == 1 ? s1 : (ustr_t)NULL;
    if (s2_len > s1_len) return (ustr_t)NULL;
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
/** @} */

/**
 * @name llsnrstrm, lusnrstrm, ulsnrstrm, and uusnrstrm
 * @brief Case-sensitive reverse search for the mth-from-last
 * match of string s2 in string s1.
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param sn Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index from the end (1 = last, 2 = second-last, ...).
 * @return Pointer to mth-from-last match in s1, or NULL if not found or source
 * error.
 * @}
 */

// Reverse substring search (case-sensitive).
static inline lcstr_t llsnrstrm(
    lcstr_t s1, lcstr_t s2, size_t sn, size_t m) {
    if (!m)
        return (lstr_t)NULL;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (lstr_t)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (lstr_t)s2_len;
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
    lcstr_t s1, ucstr_t s2, size_t sn, size_t m) {
    if (!m) return (lstr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (lstr_t)s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (lstr_t)s2_len;
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
    ucstr_t s1, lcstr_t s2, size_t sn, size_t m) {
    if (!m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (ustr_t)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (ustr_t)s2_len;
    if (!s2_len)
        return m == 1 ? (s1 ? (s1 + s1_len) : (ustr_t)NULL) : (ustr_t)NULL;
    if (s2_len > s1_len) return (ustr_t)NULL;
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
    ucstr_t s1, ucstr_t s2, size_t sn, size_t m) {
    if (!m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (ustr_t)s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (ustr_t)s2_len;
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
/** @} */

/**
 * @name llsnRSTRM, lusnRSTRM, ulsnRSTRM, and uusnRSTRM
 * @brief Case-insensitive reverse search for the mth-from-last
 *        match of string s2 in string s1.
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param sn Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @param m Occurrence index from the end (1 = last, 2 = second-last, ...).
 * @return Pointer to mth-from-last match in s1, or NULL if not found or source
 * error.
 * @{
 */

// Reverse string search (case-insensitive).
static inline lcstr_t llsnRSTRM(
    lcstr_t s1, lcstr_t s2, size_t sn, size_t m)
{   if (!m) return (lstr_t)NULL;
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (lstr_t)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (lstr_t)s2_len;
    if (!s2_len)
        return m == 1 ? (s1 ? (s1 + s1_len) : (lstr_t)NULL) : (lstr_t)NULL;
    if (s2_len > s1_len) return (lstr_t)NULL;
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
static inline lcstr_t lusnRSTRM(
    lcstr_t s1, ucstr_t s2, size_t sn, size_t m) {
    if (!m) return (lstr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (lstr_t)s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (lstr_t)s2_len;
    if (!s2_len)
        return m == 1 ? (s1 ? (s1 + s1_len) : (lstr_t)NULL) : (lstr_t)NULL;
    if (s2_len > s1_len) return (lstr_t)NULL;
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
static inline ucstr_t ulsnRSTRM(
    ucstr_t s1, lcstr_t s2, size_t sn, size_t m) {
    if (!m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (ustr_t)s1_len;
    size_t s2_len = lcsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (ustr_t)s2_len;
    if (!s2_len)
        return m == 1 ? (s1 ? (s1 + s1_len) : (ustr_t)NULL) : (ustr_t)NULL;
    if (s2_len > s1_len) return (ustr_t)NULL;
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
static inline ucstr_t uusnRSTRM(
    ucstr_t s1, ucstr_t s2, size_t sn, size_t m) {
    if (!m) return (ustr_t)NULL;
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = ucsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return (ustr_t)s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return (ustr_t)s2_len;
    if (!s2_len)
        return m == 1 ? (s1 ? (s1 + s1_len) : (ustr_t)NULL) : (ustr_t)NULL;
    if (s2_len > s1_len) return (ustr_t)NULL;
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
/** @} */

/**
 * @defgroup CountSubstrings Count Substrings
 * @name llsnsubcnt, lusnsubcnt, ulsnsubcnt, uusnsubcnt (case-sensitive)
 *       llsnSUBCNT, lusnSUBCNT, ulsnSUBCNT, uusnSUBCNT (case-insensitive)
 * @brief Count of matches of string s2 in string s1
 *        (overlapping matches are counted).
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param sn Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Count of matches, or error.
 * @note Errors;
 *       - LUB_MISSING_TERMINATOR if s1 or s2 is not null-terminated by the specified bound.
 * @note If s2 is empty, returns 0.
 * @{
 */

// Count of matches of string s2 in string s1 (case-sensitive).
static inline size_t llsnsubcnt(lcstr_t s1, lcstr_t s2, size_t sn) {
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
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
static inline size_t lusnsubcnt(lcstr_t s1, ucstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s1_len >= LUB_SIZE_ERRORS) s2_len;
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
static inline size_t ulsnsubcnt(ucstr_t s1, lcstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
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
static inline size_t uusnsubcnt(ucstr_t s1, ucstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
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
/* @}*/

/**
 * @name llsnSUBCNT, lusnSUBCNT, ulsnSUBCNT, and uusnSUBCNT
 * @brief Bounded case-insensitive count of matches of string s2 in string s1
 * (overlapping matches are counted).
 * @param s1 Haystack string.
 * @param s2 Needle string.
 * @param sn Bound on s1 and s2 (clamped to MAX_LSTRLEN or MAX_USTRLEN).
 * @return Count of matches, or MISSING_TERMINATOR on source error.
 * @note If s2 is empty, returns 0.
 * @{
 */
static inline size_t llsnSUBCNT(lcstr_t s1, lcstr_t s2, size_t sn) {
    if (sn > MAX_LSTRLEN) sn = MAX_LSTRLEN;
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
static inline size_t lusnSUBCNT(lcstr_t s1, ucstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
    size_t s1_len = lcsnlen(s1, sn);
    if (s1_len >= LUB_SIZE_ERRORS) return s1_len;
    size_t s2_len = ucsnlen(s2, sn);
    if (s2_len >= LUB_SIZE_ERRORS) return s2_len;
    if (!s2_len || s2_len > s1_len) return (size_t)0;
    size_t c = 0;
    for (size_t i = 0; i <= s1_len - s2_len; ++i) {
        size_t k = 0;
        for (; k < s2_len &&
               touupper((uchar_t)s1[i + k]) == touupper(s2[k]); ++k);
        if (k == s2_len) ++c;
    }
    return c;
}
static inline size_t ulsnSUBCNT(ucstr_t s1, lcstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
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
static inline size_t uusnSUBCNT(ucstr_t s1, ucstr_t s2, size_t sn) {
    if (sn > MAX_USTRLEN) sn = MAX_USTRLEN;
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
/** @} */

/**
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
 * - Symmetry:       Every operation exists for every encoding direction.
 * - Clarity:        Function names encode direction/type, bound, operation,
 *                   and case (sensitive/insensitive/mapping).
 * - Safety:         Explicit/default bounds, terminator validation,
 *                   representability checks.
 * - Predictability: Behavior mirrors familiar C string patterns while
 *                   making bounds and error contracts explicit.
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
 * - bchar_t = uint8_t  (raw byte, values x'00'-x'FF', no null terminator semantics)
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
 * bchar_t
 *     Raw byte (uint8_t). Values x'00'-x'FF'. No null terminator semantics.
 * 
 * lstr_t
 *     Pointer to a null-terminated string (lstr_t) of Latin characters (lchar_t).
 * 
 * ustr_t
 *     Pointer to a null-terminated string (ustr_t) of Unicode characters (uchar_t).
 * 
 * bstr_t
 *     Pointer to a string (bstr_t) of bytes (bchar_t).
 * 
 * MAX_LSTRLEN
 *     Maximum number of characters in a Latin string (lstr_t).
 * 
 * MAX_USTRLEN
 *     Maximum number of characters in a Unicode string (ustr_t).
 * 
 * MAX_BSTRLEN
 *     Maximum number of bytes in a byte string (bstr_t).
 * 
 * MISSING_TERMINATOR
 *     Error returned on unterminated source with value: (size_t)-1,
 *     which is distinct from any valid string length.
 * 
 * err_c
 *     Substitution char for out-of-range Unicode->Latin conversions.
 * 
 * n
 *     Maximum source characters/bytes; for char strings, stops at null.
 * 
 * span length
 *     Number of leading characters in s1 that satisfy a function-specific condition.
 * 
 * unterminated
 *     String that does not contain a null terminator at or before
 *     the specified or default bound.
 */

#ifdef __cplusplus
}
#endif

// End of lubtype.h
