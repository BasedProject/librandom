/* MIT License - Copyright (c) 2025 wallstop
 * Original C# implementation from: https://github.com/wallstop/unity-helpers
 * Converted to C for librandom project
 */

#ifndef SPLITMIX64_H
#define SPLITMIX64_H

#include <stdint.h>
#include <stdbool.h>

/* SplitMix64: A fast 64-bit generator often used as a high-quality seeding/mixing PRNG.
 *
 * SplitMix64 is widely used to quickly generate well-distributed 64-bit values
 * and as a seed source for other generators.
 *
 * Pros:
 * - Very fast; great as a hash/mixer and for seed generation
 * - Deterministic, portable, and simple
 *
 * Cons:
 * - Not cryptographically secure
 */

typedef struct {
    uint64_t state;
} splitmix64_t;

/* Initialize the generator with a seed */
void splitmix64_init(splitmix64_t *rng, uint64_t seed);

/* Generate next random 32-bit unsigned integer */
uint32_t splitmix64_next_uint(splitmix64_t *rng);

/* Generate next random 64-bit unsigned integer */
uint64_t splitmix64_next_ulong(splitmix64_t *rng);

/* Generate next random float in [0, 1) */
float splitmix64_next_float(splitmix64_t *rng);

/* Generate next random double in [0, 1) */
double splitmix64_next_double(splitmix64_t *rng);

#endif /* SPLITMIX64_H */
