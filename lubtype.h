/**
 * @file lubtype.h
 * @mainpage Latin/Unicode/Byte API
 */

/**
 * @section Overview Overview
 *
 * This C/C++ API header provides Latin-8, Unicode-16, and byte types, plus
 * associated macros, extern function declarations (prototypes), and static
 * inline function definitions.
 * 
 * Functions include character classification/transformation,
 * string length/classification, option validation,
 * compare, fixed/prefix/suffix compare, ptrim, skip, search, count,
 * concatenate, copy, trim, reverse, pad, replace, and format.
 * 
 *   - Variants for Latin/Unicode/Byte <- Latin/Unicode/Byte.
 * 
 *   - Variants for explicit/default bounds on source length.
 * 
 *   - Explicit bound on target buffer size.
 * 
 *   - Explicit or default bound on source length.
 * 
 *   - Variants for character case-preserving, uppercase, lowercase mapping.
 * 
 *   - Case-sensitive/insensitive character matching.
 *
 * @note Not all variants are provided for all functions due to a
 *       variant not being meaningful/useful for a particular function
 *       or the variant being easily implemented by the caller
 *       using a combination of provided variants.
 */

/**
 * @copyright Copyright (c) 2026 paulsinclair51
 * @license SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

/**
 * @section HeaderUsage Header Usage
 *
 * Include lubtype.h in every source file that uses the API.
 *
 * @code
 * #include "lubtype.h"
 * @endcode
 *
 * The API is header-only, but function body definitions are emitted
 * only when the macro LUB_DEFINITIONS is defined. Define this macro
 * in exactly one source file per final binary. In practice, this
 * source file is usually a LUB_DEFINITIONS-only source file such
 * as the following:
 * @code
// File: lubdefinitions.c
// Brief: Provide function definitions for the LUB API.
#if !defined(LUB_DEFINITIONS)
#define LUB_DEFINITIONS
#endif
#include "lubtype.h"
// End of lubdefinitions.c
 * @endcode
 * This repository includes a sample source file as [lubdefinitions.c](lubdefinitions.c)
 * that expands on the example above.
 *
 * All other source files must include lubtype.h without defining LUB_DEFINITIONS
 * in the source file or in the compile command.
 *
 * @section PolymorphicMacrosUsage Polymorphic Macros Usage
 * To use the polymorphic (x) macros (see @ref PolymorphicMacros), first
 * validate the macros LUB_X_IS_L or LUB_X_IS_U prior to including this
 * header in the source file:
 * @code
#if defined(LUB_X_IS_L) && defined(LUB_X_IS_U)
#error "Both LUB_X_IS_L and LUB_X_IS_U are defined. "
       "Specify either -DLUB_X_IS_L or -DLUB_X_IS_U, but not both. "
       "If neither is defined, LUB_X_IS_L is defined as a "
       "validator-friendly default."
#endif
#if !defined(LUB_X_IS_L) && !defined(LUB_X_IS_U)
// Set default.
#define LUB_X_IS_L
#endif
 * @endcode
 * Secondly, names of non-static functions in the source file must be
 * defined based on LUB_X_IS_L or LUB_X_IS_U. For example:
 * @code
// Define xfunc as lfunc or ufunc based on LUB_X_IS_L or LUB_X_IS_U.
#if defined(LUB_X_IS_L) 
#define xfunc lfunc
#else
#define xfunc ufunc
#endif

int xfunc(...)
{ <function body> }
 * @endcode
 * Then compile using the define flag (-D) to define LUB_X_IS_L:
 * @code
gcc -DLUB_X_IS_L -c myfile.c -o myfile.o
 * @endcode
 * and compile again to define LUB_X_IS_U:
 * @code
gcc -DLUB_X_IS_U -c myfile.c -o myfile.o
 * @endcode
 */

/**
 * @section GuidingPrinciples Guiding Principles
 *
 * - Symmetry:       Functions exist for every encoding direction
 *                   except where explicitly noted. For example,
 *                   Latin (l) <- Unicode (u) variants are not provided
 *                   for comparison search functions since a Unicode character
 *                   outside the range of Latin would not match.
 *
 * - Clarity:        Function names encode direction/type, bound,
 *                   operation, and case (sensitive/insensitive,
 *                   preserving/lowercase/uppercase).
 *
 * - Safety:         Explicit/default bounds, terminator validation,
 *                   representability checks, error checking.
 *
 *                   Casts to lchar_t and uchar_t include
 *                   explicit bounds checks.
 *
 *                   The API is thread-safe provided threads do not share
 *                   target buffers without external synchronization.
 *                   Character classification relies on <ctype.h> and
 *                   <wctype.h>, which are thread-safe when the locale is not
 *                   modified.
 *
 * - Predictability: Behavior mirrors familiar C string patterns while
 *                   making bounds, defined (instead of undefined or
 *                   implementation-defined) return values, error
 *                   values.
 *
 *                   Comparison functions operate strictly on raw code units.
 *                   They do not apply character substitution,
 *                   normalization, or any mapping beyond the specified case
 *                   (sensitive or insensitive) rules. This ensures
 *                   predictable, strcmp‑like ordering and keeps comparison
 *                   return values (-1, 0, +1) distinct from the reserved
 *                   error range (-99 to -2).
 *
 * - Portability:    Only platforms that have include files <stddef.h>,
 *                   <stdint.h>, <ctype.h>, <string.h>, <stdarg.h>, <stdio.h>,
 *                   <wchar.h>, and <wctype.h> are supported.
 *
 *                   Fixed-width types, wchar_t size must be 2 or 4 bytes.
 *                   No API-managed locale state. However, Unicode
 *                   classification and case conversion use C runtime
 *                   wide-character routines (isw* and tow*) via wchar_t
 *                   casts and, therefore, results are locale- and
 *                   CRT-dependent and results may differ across platforms
 *                   or locale settings.
 *
 *                   This API relies on two’s‑complement integer
 *                   representation and on pointer–integer round‑tripping for
 *                   error‑value encoding. Platforms must support casting
 *                   between int, size_t, intptr_t, and void * without loss of
 *                   bit patterns for values in the range -99 to -2. These
 *                   requirements are validated at compile time via static
 *                   assertions; platforms that do not satisfy them are not
 *                   supported.
 *
 * - Compatibility:  Usable with both C and C++ with compatibility
 *                   with C90 (ANSI C) compilers. No C11 features
 *                   are required.
 * 
 * @warning This API does not perform Unicode normalization or surrogate pair
 *          handling; all operations are on individual code units.
 */

/**
 * @section APINotes API Notes
 *
 * @note For functions with a target buffer t parameter and t is not NULL,
 *       the target buffer is always null-terminated on error. This avoids
 *       subsequent buffer overreads.
 *
 * @note Some search and replace functions (e.g., search, replace) may have
 *       worst-case O(n*m) complexity, where n is the length of the input and
 *       m is the pattern or map size. Most other operations are O(n).
 *
 * @note When appropriate, functions are `static inline`.
 *
 * @note Overlapping source and target buffers result in
 *       implementation‑defined behavior; functions may return an error or
 *       produce a correct result depending on the operation.
 *
 * @note No dynamic memory is allocated or freed, and no recursion occurs.
 *
 * @note This header was reviewed and refined with assistance
 *       from Microsoft Copilot.
 */

/**
 * @section Assumptions Assumptions
 *
 * - lchar_t = uint8_t (Latin-8, values 1-255, 0 reserved for null terminator).
 *
 * - uchar_t = uint16_t (Unicode BMP, values 1-65535, 0 reserved
 *   for null terminator). No normalization, surrogate handling,
 *   or multi-code-unit processing. All operations act on
 *   individual characters only.
 *
 * - byte_t = uint8_t (raw byte, values x'00'-x'FF', no null
 *   terminator semantics).
 */

 /**
 * @section ReservedNames Reserved Names
 * Besides the API provided type names and function names
 * (see @ref FunctionNamingConventions), the API reserves
 * names of the following forms:
 * 
 *   LUB_<W>[_<W>]... for macro names
 *   lub_<w>[_<w>]... for functions names
 * 
 * where <W> is one or more uppercase letters and <w> is one or more
 * lowercase letters for current and future use.
 * 
 * Macro names:
 * 
 *   1. If a macro name does not end with _DEF, it is for an API
 *      visible and documented macro (e.g., LUB_DEFINITIONS, LUB_VERSION,
 *      LUB_MAX_LCHAR, LUB_PASTE). These macros may be optionally #define'd
 *      with a different name and #undef'ed after including this header to
 *      avoid a name conflict as needed.
 * 
 *   2. Otherwise, the macro is only used when
 *      LUB_DEFINITIONS is defined before including this header (e.g., LUB_OP_DEF).
 *      If LUB_DEFINITIONS is defined, these macros must not be defined
 *      before including this header. They are undefined at the end of this header.
 * 
 * Functions names:
 * 
 *   1. If a function name ends with _ext, it is for an extern
 *     `function that is visible to API users but is not documented as
 *      part of the public API (e.g., lub_cat_cpy_pad_ext). Their
 *      definition may change with only a minor version number increase.
 *
 *    2. If a function name ends with _def, it is for a static or static
 *       inline function that is only visible when macro LUB_DEFINITIONS
 *       functions is defined (e.g., lub_get_def and lub_set_ith_def).
 *       These  functions are documented and their definitions may
 *       change with only a patch version number increase.
 * 
 *    3. Otherwise, names with the second form are reserved for future use.
 * 
 * With these conventions, the likelihood of a name conflict is low. However, 
 *
 *   - For macros, if a conflict does arise with a name
 *     when this header is included, the conflict must be resolved by
 *     an #undef of the macro before including this header and, if necessary,
 *     defining the macro again after including this header. Note that,
 *     a macro, other than the LUB_DEFINITIONS macro, is
 *     asserted that it is not defined prior to its #define in the
 *     header and, for a macro with a name ending in _DEF, an #undef of the macro
 *     occurs prior to the end of the header. 
 * 
 *   - For types and functions, if a conflict does arise with a name
 *     when this header is included, the conflict must be resolved by
 *     renaming the function in the code that includes this header. It is
 *     not recommended to resolve the conflict by renaming the type or function
 *     in this header and updating all references to it in this header
 *     since this change may require a major version number increase
 *     and potential incompatibilities for existing code.
 * 
 * If use of one of the lub_*_ext functions is needed
 * (for instance, to use in  a wrapper that implements defaults for
 * certain parameters), then provide a wrapper that references,
 * preferably, a public API function or, if necessary,
 * a lub_*_ext function. For example, an llstrim wrapper that has default
 * values for tn and sn parameters and references the API llsnntrim function
 * passing the defaults.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <wchar.h> 
#include <wctype.h>

// Allow functions to be invoked from C++.
#if defined(__cplusplus)
extern "C" {
#endif

#if defined(LUB_PASTE) || defined(LUB_TOKEN_PASTE)
#error "lubtype.h: A LUB_PASTE or LUB_TOKEN_PASTE " \
       "macro is unexpectedly already defined. " \
       "#undef before including lubtype.h."
#endif // defined macros

#if defined(LUB_STRINGIFY) || defined(LUB_TOKEN_STRINGIFY)
#error "lubtype.h: A LUB_STRINGIFY or LUB_TOKEN_STRINGIFY " \
       "macro is unexpectedly already defined. " \
       "#undef before including lubtype.h."
#endif // defined macros

/**
 * @section UtilityMacros Utility Macros
 */

/**
 * @defgroup Paste Paste
 * @name LUB_PASTE
 * @brief Macro for pasting two expanded tokens together.
 * @param a First token,
 * @param b Second token.
 * @return The result of pasting the expanded values of the
 *         two tokens together to form a single token.
 * 
 * @note Tokens a and b must each expand to a single token.
 * 
 * @note Macro LUB_TOKEN_PASTE is provided to
 *       implement expanding the tokens for LUB_PASTE.
 *       It is not intended for direct use.
 * 
 * @note A compile error is raised if either of these 
 *       macros are already defined before including this
 *       header.
 * @{
 */

// Paste tokens without expanding.
#define LUB_TOKEN_PASTE(a, b) a##b
// Expand tokens before pasting.
#define LUB_PASTE(a, b) LUB_TOKEN_PASTE(a, b)

/** @} */

/**
 * @defgroup Stringify Stringify
 * @name LUB_STRINGIFY
 * @brief Macro for stringifying a token.
 * @param s Token.
 * @return The result of stringifying the expanded token
 *         as a string literal.
 * 
 * @note Token s must expand to a single token.
 * 
 * @note Macro LUB_TOKEN_STRINGIFY is provided to
 *       implement expanding the token for LUB_STRINGIFY.
 *       It is not intended for direct use.
 * 
 * @note A compile error is raised if either of these macros
 *       are already defined before including this header.
 * @{
 */

// Stringify token without expanding.
#define LUB_TOKEN_STRINGIFY(s) #s
// Expand token before stringifying.
#define LUB_STRINGIFY(s) LUB_TOKEN_STRINGIFY(s)

/** @} */

/**
 * @defgroup StaticAssert Static Assert
 * @name LUB_STATIC_ASSERT
 * @brief Compile-time (static) assertion macro.
 * @param cond Condition to be asserted.
 * @param msg A single token (message) to be displayed if
 *            the assertion fails.
 * @return Compiler error if the assertion fails. Otherwise,
 *         a type is defined which can be ignored and a compiler
 *         error is not raised.
 *
 * @note A compile error is raised if this macro is already
 *       defined before including this header.
 * 
 * @example
 *   LUB_STATIC_ASSERT(sizeof(int) == 4, int_must_be_4_bytes);
 * 
 * Expands (C99) if true (and the type is not already defined) to:
 *   typedef char LUB_STATIC_ASSERT_int_must_be_4_bytes[1];
 *   // Assertion satisfied; compiler error not raised.
 *   // The defined type can be ignored.
 *
 * Or if false to:
 *   typedef char LUB_STATIC_ASSERT_int_must_be_4_bytes[-1];
 *   // Assertion fails; compiler error raised due to
 *   // invalid typedef statement.
 * @{
 */

#if defined(LUB_STATIC_ASSERT)
#error "lubtype.h: A LUB_STATIC_ASSERT macro is "\
       "unexpectedly already defined. " \
       "#undef before including lubtype.h."
#endif // defined macro

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
// C11 and later: use the built‑in assertion macro.
#define LUB_STATIC_ASSERT(cond, msg) _Static_assert(cond, #msg)
#else
// C99: typedef with invalid negative array size if assertion not satisfied.
#define LUB_STATIC_ASSERT(cond, msg) \
  typedef char LUB_PASTE(LUB_STATIC_ASSERT_, msg)[(cond) ? 1 : -1]
#endif

/** @} */

/**
 * @section VersioningMacros Versioning Macros
 */

/**
 * @defgroup LUBAPIVersioning LUB API Versioning
 * @name LUB_VERSION_MAJOR, LUB_VERSION_MINOR, LUB_VERSION_PATCH,
 *       LUB_VERSION, LUB_VERSION_NUM, LUB_VERSION_HEX, 
 *       LUB_VERSION_EQ, LUB_VERSION_AT_LEAST
 * @brief Versioning macros for the LUB API (lubtype.h):
 * 
 * LUB_VERSION_MAJOR
 *    Major version number, incremented for incompatible API changes.
 *    Numeric form, e.g., 1 for major version 1.
 * 
 * LUB_VERSION_MINOR
 *    Minor version number, incremented for backward-compatible additions.
 *    Numeric form, e.g., 0 for minor version 0,
 *    or 22 for minor version 22.
 * 
 * LUB_VERSION_PATCH
 *    Patch version number, incremented for bug fixes or internal improvements.
 *    Numeric form, e.g., 0 for patch version 0,
 *    or 12 for patch version 12.
 * 
 * LUB_VERSION
 *    String form, e.g., "1.0.0" for major version 1, minor
 *    version 0, patch version 0.
 * 
 * LUB_VERSION_NUM
 *    uint32_t form MMmmpp for comparisons, e.g., 10000 for
 *    version 1.0.0, 10200 for version 1.2.0, or 11212 for version 1.12.12.
 * 
 * LUB_VERSION_HEX
 *    Hexadecimal form 0xMMmmpp for display/debugging, e.g.,
 *    0x010000 for version 1.0.0, 0x010200 for version 1.2.0,
 *    or 0x011212 for version 1.12.12.
 * 
 * LUB_VERSION_EQ(maj,min,pat)
 *    True if current version is exactly maj.min.pat
 *
 * LUB_VERSION_AT_LEAST(maj,min,pat)
 *    True if current version is at least maj.min.pat.
 *
 * @note A compiler error is raised if any of the versioning macros
 *       are already defined before including lubtype.h.
 *
 * @note The naming conventions, error semantics, and safety guarantees
 *       are part of the documented and stable API and will not change without a
 *       major version increment.
 * @{
 */

#if defined(LUB_VERSION_MAJOR) || defined(LUB_VERSION_MINOR) || \
    defined(LUB_VERSION_PATCH) || \
    defined(LUB_VERSION) || \
    defined(LUB_VERSION_NUM) || defined(LUB_VERSION_HEX) || \
    defined(LUB_VERSION_EQ) || defined(LUB_VERSION_AT_LEAST)
#error "lubtype.h: A LUB_VERSION_* macro is unexpectedly " \
       "already defined. #undef before including lubtype.h."
#endif // defined macros

// LUB API version major, minor, patch.
#define LUB_VERSION_MAJOR 1
#define LUB_VERSION_MINOR 0
#define LUB_VERSION_PATCH 0

#if defined(LUB_DEFINITIONS)

// Ensure major version is greater than 0.
LUB_STATIC_ASSERT((uint32_t)LUB_VERSION_MAJOR, major_version_not_zero);

// Ensure version components fit in the encoding fields.
LUB_STATIC_ASSERT((uint32_t)LUB_VERSION_MAJOR <= 99, major_fits_in_field);
LUB_STATIC_ASSERT((uint32_t)LUB_VERSION_MINOR <= 99, minor_fits_in_field);
LUB_STATIC_ASSERT((uint32_t)LUB_VERSION_PATCH <= 99, patch_fits_in_field);

#endif // LUB_DEFINITIONS

// LUB API version string in "major.minor.patch" format.
#define LUB_VERSION \
  (LUB_STRINGIFY(LUB_VERSION_MAJOR) "." \
   LUB_STRINGIFY(LUB_VERSION_MINOR) "." \
   LUB_STRINGIFY(LUB_VERSION_PATCH))

// LUB API version as an integer for comparisons.
#define LUB_VERSION_NUM \
    ((uint32_t)LUB_VERSION_MAJOR * 10000 + \
     (uint32_t)LUB_VERSION_MINOR * 100 + \
     (uint32_t)LUB_VERSION_PATCH)

// LUB API version encoded as 0xMMmmpp (major, minor, patch) for display/debug.
#define LUB_VERSION_HEX \
    (((uint32_t)LUB_VERSION_MAJOR << 16) | \
     ((uint32_t)LUB_VERSION_MINOR << 8) | \
     (uint32_t)LUB_VERSION_PATCH)

// True if the current LUB API version is the specified version.
#define LUB_VERSION_EQ(maj, min, pat) \
    (LUB_VERSION_NUM == (uint32_t)(maj) * 10000 + \
                        (uint32_t)(min) * 100 + \
                        (uint32_t)(pat))

// True if the current LUB API version is at least the specified version.
#define LUB_VERSION_AT_LEAST(maj, min, pat) \
    (LUB_VERSION_NUM >=  (uint32_t)(maj) * 10000 + \
                         (uint32_t)(min) * 100 + \
                         (uint32_t)(pat))

/** @} */ // End of Versioning.

/**
 * @section LUBTypes LUB Types
 */

#if defined(LUB_DEFINITIONS)

// Ensure wchar_t is 2 or 4 bytes.
LUB_STATIC_ASSERT(sizeof(wchar_t) == 2 || sizeof(wchar_t) == 4,
                        wchar_t_must_be_2_or_4_bytes);

                     
// Ensure int, size_t, void *, and intptr_t are compatible
// for casting error values.
LUB_STATIC_ASSERT((int)-2 == (int)(size_t)-2 &&
                             (int)-2 == (int)(intptr_t)-2 &&
                             (size_t)-2 == (size_t)(int)-2 &&
                             (size_t)-2 == (size_t)(intptr_t)-2 &&
                             (intptr_t)-2 == (intptr_t)(int)-2 &&
                             (intptr_t)-2 == (intptr_t)(size_t)-2 &&
                             (int)-99 == (int)(size_t)-99 &&
                             (int)-99 == (int)(intptr_t)-99 &&
                             (size_t)-99 == (size_t)(int)-99 &&
                             (size_t)-99 == (size_t)(intptr_t)-99 &&
                             (intptr_t)-99 == (intptr_t)(int)-99 &&
                             (intptr_t)-99 == (intptr_t)(size_t)-99,
                             error_values_must_be_compatible_across_types);
                             

#endif // LUB_DEFINITIONS

/**
 * @defgroup LatinCharacterType Latin Character Type
 * @name lchar_t
 * @brief Latin-8 character type (1 byte) 
 *        Base type: uint8_t, Values: 0-255 (0x00-0xFF).
 * @{
 */

#if defined(lchar_t)
#error "lubtype.h: A lchar_t macro is unexpectedly already defined. " \
       "#undef before including lubtype.h."
#endif // defined macro

typedef uint8_t lchar_t;

/** @} */

/**
 * @defgroup UnicodeCharacterType Unicode Character Type
 * @name uchar_t
 * @brief UTF-16 Unicode character type (2 bytes)
 *        Base type: uint16_t, Values: 0-65535 (0x0000-0xFFFF).
 * @{
 */

#if defined(uchar_t)
#error "lubtype.h: A uchar_t macro is unexpectedly already defined. " \
       "#undef before including lubtype.h."
#endif // defined macro

typedef uint16_t uchar_t;

/** @} */

/**
 * @defgroup ByteType Byte Type
 * @name byte_t
 * @brief Byte type (1 byte)
 *        Base type: uint8_t, Values: 0-255 (0x00-0xFF).
 * @{
 */

#if defined(byte_t)
#error "lubtype.h: A byte_t macro is unexpectedly already defined. " \
       "#undef before including lubtype.h."
#endif // defined macro

typedef uint8_t byte_t;

/** @} */


/**
 * @section LUBMacros LUB_* Macros
 */

/**
 * @defgroup CharacterLimits Character Limits
 * @name LUB_MAX_LCHAR, LUB_MAX_UCHAR, LUB_MAX_BYTE
 * @brief Maximum values for lchar_t, uchar_t, and byte_t.
 * @{
 */

#if defined(LUB_MAX_LCHAR) || defined(LUB_MAX_UCHAR) || defined(LUB_MAX_BYTE)
#error "lubtype.h: A character limit LUB_MAX_LCHAR, "
       "LUB_MAX_UCHAR, LUB_MAX_BYTE macro is unexpectedly "\
       "already defined. " \
       "#undef before including lubtype.h."
#endif // defined macros

#define LUB_MAX_LCHAR ((size_t)255)
#define LUB_MAX_UCHAR ((size_t)65535)
#define LUB_MAX_BYTE ((size_t)255)

/** @} */

/**
 * @defgroup StringLimits String Limits
 * @name LUB_MAX_LSTRLEN, LUB_MAX_USTRLEN,
 *       LUB_MAX_NAMELEN, LUB_MAX_QNAMELEN,
 *       LUB_MAX_LOPTLEN, LUB_MAX_UOPTLEN,
 *       LUB_MAX_BSTRLEN, LUB_MAX_BOPTLEN
 * @brief Maximum number of characters for an lchar_t string,
 * uchar_t string, name, quoted name, or option (not
 * including the null terminator).
 * @{
 */

#if defined(LUB_MAX_LSTRLEN) || defined(LUB_MAX_USTRLEN) || \
    defined(LUB_MAX_NAMELEN) || defined(LUB_MAX_QNAMELEN) || \
    defined(LUB_MAX_LOPTLEN) || defined(LUB_MAX_UOPTLEN) || \
    defined(LUB_MAX_BSTRLEN) || defined(LUB_MAX_BOPTLEN)
#error "lubtype.h: A string limit LUB_MAX_*LEN macro " \
       "is unexpectedly already defined. " \
       "#undef before including lubtype.h."
#endif // defined macros

