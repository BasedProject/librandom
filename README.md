# librandom

`librandom` provides randomness, hopefully that was unpredictable.

This code is generally ripped and written for C.

see [Here](https://github.com/wallstop/unity-helpers) in `Runtime/Core/Random`.

## Generalized

Interface:

    random_psued_t Reentrant Random State Variable

    // initial value may always be 0
    random_psued_t random_psued_init(uint32_t Initial State)
    // seeding functions
    uint32_t random_psued32(random_psued_t * Random Pointer)
    uint64_t random_psued64(random_psued_t * Random Pointer)
    // convert those usigned int values to floats
    float  random_float(uint32_t Random)
    double random_double(uint64_t Random)
    // combine two usigned 32-bit int values into one unsigned 64 bit int
    uint64_t random_combine(uint32_t Low Bits, uint32_t High Bits)

Example:

    // This is the recommended way to initilaize states.
    random_psued_t rrsv[1] = {random_psued_init(initial)};
    uint32_t rand1 = random_psued32(rrsv);
    // Arbitrary combinations of psued32 and psued64 do not provide a different actual sequence.
    // This is always true when psued64 is provided.
    uint64_t rand2 = random_psued64(rrsv);

Replace psued with your algorithm of choice.

Only some support psued64:

| Header      | 64 Support | Shape                  |
| :-----      | :--------- | :----                  |
| lcg.h       | No         | uint32\_t              |
| pcg.h       | No         | uint64\_t (a,b)        |
| xorshift.h  | No         | uint32\_t              |
| xoroshiro.h | Yes        | uint64\_t (a,b)        |
| splitmix.h  | Yes        | uint64\_t              |
| wy.h        | Yes        | uint64\_t              |
| photon.h    | No         | Configurable at library compile time, see header. |
| romuduo.h   | No         | uint64\_t (a,b)        |

None of these should be used in security sensitive contexts unless marked otherwise.

## Headers

This may be over in some places, but should cover basically everything.

### float.h

Do primitive conversion to float.

    float  random_float(uint32_t random);
    double random_double(uint64_t random);

### combine.h

You may want to combine two uint32s into a uint64:

    uint64_t random_combine(uint32_t low, uint32_t high);

### lcg.h

    random_lcg_t random_lcg_init(uint32_t Initial State)
    uint32_t random_lcg32(random_lcg_t * Random Pointer)

A simple Linear Congruential Generator (LCG): extremely fast with low-quality randomness.

LCGs are among the oldest PRNGs. This configuration is fast and compact but exhibits correlations and
 shorter periods compared to modern generators. Best suited for cosmetic randomness where quality is not
 critical.

Pros: Fast; trivial; Tiny state; Deterministic behavior; Serializable.

Cons: Poor Statistical Quality vs. xoroshiro.h;
 noticeable patterns in some uses; Not cryptographically secure.

Use in low impact things, like visual effects, quick throwaway randomness, or prototyping.

Do not use in Gameplay-critical logic, simulations, or fairness-sensitive systems.

Example:

    random_lcg_t r[1] = {random_lcg_init(initial)};
    uint32_t rand = random_lcg32(r);

### pcg.h

    random_pcg_t random_pcg_init(uint64_t Initial State, Step) [note: RANDOM_PCG_PRIME made available for your benefit for stepper.]
    uint32_t random_pcg32(random_pcg_t * Random Pointer)

A high-quality, small-state pseudo-random number generator based on the PCG family.

Implementation based off of the reference PCG Random, found here: https://www.pcg-random.org/index.html

PCG (Permuted Congruential Generator) offers excellent statistical quality with very small state
and extremely fast generation. This implementation uses a 64-bit state with 32-bit outputs and
an increment (stream selector) to avoid overlapping sequences when constructing multiple instances.

Pros: Fast and allocation-free; suitable for gameplay hot paths.
 Great statistical quality for games and simulations; passes common PRNG test suites for 32-bit outputs.
 Deterministic and reproducible across platforms for identical seeds.

When to use: General gameplay randomness, procedural content, Monte Carlo style sampling.

Example:

    random_pcg_t r[1] = {random_pcg_init(initial, RANDOM_PCG_PRIME)};
    uint32_t rand = random_pcg32(r);

### xorshift.h

    random_xorshift_t random_xorshift_init(uint32_t Initial State)
    uint32_t random_xorshift32(random_xorshift_t * Random Pointer)

A classic, extremely fast XorShift PRNG with small state and modest quality.

XorShift generators are known for their simplicity and speed. This variant operates on a 32-bit state and
 produces 32-bit outputs. It is suitable for lightweight, cosmetic randomness where maximum statistical
 rigor is not required.

Pros: Very fast; tiny state footprint; Deterministic; Serializable.

Cons: Poor Statistical Quality vs. newer generators; Not cryptographically secure.

Use in effects, particles, jitter, or any light randomness in hot loops;
 Short-lived simulations where ultimate quality is not required.

Do not use in simulations or systems sensitive to subtle bias.

Example:

    random_xorshift_t r[1] = {random_xorshift_init(initial)};
    uint32_t rand = random_xorshift(r);

### xoroshiro.h

    random_xoroshiro_t random_xoroshiro_init(uint64_t Initial State Part 1, Initial State Part 2)
    uint32_t random_xoroshiro32(random_xoroshiro_t * Random Pointer)

A fast 128-bit state Xoroshiro-based PRNG with good quality and tiny footprint.

Xoroshiro family generators (here in a 64/64 configuration) offer an excellent balance between speed and quality
 for real-time applications. This implementation maintains two 64-bit state variables and returns 64-bit outputs.

Pros: Very fast; suitable for gameplay and procedural generation; 
 Good statistical properties for non-crypto use; long period (~2^128−1);
 Deterministic and reproducible across platforms.

Cons: Not cryptographically secure; Low bits may show weaker properties in some variants;
 use full width for mixing.

Use in general-purpose game randomness, procedural placement, shuffles, noise seeding.

Example:

    random_xoroshiro_t r[1] = {random_xoroshiro_init(initial_a, initial_b)};
    uint32_t rand = random_xoroshiro32(r);

### splitmix.h

    random_splitmix_t random_splitmix_init(uint64_t Initial State)
    uint32_t random_splitmix32(random_splitmix_t * Random Pointer)
    uint64_t random_splitmix64(random_splitmix_t * Random Pointer)
    
A fast 64-bit SplitMix generator often used as a high-quality seeding/mixing PRNG.

SplitMix64 is widely used to quickly generate well-distributed 64-bit values and as a seed source for
 other generators. In this implementation, we provide 64-bit outputs.

Pros: Very fast; great as a hash/mixer and for seed generation.
 Deterministic, portable, and simple.

Cons: Not cryptographically secure.

Use in producing seeds for other PRNGs; quick hash-like mixing; gameplay randomness.

Example:

    random_splitmix_t r[1] = {random_splitmix_init(initial)};
    uint32_t rand = random_splitmix32(r);

### wy.h

    random_wy_t random_wy_init(uint32_t Initial State)
    uint32_t random_wy32(random_wy_t * Random Pointer)
    uint64_t random_wy64(random_wy_t * Random Pointer)

A wyhash-inspired PRNG variant (WyRandom) leveraging multiply-mix operations for speed and good distribution.

Reference implementation: https://github.com/cocowalla/wyhash-dotnet/blob/master/src/WyHash/WyRng.cs

Designed around 64-bit multiply-and-mix steps, this generator is fast and suitable for general-purpose
 randomness and hashing-like use cases. It is not a cryptographic hash nor a CSPRNG.

Pros: Fast and simple; good distribution for typical gameplay uses.
 Deterministic across platforms.

Cons: Less widely standardized than PCG/Xoroshiro.

Use in general gameplay RNG, weight selection, shuffles, seed generation.

Example:

    random_wy_t r[1] = {random_wy_init(initial)};
    uint32_t rand = random_wy64(r);

### photon.h
The reason this library exists.

    random_photon_t random_photon_init(uint64_t Initial State 1, Initial State 2)
    uint32_t random_photon32(random_photon_t * Random Pointer)
    uint64_t random_photon64(random_photon_t * Random Pointer)

PhotonSpin32: a 20-word ring-buffer generator inspired by SHISHUA, tuned for high throughput and large period.

Reference: Will Stafford Parsons <https://github.com/wileylooper/photonspin>

You're free to change the 20-words to N words via the marco `RANDOM_PHOTON_BLOCK_SIZE`. Either induct photon.c into your program or recompile your library instance.

This generator produces batches of a dynamically defined new 32-bit values per round,
 offering a huge period (~2^512) and robust statistical performance. It shines when large streams are
 required, while still supporting deterministic state capture and serialization.

Pros: Large state with excellent distribution; great for heavy simulation workloads.

Cons: Higher per-instance memory (~20×4 bytes).
 Best used in procedural workloads that benefit from bulk generation and long non-overlapping streams.

Example:

    random_photon_t r[1] = {random_photon_init(initial_a, initial_b)};
    uint32_t rand = random_photon64(r);

### romuduo.h

    random_romuduo_t random_romuduo_init(uint64_t Initial State 1, Initial State 2)
    uint32_t random_romuduo32(random_romuduo_t * Random Pointer)

A member of the ROMU family (RomuDuo) emphasizing high speed and good statistical quality on modern CPUs.

RomuDuo maintains two 64-bit state variables and uses rotations and multiplies to evolve the state. It is
 competitive with Xoroshiro-style generators in speed while exhibiting strong distribution for general use.

Pros: Very fast; excellent for real-time usage.
 Good statistical behavior for non-crypto applications.
 Deterministic and reproducible across platforms.

Cons: Relatively newer family; choose proven options if organizational policy requires long-term validation.
 Use with gameplay RNG, procedural content generation, fast Monte Carlo sampling.

Example:

    random_romuduo_t r[1] = {random_romuduo_init(initial_a, initial_b)};
    uint32_t rand = random_romuduo64(r);
