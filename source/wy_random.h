/* MIT License - Copyright (c) 2025 wallstop
 * Original C# implementation from: https://github.com/wallstop/unity-helpers
 * Converted to C for librandom project
 */

#ifndef WY_RANDOM_H
#define WY_RANDOM_H

#include <stdint.h>
#include <stdbool.h>

/* WyRandom: A wyhash-inspired PRNG variant leveraging multiply-mix operations.
 *
 * Designed around 64-bit multiply-and-mix steps, this generator is fast and
 * suitable for general-purpose randomness and hashing-like use cases.
 *
 * Pros:
 * - Fast and simple; good distribution for typical gameplay uses
 * - Deterministic across platforms
 *
 * Cons:
 * - Not cryptographically secure
 */

typedef struct {
    uint64_t state;
} wy_random_t;

/* Initialize the generator with a seed */
void wy_init(wy_random_t *rng, uint64_t seed);

/* Generate next random 32-bit unsigned integer */
uint32_t wy_next_uint(wy_random_t *rng);

/* Generate next random 64-bit unsigned integer */
uint64_t wy_next_ulong(wy_random_t *rng);

/* Generate next random float in [0, 1) */
float wy_next_float(wy_random_t *rng);

/* Generate next random double in [0, 1) */
double wy_next_double(wy_random_t *rng);

#endif /* WY_RANDOM_H */