#define LUB_MAX_LSTRLEN    ((size_t)1000000)
#define LUB_MAX_USTRLEN    ((size_t)500000)
#define LUB_MAX_NAMELEN    ((size_t)128)
#define LUB_MAX_QNAMELEN   ((size_t)(2*(LUB_MAX_NAMELEN) + 2))
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
 *
 * @note For int returning comparison functions:
 * 
 *         LUB_CMP_LESS_THAN (-1) indicates s1 < s2.
 *
 *         LUB_CMP_EQUAL (0) indicates s1 == s2.
 *
 *         LUB_CMP_GREATER_THAN (1) indicates s1 > s2.
 * 
 *       Note that this differs from the standard strcmp
 *       convention of returning an undefined negative
 *       value for s1 < s2 and an undefined positive
 *       value for s1 > s2. Returning specific values
 *       allows for error return values that are between -99 and -2
 *       that are distinct from valid comparison return values.
 *
 * @note Special values (-1, 0, +1) are intentionally outside the
 *       reserved error range (-99 to -2) and can never be misclassified
 *       as error values.
 * @{
 */

#if defined(LUB_CMP_GREATER_THAN) || \
    defined(LUB_CMP_EQUAL) || \
    defined(LUB_CMP_LESS_THAN)
#error "lubtype.h: A special value LUB_CMP_* " \
       "macro is unexpectedly already defined. " \
       "#undef before including lubtype.h."
#endif // defined macros

#if defined(LUB_QUOTEDNAME) || \
    defined(LUB_UNQUOTEDNAME)
#error "lubtype.h: A special value LUB_QUOTEDNAME " \
       "or LUB_UNQUOTEDNAME macro is unexpectedly already defined. " \
       "#undef before including lubtype.h. " \
       "After including, undef and define again as needed if " \
       "a LUB_*QUOTEDNAME definition is not required."
#endif // defined macros

//Special values for comparison result.
#define LUB_CMP_GREATER_THAN   ((int)1)
#define LUB_CMP_EQUAL          ((int)0)
#define LUB_CMP_LESS_THAN      ((int)-1)

// Special values for names.
#define LUB_QUOTEDNAME         ((int)1)
#define LUB_UNQUOTEDNAME       ((int)0)

/** @} */

/**
 * @defgroup ErrorValues Error Values
 * @name LUB_PTR_INVALID
 *       LUB_UNTERMINATED
 *       LUB_BSTR_TOO_LONG
 *       LUB_NAME_TOO_LONG
 *       LUB_NAME_INVALID
 *       LUB_OPT_TOO_LONG
 *       LUB_OPT_INVALID
 *       LUB_OPT_RESERVED
 *       LUB_NON_LATIN_CHAR
 *       LUB_OVERLAP
 *       LUB_TRUNCATED
 *       LUB_INTERNAL_ERROR
 * @brief Error values for size_t, pointer, and int error-
 * returning functions (for use with LUB_*_ERR macros.
 * 
 * Error values are reserved in the range -99 to -2 for int,
 * (size_t)-99 to (size_t)-2 for size_t, and (void *)-99 to
 * (void *)-2 for pointer types. These values are distinct
 * from valid results.
 * 
  * See also @ref ErrorClassificationAndCast.
 * @{
 */

#if defined(LUB_PTR_INVALID) || \
    defined(LUB_UNTERMINATED) || \
    defined(LUB_BSTR_TOO_LONG) || \
    defined(LUB_NAME_TOO_LONG) || \
    defined(LUB_NAME_INVALID) || \
    defined(LUB_OPT_TOO_LONG) || \
    defined(LUB_OPT_INVALID) || \
    defined(LUB_OPT_RESERVED) || \
    defined(LUB_NON_LATIN_CHAR) || \
    defined(LUB_OVERLAP) || \
    defined(LUB_TRUNCATED) || \
    defined(LUB_INTERNAL_ERROR)
#error "lubtype.h: An error value LUB_* macro is unexpectedly " \
       "already defined. "
       "#undef before including lubtype.h."
#endif // defined macros

// Error values.
#define LUB_PTR_INVALID            (-2)
#define LUB_UNTERMINATED           (-3)
#define LUB_BSTR_TOO_LONG          (-4)
#define LUB_NAME_TOO_LONG          (-5)
#define LUB_NAME_INVALID           (-6)
#define LUB_OPT_TOO_LONG           (-7)
#define LUB_OPT_INVALID            (-8)
#define LUB_OPT_RESERVED           (-9)
#define LUB_NONLATIN_SOURCE        (-10)
#define LUB_NON_LATIN_CHAR         (-11)
#define LUB_OVERLAP                (-12)
#define LUB_TRUNCATED              (-13)
#define LUB_INTERNAL_ERROR         (-99)
// -13 to -98 reserved for future error values.

/** @} */

/**
 * @defgroup ErrorClassificationAndCast Error Classification and Cast
 * @name LUB_PTR_ERR, LUB_SIZE_ERR, LUB_INT_ERR
 * @brief Macros for classifying a value as an error value and casting error
 * values to pointer, size_t, and int types. Only values in the reserved
 * error range (-99 to -2) are considered error values; any non‑error
 * value yields 0, Error values are
 * representable across int, size_t, intptr_t, and void *.
 * See @ref ErrorValues and @ref GuidingPrinciples.
 * @param value The value returned by a function to be classified or cast.
 * @param error 0 (indicating any error) or a specific error value.
 * @return The value (if it is an error value) or 0 (not an error value)
 *         cast to the function type:
 *
 *             LUB_PTR_ERR  -> intptr_t
 *             LUB_SIZE_ERR -> size_t
 *             LUB_INT_ERR  -> int
 * 
 * @example Examples of using error values and macros
 * 
 *   1. Use a  LUB_*_ERR macro to check if a returned value is an error:
 * 
 *      if (LUB_SIZE_ERR(result, 0)) { error handling }
 *      else { handle non-error result }
 * 
 *      if (LUB_PTR_ERR(result, 0)) { error handling }
 *      else { handle non-error result }
 * 
 *      if (LUB_INT_ERR(result, 0)) { error handling }
 *      else { handle non-error result }
 * 
 *   2. Use the LUB_*_ERR macros to check whether a returned value
 *      is a specific error:
 * 
 *      if (LUB_SIZE_ERR(result, LUB_UNTERMINATED))
 *        { handle unterminated error }
 * 
 *      if (LUB_PTR_ERR(result, LUB_PTR_INVALID)) { handle bad pointer error }
 * 
 *   3. Use the LUB_*_ERR macros to cast an error value from a called
 *      function to the return type of the calling function:
 * 
 *      if (LUB_SIZE_ERR(result, 0))
 *        return (uchar_t *)LUB_PTR_ERR(result, 0);
 *
 *   4. For input pointer arguments (e.g., s), encountering an error value
 *      for the pointer indicates an invalid pointer:
 *
 *      if (LUB_PTR_ERR(s, 0)) return LUB_SIZE_ERR(LUB_PTR_INVALID, 0);
 * @{
 */

#if defined(LUB_PTR_ERR) || \
    defined(LUB_SIZE_ERR) || \
    defined(LUB_INT_ERR)
#error "lubtype.h: A LUB_*_ERR macro " \
       "is unexpectedly already defined. " \
       "#undef before including lubtype.h."
#endif // defined macros

#define LUB_PTR_ERR(value, error) \
    ((!(error) && \
    (intptr_t)(value) >= (intptr_t)-99 && \
    (intptr_t)(value) <= (intptr_t)-2) || \
     ((intptr_t)(error) >= (intptr_t)-99 && \
    (intptr_t)(error) <= (intptr_t)-2  && \
    (intptr_t)(value) == (intptr_t)(error) \
     ) ? (intptr_t)(value) : (intptr_t)0)

#define LUB_SIZE_ERR(value, error) \
    ((size_t)(intptr_t)LUB_PTR_ERR(value, error))

#define LUB_INT_ERR(value, error) \
    ((int)(intptr_t)LUB_PTR_ERR(value, error))

/** @} */

/**
 * @section LUBAPIFunctions LUB API Functions
 */

/**
 * @subsection NamingConventions Naming Conventions
 * 
 * The following naming conventions are used for LUB API functions.
 * These conventions are part of the documented and stable API and
 * will not change without a major version number increase other than
 * for new kind/operations (<kind>, <op>) where at least a minor version number
 * increase occurs.
 * 
 * {} required. [] optional. <> token. | alternatives.
 * 
 * l = Latin (lchar_t), u = Unicode (uchar_t), b = byte (byte_t)
 *
 * 1. Character Classification
 *
 *    is<s><kind>
 * 
 *    <s> = l for classify Latin character.
 *          u for classify Unicode character.
 * 
 *    <kind> = alpha, digit, alnum, name1c, namec,
 *             upper, lower, cntrl, print, graph, punct,
 *             blank, space, hex
 *
 *    Examples: isualpha, islhex, isunamec
 *
 * 2. Character <- Character Case Transform
 *
 *    <rs>to<case>
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
 *    Note: case indicates case-preserving transform.
 *
 *    Examples: ultoupper, lutolower, lutocase
 *
 * 3. int <- Character Transform
 *
 *    i<s>hex
 *
 *    <s> = l for Latin source character.
 *          u for Unicode source character.
 *
 *    Example: iuhex
 *
 * 4. String Length
 * 
 *    <s>csnlen
 * 
 *    <s> = l for Latin source string.
 *          u for Unicode source string.
 * 
 *    n = bounded source with an sn
 *        parameter for the bound.
 * 
 *    Examples: lcsnlen, ucsnlen
 * 
 * 5. String Classification
 *
 *    is<s>[n]<kind>
 * 
 *    <s> = l for classify Latin source string.
 *          u for classify Unicode source string.
 * 
 *    n = bounded source with an sn parameter for the bound.
 *        Required for kind latinstr, alphastr, and hexstr.
 *        Omitted for kind reserved and qname (bound defaults
 *        to LUB_MAX_QNAMELEN).
 * 
 *    <kind> = latinstr, alphastr, hexstr, reserved, qname 
 *
 *    Examples: isualpha, islhexstr, islreserved, isuqname
 *
 * 6. Option Validation
 *
 *    is<s><kind>
 * 
 *    <s> = l for Latin source string.
 *          u for Unicode source string.
 * 
 *    <kind> = truncstr, trimstr, padstr, needlestr
 *
 *    Examples: isltrimstr, isupadstr
 * 
 * 7. Other Functions
 *
 *    <ts>s[n|nn]<op>
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
 *    n = bounded target with tn parameter for the bound (implicit
 *        bound on source) or bounded source with an sn
 *        parameter for the bound depending on the operation.
 * 
 *    nn = bounded target buffer and bounded source(s) with
 *         tn and sn parameters for the target bound
 *         and source bound(s), respectively, or bounded source
 *         s1 and source s2 with s1n and s2n parameters for the bounds on
 *         s1 and s2, respectively.
 * 
 *    <op> = operation. The currently provided operations include:
 * 
 *      cmp/CMP = compare.
 *      fxdcmp/FXDCMP = fixed-length leading substring compare.
 *      pfxcmp/PFXCMP = prefix compare.
 *      sfxcmp/SFXCMP = suffix compare.
 * 
 *      ptrim = return pointer into string to start of trimmed
 *              substring plus length of trimmed substring.
 * 
 *      skip = skip characters.
 *
 *      strm/STRM = character/substring search for mth occurrence.
 * 
 *      cnt/CNT = count occurrences of characters/substrings.
 *
 *      cat/catc/CATC = concatenate to target.
 *      catq/catqc/CATQC = concatenate to target with quoting.
 *      catname/catnamec/CATNAMEC = concatenate name to target
 *                                       with quoting as needed.
 *
 *      cpy/cpyc/CPYC = copy to target.
 *      cpyq/cpyqc/CPYQC = copy to target with quoting.
 *      cpyname/cpynamec/CPYNAMEC = copy to target with quoting
 *                                     for a name (maximum length of source
 *                                     is implicitly bounded to 128).
 * 
 *      pad/trim/reverse = copy modified string to target.
 * 
 *      replace/REPLACE = character/string replacement.
 *
 *      vprintf/printf = format string.
 * 
 *    Examples: lusnncpy, ulsnnCATC, uusnnCMP, ulsnnSTRM,
 *              uusncatnamec, uusnCATNAMEC, ulsncpyname, ulsncpynamec.
 *
 * @note Operations with Case Handling
 *
 *   lowercase operation (no 'c') = case-sensitive character comparison or
 *                                  no case mapping.
 *   uppercase operation (no 'C') = case-insensitive matching/comparison.
 *   lowercase operation with 'c' = case-mapped to lowercase.
 *   uppercase operation with 'C' = case-mapped to uppercase.
 */

/**
 * @subsection  CommonParameters Common Parameters
 *
 * The following are commonly used parameters for the functions
 * in this API. Specific functions may use a subset of these
 * parameters, have additional parameters, or have additional
 * details or requirements for these parameters (see specific
 * function declarations and definitions for details).
 * 
 * @param t Pointer to target buffer (destination for operations such as
 *          concatenate, copy, trim, replace, and pad).
 * 
 * @param tn Target buffer size (excluding null terminator).
 *           tn is silently clamped by LUB_MAX_LSTRLEN, LUB_MAX_USTRLEN,
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
 *           sn itself is silently clamped by LUB_MAX_LSTRLEN, LUB_MAX_USTRLEN,
 *           or LUB_MAX_BSTRLEN based on the type of the source string
 *           and operation. If sn is omitted in a function declaration,
 *           the source is bounded by LUB_MAX_QNAMELEN.
 *           Note sn is required if the source is a byte string (*byte_t)
 *           and specifies the length of the source.
 * 
 * @param trunc Pointer to a string that specifies how to handle
 *              truncation when the result would otherwise exceed the
 *              size (tn) of the target buffer. The length of trunc
 *              is clamped by LUB_MAX_LOPTLEN or LUB_MAX_UOPTLEN.
 * 
 *              The trunc string has the form:
 * 
 *                [<truncate mode>][<truncated replacement string>]
 * 
 *              where <truncate mode> is an optional Latin alphabetic character.
 * 
 *              - If trunc is NULL, truncate mode defaults to "B" and truncated
 *                replacement string defaults to a 0-length string.
 * 
 *              - If the first character is Latin alphabetic, it specifies
 *                the truncate mode and is followed by the truncated
 *                replacement string.
 * 
 *              - If the first character is not Latin alphabetic, truncate
 *                mode defaults to 'R' and trunc is the truncated replacement
 *                string.
 * 
 *             Truncate Mode (explicit or by default):
 * 
 *             - If 'L' or 'l', truncate on
 *               the left with the truncated replacement string
 *               added on the left.
 * 
 *             - If 'R' or 'r', truncate on the right with the truncated
 *               replacement string added on the right.
 * 
 *             - If 'C' or 'c', truncate in
 *               the center with the truncated replacement string
 *               added in the center.
 * 
 *             - If 'B' or 'b', the result is the truncated replacement
 *               string.
 * 
 *             Truncated Replacement String:
 * 
 *             - May be a 0-length string.
 * 
 *             - If this string is longer than the target buffer size (tn),
 *               a 0-length string is used instead.
 *
 *             If a result is truncated, error LUB_TRUNCATED is
 *             returned by the function.
 * 
 * @param trim Pointer to a string that specifies how to handle
 *             trimming. The length of trim is clamped by LUB_MAX_LOPTLEN
 *             or LUB_MAX_UOPTLEN.
 * 
 *             The trim string has the form:
 * 
 *                [<trim mode>][<trim characters>]
 * 
 *             - Length of trim is bounded by LUB_MAX_LOPTLEN or
 *               LUB_MAX_UOPTLEN.
 * 
 *             - If NULL, trim defaults to "B".
 * 
 *             - If the first character is Latin alphabetic, it specifies
 *               the trim mode and the rest of the characters are
 *               the trim characters.
 * 
 *             - If the first character is not Latin alphabetic, trim mode defaults
 *               to 'B' and the characters in trim are the trim characters.
 * 
 *             Trim Mode (explicit or by default):
 * 
 *             - If 'L' or 'l', trim on the left.
 * 
 *             - If 'R' or 'r', trim on the right.
 * 
 *             - If 'B' or 'b', trim on both left and right.
 * 
 *             - Other Latin alphabetic characters are reserved for future use
 *               and an error occurs if used.
 *
 *             Trim Characters (explicit or by default):
 * 
 *             - Characters to trim from the beginning
 *               and/or end of the source string).
 * 
 *             - Matching is case-sensitive (include both cases
 *               as trim characters if desired).
 * 
 *             - If no trim characters, defaults to trimming
 *               whitespace characters as determined by islspace (when
 *               trim has lchar_t characters) or isuspace (when trim
 *               has uchar_t characters).
 * 
 * @param q Quote character (' or ") to enclose result. Each q
 *          character in the source is doubled in the target buffer).
 * 
 * @param delim Defines a delimiter character to delimit substrings in a 
 *              map string for replacement, s2 string for search functions,
 *              or in a trimmed result.
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
 * @param m mth occurrence for matching in search and replacement functions.
 * 
 *          m > 0 counts from the beginning of the string
 *              (1 means first occurrence).
 * 
 *          m == 0 returns NULL for search functions.
 * 
 *          m == 0 means replace all occurrences for replacement functions.
 * 
 *          m < 0 counts from the end of the string (-1 means last
 *                occurrence) when the operation supports reverse search.
 */

#if defined(LUB_DEFINITIONS)

/**
 * @subsection FunctionFamilies Function Families
 * 
 */

#if defined(LUB_OP_DEF)

 // Ensure LUB_OP_DEF is not already defined to avoid
 // silently redefining it. This macro is defined and
 // redefined as needed in lubtype.h with a helper
 // macro definition specific to a function family.
 // This macro is undefined prior to the end of header file.
 // This helper macro is typically used when the macro definition
 // is not complex and efficiency is important (e.g., for
 // character classification functions. For more complex definitions,
 // a helper lub_*_ext function is typically used instead.
#error "lubtype.h: LUB_OP_DEF macro is unexpectedly " \
       "already defined. " \
       "#undef before defining LUB_DEFINITIONS and " \
       "including lubtype.h."
#endif // LUB_OP_DEF

#endif // LUB_DEFINITIONS

/**
 * @defgroup CharacterClassification Character Classification
 * @name isualpha, islalpha
 *       isudigit, isldigit
 *       isualnum, islalnum
 *       isllatin, isulatin
 *       islname1c, isuname1c, islnamec, isunamec
 *       isuupper, islupper, isulower, isllower
 *       isucntrl, islcntrl, isuprint, islprint
 *       isugraph, islgraph, isupunct, islpunct
 *       isublank, islblank, isuspace, islspace
 *       isuhex, islhex
 * @brief Latin and Unicode character classification functions.
 * @param c Character to classify. All functions take unsigned int
 *        to avoid sign-extension issues; the appropriate cast is applied
 *        internally based on the function's character set.
 * @return 1 if the character satisfies the condition, otherwise 0.
 * 
 * @note isllatin/isulatin classifies whether c is a Latin character.
 *
 * @note islname1c/isuname1c classifies whether c is valid for
 *       the first character of an unquoted name, i.e., a
 *       Latin alphabetic or '_' character.
 *
 * @note islnamec/isunamec classifies whether c is valid in an
 *       unquoted name other than for the first character, i.e.,
 *       a Latin alphabetic, numeric, or '_' character.
 * 
 * @note islblank classifies whether c is a blank character in the Latin
 *       range (space or tab). isublank classifies whether c is a blank
 *       character in the Unicode range (space, tab, or other locale-specific
 *       blanks per the C standard).
 *
 * @note islspace classifies whether c is a Latin whitespace character,
 *       i.e., ' ', '\t', '\v', '\f', '\n', or '\r' character.
 *
 * @note isuspace classifies whether c is a Unicode whitespace character,
 *       which includes Latin whitespace characters plus additional Unicode
 *       whitespace characters (e.g., U+00A0 NO-BREAK SPACE,
 *       U+2003 EM SPACE, etc.).
 *
 * @note isuhex/islhex classifies whether c is a
 *       hexadecimal digit character, i.e., ('0' to '9',
 *       'A' to 'F', or 'a' to 'f').
 * @{
 */

static inline
int isualpha
( const unsigned int c )
{ return c <= LUB_MAX_UCHAR &&
         iswalpha((wchar_t)c) ? (int)1 : (int)0;
}

static inline
int islalpha
( const unsigned int c )
{ return c <= LUB_MAX_LCHAR && isalpha((int)c) ?
         (int)1 : (int)0;
}

static inline
int isudigit
( const unsigned int c )
{ return c <= LUB_MAX_UCHAR &&
         iswdigit((wchar_t)c) ?
         (int)1 : (int)0;
}

static inline
int isldigit
( const unsigned int c )
{ return c <= LUB_MAX_LCHAR && isdigit((int)c) ?
         (int)1 : (int)0;
}

static inline
int isualnum
( const unsigned int c )
{ return c <= LUB_MAX_UCHAR &&
         iswalnum((wchar_t)c) ?
        (int)1 : (int)0;
}

static inline
int islalnum
( const unsigned int c )
{ return c <= LUB_MAX_LCHAR && isalnum((int)c) ?
         (int)1 : (int)0;
}

static inline
int isllatin
( const unsigned int c )
{ return c <= LUB_MAX_LCHAR ?
         (int)1 : (int)0; }

static inline
int isulatin
( const unsigned int c )
{ return c <= LUB_MAX_LCHAR ?
         (int)1 : (int)0; }

static inline
int islname1c
( const unsigned int c )
{ return c <= LUB_MAX_LCHAR &&
         (islalpha((lchar_t)c) || (lchar_t)c == '_') ?
         (int)1 : (int)0;
}

static inline
int isuname1c
( const unsigned int c )
{ return islname1c(c); }

static inline
int islnamec
( const unsigned int c )
{ return c <= LUB_MAX_LCHAR &&
         (islalnum((lchar_t)c) || (lchar_t)c == '_') ?
         (int)1 : (int)0;
}
         
static inline
int isunamec
( const unsigned int c )
{ return islnamec(c); }

static inline
int isuupper
( const unsigned int c )
{ return c <= LUB_MAX_UCHAR &&
         iswupper((wchar_t)c) ?
         (int)1 : (int)0;
}

static inline
int islupper
( const unsigned int c )
{ return c <= LUB_MAX_LCHAR && isupper((int)c) ?
         (int)1 : (int)0;
}

static inline
int isulower
( const unsigned int c )
{ return c <= LUB_MAX_UCHAR &&
         iswlower((wchar_t)c) ?
         (int)1 : (int)0;
}

static inline
int isllower
( const unsigned int c )
{ return c <= LUB_MAX_LCHAR && islower((int)c) ?
         (int)1 : (int)0;
}

static inline
int isucntrl
( const unsigned int c )
{ return c <= LUB_MAX_UCHAR &&
         iswcntrl((wchar_t)c) ?
         (int)1 : (int)0;
}

static inline
int islcntrl
( const unsigned int c )
{ return c <= LUB_MAX_LCHAR && iscntrl((int)c) ?
         (int)1 : (int)0;
}

static inline
int isuprint
( const unsigned int c )
{ return c <= LUB_MAX_UCHAR &&
         iswprint((wchar_t)c) ?
         (int)1 : (int)0;
}

static inline
int islprint
( const unsigned int c )
{ return c <= LUB_MAX_LCHAR && isprint((int)c) ?
         (int)1 : (int)0;
}
         
static inline
int isugraph
( const unsigned int c )
{ return c <= LUB_MAX_UCHAR &&
         iswgraph((wchar_t)c) ?
         (int)1 : (int)0;
}

static inline
int islgraph
( const unsigned int c )
{ return c <= LUB_MAX_LCHAR && isgraph((int)c) ?
         (int)1 : (int)0;
}

static inline
int isupunct
( const unsigned int c )
{ return c <= LUB_MAX_UCHAR &&
         iswpunct((wchar_t)c) ?
         (int)1 : (int)0;
}
         
static inline
int islpunct
( const unsigned int c )
{ return c <= LUB_MAX_LCHAR && ispunct((int)c) ?
         (int)1 : (int)0;
}
            
static inline
int isublank
( const unsigned int c )
{ return c <= LUB_MAX_UCHAR &&
         iswblank((wchar_t)c) ?
         (int)1 : (int)0;
}
            
static inline
int islblank
( const unsigned int c )
{ return c <= LUB_MAX_LCHAR && isblank((int)c) ?
         (int)1 : (int)0;
}

static inline
int isuspace
( const unsigned int c )
{ return c <= LUB_MAX_UCHAR &&
         iswspace((wchar_t)c) ?
         (int)1 : (int)0;
}

static inline
int islspace
( const unsigned int c )
{ return c <= LUB_MAX_LCHAR && isspace((int)c) ?
         (int)1 : (int)0;
}

static inline
int islhex
( const unsigned int c )
{ return ((c >= '0' && c <= '9') ||
          (c >= 'a' && c <= 'f') ||
          (c >= 'A' && c <= 'F')) ?
         (int)1 : (int)0;
}

static inline
int isuhex
( const unsigned int c )
{ return c <= LUB_MAX_LCHAR ?
         islhex((uchar_t)c) : (int)0;
}

/** @} */

/**
 * @defgroup CaseConversion Case Conversion
 * @name lltocase, lutocase, ultocase, uutocase
 *       lltoupper, lutoupper, ultoupper, uutoupper
 *       lltolower, lutolower, ultolower, uutolower
 * @brief Character preserve/uppercase/lowercase conversion.
 * @param c Character to convert.
 * @param lrep For lu* functions only, Latin character
 *             to return if c is out-of-range for Latin.
 *             lrep may be the NULL ('\0') character.
 * @return Preserve/upper/lower case version of c if one defined,
 *         otherwise c.
 *
 * @note For lutocase/lutoupper/lutolower, if converted result
 *       is within the valid Latin range, return that result.
 *       If the converted result is outside the Latin range
 *       but c is within the Latin range, return c.
 *       Otherwise, return lrep.
 * @{
 */

// To case-preserve conversion.

static inline
lchar_t lltocase
( const lchar_t c )
{ return c; }

static inline
lchar_t lutocase
( const uchar_t c, lchar_t lrep )
{ return (size_t)c > LUB_MAX_LCHAR ? lrep : (lchar_t)c; }

static inline uchar_t ultocase
( const lchar_t c )
{ return (uchar_t)c; }

static inline uchar_t uutocase
( const uchar_t c )
{ return c; }

// To uppercase conversion.

static inline
lchar_t lltoupper
( const lchar_t c )
{ return (lchar_t)toupper((int)c); }

static inline
lchar_t lutoupper
( const uchar_t c, lchar_t lrep )
{ wint_t wc = towupper((wint_t)c);
  return ((unsigned)wc > LUB_MAX_LCHAR) ?
         (c > LUB_MAX_LCHAR ? lrep : (lchar_t)c) : (lchar_t)wc;
}
static inline
uchar_t ultoupper
( const lchar_t c )
{ return (uchar_t)toupper((int)c); }

static inline uchar_t uutoupper
( const uchar_t c )
{ wint_t wc = towupper((wint_t)c);
  return ((unsigned)wc > LUB_MAX_UCHAR) ?
         (uchar_t)c : (uchar_t)wc;
}

// To lowercase conversion.

static inline lchar_t lltolower
( const lchar_t c )
{ return (lchar_t)tolower((int)c); }

static inline lchar_t lutolower
( const uchar_t c, lchar_t lrep )
{ wint_t wc = towlower((wint_t)c);
  return ((unsigned)wc > LUB_MAX_LCHAR) ?
         (c > LUB_MAX_LCHAR ? lrep : (lchar_t)c) : (lchar_t)wc;
}
static inline uchar_t ultolower
( const lchar_t c )
{ return (uchar_t)tolower((int)c); }

static inline uchar_t uutolower
( const uchar_t c )
{ wint_t wc = towlower((wint_t)c);
  return ((unsigned)wc > LUB_MAX_UCHAR) ?
            (uchar_t)c : (uchar_t)wc;
}

/** @} */

/**
 * @defgroup HexDigitToIntConversion Hex Digit to Int Conversion
 * @name ilhex and iuhex
 * @brief Hex digit character to integer conversion.
 * @param  c Character to convert.
 * @return Value (0-15) of the hex digit if c is a valid hex
 *         digit ('0'-'9', 'a'-'f', 'A'-'F'); otherwise, -1.
 * @{
 */

static inline
int iuhex
( const uchar_t c )
{ if (c >= '0' && c <= '9') return c - '0';
  else if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  else if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  return (int)-1;
}

static inline
int ilhex
( const lchar_t c )
{ return iuhex((uchar_t)c); }
/** @} */

/**
 * @defgroup StringLength String Length
 * @name lcsnlen, ucsnlen
 * @brief Return the length of a source string.
 * @param s Pointer to the source string.
 * @param sn The maximum length of the string. Clamped to
 *           LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * @return String length, 0 if s is NULL, or error value.
 *
 * @note Errors:
 * - (size_t)LUB_PTR_INVALID if s is an invalid pointer.
 * - (size_t)LUB_UNTERMINATED if s is not null-terminated.
 * @{
 */

extern
size_t LUB_len_ext
( const char xs, const lchar_t *s,  size_t sn )
#if defined(LUB_DEFINITIONS)
{ if (!s) return (size_t)0;
  if (LUB_PTR_ERR(s, 0)) return LUB_SIZE_ERR(LUB_PTR_INVALID, 0);
  const size_t maxlen = (xs == 'l') ?
                        LUB_MAX_LSTRLEN : LUB_MAX_USTRLEN;
  if (sn > maxlen) sn = maxlen;
  size_t k = 0;
  if (xs == 'u')
  { const uchar_t *ss = (uchar_t *)s;
    for (; k < sn && *ss; ++k, ++ss);
    if (*ss) k = LUB_SIZE_ERR(LUB_UNTERMINATED, 0);
  }
  else
  { for (; k < sn && *s; ++k, ++s);
    if (*s) k = LUB_SIZE_ERR(LUB_UNTERMINATED, 0);
  }
  return k;
}
#else
;
#endif // LUB_DEFINITIONS for LUB__len_helper.

static inline
size_t lcsnlen
( const lchar_t *s, size_t sn )
{ return LUB_len_ext('l', s, sn); }

static inline
size_t ucsnlen
( const uchar_t *s, size_t sn )
{ return LUB_len_ext('u', (const lchar_t *)s, sn); }

/**
 * @defgroup StringClassification String Classification
 * @name islnlatinstr, isunlatinstr
 *       islnalphastr, isunalphastr
 *       islnhexstr, isunhexstr
 *       islreserved, isureserved, islqname, isuqname
 * @brief Latin and Unicode string classification.
 * @param s Pointer to source string.
 * @param sn Maximum source string length. Clamped to
 *           LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * 
 *           For islreserved, isureserved, islqname and
 *           isuqname, sn is omitted and sn defaults
 *           to LUB_MAX_NAMELEN.
 * @return 1 Condition satisfied and no error.
 *         0 Condition unsatisfied and no error.
 *         Otherwise, an error value.
 *
 * @note Errors:
 * - (int)LUB_PTR_INVALID if s is an invalid pointer.
 * - (int)LUB_UNTERMINATED if s is not null-terminated.
 * - (int)LUB_NAME_TOO_LONG if s is too long for a name.
 * - (int)LUB_NAME_INVALID if s is not a valid name.
 *
 * @note islnlatinstr, isunlatinstr:
 * 
 *       Classify whether s contains only Latin characters.
 *       Note: islnlatinstr always returns 1 if an error
 *       doesn't occur.
 *
 * @note islnalphastr, isunalphastr:
 * 
 *      Classify whether s contains only alphabetic characters.
 * 
 * @note islnhexstr, isunhexstr:
 * 
 *       Classify whether s consists only of hex digit characters
 *       ('0' to '9', 'A' to 'F', or 'a' to 'f'), s is NULL,
 *       or s is empty.
 *
 * @note islreserved, isureserved:
 * 
 *       Classify whether s is a Teradata reserved word (case-insensitive).
 *
 * @note islqname, isuqname:
 * 
 *       Classify whether s must be a quoted name.
 *       s must be quoted if the first character is not a first-name
 *       character (see islname1c and isuname1c), or any subsequent character is not
 *       a name character (see islnamec and isunamec).
 * @{
 */

// Unicode string with only Latin characters.

#if defined(LUB_DEFINITIONS)

// String classification helper macro.

#undef LUB_OP_DEF
#define LUB_OP_DEF(lkind, ukind) \
{ if (!s) return (int)1; \
  if (LUB_PTR_ERR(s, 0)) return LUB_INT_ERR(LUB_PTR_INVALID, 0); \
  const size_t maxlen = (xs == 'u') ? LUB_MAX_USTRLEN : LUB_MAX_LSTRLEN; \
  if (sn > maxlen) sn = maxlen; \
  if (xs == 'u') \
  { const uchar_t *us = (const uchar_t *)s; \
    for (; sn && *us; --sn, ++us) \
      if (!ukind) \
      { /* Verify null terminator. */ \
        for (; sn && *us; --sn, ++us); \
        if (!*us) return (int)0; \
      } \
    if (*us) return LUB_INT_ERR(LUB_UNTERMINATED, 0); \
  } \
  else \
  { for (; sn && *s; --sn, ++s) \
      if (!lkind) \
      { /* Verify null terminator. */ \
        for (; sn && *s; --sn, ++s); \
        if (!*s) return (int)0; \
      } \
    if (*s) return LUB_INT_ERR(LUB_UNTERMINATED, 0); \
  } \
  return (int)1; \
}

#endif // LUB_DEFINITIONS for string classification helper macro.

extern
int LUB_string_latin_ext
( const char xs, const lchar_t *s, size_t sn )
#if defined(LUB_DEFINITIONS)
LUB_OP_DEF(1, (*us <= (uchar_t)LUB_MAX_LCHAR))
#else
;
#endif // LUB_DEFINITIONS for LUB_string_latin_ext.

static inline
int islnlatinstr
( const lchar_t *s, size_t sn )
{ return LUB_string_latin_ext('l', s, sn); }

static inline
int isunlatinstr
( const uchar_t *s, size_t sn )
{ return LUB_string_latin_ext('u', (const lchar_t *)s, sn); }

// String with only alphabetic characters.

extern
int LUB_string_alpha_ext
( const char xs,  const lchar_t *s, size_t sn
)
#if defined(LUB_DEFINITIONS)
LUB_OP_DEF((islalpha(*s)), (isualpha(*us)))
#else
;
#endif // LUB_DEFINITIONS for LUB_string_alpha_ext.

static inline
 int islnalphastr
 ( const lchar_t *s, size_t sn )
{ return LUB_string_alpha_ext('l', s, sn); }

static inline
int isunalphastr
( const uchar_t *s, size_t sn )
{ return LUB_string_alpha_ext('u', (const lchar_t *)s, sn); }

// Hex digit string classification.

extern
int LUB_string_hex_ext
( const char xs, const lchar_t *s,  size_t sn )
#if defined(LUB_DEFINITIONS)
LUB_OP_DEF((islhex(*s)), (isuhex(*us)))
#else
;
#endif // LUB_DEFINITIONS for LUB_string_hex_ext.

static inline
int islnhexstr
( const lchar_t *s, size_t sn )
{ return LUB_string_hex_ext('l', s, sn); }

static inline 
int isunhexstr
( const uchar_t *s, size_t sn )
{ return LUB_string_hex_ext('u', (const lchar_t *)s, sn); }

extern
int lub_is_reserved_ext
( const char xs, const lchar_t *s )
#if defined(LUB_DEFINITIONS)
{ // Teradata reserved words list.
  static const char *const reserved[] =
  { "ABORT", "ABORTSESSION", "ABS", "ACCESS_LOCK", "ACCOUNT", "ACOS",
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

  const size_t MIN_RESERVEDLEN = 2;  /* Update if min changes. */
  const size_t MAX_RESERVEDLEN = 20; /* Update if max changes. */
  size_t sn = xs == 'u' ?
              ucsnlen((const uchar_t *)s, LUB_MAX_NAMELEN) :
              lcsnlen(s, LUB_MAX_NAMELEN);
  if (LUB_SIZE_ERR(sn, LUB_UNTERMINATED))
    return LUB_INT_ERR(LUB_NAME_TOO_LONG, 0);
  if (LUB_SIZE_ERR(sn, 0))
    return LUB_INT_ERR(sn, 0);
  if (!sn) // Null or empty string is not a valid name.
    return LUB_INT_ERR(LUB_NAME_INVALID, 0);
  // All spaces is not a a valid name.
  if (xs == 'u')
  { const uchar_t *us = (const uchar_t *)s;   
    for (; *us; ++us) if (*us != (uchar_t)' ') break;
    if (!*us) return LUB_INT_ERR(LUB_NAME_INVALID, 0);
  }
  else
  { const lchar_t *ls = s;
    for (; *ls; ++ls) if (*ls != (lchar_t)' ') break;
    if (!*ls) return LUB_INT_ERR(LUB_NAME_INVALID, 0);
  }
  // Check if name is reserved.
  if (sn < MIN_RESERVEDLEN || sn > MAX_RESERVEDLEN) return (int)0;
  size_t lo = 0;
  size_t hi = sizeof(reserved) / sizeof(reserved[0]);
  while (lo < hi)
  { const size_t mid = lo + ((hi - lo) / 2);
    // Compare s to reserved word r. Comparison is case-insensitive.                       
    const char *r = reserved[mid];
    int cmp;
    if (xs == 'u')
    { const uchar_t *us = (const uchar_t *)s;
      for (;; ++r, ++us)
      { if (!*r && !*us) { cmp = 0; break; }
        if (*us > LUB_MAX_LCHAR) { cmp = 0; break; }
        const uchar_t c = uutoupper(*us);
        if (c < (uchar_t)*r) { cmp = -1; break; }
        if (c > (uchar_t)*r) { cmp = 1; break; }
      }
    }
    else // xs == 'l' 
    { const lchar_t *ls = s;
      for (; ; ++r, ++ls)
      { if (!*r && !*ls) { cmp = 0; break; }
        if (*ls > LUB_MAX_LCHAR) { cmp = 0; break; }
        const lchar_t c = lltoupper(*ls);
        if (c < (lchar_t)*r) { cmp = -1; break; }
        if (c > (lchar_t)*r) { cmp = 1; break; }
      }
    }
    if (cmp == 0) return (int)1;
    if (cmp < 0) hi = mid; else lo = mid + 1;
  }
  return (int)0;
}
#else
;
#endif // LUB_DEFINITIONS for lub_is_reserved_ext.

static inline
int islreserved
( const lchar_t *s )
{ return lub_is_reserved_ext('l', s);}

static inline
int isureserved
( const uchar_t *s )
{ return lub_is_reserved_ext('u', (const lchar_t *)s);}

extern
int islqname
( const lchar_t *s )
#if defined(LUB_DEFINITIONS)
{ int reserved = islreserved(s);
  if (LUB_INT_ERR(reserved, 0)) return reserved;
  if (reserved) return (int)1;
  if (!islname1c(*s)) return (int)1;
  for (; *s && islnamec(*s); s++);
  return *s ? (int)0 : (int)1;
}
#else
;
#endif // LUB_DEFINITIONS

extern
int isuqname
( const uchar_t *s )
#if defined(LUB_DEFINITIONS)
{   int reserved = isureserved(s);
    if (LUB_INT_ERR(reserved, 0)) return reserved;
    if (reserved) return (int)1;
    if (!isuname1c(*s)) return (int)1;
    for (; *s && isunamec(*s); s++);
    return *s ? (int)0 : (int)1;
}
#else
;
#endif // LUB_DEFINITIONS


/** @} */

/**
 * @defgroup OptionValidation Option Validation
 * @name isltruncstr, isutruncstr
 *       isltrimstr, isutrimstr
 *       islpadstr, isupadstr
 *       islneedlestr, isuneedlestr
 * @brief Option (trunc, trim, pad, needle) string validation.
 * @param s Pointer to source string. Source string length is
 *          bounded to LUB_MAX_LOPTLEN or LUB_MAX_UOPTLEN.
 * @return 1 Valid option string or error.
 *
 * @note Errors:
 * - (int)LUB_PTR_INVALID if s is an invalid pointer.
 * - (int)LUB_UNTERMINATED if s is not null-terminated.
 * - (int)LUB_OPT_INVALID if s is not a valid option string.
 * - (int)LUB_OPT_TOO_LONG if s is too long.
 *
 * @note isltruncstr/isutruncstr:
 * 
 *       Verify whether s is a valid truncated string
 *       for use as a value for a trunc parameter.
 *       A valid string is NULL, null-terminated by the
 *       bound LUB_MAX_LOPTLEN, an empty string, or
 *       a string where the first character is not
 *       a reserved Latin alphabetic character. See trunc
 *       parameter in @ref CommonParameters for details.
 * 
 * @note isltrimstr/isutrimstr:
 * 
 *       Verify whether s is a valid trim string
 *       for use as a value for a trim parameter.
 *       A valid string is NULL, null-terminated by the
 *       bound LUB_MAX_LOPTLEN, an empty string, or a
 *       string where the first character is not a
 *       reserved Latin alphabetic character. See trim
 *       parameter in @ref CommonParameters for details.
 * 
 * @note islpadstr/isupadstr:
 * 
 *       Verify whether s is a valid pad string
 *       for use as a value for a pad parameter.
 *       A valid string is NULL, null-terminated by the
 *       bound LUB_MAX_LOPTLEN, an empty string, or a
 *       string where the first character is not a
 *       reserved Latin alphabetic character,
 *       and length <= 2 if the first character is a
 *       Latin alphabetic character or length <= 1. See pad
 *       parameter in @ref CommonParameters for details.
 *
 * @note islneedlestr/isuneedlestr:
 * 
 *       Verify whether s is a valid needle string for use
 *       as a value for a search parameter. A valid
 *       string is NULL, null-terminated by the bound
 *       LUB_MAX_LOPTLEN, or an empty string (see needle
 *       parameter for details).
 * @{
 */

// Trunc, Trim, Pad, and Needle Option Validation helper function.

extern
int lub_is_option_ext
( const char op, const char xs, const lchar_t *s, const char *mode )
// op: 1=trunc, 2=trim, 3=pad, 4=needle; xs: 'l' or 'u'.
#if defined(LUB_DEFINITIONS)
{ if (LUB_PTR_ERR(s, 0)) return LUB_INT_ERR(LUB_PTR_INVALID, 0);
  const uchar_t *us = (const uchar_t *)s;
  const size_t sn = xs == 'u' ?
                    ucsnlen(us, LUB_MAX_UOPTLEN) :
                    lcsnlen(s, LUB_MAX_LOPTLEN);
  if (LUB_SIZE_ERR(sn, 0)) return LUB_INT_ERR(sn, 0);
  if (!sn || op == 4 /*needlestr*/) return (int)1;
  int c = xs == 'u' ? (int)(*us) : (int)*s;
  if (islalpha(c))
  { if (!strchr(mode, toupper(c)))
      return LUB_INT_ERR(LUB_OPT_RESERVED, 0);
    if (xs == 'u') ++us; else ++s;
  }
  if (op < 3 /*not pad*/) return (int)1;
  /* pad: at most one character after optional mode letter. */ 
  if (xs == 'u')
  { if (!*us) return (int)1;
    if (!*(us+1)) return (int)1;
  }
  else
  { if (!*s) return (int)1;
    if (!*(s+1)) return (int)1;
  }
  return LUB_INT_ERR(LUB_OPT_TOO_LONG, 0);
}
#else
;
#endif // LUB_DEFINITIONS for option validation helper.

static inline
int isltruncstr
( const lchar_t *s )
{ return lub_is_option_ext(1, 'l', s, "LRCB"); }

static inline
int isutruncstr
( const uchar_t *s )
{ return lub_is_option_ext(1, 'u', (lchar_t *)s, "LRCB"); }

static inline
int isltrimstr
( const lchar_t *s )
{ return lub_is_option_ext(2, 'l', s, "LRB"); }

static inline 
int isutrimstr
( const uchar_t *s )
{ return lub_is_option_ext(2, 'u', (lchar_t *)s, "LRB"); }

static inline
int islpadstr
( const lchar_t *s)
{ return lub_is_option_ext(3, 'l', s, "LRB"); }

static inline
int isupadstr
( const uchar_t *s  )
{ return lub_is_option_ext(3, 'u', (lchar_t *)s, "LRB"); }

static inline
int islneedlestr
( const lchar_t *s )
{ return lub_is_option_ext(4, 'l', s, ""); }

static inline
int isuneedlestr
( const uchar_t *s )
{ return lub_is_option_ext(4, 'u', (lchar_t *)s, ""); }

/** @} */

// Helper function for full ('c'), fixed-length ('f'),
// prefix ('p'), and suffix ('s') comparisons.

extern
int lub_cfps_cmp_ext
// op: 'c' (full compare), 'f' (fixed-length compare),
//     'p' (prefix compare), or 's' (suffix compare).
// xs1, xs2: 'l' (Latin) or 'u' (Unicode).
// Case: 's' (case-sensitive) or 'i' (case-insensitive).
( const char op, //
  const char xs1, const lchar_t *s1, size_t s1n,
  const char xs2, const lchar_t *s2, size_t s2n,
  const char Case
)
#if defined(LUB_DEFINITIONS)
{ // Assert valid op, xs1, xs2, and Case values.
  if (op != 'c' && op != 'f' && op != 'p' && op != 's' ||
      xs1 != 'l' && xs1 != 'u' ||
      xs2 != 'l' && xs2 != 'u' ||
      Case != 's' && Case != 'i')
    return LUB_INT_ERR(LUB_INTERNAL_ERROR, 0);

  const uchar_t *us1 = (const uchar_t *)s1;
  const uchar_t *us2 = (const uchar_t *)s2;

  if (op != 'f')
  { s1n = xs1 == 'u' ?
          ucsnlen(us1, s1n > LUB_MAX_USTRLEN ? LUB_MAX_USTRLEN : s1n) :
          lcsnlen(s1, s1n > LUB_MAX_LSTRLEN ? LUB_MAX_LSTRLEN : s1n);
    if (LUB_SIZE_ERR(s1n, 0)) return LUB_INT_ERR(s1n, 0);

    s2n = xs2 == 'u' ?
          ucsnlen(us2, s2n > LUB_MAX_USTRLEN ? LUB_MAX_USTRLEN : s2n) :
          lcsnlen(s2, s2n > LUB_MAX_LSTRLEN ? LUB_MAX_LSTRLEN : s2n);
    if (LUB_SIZE_ERR(s2n, 0)) return LUB_INT_ERR(s2n, 0);
  }

  if (!s1n && !s2n) return LUB_CMP_EQUAL;
  if (!s1n) return !s2n ? LUB_CMP_EQUAL : LUB_CMP_LESS_THAN;
  if (!s2n) return !s1n ? LUB_CMP_EQUAL : LUB_CMP_GREATER_THAN;
          
  int c1 = 0; int c2 = 0;

  if (op == 's')
  { // Suffix comparison: move to end of strings and compare backward.
    if (xs1 == 'u') us1 += s1n - 1; else s1 += s1n - 1;
    if (xs2 == 'u') us2 += s2n - 1; else s2 += s2n - 1;
    
    // Backward loop for all encoding/case combinations.

    if (xs1 == 'l')
      if (xs2 == 'l')
        if (Case == 's') // ll case-sensitive
          for (; s1n && s2n; --s1n, --s2n, --s1, --s2)
          { c1 = (int)*s1; c2 = (int)*s2;
            if (c1 != c2) break;
           }
        else // ll case-insensitive
          for (; s1n && s2n; --s1n, --s2n, --s1, --s2)
          { c1 = toupper((int)*s1); c2 = toupper((int)*s2);
            if (c1 != c2) break;
          }
      else if (Case == 's') // lu case-sensitive
        for (; s1n && s2n; --s1n, --s2n, --s1, --us2)
        { c1 = (int)*s1; c2 = (int)*us2;
          if (c1 != c2) break;
        }
      else // lu case-insensitive
        for (; s1n && s2n; --s1n, --s2n, --s1, --us2)
        { c1 = toupper((int)*s1); c2 = uutoupper((int)*us2);
          if (c1 != c2) break;
        }
    else if (xs2 == 'l')
      if (Case == 's') // ul case-sensitive
        for (; s1n && s2n; --s1n, --s2n, --us1, --s2)
        { c1 = (int)*us1; c2 = (int)*s2;
          if (c1 != c2) break;
        }
      else // ul case-insensitive
        for (; s1n && s2n; --s1n, --s2n, --us1, --s2)
        { c1 = uutoupper((int)*us1); c2 = toupper((int)*s2);
          if (c1 != c2) break;
        }
    else if (Case == 's') // uu case-sensitive
      for (; s1n && s2n; --s1n, --s2n, --us1, --us2)
      { c1 = (int)*us1; c2 = (int)*us2;
        if (c1 != c2) break;
      }
    else // uu case-insensitive
      for (; s1n && s2n; --s1n, --s2n, --us1, --us2)
      { c1 = uutoupper((int)*us1); c2 = uutoupper((int)*us2);
        if (c1 != c2) break;
      }

    // Suffix: s2 is suffix of s1 if all s2 consumed and matched backward.
    if (s2n == 0) return LUB_CMP_EQUAL; // s2 fully matched backward (is a suffix),
    if (c1 != c2) return c1 < c2 ? LUB_CMP_LESS_THAN : LUB_CMP_GREATER_THAN;
    return LUB_CMP_LESS_THAN; // s1 shorter than s2.
  } // End of suffix case.

  // op is 'c', 'f', or 'p'. Forward loop for all encoding/case combinations.
  if (xs1 == 'l')
    if (xs2 == 'l')
      if (Case == 's') // ll case-sensitive
        for (; s1n && s2n && *s1 && *s2; --s1n, --s2n, ++s1, ++s2)
        { c1 = (int)*s1; c2 = (int)*s2;
          if (c1 != c2) break;
         }
      else // ll case-insensitive
        for (; s1n && s2n && *s1 && *s2; --s1n, --s2n, ++s1, ++s2)
          { c1 = toupper((int)*s1); c2 = toupper((int)*s2);
            if (c1 != c2) break;
          }
    else if (Case == 's') // lu case-sensitive
      for (; s1n && s2n && *s1 && *us2; --s1n, --s2n, ++s1, ++us2)
      { c1 = (int)*s1; c2 = (int)*us2;
        if (c1 != c2) break;
      } 
    else // lu case-insensitive
      for (; s1n && s2n && *s1 && *us2; --s1n, --s2n, ++s1, ++us2)
      { c1 = toupper((int)*s1); c2 = uutoupper((int)*us2);
        if (c1 != c2) break;
      }
  else if (xs2 == 'l')
    if (Case == 's') // ul case-sensitive
       for (; s1n && s2n && *us1 && *s2; --s1n, --s2n, ++us1, ++s2)
       { c1 = (int)*us1; c2 = (int)*s2;
         if (c1 != c2) break;
       }
    else // ul case-insensitive
      for (; s1n && s2n && *us1 && *s2;  --s1n, --s2n, ++us1, ++s2)
      { c1 = uutoupper((int)*us1); c2 = toupper((int)*s2);
        if (c1 != c2) break;
      }
  else if (Case == 's') // uu case-sensitive
    for (; s1n && s2n && *us1 && *us2; --s1n, --s2n, ++us1, ++us2)
    { c1 = (int)*us1; c2 = (int)*us2;
      if (c1 != c2) break;
    }
  else // uu case-insensitive
    for (; s1n && s2n && *us1 && *us2; --s1n, --s2n, ++us1, ++us2)
    { c1 = uutoupper((int)*us1); c2 = uutoupper((int)*us2);
      if (c1 != c2) break;
    }

  // Finalize comparison result based on op 'c', 'f', or 'p'.

  if (op == 'c')
    // Full comparison: if mismatch found, use c1 vs c2; else equal.
    return c1 != c2 ? (c1 < c2 ? LUB_CMP_LESS_THAN : LUB_CMP_GREATER_THAN)
                    : LUB_CMP_EQUAL;

  if (op == 'f')
    // Fixed-length: same logic as 'c', using provided lengths.
    return c1 != c2 ? (c1 < c2 ? LUB_CMP_LESS_THAN : LUB_CMP_GREATER_THAN)
                    : LUB_CMP_EQUAL;

  // op == 'p'
  // Prefix: s2 is prefix of s1 if all s2 consumed and matched.
  if (s2n == 0) return LUB_CMP_EQUAL; // s2 fully matched (is a prefix)
  if (c1 != c2) return c1 < c2 ? LUB_CMP_LESS_THAN : LUB_CMP_GREATER_THAN;
  return LUB_CMP_LESS_THAN; // s1 shorter than s2 (mismatch at null terminator of s1).
}
#else
;
#endif // LUB_DEFINITIONS for lub_cfps_cmp_ext.

/**
 * @defgroup Compare Compare
 * @name llsnncmp, lusnncmp, ulsnncmp, uusnncmp (case-sensitive)
 *       llsnnCMP, lusnnCMP, ulsnnCMP, uusnnCMP (case-insensitive)
 *       bbsnncmp
 * @brief Comparison of two strings (like strcmp) .
 * @param s1 Pointer to first source string.
 * @param s1n Bound on string s1. Clamped to LUB_MAX_LSTRLEN
 *            or LUB_MAX_USTRLEN.
 * @param s2 Pointer to second source string.
 * @param s2n Bound on string s2. Clamped to LUB_MAX_LSTRLEN
 *            or LUB_MAX_USTRLEN.
 * @return -1 (s1 < s2), 0 (equal), 1 (s1 > s2), or error.
 *
 * @note Errors:
 * - (int)LUB_PTR_INVALID if s1 or s2 is an invalid pointer.
 * - (int)LUB_UNTERMINATED if s1 or s2 is not null-terminated.
 * @{
 */

// Compare case-sensitive.

static inline
int llsnncmp
( const lchar_t *s1, size_t s1n, const lchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('c', 'l', s1, s1n, 'l', s2, s2n, 's'); }

static inline
int lusnncmp
( const lchar_t *s1, size_t s1n, const uchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('c', 'l', s1, s1n, 'u', (const lchar_t*)s2, s2n, 's'); }

static inline
int ulsnncmp
( const uchar_t *s1, size_t s1n, const lchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('c', 'u', (const lchar_t*)s1, s1n, 'l', s2, s2n, 's'); }

static inline
int uusnncmp
( const uchar_t *s1, size_t s1n, const uchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('c', 'u', (const lchar_t*)s1, s1n, 'u', (const lchar_t*)s2, s2n, 's'); }

// Compare case-insensitive.

static inline
int llsnnCMP
( const lchar_t *s1, size_t s1n, const lchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('c', 'l', s1, s1n, 'l', s2, s2n, 'i'); }

static inline
int lusnnCMP
( const lchar_t *s1, size_t s1n, const uchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('c', 'l', s1, s1n, 'u', (const lchar_t*)s2, s2n, 'i'); }

static inline
int ulsnnCMP
( const uchar_t *s1, size_t s1n, const lchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('c', 'u', (const lchar_t*)s1, s1n, 'l', s2, s2n, 'i'); }

static inline
int uusnnCMP
( const uchar_t *s1, size_t s1n, const uchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('c', 'u', (const lchar_t*)s1, s1n, 'u', (const lchar_t*)s2, s2n, 'i'); }

extern
int bbsnncmp
( const byte_t *s1, const byte_t *s2, size_t sn )
#if defined(LUB_DEFINITIONS)
{ if (LUB_PTR_ERR(s1, 0) || LUB_PTR_ERR(s2, 0))
    return LUB_INT_ERR(LUB_PTR_INVALID, 0);
  for (; sn; --sn, ++s1, ++s2)
  { if (*s1 != *s2)
      return *s1 < *s2 ? LUB_CMP_LESS_THAN : LUB_CMP_GREATER_THAN;
  }
  return LUB_CMP_EQUAL;
}
#else
;
#endif // LUB_DEFINITIONS for bbsnncmp.

/** @} */

/**
 * @defgroup FixedLengthLeadingSubstringCompare
 * Fixed-Length Leading Substring Compare
 * @name llsnnfxdcmp, lusnnfxdcmp, ulsnnfxdcmp, uusnnfxdcmp (case-sensitive)
 *       llsnnFXDCMP, lusnnFXDCMP, ulsnnFXDCMP, uusnnFXDCMP (case-insensitive)
 * @brief Compare of sn-length leading substrings of two source strings.
 * @param s1 Pointer to first source string.
 * @param s1n Number of characters in the leading substring
 *            of s1 to compare. Clamped to LUB_MAX_LSTRLEN or
 *            LUB_MAX_USTRLEN. Also, clamped to the length of
 *            s1 if the length of s1 is less than s1n.
 * @param s2 Pointer to second source string.
 * @param s2n Number of characters in the leading substring
 *            of s2 to compare. Clamped to LUB_MAX_LSTRLEN or
 *            LUB_MAX_USTRLEN. Also, clamped to the length of
 *            s2 if the length of s2 is less than s2n.
 * @return 0 if the leading substrings are equal.
 *         If s1 or s2 is NULL, it is treated as a 0-length string.
 *         If both substrings are 0-length, they are considered
 *         equal.
 * 
 *        -1 if leading substring of s1 < s2,
 * 
 *         1 if leading substring of s1 > s2.
 *
 * @note Errors:
 * - (int)LUB_PTR_INVALID if s1 or s2 is an invalid pointer.
 *
 * @note s1 and s2 are not checked for null-termination beyond
 *       s1n and s2n, respectively.
 * @{
 */

// Fixed-length leading substring compare case-sensitive.

static inline
int llsnnfxdcmp
( const lchar_t *s1, size_t s1n, const lchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('f', 'l', s1, s1n, 'l', s2, s2n, 's'); }

static inline
int lusnnfxdcmp
( const lchar_t *s1, size_t s1n, const uchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('f', 'l', s1, s1n, 'u', (const lchar_t*)s2, s2n, 's'); }

static inline
int ulsnnfxdcmp
( const uchar_t *s1, size_t s1n, const lchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('f', 'u', (const lchar_t*)s1, s1n, 'l', s2, s2n, 's'); }

static inline
int uusnnfxdcmp
( const uchar_t *s1, size_t s1n, const uchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('f', 'u', (const lchar_t*)s1, s1n, 'u', (const lchar_t*)s2, s2n, 's'); }

// Fixed-length leading substring compare case-insensitive.

static inline
int llsnnFXDCMP
( const lchar_t *s1, size_t s1n, const lchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('f', 'l', s1, s1n, 'l', s2, s2n, 'i'); }

static inline
int lusnnFXDCMP
( const lchar_t *s1, size_t s1n, const uchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('f', 'l', s1, s1n, 'u', (const lchar_t*)s2, s2n, 'i'); }

static inline
int ulsnnFXDCMP
( const uchar_t *s1, size_t s1n, const lchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('f', 'u', (const lchar_t*)s1, s1n, 'l', s2, s2n, 'i'); }

static inline
int uusnnFXDCMP
( const uchar_t *s1, size_t s1n, const uchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('f', 'u', (const lchar_t*)s1, s1n, 'u', (const lchar_t*)s2, s2n, 'i'); }

/** @} */

/**
 * @defgroup PrefixCompare Prefix Compare
 * @name llsnnpfxcmp, lusnnpfxcmp, ulsnnpfxcmp, and uusnnpfxcmp (case-sensitive)
 *       llsnnPFXCMP, lusnnPFXCMP, ulsnnPFXCMP, and uusnnPFXCMP (case-insensitive)
 * @brief Compare leading characters of s1 with s2.
 * @param s1 Pointer to source string.
 * @param s1n Bound on the length of s1. Clamped to
 *            LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * @param s2 Pointer to prefix string.
 * @param s2n Bound on the length of s2. Clamped to
 *            LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * @return 0 if s2 is a prefix of s1,
 *         LUB_CMP_LESS_THAN (-1) if s1 is less than s2.
 *         LUB_CMP_GREATER_THAN (1) if s1 is greater than s2.
 *         Or error.
 *
 * @note Errors:
 * - (int)LUB_PTR_INVALID if s1 or s2 is an invalid pointer.
 * - (int)LUB_UNTERMINATED if s1 or s2 is not null-terminated.
 * @{
 */

// Prefix compare case-sensitive.

static inline
int llsnnpfxcmp
( const lchar_t *s1, size_t s1n, const lchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('p', 'l', s1, s1n, 'l', s2, s2n, 's'); }

static inline
int lusnnpfxcmp
( const lchar_t *s1, size_t s1n, const uchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('p', 'l', s1, s1n, 'u', (const lchar_t*)s2, s2n, 's'); }

static inline
int ulsnnpfxcmp
( const uchar_t *s1, size_t s1n, const lchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('p', 'u', (const lchar_t*)s1, s1n, 'l', s2, s2n, 's'); }

static inline
int uusnnpfxcmp
( const uchar_t *s1, size_t s1n, const uchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('p', 'u', (const lchar_t*)s1, s1n, 'u', (const lchar_t*)s2, s2n, 's'); }

// Fixed-length leading substring compare case-insensitive.

static inline
int llsnnPFXCMP
( const lchar_t *s1, size_t s1n, const lchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('p', 'l', s1, s1n, 'l', s2, s2n, 'i'); }

static inline
int lusnnPFXCMP
( const lchar_t *s1, size_t s1n, const uchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('p', 'l', s1, s1n, 'u', (const lchar_t*)s2, s2n, 'i'); }

static inline
int ulsnnPFXCMP
( const uchar_t *s1, size_t s1n, const lchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('p', 'u', (const lchar_t*)s1, s1n, 'l', s2, s2n, 'i'); }

static inline
int uusnnPFXCMP
( const uchar_t *s1, size_t s1n, const uchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('p', 'u', (const lchar_t*)s1, s1n, 'u', (const lchar_t*)s2, s2n, 'i'); }

/** @} */

/**
 * @defgroup SuffixCompare Suffix Compare
 * @name llsnnsfxcmp, lusnnsfxcmp, ulsnnsfxcmp, and uusnnsfxcmp (case-sensitive)
 *       llsnnSFXCMP, lusnnSFXCMP, ulsnnSFXCMP, and uusnnSFXCMP (case-insensitive)
 * @brief Comparing trailing characters of s1 with s2.
 * @param s1 Pointer to source string.
 * @param s1n Bound on the length of s1.
 *            s1n clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * @param s2 Pointer to suffix string.
 * @param s2n Bound on the length of s2.
 *            s2n clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * @return 0 if s2 is a suffix of s1,
 *         LUB_CMP_LESS_THAN (-1) if the suffix of s1 is less than s2.
 *         LUB_CMP_GREATER_THAN (1) if the suffix of s1 is greater than s2.
 *         Or error.
 * 
 * @note Errors:
 * - (int)LUB_PTR_INVALID if s1 or s2 is an invalid pointer.
 * - (int)LUB_UNTERMINATED if s1 or s2 is not null-terminated.
 * @{
 */

// Suffix check (case-sensitive).
// Fixed-length leading substring compare case-sensitive.

static inline
int llsnnsfxcmp
( const lchar_t *s1, size_t s1n, const lchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('s', 'l', s1, s1n, 'l', s2, s2n, 's'); }

static inline
int lusnnsfxcmp
( const lchar_t *s1, size_t s1n, const uchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('s', 'l', s1, s1n, 'u', (lchar_t*)s2, s2n, 's'); }

static inline
int ulsnnsfxcmp
( const uchar_t *s1, size_t s1n, const lchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('s', 'u', (lchar_t*)s1, s1n, 'l', s2, s2n, 's'); }

static inline
int uusnnsfxcmp
( const uchar_t *s1, size_t s1n, const uchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('s', 'u', (lchar_t*)s1, s1n, 'u', (lchar_t*)s2, s2n, 's'); }

// Fixed-length leading substring compare case-insensitive.

static inline
int llsnnSFXCMP
( const lchar_t *s1, size_t s1n, const lchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('s', 'l', s1, s1n, 'l', s2, s2n, 'i'); }

static inline
int lusnnSFXCMP
( const lchar_t *s1, size_t s1n, const uchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('s', 'l', s1, s1n, 'u', (lchar_t*)s2, s2n, 'i'); }

static inline
int ulsnnSFXCMP
( const uchar_t *s1, size_t s1n, const lchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('s', 'u', (lchar_t*)s1, s1n, 'l', s2, s2n, 'i'); }

static inline
int uusnnSFXCMP
( const uchar_t *s1, size_t s1n, const uchar_t *s2, size_t s2n )
{ return lub_cfps_cmp_ext
           ('s', 'u', (lchar_t*)s1, s1n, 'u', (lchar_t*)s2, s2n, 'i'); }

/** @} */

/**
 * @defgroup PointerTrim Pointer Trim
 * @name llsnptrim, lusnptrim, ulsnptrim, uusnptrim
 * @brief The trimmed substring of a string.
 * @param s Pointer to source string.
 * @param sn Bound for the source string. Clamped to LUB_MAX_LSTRLEN or
 *           LUB_MAX_USTRLEN.
 * @param trim Pointer to a source trim indicator string (optional
 *             alphabetic trim mode character and trim characters).
 *             See parameter trim in @ref CommonParameters
 *             "Common Parameters" for details.
 * @param trimlen Pointer to size_t to receive the resulting length
 *                of the trimmed substring of s.
 * @return For left or both trim, pointer into s at the first 
 *         non-trim character, if any; otherwise, returns s.
 *         For right trim, returns s.
 * 
 *         If trimlen is non-NULL, sets *trimlen to the length of
 *         the trimmed substring of s (which is 0 if the trimmed result
 *         is empty or an error occurs).
 * 
 *         If the trimmed substring of s is empty (s is all trim characters),
 *         returns a pointer to the null terminator for s.
 * 
 * @note Errors:
 * - (lchar_t/uchar_t *)LUB_PTR_INVALID if s or trim is an invalid pointer.
 * - (lchar_t/uchar_t *)LUB_UNTERMINATED if s or trim is not null terminated.
 * - (lchar_t/uchar_t *)LUB_OPT_TOO_LONG if trim is too long.
 * - (lchar_t/uchar_t *)LUB_OPT_INVALID if trim is not a valid trim string.
 * - (lchar_t/uchar_t *)LUB_OPT_RESERVED if trim contains a reserved option character.
 *
 * @note The returned pointer and length can be used to access the trimmed
 *       substring of the string. The substring is null terminated if
 *       the trim mode is 'L' or 'l'; otherwise, it will not be
 *       null-terminated if characters are trimmed from the right.
 * @{
 */

// Helper function for pointer trim operations (ll, lu, ul, uu).

extern
const lchar_t *lub_pointer_trim_ext
( const char xs, const lchar_t *s, size_t sn,
  const char xt, const lchar_t *trim,
  size_t *trimlen
  // xs: 'l' (Latin) or 'u' (Unicode) for source string.
  // xt: 'l' (Latin) or 'u' (Unicode) for trim string.
)
#if defined(LUB_DEFINITIONS)
{ const uchar_t *us = (const uchar_t *)s;
  const uchar_t *utrim = (const uchar_t *)trim;

  // Validate xs and xt encoding values.
  if (xs != 'l' && xs != 'u' ||
      xt != 'l' && xt != 'u')
    return (const lchar_t *)LUB_PTR_ERR(LUB_INTERNAL_ERROR, 0);

  if (trimlen) *trimlen = 0;

  // Retrieve actual length of source string.
  sn = xs == 'u' ?
       ucsnlen(us, sn > LUB_MAX_USTRLEN ? LUB_MAX_USTRLEN : sn) :
       lcsnlen(s, sn > LUB_MAX_LSTRLEN ? LUB_MAX_LSTRLEN : sn);
  if (LUB_SIZE_ERR(sn, 0)) return (const lchar_t *)LUB_PTR_ERR(sn, 0);

  // Validate and extract trim mode and trimset.
  char trim_mode = 'B';
  const lchar_t *trimset = trim;
  if (trim)
  { int istrimstr = xt == 'u' ? isutrimstr((const uchar_t *)trim)
                              : isltrimstr(trim);
    if (LUB_INT_ERR(istrimstr, 0)) return (const lchar_t *)LUB_PTR_ERR(istrimstr, 0);
    if (xt == 'u')
    { if (*utrim && isualpha((int)*utrim))
        trim_mode = (char)uutoupper((int)*utrim++);
      if (!*utrim) utrim = (const uchar_t *)NULL;
      trimset = (const lchar_t *)utrim;
    }
    else
    { if (*trimset && islalpha((int)*trimset))
        trim_mode = (char)toupper((int)*trimset++);
      if (!*trimset) trimset = (const lchar_t *)NULL;
    }
  }

  if (!s) return (const lchar_t *)NULL;

  // Handle all encoding combinations for trim operation.
  if (xs == 'l')
  { const lchar_t *left = s;
    const lchar_t *right = s + sn;

    if (xt == 'l')
    { // Source and trim are both Latin.
      const lchar_t *ltrimset = trimset;
      if (trim_mode == 'L' || trim_mode == 'B')
        if (ltrimset)
          while (left < right)
          { const lchar_t *scan = ltrimset;
            while (*scan && (int)*left != (int)*scan) ++scan;
            if (!*scan) break;
            ++left;
          }
        else
          while (left < right && islspace((int)*left)) ++left;

      if (trim_mode == 'R' || trim_mode == 'B')
        if (ltrimset)
          while (right > left)
          { const lchar_t *scan = ltrimset;
            while (*scan && (int)*(right - 1) != (int)*scan) ++scan;
            if (!*scan) break;
            --right;
          }
        else
          while (right > left && islspace((int)*(right - 1))) --right;
    }
    else
    { // Source is Latin, trim is Unicode.
      const uchar_t *utrimset = utrim;
      if (trim_mode == 'L' || trim_mode == 'B')
        if (utrimset)
          while (left < right)
          { const uchar_t *scan = utrimset;
            while (*scan && (int)*left != (int)*scan) ++scan;
            if (!*scan) break;
            ++left;
          }
        else
          while (left < right && islspace((int)*left)) ++left;

      if (trim_mode == 'R' || trim_mode == 'B')
        if (utrimset)
          while (right > left)
          { const uchar_t *scan = utrimset;
            while (*scan && (int)*(right - 1) != (int)*scan) ++scan;
            if (!*scan) break;
            --right;
          }
        else
          while (right > left && islspace((int)*(right - 1))) --right;
    }

    if (trimlen) *trimlen = (size_t)(right - left);
    return left;
  }
  else
  { // Source is Unicode.
    const uchar_t *left = us;
    const uchar_t *right = us + sn;

    if (xt == 'l')
    { // Source is Unicode, trim is Latin.
      const lchar_t *ltrimset = trimset;
      if (trim_mode == 'L' || trim_mode == 'B')
        if (ltrimset)
          while (left < right)
          { const lchar_t *scan = ltrimset;
            while (*scan && (int)*left != (int)*scan) ++scan;
            if (!*scan) break;
            ++left;
          }
        else
          while (left < right && isuspace((int)*left)) ++left;

      if (trim_mode == 'R' || trim_mode == 'B')
        if (ltrimset)
          while (right > left)
          { const lchar_t *scan = ltrimset;
            while (*scan && (int)*(right - 1) != (int)*scan) ++scan;
            if (!*scan) break;
            --right;
          }
        else
          while (right > left && isuspace((int)*(right - 1))) --right;
    }
    else
    { // Source and trim are both Unicode.
      const uchar_t *utrimset = utrim;
      if (trim_mode == 'L' || trim_mode == 'B')
        if (utrimset)
          while (left < right)
          { const uchar_t *scan = utrimset;
            while (*scan && (int)*left != (int)*scan) ++scan;
            if (!*scan) break;
            ++left;
          }
        else
          while (left < right && isuspace((int)*left)) ++left;

      if (trim_mode == 'R' || trim_mode == 'B')
        if (utrimset)
          while (right > left)
          { const uchar_t *scan = utrimset;
            while (*scan && (int)*(right - 1) != (int)*scan) ++scan;
            if (!*scan) break;
            --right;
          }
        else
          while (right > left && isuspace((int)*(right - 1))) --right;
    }

    if (trimlen) *trimlen = (size_t)(right - left);
    return (const lchar_t *)left;
  }
}
#else
;
#endif // LUB_DEFINITIONS for lub_pointer_trim_ext.

static inline
const lchar_t *llsnptrim
( const lchar_t *s, size_t sn,
  const lchar_t * const trim,
  size_t *const trimlen
)
{ return lub_pointer_trim_ext
           ('l', s, sn, 'l', trim, trimlen); }

static inline
const lchar_t *lusnptrim
( const lchar_t *s, size_t sn,
  const uchar_t * const trim,
  size_t *const trimlen
)
{ return lub_pointer_trim_ext
           ('l', s, sn, 'u', (const lchar_t *)trim, trimlen); }

static inline
const uchar_t *ulsnptrim
( const uchar_t *s, size_t sn,
  const lchar_t * const trim,
  size_t *const trimlen
)
{ return (const uchar_t *)lub_pointer_trim_ext
           ('u', (const lchar_t *)s, sn, 'l', trim, trimlen); }

static inline
const uchar_t *uusnptrim
( const uchar_t *s, size_t sn,
  const uchar_t * const trim,
  size_t *const trimlen
)
{ return (const uchar_t *)lub_pointer_trim_ext
           ('u', (const lchar_t *)s, sn, 'u', (const lchar_t *)trim, trimlen); }

/** @} */

/**
 * @defgroup Skip Skip
 * @name llsnskip, ulsnskip, uusnskip
 * @brief Skip characters in skip.
 * @param s Pointer to source string.
 * @param sn Bound for the source string (clamped to LUB_MAX_LSTRLEN
 *           or LUB_MAX_USTRLEN).
 * @param skip Pointer to string of characters to skip.
 *             If NULL or empty string, whitespace is skipped by default.
 *             Length default bounded to LUB_MAX_LOPTLEN or LUB_MAX_UOPTLEN.
 * @return If no error, pointer into s at the first non-skip character, if any;
 *         otherwise, returns s. If the result after skipping is empty
 *         (all skip characters), returns pointer to the null terminator for s.
 *
 * @note Errors:
 * - (lchar_t/uchar_t *)LUB_PTR_INVALID if s or skip is an invalid pointer.
 * - (lchar_t/uchar_t *)LUB_UNTERMINATED if s or skip is not null terminated.
 *
 * @note lusnskip is not provided.
 * @{
 */

#if defined(LUB_DEFINITIONS)

// Skip function macro helper.
#undef LUB_OP_DEF
#define LUB_OP_DEF(s_xt, s_xmax, s_xcsnlen, \
                   skip_xt, skip_max, skip_xcsnlen, \
                   skip_space_expr, skip_match_expr) \
{ /* Validate parameters. */ \
  /* Note: *csnlen functions return 0 if argument is NULL. */ \
  sn = s_xcsnlen(s, sn > s_xmax ? s_xmax : sn); \
  if (LUB_SIZE_ERR(sn, 0)) return (s_xt *)LUB_PTR_ERR(sn, 0); \
  size_t skip_n = skip_xcsnlen(skip, skip_max); \
  if (LUB_SIZE_ERR(skip_n, 0)) return (s_xt *)LUB_PTR_ERR(skip_n, 0); \
  /* Return if s is null or has a 0 length. */ \
  if (!sn) return (const s_xt *)s; \
  if (skip_n) \
  { for (; *s; s++) \
    { const skip_xt *ss = skip; \
      for (; *ss && !(skip_match_expr); ss++); \
        if (!*ss) return (s_xt *)s; \
    } \
  } \
  else \
    while (*s && (skip_space_expr)) s++; \
  return (s_xt *)s; \
}

#endif // LUB_DEFINITIONS for skip macro helper.

extern
lchar_t *llsnskip
( const lchar_t *s, size_t sn, const lchar_t *skip )
#if defined(LUB_DEFINITIONS)
LUB_OP_DEF(lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
           lchar_t, LUB_MAX_LOPTLEN, lcsnlen,
           islspace((int)*s),
           (*s == *ss))
#else
;
#endif // LUB_DEFINITIONS for llsskip.

extern
uchar_t *ulsnskip
( const uchar_t *s, size_t sn, const lchar_t *skip )
#if defined(LUB_DEFINITIONS)
LUB_OP_DEF(uchar_t, LUB_MAX_USTRLEN, ucsnlen,
           lchar_t, LUB_MAX_LOPTLEN, lcsnlen,
           (*s <= LUB_MAX_LCHAR && islspace((int)(lchar_t)*s)),
           (*s == (uchar_t)*ss))
#else
;
#endif // LUB_DEFINITIONS for ulsnskip.

extern
uchar_t *uusnskip
( const uchar_t *s, size_t sn, const uchar_t *skip )
#if defined(LUB_DEFINITIONS)
LUB_OP_DEF(uchar_t, LUB_MAX_USTRLEN, ucsnlen,
           uchar_t, LUB_MAX_UOPTLEN, ucsnlen,
           isuspace((unsigned int)*s),
           (*s == *ss))
#else
;
#endif // LUB_DEFINITIONS for uusnskip.
/** @} */

/**
 * @defgroup StringSearch String Search
 * @name llsnstrm, lusnstrm, ulsnstrm, uusnstrm (case-sensitive)
 *       llsnSTRM, lusnSTRM, ulsnSTRM, uusnSTRM (case-insensitive)
 * @brief Search for the mth occurrence of a needle substring or a needle
 *        character in string s1.
 * @param s1 Pointer to string (haystack) to search.
 *           If NULL, treated as 0-length string.
 * @param s1n Bound on s1. Clamped to LUB_MAX_LSTRLEN or
 *            LUB_MAX_USTRLEN.
 * @param s2 Pointer to string of needle substrings separated by delim or 
 *           string of needle characters if delim is the null character.
 *           Implicitly, clamped to LUB_MAX_LOPTLEN or LUB_MAX_UOPTLEN.
 * 
 *           A zero-length needle substring is treated as a match
 *           for a zero-length string s1 if m is -1 or 1, otherwise,
 *           there is no match for this needle substring.
 * 
 *           If delim is the null character and s2 is zero-length,
 *           there is no match to s1.
 * @param delim Delimiter character (case-sensitive) for needle substrings.
 *              If delim is the null character, indicates
 *              s2 is a string of needle characters.
 * @param m Indicates to search for the mth occurrence of any needle
 *          substring or needle character.
 * 
 *          m > 0 counts from the beginning of the haystack string s1.
 *                (1 means first occurrence).
 * 
 *          m == 0 returns NULL (not found).
 * 
 *          m < 0 counts from the end of the haystack string s1.
 *               (-1 means last occurrence).
 *
 *          Example (character-needle mode, delim = '\0'):
 * 
 *            haystack = "aaabccdeebe", needles = "db"
 *            m = 1 -> points to first 'b'
 *            m = 2 -> points to 'd'
 *            m = 3 -> points to second 'b'
 * @return Pointer to mth match in s1, NULL if not found or m is 0, or error.
 *
 * @note Errors:
 *  - (lchar_t/uchar_t *)LUB_PTR_INVALID if s1 or s2 is an invalid pointer.
 *  - (lchar_t/uchar_t *)LUB_UNTERMINATED if s1 or s2 is not null-terminated.
 * @{
 */

#if defined(LUB_DEFINITIONS)

static inline int lub_search_char_def
( const char xs, const lchar_t *s, const uchar_t *us,
  const size_t i, const char Case
)
{ int c = xs == 'u' ? (int)us[i] : (int)s[i];
  return Case == 'i' ? (xs == 'u' ? uutoupper(c) : lltoupper(c)) : c;
}

static
size_t lub_match_count_def
( const char xs1, const lchar_t *s1, size_t *s1n_ptr,
  const char xs2, const lchar_t *s2, size_t *s2_n_ptr,
  const int delim, const char Case
)
{ const uchar_t *us1 = (const uchar_t *)s1;
  const uchar_t *us2 = (const uchar_t *)s2;

  if (xs1 != 'l' && xs1 != 'u' ||
      xs2 != 'l' && xs2 != 'u' ||
      Case != 's' && Case != 'i')
    return LUB_SIZE_ERR(LUB_INTERNAL_ERROR, 0);

  if (LUB_PTR_ERR(s1, 0)) return LUB_SIZE_ERR(LUB_PTR_INVALID, 0);
  *s1n_ptr = xs1 == 'u' ?
             ucsnlen(us1, *s1n_ptr > LUB_MAX_USTRLEN ?
             LUB_MAX_USTRLEN : *s1n_ptr) :
             lcsnlen(s1, *s1n_ptr > LUB_MAX_LSTRLEN ?
             LUB_MAX_LSTRLEN : *s1n_ptr);
  if (LUB_SIZE_ERR(*s1n_ptr, 0)) return *s1n_ptr;

  int isneedlestr = xs2 == 'u' ?
                    isuneedlestr(us2) :
                    islneedlestr(s2);
  if (LUB_INT_ERR(isneedlestr, 0))
    return LUB_SIZE_ERR(isneedlestr, 0);

  *s2_n_ptr = 0;
  if (xs2 == 'u')
    for (; us2[*s2_n_ptr]; ++(*s2_n_ptr));
  else
    for (; s2[*s2_n_ptr]; ++(*s2_n_ptr));

  if (!*s2_n_ptr) return (size_t)0;

  size_t s1n = *s1n_ptr;
  size_t s2_n = *s2_n_ptr;
  size_t total_count = 0;

  if (!delim)
  { /* Character-set mode: count occurrences of each character in s2 within s1 */
    for (size_t j = 0; j < s2_n; ++j)
    { int needle_char = lub_search_char_def(xs2, s2, us2, j, Case);
      for (size_t i = 0; i < s1n; ++i)
      { int s1_char = lub_search_char_def(xs1, s1, us1, i, Case);
        if (s1_char == needle_char)
          ++total_count;
      }
    }
  }
  else
  { /* Token mode: parse delim-separated tokens and count each */
    size_t seg = 0;
    for (; seg <= s2_n; )
    { size_t end = seg;
      while (end < s2_n)
      { int cd = lub_search_char_def(xs2, s2, us2, end, 's');
        if (cd == delim) { break; }
        ++end;
      }
      size_t tok_n = end - seg;
      if (!tok_n) { if (!s1n) { ++total_count; } }
      else if (tok_n <= s1n)
      { for (size_t i = 0; i <= s1n - tok_n; ++i)
        { size_t k = 0;
          for (; k < tok_n; ++k)
          { int c1 = lub_search_char_def(xs1, s1, us1, i + k, Case);
            int c2 = lub_search_char_def(xs2, s2, us2, seg + k, Case);
            if (c1 != c2) { break; }
          }
          if (k == tok_n) { ++total_count; }
      }
        }
      seg = end + 1;
    }
  }

  return total_count;
}

#endif // LUB_DEFINITIONS for strm/cnt functions.

extern
lchar_t *lub_strm_ext
( const char xs1, const lchar_t *s1, size_t s1n,
  const char xs2, const lchar_t *s2,
  const int delim, const int m,
  const char Case
)
#if defined(LUB_DEFINITIONS)
{ const uchar_t *us1 = (const uchar_t *)s1;
  const uchar_t *us2 = (const uchar_t *)s2;

  if (!m) { return (lchar_t *)NULL; }

  size_t s2_n = s1n;
  size_t total_count = lub_match_count_def(xs1, s1, &s1n, xs2, s2, &s2_n, delim, Case);
  if (LUB_SIZE_ERR(total_count, 0)) { return (lchar_t *)LUB_PTR_ERR(total_count, 0); }

  if (!delim && !s2_n) return (lchar_t *)NULL;

  size_t target = (size_t)(m < 0 ? -(long long)m : (long long)m);
  int pass = m < 0 ? 0 : 1;
  for (; pass < 2; ++pass)
  { size_t count = 0;
    if (!delim)
    { for (size_t i = 0; i < s1n; ++i)
      { for (size_t k = 0; k < s2_n; ++k)
        { int c1 = lub_search_char_def(xs1, s1, us1, i, Case);
          int c2 = lub_search_char_def(xs2, s2, us2, k, Case);
          if (c1 == c2)
          { if (!pass) { ++count; }
            else if (++count == target)
            { return xs1 == 'u' ? (lchar_t *)(us1 + i)
                                : (lchar_t *)(s1 + i);
            }
            break;
          }
        }
      }
    }
    else
    { size_t seg = 0;
      for (; seg <= s2_n; )
      { size_t end = seg;
        while (end < s2_n)
        { int cd = lub_search_char_def(xs2, s2, us2, end, 's');
          if (cd == delim) { break; }
          ++end;
        }
        size_t tok_n = end - seg;
        if (!tok_n)
        { if (!s1n && (m == 1 || m == -1))
          { if (!pass) { ++count; }
            else if (++count == target) { return (lchar_t *)s1; }
          }
        }
        else if (tok_n <= s1n)
        { for (size_t i = 0; i <= s1n - tok_n; ++i)
          { size_t k = 0;
            for (; k < tok_n; ++k)
            { int c1 = lub_search_char_def(xs1, s1, us1, i + k, Case);
              int c2 = lub_search_char_def(xs2, s2, us2, seg + k, Case);
              if (c1 != c2) { break; }
            }
            if (k == tok_n)
            { if (!pass) { ++count; }
              else if (++count == target)
              { return xs1 == 'u' ? (lchar_t *)(us1 + i)
                                  : (lchar_t *)(s1 + i);
              }
            }
          }
        }
        seg = end + 1;
      }
    }

    if (!pass)
    { if (count < target) { return (lchar_t *)NULL; }
      target = count - target + 1;
    }
  }

  return (lchar_t *)NULL;
}
#else
;
#endif // LUB_DEFINITIONS for LUB__strm_helper.

// String search (case-sensitive).

static inline
lchar_t *llsnstrm
( const lchar_t *s1, size_t s1n, const lchar_t *const s2,
  const lchar_t delim, const int m
)
{ return (lchar_t *)lub_strm_ext
           ('l', s1, s1n,
            'l', s2, (int)delim, m, 's'); }

static inline
lchar_t *lusnstrm
( const lchar_t *s1, size_t s1n, const uchar_t *const s2,
  const uchar_t delim, const int m
)
{ return (lchar_t *)lub_strm_ext
           ('l', s1, s1n,
            'u', (const lchar_t *)s2, (int)delim, m, 's'); }

static inline
uchar_t *ulsnstrm
( const uchar_t *s1, size_t s1n, const lchar_t *const s2,
  const lchar_t delim, const int m
)
{ return (uchar_t *)lub_strm_ext
           ('u', (const lchar_t *)s1, s1n,
            'l', s2, (int)delim, m, 's'); }

static inline
uchar_t *uusnstrm
( const uchar_t *const s1, size_t s1n, const uchar_t *const s2,
  const uchar_t delim, const int m
)
{ return (uchar_t *)lub_strm_ext
           ('u', (const lchar_t *)s1, s1n,
            'u', (const lchar_t *)s2, (int)delim, m, 's'); }

// String search (case-insensitive).

static inline
lchar_t *llsnSTRM
( const lchar_t *s1, size_t s1n, const lchar_t *const s2,
  const lchar_t delim, const int m
)
{ return (lchar_t *)lub_strm_ext
           ('l', s1, s1n,
            'l', s2, (int)delim, m, 'i'); }

static inline
lchar_t *lusnSTRM
( const lchar_t *s1, size_t s1n, const uchar_t *const s2,
    const uchar_t delim, const int m
)
{ return (lchar_t *)lub_strm_ext
           ('l', s1, s1n, 'u',
            (const lchar_t *)s2, (int)delim, m, 'i'); }

static inline
uchar_t *ulsnSTRM
( const uchar_t *s1, size_t s1n, const lchar_t *const s2,
  const lchar_t delim, const int m
)
{ return (uchar_t *)lub_strm_ext
           ('u', (const lchar_t *)s1, s1n,
            'l', s2, (int)delim, m, 'i'); }

static inline
uchar_t *uusnSTRM
( const uchar_t *s1, size_t s1n, const uchar_t *const s2,
  const uchar_t delim, const int m
)
{ return (uchar_t *)lub_strm_ext
           ('u', (const lchar_t *)s1, s1n,
            'u', (const lchar_t *)s2, (int)delim, m, 'i'); }

/** @} */

/**
 * @defgroup Count Count
 * @name llsncnt, ulsncnt, uusncnt (case-sensitive)
 *       llsnCNT, ulsnCNT, uusnCNT (case-insensitive)
 * @brief Count of matches of needle substrings or needle
 *        characters in string s1 (for substrings, overlapping
 *        matches are counted).
 * @param s1 Pointer to string (haystack) to search.
 *           If NULL, treated as 0-length string.
 * @param s1n Bound on s1. Clamped to LUB_MAX_LSTRLEN or
 *            LUB_MAX_USTRLEN.
 * @param s2 Pointer to string of needle substrings separated by delim or 
 *           string of needle characters if delim is the null character.
 *           Implicitly, clamped to LUB_MAX_LOPTLEN or LUB_MAX_UOPTLEN.
 * @param delim Delimiter character (case-sensitive) for needle substrings.
 *              If delim is the null character, indicates s2 is a string
 *              of needle characters.
 * @return Count of matches, or error.
 *
 * @note Errors:
 * - (size_t)LUB_PTR_INVALID if s1 or s2 is an invalid pointer.
 * - (size_t)LUB_UNTERMINATED if s1 or s2 is not null-terminated.
 *
 * @note If s2 is empty, returns 0.
 * @{
 */

#if defined(LUB_DEFINITIONS)

static inline int lub_cnt_char_def
( const char xs, const lchar_t *s, const uchar_t *us,
  const size_t i, const char Case
)
{ int c = xs == 'u' ? (int)us[i] : (int)s[i];
  return Case == 'i' ? (xs == 'u' ? uutoupper(c) : lltoupper(c)) : c;
}

#endif // LUB_DEFINITIONS for lub_cnt_char_def

extern
size_t lub_cnt_ext
( const char xs1, const lchar_t *s1, size_t s1n,
  const char xs2, const lchar_t *s2,
  const int delim, const char Case
)
#if defined(LUB_DEFINITIONS)
{ size_t s2_n = s1n;
  return lub_match_count_def(xs1, s1, &s1n, xs2, s2, &s2_n, delim, Case);
}
#else
;
#endif // LUB_DEFINITIONS for LUB__cnt__ext.

// Count of matches of s2 needles in haystack string s1 (case-sensitive).

static inline
size_t llsncnt
( const lchar_t *s1, size_t s1n, const lchar_t *const s2,
        const lchar_t delim
)
{ return lub_cnt_ext
           ('l', s1, s1n,
            'l', s2, (int)delim, 's'); }

static inline
size_t ulsncnt(const uchar_t *s1, size_t s1n, const lchar_t *const s2,
                      const lchar_t delim)
{ return lub_cnt_ext
           ('u', (const lchar_t *)s1, s1n,
            'l', s2, (int)delim, 's'); }

static inline
size_t uusncnt(const uchar_t *s1, size_t s1n, const uchar_t *const s2,
                      const uchar_t delim)
{ return lub_cnt_ext
           ('u', (const lchar_t *)s1, s1n,
            'u', (const lchar_t *)s2, (int)delim, 's'); }

// Count of matches of s2 needles in string s1 (case-insensitive).

static inline
size_t llsnCNT(const lchar_t *s1, size_t s1n, const lchar_t *s2,
                      const lchar_t delim)
{ return lub_cnt_ext
           ('l', s1, s1n,
            'l', s2, (int)delim, 'i'); }

static inline
size_t ulsnCNT(const uchar_t *s1, size_t s1n, const lchar_t *const s2,
                      const lchar_t delim)
{ return lub_cnt_ext
           ('u', (const lchar_t *)s1, s1n,
            'l', s2, (int)delim, 'i'); }

static inline
size_t uusnCNT(const uchar_t *s1, size_t s1n, const uchar_t *const s2,
                      const uchar_t delim)
{ return lub_cnt_ext
           ('u', (const lchar_t *)s1, s1n,
            'u', (const lchar_t *)s2, (int)delim, 'i'); }

/** @} */

#if defined(LUB_DEFINITIONS)

// Concatenate/copy local static functions. Only visible
// to an API user if LUB_DEFINITIONS is defined. Documentation
// is not provided to API users.

static inline uchar_t lub_get_def
( const char xs, const lchar_t *s )
{ return (xs == 'u') ?
         *((const uchar_t *)s) :
         (lchar_t)*s;
}
static inline uchar_t lub_get_ith_def
( const char xs, const lchar_t *s, const size_t i )
{ return (xs == 'u') ?
         *((const uchar_t *)s + i) :
         (lchar_t)*(s + i);
}
static inline void lub_set_def
( char xt, lchar_t *t, const uchar_t c, const lchar_t lrep)
{ if (xt == 'u') { *((uchar_t *)t) = c; }
  else { *t = (c > LUB_MAX_LCHAR) ?
            (!lrep ? (lchar_t)'?' : lrep) : (lchar_t)c; }
  return;
}
static inline void lub_set_ith_def
( char xt, lchar_t *t, const size_t i, const uchar_t c, const lchar_t lrep )
{ if (xt == 'u') { *((uchar_t *)t + i) = c; }
  else { *(t + i) = (c > LUB_MAX_LCHAR) ?
                  (!lrep ? (lchar_t)'?' : lrep) : (lchar_t)c; }
  return;
}

static inline
void lub_terminate_def
( char xt, lchar_t *t)
{ if (xt == 'u') { *((uchar_t *)t) = (uchar_t)0; }
  else { *t = (lchar_t)0; }
  return;
}
static inline
void lub_terminate_ith_def
( char xt, lchar_t *t, const size_t i )
{ if (xt == 'u') { *((uchar_t *)t + i) = (uchar_t)0; }
  else { *(t + i) = (lchar_t)0; }
  return;
}

static
lchar_t *lub_cat_quoted_def
( const char xt, const char xs,
  const lchar_t q, const char Case,
  lchar_t *t, size_t tn, size_t tl, lchar_t *s, size_t sn,
  const char trunc_mode,
  const lchar_t *trunc, const size_t trunc_n,
  const lchar_t lrep
)
{ /** @todo check for truncation and trunc fit.
   */
   const size_t t_size = (xt == 'u') ? sizeof(uchar_t)
                                     : sizeof(lchar_t);
  const size_t s_size = (xs == 'u') ? sizeof(uchar_t)
                                    : sizeof(lchar_t);
  char *tb = (char *)t;
  const char *sb = (const char *)s;
  size_t tr = (tn > tl) ? (tn - tl) : 0;
  if (tr > 1)
  { lub_set_def(xt, (lchar_t *)tb, (uchar_t)q, lrep);
    tb += t_size;
    tr -= 2;
    while (tr && sn)
    { uchar_t c = lub_get_def(xs, (const lchar_t *)sb);
      if (!c) { break; }
      sb += s_size;
      sn--;
      if (c == (uchar_t)q)
      { if (tr == 1) { break; }
        lub_set_def(xt, (lchar_t *)tb, (uchar_t)q, lrep);
        tb += t_size;
        tr--;
      }
      c = (Case == 'C') ? uutoupper(c)
                        : (Case == 'c') ? uutolower(c)
                        : c;
      lub_set_def(xt, (lchar_t *)tb, c, lrep);
      tb += t_size;
      tr--;
    }    
    lub_set_def(xt, (lchar_t *)tb, (uchar_t)q, lrep);
    tb += t_size;
  }
  lub_set_def(xt, (lchar_t *)tb, 0, lrep);
  return sn ? (lchar_t *)LUB_PTR_ERR(LUB_TRUNCATED, 0)
            : (lchar_t *)tb;
}

#endif // LUB_DEFINITIONS

// Concatenate/copy/pad extern helper function.

extern
lchar_t *lub_cat_cpy_pad_ext
(   const char op,   // 0: cat, 1: cpy, 2: pad.
    const char xt,   // 'l' or 'u' for target type.
    const char xs,   // 'l' or 'u' for source type.
    const char Q,    // 0: not quoted.
                     // 'q' or 'Q': quoted or, if
                     //  Name = 'N', quoted as needed.  
    const char Name, // 0: not a name, 'N': name.
    const char Case, // 0: preserve, 'c': lower, 'C': upper.
    lchar_t *t, size_t tn, lchar_t *s, size_t sn,
    lchar_t q,       // If Q, must be '\'' or '"' and is the quote!
                     // character for quoted string and quoted
                     // name functions. If !Q, q must be 0.
    const lchar_t *trunc,
    const lchar_t *pad, // Must be NULL if op != 2 (pad).
    const lchar_t lrep
)
#if defined(LUB_DEFINITIONS)
{   // Assert valid internal argument values.
    if (op > 2 ||
        (xt != 'l' && xt != 'u') ||
        (xs != 'l' && xs != 'u') ||
        (Q && Q != 'q' && Q != 'Q') ||
        (Name && Name != 'N') ||
        (Case && Case != 'c' && Case != 'C') ||
        (xt != 'l' && xs != 'u' && lrep) ||
        (op != 2 /*pad*/ && pad))
    { return (lchar_t *)LUB_PTR_ERR(LUB_INTERNAL_ERROR, 0); }

    size_t t_size = (xt == 'u') ? sizeof(uchar_t) : sizeof(lchar_t);
    size_t s_size = (xs == 'u') ? sizeof(uchar_t) : sizeof(lchar_t);

    // Validate target t.
    size_t tl = 0; // Current length of t. Set to 0 for copy and pad.
    if (xt == 'u' && tn > LUB_MAX_USTRLEN)
    { tn = LUB_MAX_USTRLEN; }
    else if (tn > LUB_MAX_LSTRLEN)
    { tn = LUB_MAX_LSTRLEN; }
    if (!op /*concatenate*/)
    { // Get current length tl of t for concatenate.
      tl = (xt == 'u') ? ucsnlen((uchar_t *)t, tn) : lcsnlen(t, tn);
      if (LUB_SIZE_ERR(tl, 0))
      { if (LUB_INT_ERR(tl, LUB_UNTERMINATED) && t)
        { lub_terminate_def(xt, t); }
        return (lchar_t *)LUB_PTR_ERR(tl, 0);
      }
    }
    if (Name != 'N')
    { // Validate s and determine s length.
      sn = (xs == 'u') ?
            ucsnlen((uchar_t *)s,
                    sn > LUB_MAX_USTRLEN ? LUB_MAX_USTRLEN : sn) :
            lcsnlen(s,
                    sn > LUB_MAX_LSTRLEN ? LUB_MAX_LSTRLEN : sn);
      if (LUB_SIZE_ERR(sn, 0))
      { if (op && t) { lub_terminate_def(xt, t); }
        return (lchar_t *)LUB_PTR_ERR(LUB_PTR_INVALID, 0);
      }
      /* Validate q. */
      if (q != '\0' && q != '\'' && q != '"')
      { return (lchar_t *)LUB_PTR_ERR(LUB_OPT_INVALID, 0); }
    }
    else
    { const int quoted = (xs == 'u') ?
                         isuqname((const uchar_t *)s) :
                         islqname(s);
      if (LUB_INT_ERR(quoted, 0))
      { if (op && t) lub_terminate_def(xt, t);
        { return (lchar_t *)LUB_PTR_ERR(quoted, 0); }
      }
      if (q != '\'' && q != '"')
      { return (lchar_t *)LUB_PTR_ERR(LUB_OPT_INVALID, 0); }
      if (!quoted) { q = '\0'; }
    }

    // Validate trunc.
    const int istruncstr = xt == 'u' ?
                           isutruncstr((const uchar_t *)trunc) :
                           isltruncstr(trunc);
    if (LUB_INT_ERR(istruncstr, 0))
    { if (op && t) { lub_terminate_def(xt, t); }
      return (lchar_t *)LUB_PTR_ERR(istruncstr, 0);
    }

    // Determine trunc mode and trunc string.
    char trunc_mode = 'B';
    size_t trunc_n = 0;
    if (trunc)
    { trunc_mode = 'R';
      trunc_n = xt == 'u' ?
                ucsnlen((const uchar_t *)trunc, LUB_MAX_UOPTLEN) :
                lcsnlen(trunc, LUB_MAX_LOPTLEN);
      const int c = (int)lub_get_def(xt, trunc);
      if (islalpha(c))
      { trunc_mode = (char)toupper(c);
        trunc += t_size; trunc_n--;
      }
      if (trunc_n > tn)
      { if (op && t) { lub_terminate_def(xt, t); }
        return (lchar_t *)LUB_PTR_ERR(LUB_OPT_TOO_LONG, 0);
      }
    }

    char pad_mode = 0;
    uchar_t pad_c = 0;
    if (op == 2 /*pad*/)
    { // Validate pad.
      const int ispadstr = xt == 'u' ?
                           isupadstr((const uchar_t *)pad) :
                           islpadstr(pad);
      if (LUB_INT_ERR(ispadstr, 0))
      { if (t) { lub_terminate_def(xt, t); }
        return (lchar_t *)LUB_PTR_ERR(ispadstr, 0);
      }
      pad_mode = 'R';
      pad_c = (uchar_t)lub_get_def(xt, pad);
      if (islalpha((int)pad_c))
      { pad_mode = (char)toupper((int)pad_c);
        pad += t_size;
        pad_c = (uchar_t)lub_get_def(xt, pad);
      }
      if (!pad_c) { pad_c = (uchar_t)' '; }
    }

    // Validate source does not not contain out-of-range characters
    // when source is Unicode, target is Latin, and lrep is NULL.
    if (xt == 'l' && xs == 'u' && !lrep &&
        !isunlatinstr((const uchar_t *)s, sn))
    { if (op && t) { lub_terminate_def(xt, t); }
      return (lchar_t *)LUB_PTR_ERR(LUB_NONLATIN_SOURCE, 0);
    }
    if (!t) { return (lchar_t *)NULL; }

    if (q)
    { return lub_cat_quoted_def
               (xt, xs, q, Case,
                t + t_size * tl, tn, tl, s, sn,
                trunc_mode, trunc, trunc_n, lrep);
    }

    if (!s || !sn)
      { lub_terminate_def(xt, t + t_size * tl);
        return t + t_size * tl;
    }

    size_t copied = 0;
    for (; copied < sn && tl + copied < tn; ++copied)
    { uchar_t c = lub_get_ith_def(xs, s, copied);
      if (Case == 'C') c = uutoupper(c);
      else if (Case == 'c') c = uutolower(c);
      lub_set_ith_def(xt, t, tl + copied, c, lrep);
    }

    size_t out_n = tl + copied;
    const int was_truncated = (copied < sn);
    if (was_truncated)
    { if (trunc_mode && trunc && trunc_n)
      { if (trunc_n <= tn)
        { size_t keep = tn - trunc_n;
          if (out_n > keep) { out_n = keep;
          for (size_t i = 0; i < trunc_n && out_n < tn; ++i)
          { lub_set_ith_def(xt, t, out_n++, lub_get_ith_def(xt, trunc, i), lrep); }
        }
      }
        }
      lub_terminate_ith_def(xt, t, out_n);
      return (lchar_t *)LUB_PTR_ERR(LUB_TRUNCATED, 0);
    }

    if (op == 2 /*pad*/ && out_n < tn)
    { const size_t pad_n = tn - out_n;
      size_t left_pad = 0;
      size_t right_pad = 0;

      if (pad_mode == 'L') left_pad = pad_n;
      else if (pad_mode == 'R') right_pad = pad_n;
      else
      { left_pad = pad_n / 2;
        right_pad = pad_n - left_pad;
      }

      if (left_pad)
      { for (size_t i = out_n; i > 0; --i)
          lub_set_ith_def(xt, t, i + left_pad - 1,
                          lub_get_ith_def(xt, t, i - 1), lrep);
        for (size_t i = 0; i < left_pad; ++i)
          lub_set_ith_def(xt, t, i, pad_c, lrep);
      }

      if (right_pad)
      { const size_t right_start = left_pad + out_n;
        for (size_t i = 0; i < right_pad; ++i)
          lub_set_ith_def(xt, t, right_start + i, pad_c, lrep);
      }

      out_n = tn;
    }

    lub_terminate_ith_def(xt, t, out_n);
    return (xt == 'u') ? (lchar_t *)((uchar_t *)t + out_n)
                       : t + out_n;
}
#else
    ;
#endif // LUB_DEFINITIONS

/**
 * @defgroup Concatenate Concatenate
 * @name llsnncat, lusnncat, ulsnncat, uusnncat (case-preserving)
 *       llsnncatc, lusnncatc, ulsnncatc, uusnncatc (lowercase)
 *       llsnnCATC, lusnnCATC, ulsnnCATC, uusnnCATC (uppercase)
 *       llsnncatq, lusnncatq, ulsnncatq, uusnncatq (case-preserving, quoted)
 *       llsnncatqc, lusnncatqc, ulsnncatqc, uusnncatqc (lowercase, quoted)
 *       llsnnCATQC, lusnnCATQC, ulsnnCATQC, uusnnCATQC (uppercase, quoted)
 *       llsncatname, lusncatname, ulsncatname, uusncatname (case-preserving, unquoted/quoted)
 *       llsncatnamec, lusncatnamec, ulsncatnamec, uusncatnamec (lowercase, unquoted/quoted)
 *       llsnCATNAMEC, lusnCATNAMEC, ulsnCATNAMEC, uusnCATNAMEC (uppercase, unquoted/quoted)
 *       lbsnncatc, ubsnncatc (lowercase)
 *       lbsnnCATC, ubsnnCATC (uppercase)
 * @brief Concatenate to target character buffer with options for
 *        case-mapping (preserving, lower, or upper), quoting, and
 *        truncation handling.
 *
 *        For sncatname/snCATNAME functions, concatenate source
 *        if it is a valid unquoted name (see isuqname);
 *        otherwise, if it is valid name, concatenate quoted source.
 *        Error if not a valid name.
 * @param t Pointer to the target buffer.
 * @param tn tn is the maximum number of characters for buffer t,
 *           excluding the null terminator; tn is clamped to
 *           LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * @param s Pointer to source string.
 * @param sn For a character source string, sn is the maximum length of the
 *           string. sn is clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * 
 *           sn is omitted for sncatname/snCATNAME functions
 *           since the source is expected to be a name
 *           and thus sn defaults to LUB_MAX_NAMELEN (128 characters).
 * 
 *           For a copy of a byte string to a character string, sn 
 *           defines the length of the source string.
 * @param trunc Pointer to a string that specifies how to handle
 *              truncation (optional alphabetic truncate mode
 *              character and truncated replacement string).
 *              See parameter trunc in @ref CommonParameters
 *              "Common Parameters" for details.
 * @param q  Quote (' or ") character for quoted string and
 *           quoted name concatenate functions.
 * @param lrep For lus functions only,
 *             - if NULL ('\0') character, return an error for
 *               a Unicode source containing a character that is
 *               out-of-range for Latin.
 *             - Otherwise, use lrep as the replacement Latin character
 *               for out-of-range Unicode source characters.
 * @return t, NULL (if t is null), or an error.
 * 
 *         If t is not null and t is not null-terminated, the target buffer
 *         is set to an empty string (that is, *t = '\0').
 *
 * @note Errors:
 * - (lchar_t/uchar_t *)LUB_PTR_INVALID if t or s is an
 *   invalid pointer.
 * - (lchar_t/uchar_t *)LUB_UNTERMINATED if t or s is
 *   not null-terminated.
 * - (lchar_t/uchar_t *)LUB_OPT_TOO_LONG if trunc is too long.
 * - (lchar_t/uchar_t *)LUB_OPT_INVALID if trunc is invalid.
 * - (lchar_t/uchar_t *)LUB_OPT_RESERVED if trunc is valid
 *   except that the first character is a reserved alphabetic
 *   character.
 * - (lchar_t/uchar_t *)LUB_OVERLAP if source and target
 *   overlap when not allowed.
 * - (lchar_t/uchar_t *)LUB_TRUNCATED if truncation occurs.
 * - (lchar_t/uchar_t *)LUB_NONLATIN_SOURCE if a character
 *   for Latin when lrep is null.
 * - (lchar_t/uchar_t *)LUB_INTERNAL_ERROR if internal error
 *   detected.
 *
 * @note Except for lus and uls, q/Q, name/NAME functions,
 *       concatenate is target buffer overlap-safe (target
 *       result is correct but source may be overridden if
 *       there is overlap).
 * 
 *       For lus and uls, q/Q, name/NAME functions,
 *       overlap raises an error.
 * @{
 */

// Concatenate case-preserving.

static inline
lchar_t *llsnncat
(   lchar_t *t, size_t tn,
    lchar_t *s, size_t sn,
    const lchar_t *trunc
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'l', 'l', '\0', '\0', '\0',
             t, tn, s, sn,
             (lchar_t)'\0', trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           ); \
}

