/* MIT License - Copyright (c) 2025 wallstop
 * Original C# implementation from: https://github.com/wallstop/unity-helpers
 * Converted to C for librandom project
 *
 * @BAKE cc -O2 -Wall -Wextra -std=c99 -o wy_random wy_random.c -lm @STOP
 */

#include "wy_random.h"

#define WY_PRIME0 0xa0761d6478bd642fULL
#define WY_PRIME1 0xe7037ed1a0b428dbULL

/* Multiply two 64-bit integers and return hi and lo parts of 128-bit result */
static inline void multiply_64(uint64_t x, uint64_t y, uint64_t *hi, uint64_t *lo) {
    uint64_t x0 = (uint32_t)x;
    uint64_t x1 = x >> 32;
    uint64_t y0 = (uint32_t)y;
    uint64_t y1 = y >> 32;
    
    uint64_t p11 = x1 * y1;
    uint64_t p01 = x0 * y1;
    uint64_t p10 = x1 * y0;
    uint64_t p00 = x0 * y0;
    
    uint64_t middle = p10 + (p00 >> 32) + (uint32_t)p01;
    
    *lo = (middle << 32) | (uint32_t)p00;
    *hi = p11 + (middle >> 32) + (p01 >> 32);
}

/* MUM (MUltiply and Mix) operation */
static inline uint64_t mum(uint64_t x, uint64_t y) {
    uint64_t hi, lo;
    multiply_64(x, y, &hi, &lo);
    return hi ^ lo;
}

void wy_init(wy_random_t *rng, uint64_t seed) {
    rng->state = seed;
}

uint32_t wy_next_uint(wy_random_t *rng) {
    rng->state += WY_PRIME0;
    return (uint32_t)mum(rng->state ^ WY_PRIME1, rng->state);
}

uint64_t wy_next_ulong(wy_random_t *rng) {
    rng->state += WY_PRIME0;
    return mum(rng->state ^ WY_PRIME1, rng->state);
}

float wy_next_float(wy_random_t *rng) {
    uint32_t value = wy_next_uint(rng);
    return (value >> 8) * 0x1.0p-24f;
}

double wy_next_double(wy_random_t *rng) {
    uint64_t value = wy_next_ulong(rng);
    return (value >> 11) * 0x1.0p-53;
}

#ifdef WY_TEST_MAIN
#include <stdio.h>
#include <time.h>

int main(void) {
    wy_random_t rng;
    
    printf("WyRandom C Implementation Test\n");
    printf("===============================\n\n");
    
    wy_init(&rng, 42);
    
    printf("Testing with seed 42:\n");
    printf("First 10 uint32_t values:\n");
    for (int i = 0; i < 10; i++) {
        printf("  %u\n", wy_next_uint(&rng));
    }
    
    printf("\nFirst 10 uint64_t values:\n");
    wy_init(&rng, 42);
    for (int i = 0; i < 10; i++) {
        printf("  %llu\n", (unsigned long long)wy_next_ulong(&rng));
    }
    
    printf("\nFirst 10 float values:\n");
    wy_init(&rng, 42);
    for (int i = 0; i < 10; i++) {
        printf("  %.6f\n", wy_next_float(&rng));
    }
    
    printf("\nPerformance test: generating 10 million uint64_t values...\n");
    wy_init(&rng, (uint64_t)time(NULL));
    clock_t start = clock();
    uint64_t sum = 0;
    for (int i = 0; i < 10000000; i++) {
        sum += wy_next_ulong(&rng);
    }
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %.3f seconds (sum: %llu to prevent optimization)\n", elapsed, (unsigned long long)sum);
    
    return 0;
}
#endif
