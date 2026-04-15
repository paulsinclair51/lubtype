/**
 * @file xunicode.h
 * @brief Auto-generated macro remapping: x macros -> Unicode (u) equivalents
 *   for lubtype.h API.
 *
 * This file remaps x macros (polymorphic macros with x in their name)
 * to their Unicode (u) equivalents for use with the lubtype.h API.
  * A .c file can reference the x macros for polymorphic code that
 * works for both Latin and Unicode by compiling once with xlatin.h
 * and once with xunicode.h.
 *
 * Mapping:
 *   lx -> lu, xl -> ul, xu -> uu, ux -> uu, xx -> uu, x -> u
 *
 * To map x macros to Latin (l) equivalents, include @ref xlatin.h instead.
 *
 * @warning Do not edit manually. Regenerate with the code generator if
 * lubtype.h changes.
 * @note To regenerate: see scripts/gen_xunicode.py (or the appropriate
 * generator script).
 *
 * @see xlatin.h
 * @see lubtype.h
 * @see scripts/gen_xunicode.py
 *
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
*/

#undef LUB_XTYPE
#define LUB_XTYPE u

#undef MAX_XCHAR
#define MAX_XCHAR MAX_UCHAR

#undef MAX_XSTRLEN
#define MAX_XSTRLEN MAX_USTRLEN

#undef xstr_t
#define xstr_t ustr_t
#undef xcstr_t
#define xcstr_t ucstr_t
#undef xchar_t
#define xchar_t uchar_t

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
#define isxcntrl isucontrol
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


