/* MIT License - Copyright (c) 2025 wallstop
 * Original C# implementation from: https://github.com/wallstop/unity-helpers
 * Converted to C for librandom project
 */

#ifndef XORSHIFT_RANDOM_H
#define XORSHIFT_RANDOM_H

#include <stdint.h>
#include <stdbool.h>

/* XorShift: A classic, extremely fast PRNG with small state and modest quality.
 *
 * XorShift generators are known for their simplicity and speed. This variant
 * operates on a 32-bit state and produces 32-bit outputs.
 *
 * Pros:
 * - Very fast; tiny state footprint
 * - Deterministic and easy to serialize/restore
 *
 * Cons:
 * - Lower statistical quality than newer generators
 * - Not cryptographically secure
 */

#define XORSHIFT_DEFAULT_STATE 2463534242U

typedef struct {
    uint32_t state;
} xorshift_random_t;

/* Initialize the generator with a seed */
void xorshift_init(xorshift_random_t *rng, uint32_t seed);

/* Generate next random 32-bit unsigned integer */
uint32_t xorshift_next_uint(xorshift_random_t *rng);

/* Generate next random float in [0, 1) */
float xorshift_next_float(xorshift_random_t *rng);

/* Generate next random double in [0, 1) */
double xorshift_next_double(xorshift_random_t *rng);

#endif /* XORSHIFT_RANDOM_H */
