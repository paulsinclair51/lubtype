/**
 * @file xlatin.h
 * @brief Auto-generated macro remapping: x macros -> Latin (l) equivalents
 *   for lubtype.h API.
 *
 * This file remaps x macros (polymorphic macros with x in their name)
 * to their Latin (l) equivalents for use with the lubtype.h API.
 * A .c file can reference the x macros for polymorphic code that
 * works for both Latin and Unicode by compiling once with xlatin.h
 * and once with xunicode.h.
*
* Mapping:
*   lx -> ll, xl -> ll, xu -> lu, ux -> ul, xx -> ll, x -> l
 *
 * To map x macros to Unicode (u) equivalents, include @ref xunicode.h
 * instead.
 *
 * @warning Do not edit manually. Regenerate with the code generator if
 * lubtype.h changes.
 * @note To regenerate: see scripts/gen_xlatin.py (or the appropriate
 * generator script).
 *
 * @see xunicode.h
 * @see lubtype.h
 * @see scripts/gen_xlatin.py
 *
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
*/

#undef LUB_XTYPE
#define LUB_XTYPE l

#undef MAX_XCHAR
#define MAX_XCHAR MAX_LCHAR
#undef MAX_XSTRLEN
#define MAX_XSTRLEN MAX_LSTRLEN

#undef xstr_t
#define xstr_t lstr_t
#undef xcstr_t
#define xcstr_t lcstr_t
#undef xchar_t
#define xchar_t lchar_t

#undef isxalpha
#define isxalpha isalpha
#undef isxdigit
#define isxdigit isdigit
#undef isxalnum
#define isxalnum isalnum
#undef isxupper
#define isxupper isupper
#undef isxlower
#define isxlower islower
#undef isxcntrl
#define isxcntrl iscntrl
#undef isxprint
#define isxprint isprint
#undef isxgraph
#define isxgraph islgraph
#undef isxpunct
#define isxpunct islpunct
#undef isxblank
#define isxblank islblank
#undef isxspace
#define isxspace islspace

#undef xcsnlen
#define xcsnlen lcsnlen
#undef xcslen
#define xcslen lcslen

#undef xlsnapp
#define xlsnapp llsnapp
#undef xlsapp
#define xlsapp llsapp
#undef xlsnappc
#define xlsnappc llsnappc
#undef xlsappc
#define xlsappc llsappc
#undef xlsnAPPC
#define xlsnAPPC llsnAPPC
#undef xlsAPPC
#define xlsAPPC llsAPPC
#undef xlsncpy
#define xlsncpy llsncpy
#undef xlscpy
#define xlscpy llscpy
#undef xlsncpyc
#define xlsncpyc llsncpyc
#undef xlscpyc
#define xlscpyc llscpyc
#undef xlsnCPYC
#define xlsnCPYC llsnCPYC
#undef xlsCPYC
#define xlsCPYC llsCPYC
#undef xlsncat
#define xlsncat llsncat
#undef xlscat
#define xlscat llscat
#undef xlsncatc
#define xlsncatc llsncatc
#undef xlscatc
#define xlscatc llscatc
#undef xlsnCATC
#define xlsnCATC llsnCATC
#undef xlsCATC
#define xlsCATC llsCATC

#undef xlsntrim
#define xlsntrim llsntrim
#undef xlstrim
#define xlstrim llstrim
#undef xlsnrev
#define xlsnrev llsnrev
#undef xlsrev
#define xlsrev llsrev
#undef xlsnpad
#define xlsnpad llsnpad
#undef xlspad
#define xlspad llspad
#undef xlsnreplace
#define xlsnreplace llsnreplace
#undef xlsreplace
#define xlsreplace llsreplace
#undef xlsnREPLACE
#define xlsnREPLACE llsnREPLACE
#undef xlsREPLACE
#define xlsREPLACE llsREPLACE
#undef xlsnrepeat
#define xlsnrepeat llsnrepeat
#undef xlsrepeat
#define xlsrepeat llsrepeat
#undef xlsnsub
#define xlsnsub llsnsub
#undef xlssub
#define xlssub llssub
#undef xlsnsplit
#define xlsnsplit llsnsplit
#undef xlsplit
#define xlsplit llsplit
#undef xlsnvprintf
#define xlsnvprintf llsnvprintf
#undef xlsnprintf
#define xlsnprintf llsnprintf
#undef xlsvprintf
#define xlsvprintf llsvprintf
#undef xlsprintf
#define xlsprintf llsprintf

#undef xlsncmp
#define xlsncmp llsncmp
#undef xlscmp
#define xlscmp llscmp
#undef xlsnCMP
#define xlsnCMP llsnCMP
#undef xlsCMP
#define xlsCMP llsCMP
#undef xlsnpfx
#define xlsnpfx llsnpfx
#undef xlspfx
#define xlspfx llspfx
#undef xlsnPFX
#define xlsnPFX llsnPFX
#undef xlsPFX
#define xlsPFX llsPFX
#undef xlsnsfx
#define xlsnsfx llsnsfx
#undef xlssfx
#define xlssfx llssfx
#undef xlsnSFX
#define xlsnSFX llsnSFX
#undef xlsSFX
#define xlsSFX llsSFX

