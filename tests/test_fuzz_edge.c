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
        llsnncpy(dst, sizeof(dst), src, len + 1, NULL);
        ulsnncpy(udst, sizeof(udst), src, len + 1, NULL);
        llsnncat(dst, sizeof(dst), src, len + 1, NULL);
        uusnncat(udst, sizeof(udst), usrc, len + 1, NULL);
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
    // Null pointers
    assert(LUB_PTR_ERR(llsnncpy(NULL, 8, buf, 8, NULL), 0));
    assert(LUB_PTR_ERR(ulsnncpy(NULL, 8, buf, 8, NULL), 0));
    // Overflows
    memset(buf, 'A', sizeof(buf));
    buf[7] = 0;
    llsnncpy(buf, sizeof(buf), (lchar_t*)"ABCDEFGH", 8, NULL); // Should not overflow
    assert(buf[7] == 0);
    // Invalid input
    assert(lcsnlen(NULL, 10) == 0);
    assert(ucsnlen(NULL, 10) == 0);
}

// Test: overlapping buffers
static void test_overlapping_buffers(void) {
    lchar_t buf[32] = "abcdefghijklmno";
    uchar_t ubuf[32];
    // Overlap: src ahead of dst
    strcpy((char*)buf, "abcdefghijklmno");
    llsnncpy(buf, sizeof(buf), buf + 2, 10, NULL);
    llsnncat(buf, sizeof(buf), buf + 2, 10, NULL);
    // Overlap: dst ahead of src
    strcpy((char*)buf, "abcdefghijklmno");
    llsnncpy(buf + 2, sizeof(buf) - 2, buf, 10, NULL);
    llsnncat(buf + 2, sizeof(buf) - 2, buf, 10, NULL);
    // Overlap: uu variants
    for (int i = 0; i < 16; ++i) ubuf[i] = (uchar_t)('a' + i);
    ubuf[16] = 0;
    uusnncpy(ubuf, sizeof(ubuf), ubuf + 2, 10, NULL);
    uusnncat(ubuf, sizeof(ubuf), ubuf + 2, 10, NULL);
    uusnncpy(ubuf + 2, sizeof(ubuf) - 2, ubuf, 10, NULL);
    uusnncat(ubuf + 2, sizeof(ubuf) - 2, ubuf, 10, NULL);

    // Cross-type overlap: expect pointer-encoded error.
    strcpy((char*)buf, "abcdefg");
    for (int i = 0; i < 8; ++i) ubuf[i] = (uchar_t)('a' + i);
    ubuf[7] = 0;
    assert(LUB_PTR_ERR(lusnncat(buf, sizeof(buf), ubuf, 7, NULL, '?'), 0));
    assert(LUB_PTR_ERR(ulsnncat(ubuf, sizeof(ubuf), buf, 7, NULL), 0));

    // Null-termination on error path.
    buf[0] = 'X'; buf[1] = 0;
    ubuf[0] = 'X'; ubuf[1] = 0;
    lusnncat(buf, sizeof(buf), ubuf, 7, NULL, '?');
    assert(buf[0] == 0);
    ulsnncat(ubuf, sizeof(ubuf), buf, 7, NULL);
    assert(ubuf[0] == 0);

    // Error substitution: Unicode->Latin with out-of-range
    uchar_t uni_bad[8] = {0x1234, 0x20, 0x7F, 0x100, 0};
    lchar_t out[8];
    lusnncat(out, sizeof(out), uni_bad, 8, NULL, '?');
    assert(out[0] == '?');
    assert(out[1] == 0);
}

void run_fuzz_edge_tests(void) {
    srand(42);
    fuzz_copy_append_cat();
    test_error_paths();
    test_overlapping_buffers();
    printf("Fuzz/edge-case tests passed.\n");
}
