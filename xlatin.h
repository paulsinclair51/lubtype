/**
 * @file xlatin.h
 * @brief Polymorphic macro remapping: x macros -> Latin (l) equivalents
 *   for lubtype.h API.
 *
 * This file remaps x macros (polymorphic macros with x in their name)
 * to their Latin (l) equivalents for use with the lubtype.h API.
 * A .c file can reference the x macros for polymorphic code that
 * works for both Latin and Unicode by compiling once with xlatin.h
 * and once with xunicode.h.
 *
 * Mapping (x resolved to l):
 *
 *   xl -> ll  (x-target <- l-source becomes l-target <- l-source)
 *   xx -> ll  (x-target <- x-source becomes l-target <- l-source)
 *    x -> l   (x-type becomes l-type)
 *
 * To map x macros to Unicode (u) equivalents, include @ref xunicode.h
 * instead.
 *
 * @note Only macros with corresponding functions defined in lubtype.h
 *       are provided. Not all encoding directions are available for
 *       every operation; see lubtype.h for details.
 *
 * @see xunicode.h
 * @see lubtype.h
 *
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
*/

#undef LUB_XTYPE
#define LUB_XTYPE l

// ---------------------------------------------------------------------------
// Limits
// ---------------------------------------------------------------------------

#undef LUB_MAX_XCHAR
#define LUB_MAX_XCHAR LUB_MAX_LCHAR
#undef LUB_MAX_XSTRLEN
#define LUB_MAX_XSTRLEN LUB_MAX_LSTRLEN

// ---------------------------------------------------------------------------
// Character classification  (x -> l)
// ---------------------------------------------------------------------------

#undef isxalpha
#define isxalpha islalpha
#undef isxdigit
#define isxdigit isldigit
#undef isxalnum
#define isxalnum islalnum
#undef isxupper
#define isxupper islupper
#undef isxlower
#define isxlower isllower
#undef isxcntrl
#define isxcntrl islcntrl
#undef isxprint
#define isxprint islprint
#undef isxgraph
#define isxgraph islgraph
#undef isxpunct
#define isxpunct islpunct
#undef isxblank
#define isxblank islblank
#undef isxspace
#define isxspace islspace
#undef isxhexdigit
#define isxhexdigit islhexdigit

// ---------------------------------------------------------------------------
// Hex digit -> int  (x -> l)
// ---------------------------------------------------------------------------

#undef ixhexdigit
#define ixhexdigit ilhexdigit

// ---------------------------------------------------------------------------
// String length  (x -> l)
// ---------------------------------------------------------------------------

#undef xcsnlen
#define xcsnlen lcsnlen

// ---------------------------------------------------------------------------
// Character case transforms
//   xl -> ll  (x-target <- l-source)
//   xx -> ll  (x-target <- x-source; same as xl when x=l)
// ---------------------------------------------------------------------------

#undef xltocase
#define xltocase lltocase
#undef xltolower
#define xltolower lltolower
#undef xltoupper
#define xltoupper lltoupper

#undef xxtocase
#define xxtocase lltocase
#undef xxtolower
#define xxtolower lltolower
#undef xxtoupper
#define xxtoupper lltoupper

// ---------------------------------------------------------------------------
// Compare
//   xl -> ll  (x-target/s1 <- l-source/s2)
//   xx -> ll  (x s1/s2; same as xl when x=l)
// ---------------------------------------------------------------------------

#undef xlsncmp
#define xlsncmp llsncmp
#undef xlsnCMP
#define xlsnCMP llsnCMP
#undef xlsnfxdcmp
#define xlsnfxdcmp llsnfxdcmp
#undef xlsnFXDCMP
#define xlsnFXDCMP llsnFXDCMP
#undef xlsnpfxcmp
#define xlsnpfxcmp llsnpfxcmp
#undef xlsnPFXCMP
#define xlsnPFXCMP llsnPFXCMP
#undef xlsnsfxcmp
#define xlsnsfxcmp llsnsfxcmp
#undef xlsnSFXCMP
#define xlsnSFXCMP llsnSFXCMP

