/* MIT License - Copyright (c) 2025 wallstop
 * Original C# implementation from: https://github.com/wallstop/unity-helpers
 * Converted to C for librandom project
 */

#ifndef PCG_RANDOM_H
#define PCG_RANDOM_H

#include <stdint.h>
#include <stdbool.h>

/* PCG (Permuted Congruential Generator): High-quality, small-state PRNG.
 *
 * PCG offers excellent statistical quality with very small state and extremely
 * fast generation. This implementation uses a 64-bit state with 32-bit outputs
 * and an increment (stream selector).
 *
 * Pros:
 * - Fast and allocation-free; suitable for gameplay hot paths
 * - Great statistical quality; passes TestU01 BigCrush and PractRand
 * - Deterministic and reproducible across platforms
 * - Small state footprint
 *
 * Cons:
 * - Not cryptographically secure
 * - 32-bit outputs (use multiple calls for 64-bit)
 */

typedef struct {
    uint64_t state;
    uint64_t increment;
} pcg_random_t;

/* Initialize the generator with a seed */
void pcg_init(pcg_random_t *rng, uint64_t seed);

/* Initialize with explicit state and increment */
void pcg_init_full(pcg_random_t *rng, uint64_t state, uint64_t increment);

/* Generate next random 32-bit unsigned integer */
uint32_t pcg_next_uint(pcg_random_t *rng);

/* Generate next random 64-bit unsigned integer */
uint64_t pcg_next_ulong(pcg_random_t *rng);

/* Generate next random float in [0, 1) */
float pcg_next_float(pcg_random_t *rng);

/* Generate next random double in [0, 1) */
double pcg_next_double(pcg_random_t *rng);

#endif /* PCG_RANDOM_H */
