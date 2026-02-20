/* MIT License - Copyright (c) 2025 wallstop
 * Original C# implementation from: https://github.com/wallstop/unity-helpers
 * Converted to C for librandom project
 */

#ifndef PHOTON_SPIN_RANDOM_H
#define PHOTON_SPIN_RANDOM_H

#include <stdint.h>
#include <stdbool.h>

/* PhotonSpin32: a 20-word ring-buffer generator inspired by SHISHUA,
 * tuned for high throughput and large period.
 *
 * Reference: Will Stafford Parsons (wileylooper/photonspin, repository offline).
 * This generator produces batches of 20 new 32-bit values per round,
 * offering a huge period (~2^512) and robust statistical performance.
 *
 * Pros:
 * - Large state with excellent distribution; great for heavy simulation workloads
 * - Deterministic snapshots through state capture
 *
 * Cons:
 * - Higher per-instance memory (~20×4 bytes)
 * - Not cryptographically secure
 */

#define PHOTON_SPIN_BLOCK_SIZE 20
#define PHOTON_SPIN_INCREMENT 111111U

typedef struct {
    uint32_t elements[PHOTON_SPIN_BLOCK_SIZE];
    uint32_t a;
    uint32_t b;
    uint32_t c;
    int index;
    bool has_primed;
} photon_spin_random_t;

/* Initialize the generator with a single seed */
void photon_spin_init(photon_spin_random_t *rng, uint32_t seed);

/* Initialize the generator with three seeds */
void photon_spin_init_scalars(photon_spin_random_t *rng, uint32_t seed_a, uint32_t seed_b, uint32_t seed_c);

/* Initialize the generator with two 64-bit seeds */
void photon_spin_init_ulongs(photon_spin_random_t *rng, uint64_t seed0, uint64_t seed1);

/* Generate next random 32-bit unsigned integer */
uint32_t photon_spin_next_uint(photon_spin_random_t *rng);

/* Generate next random float in [0, 1) */
float photon_spin_next_float(photon_spin_random_t *rng);

/* Generate next random double in [0, 1) */
double photon_spin_next_double(photon_spin_random_t *rng);

/* Generate next random int in range [min, max) */
int32_t photon_spin_next_int_range(photon_spin_random_t *rng, int32_t min, int32_t max);

#endif /* PHOTON_SPIN_RANDOM_H */