#undef xcsnlen
#define xcsnlen ucsnlen
#undef xcslen
#define xcslen ucslen
#undef xlsnapp
#define xlsnapp uusnapp
#undef xlsapp
#define xlsapp uusapp
#undef xlsnappc
#define xlsnappc uusnappc
#undef xlsappc
#define xlsappc uusappc
#undef xlsnAPPC
#define xlsnAPPC uusnAPPC
#undef xlsAPPC
#define xlsAPPC uusAPPC
#undef xlsncpy
#define xlsncpy uusncpy
#undef xlscpy
#define xlscpy uuscpy
#undef xlsncpyc
#define xlsncpyc uusncpyc
#undef xlscpyc
#define xlscpyc uuscpyc
#undef xlsnCPYC
#define xlsnCPYC uusnCPYC
#undef xlsCPYC
#define xlsCPYC uusCPYC
#undef xlsncat
#define xlsncat uusncat
#undef xlscat
#define xlscat uuscat
#undef xlsncatc
#define xlsncatc uusncatc
#undef xlscatc
#define xlscatc uuscatc
#undef xlsnCATC
#define xlsnCATC uusnCATC
#undef xlsCATC
#define xlsCATC uusCATC
#undef xlsntrim
#define xlsntrim uusntrim
#undef xlstrim
#define xlstrim uustrim
#undef xlsnrev
#define xlsnrev uusnrev
#undef xlsrev
#define xlsrev uusrev
#undef xlsnpad
#define xlsnpad uusnpad
#undef xlspad
#define xlspad uuspadd
#undef xlsnreplace
#define xlsnreplace uusnreplace
#undef xlsreplace
#define xlsreplace uusreplace
#undef xlsnREPLACE
#define xlsnREPLACE uusnREPLACE
#undef xlsREPLACE
#define xlsREPLACE uusREPLACE
#undef xlsnrepeat
#define xlsnrepeat uusnrepeat
#undef xlsrepeat
#define xlsrepeat uusrepeat
#undef xlsnsub
#define xlsnsub uusnsub
#undef xlssub
#define xlssub uussub
#undef xlsnsplit
#define xlsnsplit uusnsplit
#undef xlsplit
#define xlsplit uusplit
#undef xlsnvprintf
#define xlsnvprintf uusnvprintf
#undef xlsnprintf
#define xlsnprintf uusnprintf
#undef xlsvprintf
#define xlsvprintf uusvprintf
#undef xlsprintf
#define xlsprintf uusprintf
#undef xlsncmp
#define xlsncmp uusncmp
#undef xlscmp
#define xlscmp uuscpm
#undef xlsnCMP
#define xlsnCMP uusnCMP
#undef xlsCMP
#define xlsCMP uusCMP
#undef xlsnpfx
#define xlsnpfx uusnpfx
#undef xlspfx
#define xlspfx uuspfx
#undef xlsnPFX
#define xlsnPFX uusnPFX
#undef xlsPFX
#define xlsPFX uusPFX
#undef xlsnsfx
#define xlsnsfx uusnsfx
#undef xlssfx
#define xlssfx uussfx
#undef xlsnSFX
#define xlsnSFX uusnSFX
#undef xlsSFX
#define xlsSFX uusSFX
#undef xlsnchrm
#define xlsnchrm uusnchrm
#undef xlsnchr
#define xlsnchr uusnchr
#undef xlschr
#define xlschr uuschr
#undef xlsnCHRM
#define xlsnCHRM uusnCHRM
#undef xlsnCHR
#define xlsnCHR uusnCHR
#undef xlsCHR
#define xlsCHR uusCHR
#undef xlschrm
#define xlschrm uuschrm
#undef xlsCHRM
#define xlsCHRM uusCHRM
#undef xlsnrchrm
#define xlsnrchrm uusnrchrm
#undef xlsnrchr
#define xlsnrchr uusnrchr
#undef xlsrchr
#define xlsrchr uusrchr
#undef xlsrchrm
#define xlsrchrm uusrchrm
#undef xlsnRCHRM
#define xlsnRCHRM uusnRCHRM
#undef xlsnRCHR
#define xlsnRCHR uusnRCHR
#undef xlsRCHR
#define xlsRCHR uusRCHR
#undef xlsRCHRM
#define xlsRCHRM uusRCHRM
#undef xlsnpbrkm
#define xlsnpbrkm uusnpbrkm
#undef xlsnpbrkm
#define xlsnpbrkm uusnpbrkm
#undef xlsnpbrk
#define xlsnpbrk uusnpbrk
#undef xlspbrk
#define xlspbrk uuspbrk
#undef xlsnPBRKM
#define xlsnPBRKM uusnPBRKM
#undef xlsPBRKM
#define xlsPBRKM uusPBRKM
#undef xlsnPBRK
#define xlsnPBRK uusnPBRK
#undef xlsPBRK
#define xlsPBRK uusPBRK
#undef xlsnrpbrkm
#define xlsnrpbrkm uusnrpbrkm
#undef xlsrpbrkm
#define xlsrpbrkm uusrpbrkm
#undef xlsnrpbrk
#define xlsnrpbrk uusnrpbrk
#undef xlsrpbrk
#define xlsrpbrk uusrpbrk
#undef xlsnRPBRKM
#define xlsnRPBRKM uusnRPBRKM
#undef xlsRPBRKM
#define xlsRPBRKM uusRPBRKM
#undef xlsnRPBRK
#define xlsnRPBRK uusnRPBRK
#undef xlsRPBRK
#define xlsRPBRK uusRPBRK
#undef xlsnspn
#define xlsnspn uusnspn
#undef xlsspn
#define xlsspn uussppn
#undef xlsnSPN
#define xlsnSPN uusnSPN
#undef xlsSPN
#define xlsSPN uusSPN
#undef xlsncspn
#define xlsncspn uusncspn
#undef xlscspn
#define xlscspn uuscspn
#undef xlsnCSPN
#define xlsnCSPN uusnCSPN
#undef xlsCSPN
#define xlsCSPN uusCSPN
#undef xlsncnt
#define xlsncnt uusncnt
#undef xlscnt
#define xlscnt uuscnt
#undef xlsnCNT
#define xlsnCNT uusnCNT
#undef xlsCNT
#define xlsCNT uusCNT
#undef xlsnstrm
#define xlsnstrm uusnstrm
#undef xlsstrm
#define xlsstrm uusstrm
#undef xlsnstr
#define xlsnstr uusnstr
#undef xlsstr
#define xlsstr uusstr
#undef xlsnSTRM
#define xlsnSTRM uusnSTRM
#undef xlsSTRM
#define xlsSTRM uusSTRM
#undef xlsnSTR
#define xlsnSTR uusnSTR
#undef xlsSTR
#define xlsSTR uusSTR
#undef xlsnrstrm
#define xlsnrstrm uusnrstrm
#undef xlsrstrm
#define xlsrstrm uusrstrm
#undef xlsnrstr
#define xlsnrstr uusnrstr
#undef xlsrstr
#define xlsrstr uusrstr
#undef xlsnRSTRM
#define xlsnRSTRM uusnRSTRM
#undef xlsRSTRM
#define xlsRSTRM uusRSTRM
#undef xlsnRSTR
#define xlsnRSTR uusnRSTR
#undef xlsRSTR
#define xlsRSTR uusRSTR
#undef xlsnsubcnt
#define xlsnsubcnt uusnsubcnt
#undef xlssubcnt
#define xlssubcnt uussubcnt
#undef xlsnSUBCNT
#define xlsnSUBCNT uusnSUBCNT
#undef xlsSUBCNT
#define xlsSUBCNT uusSUBCNT
// End of xunicode.h
