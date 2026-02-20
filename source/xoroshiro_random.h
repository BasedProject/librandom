/* MIT License - Copyright (c) 2025 wallstop
 * Original C# implementation from: https://github.com/wallstop/unity-helpers
 * Converted to C for librandom project
 */

#ifndef XOROSHIRO_RANDOM_H
#define XOROSHIRO_RANDOM_H

#include <stdint.h>
#include <stdbool.h>

/* XoroShiro: A fast 128-bit state Xoroshiro-based PRNG with good quality.
 *
 * Xoroshiro family generators offer an excellent balance between speed and
 * quality for real-time applications. This implementation maintains two 64-bit
 * state variables.
 *
 * Pros:
 * - Very fast; suitable for gameplay and procedural generation
 * - Good statistical properties; long period (~2^128-1)
 * - Deterministic and reproducible across platforms
 *
 * Cons:
 * - Not cryptographically secure
 * - Low bits may show weaker properties in some variants
 */

typedef struct {
    uint64_t s0;
    uint64_t s1;
} xoroshiro_random_t;

/* Initialize the generator with two seeds */
void xoroshiro_init(xoroshiro_random_t *rng, uint64_t seed1, uint64_t seed2);

/* Generate next random 32-bit unsigned integer */
uint32_t xoroshiro_next_uint(xoroshiro_random_t *rng);

/* Generate next random 64-bit unsigned integer */
uint64_t xoroshiro_next_ulong(xoroshiro_random_t *rng);

/* Generate next random float in [0, 1) */
float xoroshiro_next_float(xoroshiro_random_t *rng);

/* Generate next random double in [0, 1) */
double xoroshiro_next_double(xoroshiro_random_t *rng);

#endif /* XOROSHIRO_RANDOM_H */
