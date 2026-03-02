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
  * [Dieharder Performance](#dieharder-performance)
    + [lcg](#lcg)
    + [lcg2](#lcg2)
    + [pcg](#pcg)
    + [xorshift](#xorshift)
    + [xoroshiro](#xoroshiro)
    + [splitmix](#splitmix)
    + [wy](#wy)
    + [photon](#photon)
    + [romuduo](#romuduo)
    + [mt19937](#mt19937)
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

## Dieharder Performance

[Skip to next section](#copyright-)

All below runs are seeded with the default value of 0. PCG uses a prime increment.

### xorshift
```dieharder
Proceeding with test 0...
#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |rands/second|   Seed   |
stdin_input_raw|  6.27e+07  |2122178445|
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.53642569|  PASSED
      diehard_operm5|   0|   1000000|     100|0.03961164|  PASSED
  diehard_rank_32x32|   0|     40000|     100|0.00000000|  FAILED
    diehard_rank_6x8|   0|    100000|     100|0.88957108|  PASSED
   diehard_bitstream|   0|   2097152|     100|0.80640512|  PASSED
        diehard_opso|   0|   2097152|     100|0.28845085|  PASSED
        diehard_oqso|   0|   2097152|     100|0.75556675|  PASSED
         diehard_dna|   0|   2097152|     100|0.08450089|  PASSED
diehard_count_1s_str|   0|    256000|     100|0.00000000|  FAILED
diehard_count_1s_byt|   0|    256000|     100|0.19170438|  PASSED
 diehard_parking_lot|   0|     12000|     100|0.66912419|  PASSED
    diehard_2dsphere|   2|      8000|     100|0.98738804|  PASSED
    diehard_3dsphere|   3|      4000|     100|0.78948668|  PASSED
     diehard_squeeze|   0|    100000|     100|0.29019454|  PASSED
        diehard_sums|   0|       100|     100|0.54678793|  PASSED
        diehard_runs|   0|    100000|     100|0.46966316|  PASSED
        diehard_runs|   0|    100000|     100|0.31219271|  PASSED
       diehard_craps|   0|    200000|     100|0.24277725|  PASSED
       diehard_craps|   0|    200000|     100|0.54345280|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.07571716|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.24811787|  PASSED
         sts_monobit|   1|    100000|     100|0.43000839|  PASSED
            sts_runs|   2|    100000|     100|0.36578511|  PASSED
          sts_serial|   1|    100000|     100|0.59517001|  PASSED
          sts_serial|   2|    100000|     100|0.82307299|  PASSED
          sts_serial|   3|    100000|     100|0.65305437|  PASSED
          sts_serial|   3|    100000|     100|0.66190477|  PASSED
          sts_serial|   4|    100000|     100|0.01736061|  PASSED
          sts_serial|   4|    100000|     100|0.33172256|  PASSED
          sts_serial|   5|    100000|     100|0.24105442|  PASSED
          sts_serial|   5|    100000|     100|0.81024808|  PASSED
          sts_serial|   6|    100000|     100|0.67651036|  PASSED
          sts_serial|   6|    100000|     100|0.60045416|  PASSED
          sts_serial|   7|    100000|     100|0.38037782|  PASSED
          sts_serial|   7|    100000|     100|0.73726343|  PASSED
          sts_serial|   8|    100000|     100|0.15241107|  PASSED
          sts_serial|   8|    100000|     100|0.10499551|  PASSED
          sts_serial|   9|    100000|     100|0.91503579|  PASSED
          sts_serial|   9|    100000|     100|0.30793062|  PASSED
          sts_serial|  10|    100000|     100|0.95062526|  PASSED
          sts_serial|  10|    100000|     100|0.99348605|  PASSED
          sts_serial|  11|    100000|     100|0.80898455|  PASSED
          sts_serial|  11|    100000|     100|0.18821345|  PASSED
          sts_serial|  12|    100000|     100|0.12581860|  PASSED
          sts_serial|  12|    100000|     100|0.10190788|  PASSED
          sts_serial|  13|    100000|     100|0.76218225|  PASSED
          sts_serial|  13|    100000|     100|0.08648427|  PASSED
          sts_serial|  14|    100000|     100|0.64157666|  PASSED
          sts_serial|  14|    100000|     100|0.08202735|  PASSED
          sts_serial|  15|    100000|     100|0.98503258|  PASSED
          sts_serial|  15|    100000|     100|0.52435051|  PASSED
          sts_serial|  16|    100000|     100|0.36452283|  PASSED
          sts_serial|  16|    100000|     100|0.14360079|  PASSED
         rgb_bitdist|   1|    100000|     100|0.00000000|  FAILED
         rgb_bitdist|   2|    100000|     100|0.05051504|  PASSED
         rgb_bitdist|   3|    100000|     100|0.25614480|  PASSED
         rgb_bitdist|   4|    100000|     100|0.93339049|  PASSED
         rgb_bitdist|   5|    100000|     100|0.70521924|  PASSED
         rgb_bitdist|   6|    100000|     100|0.49985336|  PASSED
         rgb_bitdist|   7|    100000|     100|0.93150381|  PASSED
         rgb_bitdist|   8|    100000|     100|0.39571891|  PASSED
         rgb_bitdist|   9|    100000|     100|0.62507471|  PASSED
         rgb_bitdist|  10|    100000|     100|0.76762647|  PASSED
         rgb_bitdist|  11|    100000|     100|0.69622258|  PASSED
         rgb_bitdist|  12|    100000|     100|0.36148502|  PASSED
rgb_minimum_distance|   2|     10000|    1000|0.45249550|  PASSED
rgb_minimum_distance|   3|     10000|    1000|0.31583536|  PASSED
rgb_minimum_distance|   4|     10000|    1000|0.52356354|  PASSED
rgb_minimum_distance|   5|     10000|    1000|0.97996654|  PASSED
    rgb_permutations|   2|    100000|     100|0.24249630|  PASSED
    rgb_permutations|   3|    100000|     100|0.31169667|  PASSED
    rgb_permutations|   4|    100000|     100|0.99047844|  PASSED
    rgb_permutations|   5|    100000|     100|0.62659676|  PASSED
      rgb_lagged_sum|   0|   1000000|     100|0.99267780|  PASSED
      rgb_lagged_sum|   1|   1000000|     100|0.87053138|  PASSED
      rgb_lagged_sum|   2|   1000000|     100|0.73848226|  PASSED
      rgb_lagged_sum|   3|   1000000|     100|0.56350676|  PASSED
      rgb_lagged_sum|   4|   1000000|     100|0.30797005|  PASSED
      rgb_lagged_sum|   5|   1000000|     100|0.71846590|  PASSED
      rgb_lagged_sum|   6|   1000000|     100|0.30906571|  PASSED
      rgb_lagged_sum|   7|   1000000|     100|0.69790814|  PASSED
      rgb_lagged_sum|   8|   1000000|     100|0.08346459|  PASSED
      rgb_lagged_sum|   9|   1000000|     100|0.59641738|  PASSED
      rgb_lagged_sum|  10|   1000000|     100|0.96319122|  PASSED
      rgb_lagged_sum|  11|   1000000|     100|0.69731926|  PASSED
      rgb_lagged_sum|  12|   1000000|     100|0.17245619|  PASSED
      rgb_lagged_sum|  13|   1000000|     100|0.48650938|  PASSED
      rgb_lagged_sum|  14|   1000000|     100|0.06077480|  PASSED
      rgb_lagged_sum|  15|   1000000|     100|0.74732777|  PASSED
      rgb_lagged_sum|  16|   1000000|     100|0.31327041|  PASSED
      rgb_lagged_sum|  17|   1000000|     100|0.48794712|  PASSED
      rgb_lagged_sum|  18|   1000000|     100|0.91372185|  PASSED
      rgb_lagged_sum|  19|   1000000|     100|0.74600855|  PASSED
      rgb_lagged_sum|  20|   1000000|     100|0.90139504|  PASSED
      rgb_lagged_sum|  21|   1000000|     100|0.83379286|  PASSED
      rgb_lagged_sum|  22|   1000000|     100|0.98871326|  PASSED
      rgb_lagged_sum|  23|   1000000|     100|0.80184775|  PASSED
      rgb_lagged_sum|  24|   1000000|     100|0.49506663|  PASSED
      rgb_lagged_sum|  25|   1000000|     100|0.65978133|  PASSED
      rgb_lagged_sum|  26|   1000000|     100|0.78156427|  PASSED
      rgb_lagged_sum|  27|   1000000|     100|0.87272882|  PASSED
      rgb_lagged_sum|  28|   1000000|     100|0.50825448|  PASSED
      rgb_lagged_sum|  29|   1000000|     100|0.07029373|  PASSED
      rgb_lagged_sum|  30|   1000000|     100|0.11101590|  PASSED
      rgb_lagged_sum|  31|   1000000|     100|0.99820396|   WEAK
      rgb_lagged_sum|  31|   1000000|     200|0.75755660|  PASSED
      rgb_lagged_sum|  32|   1000000|     100|0.22174907|  PASSED
     rgb_kstest_test|   0|     10000|    1000|0.64849060|  PASSED
     dab_bytedistrib|   0|  51200000|       1|0.97417504|  PASSED
             dab_dct| 256|     50000|       1|0.23672191|  PASSED
        dab_filltree|  32|  15000000|       1|0.65914267|  PASSED
        dab_filltree|  32|  15000000|       1|0.44083370|  PASSED
       dab_filltree2|   0|   5000000|       1|0.40593211|  PASSED
       dab_filltree2|   1|   5000000|       1|0.45491433|  PASSED
        dab_monobit2|  12|  65000000|       1|1.00000000|  FAILED

real    31m25.193s
user    48m15.908s
sys     2m45.736s
```

### lcg
```dieharder
Proceeding with test 1...
#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |rands/second|   Seed   |
stdin_input_raw|  7.34e+07  |2214967880|
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.33457820|  PASSED
      diehard_operm5|   0|   1000000|     100|0.29894566|  PASSED
  diehard_rank_32x32|   0|     40000|     100|0.41803285|  PASSED
    diehard_rank_6x8|   0|    100000|     100|0.94733497|  PASSED
   diehard_bitstream|   0|   2097152|     100|0.86491554|  PASSED
        diehard_opso|   0|   2097152|     100|0.97150238|  PASSED
        diehard_oqso|   0|   2097152|     100|0.91805958|  PASSED
         diehard_dna|   0|   2097152|     100|0.82651836|  PASSED
diehard_count_1s_str|   0|    256000|     100|0.32458885|  PASSED
diehard_count_1s_byt|   0|    256000|     100|0.31254831|  PASSED
 diehard_parking_lot|   0|     12000|     100|0.15798710|  PASSED
    diehard_2dsphere|   2|      8000|     100|0.16667624|  PASSED
    diehard_3dsphere|   3|      4000|     100|0.47575711|  PASSED
     diehard_squeeze|   0|    100000|     100|0.92632662|  PASSED
        diehard_sums|   0|       100|     100|0.29335448|  PASSED
        diehard_runs|   0|    100000|     100|0.68894983|  PASSED
        diehard_runs|   0|    100000|     100|0.41037643|  PASSED
       diehard_craps|   0|    200000|     100|0.16134816|  PASSED
       diehard_craps|   0|    200000|     100|0.57493639|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.37122683|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.86036127|  PASSED
         sts_monobit|   1|    100000|     100|0.64597252|  PASSED
            sts_runs|   2|    100000|     100|0.98641542|  PASSED
          sts_serial|   1|    100000|     100|0.53853951|  PASSED
          sts_serial|   2|    100000|     100|0.70424586|  PASSED
          sts_serial|   3|    100000|     100|0.96506722|  PASSED
          sts_serial|   3|    100000|     100|0.65542711|  PASSED
          sts_serial|   4|    100000|     100|0.88312950|  PASSED
          sts_serial|   4|    100000|     100|0.51876793|  PASSED
          sts_serial|   5|    100000|     100|0.76214922|  PASSED
          sts_serial|   5|    100000|     100|0.21427023|  PASSED
          sts_serial|   6|    100000|     100|0.90204871|  PASSED
          sts_serial|   6|    100000|     100|0.79763342|  PASSED
          sts_serial|   7|    100000|     100|0.76530561|  PASSED
          sts_serial|   7|    100000|     100|0.30999472|  PASSED
          sts_serial|   8|    100000|     100|0.36637068|  PASSED
          sts_serial|   8|    100000|     100|0.16021901|  PASSED
          sts_serial|   9|    100000|     100|0.37931868|  PASSED
          sts_serial|   9|    100000|     100|0.77379405|  PASSED
          sts_serial|  10|    100000|     100|0.99581838|   WEAK
          sts_serial|  10|    100000|     100|0.88664066|  PASSED
          sts_serial|  11|    100000|     100|0.76801528|  PASSED
          sts_serial|  11|    100000|     100|0.82800991|  PASSED
          sts_serial|  12|    100000|     100|0.11194082|  PASSED
          sts_serial|  12|    100000|     100|0.22372524|  PASSED
          sts_serial|  13|    100000|     100|0.39905546|  PASSED
          sts_serial|  13|    100000|     100|0.73698133|  PASSED
          sts_serial|  14|    100000|     100|0.31380011|  PASSED
          sts_serial|  14|    100000|     100|0.35392552|  PASSED
          sts_serial|  15|    100000|     100|0.18914661|  PASSED
          sts_serial|  15|    100000|     100|0.15664135|  PASSED
          sts_serial|  16|    100000|     100|0.14665341|  PASSED
          sts_serial|  16|    100000|     100|0.68584797|  PASSED
          sts_serial|   1|    100000|     200|0.14616579|  PASSED
          sts_serial|   2|    100000|     200|0.33227912|  PASSED
          sts_serial|   3|    100000|     200|0.43927695|  PASSED
          sts_serial|   3|    100000|     200|0.27806076|  PASSED
          sts_serial|   4|    100000|     200|0.96188539|  PASSED
          sts_serial|   4|    100000|     200|0.08077551|  PASSED
          sts_serial|   5|    100000|     200|0.65933459|  PASSED
          sts_serial|   5|    100000|     200|0.22963145|  PASSED
          sts_serial|   6|    100000|     200|0.56571499|  PASSED
          sts_serial|   6|    100000|     200|0.18318542|  PASSED
          sts_serial|   7|    100000|     200|0.51057524|  PASSED
          sts_serial|   7|    100000|     200|0.18089407|  PASSED
          sts_serial|   8|    100000|     200|0.35604349|  PASSED
          sts_serial|   8|    100000|     200|0.40304211|  PASSED
          sts_serial|   9|    100000|     200|0.76408872|  PASSED
          sts_serial|   9|    100000|     200|0.95273185|  PASSED
          sts_serial|  10|    100000|     200|0.80839950|  PASSED
          sts_serial|  10|    100000|     200|0.54028571|  PASSED
          sts_serial|  11|    100000|     200|0.41294571|  PASSED
          sts_serial|  11|    100000|     200|0.78724198|  PASSED
          sts_serial|  12|    100000|     200|0.05113071|  PASSED
          sts_serial|  12|    100000|     200|0.39448033|  PASSED
          sts_serial|  13|    100000|     200|0.12212867|  PASSED
          sts_serial|  13|    100000|     200|0.56044107|  PASSED
          sts_serial|  14|    100000|     200|0.09281828|  PASSED
          sts_serial|  14|    100000|     200|0.38719729|  PASSED
          sts_serial|  15|    100000|     200|0.41297447|  PASSED
          sts_serial|  15|    100000|     200|0.29324158|  PASSED
          sts_serial|  16|    100000|     200|0.12077055|  PASSED
          sts_serial|  16|    100000|     200|0.95343896|  PASSED
         rgb_bitdist|   1|    100000|     100|0.44955797|  PASSED
         rgb_bitdist|   2|    100000|     100|0.36750579|  PASSED
         rgb_bitdist|   3|    100000|     100|0.84794442|  PASSED
         rgb_bitdist|   4|    100000|     100|0.35777443|  PASSED
         rgb_bitdist|   5|    100000|     100|0.61581322|  PASSED
         rgb_bitdist|   6|    100000|     100|0.92343470|  PASSED
         rgb_bitdist|   7|    100000|     100|0.76128295|  PASSED
         rgb_bitdist|   8|    100000|     100|0.74379843|  PASSED
         rgb_bitdist|   9|    100000|     100|0.75402183|  PASSED
         rgb_bitdist|  10|    100000|     100|0.07179968|  PASSED
         rgb_bitdist|  11|    100000|     100|0.92842765|  PASSED
         rgb_bitdist|  12|    100000|     100|0.26134736|  PASSED
rgb_minimum_distance|   2|     10000|    1000|0.67990100|  PASSED
rgb_minimum_distance|   3|     10000|    1000|0.08683582|  PASSED
rgb_minimum_distance|   4|     10000|    1000|0.94180883|  PASSED
rgb_minimum_distance|   5|     10000|    1000|0.33689745|  PASSED
    rgb_permutations|   2|    100000|     100|0.25393941|  PASSED
    rgb_permutations|   3|    100000|     100|0.76791678|  PASSED
    rgb_permutations|   4|    100000|     100|0.99051602|  PASSED
    rgb_permutations|   5|    100000|     100|0.78590914|  PASSED
      rgb_lagged_sum|   0|   1000000|     100|0.57815982|  PASSED
      rgb_lagged_sum|   1|   1000000|     100|0.06424954|  PASSED
      rgb_lagged_sum|   2|   1000000|     100|0.76368224|  PASSED
      rgb_lagged_sum|   3|   1000000|     100|0.57433562|  PASSED
      rgb_lagged_sum|   4|   1000000|     100|0.01684553|  PASSED
      rgb_lagged_sum|   5|   1000000|     100|0.35834799|  PASSED
      rgb_lagged_sum|   6|   1000000|     100|0.61001398|  PASSED
      rgb_lagged_sum|   7|   1000000|     100|0.62421266|  PASSED
      rgb_lagged_sum|   8|   1000000|     100|0.88989732|  PASSED
      rgb_lagged_sum|   9|   1000000|     100|0.86525276|  PASSED
      rgb_lagged_sum|  10|   1000000|     100|0.73744893|  PASSED
      rgb_lagged_sum|  11|   1000000|     100|0.63316274|  PASSED
      rgb_lagged_sum|  12|   1000000|     100|0.12006619|  PASSED
      rgb_lagged_sum|  13|   1000000|     100|0.79019915|  PASSED
      rgb_lagged_sum|  14|   1000000|     100|0.30371771|  PASSED
      rgb_lagged_sum|  15|   1000000|     100|0.94126753|  PASSED
      rgb_lagged_sum|  16|   1000000|     100|0.85641134|  PASSED
      rgb_lagged_sum|  17|   1000000|     100|0.24325745|  PASSED
      rgb_lagged_sum|  18|   1000000|     100|0.63633569|  PASSED
      rgb_lagged_sum|  19|   1000000|     100|0.98002635|  PASSED
      rgb_lagged_sum|  20|   1000000|     100|0.42030287|  PASSED
      rgb_lagged_sum|  21|   1000000|     100|0.54295046|  PASSED
      rgb_lagged_sum|  22|   1000000|     100|0.27653423|  PASSED
      rgb_lagged_sum|  23|   1000000|     100|0.59050249|  PASSED
      rgb_lagged_sum|  24|   1000000|     100|0.78746596|  PASSED
      rgb_lagged_sum|  25|   1000000|     100|0.23877934|  PASSED
      rgb_lagged_sum|  26|   1000000|     100|0.95978547|  PASSED
      rgb_lagged_sum|  27|   1000000|     100|0.13671078|  PASSED
      rgb_lagged_sum|  28|   1000000|     100|0.99656967|   WEAK
      rgb_lagged_sum|  28|   1000000|     200|0.45338072|  PASSED
      rgb_lagged_sum|  29|   1000000|     100|0.33542744|  PASSED
      rgb_lagged_sum|  30|   1000000|     100|0.82899800|  PASSED
      rgb_lagged_sum|  31|   1000000|     100|0.80961599|  PASSED
      rgb_lagged_sum|  32|   1000000|     100|0.65266391|  PASSED
     rgb_kstest_test|   0|     10000|    1000|0.22113767|  PASSED
     dab_bytedistrib|   0|  51200000|       1|0.38850445|  PASSED
             dab_dct| 256|     50000|       1|0.34083378|  PASSED
        dab_filltree|  32|  15000000|       1|0.27558556|  PASSED
        dab_filltree|  32|  15000000|       1|0.22017168|  PASSED
       dab_filltree2|   0|   5000000|       1|0.30648438|  PASSED
       dab_filltree2|   1|   5000000|       1|0.62143179|  PASSED
        dab_monobit2|  12|  65000000|       1|0.58175537|  PASSED

real    34m5.210s
user    43m30.760s
sys     3m13.612s
```

### lcg2
```dieharder
Proceeding with test 2...
#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |rands/second|   Seed   |
stdin_input_raw|  6.78e+07  |1039957948|
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.18239170|  PASSED
      diehard_operm5|   0|   1000000|     100|0.08220352|  PASSED
  diehard_rank_32x32|   0|     40000|     100|0.97999508|  PASSED
    diehard_rank_6x8|   0|    100000|     100|0.00000000|  FAILED
   diehard_bitstream|   0|   2097152|     100|0.00000000|  FAILED
        diehard_opso|   0|   2097152|     100|0.00000000|  FAILED
        diehard_oqso|   0|   2097152|     100|0.00000000|  FAILED
         diehard_dna|   0|   2097152|     100|0.00000000|  FAILED
diehard_count_1s_str|   0|    256000|     100|0.00000000|  FAILED
diehard_count_1s_byt|   0|    256000|     100|0.00000000|  FAILED
 diehard_parking_lot|   0|     12000|     100|0.38232396|  PASSED
    diehard_2dsphere|   2|      8000|     100|0.08016803|  PASSED
    diehard_3dsphere|   3|      4000|     100|0.02304711|  PASSED
     diehard_squeeze|   0|    100000|     100|0.86029977|  PASSED
        diehard_sums|   0|       100|     100|0.60204831|  PASSED
        diehard_runs|   0|    100000|     100|0.55211834|  PASSED
        diehard_runs|   0|    100000|     100|0.99216828|  PASSED
       diehard_craps|   0|    200000|     100|0.95369538|  PASSED
       diehard_craps|   0|    200000|     100|0.61599063|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.00000000|  FAILED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.00000000|  FAILED
         sts_monobit|   1|    100000|     100|0.17769920|  PASSED
            sts_runs|   2|    100000|     100|0.05677104|  PASSED
          sts_serial|   1|    100000|     100|0.14816961|  PASSED
          sts_serial|   2|    100000|     100|0.00000007|  FAILED
          sts_serial|   3|    100000|     100|0.00000000|  FAILED
          sts_serial|   3|    100000|     100|0.00001385|   WEAK
          sts_serial|   4|    100000|     100|0.00000000|  FAILED
          sts_serial|   4|    100000|     100|0.00000113|   WEAK
          sts_serial|   5|    100000|     100|0.00000000|  FAILED
          sts_serial|   5|    100000|     100|0.00000000|  FAILED
          sts_serial|   6|    100000|     100|0.00000000|  FAILED
          sts_serial|   6|    100000|     100|0.00000000|  FAILED
          sts_serial|   7|    100000|     100|0.00000000|  FAILED
          sts_serial|   7|    100000|     100|0.00000000|  FAILED
          sts_serial|   8|    100000|     100|0.00000000|  FAILED
          sts_serial|   8|    100000|     100|0.00000000|  FAILED
          sts_serial|   9|    100000|     100|0.00000000|  FAILED
          sts_serial|   9|    100000|     100|0.00000000|  FAILED
          sts_serial|  10|    100000|     100|0.00000000|  FAILED
          sts_serial|  10|    100000|     100|0.00000000|  FAILED
          sts_serial|  11|    100000|     100|0.00000000|  FAILED
          sts_serial|  11|    100000|     100|0.00000000|  FAILED
          sts_serial|  12|    100000|     100|0.00000000|  FAILED
          sts_serial|  12|    100000|     100|0.00000000|  FAILED
          sts_serial|  13|    100000|     100|0.00000000|  FAILED
          sts_serial|  13|    100000|     100|0.00000000|  FAILED
          sts_serial|  14|    100000|     100|0.00000000|  FAILED
          sts_serial|  14|    100000|     100|0.00000000|  FAILED
          sts_serial|  15|    100000|     100|0.00000000|  FAILED
          sts_serial|  15|    100000|     100|0.00000000|  FAILED
          sts_serial|  16|    100000|     100|0.00000000|  FAILED
          sts_serial|  16|    100000|     100|0.00000000|  FAILED
         rgb_bitdist|   1|    100000|     100|0.00000000|  FAILED
         rgb_bitdist|   2|    100000|     100|0.00000000|  FAILED
         rgb_bitdist|   3|    100000|     100|0.00000000|  FAILED
         rgb_bitdist|   4|    100000|     100|0.00000000|  FAILED
         rgb_bitdist|   5|    100000|     100|0.00000000|  FAILED
         rgb_bitdist|   6|    100000|     100|0.00000000|  FAILED
         rgb_bitdist|   7|    100000|     100|0.00000000|  FAILED
         rgb_bitdist|   8|    100000|     100|0.00000000|  FAILED
         rgb_bitdist|   9|    100000|     100|0.00001849|   WEAK
         rgb_bitdist|   9|    100000|     200|0.00000000|  FAILED
         rgb_bitdist|  10|    100000|     100|0.00466185|   WEAK
         rgb_bitdist|  10|    100000|     200|0.00004169|   WEAK
         rgb_bitdist|  10|    100000|     300|0.00000405|   WEAK
         rgb_bitdist|  10|    100000|     400|0.00000001|  FAILED
         rgb_bitdist|  11|    100000|     100|0.06176102|  PASSED
         rgb_bitdist|  12|    100000|     100|0.01292240|  PASSED
rgb_minimum_distance|   2|     10000|    1000|0.00000280|   WEAK
rgb_minimum_distance|   2|     10000|    1100|0.00000046|  FAILED
rgb_minimum_distance|   3|     10000|    1000|0.00000000|  FAILED
rgb_minimum_distance|   4|     10000|    1000|0.00000000|  FAILED
rgb_minimum_distance|   5|     10000|    1000|0.00000000|  FAILED
    rgb_permutations|   2|    100000|     100|0.31163984|  PASSED
    rgb_permutations|   3|    100000|     100|0.62790419|  PASSED
    rgb_permutations|   4|    100000|     100|0.31753963|  PASSED
    rgb_permutations|   5|    100000|     100|0.63901564|  PASSED
      rgb_lagged_sum|   0|   1000000|     100|0.16584927|  PASSED
      rgb_lagged_sum|   1|   1000000|     100|0.94973738|  PASSED
      rgb_lagged_sum|   2|   1000000|     100|0.57397559|  PASSED
      rgb_lagged_sum|   3|   1000000|     100|0.24670626|  PASSED
      rgb_lagged_sum|   4|   1000000|     100|0.99138207|  PASSED
      rgb_lagged_sum|   5|   1000000|     100|0.13523001|  PASSED
      rgb_lagged_sum|   6|   1000000|     100|0.09283530|  PASSED
      rgb_lagged_sum|   7|   1000000|     100|0.91873771|  PASSED
      rgb_lagged_sum|   8|   1000000|     100|0.53453056|  PASSED
      rgb_lagged_sum|   9|   1000000|     100|0.95453201|  PASSED
      rgb_lagged_sum|  10|   1000000|     100|0.55268456|  PASSED
      rgb_lagged_sum|  11|   1000000|     100|0.58802995|  PASSED
      rgb_lagged_sum|  12|   1000000|     100|0.60337550|  PASSED
      rgb_lagged_sum|  13|   1000000|     100|0.92631808|  PASSED
      rgb_lagged_sum|  14|   1000000|     100|0.37145119|  PASSED
      rgb_lagged_sum|  15|   1000000|     100|0.89153804|  PASSED
      rgb_lagged_sum|  16|   1000000|     100|0.98742048|  PASSED
      rgb_lagged_sum|  17|   1000000|     100|0.84737309|  PASSED
      rgb_lagged_sum|  18|   1000000|     100|0.26088204|  PASSED
      rgb_lagged_sum|  19|   1000000|     100|0.98609510|  PASSED
      rgb_lagged_sum|  20|   1000000|     100|0.91702422|  PASSED
      rgb_lagged_sum|  21|   1000000|     100|0.81805952|  PASSED
      rgb_lagged_sum|  22|   1000000|     100|0.88099331|  PASSED
      rgb_lagged_sum|  23|   1000000|     100|0.19365103|  PASSED
      rgb_lagged_sum|  24|   1000000|     100|0.54870579|  PASSED
      rgb_lagged_sum|  25|   1000000|     100|0.54330647|  PASSED
      rgb_lagged_sum|  26|   1000000|     100|0.42251350|  PASSED
      rgb_lagged_sum|  27|   1000000|     100|0.10712485|  PASSED
      rgb_lagged_sum|  28|   1000000|     100|0.37963593|  PASSED
      rgb_lagged_sum|  29|   1000000|     100|0.76784220|  PASSED
      rgb_lagged_sum|  30|   1000000|     100|0.01567055|  PASSED
      rgb_lagged_sum|  31|   1000000|     100|0.48146287|  PASSED
      rgb_lagged_sum|  32|   1000000|     100|0.92155118|  PASSED
     rgb_kstest_test|   0|     10000|    1000|0.35709674|  PASSED
     dab_bytedistrib|   0|  51200000|       1|1.00000000|  FAILED
             dab_dct| 256|     50000|       1|0.00000000|  FAILED
        dab_filltree|  32|  15000000|       1|0.36136249|  PASSED
        dab_filltree|  32|  15000000|       1|0.58724942|  PASSED
       dab_filltree2|   0|   5000000|       1|0.00000000|  FAILED
       dab_filltree2|   1|   5000000|       1|0.00000000|  FAILED
        dab_monobit2|  12|  65000000|       1|1.00000000|  FAILED

real    32m29.536s
user    51m25.599s
sys     2m4.070s
```

### pcg
```dieharder
Proceeding with test 3...
#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |rands/second|   Seed   |
stdin_input_raw|  7.45e+07  |3648078200|
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.63732197|  PASSED
      diehard_operm5|   0|   1000000|     100|0.74805909|  PASSED
  diehard_rank_32x32|   0|     40000|     100|0.40467657|  PASSED
    diehard_rank_6x8|   0|    100000|     100|0.94182625|  PASSED
   diehard_bitstream|   0|   2097152|     100|0.80262975|  PASSED
        diehard_opso|   0|   2097152|     100|0.66029399|  PASSED
        diehard_oqso|   0|   2097152|     100|0.99967748|   WEAK
        diehard_oqso|   0|   2097152|     200|0.99775239|   WEAK
        diehard_oqso|   0|   2097152|     300|0.93533070|  PASSED
         diehard_dna|   0|   2097152|     100|0.26759731|  PASSED
diehard_count_1s_str|   0|    256000|     100|0.22594823|  PASSED
diehard_count_1s_byt|   0|    256000|     100|0.66177357|  PASSED
 diehard_parking_lot|   0|     12000|     100|0.66609484|  PASSED
    diehard_2dsphere|   2|      8000|     100|0.19911018|  PASSED
    diehard_3dsphere|   3|      4000|     100|0.80812860|  PASSED
     diehard_squeeze|   0|    100000|     100|0.14555057|  PASSED
        diehard_sums|   0|       100|     100|0.52779777|  PASSED
        diehard_runs|   0|    100000|     100|0.78264259|  PASSED
        diehard_runs|   0|    100000|     100|0.16299901|  PASSED
       diehard_craps|   0|    200000|     100|0.03971184|  PASSED
       diehard_craps|   0|    200000|     100|0.79556783|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.73991904|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.21735202|  PASSED
         sts_monobit|   1|    100000|     100|0.01011321|  PASSED
            sts_runs|   2|    100000|     100|0.16649590|  PASSED
          sts_serial|   1|    100000|     100|0.54583455|  PASSED
          sts_serial|   2|    100000|     100|0.22898848|  PASSED
          sts_serial|   3|    100000|     100|0.43934035|  PASSED
          sts_serial|   3|    100000|     100|0.79156144|  PASSED
          sts_serial|   4|    100000|     100|0.96619670|  PASSED
          sts_serial|   4|    100000|     100|0.62149638|  PASSED
          sts_serial|   5|    100000|     100|0.23243081|  PASSED
          sts_serial|   5|    100000|     100|0.25672761|  PASSED
          sts_serial|   6|    100000|     100|0.02449885|  PASSED
          sts_serial|   6|    100000|     100|0.73818292|  PASSED
          sts_serial|   7|    100000|     100|0.51995340|  PASSED
          sts_serial|   7|    100000|     100|0.55897034|  PASSED
          sts_serial|   8|    100000|     100|0.98179995|  PASSED
          sts_serial|   8|    100000|     100|0.73672710|  PASSED
          sts_serial|   9|    100000|     100|0.61920724|  PASSED
          sts_serial|   9|    100000|     100|0.25039215|  PASSED
          sts_serial|  10|    100000|     100|0.90956256|  PASSED
          sts_serial|  10|    100000|     100|0.51635661|  PASSED
          sts_serial|  11|    100000|     100|0.53974345|  PASSED
          sts_serial|  11|    100000|     100|0.17424404|  PASSED
          sts_serial|  12|    100000|     100|0.97449861|  PASSED
          sts_serial|  12|    100000|     100|0.88199676|  PASSED
          sts_serial|  13|    100000|     100|0.36254173|  PASSED
          sts_serial|  13|    100000|     100|0.23768443|  PASSED
          sts_serial|  14|    100000|     100|0.15037681|  PASSED
          sts_serial|  14|    100000|     100|0.81097511|  PASSED
          sts_serial|  15|    100000|     100|0.91640522|  PASSED
          sts_serial|  15|    100000|     100|0.97525463|  PASSED
          sts_serial|  16|    100000|     100|0.94865801|  PASSED
          sts_serial|  16|    100000|     100|0.39776814|  PASSED
         rgb_bitdist|   1|    100000|     100|0.86550684|  PASSED
         rgb_bitdist|   2|    100000|     100|0.21426081|  PASSED
         rgb_bitdist|   3|    100000|     100|0.10969817|  PASSED
         rgb_bitdist|   4|    100000|     100|0.56142581|  PASSED
         rgb_bitdist|   5|    100000|     100|0.48447428|  PASSED
         rgb_bitdist|   6|    100000|     100|0.08515734|  PASSED
         rgb_bitdist|   7|    100000|     100|0.87904263|  PASSED
         rgb_bitdist|   8|    100000|     100|0.96601006|  PASSED
         rgb_bitdist|   9|    100000|     100|0.97531367|  PASSED
         rgb_bitdist|  10|    100000|     100|0.41108122|  PASSED
         rgb_bitdist|  11|    100000|     100|0.17222689|  PASSED
         rgb_bitdist|  12|    100000|     100|0.20870315|  PASSED
rgb_minimum_distance|   2|     10000|    1000|0.19526993|  PASSED
rgb_minimum_distance|   3|     10000|    1000|0.95943807|  PASSED
rgb_minimum_distance|   4|     10000|    1000|0.45604526|  PASSED
rgb_minimum_distance|   5|     10000|    1000|0.10684121|  PASSED
    rgb_permutations|   2|    100000|     100|0.95525853|  PASSED
    rgb_permutations|   3|    100000|     100|0.78790737|  PASSED
    rgb_permutations|   4|    100000|     100|0.40266690|  PASSED
    rgb_permutations|   5|    100000|     100|0.73102616|  PASSED
      rgb_lagged_sum|   0|   1000000|     100|0.91280248|  PASSED
      rgb_lagged_sum|   1|   1000000|     100|0.80982184|  PASSED
      rgb_lagged_sum|   2|   1000000|     100|0.99229451|  PASSED
      rgb_lagged_sum|   3|   1000000|     100|0.14511299|  PASSED
      rgb_lagged_sum|   4|   1000000|     100|0.83200017|  PASSED
      rgb_lagged_sum|   5|   1000000|     100|0.49950952|  PASSED
      rgb_lagged_sum|   6|   1000000|     100|0.51930677|  PASSED
      rgb_lagged_sum|   7|   1000000|     100|0.86953285|  PASSED
      rgb_lagged_sum|   8|   1000000|     100|0.04134828|  PASSED
      rgb_lagged_sum|   9|   1000000|     100|0.24186104|  PASSED
      rgb_lagged_sum|  10|   1000000|     100|0.73994601|  PASSED
      rgb_lagged_sum|  11|   1000000|     100|0.87167765|  PASSED
      rgb_lagged_sum|  12|   1000000|     100|0.29758902|  PASSED
      rgb_lagged_sum|  13|   1000000|     100|0.37995468|  PASSED
      rgb_lagged_sum|  14|   1000000|     100|0.47040886|  PASSED
      rgb_lagged_sum|  15|   1000000|     100|0.48695928|  PASSED
      rgb_lagged_sum|  16|   1000000|     100|0.82951031|  PASSED
      rgb_lagged_sum|  17|   1000000|     100|0.50266070|  PASSED
      rgb_lagged_sum|  18|   1000000|     100|0.61839900|  PASSED
      rgb_lagged_sum|  19|   1000000|     100|0.92577354|  PASSED
      rgb_lagged_sum|  20|   1000000|     100|0.89003579|  PASSED
      rgb_lagged_sum|  21|   1000000|     100|0.20636591|  PASSED
      rgb_lagged_sum|  22|   1000000|     100|0.84200002|  PASSED
      rgb_lagged_sum|  23|   1000000|     100|0.96673980|  PASSED
      rgb_lagged_sum|  24|   1000000|     100|0.64099317|  PASSED
      rgb_lagged_sum|  25|   1000000|     100|0.47716601|  PASSED
      rgb_lagged_sum|  26|   1000000|     100|0.76870266|  PASSED
      rgb_lagged_sum|  27|   1000000|     100|0.98441915|  PASSED
      rgb_lagged_sum|  28|   1000000|     100|0.84478636|  PASSED
      rgb_lagged_sum|  29|   1000000|     100|0.69961538|  PASSED
      rgb_lagged_sum|  30|   1000000|     100|0.78122189|  PASSED
      rgb_lagged_sum|  31|   1000000|     100|0.98860679|  PASSED
      rgb_lagged_sum|  32|   1000000|     100|0.94189267|  PASSED
     rgb_kstest_test|   0|     10000|    1000|0.50779821|  PASSED
     dab_bytedistrib|   0|  51200000|       1|0.45019745|  PASSED
             dab_dct| 256|     50000|       1|0.11075047|  PASSED
        dab_filltree|  32|  15000000|       1|0.91331426|  PASSED
        dab_filltree|  32|  15000000|       1|0.95647887|  PASSED
       dab_filltree2|   0|   5000000|       1|0.73505264|  PASSED
       dab_filltree2|   1|   5000000|       1|0.30691804|  PASSED
        dab_monobit2|  12|  65000000|       1|0.82977657|  PASSED

real    28m20.400s
user    42m53.851s
sys     2m21.141s
```

### photon
```dieharder
Proceeding with test 4...
#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |rands/second|   Seed   |
stdin_input_raw|  2.44e+07  |1143382116|
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.83378898|  PASSED
      diehard_operm5|   0|   1000000|     100|0.11074811|  PASSED
  diehard_rank_32x32|   0|     40000|     100|0.10940568|  PASSED
    diehard_rank_6x8|   0|    100000|     100|0.66687109|  PASSED
   diehard_bitstream|   0|   2097152|     100|0.65339114|  PASSED
        diehard_opso|   0|   2097152|     100|0.20381727|  PASSED
        diehard_oqso|   0|   2097152|     100|0.45028388|  PASSED
         diehard_dna|   0|   2097152|     100|0.28567043|  PASSED
diehard_count_1s_str|   0|    256000|     100|0.99999549|   WEAK
diehard_count_1s_str|   0|    256000|     200|0.97766955|  PASSED
diehard_count_1s_byt|   0|    256000|     100|0.03622932|  PASSED
 diehard_parking_lot|   0|     12000|     100|0.79390338|  PASSED
    diehard_2dsphere|   2|      8000|     100|0.76011928|  PASSED
    diehard_3dsphere|   3|      4000|     100|0.90793543|  PASSED
     diehard_squeeze|   0|    100000|     100|0.99280336|  PASSED
        diehard_sums|   0|       100|     100|0.67798045|  PASSED
        diehard_runs|   0|    100000|     100|0.79268680|  PASSED
        diehard_runs|   0|    100000|     100|0.59688401|  PASSED
       diehard_craps|   0|    200000|     100|0.63862533|  PASSED
       diehard_craps|   0|    200000|     100|0.92353579|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.02817940|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.31391734|  PASSED
         sts_monobit|   1|    100000|     100|0.10093663|  PASSED
            sts_runs|   2|    100000|     100|0.23499202|  PASSED
          sts_serial|   1|    100000|     100|0.00630499|  PASSED
          sts_serial|   2|    100000|     100|0.72057382|  PASSED
          sts_serial|   3|    100000|     100|0.96312442|  PASSED
          sts_serial|   3|    100000|     100|0.99899415|   WEAK
          sts_serial|   4|    100000|     100|0.99816826|   WEAK
          sts_serial|   4|    100000|     100|0.91586245|  PASSED
          sts_serial|   5|    100000|     100|0.82633694|  PASSED
          sts_serial|   5|    100000|     100|0.19587393|  PASSED
          sts_serial|   6|    100000|     100|0.91761069|  PASSED
          sts_serial|   6|    100000|     100|0.52513922|  PASSED
          sts_serial|   7|    100000|     100|0.45153034|  PASSED
          sts_serial|   7|    100000|     100|0.03322652|  PASSED
          sts_serial|   8|    100000|     100|0.88769854|  PASSED
          sts_serial|   8|    100000|     100|0.43671629|  PASSED
          sts_serial|   9|    100000|     100|0.97590842|  PASSED
          sts_serial|   9|    100000|     100|0.69257840|  PASSED
          sts_serial|  10|    100000|     100|0.59833994|  PASSED
          sts_serial|  10|    100000|     100|0.66499850|  PASSED
          sts_serial|  11|    100000|     100|0.72982712|  PASSED
          sts_serial|  11|    100000|     100|0.98827266|  PASSED
          sts_serial|  12|    100000|     100|0.87114633|  PASSED
          sts_serial|  12|    100000|     100|0.21551763|  PASSED
          sts_serial|  13|    100000|     100|0.73422617|  PASSED
          sts_serial|  13|    100000|     100|0.22856539|  PASSED
          sts_serial|  14|    100000|     100|0.90118270|  PASSED
          sts_serial|  14|    100000|     100|0.75454927|  PASSED
          sts_serial|  15|    100000|     100|0.83366533|  PASSED
          sts_serial|  15|    100000|     100|0.67909453|  PASSED
          sts_serial|  16|    100000|     100|0.58579409|  PASSED
          sts_serial|  16|    100000|     100|0.60915688|  PASSED
          sts_serial|   1|    100000|     200|0.28162005|  PASSED
          sts_serial|   2|    100000|     200|0.78471420|  PASSED
          sts_serial|   3|    100000|     200|0.48246711|  PASSED
          sts_serial|   3|    100000|     200|0.74842197|  PASSED
          sts_serial|   4|    100000|     200|0.79904627|  PASSED
          sts_serial|   4|    100000|     200|0.56607075|  PASSED
          sts_serial|   5|    100000|     200|0.87822180|  PASSED
          sts_serial|   5|    100000|     200|0.45140129|  PASSED
          sts_serial|   6|    100000|     200|0.83790436|  PASSED
          sts_serial|   6|    100000|     200|0.33834889|  PASSED
          sts_serial|   7|    100000|     200|0.22124561|  PASSED
          sts_serial|   7|    100000|     200|0.29412559|  PASSED
          sts_serial|   8|    100000|     200|0.70252801|  PASSED
          sts_serial|   8|    100000|     200|0.40120801|  PASSED
          sts_serial|   9|    100000|     200|0.86453815|  PASSED
          sts_serial|   9|    100000|     200|0.52980563|  PASSED
          sts_serial|  10|    100000|     200|0.47597053|  PASSED
          sts_serial|  10|    100000|     200|0.31035891|  PASSED
          sts_serial|  11|    100000|     200|0.53481932|  PASSED
          sts_serial|  11|    100000|     200|0.91696768|  PASSED
          sts_serial|  12|    100000|     200|0.25138889|  PASSED
          sts_serial|  12|    100000|     200|0.27248262|  PASSED
          sts_serial|  13|    100000|     200|0.36713763|  PASSED
          sts_serial|  13|    100000|     200|0.67522776|  PASSED
          sts_serial|  14|    100000|     200|0.12145974|  PASSED
          sts_serial|  14|    100000|     200|0.19604013|  PASSED
          sts_serial|  15|    100000|     200|0.35014771|  PASSED
          sts_serial|  15|    100000|     200|0.80488704|  PASSED
          sts_serial|  16|    100000|     200|0.74213827|  PASSED
          sts_serial|  16|    100000|     200|0.67455752|  PASSED
         rgb_bitdist|   1|    100000|     100|0.99857242|   WEAK
         rgb_bitdist|   1|    100000|     200|0.38663228|  PASSED
         rgb_bitdist|   2|    100000|     100|0.37657927|  PASSED
         rgb_bitdist|   3|    100000|     100|0.91178308|  PASSED
         rgb_bitdist|   4|    100000|     100|0.77225807|  PASSED
         rgb_bitdist|   5|    100000|     100|0.47436150|  PASSED
         rgb_bitdist|   6|    100000|     100|0.21963449|  PASSED
         rgb_bitdist|   7|    100000|     100|0.97628330|  PASSED
         rgb_bitdist|   8|    100000|     100|0.38024744|  PASSED
         rgb_bitdist|   9|    100000|     100|0.19552659|  PASSED
         rgb_bitdist|  10|    100000|     100|0.75937953|  PASSED
         rgb_bitdist|  11|    100000|     100|0.53867016|  PASSED
         rgb_bitdist|  12|    100000|     100|0.61458958|  PASSED
rgb_minimum_distance|   2|     10000|    1000|0.20940127|  PASSED
rgb_minimum_distance|   3|     10000|    1000|0.76364138|  PASSED
rgb_minimum_distance|   4|     10000|    1000|0.76042473|  PASSED
rgb_minimum_distance|   5|     10000|    1000|0.83217621|  PASSED
    rgb_permutations|   2|    100000|     100|0.63807116|  PASSED
    rgb_permutations|   3|    100000|     100|0.95653926|  PASSED
    rgb_permutations|   4|    100000|     100|0.01501899|  PASSED
    rgb_permutations|   5|    100000|     100|0.27755206|  PASSED
      rgb_lagged_sum|   0|   1000000|     100|0.87512749|  PASSED
      rgb_lagged_sum|   1|   1000000|     100|0.16432891|  PASSED
      rgb_lagged_sum|   2|   1000000|     100|0.77188181|  PASSED
      rgb_lagged_sum|   3|   1000000|     100|0.88897657|  PASSED
      rgb_lagged_sum|   4|   1000000|     100|0.73679350|  PASSED
      rgb_lagged_sum|   5|   1000000|     100|0.35856290|  PASSED
      rgb_lagged_sum|   6|   1000000|     100|0.97328210|  PASSED
      rgb_lagged_sum|   7|   1000000|     100|0.38922204|  PASSED
      rgb_lagged_sum|   8|   1000000|     100|0.35127684|  PASSED
      rgb_lagged_sum|   9|   1000000|     100|0.29714838|  PASSED
      rgb_lagged_sum|  10|   1000000|     100|0.38609845|  PASSED
      rgb_lagged_sum|  11|   1000000|     100|0.92690122|  PASSED
      rgb_lagged_sum|  12|   1000000|     100|0.47102391|  PASSED
      rgb_lagged_sum|  13|   1000000|     100|0.74530171|  PASSED
      rgb_lagged_sum|  14|   1000000|     100|0.92930924|  PASSED
      rgb_lagged_sum|  15|   1000000|     100|0.27510925|  PASSED
      rgb_lagged_sum|  16|   1000000|     100|0.11961240|  PASSED
      rgb_lagged_sum|  17|   1000000|     100|0.88942768|  PASSED
      rgb_lagged_sum|  18|   1000000|     100|0.94437310|  PASSED
      rgb_lagged_sum|  19|   1000000|     100|0.73655266|  PASSED
      rgb_lagged_sum|  20|   1000000|     100|0.43087842|  PASSED
      rgb_lagged_sum|  21|   1000000|     100|0.72917059|  PASSED
      rgb_lagged_sum|  22|   1000000|     100|0.12442657|  PASSED
      rgb_lagged_sum|  23|   1000000|     100|0.25731968|  PASSED
      rgb_lagged_sum|  24|   1000000|     100|0.20180897|  PASSED
      rgb_lagged_sum|  25|   1000000|     100|0.84251237|  PASSED
      rgb_lagged_sum|  26|   1000000|     100|0.93030318|  PASSED
      rgb_lagged_sum|  27|   1000000|     100|0.78275204|  PASSED
      rgb_lagged_sum|  28|   1000000|     100|0.61655765|  PASSED
      rgb_lagged_sum|  29|   1000000|     100|0.99887114|   WEAK
      rgb_lagged_sum|  29|   1000000|     200|0.75144744|  PASSED
      rgb_lagged_sum|  30|   1000000|     100|0.08702737|  PASSED
      rgb_lagged_sum|  31|   1000000|     100|0.08462572|  PASSED
      rgb_lagged_sum|  32|   1000000|     100|0.64105476|  PASSED
     rgb_kstest_test|   0|     10000|    1000|0.46945209|  PASSED
     dab_bytedistrib|   0|  51200000|       1|0.16206419|  PASSED
             dab_dct| 256|     50000|       1|0.67850826|  PASSED
        dab_filltree|  32|  15000000|       1|0.37389283|  PASSED
        dab_filltree|  32|  15000000|       1|0.91456452|  PASSED
       dab_filltree2|   0|   5000000|       1|0.34672356|  PASSED
       dab_filltree2|   1|   5000000|       1|0.90522693|  PASSED
        dab_monobit2|  12|  65000000|       1|0.53229643|  PASSED

real    58m0.897s
user    82m24.074s
sys     3m4.735s
```

### romuduo
```dieharder
Proceeding with test 5...
#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |rands/second|   Seed   |
stdin_input_raw|  2.28e+07  | 207561978|
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.28629211|  PASSED
      diehard_operm5|   0|   1000000|     100|0.29095158|  PASSED
  diehard_rank_32x32|   0|     40000|     100|0.94187198|  PASSED
    diehard_rank_6x8|   0|    100000|     100|0.50570452|  PASSED
   diehard_bitstream|   0|   2097152|     100|0.70973100|  PASSED
        diehard_opso|   0|   2097152|     100|0.81396361|  PASSED
        diehard_oqso|   0|   2097152|     100|0.55404492|  PASSED
         diehard_dna|   0|   2097152|     100|0.59576259|  PASSED
diehard_count_1s_str|   0|    256000|     100|0.41150196|  PASSED
diehard_count_1s_byt|   0|    256000|     100|0.59361529|  PASSED
 diehard_parking_lot|   0|     12000|     100|0.61714448|  PASSED
    diehard_2dsphere|   2|      8000|     100|0.72059018|  PASSED
    diehard_3dsphere|   3|      4000|     100|0.51450680|  PASSED
     diehard_squeeze|   0|    100000|     100|0.17329854|  PASSED
        diehard_sums|   0|       100|     100|0.07070376|  PASSED
        diehard_runs|   0|    100000|     100|0.86395764|  PASSED
        diehard_runs|   0|    100000|     100|0.93816198|  PASSED
       diehard_craps|   0|    200000|     100|0.95585222|  PASSED
       diehard_craps|   0|    200000|     100|0.65540547|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.41115609|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.33350928|  PASSED
         sts_monobit|   1|    100000|     100|0.77384565|  PASSED
            sts_runs|   2|    100000|     100|0.70135538|  PASSED
          sts_serial|   1|    100000|     100|0.95172288|  PASSED
          sts_serial|   2|    100000|     100|0.67018252|  PASSED
          sts_serial|   3|    100000|     100|0.98565503|  PASSED
          sts_serial|   3|    100000|     100|0.50531479|  PASSED
          sts_serial|   4|    100000|     100|0.56508693|  PASSED
          sts_serial|   4|    100000|     100|0.46946419|  PASSED
          sts_serial|   5|    100000|     100|0.87113969|  PASSED
          sts_serial|   5|    100000|     100|0.93707026|  PASSED
          sts_serial|   6|    100000|     100|0.85461052|  PASSED
          sts_serial|   6|    100000|     100|0.91110547|  PASSED
          sts_serial|   7|    100000|     100|0.80362203|  PASSED
          sts_serial|   7|    100000|     100|0.80241340|  PASSED
          sts_serial|   8|    100000|     100|0.68647035|  PASSED
          sts_serial|   8|    100000|     100|0.48737781|  PASSED
          sts_serial|   9|    100000|     100|0.84423894|  PASSED
          sts_serial|   9|    100000|     100|0.74808082|  PASSED
          sts_serial|  10|    100000|     100|0.64162208|  PASSED
          sts_serial|  10|    100000|     100|0.78238957|  PASSED
          sts_serial|  11|    100000|     100|0.96683460|  PASSED
          sts_serial|  11|    100000|     100|0.88175769|  PASSED
          sts_serial|  12|    100000|     100|0.65121409|  PASSED
          sts_serial|  12|    100000|     100|0.77086645|  PASSED
          sts_serial|  13|    100000|     100|0.75978986|  PASSED
          sts_serial|  13|    100000|     100|0.83508048|  PASSED
          sts_serial|  14|    100000|     100|0.89055257|  PASSED
          sts_serial|  14|    100000|     100|0.59772729|  PASSED
          sts_serial|  15|    100000|     100|0.98782369|  PASSED
          sts_serial|  15|    100000|     100|0.42381391|  PASSED
          sts_serial|  16|    100000|     100|0.43652211|  PASSED
          sts_serial|  16|    100000|     100|0.61239470|  PASSED
         rgb_bitdist|   1|    100000|     100|0.24302260|  PASSED
         rgb_bitdist|   2|    100000|     100|0.45154419|  PASSED
         rgb_bitdist|   3|    100000|     100|0.46964699|  PASSED
         rgb_bitdist|   4|    100000|     100|0.90462188|  PASSED
         rgb_bitdist|   5|    100000|     100|0.13739692|  PASSED
         rgb_bitdist|   6|    100000|     100|0.29662347|  PASSED
         rgb_bitdist|   7|    100000|     100|0.77454492|  PASSED
         rgb_bitdist|   8|    100000|     100|0.20028003|  PASSED
         rgb_bitdist|   9|    100000|     100|0.98409353|  PASSED
         rgb_bitdist|  10|    100000|     100|0.62696816|  PASSED
         rgb_bitdist|  11|    100000|     100|0.15205917|  PASSED
         rgb_bitdist|  12|    100000|     100|0.91999581|  PASSED
rgb_minimum_distance|   2|     10000|    1000|0.41633235|  PASSED
rgb_minimum_distance|   3|     10000|    1000|0.76361059|  PASSED
rgb_minimum_distance|   4|     10000|    1000|0.85710631|  PASSED
rgb_minimum_distance|   5|     10000|    1000|0.21002021|  PASSED
    rgb_permutations|   2|    100000|     100|0.48482792|  PASSED
    rgb_permutations|   3|    100000|     100|0.69833246|  PASSED
    rgb_permutations|   4|    100000|     100|0.97316861|  PASSED
    rgb_permutations|   5|    100000|     100|0.75353420|  PASSED
      rgb_lagged_sum|   0|   1000000|     100|0.97561747|  PASSED
      rgb_lagged_sum|   1|   1000000|     100|0.31525484|  PASSED
      rgb_lagged_sum|   2|   1000000|     100|0.51624044|  PASSED
      rgb_lagged_sum|   3|   1000000|     100|0.66274828|  PASSED
      rgb_lagged_sum|   4|   1000000|     100|0.11554343|  PASSED
      rgb_lagged_sum|   5|   1000000|     100|0.99324826|  PASSED
      rgb_lagged_sum|   6|   1000000|     100|0.28119155|  PASSED
      rgb_lagged_sum|   7|   1000000|     100|0.06094867|  PASSED
      rgb_lagged_sum|   8|   1000000|     100|0.55195719|  PASSED
      rgb_lagged_sum|   9|   1000000|     100|0.80708004|  PASSED
      rgb_lagged_sum|  10|   1000000|     100|0.73631114|  PASSED
      rgb_lagged_sum|  11|   1000000|     100|0.79222512|  PASSED
      rgb_lagged_sum|  12|   1000000|     100|0.95566101|  PASSED
      rgb_lagged_sum|  13|   1000000|     100|0.95152647|  PASSED
      rgb_lagged_sum|  14|   1000000|     100|0.55058551|  PASSED
      rgb_lagged_sum|  15|   1000000|     100|0.95769136|  PASSED
      rgb_lagged_sum|  16|   1000000|     100|0.51640901|  PASSED
      rgb_lagged_sum|  17|   1000000|     100|0.77053452|  PASSED
      rgb_lagged_sum|  18|   1000000|     100|0.55126342|  PASSED
      rgb_lagged_sum|  19|   1000000|     100|0.69438193|  PASSED
      rgb_lagged_sum|  20|   1000000|     100|0.79371432|  PASSED
      rgb_lagged_sum|  21|   1000000|     100|0.33022895|  PASSED
      rgb_lagged_sum|  22|   1000000|     100|0.96602510|  PASSED
      rgb_lagged_sum|  23|   1000000|     100|0.76983873|  PASSED
      rgb_lagged_sum|  24|   1000000|     100|0.67144067|  PASSED
      rgb_lagged_sum|  25|   1000000|     100|0.30301276|  PASSED
      rgb_lagged_sum|  26|   1000000|     100|0.42719714|  PASSED
      rgb_lagged_sum|  27|   1000000|     100|0.26737030|  PASSED
      rgb_lagged_sum|  28|   1000000|     100|0.97487617|  PASSED
      rgb_lagged_sum|  29|   1000000|     100|0.88250801|  PASSED
      rgb_lagged_sum|  30|   1000000|     100|0.00704064|  PASSED
      rgb_lagged_sum|  31|   1000000|     100|0.23229766|  PASSED
      rgb_lagged_sum|  32|   1000000|     100|0.53856533|  PASSED
     rgb_kstest_test|   0|     10000|    1000|0.01874423|  PASSED
     dab_bytedistrib|   0|  51200000|       1|0.16032598|  PASSED
             dab_dct| 256|     50000|       1|0.52317495|  PASSED
        dab_filltree|  32|  15000000|       1|0.63541947|  PASSED
        dab_filltree|  32|  15000000|       1|0.15860369|  PASSED
       dab_filltree2|   0|   5000000|       1|0.23915073|  PASSED
       dab_filltree2|   1|   5000000|       1|0.30225578|  PASSED
        dab_monobit2|  12|  65000000|       1|0.88992858|  PASSED

real    60m40.636s
user    83m1.317s
sys     3m9.051s
```

### xoroshift
```dieharder
Proceeding with test 6...
#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |rands/second|   Seed   |
stdin_input_raw|  3.45e+07  |2131959076|
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.33457820|  PASSED
      diehard_operm5|   0|   1000000|     100|0.29894566|  PASSED
  diehard_rank_32x32|   0|     40000|     100|0.41803285|  PASSED
    diehard_rank_6x8|   0|    100000|     100|0.94733497|  PASSED
   diehard_bitstream|   0|   2097152|     100|0.86491554|  PASSED
        diehard_opso|   0|   2097152|     100|0.97150238|  PASSED
        diehard_oqso|   0|   2097152|     100|0.91805958|  PASSED
         diehard_dna|   0|   2097152|     100|0.82651836|  PASSED
diehard_count_1s_str|   0|    256000|     100|0.32458885|  PASSED
diehard_count_1s_byt|   0|    256000|     100|0.31254831|  PASSED
 diehard_parking_lot|   0|     12000|     100|0.15798710|  PASSED
    diehard_2dsphere|   2|      8000|     100|0.16667624|  PASSED
    diehard_3dsphere|   3|      4000|     100|0.47575711|  PASSED
     diehard_squeeze|   0|    100000|     100|0.92632662|  PASSED
        diehard_sums|   0|       100|     100|0.29335448|  PASSED
        diehard_runs|   0|    100000|     100|0.68894983|  PASSED
        diehard_runs|   0|    100000|     100|0.41037643|  PASSED
       diehard_craps|   0|    200000|     100|0.16134816|  PASSED
       diehard_craps|   0|    200000|     100|0.57493639|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.37122683|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.86036127|  PASSED
         sts_monobit|   1|    100000|     100|0.64597252|  PASSED
            sts_runs|   2|    100000|     100|0.98641542|  PASSED
          sts_serial|   1|    100000|     100|0.53853951|  PASSED
          sts_serial|   2|    100000|     100|0.70424586|  PASSED
          sts_serial|   3|    100000|     100|0.96506722|  PASSED
          sts_serial|   3|    100000|     100|0.65542711|  PASSED
          sts_serial|   4|    100000|     100|0.88312950|  PASSED
          sts_serial|   4|    100000|     100|0.51876793|  PASSED
          sts_serial|   5|    100000|     100|0.76214922|  PASSED
          sts_serial|   5|    100000|     100|0.21427023|  PASSED
          sts_serial|   6|    100000|     100|0.90204871|  PASSED
          sts_serial|   6|    100000|     100|0.79763342|  PASSED
          sts_serial|   7|    100000|     100|0.76530561|  PASSED
          sts_serial|   7|    100000|     100|0.30999472|  PASSED
          sts_serial|   8|    100000|     100|0.36637068|  PASSED
          sts_serial|   8|    100000|     100|0.16021901|  PASSED
          sts_serial|   9|    100000|     100|0.37931868|  PASSED
          sts_serial|   9|    100000|     100|0.77379405|  PASSED
          sts_serial|  10|    100000|     100|0.99581838|   WEAK
          sts_serial|  10|    100000|     100|0.88664066|  PASSED
          sts_serial|  11|    100000|     100|0.76801528|  PASSED
          sts_serial|  11|    100000|     100|0.82800991|  PASSED
          sts_serial|  12|    100000|     100|0.11194082|  PASSED
          sts_serial|  12|    100000|     100|0.22372524|  PASSED
          sts_serial|  13|    100000|     100|0.39905546|  PASSED
          sts_serial|  13|    100000|     100|0.73698133|  PASSED
          sts_serial|  14|    100000|     100|0.31380011|  PASSED
          sts_serial|  14|    100000|     100|0.35392552|  PASSED
          sts_serial|  15|    100000|     100|0.18914661|  PASSED
          sts_serial|  15|    100000|     100|0.15664135|  PASSED
          sts_serial|  16|    100000|     100|0.14665341|  PASSED
          sts_serial|  16|    100000|     100|0.68584797|  PASSED
          sts_serial|   1|    100000|     200|0.14616579|  PASSED
          sts_serial|   2|    100000|     200|0.33227912|  PASSED
          sts_serial|   3|    100000|     200|0.43927695|  PASSED
          sts_serial|   3|    100000|     200|0.27806076|  PASSED
          sts_serial|   4|    100000|     200|0.96188539|  PASSED
          sts_serial|   4|    100000|     200|0.08077551|  PASSED
          sts_serial|   5|    100000|     200|0.65933459|  PASSED
          sts_serial|   5|    100000|     200|0.22963145|  PASSED
          sts_serial|   6|    100000|     200|0.56571499|  PASSED
          sts_serial|   6|    100000|     200|0.18318542|  PASSED
          sts_serial|   7|    100000|     200|0.51057524|  PASSED
          sts_serial|   7|    100000|     200|0.18089407|  PASSED
          sts_serial|   8|    100000|     200|0.35604349|  PASSED
          sts_serial|   8|    100000|     200|0.40304211|  PASSED
          sts_serial|   9|    100000|     200|0.76408872|  PASSED
          sts_serial|   9|    100000|     200|0.95273185|  PASSED
          sts_serial|  10|    100000|     200|0.80839950|  PASSED
          sts_serial|  10|    100000|     200|0.54028571|  PASSED
          sts_serial|  11|    100000|     200|0.41294571|  PASSED
          sts_serial|  11|    100000|     200|0.78724198|  PASSED
          sts_serial|  12|    100000|     200|0.05113071|  PASSED
          sts_serial|  12|    100000|     200|0.39448033|  PASSED
          sts_serial|  13|    100000|     200|0.12212867|  PASSED
          sts_serial|  13|    100000|     200|0.56044107|  PASSED
          sts_serial|  14|    100000|     200|0.09281828|  PASSED
          sts_serial|  14|    100000|     200|0.38719729|  PASSED
          sts_serial|  15|    100000|     200|0.41297447|  PASSED
          sts_serial|  15|    100000|     200|0.29324158|  PASSED
          sts_serial|  16|    100000|     200|0.12077055|  PASSED
          sts_serial|  16|    100000|     200|0.95343896|  PASSED
         rgb_bitdist|   1|    100000|     100|0.44955797|  PASSED
         rgb_bitdist|   2|    100000|     100|0.36750579|  PASSED
         rgb_bitdist|   3|    100000|     100|0.84794442|  PASSED
         rgb_bitdist|   4|    100000|     100|0.35777443|  PASSED
         rgb_bitdist|   5|    100000|     100|0.61581322|  PASSED
         rgb_bitdist|   6|    100000|     100|0.92343470|  PASSED
         rgb_bitdist|   7|    100000|     100|0.76128295|  PASSED
         rgb_bitdist|   8|    100000|     100|0.74379843|  PASSED
         rgb_bitdist|   9|    100000|     100|0.75402183|  PASSED
         rgb_bitdist|  10|    100000|     100|0.07179968|  PASSED
         rgb_bitdist|  11|    100000|     100|0.92842765|  PASSED
         rgb_bitdist|  12|    100000|     100|0.26134736|  PASSED
rgb_minimum_distance|   2|     10000|    1000|0.67990100|  PASSED
rgb_minimum_distance|   3|     10000|    1000|0.08683582|  PASSED
rgb_minimum_distance|   4|     10000|    1000|0.94180883|  PASSED
rgb_minimum_distance|   5|     10000|    1000|0.33689745|  PASSED
    rgb_permutations|   2|    100000|     100|0.25393941|  PASSED
    rgb_permutations|   3|    100000|     100|0.76791678|  PASSED
    rgb_permutations|   4|    100000|     100|0.99051602|  PASSED
    rgb_permutations|   5|    100000|     100|0.78590914|  PASSED
      rgb_lagged_sum|   0|   1000000|     100|0.57815982|  PASSED
      rgb_lagged_sum|   1|   1000000|     100|0.06424954|  PASSED
      rgb_lagged_sum|   2|   1000000|     100|0.76368224|  PASSED
      rgb_lagged_sum|   3|   1000000|     100|0.57433562|  PASSED
      rgb_lagged_sum|   4|   1000000|     100|0.01684553|  PASSED
      rgb_lagged_sum|   5|   1000000|     100|0.35834799|  PASSED
      rgb_lagged_sum|   6|   1000000|     100|0.61001398|  PASSED
      rgb_lagged_sum|   7|   1000000|     100|0.62421266|  PASSED
      rgb_lagged_sum|   8|   1000000|     100|0.88989732|  PASSED
      rgb_lagged_sum|   9|   1000000|     100|0.86525276|  PASSED
      rgb_lagged_sum|  10|   1000000|     100|0.73744893|  PASSED
      rgb_lagged_sum|  11|   1000000|     100|0.63316274|  PASSED
      rgb_lagged_sum|  12|   1000000|     100|0.12006619|  PASSED
      rgb_lagged_sum|  13|   1000000|     100|0.79019915|  PASSED
      rgb_lagged_sum|  14|   1000000|     100|0.30371771|  PASSED
      rgb_lagged_sum|  15|   1000000|     100|0.94126753|  PASSED
      rgb_lagged_sum|  16|   1000000|     100|0.85641134|  PASSED
      rgb_lagged_sum|  17|   1000000|     100|0.24325745|  PASSED
      rgb_lagged_sum|  18|   1000000|     100|0.63633569|  PASSED
      rgb_lagged_sum|  19|   1000000|     100|0.98002635|  PASSED
      rgb_lagged_sum|  20|   1000000|     100|0.42030287|  PASSED
      rgb_lagged_sum|  21|   1000000|     100|0.54295046|  PASSED
      rgb_lagged_sum|  22|   1000000|     100|0.27653423|  PASSED
      rgb_lagged_sum|  23|   1000000|     100|0.59050249|  PASSED
      rgb_lagged_sum|  24|   1000000|     100|0.78746596|  PASSED
      rgb_lagged_sum|  25|   1000000|     100|0.23877934|  PASSED
      rgb_lagged_sum|  26|   1000000|     100|0.95978547|  PASSED
      rgb_lagged_sum|  27|   1000000|     100|0.13671078|  PASSED
      rgb_lagged_sum|  28|   1000000|     100|0.99656967|   WEAK
      rgb_lagged_sum|  28|   1000000|     200|0.45338072|  PASSED
      rgb_lagged_sum|  29|   1000000|     100|0.33542744|  PASSED
      rgb_lagged_sum|  30|   1000000|     100|0.82899800|  PASSED
      rgb_lagged_sum|  31|   1000000|     100|0.80961599|  PASSED
      rgb_lagged_sum|  32|   1000000|     100|0.65266391|  PASSED
     rgb_kstest_test|   0|     10000|    1000|0.22113767|  PASSED
     dab_bytedistrib|   0|  51200000|       1|0.38850445|  PASSED
             dab_dct| 256|     50000|       1|0.34083378|  PASSED
        dab_filltree|  32|  15000000|       1|0.27558556|  PASSED
        dab_filltree|  32|  15000000|       1|0.22017168|  PASSED
       dab_filltree2|   0|   5000000|       1|0.30648438|  PASSED
       dab_filltree2|   1|   5000000|       1|0.62143179|  PASSED
        dab_monobit2|  12|  65000000|       1|0.58175537|  PASSED

real    48m31.392s
user    69m38.160s
sys     3m15.175s
```

### splitmix
```dieharder
Proceeding with test 7...
#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |rands/second|   Seed   |
stdin_input_raw|  7.70e+07  |4279176622|
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.50723465|  PASSED
      diehard_operm5|   0|   1000000|     100|0.65185515|  PASSED
  diehard_rank_32x32|   0|     40000|     100|0.17368617|  PASSED
    diehard_rank_6x8|   0|    100000|     100|0.94931273|  PASSED
   diehard_bitstream|   0|   2097152|     100|0.96745954|  PASSED
        diehard_opso|   0|   2097152|     100|0.95107934|  PASSED
        diehard_oqso|   0|   2097152|     100|0.63415543|  PASSED
         diehard_dna|   0|   2097152|     100|0.25053825|  PASSED
diehard_count_1s_str|   0|    256000|     100|0.37882776|  PASSED
diehard_count_1s_byt|   0|    256000|     100|0.81584371|  PASSED
 diehard_parking_lot|   0|     12000|     100|0.54735549|  PASSED
    diehard_2dsphere|   2|      8000|     100|0.92449630|  PASSED
    diehard_3dsphere|   3|      4000|     100|0.46969725|  PASSED
     diehard_squeeze|   0|    100000|     100|0.57098722|  PASSED
        diehard_sums|   0|       100|     100|0.01158522|  PASSED
        diehard_runs|   0|    100000|     100|0.90098469|  PASSED
        diehard_runs|   0|    100000|     100|0.77575456|  PASSED
       diehard_craps|   0|    200000|     100|0.77589648|  PASSED
       diehard_craps|   0|    200000|     100|0.15161009|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.94682958|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.02368265|  PASSED
         sts_monobit|   1|    100000|     100|0.34963569|  PASSED
            sts_runs|   2|    100000|     100|0.06065986|  PASSED
          sts_serial|   1|    100000|     100|0.79902472|  PASSED
          sts_serial|   2|    100000|     100|0.92581476|  PASSED
          sts_serial|   3|    100000|     100|0.89618214|  PASSED
          sts_serial|   3|    100000|     100|0.69363505|  PASSED
          sts_serial|   4|    100000|     100|0.19510083|  PASSED
          sts_serial|   4|    100000|     100|0.25240524|  PASSED
          sts_serial|   5|    100000|     100|0.92428507|  PASSED
          sts_serial|   5|    100000|     100|0.16977215|  PASSED
          sts_serial|   6|    100000|     100|0.68814279|  PASSED
          sts_serial|   6|    100000|     100|0.94251917|  PASSED
          sts_serial|   7|    100000|     100|0.90545559|  PASSED
          sts_serial|   7|    100000|     100|0.36211806|  PASSED
          sts_serial|   8|    100000|     100|0.26452538|  PASSED
          sts_serial|   8|    100000|     100|0.96240900|  PASSED
          sts_serial|   9|    100000|     100|0.87273303|  PASSED
          sts_serial|   9|    100000|     100|0.07126774|  PASSED
          sts_serial|  10|    100000|     100|0.32298937|  PASSED
          sts_serial|  10|    100000|     100|0.70520340|  PASSED
          sts_serial|  11|    100000|     100|0.89342154|  PASSED
          sts_serial|  11|    100000|     100|0.95177080|  PASSED
          sts_serial|  12|    100000|     100|0.13582890|  PASSED
          sts_serial|  12|    100000|     100|0.19724861|  PASSED
          sts_serial|  13|    100000|     100|0.64903255|  PASSED
          sts_serial|  13|    100000|     100|0.29060614|  PASSED
          sts_serial|  14|    100000|     100|0.81621093|  PASSED
          sts_serial|  14|    100000|     100|0.17939822|  PASSED
          sts_serial|  15|    100000|     100|0.54716856|  PASSED
          sts_serial|  15|    100000|     100|0.80927274|  PASSED
          sts_serial|  16|    100000|     100|0.90313990|  PASSED
          sts_serial|  16|    100000|     100|0.35721286|  PASSED
         rgb_bitdist|   1|    100000|     100|0.65270475|  PASSED
         rgb_bitdist|   2|    100000|     100|0.95308814|  PASSED
         rgb_bitdist|   3|    100000|     100|0.29033447|  PASSED
         rgb_bitdist|   4|    100000|     100|0.74121103|  PASSED
         rgb_bitdist|   5|    100000|     100|0.97049311|  PASSED
         rgb_bitdist|   6|    100000|     100|0.87325772|  PASSED
         rgb_bitdist|   7|    100000|     100|0.81183949|  PASSED
         rgb_bitdist|   8|    100000|     100|0.52080826|  PASSED
         rgb_bitdist|   9|    100000|     100|0.48265420|  PASSED
         rgb_bitdist|  10|    100000|     100|0.00178459|   WEAK
         rgb_bitdist|  10|    100000|     200|0.25358345|  PASSED
         rgb_bitdist|  11|    100000|     100|0.92460630|  PASSED
         rgb_bitdist|  12|    100000|     100|0.98905784|  PASSED
rgb_minimum_distance|   2|     10000|    1000|0.64717073|  PASSED
rgb_minimum_distance|   3|     10000|    1000|0.83927607|  PASSED
rgb_minimum_distance|   4|     10000|    1000|0.95266645|  PASSED
rgb_minimum_distance|   5|     10000|    1000|0.77820533|  PASSED
    rgb_permutations|   2|    100000|     100|0.73546984|  PASSED
    rgb_permutations|   3|    100000|     100|0.39333681|  PASSED
    rgb_permutations|   4|    100000|     100|0.32121588|  PASSED
    rgb_permutations|   5|    100000|     100|0.15020788|  PASSED
      rgb_lagged_sum|   0|   1000000|     100|0.41307796|  PASSED
      rgb_lagged_sum|   1|   1000000|     100|0.60257888|  PASSED
      rgb_lagged_sum|   2|   1000000|     100|0.99878580|   WEAK
      rgb_lagged_sum|   2|   1000000|     200|0.61994344|  PASSED
      rgb_lagged_sum|   3|   1000000|     100|0.98281772|  PASSED
      rgb_lagged_sum|   4|   1000000|     100|0.12154567|  PASSED
      rgb_lagged_sum|   5|   1000000|     100|0.76422952|  PASSED
      rgb_lagged_sum|   6|   1000000|     100|0.59104206|  PASSED
      rgb_lagged_sum|   7|   1000000|     100|0.96625073|  PASSED
      rgb_lagged_sum|   8|   1000000|     100|0.99872305|   WEAK
      rgb_lagged_sum|   8|   1000000|     200|0.96352429|  PASSED
      rgb_lagged_sum|   9|   1000000|     100|0.75467515|  PASSED
      rgb_lagged_sum|  10|   1000000|     100|0.40714000|  PASSED
      rgb_lagged_sum|  11|   1000000|     100|0.24178508|  PASSED
      rgb_lagged_sum|  12|   1000000|     100|0.81085536|  PASSED
      rgb_lagged_sum|  13|   1000000|     100|0.03150254|  PASSED
      rgb_lagged_sum|  14|   1000000|     100|0.86638200|  PASSED
      rgb_lagged_sum|  15|   1000000|     100|0.96101687|  PASSED
      rgb_lagged_sum|  16|   1000000|     100|0.59824822|  PASSED
      rgb_lagged_sum|  17|   1000000|     100|0.95675404|  PASSED
      rgb_lagged_sum|  18|   1000000|     100|0.16437410|  PASSED
      rgb_lagged_sum|  19|   1000000|     100|0.88902106|  PASSED
      rgb_lagged_sum|  20|   1000000|     100|0.79764570|  PASSED
      rgb_lagged_sum|  21|   1000000|     100|0.83212463|  PASSED
      rgb_lagged_sum|  22|   1000000|     100|0.21319300|  PASSED
      rgb_lagged_sum|  23|   1000000|     100|0.09498140|  PASSED
      rgb_lagged_sum|  24|   1000000|     100|0.95557122|  PASSED
      rgb_lagged_sum|  25|   1000000|     100|0.68219343|  PASSED
      rgb_lagged_sum|  26|   1000000|     100|0.77277915|  PASSED
      rgb_lagged_sum|  27|   1000000|     100|0.07184296|  PASSED
      rgb_lagged_sum|  28|   1000000|     100|0.65109469|  PASSED
      rgb_lagged_sum|  29|   1000000|     100|0.81550762|  PASSED
      rgb_lagged_sum|  30|   1000000|     100|0.86791969|  PASSED
      rgb_lagged_sum|  31|   1000000|     100|0.82112437|  PASSED
      rgb_lagged_sum|  32|   1000000|     100|0.30146121|  PASSED
     rgb_kstest_test|   0|     10000|    1000|0.61124340|  PASSED
     dab_bytedistrib|   0|  51200000|       1|0.76804867|  PASSED
             dab_dct| 256|     50000|       1|0.65650776|  PASSED
        dab_filltree|  32|  15000000|       1|0.37493183|  PASSED
        dab_filltree|  32|  15000000|       1|0.32876007|  PASSED
       dab_filltree2|   0|   5000000|       1|0.54291724|  PASSED
       dab_filltree2|   1|   5000000|       1|0.34372885|  PASSED
        dab_monobit2|  12|  65000000|       1|0.65446814|  PASSED

real    31m44.129s
user    47m52.987s
sys     2m32.788s
```

### wy
```dieharder
Proceeding with test 8...
#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |rands/second|   Seed   |
stdin_input_raw|  6.26e+07  |3556259292|
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.38791554|  PASSED
      diehard_operm5|   0|   1000000|     100|0.06981309|  PASSED
  diehard_rank_32x32|   0|     40000|     100|0.91192284|  PASSED
    diehard_rank_6x8|   0|    100000|     100|0.40481036|  PASSED
   diehard_bitstream|   0|   2097152|     100|0.40981720|  PASSED
        diehard_opso|   0|   2097152|     100|0.44714116|  PASSED
        diehard_oqso|   0|   2097152|     100|0.29570434|  PASSED
         diehard_dna|   0|   2097152|     100|0.75869507|  PASSED
diehard_count_1s_str|   0|    256000|     100|0.16660609|  PASSED
diehard_count_1s_byt|   0|    256000|     100|0.84461820|  PASSED
 diehard_parking_lot|   0|     12000|     100|0.96352334|  PASSED
    diehard_2dsphere|   2|      8000|     100|0.23543464|  PASSED
    diehard_3dsphere|   3|      4000|     100|0.26008213|  PASSED
     diehard_squeeze|   0|    100000|     100|0.04099708|  PASSED
        diehard_sums|   0|       100|     100|0.07367579|  PASSED
        diehard_runs|   0|    100000|     100|0.67896101|  PASSED
        diehard_runs|   0|    100000|     100|0.67197202|  PASSED
       diehard_craps|   0|    200000|     100|0.88259910|  PASSED
       diehard_craps|   0|    200000|     100|0.43144384|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.98555699|  PASSED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.04865127|  PASSED
         sts_monobit|   1|    100000|     100|0.91236717|  PASSED
            sts_runs|   2|    100000|     100|0.99667493|   WEAK
            sts_runs|   2|    100000|     200|0.86328092|  PASSED
          sts_serial|   1|    100000|     100|0.96141523|  PASSED
          sts_serial|   2|    100000|     100|0.70306099|  PASSED
          sts_serial|   3|    100000|     100|0.06458362|  PASSED
          sts_serial|   3|    100000|     100|0.40355648|  PASSED
          sts_serial|   4|    100000|     100|0.14516779|  PASSED
          sts_serial|   4|    100000|     100|0.67443411|  PASSED
          sts_serial|   5|    100000|     100|0.75087868|  PASSED
          sts_serial|   5|    100000|     100|0.26608767|  PASSED
          sts_serial|   6|    100000|     100|0.49895790|  PASSED
          sts_serial|   6|    100000|     100|0.45666202|  PASSED
          sts_serial|   7|    100000|     100|0.63243633|  PASSED
          sts_serial|   7|    100000|     100|0.98132482|  PASSED
          sts_serial|   8|    100000|     100|0.45946508|  PASSED
          sts_serial|   8|    100000|     100|0.88913588|  PASSED
          sts_serial|   9|    100000|     100|0.32398491|  PASSED
          sts_serial|   9|    100000|     100|0.25989469|  PASSED
          sts_serial|  10|    100000|     100|0.21790613|  PASSED
          sts_serial|  10|    100000|     100|0.80258604|  PASSED
          sts_serial|  11|    100000|     100|0.24436518|  PASSED
          sts_serial|  11|    100000|     100|0.22853921|  PASSED
          sts_serial|  12|    100000|     100|0.29543201|  PASSED
          sts_serial|  12|    100000|     100|0.42093094|  PASSED
          sts_serial|  13|    100000|     100|0.43693409|  PASSED
          sts_serial|  13|    100000|     100|0.27438839|  PASSED
          sts_serial|  14|    100000|     100|0.01909152|  PASSED
          sts_serial|  14|    100000|     100|0.04613332|  PASSED
          sts_serial|  15|    100000|     100|0.23329190|  PASSED
          sts_serial|  15|    100000|     100|0.83929632|  PASSED
          sts_serial|  16|    100000|     100|0.50018641|  PASSED
          sts_serial|  16|    100000|     100|0.90484226|  PASSED
         rgb_bitdist|   1|    100000|     100|0.64477875|  PASSED
         rgb_bitdist|   2|    100000|     100|0.79546824|  PASSED
         rgb_bitdist|   3|    100000|     100|0.63733739|  PASSED
         rgb_bitdist|   4|    100000|     100|0.33664744|  PASSED
         rgb_bitdist|   5|    100000|     100|0.96548896|  PASSED
         rgb_bitdist|   6|    100000|     100|0.78431322|  PASSED
         rgb_bitdist|   7|    100000|     100|0.35535135|  PASSED
         rgb_bitdist|   8|    100000|     100|0.15056118|  PASSED
         rgb_bitdist|   9|    100000|     100|0.63053417|  PASSED
         rgb_bitdist|  10|    100000|     100|0.77840540|  PASSED
         rgb_bitdist|  11|    100000|     100|0.74576130|  PASSED
         rgb_bitdist|  12|    100000|     100|0.98351388|  PASSED
rgb_minimum_distance|   2|     10000|    1000|0.97800922|  PASSED
rgb_minimum_distance|   3|     10000|    1000|0.66540319|  PASSED
rgb_minimum_distance|   4|     10000|    1000|0.66149812|  PASSED
rgb_minimum_distance|   5|     10000|    1000|0.44796740|  PASSED
    rgb_permutations|   2|    100000|     100|0.84836584|  PASSED
    rgb_permutations|   3|    100000|     100|0.58063126|  PASSED
    rgb_permutations|   4|    100000|     100|0.75758431|  PASSED
    rgb_permutations|   5|    100000|     100|0.16843526|  PASSED
      rgb_lagged_sum|   0|   1000000|     100|0.80374872|  PASSED
      rgb_lagged_sum|   1|   1000000|     100|0.04650344|  PASSED
      rgb_lagged_sum|   2|   1000000|     100|0.33761513|  PASSED
      rgb_lagged_sum|   3|   1000000|     100|0.22999624|  PASSED
      rgb_lagged_sum|   4|   1000000|     100|0.86721704|  PASSED
      rgb_lagged_sum|   5|   1000000|     100|0.64067814|  PASSED
      rgb_lagged_sum|   6|   1000000|     100|0.36340449|  PASSED
      rgb_lagged_sum|   7|   1000000|     100|0.16994976|  PASSED
      rgb_lagged_sum|   8|   1000000|     100|0.67936985|  PASSED
      rgb_lagged_sum|   9|   1000000|     100|0.09600713|  PASSED
      rgb_lagged_sum|  10|   1000000|     100|0.92400383|  PASSED
      rgb_lagged_sum|  11|   1000000|     100|0.67683460|  PASSED
      rgb_lagged_sum|  12|   1000000|     100|0.66966787|  PASSED
      rgb_lagged_sum|  13|   1000000|     100|0.15144607|  PASSED
      rgb_lagged_sum|  14|   1000000|     100|0.91699162|  PASSED
      rgb_lagged_sum|  15|   1000000|     100|0.36044338|  PASSED
      rgb_lagged_sum|  16|   1000000|     100|0.55214393|  PASSED
      rgb_lagged_sum|  17|   1000000|     100|0.34531391|  PASSED
      rgb_lagged_sum|  18|   1000000|     100|0.69236898|  PASSED
      rgb_lagged_sum|  19|   1000000|     100|0.90811432|  PASSED
      rgb_lagged_sum|  20|   1000000|     100|0.37961272|  PASSED
      rgb_lagged_sum|  21|   1000000|     100|0.81417922|  PASSED
      rgb_lagged_sum|  22|   1000000|     100|0.34761635|  PASSED
      rgb_lagged_sum|  23|   1000000|     100|0.65521643|  PASSED
      rgb_lagged_sum|  24|   1000000|     100|0.96820534|  PASSED
      rgb_lagged_sum|  25|   1000000|     100|0.06687001|  PASSED
      rgb_lagged_sum|  26|   1000000|     100|0.65695881|  PASSED
      rgb_lagged_sum|  27|   1000000|     100|0.13504145|  PASSED
      rgb_lagged_sum|  28|   1000000|     100|0.99921715|   WEAK
      rgb_lagged_sum|  28|   1000000|     200|0.73113488|  PASSED
      rgb_lagged_sum|  29|   1000000|     100|0.10290411|  PASSED
      rgb_lagged_sum|  30|   1000000|     100|0.92891010|  PASSED
      rgb_lagged_sum|  31|   1000000|     100|0.68686129|  PASSED
      rgb_lagged_sum|  32|   1000000|     100|0.18125075|  PASSED
     rgb_kstest_test|   0|     10000|    1000|0.84369563|  PASSED
     dab_bytedistrib|   0|  51200000|       1|0.85992163|  PASSED
             dab_dct| 256|     50000|       1|0.10079273|  PASSED
        dab_filltree|  32|  15000000|       1|0.44756120|  PASSED
        dab_filltree|  32|  15000000|       1|0.02418158|  PASSED
       dab_filltree2|   0|   5000000|       1|0.65013318|  PASSED
       dab_filltree2|   1|   5000000|       1|0.24490968|  PASSED
        dab_monobit2|  12|  65000000|       1|0.69499842|  PASSED

real    32m23.333s
user    42m9.631s
sys     2m42.819s
```

### mt19937
```dieharder
```

I haven't ran this test yet.

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
