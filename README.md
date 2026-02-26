# librandom

`librandom` provides randomness, hopefully that was unpredictable.

This code is generally ripped and written for C.

see [Here](https://github.com/wallstop/unity-helpers) in `Runtime/Core/Random`.

- [librandom](#librandom)
  * [Generalized](#generalized)
  * [Headers](#headers)
    + [float.h](#floath)
    + [combine.h](#combineh)
    + [lcg.h](#lcgh)
    + [lcg2.h](#lcg2h)
    + [pcg.h](#pcgh)
    + [xorshift.h](#xorshifth)
    + [xoroshiro.h](#xoroshiroh)
    + [splitmix.h](#splitmixh)
    + [wy.h](#wyh)
    + [photon.h](#photonh)
    + [romuduo.h](#romuduoh)
    + [mt19937.h](#mt19937h)
  * [Copyright?](#copyright-)
    + [librandom License](#librandom-license)
    + [Wallstop MIT License](#wallstop-mit-license)

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

| Header																					 | 64 Support | Shape                  |
| :-----																					 | :--------- | :----                  |
| [lcg.h](https://github.com/BasedProject/librandom/blob/master/source/lcg.h)				 | No         | uint32\_t              |
| [lcg2.h](https://github.com/BasedProject/librandom/blob/master/source/lcg2.h)				 | Yes        | uint64\_t              |
| [pcg.h](https://github.com/BasedProject/librandom/blob/master/source/pcg.h)				 | No         | uint64\_t (a,b)        |
| [xorshift.h](https://github.com/BasedProject/librandom/blob/master/source/xorshift.h)		 | No         | uint32\_t              |
| [xoroshiro.h](https://github.com/BasedProject/librandom/blob/master/source/xoroshiro.h)	 | Yes        | uint64\_t (a,b)        |
| [splitmix.h](https://github.com/BasedProject/librandom/blob/master/source/splitmix.h)		 | Yes        | uint64\_t              |
| [wy.h](https://github.com/BasedProject/librandom/blob/master/source/wy.h)					 | Yes        | uint64\_t              |
| [photon.h](https://github.com/BasedProject/librandom/blob/master/source/photon.h)			 | No         | Configurable at library compile time, see header. |
| [romuduo.h](https://github.com/BasedProject/librandom/blob/master/source/romuduo.h)		 | No         | uint64\_t (a,b)        |

None of these should be used in security sensitive contexts unless marked otherwise.

## Headers

This may be over in some places, but should cover basically everything.

### [float.h](https://github.com/BasedProject/librandom/blob/master/source/float.h)

Do primitive conversion to float.

    float  random_float(uint32_t random);
    double random_double(uint64_t random);

### [combine.h](https://github.com/BasedProject/librandom/blob/master/source/combine.h)

You may want to combine two uint32s into a uint64:

    uint64_t random_combine(uint32_t low, uint32_t high);

### [lcg.h](https://github.com/BasedProject/librandom/blob/master/source/lcg.h)

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

### [lcg2.h](https://github.com/BasedProject/librandom/blob/master/source/lcg2.h)

    random_lcg2_t random_lcg2_init(uint64_t Initial State)
    uint32_t random_lcg232(random_lcg2_t * Random Pointer)
    uint64_t random_lcg264(random_lcg2_t * Random Pointer)

Similiar to lcg.h, except employs the 64-bit LCG described [here](https://nuclear.llnl.gov/CNP/rng/rngman/node4.html).

Per Bret R. Beck and Eugene D. Brooks III.

Example:

    random_lcg2_t r[1] = {random_lcg2_init(initial)};
    uint64_t rand = random_lcg264(r);

### [pcg.h](https://github.com/BasedProject/librandom/blob/master/source/pcg.h)

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

### [xorshift.h](https://github.com/BasedProject/librandom/blob/master/source/xorshift.h)

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
    uint32_t rand = random_xorshift32(r);

### [xoroshiro.h](https://github.com/BasedProject/librandom/blob/master/source/xoroshiro.h)

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

### [splitmix.h](https://github.com/BasedProject/librandom/blob/master/source/splitmix.h)

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

### [wy.h](https://github.com/BasedProject/librandom/blob/master/source/wy.h)

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
    uint32_t rand1 = random_wy32(r);
	uint64_t rand2 = random_wy64(r);

### [photon.h](https://github.com/BasedProject/librandom/blob/master/source/photon.h)
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
    uint32_t rand = random_photon32(r);

### [romuduo.h](https://github.com/BasedProject/librandom/blob/master/source/romuduo.h)

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
    uint32_t rand = random_romuduo32(r);

### [mt19937.h](https://github.com/BasedProject/librandom/blob/master/source/mt19937.h)
`BSD-3-clause -- Copyright 1997 - 2002, Makoto Matsumoto and Takuji Nishimura -- All rights reserved.` 

See [mt19937.c](https://github.com/BasedProject/librandom/blob/master/source/mt19937.c) for full licensing.

    random_mt19937_t random_mt19937_init(uint32_t Initial State)
    uint32_t random_mt1993732(random_mt19937_t * Random Pointer)

See [Here.](https://en.wikipedia.org/wiki/Mersenne_Twister)

Super large period of 2^19937-1. Based on Mersenne primes.

Pros: Generally robust.
 Good for simulations.
 
Cons: like all other PRNGs here, not for cryptographic use.
 Not the one true solution to PRNGs, but close enough.
 
Example:

    random_mt19937_t r[1] = {random_mt19937_init(initial)};
    uint32_t rand = random_mt1993732(r);

## Copyright?

All of these headers are marked as [`MIT License - Copyright 2025 wallstop`](https://mit-license.org/) unless otherwise specified or applicable to some larger domain, in which case the notice could be prunable. (i.e. very well known algorithms released to the public domain.) The text in this document is generally divined the initial project.

### librandom License

[librandom](https://github.com/BasedProject/librandom) was composed by [Emil Williams](https://github.com/8e8m) for the benefit of noone.

Any source within the bounds of this project excluding already clearly marked code, is marked [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/).
There is no warranty under any circumstance, use librandom at your own detriment.

![(CC)](https://mirrors.creativecommons.org/presskit/icons/cc.svg)
![(0)](https://mirrors.creativecommons.org/presskit/icons/zero.svg)

### Wallstop MIT License

Copyright © 2025 wallstop

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