#undef xlsnchrm
#define xlsnchrm llsnchrm
#undef xlsnchr
#define xlsnchr llsnchr
#undef xlschr
#define xlschr llschr
#undef xlsnCHRM
#define xlsnCHRM llsnCHRM
#undef xlsnCHR
#define xlsnCHR llsnCHR
#undef xlsCHR
#define xlsCHR llsCHR
#undef xlschrm
#define xlschrm llschrm
#undef xlsCHRM
#define xlsCHRM llsCHRM

#undef xlsnrchrm
#define xlsnrchrm llsnrchrm
#undef xlsnrchr
#define xlsnrchr llsnrchr
#undef xlsrchr
#define xlsrchr llsrchr
#undef xlsrchrm
#define xlsrchrm llsrchrm
#undef xlsnRCHRM
#define xlsnRCHRM llsnRCHRM
#undef xlsnRCHR
#define xlsnRCHR llsnRCHR
#undef xlsRCHR
#define xlsRCHR llsRCHR
#undef xlsRCHRM
#define xlsRCHRM llsRCHRM

#undef xlsnpbrkm
#define xlsnpbrkm llsnpbrkm
#undef xlsnpbrkm
#define xlsnpbrkm llsnpbrkm
#undef xlsnpbrk
#define xlsnpbrk llsnpbrk

#undef xlspbrk
#define xlspbrk llspbrk
#undef xlsnPBRKM
#define xlsnPBRKM llsnPBRKM
#undef xlsPBRKM
#define xlsPBRKM llsPBRKM
#undef xlsnPBRK
#define xlsnPBRK llsnPBRK
#undef xlsPBRK
#define xlsPBRK llsPBRK

#undef xlsnrpbrkm
#define xlsnrpbrkm llsnrpbrkm
#undef xlsrpbrkm
#define xlsrpbrkm llsrpbrkm
#undef xlsnrpbrk
#define xlsnrpbrk llsnrpbrk
#undef xlsrpbrk
#define xlsrpbrk llsrpbrk
#undef xlsnRPBRKM
#define xlsnRPBRKM llsnRPBRKM
#undef xlsRPBRKM
#define xlsRPBRKM llsRPBRKM
#undef xlsnRPBRK
#define xlsnRPBRK llsnRPBRK
#undef xlsRPBRK
#define xlsRPBRK llsRPBRK

#undef xlsnspn
#define xlsnspn llsnspn
#undef xlsspn
#define xlsspn llsspn
#undef xlsnSPN
#define xlsnSPN llsnSPN
#undef xlsSPN
#define xlsSPN llsSPN
#undef xlsncspn
#define xlsncspn llsncspn
#undef xlscspn
#define xlscspn llscspn
#undef xlsnCSPN
#define xlsnCSPN llsnCSPN
#undef xlsCSPN
#define xlsCSPN llsCSPN
#undef xlsncnt
#define xlsncnt llsncnt
#undef xlscnt
#define xlscnt llscnt
#undef xlsnCNT
#define xlsnCNT llsnCNT
#undef xlsCNT
#define xlsCNT llsCNT

#undef xlsnstrm
#define xlsnstrm llsnstrm
#undef xlsstrm
#define xlsstrm llsstrm
#undef xlsnstr
#define xlsnstr llsnstr
#undef xlsstr
#define xlsstr llsstr
#undef xlsnSTRM
#define xlsnSTRM llsnSTRM
#undef xlsSTRM
#define xlsSTRM llsSTRM
#undef xlsnSTR
#define xlsnSTR llsnSTR
#undef xlsSTR
#define xlsSTR llsSTR

#undef xlsnrstrm
#define xlsnrstrm llsnrstrm
#undef xlsrstrm
#define xlsrstrm llsrstrm
#undef xlsnrstr
#define xlsnrstr llsnrstr
#undef xlsrstr
#define xlsrstr llsrstr
#undef xlsnRSTRM
#define xlsnRSTRM llsnRSTRM
#undef xlsRSTRM
#define xlsRSTRM llsRSTRM
#undef xlsnRSTR
#define xlsnRSTR llsnRSTR
#undef xlsRSTR
#define xlsRSTR llsRSTR

#undef xlsnsubcnt
#define xlsnsubcnt llsnsubcnt
#undef xlssubcnt
#define xlssubcnt llssubcnt
#undef xlsnSUBCNT
#define xlsnSUBCNT llsnSUBCNT
#undef xlsSUBCNT
#define xlsSUBCNT llsSUBCNT

// End of xlatin.h
