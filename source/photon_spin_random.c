/* MIT License - Copyright (c) 2025 wallstop
 * Original C# implementation from: https://github.com/wallstop/unity-helpers
 * Converted to C for librandom project
 *
 * @BAKE cc -O2 -Wall -Wextra -std=c99 -o photon_spin_random photon_spin_random.c -lm @STOP
 */

#include "photon_spin_random.h"
#include <string.h>

/* Rotate left helper */
static inline uint32_t rotate_left(uint32_t value, int shift) {
    return (value << shift) | (value >> (32 - shift));
}

/* Mix helper for initialization */
static uint32_t mix32(uint64_t *mixer) {
    *mixer = (*mixer ^ (*mixer >> 33)) * 0xFF51AFD7ED558CCDULL;
    *mixer = (*mixer ^ (*mixer >> 33)) * 0xC4CEB9FE1A85EC53ULL;
    *mixer ^= (*mixer >> 33);
    uint32_t result = (uint32_t)(*mixer >> 32);
    *mixer += 0x9E3779B97F4A7C15ULL;
    return result;
}

/* Normalize index helper */
static void normalize_index(photon_spin_random_t *rng) {
    if (rng->index < 0 || rng->index > PHOTON_SPIN_BLOCK_SIZE) {
        rng->index = PHOTON_SPIN_BLOCK_SIZE;
    }
}

/* Generate a new block of random numbers */
static void generate_block(photon_spin_random_t *rng) {
    uint32_t mix[4];
    int base_index = (int)(rng->a & 15U);
    
    mix[0] = rng->elements[base_index];
    mix[1] = rng->elements[(base_index + 3) & 15];
    mix[2] = rng->elements[(base_index + 6) & 15];
    mix[3] = rng->elements[(base_index + 9) & 15];
    
    rng->a += PHOTON_SPIN_INCREMENT;
    
    int k = 0;
    for (int i = 0; i < 4; ++i) {
        rng->b += rng->a;
        rng->c ^= rng->a;
        mix[i] += rotate_left(rng->b, 9);
        
        for (int j = 0; j < 5; ++j) {
            rng->elements[k] += rotate_left(mix[i], 25);
            rng->elements[k] ^= rng->c;
            mix[i] += rng->elements[k];
            k++;
        }
        
        /* Check for collision patterns and perturb if needed.
         * Note: k is always 5, 10, 15, or 20 here (after the inner loop),
         * so k-5 is always >= 0 (safe array access) */
        if (k >= 5 && 
            rng->elements[k - 1] == rng->elements[k - 2] &&
            rng->elements[k - 3] == rng->elements[k - 4]) {
            rng->elements[k - 1] += mix[i] ^ 81016U;
            rng->elements[k - 2] += mix[i] ^ 297442265U;
            rng->elements[k - 3] += (mix[i] ^ 5480U) | (mix[i] & 1U);
            rng->elements[k - 4] += mix[i] ^ 19006808U;
            rng->elements[k - 5] += mix[i];
        }
    }
    
    rng->b += rotate_left(mix[0], 23);
    rng->b ^= mix[1];
    rng->c += rotate_left(mix[2], 13);
    rng->c ^= mix[3];
    
    rng->index = 0;
}

/* Initialize from two 64-bit seeds */
void photon_spin_init_ulongs(photon_spin_random_t *rng, uint64_t seed0, uint64_t seed1) {
    memset(rng, 0, sizeof(photon_spin_random_t));
    
    uint64_t mixer = seed0 ^ (seed1 << 1) ^ 0x9E3779B97F4A7C15ULL;
    
    for (int i = 0; i < PHOTON_SPIN_BLOCK_SIZE; ++i) {
        rng->elements[i] = mix32(&mixer);
    }
    
    rng->a = mix32(&mixer);
    rng->b = mix32(&mixer);
    rng->c = mix32(&mixer);
    
    rng->index = PHOTON_SPIN_BLOCK_SIZE;
    rng->has_primed = false;
    normalize_index(rng);
}

