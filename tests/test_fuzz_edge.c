/**
 * @file test_fuzz_edge.c
 * @brief Fuzz and edge-case tests for lubtype.h string operations.
 * @copyright Copyright (c) 2026 paulsinclair51
 * SPDX-License-Identifier: MIT
 * For license details, see the LICENSE file in the project root.
 */

// Fuzz and edge-case tests for lubtype.h string operations
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lubtype.h"

// Helper: fill buffer with random bytes
/**
 * @brief Fill a buffer with random lchar_t values.
 */
static void fill_random_lchar(lchar_t *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) buf[i] = (lchar_t)(rand() % 256);
}

/**
 * @brief Fill a buffer with random uchar_t values.
 */
static void fill_random_uchar(uchar_t *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) buf[i] = (uchar_t)(rand() % 65536);
}

// Fuzz test: random copy/append/cat with bounds
/**
 * @brief Fuzz test: random copy/append/cat with bounds.
 *
 * Fills buffers with random data and tests copy/append/cat functions for null-termination and correctness.
 */
static void fuzz_copy_append_cat(void) {
    lchar_t src[128], dst[128];
    uchar_t usrc[128], udst[128];
    for (int i = 0; i < 1000; ++i) {
        size_t len = rand() % 127;
        fill_random_lchar(src, len); src[len] = 0;
        fill_random_uchar(usrc, len); usrc[len] = 0;
        memset(dst, 0, sizeof(dst));
        memset(udst, 0, sizeof(udst));
        llsncpy(dst, src, len+1);
        ulsncpy(udst, src, len+1);
        llsnapp(dst, src, len+1);
        ulsnapp(udst, src, len+1);
        // Check null-termination
        assert(dst[len] == 0);
        assert(udst[len] == 0);
    }
}

// Test: error paths (null, overflow, invalid)
/**
 * @brief Test error paths (null, overflow, invalid input).
 */
static void test_error_paths(void) {
    lchar_t buf[8] = {0};
    uchar_t ubuf[8] = {0};
    // Null pointers
    assert(llsncpy(NULL, NULL, 0) == NULL);
    assert(ulsncpy(NULL, NULL, 0) == NULL);
    // Overflows
    memset(buf, 'A', sizeof(buf));
    buf[7] = 0;
    llsncpy(buf, (lchar_t*)"ABCDEFGH", 8); // Should not overflow
    assert(buf[7] == 0);
    // Invalid input
    assert(lcsnlen(NULL, 10) == MISSING_STR_TERMINATOR);
    assert(ucsnlen(NULL, 10) == MISSING_STR_TERMINATOR);
}

// Test: overlapping buffers
static void test_overlapping_buffers(void) {
    lchar_t buf[32] = "abcdefghijklmno";
    uchar_t ubuf[32];
    lchar_t latin_src[32] = "abcdefg";
    uchar_t uni_src[32] = {L'a',L'b',L'c',L'd',L'e',L'f',L'g',0};
    // Overlap: src ahead of dst (llsncpy, llsnapp, llsnappc, llsnAPPC)
    strcpy((char*)buf, "abcdefghijklmno");
    llsncpy(buf, buf+2, 10);
    llsnapp(buf, buf+2, 10);
    llsnappc(buf, buf+2, 10);
    llsnAPPC(buf, buf+2, 10);
    // Overlap: dst ahead of src
    strcpy((char*)buf, "abcdefghijklmno");
    llsncpy(buf+2, buf, 10);
    llsnapp(buf+2, buf, 10);
    llsnappc(buf+2, buf, 10);
    llsnAPPC(buf+2, buf, 10);
    // Overlap: uu variants
    for (int i = 0; i < 16; ++i) ubuf[i] = (uchar_t)('a'+i); ubuf[16]=0;
    uusncpy(ubuf, ubuf+2, 10);
    uusnapp(ubuf, ubuf+2, 10);
    uusnappc(ubuf, ubuf+2, 10);
    uusnAPPC(ubuf, ubuf+2, 10);
    uusncpy(ubuf+2, ubuf, 10);
    uusnapp(ubuf+2, ubuf, 10);
    uusnappc(ubuf+2, ubuf, 10);
    uusnAPPC(ubuf+2, ubuf, 10);
    // Cross-type overlap: should return NULL and not corrupt
    strcpy((char*)buf, "abcdefg");
    for (int i = 0; i < 8; ++i) ubuf[i] = (uchar_t)('a'+i); ubuf[7]=0;
    assert(lusnapp(buf, (ucstr_t)buf, 7, '?') == NULL);
    assert(ulsnapp(ubuf, (lcstr_t)ubuf, 7) == NULL);
    assert(lusnappc(buf, (ucstr_t)buf, 7, '?') == NULL);
    assert(ulsnappc(ubuf, (lcstr_t)ubuf, 7) == NULL);
    assert(lusnAPPC(buf, (ucstr_t)buf, 7, '?') == NULL);
    assert(ulsnAPPC(ubuf, (lcstr_t)ubuf, 7) == NULL);
    // Null-termination on error: cross-type overlap
    buf[0] = 'X'; buf[1] = 0;
    ubuf[0] = 'X'; ubuf[1] = 0;
    lusnapp(buf, (ucstr_t)buf, 7, '?');
    assert(buf[0] == 0);
    ulsnapp(ubuf, (lcstr_t)ubuf, 7);
    assert(ubuf[0] == 0);
    // Error substitution: Unicode->Latin with out-of-range
    uchar_t uni_bad[8] = {0x1234, 0x20, 0x7F, 0x100, 0};
    lchar_t out[8];
    lusnapp(out, uni_bad, 8, '?');
    assert(out[0] == '?');
    assert(out[1] == 0x20);
    assert(out[2] == 0x7F);
    assert(out[3] == '?');
    // Case-mapping variants: overlap and error
    strcpy((char*)buf, "AbCdEfG");
    llsnappc(buf, buf+2, 5);
    llsnAPPC(buf, buf+2, 5);
    // Null-termination on error for all variants
    buf[0] = 'X'; buf[1] = 0;
    llsnapp(NULL, NULL, 0);
    assert(buf[0] == 'X'); // untouched
    llsnapp(buf, NULL, 0);
    assert(buf[0] == 0);
    llsnapp(NULL, buf, 0);
    // Should not crash
}

int main(void) {
    srand(42);
    fuzz_copy_append_cat();
    test_error_paths();
    test_overlapping_buffers();
    printf("Fuzz/edge-case tests passed.\n");
    return 0;

}