#undef xxsncmp
#define xxsncmp llsncmp
#undef xxsnCMP
#define xxsnCMP llsnCMP
#undef xxsnfxdcmp
#define xxsnfxdcmp llsnfxdcmp
#undef xxsnFXDCMP
#define xxsnFXDCMP llsnFXDCMP
#undef xxsnpfxcmp
#define xxsnpfxcmp llsnpfxcmp
#undef xxsnPFXCMP
#define xxsnPFXCMP llsnPFXCMP
#undef xxsnsfxcmp
#define xxsnsfxcmp llsnsfxcmp
#undef xxsnSFXCMP
#define xxsnSFXCMP llsnSFXCMP

// ---------------------------------------------------------------------------
// Search
// ---------------------------------------------------------------------------

#undef xlsnstrm
#define xlsnstrm llsnstrm
#undef xlsnSTRM
#define xlsnSTRM llsnSTRM

#undef xxsnstrm
#define xxsnstrm llsnstrm
#undef xxsnSTRM
#define xxsnSTRM llsnSTRM

// ---------------------------------------------------------------------------
// Count
// ---------------------------------------------------------------------------

#undef xlsncnt
#define xlsncnt llsncnt
#undef xlsnCNT
#define xlsnCNT llsnCNT

#undef xxsncnt
#define xxsncnt llsncnt
#undef xxsnCNT
#define xxsnCNT llsnCNT

// ---------------------------------------------------------------------------
// Concatenate
// ---------------------------------------------------------------------------

#undef xlsnncat
#define xlsnncat llsnncat
#undef xlsnncatc
#define xlsnncatc llsnncatc
#undef xlsnnCATC
#define xlsnnCATC llsnnCATC
#undef xlsnncatq
#define xlsnncatq llsnncatq
#undef xlsnncatqc
#define xlsnncatqc llsnncatqc
#undef xlsnnCATQC
#define xlsnnCATQC llsnnCATQC

#undef xxsnncat
#define xxsnncat llsnncat
#undef xxsnncatc
#define xxsnncatc llsnncatc
#undef xxsnnCATC
#define xxsnnCATC llsnnCATC
#undef xxsnncatq
#define xxsnncatq llsnncatq
#undef xxsnncatqc
#define xxsnncatqc llsnncatqc
#undef xxsnnCATQC
#define xxsnnCATQC llsnnCATQC

// ---------------------------------------------------------------------------
// Copy
// ---------------------------------------------------------------------------

#undef xlsnncpy
#define xlsnncpy llsnncpy
#undef xlsnncpyc
#define xlsnncpyc llsnncpyc
#undef xlsnnCPYC
#define xlsnnCPYC llsnnCPYC
#undef xlsnncpyq
#define xlsnncpyq llsnncpyq
#undef xlsnncpyqc
#define xlsnncpyqc llsnncpyqc
#undef xlsnnCPYQC
#define xlsnnCPYQC llsnnCPYQC

#undef xxsnncpy
#define xxsnncpy llsnncpy
#undef xxsnncpyc
#define xxsnncpyc llsnncpyc
#undef xxsnnCPYC
#define xxsnnCPYC llsnnCPYC
#undef xxsnncpyq
#define xxsnncpyq llsnncpyq
#undef xxsnncpyqc
#define xxsnncpyqc llsnncpyqc
#undef xxsnnCPYQC
#define xxsnnCPYQC llsnnCPYQC

// ---------------------------------------------------------------------------
// Replace  (case-sensitive replace not available for xl/xx -> ll)
// ---------------------------------------------------------------------------

#undef xlsnnREPLACE
#define xlsnnREPLACE llsnnREPLACE

#undef xxsnnREPLACE
#define xxsnnREPLACE llsnnREPLACE

// ---------------------------------------------------------------------------
// Pad / Repeat / Reverse
// ---------------------------------------------------------------------------

#undef xlsnnpad
#define xlsnnpad llsnnpad
#undef xlsnnrepeat
#define xlsnnrepeat llsnnrepeat
#undef xlsnnreverse
#define xlsnnreverse llsnnreverse

#undef xxsnnpad
#define xxsnnpad llsnnpad
#undef xxsnnrepeat
#define xxsnnrepeat llsnnrepeat
#undef xxsnnreverse
#define xxsnnreverse llsnnreverse

// ---------------------------------------------------------------------------
// Printf-style formatting  (Latin-only; no Unicode equivalent)
// ---------------------------------------------------------------------------

#undef xlsnprintf
#define xlsnprintf llsnprintf
#undef xlsnvprintf
#define xlsnvprintf llsnvprintf

// End of xlatin.h
