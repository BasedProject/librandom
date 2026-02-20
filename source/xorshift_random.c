/* MIT License - Copyright (c) 2025 wallstop
 * Original C# implementation from: https://github.com/wallstop/unity-helpers
 * Converted to C for librandom project
 *
 * @BAKE cc -O2 -Wall -Wextra -std=c99 -o xorshift_random xorshift_random.c -lm @STOP
 */

#include "xorshift_random.h"

static uint32_t normalize_state(uint32_t state) {
    return state != 0 ? state : XORSHIFT_DEFAULT_STATE;
}

void xorshift_init(xorshift_random_t *rng, uint32_t seed) {
    rng->state = normalize_state(seed);
}

uint32_t xorshift_next_uint(xorshift_random_t *rng) {
    rng->state = normalize_state(rng->state);
    rng->state ^= rng->state << 13;
    rng->state ^= rng->state >> 17;
    rng->state ^= rng->state << 5;
    return rng->state;
}

float xorshift_next_float(xorshift_random_t *rng) {
    uint32_t value = xorshift_next_uint(rng);
    return (value >> 8) * 0x1.0p-24f;
}

double xorshift_next_double(xorshift_random_t *rng) {
    uint32_t high = xorshift_next_uint(rng);
    uint32_t low = xorshift_next_uint(rng);
    uint64_t combined = ((uint64_t)high << 32) | low;
    return (combined >> 11) * 0x1.0p-53;
}

#ifdef XORSHIFT_TEST_MAIN
#include <stdio.h>
#include <time.h>

int main(void) {
    xorshift_random_t rng;
    
    printf("XorShift Random C Implementation Test\n");
    printf("=====================================\n\n");
    
    xorshift_init(&rng, 42);
    
    printf("Testing with seed 42:\n");
    printf("First 10 uint32_t values:\n");
    for (int i = 0; i < 10; i++) {
        printf("  %u\n", xorshift_next_uint(&rng));
    }
    
    printf("\nFirst 10 float values:\n");
    xorshift_init(&rng, 42);
    for (int i = 0; i < 10; i++) {
        printf("  %.6f\n", xorshift_next_float(&rng));
    }
    
    printf("\nPerformance test: generating 10 million uint32_t values...\n");
    xorshift_init(&rng, (uint32_t)time(NULL));
    clock_t start = clock();
    uint64_t sum = 0;
    for (int i = 0; i < 10000000; i++) {
        sum += xorshift_next_uint(&rng);
    }
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %.3f seconds (sum: %llu to prevent optimization)\n", elapsed, (unsigned long long)sum);
    
    return 0;
}
#endif