static inline
lchar_t *lusnncat
(   lchar_t *t, size_t tn,
    uchar_t *s, size_t sn, const lchar_t *trunc,
    const lchar_t lrep
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'l', 'u', '\0', '\0', '\0',
             t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', trunc, (lchar_t *)NULL,
             lrep
           ); \
}

static inline
uchar_t *ulsnncat
( uchar_t *t, size_t tn,
  lchar_t *s, size_t sn,
  const lchar_t *trunc
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'u', 'l', '\0', '\0', '\0',
             (lchar_t *)t, tn, s, sn,
             (lchar_t)'\0', (lchar_t *)trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           ); \
}

static inline
uchar_t *uusnncat
( uchar_t *t, size_t tn,
  uchar_t *s, size_t sn,
  const lchar_t *trunc
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'u', 'u', '\0', '\0', '\0',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
              (lchar_t)'\0', (lchar_t *)trunc, (lchar_t *)NULL,
              (lchar_t)'\0'
           ); \
}

// Concatenate lowercase.

static inline
lchar_t *llsnncatc
( lchar_t *t, size_t tn,
  lchar_t *s, size_t sn,
  const lchar_t *trunc
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'l', 'l', '\0', '\0', 'c',
             t, tn, s, sn,
              (lchar_t)'\0', trunc, (lchar_t *)NULL,
              (lchar_t)'\0'
           ); \
}

