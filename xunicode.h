/**
 * @file xunicode.h
 * @brief Polymorphic macro remapping: x macros -> Unicode (u) equivalents
 *   for lubtype.h API.
 *
 * This file remaps x macros (polymorphic macros with x in their name)
 * to their Unicode (u) equivalents for use with the lubtype.h API.
 * A .c file can reference the x macros for polymorphic code that
 * works for both Latin and Unicode by compiling once with xlatin.h
 * and once with xunicode.h.
 *
 * Mapping (x resolved to u):
 *
 *   xl -> ul  (x-target <- l-source becomes u-target <- l-source)
 *   xx -> uu  (x-target <- x-source becomes u-target <- u-source)
 *    x -> u   (x-type becomes u-type)
 *
 * To map x macros to Latin (l) equivalents, include @ref xlatin.h instead.
 *
 * @note Only macros with corresponding functions defined in lubtype.h
 *       are provided. Not all encoding directions are available for
 *       every operation; see lubtype.h for details.
 *
 * @see xlatin.h
 * @see lubtype.h
 *
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
*/

#undef LUB_XTYPE
#define LUB_XTYPE u

// ---------------------------------------------------------------------------
// Limits
// ---------------------------------------------------------------------------

#undef LUB_MAX_XCHAR
#define LUB_MAX_XCHAR LUB_MAX_UCHAR
#undef LUB_MAX_XSTRLEN
#define LUB_MAX_XSTRLEN LUB_MAX_USTRLEN

// ---------------------------------------------------------------------------
// Character classification  (x -> u)
// ---------------------------------------------------------------------------

#undef isxalpha
#define isxalpha isualpha
#undef isxdigit
#define isxdigit isudigit
#undef isxalnum
#define isxalnum isualnum
#undef isxupper
#define isxupper isuupper
#undef isxlower
#define isxlower isulower
#undef isxcntrl
#define isxcntrl isucntrl
#undef isxprint
#define isxprint isuprint
#undef isxgraph
#define isxgraph isugraph
#undef isxpunct
#define isxpunct isupunct
#undef isxblank
#define isxblank isublank
#undef isxspace
#define isxspace isuspace
#undef isxhexdigit
#define isxhexdigit isuhexdigit

// ---------------------------------------------------------------------------
// Hex digit -> int  (x -> u)
// ---------------------------------------------------------------------------

#undef ixhexdigit
#define ixhexdigit iuhexdigit

// ---------------------------------------------------------------------------
// String length  (x -> u)
// ---------------------------------------------------------------------------

#undef xcsnlen
#define xcsnlen ucsnlen

// ---------------------------------------------------------------------------
// Character case transforms
//   xl -> ul  (u-target <- l-source)
//   xx -> uu  (u-target <- u-source)
// ---------------------------------------------------------------------------

#undef xltocase
#define xltocase ultocase
#undef xltolower
#define xltolower ultolower
#undef xltoupper
#define xltoupper ultoupper

#undef xxtocase
#define xxtocase uutocase
#undef xxtolower
#define xxtolower uutolower
#undef xxtoupper
#define xxtoupper uutoupper

// ---------------------------------------------------------------------------
// Compare
//   xl -> ul  (u s1 vs l s2)
//   xx -> uu  (u s1 vs u s2)
// ---------------------------------------------------------------------------

#undef xlsncmp
#define xlsncmp ulsncmp
#undef xlsnCMP
#define xlsnCMP ulsnCMP
#undef xlsnfxdcmp
#define xlsnfxdcmp ulsnfxdcmp
#undef xlsnFXDCMP
#define xlsnFXDCMP ulsnFXDCMP
#undef xlsnpfxcmp
#define xlsnpfxcmp ulsnpfxcmp
#undef xlsnPFXCMP
#define xlsnPFXCMP ulsnPFXCMP
#undef xlsnsfxcmp
#define xlsnsfxcmp ulsnsfxcmp
#undef xlsnSFXCMP
#define xlsnSFXCMP ulsnSFXCMP

