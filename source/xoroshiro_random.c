/* MIT License - Copyright (c) 2025 wallstop
 * Original C# implementation from: https://github.com/wallstop/unity-helpers
 * Converted to C for librandom project
 *
 * @BAKE cc -O2 -Wall -Wextra -std=c99 -o xoroshiro_random xoroshiro_random.c -lm @STOP
 */

#include "xoroshiro_random.h"

static inline uint64_t rotl(uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}

static void ensure_non_zero_state(xoroshiro_random_t *rng) {
    if ((rng->s0 | rng->s1) == 0) {
        rng->s0 = 0x9E3779B97F4A7C15ULL;
        rng->s1 = 0xD1B54A32D192ED03ULL;
    }
}

void xoroshiro_init(xoroshiro_random_t *rng, uint64_t seed1, uint64_t seed2) {
    rng->s0 = seed1;
    rng->s1 = seed2;
    ensure_non_zero_state(rng);
}

uint32_t xoroshiro_next_uint(xoroshiro_random_t *rng) {
    ensure_non_zero_state(rng);
    
    uint64_t s0 = rng->s0;
    uint64_t s1 = rng->s1;
    uint64_t result = s0 + s1;
    
    s1 ^= s0;
    rng->s0 = rotl(s0, 24) ^ s1 ^ (s1 << 16);
    rng->s1 = rotl(s1, 37);
    
    return (uint32_t)result;
}

uint64_t xoroshiro_next_ulong(xoroshiro_random_t *rng) {
    ensure_non_zero_state(rng);
    
    uint64_t s0 = rng->s0;
    uint64_t s1 = rng->s1;
    uint64_t result = s0 + s1;
    
    s1 ^= s0;
    rng->s0 = rotl(s0, 24) ^ s1 ^ (s1 << 16);
    rng->s1 = rotl(s1, 37);
    
    return result;
}

float xoroshiro_next_float(xoroshiro_random_t *rng) {
    uint32_t value = xoroshiro_next_uint(rng);
    return (value >> 8) * 0x1.0p-24f;
}

double xoroshiro_next_double(xoroshiro_random_t *rng) {
    uint64_t value = xoroshiro_next_ulong(rng);
    return (value >> 11) * 0x1.0p-53;
}

#ifdef XOROSHIRO_TEST_MAIN
#include <stdio.h>
#include <time.h>

int main(void) {
    xoroshiro_random_t rng;
    
    printf("XoroShiro Random C Implementation Test\n");
    printf("=======================================\n\n");
    
    xoroshiro_init(&rng, 42, 12345);
    
    printf("Testing with seeds 42, 12345:\n");
    printf("First 10 uint32_t values:\n");
    for (int i = 0; i < 10; i++) {
        printf("  %u\n", xoroshiro_next_uint(&rng));
    }
    
    printf("\nFirst 10 uint64_t values:\n");
    xoroshiro_init(&rng, 42, 12345);
    for (int i = 0; i < 10; i++) {
        printf("  %llu\n", (unsigned long long)xoroshiro_next_ulong(&rng));
    }
    
    printf("\nFirst 10 float values:\n");
    xoroshiro_init(&rng, 42, 12345);
    for (int i = 0; i < 10; i++) {
        printf("  %.6f\n", xoroshiro_next_float(&rng));
    }
    
    printf("\nPerformance test: generating 10 million uint64_t values...\n");
    xoroshiro_init(&rng, (uint64_t)time(NULL), (uint64_t)time(NULL) * 2);
    clock_t start = clock();
    uint64_t sum = 0;
    for (int i = 0; i < 10000000; i++) {
        sum += xoroshiro_next_ulong(&rng);
    }
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %.3f seconds (sum: %llu to prevent optimization)\n", elapsed, (unsigned long long)sum);
    
    return 0;
}
#endif
