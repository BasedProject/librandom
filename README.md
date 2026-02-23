# librandom
Schizophrenic Man's Rewritten Code Rewritten Again For The Benefit Of The Based Among Us

### Generalized

Interface:

    random_psued_t [same as uint32_t, hence 32]
    random_psued_t random_psued_init(uint32_t Initial State)
    uint32_t random_psued(random_psued_t * Random Pointer)

Example:

	random_psued_t r[1] = {random_psued_init(initial)};
	uint32_t rand = random_psued(r);

### float.h

Do primitive conversion to float.

    float  random_float(uint32_t random);
	double random_float(uint64_t random);

### combine.h

You may want to combine two uint32s into a uint64:

	uint64_t random_combine(uint32_t low, uint32_t high);

### lcg.h

    random_lcg32_t [same as uint32_t, hence 32]
    random_lcg32_t random_lcg32_init(uint32_t Initial State)
    uint32_t random_lcg32(random_lcg32_t * Random Pointer)

A simple Linear Congruential Generator (LCG): extremely fast with low-quality randomness.

LCGs are among the oldest PRNGs. This configuration is fast and compact but exhibits correlations and
 shorter periods compared to modern generators. Best suited for cosmetic randomness where quality is not
 critical.

+ Pros: Fast; trivial; Tiny state; Deterministic behavior; Serializable.
+ Cons: Poor Statistical Quality vs. PCG/Xoroshiro;
      noticeable patterns in some uses; Not cryptographically secure.

+ Use in low impact things, like visual effects, quick throwaway randomness, or prototyping.
+ Do not use in Gameplay-critical logic, simulations, or fairness-sensitive systems.

Example:

	random_lcg32_t r[1] = {random_lcg32_init(initial)};
	uint32_t rand = random_lcg32(r);

### xorshift.h

    random_xorshift32_t [same as uint32_t, hence 32]
    random_xorshift32_t random_xorshift32_init(uint32_t Initial State)
    uint32_t random_xorshift32(random_xorshift32_t * Random Pointer)

A classic, extremely fast XorShift PRNG with small state and modest quality.

XorShift generators are known for their simplicity and speed. This variant operates on a 32-bit state and
 produces 32-bit outputs. It is suitable for lightweight, cosmetic randomness where maximum statistical
 rigor is not required.

+ Pros: Very fast; tiny state footprint; Deterministic; Serializable.
+ Cons: Poor Statistical Quality vs. newer generators; Not cryptographically secure.

+ Use in Effects, particles, jitter, or any light randomness in hot loops;
 Short-lived simulations where ultimate quality is not required.

+ Do not use in Simulations or systems sensitive to subtle bias;
 especially not Security-sensitive contexts.

Example:

	random_xorshift32_t r[1] = {random_xorshift32_init(initial)};
	uint32_t rand = random_xorshift32(r);