static inline
lchar_t *lusnncatc
( lchar_t *t, size_t tn,
  uchar_t *s, size_t sn,
  const lchar_t *trunc,
  const lchar_t lrep
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'l', 'u', '\0', '\0', 'c',
             t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', trunc, (lchar_t *)NULL,
             lrep
           ); \
}

static inline
uchar_t *ulsnncatc
( uchar_t *t, size_t tn,
  lchar_t *s, size_t sn,
  const lchar_t *trunc
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'u', 'l', '\0', '\0', 'c',
             (lchar_t *)t, tn, s, sn,
             (lchar_t)'\0', (lchar_t *)trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           ); \
}

static inline
uchar_t *uusnncatc
( uchar_t *t, size_t tn,
  uchar_t *s, size_t sn,
  const lchar_t *trunc
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'u', 'u', '\0', '\0', 'c',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', (lchar_t *)trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           ); \
}

// Concatenate uppercase.

static inline
lchar_t *llsnnCATC
( lchar_t *t, size_t tn,
  lchar_t *s, size_t sn,
  const lchar_t *trunc
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'l', 'l', '\0', '\0', 'C',
             t, tn, s, sn,
             (lchar_t)'\0', trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           ); \
}

static inline
lchar_t *lusnnCATC
( lchar_t *t, size_t tn,
  uchar_t *s, size_t sn,
  const lchar_t *trunc,
  const lchar_t lrep
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'l', 'u', '\0', '\0', 'C',
             t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', trunc, (lchar_t *)NULL,
             lrep
           ); \
}

