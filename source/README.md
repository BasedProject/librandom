# Random Number Generator C Implementations

This directory contains C implementations of various random number generators, originally written in C# by Will Stafford Parsons and distributed in the [unity-helpers](https://github.com/wallstop/unity-helpers) repository.

## Files

### PhotonSpinRandom
- **photon_spin_random.h** / **photon_spin_random.c** - 20-word ring-buffer RNG inspired by SHISHUA
- Large state, excellent distribution, huge period (~2^512)
- Best for: Heavy simulation workloads requiring large streams

### XorShift
- **xorshift_random.h** / **xorshift_random.c** - Classic, extremely fast PRNG
- Tiny state (32-bit), very fast, modest quality
- Best for: Effects, particles, jitter, lightweight randomness

### SplitMix64
- **splitmix64.h** / **splitmix64.c** - Fast 64-bit seeding/mixing generator
- Very fast, great for seed generation and hashing
- Best for: Seeding other PRNGs, quick mixing, gameplay randomness

### WyRandom
- **wy_random.h** / **wy_random.c** - Wyhash-inspired multiply-mix PRNG
- Fast, good distribution, multiply-based mixing
- Best for: General gameplay RNG, weight selection, shuffles

### PCG (Permuted Congruential Generator)
- **pcg_random.h** / **pcg_random.c** - High-quality, small-state PRNG
- Excellent statistical quality, passes TestU01 BigCrush and PractRand
- Best for: General gameplay, procedural content, Monte Carlo sampling

### XoroShiro
- **xoroshiro_random.h** / **xoroshiro_random.c** - Fast 128-bit state Xoroshiro PRNG
- Very fast, good quality, long period (~2^128−1)
- Best for: General-purpose randomness, procedural generation

### Build System
- **Makefile** - Build configuration for all generators
- **donut.c** - ASCII art donut (QA artifact, not a source file)

## Building

### Using Make

```bash
make        # Build all test programs
make test   # Build and run all tests
make clean  # Clean build artifacts
```

### Using @BAKE

Each source file includes a `@BAKE` statement for use with the [bake](https://github.com/8e8m/bake) build tool. Example from photon_spin_random.c:

```bash
# @BAKE cc -O2 -Wall -Wextra -std=c99 -o photon_spin_random photon_spin_random.c -lm @STOP
```

### Manual Compilation

```bash
# For testing with built-in test harness:
cc -O2 -Wall -Wextra -std=c99 -DPHOTON_SPIN_TEST_MAIN -o photon_spin_random photon_spin_random.c -lm
cc -O2 -Wall -Wextra -std=c99 -DXORSHIFT_TEST_MAIN -o xorshift_random xorshift_random.c -lm
cc -O2 -Wall -Wextra -std=c99 -DSPLITMIX64_TEST_MAIN -o splitmix64 splitmix64.c -lm
cc -O2 -Wall -Wextra -std=c99 -DWY_TEST_MAIN -o wy_random wy_random.c -lm
cc -O2 -Wall -Wextra -std=c99 -DPCG_TEST_MAIN -o pcg_random pcg_random.c -lm
cc -O2 -Wall -Wextra -std=c99 -DXOROSHIRO_TEST_MAIN -o xoroshiro_random xoroshiro_random.c -lm

# As libraries (without test main):
cc -O2 -Wall -Wextra -std=c99 -c photon_spin_random.c
# ... etc for other generators
```

## Usage Examples

### PhotonSpinRandom
```c
#include "photon_spin_random.h"

photon_spin_random_t rng;
photon_spin_init(&rng, 42);
uint32_t rand_uint = photon_spin_next_uint(&rng);
float rand_float = photon_spin_next_float(&rng);  // [0, 1)
```

### XorShift
```c
#include "xorshift_random.h"

xorshift_random_t rng;
xorshift_init(&rng, 42);
uint32_t value = xorshift_next_uint(&rng);
```

### SplitMix64
```c
#include "splitmix64.h"

splitmix64_t rng;
splitmix64_init(&rng, 42);
uint64_t value = splitmix64_next_ulong(&rng);
```

### WyRandom
```c
#include "wy_random.h"

wy_random_t rng;
wy_init(&rng, 42);
uint32_t value = wy_next_uint(&rng);
```

### PCG
```c
#include "pcg_random.h"

pcg_random_t rng;
pcg_init(&rng, 42);
uint32_t value = pcg_next_uint(&rng);
```

### XoroShiro
```c
#include "xoroshiro_random.h"

xoroshiro_random_t rng;
xoroshiro_init(&rng, 42, 12345);
uint64_t value = xoroshiro_next_ulong(&rng);
```

## Choosing a Generator

| Generator | Speed | Quality | State Size | Use Case |
|-----------|-------|---------|------------|----------|
| XorShift | Fastest | Fair | 4 bytes | Particles, effects, simple randomness |
| SplitMix64 | Very Fast | Very Good | 8 bytes | Seeding, mixing, quick generation |
| WyRandom | Very Fast | Very Good | 8 bytes | General gameplay, hashing-like uses |
| PCG | Fast | Excellent | 16 bytes | High-quality gameplay, simulations |
| XoroShiro | Very Fast | Very Good | 16 bytes | General-purpose, procedural gen |
| PhotonSpin | Fast | Excellent | 80 bytes | Large streams, heavy simulations |

**Note**: None of these generators are cryptographically secure. Do not use for security-sensitive applications.

## License

MIT License - Copyright (c) 2025 wallstop

Original C# implementations from: https://github.com/wallstop/unity-helpers

Converted to C for the librandom project.
