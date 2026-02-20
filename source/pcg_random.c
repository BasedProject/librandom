/* MIT License - Copyright (c) 2025 wallstop
 * Original C# implementation from: https://github.com/wallstop/unity-helpers
 * Converted to C for librandom project
 *
 * @BAKE cc -O2 -Wall -Wextra -std=c99 -o pcg_random pcg_random.c -lm @STOP
 */

#include "pcg_random.h"

static inline uint64_t normalize_increment(uint64_t increment) {
    return (increment & 1ULL) == 0 ? increment | 1ULL : increment;
}

void pcg_init(pcg_random_t *rng, uint64_t seed) {
    rng->increment = normalize_increment(6554638469ULL);
    rng->state = seed;
    /* Warm up the state */
    pcg_next_uint(rng);
    rng->increment = normalize_increment((uint64_t)pcg_next_uint(rng) | ((uint64_t)pcg_next_uint(rng) << 32));
}

void pcg_init_full(pcg_random_t *rng, uint64_t state, uint64_t increment) {
    rng->state = state;
    rng->increment = normalize_increment(increment);
}

uint32_t pcg_next_uint(pcg_random_t *rng) {
    uint64_t old_state = rng->state;
    rng->state = old_state * 6364136223846793005ULL + rng->increment;
    uint32_t xor_shifted = (uint32_t)(((old_state >> 18) ^ old_state) >> 27);
    int rot = (int)(old_state >> 59);
    return (xor_shifted >> rot) | (xor_shifted << ((-rot) & 31));
}

uint64_t pcg_next_ulong(pcg_random_t *rng) {
    uint64_t high = (uint64_t)pcg_next_uint(rng) << 32;
    uint64_t low = (uint64_t)pcg_next_uint(rng);
    return high | low;
}

float pcg_next_float(pcg_random_t *rng) {
    uint32_t value = pcg_next_uint(rng);
    return (value >> 8) * 0x1.0p-24f;
}

double pcg_next_double(pcg_random_t *rng) {
    uint64_t value = pcg_next_ulong(rng);
    return (value >> 11) * 0x1.0p-53;
}

#ifdef PCG_TEST_MAIN
#include <stdio.h>
#include <time.h>

int main(void) {
    pcg_random_t rng;
    
    printf("PCG Random C Implementation Test\n");
    printf("=================================\n\n");
    
    pcg_init(&rng, 42);
    
    printf("Testing with seed 42:\n");
    printf("First 10 uint32_t values:\n");
    for (int i = 0; i < 10; i++) {
        printf("  %u\n", pcg_next_uint(&rng));
    }
    
    printf("\nFirst 10 uint64_t values:\n");
    pcg_init(&rng, 42);
    for (int i = 0; i < 10; i++) {
        printf("  %llu\n", (unsigned long long)pcg_next_ulong(&rng));
    }
    
    printf("\nFirst 10 float values:\n");
    pcg_init(&rng, 42);
    for (int i = 0; i < 10; i++) {
        printf("  %.6f\n", pcg_next_float(&rng));
    }
    
    printf("\nPerformance test: generating 10 million uint32_t values...\n");
    pcg_init(&rng, (uint64_t)time(NULL));
    clock_t start = clock();
    uint64_t sum = 0;
    for (int i = 0; i < 10000000; i++) {
        sum += pcg_next_uint(&rng);
    }
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %.3f seconds (sum: %llu to prevent optimization)\n", elapsed, (unsigned long long)sum);
    
    return 0;
}
#endif