static inline
uchar_t *ulsnnCATC
( uchar_t *t, size_t tn,
  lchar_t *s, size_t sn,
  const lchar_t *trunc
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'u', 'l', '\0', '\0', 'C',
             (lchar_t *)t, tn, s, sn,
             (lchar_t)'\0', (lchar_t *)trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           ); \
}

static inline
uchar_t *uusnnCATC
( uchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'u', 'u', '\0', '\0', 'C',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', (lchar_t *)trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           ); \
}

// Concatenate quoted case-preserving.

static inline
lchar_t *llsnncatq
( lchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  const lchar_t *trunc, lchar_t q
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'l', 'l', 'q', '\0', '\0',
             t, tn, s, sn,
             q, trunc, (lchar_t)NULL,
             (lchar_t)'\0'
           ); \
}

static inline
lchar_t *lusnncatq
( lchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc, lchar_t q,
  const lchar_t lrep
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'l', 'u', 'q', '\0', '\0',
             t, tn, (lchar_t *)s, sn,
             q, trunc, (lchar_t)NULL,
             lrep
            ); \
}

static inline
uchar_t *ulsnncatq
( uchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  const lchar_t *trunc, lchar_t q
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'u', 'l', 'q', '\0', '\0',
             (lchar_t *)t, tn, s, sn,
             q, (lchar_t *)trunc, (lchar_t)NULL,
             (lchar_t)'\0'
           ); \
}