#undef xxsncmp
#define xxsncmp uusncmp
#undef xxsnCMP
#define xxsnCMP uusnCMP
#undef xxsnfxdcmp
#define xxsnfxdcmp uusnfxdcmp
#undef xxsnFXDCMP
#define xxsnFXDCMP uusnFXDCMP
#undef xxsnpfxcmp
#define xxsnpfxcmp uusnpfxcmp
#undef xxsnPFXCMP
#define xxsnPFXCMP uusnPFXCMP
#undef xxsnsfxcmp
#define xxsnsfxcmp uusnsfxcmp
#undef xxsnSFXCMP
#define xxsnSFXCMP uusnSFXCMP

// ---------------------------------------------------------------------------
// Search
// ---------------------------------------------------------------------------

#undef xlsnstrm
#define xlsnstrm ulsnstrm
#undef xlsnSTRM
#define xlsnSTRM ulsnSTRM

#undef xxsnstrm
#define xxsnstrm uusnstrm
#undef xxsnSTRM
#define xxsnSTRM uusnSTRM

// ---------------------------------------------------------------------------
// Count
// ---------------------------------------------------------------------------

#undef xlsncnt
#define xlsncnt ulsncnt
#undef xlsnCNT
#define xlsnCNT ulsnCNT

#undef xxsncnt
#define xxsncnt uusncnt
#undef xxsnCNT
#define xxsnCNT uusnCNT

// ---------------------------------------------------------------------------
// Concatenate
// ---------------------------------------------------------------------------

#undef xlsnncat
#define xlsnncat ulsnncat
#undef xlsnncatc
#define xlsnncatc ulsnncatc
#undef xlsnnCATC
#define xlsnnCATC ulsnnCATC
#undef xlsnncatq
#define xlsnncatq ulsnncatq
#undef xlsnncatqc
#define xlsnncatqc ulsnncatqc
#undef xlsnnCATQC
#define xlsnnCATQC ulsnnCATQC

#undef xxsnncat
#define xxsnncat uusnncat
#undef xxsnncatc
#define xxsnncatc uusnncatc
#undef xxsnnCATC
#define xxsnnCATC uusnnCATC
#undef xxsnncatq
#define xxsnncatq uusnncatq
#undef xxsnncatqc
#define xxsnncatqc uusnncatqc
#undef xxsnnCATQC
#define xxsnnCATQC uusnnCATQC

// ---------------------------------------------------------------------------
// Copy
// ---------------------------------------------------------------------------

#undef xlsnncpy
#define xlsnncpy ulsnncpy
#undef xlsnncpyc
#define xlsnncpyc ulsnncpyc
#undef xlsnnCPYC
#define xlsnnCPYC ulsnnCPYC
#undef xlsnncpyq
#define xlsnncpyq ulsnncpyq
#undef xlsnncpyqc
#define xlsnncpyqc ulsnncpyqc
#undef xlsnnCPYQC
#define xlsnnCPYQC ulsnnCPYQC

#undef xxsnncpy
#define xxsnncpy uusnncpy
#undef xxsnncpyc
#define xxsnncpyc uusnncpyc
#undef xxsnnCPYC
#define xxsnnCPYC uusnnCPYC
#undef xxsnncpyq
#define xxsnncpyq uusnncpyq
#undef xxsnncpyqc
#define xxsnncpyqc uusnncpyqc
#undef xxsnnCPYQC
#define xxsnnCPYQC uusnnCPYQC

// ---------------------------------------------------------------------------
// Replace
//   xl -> ul  (u-target <- l-source, case-insensitive variant)
//   xx -> uu  (u-target <- u-source, both case variants)
// ---------------------------------------------------------------------------

#undef xlsnnREPLACE
#define xlsnnREPLACE ulsnnREPLACE

#undef xxsnnreplace
#define xxsnnreplace uusnnreplace
#undef xxsnnREPLACE
#define xxsnnREPLACE uusnnREPLACE

// ---------------------------------------------------------------------------
// Pad / Repeat / Reverse
// ---------------------------------------------------------------------------

#undef xlsnnpad
#define xlsnnpad ulsnnpad
#undef xlsnnrepeat
#define xlsnnrepeat ulsnnrepeat
#undef xlsnnreverse
#define xlsnnreverse ulsnnreverse

#undef xxsnnpad
#define xxsnnpad uusnnpad
#undef xxsnnrepeat
#define xxsnnrepeat uusnnrepeat
#undef xxsnnreverse
#define xxsnnreverse uusnnreverse

// End of xunicode.h
