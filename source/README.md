# PhotonSpinRandom C Implementation

This directory contains a C implementation of the PhotonSpinRandom generator, originally written in C# by Will Stafford Parsons and distributed in the [unity-helpers](https://github.com/wallstop/unity-helpers) repository.

## Files

- **photon_spin_random.h** - Header file with type definitions and function declarations
- **photon_spin_random.c** - Implementation of the PhotonSpin32 random number generator
- **Makefile** - Build configuration for easy compilation
- **donut.c** - ASCII art donut (not a source file, QA artifact)

## About PhotonSpinRandom

PhotonSpin32 is a 20-word ring-buffer random number generator inspired by SHISHUA, designed for high throughput and large period (~2^512). It excels at:

- Generating large streams of high-quality random numbers
- Heavy simulation workloads requiring excellent distribution
- Deterministic state capture and restoration

**Not suitable for**: Cryptographic or security-sensitive applications.

## Building

### Using Make

```bash
make        # Build the test program
make test   # Build and run tests
make clean  # Clean build artifacts
```

### Using @BAKE

The source file includes a `@BAKE` statement for use with the [bake](https://github.com/8e8m/bake) build tool:

```bash
# The @BAKE command is embedded in photon_spin_random.c:
# @BAKE cc -O2 -Wall -Wextra -std=c99 -o photon_spin_random photon_spin_random.c -lm @STOP
```

To use it with bake, simply run bake in this directory.

### Manual Compilation

```bash
# For testing with built-in test harness:
cc -O2 -Wall -Wextra -std=c99 -DPHOTON_SPIN_TEST_MAIN -o photon_spin_random photon_spin_random.c -lm

# As a library (without test main):
cc -O2 -Wall -Wextra -std=c99 -c photon_spin_random.c
```

## Usage Example

```c
#include "photon_spin_random.h"

int main(void) {
    photon_spin_random_t rng;
    
    // Initialize with a seed
    photon_spin_init(&rng, 42);
    
    // Generate random numbers
    uint32_t rand_uint = photon_spin_next_uint(&rng);
    float rand_float = photon_spin_next_float(&rng);  // [0, 1)
    double rand_double = photon_spin_next_double(&rng);  // [0, 1)
    int32_t rand_range = photon_spin_next_int_range(&rng, 0, 100);  // [0, 100)
    
    return 0;
}
```

## API Reference

### Initialization Functions

- `photon_spin_init(rng, seed)` - Initialize with a single 32-bit seed
- `photon_spin_init_scalars(rng, seed_a, seed_b, seed_c)` - Initialize with three 32-bit seeds
- `photon_spin_init_ulongs(rng, seed0, seed1)` - Initialize with two 64-bit seeds

### Generation Functions

- `photon_spin_next_uint(rng)` - Generate next 32-bit unsigned integer
- `photon_spin_next_float(rng)` - Generate next float in [0, 1)
- `photon_spin_next_double(rng)` - Generate next double in [0, 1)
- `photon_spin_next_int_range(rng, min, max)` - Generate integer in [min, max)

## License

MIT License - Copyright (c) 2025 wallstop

Original C# implementation from: https://github.com/wallstop/unity-helpers

Converted to C for the librandom project.
