/* MIT License - Copyright (c) 2025 wallstop
 * Original C# implementation from: https://github.com/wallstop/unity-helpers
 * Converted to C for librandom project
 *
 * @BAKE cc -O2 -Wall -Wextra -std=c99 -o splitmix64 splitmix64.c -lm @STOP
 */

#include "splitmix64.h"

void splitmix64_init(splitmix64_t *rng, uint64_t seed) {
    rng->state = seed;
}

uint32_t splitmix64_next_uint(splitmix64_t *rng) {
    rng->state += 0x9E3779B97F4A7C15ULL;
    
    uint64_t z = rng->state;
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    z ^= z >> 31;
    
    return (uint32_t)z;
}

uint64_t splitmix64_next_ulong(splitmix64_t *rng) {
    rng->state += 0x9E3779B97F4A7C15ULL;
    
    uint64_t z = rng->state;
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    z ^= z >> 31;
    
    return z;
}

float splitmix64_next_float(splitmix64_t *rng) {
    uint32_t value = splitmix64_next_uint(rng);
    return (value >> 8) * 0x1.0p-24f;
}

double splitmix64_next_double(splitmix64_t *rng) {
    uint64_t value = splitmix64_next_ulong(rng);
    return (value >> 11) * 0x1.0p-53;
}

#ifdef SPLITMIX64_TEST_MAIN
#include <stdio.h>
#include <time.h>

int main(void) {
    splitmix64_t rng;
    
    printf("SplitMix64 C Implementation Test\n");
    printf("=================================\n\n");
    
    splitmix64_init(&rng, 42);
    
    printf("Testing with seed 42:\n");
    printf("First 10 uint32_t values:\n");
    for (int i = 0; i < 10; i++) {
        printf("  %u\n", splitmix64_next_uint(&rng));
    }
    
    printf("\nFirst 10 uint64_t values:\n");
    splitmix64_init(&rng, 42);
    for (int i = 0; i < 10; i++) {
        printf("  %llu\n", (unsigned long long)splitmix64_next_ulong(&rng));
    }
    
    printf("\nFirst 10 float values:\n");
    splitmix64_init(&rng, 42);
    for (int i = 0; i < 10; i++) {
        printf("  %.6f\n", splitmix64_next_float(&rng));
    }
    
    printf("\nPerformance test: generating 10 million uint64_t values...\n");
    splitmix64_init(&rng, (uint64_t)time(NULL));
    clock_t start = clock();
    uint64_t sum = 0;
    for (int i = 0; i < 10000000; i++) {
        sum += splitmix64_next_ulong(&rng);
    }
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %.3f seconds (sum: %llu to prevent optimization)\n", elapsed, (unsigned long long)sum);
    
    return 0;
}
#endif