static inline
uchar_t *uusnncatq
( uchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc, lchar_t q
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'u', 'u', 'q', '\0', '\0',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, (lchar_t *)trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

// Concatenate quoted lowercase.

static inline
lchar_t *llsnncatqc
( lchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  const lchar_t *trunc, lchar_t q
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'l', 'l', 'q', '\0', 'c',
             t, tn, s, sn,
             q, trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

static inline
lchar_t *lusnncatqc
( lchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc, lchar_t q,
  const lchar_t lrep
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'l', 'u', 'q', '\0', 'c',
             t, tn, (lchar_t *)s, sn,
             q, trunc, (lchar_t *)NULL,
             lrep
           ); \
}

static inline
uchar_t *ulsnncatqc
( uchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  const lchar_t *trunc, lchar_t q
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
            ( 0, 'u', 'l', 'q', '\0', 'c',
              (lchar_t *)t, tn, s, sn,
              q, (lchar_t *)trunc, (lchar_t *)NULL,
              (lchar_t)'\0'
            );
}

static inline
uchar_t *uusnncatqc
( uchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc, lchar_t q
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'u', 'u', 'q', '\0', 'c',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, (lchar_t *)trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

// Concatenate quoted uppercase.

static inline
lchar_t *llsnnCATQC
( lchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  const lchar_t *trunc, lchar_t q
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'l', 'l', 'Q', '\0', 'C',
             t, tn, s, sn,
             q, trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

static inline
lchar_t *lusnnCATQC
( lchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc, lchar_t q,
  const lchar_t lrep
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'l', 'u', 'Q', '\0', 'C',
             t, tn, (lchar_t *)s, sn,
             q, trunc, (lchar_t *)NULL,
             lrep
           );
}

static inline
uchar_t *ulsnnCATQC
( uchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  const lchar_t *trunc, lchar_t q
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'u', 'l', 'Q', '\0', 'C',
             (lchar_t *)t, tn, s, sn,
             q, (lchar_t *)trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

static inline
uchar_t *uusnnCATQC
( uchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc, lchar_t q
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'u', 'u', 'Q', '\0', 'C',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, (lchar_t *)trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

// Concatenate name case-preserving.

static inline
uchar_t *uusncatname
( uchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  lchar_t q, const lchar_t *trunc
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'u', 'u', 'q', 'N', '\0',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, (lchar_t *)trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

// Concatenate name lowercase.

static inline
uchar_t *llsncatnamec
( lchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  lchar_t q, const lchar_t *trunc,
  const lchar_t lrep
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'l', 'l', 'q', 'n', 'c',
             t, tn, s, sn,
             q, (lchar_t *)trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

static inline
lchar_t *lusncatnamec
( lchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  lchar_t q, const lchar_t *trunc,
  const lchar_t lrep
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'l', 'l', 'q', 'n', 'c',
             t, tn, (lchar_t *)s, sn,
             q, trunc, (lchar_t *)NULL,
             lrep
           );
}

static inline
uchar_t *ulsncatnamec
( uchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  lchar_t q, const uchar_t *trunc
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'l', 'l', 'q', 'n', 'c',
             (lchar_t *)t, tn, s, sn,
             q, (lchar_t *)trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

static inline
uchar_t *uusncatnamec
( uchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
    lchar_t q, const lchar_t *trunc
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'u', 'l', 'q', 'n', 'c',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, (lchar_t *)trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

// Concatenate name uppercase.

static inline
uchar_t *uusnCATNAMEC
( uchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  lchar_t q, const lchar_t *trunc
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 0, 'u', 'u', 'Q', 'N', 'C',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, (lchar_t *)trunc, (lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

// Concatenate hex digit characters <- bytes function helpers.

#if defined(LUB_DEFINITIONS)

static
lchar_t lub_to_hex_def
( const byte_t b, int shift, const uint8_t Case)
{ if (shift) { shift = 4; }
  const byte_t nib = (byte_t)((b >> shift) & 0xF);
  return nib < 10 ?
         (lchar_t)(nib + '0') :
         (lchar_t)(nib - 10 + (Case == 'c' ? 'a' : 'A'));
}

#endif // LUB_DEFINITIONS.

extern
lchar_t *lub_lbs_cat_ext
( lchar_t *t, size_t tn, 
  const byte_t *s, size_t sn,
  const lchar_t *trunc,
  const uint8_t Case
)
#if defined(LUB_DEFINITIONS)
{ if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0) || LUB_PTR_ERR(trunc, 0))
  { return (lchar_t *)LUB_PTR_ERR(LUB_PTR_INVALID, 0); }
  if (!t) { return (lchar_t *)NULL; }
  if (tn > LUB_MAX_LSTRLEN) { tn = LUB_MAX_LSTRLEN; }
  const size_t tl = lcsnlen(t, tn);
  if (LUB_SIZE_ERR(tl, 0)) { return (lchar_t *)LUB_PTR_ERR(tl, 0); }
  lchar_t *o = t + tl;
  size_t rem = (tl < tn) ? (tn - tl) : 0;
  int truncated = 0;
  if (sn > LUB_MAX_LSTRLEN >> 1) { sn = LUB_MAX_LSTRLEN >> 1; }
  if (sn > LUB_MAX_BSTRLEN) sn = LUB_MAX_BSTRLEN;
  for (; sn; sn--)
  { // Note: if s is NULL, treat as sn x'00' bytes.
    const byte_t c = s ? *s++ : (byte_t)0;
    if (rem < 2) { truncated = 1; break; }
    *o++ = lub_to_hex_def(c, 4, Case);
    *o++ = lub_to_hex_def(c, 0, Case);
    rem -= 2;
  }
  if (rem) *o = (lchar_t)0;
  else if (tn) t[tn - 1] = (lchar_t)0;
  return truncated ? (lchar_t *)LUB_PTR_ERR(LUB_TRUNCATED, 0) : o;
}
#else
;
#endif // LUB_DEFINITIONS.

extern
uchar_t *lub_ubs_cat_ext
( uchar_t *t, size_t tn,
  const byte_t *s, size_t sn,
  const lchar_t *trunc,
  const uint8_t Case
)
#if defined(LUB_DEFINITIONS)
{ if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0) || LUB_PTR_ERR(trunc, 0))
  { return (uchar_t *)LUB_PTR_ERR(LUB_PTR_INVALID, 0); }
  if (!t) { return (uchar_t *)NULL; }
  if (tn > LUB_MAX_USTRLEN) { tn = LUB_MAX_USTRLEN; }
  const size_t tl = ucsnlen(t, tn);
  if (LUB_SIZE_ERR(tl, 0)) { return (uchar_t *)LUB_PTR_ERR(tl, 0); }
  uchar_t *o = t + tl;
  size_t rem = (tl < tn) ? (tn - tl) : 0;
  int truncated = 0;
  if (sn > LUB_MAX_USTRLEN >> 1) { sn = LUB_MAX_USTRLEN >> 1; }
  if (sn > LUB_MAX_BSTRLEN) { sn = LUB_MAX_BSTRLEN; }
  for (; sn; sn--)
  { // Note: if s is NULL, treat as sn x'00' bytes.
    const byte_t c = s ? *s++ : (byte_t)0;
    if (rem < 2) { truncated = 1; break; }
    *o++ = (uchar_t)lub_to_hex_def(c, 4, Case);
    *o++ = (uchar_t)lub_to_hex_def(c, 0, Case);
    rem -= 2;
  }
  if (rem) { *o = (uchar_t)0; }
  else if (tn) { t[tn - 1] = (uchar_t)0; }
  return truncated ?
          (uchar_t *)LUB_PTR_ERR(LUB_TRUNCATED, 0) :
          o;
}
#else
;
#endif // LUB_DEFINITIONS.

// Concatenate hex digit characters (lowercase) <- bytes.

static inline
lchar_t *lbsnncatc
( lchar_t *t, size_t tn,
  const byte_t *s, size_t sn,
  const lchar_t *trunc
)
{ return lub_lbs_cat_ext(t, tn, s, sn, trunc, -1);}

static inline
uchar_t *ubsnncatc
( uchar_t *t, size_t tn,
  const byte_t *s, size_t sn,
  const lchar_t *trunc
)
{ return lub_ubs_cat_ext(t, tn, s, sn, trunc, -1);}

// Concatenate hex digit characters (uppercase) <- bytes.

static inline
lchar_t *lbsnnCATC
( lchar_t *t, size_t tn,
  const byte_t *s, size_t sn,
  const lchar_t *trunc
)
{ return lub_lbs_cat_ext(t, tn, s, sn, trunc, 1);}

static inline
uchar_t *ubsnnCATC
( uchar_t *t, size_t tn,
  const byte_t *s, size_t sn,
  const lchar_t *trunc
)
{ return lub_ubs_cat_ext(t, tn, s, sn, trunc, 1);}

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
 * @param tn For a character target, tn is the maximum number
 *           of characters to copy to buffer t, excluding the
 *           null terminator; tn is clamped to LUB_MAX_LSTRLEN
 *           or LUB_MAX_USTRLEN.
 * 
 *           For bbsnncpy, tn is the number of bytes to copy the target
 *           buffer. The target buffer must be at least tn bytes long.
 * @param s Pointer to source string.
 * @param sn For a character source string, sn is the maximum length of the
 *           string. sn is clamped to LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * 
 *           For uusncpyname, uusncpynamec, and uusnCPYNAMEC, sn is omitted
 *           since the source must be a valid name and
 *           thus sn defaults to LUB_MAX_NAMELEN (128 characters).
 * 
 *           For a copy of a byte string, sn is the length of the source
 *           string. Error if sn is greater than LUB_MAX_BSTRLEN.
 * @param q Quote character for quoted string and quoted name copy functions.
 * @param trunc Pointer to a string that specifies how to handle
 *              truncation (optional alphabetic truncate mode
 *              character and truncated replacement string).
 *              See parameter trunc in @ref CommonParameters
 *              "Common Parameters" for details.
 * @param  lrep  Replacement for out-of-range Unicode characters
 *                (lus functions only).
 * @return t, or NULL if an error occurs.
 * 
 *         If t is not null and an error occurs, the target buffer
 *         is set to an empty string for a character target or filled
 *         with sn x'00' bytes for a byte target (truncated if sn > tn
 *         or padded with x'00' bytes to a length of tn).
 *
 * @note Errors:
 * - (lchar_t/uchar_t *)LUB_UNTERMINATED if is not null terminated.
 * - (lchar_t/uchar_t *)LUB_BSTR_TOO_LONG if, for a byte string,
 *   tn or sn exceeds LUB_MAX_BSTRLEN.
 * - (lchar_t/uchar_t *)LUB_OVERLAP if source and target have
 *   different types, or when quoted.
 * - (lchar_t/uchar_t *)LUB_TRUNCATED if length of trunc
 *   string exceeds 31 or tn if trunc is not NULL.
 *
 * @note Copy is overlap-safe if target and source have the
 *       same type and not quoted (target result is correct
 *       but source may be overridden if there is overlap).
 *
 *       For copy with target and source of different types
 *       or quoted, overlap is not allowed.
 *
 * @note For concatenate with a byte target and a character source:
 *       - If the source string contains an odd number of hex digits, the last
 *         hex digit is treated as the high nibble of a byte, with an implicit
 *         low nibble of zero.
 *       - If the number of bytes copied into the target
 *         buffer is less than tn,
 *         the target buffer is padded with x'00' bytes to a length of tn.
 * @{
 */

// Copy case-preserving.

static inline
lchar_t *llsnncpy
( lchar_t *t, size_t tn, const lchar_t *s, size_t sn,
  const lchar_t *trunc
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
          ( 1, 'l', 'l', '\0', '\0', '\0',
            (lchar_t *)t, tn, (lchar_t *)s, sn,
            (lchar_t)'\0', trunc, (const lchar_t *)NULL,
            (lchar_t)'\0'
          );
}

static inline
lchar_t *lusnncpy
( lchar_t *t, size_t tn, uchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t lrep
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'l', 'u', '\0', '\0', '\0',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', trunc, (const lchar_t *)NULL,
             lrep
           );
}

static inline
uchar_t *ulsnncpy
( uchar_t *t, size_t tn, lchar_t *s, size_t sn,
  const lchar_t *trunc
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'u', 'l', '\0', '\0', '\0',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

static inline
uchar_t *uusnncpy
( uchar_t *t, size_t tn, uchar_t *s, size_t sn,
  const lchar_t *trunc
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'u', 'u', '\0', '\0', '\0',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

// Copy lowercase.

static inline
lchar_t *llsnncpyc
( lchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  const lchar_t *trunc
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'l', 'l', '\0', '\0', 'c',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

static inline
lchar_t *lusnncpyc
( lchar_t *t, size_t tn,
  uchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t lrep
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'l', 'u', '\0', '\0', 'c',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', trunc, (const lchar_t *)NULL,
             lrep
           );
}

static inline
uchar_t *ulsnncpyc
( uchar_t *t, size_t tn,
  lchar_t *s, size_t sn,
  const lchar_t *trunc)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'u', 'l', '\0', '\0', 'c',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

static inline
uchar_t *uusnncpyc
( uchar_t *t, size_t tn,
  uchar_t *s, size_t sn,
  const lchar_t *trunc)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'u', 'u', '\0', '\0', 'c',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

// Copy uppercase.

static inline
lchar_t *llsnnCPYC
(lchar_t *t, size_t tn,
                                 const lchar_t *s, size_t sn,
                                 const lchar_t *trunc)
{ return (lchar_t *)lub_cat_cpy_pad_ext
         ( 1, 'l', 'l', '\0', '\0', 'C',
           (lchar_t *)t, tn, (lchar_t *)s, sn,
           (lchar_t)'\0', trunc, (const lchar_t *)NULL,
           (lchar_t)'\0'
         );
}

static inline
lchar_t *lusnnCPYC
( lchar_t *t, size_t tn, uchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t lrep
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'l', 'u', '\0', '\0', 'C',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', trunc, (const lchar_t *)NULL,
             lrep
           );
}

static inline
uchar_t *ulsnnCPYC
( uchar_t *t, size_t tn, uchar_t *s, size_t sn,
  const lchar_t *trunc
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'u', 'l', '\0', '\0', 'C',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

static inline
uchar_t *uusnnCPYC
( uchar_t *t, size_t tn, uchar_t *s, size_t sn,
  const lchar_t *trunc
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'u', 'u', '\0', '\0', 'C',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

// Copy quoted case-preserving.

static inline
lchar_t *llsnncpyq
( lchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t q
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'l', 'l', q, '\0', '\0',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

static inline
lchar_t *lusnncpyq
( lchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t q,
  const lchar_t lrep
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'l', 'u', q, '\0', '\0',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, trunc, (const lchar_t *)NULL,
             lrep
           );
}

static inline
uchar_t *ulsnncpyq
( uchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t q
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'u', 'l', q, '\0', '\0',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
               q, trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

static inline
uchar_t *uusnncpyq
( uchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t q
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'u', 'u', q, '\0', '\0',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

// Copy quoted lowercase.

static inline
lchar_t *llsnncpyqc
( lchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t q
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'l', 'l', q, '\0', 'c',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

static inline
lchar_t *lusnncpyqc
( lchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t q,
  const lchar_t lrep
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'l', 'u', q, '\0', 'c',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, trunc, (const lchar_t *)NULL,
             lrep
           );
}

static inline
uchar_t *ulsnncpyqc
( uchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t q
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'u', 'l', q, '\0', 'c',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

static inline
uchar_t *uusnncpyqc
( uchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t q
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'u', 'u', q, '\0', 'c',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

// Copy quoted uppercase.

static inline
lchar_t *llsnnCPYQC
( lchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t q
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'l', 'l', q, '\0', 'C',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

static inline
lchar_t *lusnnCPYQC
( lchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t q,
  const lchar_t lrep
)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'l', 'u', q, '\0', 'C',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, trunc, (const lchar_t *)NULL,
             lrep
           );
}

static inline
uchar_t *ulsnnCPYQC
( uchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t q
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'u', 'l', q, '\0', 'C',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

static inline
uchar_t *uusnnCPYQC
( uchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t q
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'u', 'u', q, '\0', 'C',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

// Copy quoted/unquoted name case-preserving.

static inline
uchar_t *uusncpyname
( uchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t q
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'u', 'u', q, 'N', '\0',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

// Copy quoted/unquoted name lowercase.

static inline
uchar_t *uusncpynamec
( uchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t q
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'u', 'u', q, 'N', 'c',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

// Copy quoted/unquoted name uppercase.

static inline
uchar_t *uusnCPYNAMEC
( uchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc, const lchar_t q
)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 1, 'u', 'u', q, 'N', 'C',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             q, trunc, (const lchar_t *)NULL,
             (lchar_t)'\0'
           );
}

// Copy hex digit characters lowercase <- bytes.

static inline
lchar_t *lbsnncpyc
( lchar_t *t, size_t tn,
  const byte_t *s, size_t sn,
  const lchar_t *trunc
)
{if (!t) return (lchar_t *)NULL;
     if (LUB_PTR_ERR(t, 0)) return (lchar_t *)LUB_PTR_ERR(LUB_PTR_INVALID, 0);
     *t = (lchar_t)0;
     return lbsnncatc(t, tn, s, sn, trunc) ? t : (lchar_t *)NULL;}

static inline
uchar_t *ubsnncpyc
( uchar_t *t, size_t tn,
  const byte_t *s, size_t sn,
  const lchar_t *trunc
)
{ if (!t) { return ( uchar_t *)NULL; }
  if (LUB_PTR_ERR(t, 0))
  { return (uchar_t *)LUB_PTR_ERR(LUB_PTR_INVALID, 0); }
  *t = (uchar_t)0;
  return ubsnncatc(t, tn, s, sn, trunc) ?
         t : (uchar_t *)NULL;
}

// Copy hex digit characters uppercase <- bytes.

static inline
lchar_t *lbsnnCPYC
( lchar_t *t, size_t tn,
  const byte_t *s, size_t sn,
  const lchar_t *trunc
)
{ if (!t) { return (lchar_t *)NULL; }
  if (LUB_PTR_ERR(t, 0))
  { return (lchar_t *)LUB_PTR_ERR(LUB_PTR_INVALID, 0); } 
  *t = (lchar_t)0;
  return lbsnnCATC(t, tn, s, sn, trunc) ?
         t : (lchar_t *)NULL;
}

static inline
uchar_t *ubsnnCPYC
( uchar_t *t, size_t tn,
  const byte_t *s, size_t sn,
  const lchar_t *trunc
)
{ if (!t) { return (uchar_t *)NULL; }
  if (LUB_PTR_ERR(t, 0))
  { return (uchar_t *)LUB_PTR_ERR(LUB_PTR_INVALID, 0); }
  *t = (uchar_t)0;
  return ubsnnCATC(t, tn, s, sn, trunc) ? t : (uchar_t *)NULL;
}

// Copy bytes <- hex digit characters.

extern
byte_t *blsnncpy
( byte_t *t, size_t tn, const lchar_t *s, size_t sn)
#if defined(LUB_DEFINITIONS)
{ if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0))
  { return (byte_t *)LUB_PTR_ERR(LUB_PTR_INVALID, 0); }
  if (!t || !s) { return t; }
  if (sn > LUB_MAX_LSTRLEN) { sn = LUB_MAX_LSTRLEN; }

  int hi, lo;
  for (; sn && *s; sn--, s++)
  { hi = ilhex(*s);
    if (sn && s[1]) { lo = ilhex(*++s); sn--; } else { lo = 0; }
    if (hi < 0 || lo < 0) // Invalid hex digit.
    { return (byte_t *)NULL; } 
    *t++ = (unsigned char)((hi << 4) | lo);
  }

  if (*s) // Missing null terminator for source string.
  { return (byte_t *)NULL; }

  return t;
}
#else
;
#endif // LUB_DEFINITIONS

extern
byte_t *busnncpy
( byte_t *t, size_t tn, const uchar_t *s, size_t sn )
#if defined(LUB_DEFINITIONS)
{ if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0))
  { return (byte_t *)LUB_PTR_ERR(LUB_PTR_INVALID, 0); }
  if (!t || !s) { return t; }
  if (sn > LUB_MAX_USTRLEN) { sn = LUB_MAX_USTRLEN; }

  int hi, lo;
  for (; sn && *s; sn--, s++)
  { hi = iuhex(*s);
    lo = 0;
    if (sn && s[1]) { lo = iuhex(*++s); sn--; } else { lo = 0; }
    if (hi < 0 || lo < 0) // Invalid hex digit.
    { return (byte_t *)NULL; }
    *t++ = (unsigned char)((hi << 4) | lo);
  }

  if (*s) // Missing null terminator for source string.
  { return (byte_t *)NULL; }

  return t;
}
#else
;
#endif // LUB_DEFINITIONS

// Copy bytes <- bytes.

extern
byte_t *bbsnncpy
( byte_t *t, size_t tn, const byte_t *s, size_t sn)
#if defined(LUB_DEFINITIONS)
{ if (!t) { return (byte_t *)NULL; }
  if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0))
  { return (byte_t *)LUB_PTR_ERR(LUB_PTR_INVALID, 0); }
  if (sn > LUB_MAX_BSTRLEN) { return (byte_t *)NULL; }

  if (!s)
  {  // Copy sn null bytes if s is NULL.
    for (; sn; sn--) *t++ = (byte_t)0;
  }
  else if (t <= s || t >= s + sn)
  { // Safe to copy sn bytes left-to-right, even with overlap.
    for (; sn; sn--) *t++ = *s++;
  }
  else
  { // Avoid potential overlap with copy sn bytes right-to-left.
    byte_t *tt = (t += sn) - 1;
    s += sn - 1;
    for (; sn; sn--) *tt-- = *s--;
  }

  return t;
}
#else
;
#endif // LUB_DEFINITIONS

/** @} */

/**
 * @defgroup Pad Pad
 * @name llsnnpad, lusnnpad, ulsnnpad, uusnnpad
 * @brief Pad source string to exactly tn characters using left, center, or
 *        right alignment (bounded).
 * @param t Pointer to target buffer.
 * @param tn Maximum buffer length (excluding null terminator).
 * @param s Pointer to source string.
 * @param sn Bound on source string. Clamped to LUB_MAX_LSTRLEN
 *           or LUB_MAX_USTRLEN.
 * @param trunc Pointer to a string that specifies how to handle
 *              truncation (optional alphabetic truncate mode
 *              character and truncated replacement string).
 *              See parameter trunc in @ref CommonParameters
 *              "Common Parameters" for details.
 * @param pad Pointer to a pad indicator string (optional
 *            alphabetic pad mode character and pad character):
 * 
 *            - Maximum length of pad is 2.
 * 
 *            - If NULL, pad indicator string defaults to "B ".
 * 
 *            - If the first character is Latin alphabetic, it specifies
 *              the pad mode and the second character, if any, is the pad character.
 * 
 *            - If the first character is not Latin alphabetic, pad mode defaults
 *              to 'B' and the first character, if any, is the pad character.
 * 
 *            Pad mode character (explicit or by default):
 * 
 *             - 'L' or 'l' pad on left (for right-aligned text).
 * 
 *             - 'R' or 'r' pad on right (for left-aligned text).
 * 
 *             - 'B' or 'b' pad on left and right (for center-aligned text).
 *                          For odd-width padding, the extra pad character
 *                          is added on the right.
 * 
 *             - Other alphabetic characters are reserved for future use
 *               and an error occurs if used.
 *
 *             Pad character (explicit or by default): 
 * 
 *               Character to pad from the beginning
 *               and/or end of the source string.
 * 
 *               If no pad character specified, defaults to Latin ' ' (space).
 * @param lrep For lus* functions only,
 *             - if NULL ('\0') character, return an error for
 *               a Unicode source containing a character that is
 *               out-of-range for Latin.
 *             - Otherwise, use lrep as the replacement Latin character
 *               for out-of-range Unicode source characters.
 * @return t, NULL (if t is NULL), error. For an error,
 *         *t is set to a null-terminator.
 *
 * @note Errors:
 * - (lchar_t/uchar_t *)LUB_PTR_INVALID if t or s is not a valid pointer.
 * - (lchar_t/uchar_t *)LUB_UNTERMINATED if source string
 *   is not null-terminated.
 * - (lchar_t/uchar_t *)LUB_OPT_TOO_LONG if trunc or pad string
 *   exceeds maximum length of LUB_MAX_LOPTLEN or LUB_MAX_UOPTLEN.
 * - (lchar_t/uchar_t *)LUB_OPT_RESERVED if trunc or pad
 *   uses a reserved trunc mode or pad mode, respectively.
 * - (lchar_t/uchar_t *)LUB_OPT_INVALID if trunc or pad is invalid.
 * - (lchar_t/uchar_t *)LUB_TRUNCATED if source string is
 *   longer than tn.
 * - (lchar_t/uchar_t *)LUB_NONLATIN_SOURCE if a character
 *   in a Unicode source for a Latin target is out-of-range
 *   for Latin when lrep is null.
 * 
 * @note If the source and target buffer overlap, the target
 *       buffer is set correctly to the padded source string
 *       (the source string may be overwritten during the operation).
 * @{
 */

extern
lchar_t *llsnnpad
( lchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
   const lchar_t *pad
)
#if defined(LUB_DEFINITIONS)
{ return (lchar_t *)lub_cat_cpy_pad_ext
       ( 2, 'l', 'l', '\0', '\0', '\0',
       t, tn, (lchar_t *)s, sn,
       (lchar_t)'\0', (const lchar_t *)NULL, pad,
       (lchar_t)'\0'
       );
}
#else
;
#endif // LUB_DEFINITIONS for llsnnpad.

extern
lchar_t *lusnnpad
( lchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *pad,
  const lchar_t lrep
)
#if defined(LUB_DEFINITIONS)
{ return (lchar_t *)lub_cat_cpy_pad_ext
           ( 2, 'l', 'u', '\0', '\0', '\0',
             t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', (const lchar_t *)NULL, pad,
             lrep
           );
}
#else
;
#endif // LUB_DEFINITIONS for lusnnpad.

extern
uchar_t *ulsnnpad
( uchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
   const uchar_t *pad
)
#if defined(LUB_DEFINITIONS)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 2, 'u', 'l', '\0', '\0', '\0',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', (const lchar_t *)NULL, (const lchar_t *)pad,
             (lchar_t)'\0'
           );
}
#else
;
#endif // LUB_DEFINITIONS for ulsnnpad.

extern
uchar_t *uusnnpad
( uchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const uchar_t *pad
)
#if defined(LUB_DEFINITIONS)
{ return (uchar_t *)lub_cat_cpy_pad_ext
           ( 2, 'u', 'u', '\0', '\0', '\0',
             (lchar_t *)t, tn, (lchar_t *)s, sn,
             (lchar_t)'\0', (const lchar_t *)NULL, (const lchar_t *)pad,
             (lchar_t)'\0'
           );
}
#else
;
#endif // LUB_DEFINITIONS for uusnnpad.

/** @} */

/**
 * @defgroup Trim Trim
 * @name llsnntrim, ulsnntrim, uusnntrim (case-preserving)
 *       llsnntrimc, ulsnntrimc, uusnntrimc (lowercase)
 *       llsnntrimC, ulsnntrimC, uusnntrimC (uppercase)
 * @brief Trim leading and/or trailing characters that match
 *        one of the specified characters
 *        and optionally collapse embedded specified characters.
 * @param t Target buffer for the trimmed string.
 * @param tn Bound for the target buffer. Clamped to LUB_MAX_LSTRLEN or
 *           LUB_MAX_USTRLEN.
 * @param s Source string to trim.
 * @param sn Bound for the source string (clamped to LUB_MAX_LSTRLEN or
 *           LUB_MAX_USTRLEN).
 * @param trunc Pointer to a string that specifies how to handle
 *              truncation (optional alphabetic truncate mode
 *              character and truncated replacement string).
 *              See parameter trunc in @ref CommonParameters
 *              "Common Parameters" for details.
 * @param trim Pointer to a source trim indicator string (optional
 *             alphabetic trim mode character and trim characters).
 *             See parameter trim in @ref CommonParameters
 *             "Common Parameters" for details.
 * @param delim Delimiter character for collapsing embedded trim characters.
 *              A null character indicates embedded trim characters
 *              in the source string are not collapsed and trim mode must 
 *              be 'B', 'b', or default to 'B'.
 * @param lrep For lus* functions only,
 *             - if NULL ('\0') character, return an error for
 *               a Unicode source containing a character that is
 *               out-of-range for Latin.
 *             - Otherwise, use lrep as the replacement Latin character
 *               for out-of-range Unicode source characters.
 * @return Pointer to t, or NULL if an error occurs,
 *
 * @note Errors:
 * - LUB_PTR_INVALID if t, s, st, or trim is not a valid pointer (value
 *   is reserved as an error value).
 * - LUB_UNTERMINATED if s, trunc, or trim is not null terminated.
 * - LUB_OPT_INVALID if invalid trunc or trim value.
 * - LUB_TRUNCATED if result is truncated.
 * - (lchar_t/uchar_t *)LUB_NONLATIN_SOURCE if a character
 *   in a Unicode source for a Latin target is out-of-range
 *   for Latin when lrep is null.
 * @{
 */

#if defined(LUB_DEFINITIONS)

static inline
int lub_trim_match_def
( const uchar_t c,
  const char trim_xt,
  const lchar_t *trimset
)
{ if (!trimset) { return isuspace((int)c); }

  if (trim_xt == 'u')
  { const uchar_t *scan = (const uchar_t *)trimset;
    while (*scan)
    { if (*scan++ == c) { return 1; } }
  }
  else // trim_xt == 'l'
  { while (*trimset)
    { if ((uchar_t)*trimset++ == c) { return 1;} }
  }
  return 0;
}

extern
lchar_t *lub_trim_ext
( const char xt,
  const char xs,
  const char trim_xt,
  lchar_t *t,
  size_t tn,
  const lchar_t *s,
  size_t sn,
  const lchar_t *trunc,
  const lchar_t *trim,
  const lchar_t delim,
  const lchar_t lrep
)
{ if (!t) return (lchar_t *)NULL;

  if (LUB_PTR_ERR(t, 0))
  { return (lchar_t *)LUB_PTR_ERR(LUB_PTR_INVALID, 0); }

  lub_terminate_def(xt, t);

  if (LUB_PTR_ERR(s, 0) || LUB_PTR_ERR(trunc, 0) || LUB_PTR_ERR(trim, 0))
  { return (lchar_t *)LUB_PTR_ERR(LUB_PTR_INVALID, 0); }
  if (!s) return (lchar_t *)NULL;

  if (xt == 'u') {
    if (tn > LUB_MAX_USTRLEN) tn = LUB_MAX_USTRLEN;
  } else if (tn > LUB_MAX_LSTRLEN) {
    tn = LUB_MAX_LSTRLEN;
  }

  size_t trimlen = 0;
  const lchar_t *trimmed =
           lub_pointer_trim_ext
             (xs, s, sn, trim_xt, trim, &trimlen);

  if (LUB_PTR_ERR(trimmed, 0)) { return (lchar_t *)trimmed; }
  if (!trimmed) { return (lchar_t *)NULL; }

  const size_t t_size = (xt == 'u') ? sizeof(uchar_t) : sizeof(lchar_t);
  const size_t s_size = (xs == 'u') ? sizeof(uchar_t) : sizeof(lchar_t);
  const char *tb = (const char *)t;
  const char *sb = (const char *)trimmed;
  const size_t t_bytes = (tn + 1) * t_size;
  const size_t s_bytes = (trimlen + 1) * s_size;

  if (tb < sb + s_bytes && sb < tb + t_bytes)
  { return (lchar_t *)LUB_PTR_ERR(LUB_OVERLAP, 0); }
  
  const lchar_t *trimset = (const lchar_t *)NULL;
  if (trim)
  { if (trim_xt == 'u')
    { const uchar_t *utrim = (const uchar_t *)trim;
      if (*utrim && islalpha((int)*utrim)) ++utrim;
      trimset = *utrim ? (const lchar_t *)utrim : (const lchar_t *)NULL;
    }
    else
    { const lchar_t *ltrim = trim;
      if (*ltrim && islalpha((int)*ltrim)) ++ltrim;
      trimset = *ltrim ? ltrim : (const lchar_t *)NULL;
    }
  }

  size_t normalized_len = 0;
  uchar_t normalized[(trimlen ? trimlen : 1) + 1];
  int pending_trim = 0;

  for (size_t i = 0; i < trimlen; ++i)
  { const uchar_t c = lub_get_ith_def(xs, trimmed, i);
    const int is_trim = delim && lub_trim_match_def(c, trim_xt, trimset);

    if (is_trim) { pending_trim = 1; continue; }

    if (pending_trim && normalized_len)
    { normalized[normalized_len++] = (uchar_t)delim; }

    if (xt == 'l' && c > LUB_MAX_LCHAR && !lrep)
    { lub_terminate_def(xt, t);
      return (lchar_t *)LUB_PTR_ERR(LUB_NONLATIN_SOURCE, 0);
    }

    normalized[normalized_len++] = c;
    pending_trim = 0;
  }

  normalized[normalized_len] = (uchar_t)0;

  if (normalized_len <= tn)
  { for (size_t i = 0; i < normalized_len; ++i)
    { lub_set_ith_def(xt, t, i, normalized[i], lrep); }
    lub_terminate_ith_def(xt, t, normalized_len);
    return t;
  }

  char trunc_mode = 'R';
  const lchar_t *replacement = trunc;
  size_t replacement_len = 0;

  if (trunc)
  { replacement_len = lcsnlen(trunc, LUB_MAX_LOPTLEN);
    if (LUB_SIZE_ERR(replacement_len, 0))
    { return (lchar_t *)LUB_PTR_ERR(replacement_len, 0); }
    if (*replacement && islalpha((int)*replacement))
    { trunc_mode = (char)toupper((int)*replacement++);
      --replacement_len;
    }
  }

  size_t keep_left = 0;
  size_t keep_right = 0;
  if (replacement_len && tn > 2 * replacement_len)
  { const size_t keep_total = tn - 2 * replacement_len;
    if (trunc_mode == 'L')
    { keep_right = keep_total; }
    else if (trunc_mode == 'C')
    { keep_left = (keep_total + 1) / 2;
      keep_right = keep_total / 2;
    }
    else if (trunc_mode == 'B')
    { keep_left = 0;
      keep_right = 0;
    }
    else
    { keep_left = keep_total; }
  }

  size_t out = 0;
  for (size_t i = 0; i < keep_left && out < tn; ++i)
  { lub_set_ith_def(xt, t, out++, normalized[i], lrep); }
  for (size_t i = 0; i < replacement_len && out < tn; ++i)
  { lub_set_ith_def(xt, t, out++, (uchar_t)replacement[i], lrep); }
  if (keep_right)
  { const size_t start = normalized_len - keep_right;
    for (size_t i = start; i < normalized_len && out < tn; ++i)
    { lub_set_ith_def(xt, t, out++, normalized[i], lrep); }
  }
  lub_terminate_ith_def(xt, t, out);

  return (lchar_t *)LUB_PTR_ERR(LUB_TRUNCATED, 0);
}

#endif // LUB_DEFINITIONS for trim mode macros.

// Trim (case-preserving).
static inline
lchar_t *llsnntrim
( lchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  const lchar_t *trunc,
  const lchar_t *trim,
  const lchar_t delim
)
{ return lub_trim_ext
           ('l', 'l', 'l',
             t, tn, s, sn,
             trunc, trim,
             delim, (lchar_t)'\0');
}

static inline
lchar_t *lusnntrim
( lchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc,
  const lchar_t *trim,
  const lchar_t delim,
  const lchar_t lrep
)
{ return lub_trim_ext
           ('l', 'u', 'l',
             t, tn, (const lchar_t *)s, sn,
             trunc, trim,
             delim, lrep);
}

static inline
uchar_t *ulsnntrim
( uchar_t *t, size_t tn,
  const lchar_t *s, size_t sn,
  const lchar_t *trunc,
  const lchar_t *trim,
  const uchar_t delim)
{ return (uchar_t *)lub_trim_ext
           ('u', 'l', 'l',
            (lchar_t *)t, tn, s, sn,
            trunc, trim,
            (lchar_t)delim, (lchar_t)'\0');
}

static inline
uchar_t *uusnntrim
( uchar_t *t, size_t tn,
  const uchar_t *s, size_t sn,
  const lchar_t *trunc,
  const uchar_t *trim,
  const uchar_t delim
)
{ return (uchar_t *)lub_trim_ext
           ('u', 'u', 'u', (lchar_t *)t, tn,
            (const lchar_t *)s, sn,
             trunc, (const lchar_t *)trim,
             (lchar_t)delim, (lchar_t)'\0');
}

/** @} */

/**
 * @defgroup Reverse Reverse
 * @name llsnnreverse, lusnnreverse, ulsnnreverse, uusnnreverse
 * @brief Reverse of the source string.
 * @param t Pointer to target buffer.
 * @param tn Target buffer character size (excluding null-terminator).
 * @param s Pointer to source string.
 * @param sn Bound for the source string. Clamped to LUB_MAX_LSTRLEN or
 *           LUB_MAX_USTRLEN.
 * @param trunc Pointer to a string that specifies how to handle
 *              truncation (optional alphabetic truncate mode
 *              character and truncated replacement string).
 *              See parameter trunc in @ref CommonParameters
 *              "Common Parameters" for details.
 * @return t, NULL if t is NULL, or an error.
 *
 * @note Errors: 
 * - LUB_PTR_INVALID if t or s is not a valid pointer.
 * - LUB_UNTERMINATED if source string is not NULL terminated.
 * - LUB_TRUNCATED if the target buffer is too small.
 * 
 * @note If the source and target buffer overlap, the target
 *       buffer is set correctly to the reverse of the source
 *       string (the source string may be overwritten during
 *       the operation).
 * @{
 */
 
#if defined(LUB_DEFINITIONS)

// Reverse helper macro.

#undef LUB_OP_DEF
#define LUB_OP_DEF(t_xt, tn_max, \
                   s_xt, sn_max, xcsnlen, \
                   lrep) \
{ if (LUB_PTR_ERR(t, 0)) \
  { return (t_xt *)LUB_PTR_ERR(LUB_PTR_INVALID, 0); } \
  if (tn > tn_max) { tn = tn_max; } \
  if (sn > sn_max) { sn = sn_max; } \
  sn = xcsnlen(s, sn); \
  if (LUB_SIZE_ERR(sn, 0)) \
  { if (t) { *t = (t_xt)0; } \
    return (t_xt *)LUB_PTR_ERR(sn, 0); \
  } \
  if (sn > tn) \
  { if (t) { *t = (t_xt)0; } \
    return (t_xt *)LUB_PTR_ERR(LUB_TRUNCATED, 0); \
  } \
  if (!t) { return (t_xt *)NULL; } \
  t_xt *tt = t; \
  size_t i = sn; \
  if (!sn) \
  { *t = (t_xt)0; \
    return t; \
  } \
  if (t + sn <= (t_xt *)s || t >= (t_xt *)(s + sn)) \
  { /* No overlap: simple reverse copy. */ \
    s += (sn - 1); \
    if (sizeof(t_xt) < sizeof(s_xt)) \
    { for (; i; --i, ++tt, --s) \
      { if (*s > LUB_MAX_LCHAR) \
        { if (lrep) \
          { *tt = lrep; } \
            else \
          { return (t_xt *)LUB_PTR_ERR(LUB_NON_LATIN_CHAR, 0); } \
        } \
        else { *tt = (t_xt)*s; } \
      } \
    } \
    else \
      for (; i; --i, ++tt, --s) *tt = (t_xt)*s; \
    *tt = (t_xt)0; \
    return t; \
  } \
  /* Handle overlap. */ \
  /* Copy source to target with overlap-safe logic. */ \
  if (t <= (t_xt *)s) \
  { /* Safe to copy left to right. */ \
    if (t < (t_xt *)s && sizeof(t_xt) == sizeof(s_xt)) \
    { for (; i; --i, ++tt, ++s) *tt = *s; \
    } \
    else if (sizeof(t_xt) < sizeof(s_xt)) \
    { /* Copy Unicode cast as Latin to target buffer left to right. */ \
      for (; i; --i, ++tt, ++s) \
      { if (*s > LUB_MAX_LCHAR) \
        { if (lrep) { *tt = (t_xt)lrep; } \
          else return (t_xt *)LUB_PTR_ERR(LUB_NON_LATIN_CHAR, 0); \
        } \
        else \
        { *tt = (t_xt)*s; } \
      } \
    } \
    else \
    { /* Copy Latin characters to target buffer left to right. */ \
      s_xt *ss = (s_xt *)t; \
      for (; i; --i, ++ss, ++s) { *ss = *s; } \
    } \
  } else \
  { /* Safe to copy right to left. */ \
    tt += sn - 1; \
    s += sn - 1; \
    if (sizeof(t_xt) == sizeof(s_xt)) \
    { for (; i;  --i, --tt, --s) *tt = *s; } \
    else if (sizeof(t_xt) < sizeof(s_xt)) \
    { /* Copy Unicode cast as Latin to target buffer right to left. */ \
      for (; i; --i, --tt, --s) \
      { if (*s > LUB_MAX_LCHAR) \
          if (lrep) *tt = (t_xt)lrep; \
          else return (t_xt *)LUB_PTR_ERR(LUB_NON_LATIN_CHAR, 0); \
        else *tt = (t_xt)*s; \
      } \
    } \
    else \
    { /* Copy Latin characters to target buffer right to left. */ \
      s_xt *ss = ((s_xt *)t) + sn - 1; \
      for (; i; --i, --ss, --s) { *ss = *s; } \
    } \
  } \
  if (sizeof(t_xt) > sizeof(s_xt)) \
  { /* Expand Latin characters to Unicode right to left. */ \
    tt = t + sn - 1; \
    s = (s_xt *)t + sn - 1; \
    i = sn; \
    /* Expand Latin characters to Unicode right to left. */ \
    for (; i; --i, --tt, --s) { *tt = (t_xt)*s; } \
  } \
  /* In-place reversal */ \
  tt = t + sn - 1; \
  t_xt *tr = t; \
  for (; tt > tr; --tt, ++tr) \
  { t_xt tmp = *tr; \
    *tr = *tt; \
    *tt = tmp; \
  } \
  t[sn] = (t_xt)0; \
  return t; \
}
#endif // LUB_DEFINITIONS for reverse macro helper.

// Reverse.

extern
lchar_t *llsnnreverse
( lchar_t *t, size_t tn,
  lchar_t *s, size_t sn
)
#if defined(LUB_DEFINITIONS)
LUB_OP_DEF(lchar_t, LUB_MAX_LSTRLEN,
           lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
           ((lchar_t)'\0'))
#else
;
#endif // LUB_DEFINITIONS for llsnnreverse.

extern
lchar_t *lusnnreverse
( lchar_t *t, size_t tn,
  uchar_t *s, size_t sn,
  const lchar_t lrep
)
#if defined(LUB_DEFINITIONS)
LUB_OP_DEF(lchar_t, LUB_MAX_LSTRLEN,
           uchar_t, LUB_MAX_USTRLEN, ucsnlen,
           lrep)
#else
    ;
#endif // LUB_DEFINITIONS for lusnnreverse.

extern
uchar_t *ulsnnreverse
( uchar_t *t, size_t tn,
  const lchar_t *s, size_t sn
)
#if defined(LUB_DEFINITIONS)
LUB_OP_DEF(uchar_t, LUB_MAX_USTRLEN,
           lchar_t, LUB_MAX_LSTRLEN, lcsnlen,
           ((lchar_t)'\0'))
#else
    ;
#endif // LUB_DEFINITIONS for ulsnnreverse.

extern
uchar_t *uusnnreverse
( uchar_t *t, size_t tn,
  const uchar_t *s, size_t sn
)
#if defined(LUB_DEFINITIONS)
LUB_OP_DEF(uchar_t, LUB_MAX_USTRLEN,
           uchar_t, LUB_MAX_USTRLEN, ucsnlen,
           ((lchar_t)'\0'))
#else
;
#endif // LUB_DEFINITIONS for uusnnreverse.

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
 * @param sn Bound on source string. Clamped to
 *           LUB_MAX_LSTRLEN or LUB_MAX_USTRLEN.
 * @param times Number of times to repeat source. 0 yields empty string.
 * @return t, null (if t is NULL), or error.
 * 
 *         For an error, if t, *t is set to a null-terminator.
 *
 * @note Errors:
 * - LUB_PTR_INVALID if s is an invalid pointer.
 * - LUB_UNTERMINATED if s is not null-terminated.
 * - LUB_OVERLAP if target and source buffers overlap in a way
 *   that could cause incorrect results in the target buffer.
 * - LUB_TRUNCATED if the result cannot fit in the target buffer.
 * @{
 */

extern
lchar_t *llsnnrepeat
( lchar_t *t, size_t tn, const lchar_t *s, size_t sn, size_t times)
#if defined(LUB_DEFINITIONS)
{ if (!t || !s) { return (lchar_t *)NULL; }
  if (tn > LUB_MAX_LSTRLEN) { tn = LUB_MAX_LSTRLEN; }
  if (sn > LUB_MAX_LSTRLEN) { sn = LUB_MAX_LSTRLEN; }

  size_t s_len = lcsnlen(s, sn);
  if (LUB_SIZE_ERR(s_len, 0))
  { t[tn] = (lchar_t)0; return (lchar_t *)s_len; }

  if (times == 0) { t[0] = (lchar_t)0;  return t; }

  if (s_len && times > tn / s_len)
  { t[tn] = (lchar_t)0; return (lchar_t *)NULL; }

  size_t needed = s_len * times;
  if (needed > tn)
  { t[tn] = (lchar_t)0; return (lchar_t *)NULL; }

  for (size_t rep = 0; rep < times; ++rep)
  { for (size_t i = 0; i < s_len; ++i)
    { t[rep * s_len + i] = s[i]; }
  }

  t[needed] = (lchar_t)0;
  return t;
}
#else
;
#endif

extern uchar_t *ulsnnrepeat(
     uchar_t *t, size_t tn, const lchar_t *s, size_t sn, size_t times)
#if defined(LUB_DEFINITIONS)
{ if (!t || !s) { return (uchar_t *)NULL; }
  if (tn > LUB_MAX_USTRLEN) { tn = LUB_MAX_USTRLEN; }
  if (sn > LUB_MAX_LSTRLEN) { sn = LUB_MAX_LSTRLEN; }

  size_t s_len = lcsnlen(s, sn);
  if (LUB_SIZE_ERR(s_len, 0))
  { t[tn] = (uchar_t)0; return (uchar_t *)s_len; }

  if (times == 0) { t[0] = (uchar_t)0; return t; }

  if (s_len && times > tn / s_len)
  { t[tn] = (uchar_t)0; return (uchar_t *)NULL; }

  size_t needed = s_len * times;
  if (needed > tn)
  { t[tn] = (uchar_t)0; return (uchar_t *)NULL; }

  for (size_t rep = 0; rep < times; ++rep)
  { for (size_t i = 0; i < s_len; ++i)
    { t[rep * s_len + i] = (uchar_t)s[i]; }
  }

  t[needed] = (uchar_t)0;
  return t;
}
#else
;
#endif // LUB_DEFINITIONS for ulsnnrepeat.

extern
uchar_t *uusnnrepeat
( uchar_t *t, size_t tn, const uchar_t *s, size_t sn, size_t times)
#if defined(LUB_DEFINITIONS)
{ if (!t || !s) return (uchar_t *)NULL;
  if (tn > LUB_MAX_USTRLEN) tn = LUB_MAX_USTRLEN;
  if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;

  size_t s_len = ucsnlen(s, sn);
  if (LUB_SIZE_ERR(s_len, 0))
  { t[tn] = (uchar_t)0;  return (uchar_t *)s_len; }

  if (times == 0)
  { t[0] = (uchar_t)0; return t; }

  if (s_len && times > tn / s_len)
  { t[tn] = (uchar_t)0; return (uchar_t *)NULL; }

  size_t needed = s_len * times;
  if (needed > tn)
  { t[tn] = (uchar_t)0; return (uchar_t *)NULL; }

  for (size_t rep = 0; rep < times; ++rep)
  { for (size_t i = 0; i < s_len; ++i)
    { t[rep * s_len + i] = s[i]; }
  }

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
 * @param trunc Pointer to string that specifies how to handle
 *              truncation (optional alphabetic truncate mode
 *              character and truncated replacement string).
 *              See parameter trunc in @ref CommonParameters
 *              "Common Parameters" for details.
 * @param map Pointer to a string of needles and their replacements.
 * 
 *   1. If delim is not a null-character, string is encoded as:
 * 
 *         <<needle1><delim><rep1><delim><needle2><delim><rep2>...<null-terminator>
 *
 *      where needle and replace substrings are not null-terminated
 *      but are separated by the delim character.
 * 
 *      A zero-length needle substring does not match any substring
 *      and is effectively ignored along with its
 *      corresponding replace substring.
 * 
 *      A zero-length replace substring is allowed (followed by the delimiter
 *      or the map string's null-terminator) and replaces a substring
 *      in the source that matches the preceding needle by
 *      an empty string.
 * 
 *   2. If delim is a null-character, string is encoded as:
 *
 *         <needle1><rep1><needle2><rep2>...<null-terminator>
 * 
 *      where needles and replacements are single nonnull characters.
 * @param delim Mapping delimiter. Null character indicates character mapping
 *              otherwise, delimited substring mapping.
 * @param m Indicates to search for the mth occurrence of any needle
 *          substring or needle character.
 * 
 *          m > 0 counts from the beginning of the haystack string s1.
 *                (1 means first occurrence).
 * 
 *          m = 0 replaces all occurrences of any needle substring
 *                or needle character.
 * 
 *          m < 0 counts from the end of the haystack string s1.
 *                (-1 means last occurrence).
 *
 *          Example (character-needle mode, delim = '\0'):
 * 
 *            haystack = "aaabccdeebe", map = "dgbh"
 *            m = 0 -> replaces all 'b' with 'h' and 'd' with 'g': "aaahccgeehe"
 *            m = 1 -> replaces first 'b' with 'h': "aaahccdeebe"
 *            m = 2 -> replaces 'd' with 'g': "aaabccgeebe"
 *            m = 3 -> replaces second 'b' with 'h': "aaabccdeehe"
 * 
 *           <0 replaces the mth match from end (-1 is last).
 * @return t, NULL (if t is NULL), or error.
 * 
 *         For an error, if t, *t is set to a null-terminator.
 *
 * @note Errors: 
 * - LUB_PTR_INVALID if s or map is an invalid pointer.
 * - LUB_UNTERMINATED if s or map is not null-terminated
 * - LUB_OPT_INVALID if malformed map syntax
 *   (missing delimiter or empty needle)
 * - LUB_OVERLAP if target and source buffers overlap in a way
 *   that would cause incorrect results in the target buffer.
 *  - LUB_TRUNCATED if the result cannot fit in the target buffer.
 *
 * @note The first matching needle substring/character in map wins.
 *       For a substring map, list a longer needle before any of its
 *       prefixes to match to the longer needle (otherwise, the shorter
 *       prefix matches first and the longer needle is ignored).
 *
 * @note Case Handling:
 * 
 *   For replace variants, character/substring matching is case-sensitive.
 * 
 *   For REPLACE variants, character/substring matching is case-insensitive.
 * 
 *   delimiter matching is case-sensitive;
 * 
 *   replacement is case-preserving.
 *
 * @note For multi-pair maps, the first matching needle in map order wins.
 *       List longer needles before shorter ones if overlap matters.
 * @{
 */

#if defined(LUB_DEFINITIONS)

// Replace case-sensitive.

static
lchar_t *lub_llsnnreplace_def
( lchar_t *t, size_t tn, const lchar_t *s, size_t sn,
  const lchar_t *map, lchar_t delim, ptrdiff_t m
)
{ if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0) || LUB_PTR_ERR(map, 0))
    return LUB_PTR_ERR(LUB_PTR_INVALID, 0);
  if (!t || !s || !map || !delim) return (lchar_t *)NULL;
  if (tn > LUB_MAX_LSTRLEN) tn = LUB_MAX_LSTRLEN;
  if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;

  size_t s_len = lcsnlen(s, sn);
  if (LUB_SIZE_ERR(s_len, 0)) return NULL;
  size_t map_len = lcsnlen(map, LUB_MAX_LOPTLEN);
  if (LUB_SIZE_ERR(map_len, 0)) return (lchar_t *)NULL;

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
#else
;
#endif // LUB_DEFINITIONS for uusnnrepeat

static
uchar_t *lub_ulsnnreplace_def
( uchar_t *t, size_t tn, const lchar_t *s, size_t sn,
  const lchar_t *map, lchar_t delim, ptrdiff_t m
)
{ if (!t || !s || !map || !delim) return (uchar_t *)NULL;
  if (tn > LUB_MAX_USTRLEN) tn = LUB_MAX_USTRLEN;
  if (sn > LUB_MAX_LSTRLEN) sn = LUB_MAX_LSTRLEN;

  size_t s_len = ucsnlen(s, sn);
  if (LUB_SIZE_ERR(s_len, 0)) return NULL;
  size_t map_len = lcsnlen(map, LUB_MAX_LOPTLEN);
  if (LUB_SIZE_ERR(map_len, 0)) return NULL;

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

static
uchar_t *lub_uusnnreplace_def
( uchar_t *t, size_t tn, const uchar_t *s, size_t sn, const lchar_t *st,
  const uchar_t *map, uchar_t delim, ptrdiff_t m)
{   if (!t || !s || !map || !delim) return (uchar_t *)NULL;
    if (tn > LUB_MAX_USTRLEN) tn = LUB_MAX_USTRLEN;
    if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;

    size_t s_len = ucsnlen(s, sn);
  if (LUB_SIZE_ERR(s_len, 0)) return NULL;
    size_t map_len = ucsnlen(map, LUB_MAX_UOPTLEN);
  if (LUB_SIZE_ERR(map_len, 0)) return NULL;

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

static lchar_t *lub_llsnnREPLACE_def
( lchar_t *t, size_t tn, const lchar_t *s, const lchar_t *map, lchar_t delim, size_t n,
    ptrdiff_t m
)
{ if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(s, 0)) \
    return LUB_PTR_ERR(LUB_PTR_INVALID, 0); \
  if (!t || !s || !map || !delim) return (lchar_t *)NULL;
    if (tn > LUB_MAX_LSTRLEN) tn = LUB_MAX_LSTRLEN;
    if (n > LUB_MAX_LSTRLEN) n = LUB_MAX_LSTRLEN;

    size_t s_len = lcsnlen(s, n);
  if (LUB_SIZE_ERR(s_len, 0)) return NULL;
    size_t map_len = lcsnlen(map, LUB_MAX_LOPTLEN);
  if (LUB_SIZE_ERR(map_len, 0)) return NULL;

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

static
uchar_t *lub_ulsnnREPLACE_def
( uchar_t *t, size_t tn, const uchar_t *s,
  const lchar_t *map, lchar_t delim, size_t n,
  ptrdiff_t m
)
{ if (!t || !s || !map || !delim) return (uchar_t *)NULL;
  if (tn > LUB_MAX_USTRLEN) tn = LUB_MAX_USTRLEN;
  if (n > LUB_MAX_USTRLEN) n = LUB_MAX_USTRLEN;

    size_t s_len = ucsnlen(s, n);
  if (LUB_SIZE_ERR(s_len, 0)) return NULL;
    size_t map_len = lcsnlen(map, LUB_MAX_LOPTLEN);
  if (LUB_SIZE_ERR(map_len, 0)) return NULL;

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
                    if (uutoupper(s[si + j]) != uutoupper((uchar_t)first_needle[j]))
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
                if (uutoupper(s[si + j]) != uutoupper((uchar_t)first_needle[j]))
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
                if (uutoupper(s[si + j]) != uutoupper((uchar_t)needle[j])) match = 0;

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

static
uchar_t *lub_uusnnREPLACE_def
( uchar_t *t, size_t tn, const uchar_t *s, size_t sn, const lchar_t *st,
  const uchar_t *map, uchar_t delim, ptrdiff_t m
)
{ if (!t || !s || !map || !delim) return (uchar_t *)NULL;
  if (tn > LUB_MAX_USTRLEN) tn = LUB_MAX_USTRLEN;
  if (sn > LUB_MAX_USTRLEN) sn = LUB_MAX_USTRLEN;

  size_t s_len = ucsnlen(s, sn);
  if (LUB_SIZE_ERR(s_len, 0)) return NULL;
  size_t map_len = ucsnlen(map, LUB_MAX_UOPTLEN);
  if (LUB_SIZE_ERR(map_len, 0)) return NULL;
  
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
        if (uutoupper(s[si + j]) != uutoupper(first_needle[j])) {match = 0; break;}
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
        if (uutoupper(s[si + j]) != uutoupper(first_needle[j])) match = 0;

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
                if (uutoupper(s[si + j]) != uutoupper(needle[j])) match = 0;

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

extern lchar_t *lub_replace_ext
( const char xt, lchar_t *t, size_t tn,
  const char xs, const lchar_t *s, size_t sn,
  const char xm, const lchar_t *map, size_t mapn,
  const int delim, const ptrdiff_t m, const char Case
)
#if defined(LUB_DEFINITIONS)
{ (void)mapn;
  if (Case == 's')
  { if (xt == 'l' && xs == 'l' && xm == 'l')
      return (lchar_t *)lub_llsnnreplace_def
               ((lchar_t *)t, tn, (const lchar_t *)s, sn,
                (const lchar_t *)map, (lchar_t)delim, m);
    if (xt == 'u' && xs == 'l' && xm == 'l')
      return (lchar_t *)lub_ulsnnreplace_def
             ((uchar_t *)t, tn, (const lchar_t *)s, sn,
              (const lchar_t *)map, (lchar_t)delim, m);
    if (xt == 'u' && xs == 'u' && xm == 'u')
      return (lchar_t *)lub_uusnnreplace_def
               ((uchar_t *)t, tn, (const uchar_t *)s, sn,
                (const lchar_t *)NULL,
                (const uchar_t *)map, (uchar_t)delim, m);
  }
  else if (Case == 'i')
  { if (xt == 'l' && xs == 'l' && xm == 'l')
      return (lchar_t *)lub_llsnnREPLACE_def
               ((lchar_t *)t, tn, (const lchar_t *)s,
                (const lchar_t *)map, (lchar_t)delim, sn, m);
    if (xt == 'u' && xs == 'u' && xm == 'l')
      return (lchar_t *)lub_ulsnnREPLACE_def
               ((uchar_t *)t, tn, (const uchar_t *)s,
                (const lchar_t *)map, (lchar_t)delim, sn, m);
    if (xt == 'u' && xs == 'u' && xm == 'u')
      return (lchar_t *)lub_uusnnREPLACE_def
               ((uchar_t *)t, tn, (const uchar_t *)s, sn, (const lchar_t *)NULL,
                (const uchar_t *)map, (uchar_t)delim, m);
  }
  return (lchar_t *)LUB_PTR_ERR(LUB_INTERNAL_ERROR, 0);
}
#else
;
#endif

static inline
lchar_t *llsnnreplace
( lchar_t *t, size_t tn, const lchar_t *s, size_t sn,
  const lchar_t *map, lchar_t delim, ptrdiff_t m
)
{ return (lchar_t *)lub_replace_ext
           ( 'l', t, tn,
             'l', s, sn,
             'l', map, LUB_MAX_LOPTLEN,
             (int)delim, m, 's'
           );
}

static inline
uchar_t *ulsnnreplace
( uchar_t *t, size_t tn, const lchar_t *s, size_t sn,
  const lchar_t *map, lchar_t delim, ptrdiff_t m
)
{ return (uchar_t *)lub_replace_ext
           ( 'u', (lchar_t *)t, tn,
             'l', s, sn,
             'l', map, LUB_MAX_LOPTLEN,
             (int)delim, m, 's'
           );
}

static inline
uchar_t *uusnnreplace
( uchar_t *t, size_t tn, const uchar_t *s, size_t sn,
  const uchar_t *map, uchar_t delim, ptrdiff_t m )
{ return (uchar_t *)lub_replace_ext
           ( 'u', (lchar_t *)t, tn,
             'u', (const lchar_t *)s, sn,
             'u', (const lchar_t *)map, LUB_MAX_UOPTLEN,
             (int)delim, m, 's'
           );
}

static inline
lchar_t *llsnnREPLACE
( lchar_t *t, size_t tn, const lchar_t *s,
  const lchar_t *map, lchar_t delim, size_t n,
  ptrdiff_t m
)
{ return (lchar_t *)lub_replace_ext
           ( 'l', t, tn,
             'l', s, n,
             'l', map, LUB_MAX_LOPTLEN,
             (int)delim, m, 'i'
           );
}

static inline
uchar_t *ulsnnREPLACE
( uchar_t *t, size_t tn, const uchar_t *s,
  const lchar_t *map, lchar_t delim, size_t n,
  ptrdiff_t m
)
{ return (uchar_t *)lub_replace_ext
           ( 'u', (lchar_t *)t, tn,
             'u', (const lchar_t *)s, n,
             'l', map, LUB_MAX_LOPTLEN,
             (int)delim, m, 'i'
           );
}

static inline
uchar_t *uusnnREPLACE
( uchar_t *t, size_t tn, const uchar_t *s, size_t sn,
  const uchar_t *map, uchar_t delim, ptrdiff_t m
)
{ return (uchar_t *)lub_replace_ext
           ( 'u', (lchar_t *)t, tn,
             'u', (const lchar_t *)s, sn,
             'u', (const lchar_t *)map, LUB_MAX_UOPTLEN,
             (int)delim, m, 'i'
           );
}

/** @} */

/**
 * @defgroup PrintfStyleFormatting Printf-style Formatting
 * @name llsnvprintf, llsnprintf
 * @brief Format Latin text into target buffer.
 * @param t Target buffer.
 * @param tn Maximum number of characters for target buffer
 *           (excluding terminator).
 * @param fmt Format string.
 * @param ap Variable argument list for llsnvprintf.
 * @return Number of chars written (excluding terminator),
 *         or -1 on error/truncate.
 *
 * @note This family uses C vsnprintf semantics and treats
 *       lchar_t storage as bytes.
 * @{
 */

extern
int llsnvprintf
( lchar_t *t, size_t tn, const lchar_t *fmt, va_list ap )
#if defined(LUB_DEFINITIONS)
{ if (LUB_PTR_ERR(t, 0) || LUB_PTR_ERR(fmt, 0))
  { return LUB_INT_ERR(LUB_PTR_INVALID, 0); }
  if (!t || !fmt || !tn) {
   if (t && tn) *t = (lchar_t)0;
   return -1;
  }

  if (tn > LUB_MAX_LSTRLEN + 1) tn = LUB_MAX_LSTRLEN + 1;

  int rc = vsnprintf((char *)t, tn, (const char *)fmt, ap);
  if (rc < 0 || (size_t)rc >= tn)
  { t[tn - 1] = (lchar_t)0;
    return -1;
  }

  return rc;
}
#else
;
#endif // LUB_DEFINITIONS

extern
int llsnprintf
( lchar_t *t, size_t tn, const lchar_t *fmt, ... )
#if defined(LUB_DEFINITIONS)
{ va_list ap;
  va_start(ap, fmt);
  int rc = llsnvprintf(t, tn, fmt, ap);
  va_end(ap);
  return rc;
}
#else
;
#endif // LUB_DEFINITIONS

/** @} */

/**
 * @section PolymorphicMacros Polymorphic Macros
 *
 * @brief Polymorphic macros (also referred to as x macros) are
 * macros that map an x in their names to l if LUB_X_IS_L is defined
 * or u if LUB_X_IS_U is defined such that the resulting name
 * is the name of an API function or macro.
 * 
 * A .c file can reference the x macros for polymorphic code that
 * works for both Latin and Unicode by compiling once for Latin
 * and once for Unicode.
 *
 * Mapping (x resolved to l):
 *
 *   xl -> ll  (x-target <- l-source becomes l-target <- l-source)
 *   lx -> ll  (l-target <- x-source becomes l-target <- l-source)
 *   xx -> ll  (x-target <- x-source becomes l-target <- l-source)
 *    x -> l   (x-type becomes l-type)
 * 
 * Mapping (x resolved to u):
 *
 *   xu -> uu  (x-target <- u-source becomes u-target <- u-source)
 *   lx -> lu  (l-target <- x-source becomes l-target <- u-source)
 *   xx -> uu  (x-target <- x-source becomes u-target <- u-source)
 *    x -> u   (x-type becomes u-type)
 * 
 * @note At most one of LUB_X_IS_L and LUB_X_IS_U can be defined when
 *       including lubtype.h for polymorphic macros.
 *       If neither is defined, the x macros are not defined.
 * 
 * @note LUB_DEFINITIONS must not be defined when including lubtype.h for
 *       polymorphic macros. If LUB_DEFINITIONS is defined, it indicates that
 *       the function definitions are being included, and the x macros are not
 *       intended to be used in that context.
 */

#if defined(LUB_X_IS_L) || defined(LUB_X_IS_U)

#if defined(LUB_DEFINITIONS)
#error "lubtype.h: LUB_DEFINITIONS macro must not be defined " \
       "when including lubtype.h for polymorphic macros (i.e., " \
       "when LUB_X_IS_L or LUB_X_IS_U is defined)."
#endif

#if defined(LUB_X_IS_L) && defined(LUB_X_IS_U)
#error "lubtype.h: Both LUB_X_IS_L and LUB_X_IS_U are defined. " \
       "At most one of them can be defined at a time."
#endif

/**
 * @name LUB_X
 * @brief A macro that defines whether x maps to l or to u for
 *        polymorphic (x) macros.
 *        Value is l if LUB_X_IS_L is defined, u if LUB_X_IS_U is defined.
 *        If neither is defined, LUB_X is not defined and the polymorphic
 *        macros are not defined.
 */

#if defined(LUB_X)
#error "lubtype.h: LUB_X macro is unexpectedly already defined. " \
       "#undef before including lubtype.h."
#endif

#if defined(LUB_X_IS_L)
#define LUB_X l
#else
#define LUB_X u
#endif

// Types

#define xchar_t LUB_PASTE(LUB_X, char_t)

// Limits

#define LUB_MAX_XCHAR LUB_PASTE(LUB_PASTE(LUB_MAX_, LUB_X), CHAR)
#define LUB_MAX_XSTRLEN LUB_PASTE(LUB_PASTE(LUB_MAX_, LUB_X), STRLEN)
#define LUB_MAX_XOPTLEN LUB_PASTE(LUB_PASTE(LUB_MAX_, LUB_X), OPTLEN)

// Character classification

#define isxalpha LUB_PASTE(LUB_PASTE(is, LUB_X), alpha)
#define isxdigit LUB_PASTE(LUB_PASTE(is, LUB_X), digit)
#define isxalnum LUB_PASTE(LUB_PASTE(is, LUB_X), alnum)
#define isxlatin LUB_PASTE(LUB_PASTE(is, LUB_X), latin)
#define isxname1c LUB_PASTE(LUB_PASTE(is, LUB_X), name1c)
#define isxnamec LUB_PASTE(LUB_PASTE(is, LUB_X), namec)
#define isxupper LUB_PASTE(LUB_PASTE(is, LUB_X), upper)
#define isxlower LUB_PASTE(LUB_PASTE(is, LUB_X), lower)
#define isxcntrl LUB_PASTE(LUB_PASTE(is, LUB_X), cntrl)
#define isxprint LUB_PASTE(LUB_PASTE(is, LUB_X), print)
#define isxpunct LUB_PASTE(LUB_PASTE(is, LUB_X), punct)
#define isxblank LUB_PASTE(LUB_PASTE(is, LUB_X), blank)
#define isxspace LUB_PASTE(LUB_PASTE(is, LUB_X), space)
#define isxgraph LUB_PASTE(LUB_PASTE(is, LUB_X), graph)
#define isxhex LUB_PASTE(LUB_PASTE(is, LUB_X), hex)

// int <- Hex digit

#define ixhex(c) LUB_PASTE(LUB_PASTE(is, LUB_X), hex)

// String length

#define xcsnlen LUB_PASTE(LUB_X, csnlen)

// String classification

#define isxnlatinstr LUB_PASTE(is, LUB_PASTE(LUB_X, nlatinstr))
#define isxnalphastr LUB_PASTE(is, LUB_PASTE(LUB_X, nalphastr))
#define isxnhexstr LUB_PASTE(is, LUB_PASTE(LUB_X, nhexstr))
#define isxreserved LUB_PASTE(is, LUB_PASTE(LUB_X, reserved))
#define isxqname LUB_PASTE(is, LUB_PASTE(LUB_X, qname))

#define isxtruncstr LUB_PASTE(is, LUB_PASTE(LUB_X, truncstr))
#define isxtrimstr LUB_PASTE(is, LUB_PASTE(LUB_X, trimstr))
#define isxpadstr LUB_PASTE(is, LUB_PASTE(LUB_X, padstr))
#define isxneedlestr LUB_PASTE(is, LUB_PASTE(LUB_X, needlestr))

// Character case transforms

#define xltocase LUB_PASTE(LUB_X, ltocase)
#define xltolower LUB_PASTE(LUB_X, ltolower)
#define xltoupper LUB_PASTE(LUB_X, ltoupper)

#if defined(LUB_X_IS_L)
#define xutocase(c, lrep) LUB_PASTE(LUB_X, utocase)((c), (lrep))
#define xutolower(c, lrep) LUB_PASTE(LUB_X, utolower)((c), (lrep))
#define xutoupper(c, lrep) LUB_PASTE(LUB_X, utoupper)((c), (lrep))
#else // LUB_X_IS_U
#define xutocase(c, lrep) LUB_PASTE(LUB_X, utocase)((c))
#define xutolower(c, lrep) LUB_PASTE(LUB_X, utolower)((c))
#define xutoupper(c, lrep) LUB_PASTE(LUB_X, utoupper)((c))
#endif

#define xxtocase LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), tocase)
#define xxtolower LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), tolower)
#define xxtoupper LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), toupper)

// Compare

#define xlsnncmp LUB_PASTE(LUB_X, lsnncmp)
#define xlsnnCMP LUB_PASTE(LUB_X, lsnnCMP)
#define xlsnnfxdcmp LUB_PASTE(LUB_X, lsnnfxdcmp)
#define xlsnnFXDCMP LUB_PASTE(LUB_X, lsnnFXDCMP)
#define xlsnnpfxcmp LUB_PASTE(LUB_X, lsnnpfxcmp)
#define xlsnnPFXCMP LUB_PASTE(LUB_X, lsnnPFXCMP)
#define xlsnnsfxcmp LUB_PASTE(LUB_X, lsnnsfxcmp)
#define xlsnnSFXCMP LUB_PASTE(LUB_X, lsnnSFXCMP)

#define xusnncmp LUB_PASTE(LUB_X, usnncmp)
#define xusnnCMP LUB_PASTE(LUB_X, usnnCMP)
#define xusnnfxdcmp LUB_PASTE(LUB_X, usnnfxdcmp)
#define xusnnFXDCMP LUB_PASTE(LUB_X, usnnFXDCMP)
#define xusnnpfxcmp LUB_PASTE(LUB_X, usnnpfxcmp)
#define xusnnPFXCMP LUB_PASTE(LUB_X, usnnPFXCMP)
#define xusnnsfxcmp LUB_PASTE(LUB_X, usnnsfxcmp)
#define xusnnSFXCMP LUB_PASTE(LUB_X, usnnSFXCMP)

#define xxsnncmp LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snncmp)
#define xxsnnCMP LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snnCMP)
#define xxsnnfxdcmp LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snnfxdcmp)
#define xxsnnFXDCMP LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snnFXDCMP)
#define xxsnnpfxcmp LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snnpfxcmp)
#define xxsnnPFXCMP LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snnPFXCMP)
#define xxsnnsfxcmp LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snnsfxcmp)
#define xxsnnSFXCMP LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snnSFXCMP)

// Pointer trim

#define xlsnptrim LUB_PASTE(LUB_X, lsnptrim)
#define xusnptrim LUB_PASTE(LUB_X, usnptrim)
#define xxsnptrim LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snptrim)

// Skip

#define xlsnskip LUB_PASTE(LUB_X, lsnskip)
#define xxsnskip LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snskip)

// Search

#define xlsnstrm LUB_PASTE(LUB_X, lsnstrm)
#define xusnstrm LUB_PASTE(LUB_X, usnstrm)
#define xxsnstrm LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snstrm)

#define xlsnSTRM LUB_PASTE(LUB_X, lsnSTRM)
#define xusnSTRM LUB_PASTE(LUB_X, usnSTRM)
#define xxsnSTRM LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snSTRM)

// Count

#define xlsncnt LUB_PASTE(LUB_X, lsncnt)
#define xlsnCNT LUB_PASTE(LUB_X, lsnCNT)

#define xxsncnt LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), sncnt)
#define xxsnCNT LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snCNT)

// Concatenate

#define xlsnncat LUB_PASTE(LUB_X, lsnncat)
#define xlsnncatc LUB_PASTE(LUB_X, lsnncatc)
#define xlsnnCATC LUB_PASTE(LUB_X, lsnnCATC)
#define xlsnncatq LUB_PASTE(LUB_X, lsnncatq)
#define xlsnncatqc LUB_PASTE(LUB_X, lsnncatqc)
#define xlsnnCATQC LUB_PASTE(LUB_X, lsnnCATQC)

#if defined(LUB_X_IS_L)
#define xusnncat(t, tn, s, sn, trunc, lrep) \
  lusnncat((t), (tn), (s), (sn), (trunc), (lrep))
#define xusnncatc(t, tn, s, sn, trunc, lrep) \
  lusnncatc((t), (tn), (s), (sn), (trunc), (lrep))
#define xusnnCATC(t, tn, s, sn, trunc, lrep) \
  lusnnCATC((t), (tn), (s), (sn), (trunc), (lrep))
#define xusnncatq(t, tn, s, sn, trunc, q, lrep) \
  lusnncatq((t), (tn), (s), (sn), (trunc), (q), (lrep))
#define xusnncatqc(t, tn, s, sn, trunc, q, lrep) \
  lusnncatqc((t), (tn), (s), (sn), (trunc), (q), (lrep))
#define xusnnCATQC(t, tn, s, sn, trunc, q, lrep) \
  lusnnCATQC((t), (tn), (s), (sn), (trunc), (q), (lrep))
#else // LUB_X_IS_U
#define xusnncat(t, tn, s, sn, trunc, lrep) \
  uusnncat((t), (tn), (s), (sn), (trunc))
#define xusnncatc(t, tn, s, sn, trunc, lrep) \
  uusnncatc((t), (tn), (s), (sn), (trunc))
#define xusnnCATC(t, tn, s, sn, trunc, lrep) \
  uusnnCATC((t), (tn), (s), (sn), (trunc))
#define xusnncatq(t, tn, s, sn, trunc, q, lrep) \
  uusnncatq((t), (tn), (s), (sn), (trunc), (q))
#define xusnncatqc(t, tn, s, sn, trunc, q, lrep) \
  uusnncatqc((t), (tn), (s), (sn), (trunc), (q))
#define xusnnCATQC(t, tn, s, sn, trunc, q, lrep) \
  uusnnCATQC((t), (tn), (s), (sn), (trunc), (q))
#endif

#define xxsnncat LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snncat)
#define xxsnncatc LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snncatc)
#define xxsnnCATC LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snnCATC)
#define xxsnncatq LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snncatq)
#define xxsnncatqc LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snncatqc)
#define xxsnnCATQC LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snnCATQC)

// Copy

#define xlsnncpy LUB_PASTE(LUB_X, lsnncpy)
#define xlsnncpyc LUB_PASTE(LUB_X, lsnncpyc)
#define xlsnnCPYC LUB_PASTE(LUB_X, lsnnCPYC)
#define xlsnncpyq LUB_PASTE(LUB_X, lsnncpyq)
#define xlsnncpyqc LUB_PASTE(LUB_X, lsnncpyqc)
#define xlsnnCPYQC LUB_PASTE(LUB_X, lsnnCPYQC)

#if defined(LUB_X_IS_L)
#define xusnncpy(t, tn, s, sn, trunc, lrep) \
  lusnncpy((t), (tn), (s), (sn), (trunc), (lrep))
#define xusnncpyc(t, tn, s, sn, trunc, lrep) \
  lusnncpyc((t), (tn), (s), (sn), (trunc), (lrep))
#define xusnnCPYC(t, tn, s, sn, trunc, lrep) \
  lusnnCPYC((t), (tn), (s), (sn), (trunc), (lrep))
#define xusnncpyq(t, tn, s, sn, trunc, q, lrep) \
  lusnncpyq((t), (tn), (s), (sn), (trunc), (q), (lrep))
#define xusnncpyqc(t, tn, s, sn, trunc, q, lrep) \
  lusnncpyqc((t), (tn), (s), (sn), (trunc), (q), (lrep))
#define xusnnCPYQC(t, tn, s, sn, trunc, q, lrep) \
  lusnnCPYQC((t), (tn), (s), (sn), (trunc), (q), (lrep))
#else // LUB_X_IS_U
#define xusnncpy(t, tn, s, sn, trunc, lrep) \
  uusnncpy((t), (tn), (s), (sn), (trunc))
#define xusnncpyc(t, tn, s, sn, trunc, lrep) \
  uusnncpyc((t), (tn), (s), (sn), (trunc))
#define xusnnCPYC(t, tn, s, sn, trunc, lrep) \
  uusnnCPYC((t), (tn), (s), (sn), (trunc))
#define xusnncpyq(t, tn, s, sn, trunc, q, lrep) \
  uusnncpyq((t), (tn), (s), (sn), (trunc), (q))
#define xusnncpyqc(t, tn, s, sn, trunc, q, lrep) \
  uusnncpyqc((t), (tn), (s), (sn), (trunc), (q))
#define xusnnCPYQC(t, tn, s, sn, trunc, q, lrep) \
  uusnnCPYQC((t), (tn), (s), (sn), (trunc), (q))
#endif

#define xxsnncpy LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snncpy)
#define xxsnncpyc LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snncpyc)
#define xxsnnCPYC LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snnCPYC)
#define xxsnncpyq LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snncpyq)
#define xxsnncpyqc LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snncpyqc)
#define xxsnnCPYQC LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snnCPYQC)

// Pad

#define xlsnnpad LUB_PASTE(LUB_X, lsnnpad)

#if defined(LUB_X_IS_L)
#define xusnnpad(t, tn, s, sn, pad, lrep) \
  lusnnpad((t), (tn), (s), (sn), (pad), (lrep))
#else // LUB_X_IS_U
#define xusnnpad(t, tn, s, sn, pad, lrep) \
  uusnnpad((t), (tn), (s), (sn), (pad))
#endif

#define xxsnnpad LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snnpad)

// Trim

#define xlsnntrim LUB_PASTE(LUB_X, lsnntrim)

#if defined(LUB_X_IS_L)
#define xusnntrim(t, tn, s, sn, trunc, trim, delim, lrep) \
  lusnntrim((t), (tn), (s), (sn), (trunc), (trim), (delim), (lrep))
#else // LUB_X_IS_U
#define xusnntrim(t, tn, s, sn, trunc, trim, delim, lrep) \
  uusnntrim((t), (tn), (s), (sn), (trunc), (trim), (delim))
#endif

#define xxsnntrim LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snntrim)

// Replace

#define xlsnnreplace LUB_PASTE(LUB_X, lsnnreplace)
#define xlsnnREPLACE LUB_PASTE(LUB_X, lsnnREPLACE)
#define xxsnnreplace LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snnreplace)
#define xxsnnREPLACE LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snnREPLACE)

// Repeat

#define xlsnnrepeat LUB_PASTE(LUB_X, lsnnrepeat)
#define xxsnnrepeat LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snnrepeat)

// Reverse

#define xlsnnreverse LUB_PASTE(LUB_X, lsnnreverse)

#if defined(LUB_X_IS_L)
#define xusnnreverse(t, tn, s, sn, lrep) \
  lusnnreverse((t), (tn), (s), (sn), (lrep))
#else // LUB_X_IS_U
#define xusnnreverse(t, tn, s, sn, lrep) \
  uusnnreverse((t), (tn), (s), (sn))
#endif

#define xxsnnreverse LUB_PASTE(LUB_PASTE(LUB_X, LUB_X), snnreverse)

// Printf-style formatting (Latin-only; no Unicode equivalent)

#if defined(LUB_X_IS_L)
#define xlsnprintf llsnprintf
#define xlsnvprintf llsnvprintf
#endif

#endif // !defined(LUB_DEFINITIONS)

/**
 * @section Examples Examples
 *
 * - Copy, case-preserving:
 *   llsnncpy(dst_l, 200, src_l, 32, NULL);
 *
 * - Search mth occurrence, case-sensitive:
 *   const lchar_t *p = llsnstrm(haystack, needle, 32, 0, 2);
 *   // p points at the second "two"
 *
 * - Trim whitespace left and right (B) with no collapse of interior whitespace:
 *   llsnntrim(dst_l, 200, spaced, 200, 0, NULL, 'B', '\0');
 *   // -> "abc   de"
 *
 * - Replace, case-sensitive, multi-pair map, 2nd occurrence:
 *   llsnnreplace(t, 64, src, 64, map, '|', 2);
 *   // -> "cat and dog"
 *
 *   llsnnreplace(t, 64, src2, 64, map2, '|', -1);
 *   // -> "x_x_y"
 *
 *   llsnnreplace(t, 64, src3, 64, map3, '|', 0);
 *   // -> "ba ab" (first matching pair wins)
 *
 *   llsnnreplace(t, 64, src4, 64, map3, '|', 1);
 *   // -> NULL (multi-pair map requires m == 0)
 *
 * - Replace, case-insensitive:
 *   llsnnREPLACE(t, 64, src5, 64, map5, '|', 0);
 *   // -> "hi World"
 *
 * - Replace, case-insensitive:
 *   llsnnREPLACE(t, 64, src6, 64, map, '|', 0);
 *   // -> "dog and dog"
 *
 * - Reverse:
 *   llsnnreverse(t, 64, "stressed", 8);
 *   // -> "desserts"
 *
 * - Pad (left, right, center):
 *   llsnnpad(t, 6, src7, 6, "L.");
 *   // -> "...cat"
 *   llsnnpad(t, 6, src7, 6, "R.");
 *   // -> "cat..."
 *   llsnnpad(t, 7, src7, 7, "B.");
 *   // -> "..cat.."
 *
 * - Format:
 *   llsnprintf(dst_l, 64, "id=%u", (unsigned)42);
 *   // -> "id=42"
 */

/**
 * @section GlossaryOfTerms Glossary
 * byte_t
 *   Raw byte (uint8_t). Values x'00'-x'FF'. No null terminator semantics.
 *
 * byte_t *
 *   Pointer to a string (*byte_t) of bytes (byte_t). A specific
 *   length must be provided for byte strings, as they do not
 *   have null terminator semantics.
 *
 * lchar_t
 *   Latin-8 character (uint8_t). Values 1-255.
 *   0 is reserved for null terminator.
 *
 * lchar_t *
 *   Pointer to a null-terminated string (lchar_t *)
 *   of Latin characters (lchar_t).
 *
 * lrep
 *   Substitution char for out-of-range Unicode->Latin conversions.
 * 
 * LUB_MAX_BSTRLEN
 *   Maximum number of bytes in a byte string (*byte_t).
 * 
 * LUB_MAX_LSTRLEN
 *   Maximum number of characters in a Latin string (lchar_t *).
 *
 * LUB_MAX_USTRLEN
 *   Maximum number of characters in a Unicode string (uchar_t *).
 * 
 * Polymorphic macros (x macros)
 *   Macros with x in their names where x is mapped to
 *   l if LUB_X_IS_L is defined or u if LUB_X_IS_U is defined.
 *   If neither is defined, x macros are not defined.
 *   Polymorphic macros are not allowed if LUB_DEFINITIONS is defined.
 * 
 * sn
 *   Maximum source characters/bytes; for char strings, stops at null.
 * 
 * tn
 *   Maximum target characters excluding null-terminator or
 *   target buffer byte length.
 *
 * uchar_t
 *   Unicode-16 character (uint16_t). Values 1-65535.
 *   0 is reserved for null terminator.
 * 
 * uchar_t *
 *   Pointer to a null-terminated string (uchar_t *)
 *   of Unicode characters (uchar_t).
 *
 * unterminated
 *   A character string that does not have a null terminator
 *   at or before the specified or default bound.
 * 
 * x macros
 *   See Polymorphic macros above.
 */

// Undefine LUB_*_DEF macros to avoid namespace pollution.

#if defined(LUB_DEFINITIONS)

#undef LUB_OP_DEF

#endif // LUB_DEFINITIONS

#if defined(__cplusplus)
}
#endif

// End of lubtype.h