/* Initialize from three scalars */
void photon_spin_init_scalars(photon_spin_random_t *rng, uint32_t seed_a, uint32_t seed_b, uint32_t seed_c) {
    uint64_t seed0 = ((uint64_t)seed_a << 32) | seed_b;
    uint64_t seed1 = ((uint64_t)seed_c << 32) | (seed_b ^ 0xA5A5A5A5U);
    photon_spin_init_ulongs(rng, seed0, seed1);
}

/* Initialize from a single seed */
void photon_spin_init(photon_spin_random_t *rng, uint32_t seed) {
    uint32_t seed_a = seed;
    uint32_t seed_b = seed ^ 0x9E3779B9U;
    if (seed_b == 0) {
        seed_b = 1U;
    }
    uint32_t seed_c = seed + 0x6C8E9CF5U;
    photon_spin_init_scalars(rng, seed_a, seed_b, seed_c);
}

/* Generate next random uint32_t */
uint32_t photon_spin_next_uint(photon_spin_random_t *rng) {
    if (!rng->has_primed) {
        generate_block(rng);
        rng->has_primed = true;
        rng->index = PHOTON_SPIN_BLOCK_SIZE;
    }
    
    if (rng->index >= PHOTON_SPIN_BLOCK_SIZE) {
        generate_block(rng);
    }
    
    uint32_t value = rng->elements[rng->index];
    rng->index += 1;
    return value;
}

/* Generate next random float in [0, 1) */
float photon_spin_next_float(photon_spin_random_t *rng) {
    uint32_t value = photon_spin_next_uint(rng);
    /* Convert to float in [0, 1) using standard method */
    return (value >> 8) * 0x1.0p-24f;
}

/* Generate next random double in [0, 1) */
double photon_spin_next_double(photon_spin_random_t *rng) {
    uint32_t high = photon_spin_next_uint(rng);
    uint32_t low = photon_spin_next_uint(rng);
    uint64_t combined = ((uint64_t)high << 32) | low;
    /* Convert to double in [0, 1) using standard method */
    return (combined >> 11) * 0x1.0p-53;
}

/* Generate next random int in range [min, max) */
int32_t photon_spin_next_int_range(photon_spin_random_t *rng, int32_t min, int32_t max) {
    if (min >= max) {
        return min;
    }
    
    uint32_t range = (uint32_t)(max - min);
    uint32_t value = photon_spin_next_uint(rng);
    
    /* Use rejection sampling to avoid modulo bias */
    uint32_t limit = (0xFFFFFFFFU / range) * range;
    while (value >= limit) {
        value = photon_spin_next_uint(rng);
    }
    
    return min + (int32_t)(value % range);
}

#ifdef PHOTON_SPIN_TEST_MAIN
#include <stdio.h>
#include <time.h>

int main(void) {
    photon_spin_random_t rng;
    
    /* Test with fixed seed */
    printf("PhotonSpinRandom C Implementation Test\n");
    printf("======================================\n\n");
    
    photon_spin_init(&rng, 42);
    
    printf("Testing with seed 42:\n");
    printf("First 10 uint32_t values:\n");
    for (int i = 0; i < 10; i++) {
        printf("  %u\n", photon_spin_next_uint(&rng));
    }
    
    printf("\nFirst 10 float values:\n");
    photon_spin_init(&rng, 42);
    for (int i = 0; i < 10; i++) {
        printf("  %.6f\n", photon_spin_next_float(&rng));
    }
    
    printf("\nFirst 10 double values:\n");
    photon_spin_init(&rng, 42);
    for (int i = 0; i < 10; i++) {
        printf("  %.10f\n", photon_spin_next_double(&rng));
    }
    
    printf("\n10 random ints in range [0, 100):\n");
    photon_spin_init(&rng, 42);
    for (int i = 0; i < 10; i++) {
        printf("  %d\n", photon_spin_next_int_range(&rng, 0, 100));
    }
    
    /* Performance test */
    printf("\nPerformance test: generating 10 million uint32_t values...\n");
    photon_spin_init(&rng, (uint32_t)time(NULL));
    clock_t start = clock();
    uint64_t sum = 0;
    for (int i = 0; i < 10000000; i++) {
        sum += photon_spin_next_uint(&rng);
    }
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time: %.3f seconds (sum: %llu to prevent optimization)\n", elapsed, (unsigned long long)sum);
    
    return 0;
}
#endif
