# librandom
> such notable numbers as 795369128, 9225981519167521260 0.446480, and especially 2503112483. Some may also know 2974294238. 

`librandom` provides randomness. This code is written for C23.

see [Here](https://github.com/wallstop/unity-helpers) in `Runtime/Core/Random` for where we got the algos here.

This code is in no way meant to be a cryptographic library, and you should not use any implementation here as a psuedorandom number generator (PRNG) for crypto.

- [librandom](#librandom)
  * [Interface](#interface)
  * [Headers](#headers)
    + [random.h](#randomh)
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

## Interface

```c
// XXX
<native> rng_init_raw(...);     /* usually u<size>, sometimes (read: just pcg) u64 u64 */
<native> rng_next(rng_t * rng); /* <native> is dependent on the algo, usually u64/32  */


// XXX
rng_t rng_init([[nullable]] const char * buffer, size_t length);
typename rng_typename(rng_t * rng);
/* where <type> may be always u32, u64, u128, (signed versions), f32, f64. downcast as needed. */

typename random_typename(<rng>); /* provided, note that this is not truly GENERIC. */
/* <type> in this case is more expansive, only numeric types not included are >f80 and _ prefixed math types. */
```

Replace rng_t with your algorithm of choice.

I recommend mt19937 or photon for games,
avoid `mt19937` if you care about size constraints as it's a bit large (a few kilobytes in memory.)

MT19937 is the most generally robust and widely used (for insecure means) psuedorandom algo that I'm aware of.

Global Interface:

```c
#define GRAND photon // Not required. This is the default.
#include <random.h>

grand_init(NULLABLE void * Initial State, size_t Length); // Not required.
TYPE grand(void); // (<native> type of algo., u32 for photon)
TYPE grand_TYPE(void);
TYPE grand_range_TYPE(rng_t * rng, TYPE small, TYPE big);
// range works for except 128-bit types. Inclusive.
// We use Lemire's unbiased multiply-high method for integer types.
```

grand_init(NULLABLE void * Initial State, size_t Length); // Not required.
TYPE grand(void); // (<native> type of algo., u32 for photon)
TYPE grand_TYPE(void);
TYPE grand_range_TYPE(rng_t * rng, TYPE small, TYPE big);
// range works for except 128-bit types. Inclusive.
// We use Lemire's unbiased multiply-high method for integer types.
```

None of these should ever be used in security sensitive contexts, don't use computers if you want security.

```c
#define USE_RANDOM_PREFIX
```

Will ensure every accessible function uses `random_` as a prefix. this is disabled by default.

```c
#define ALIGN_RANDOM // optional, 64 or 128; effectively 32 by default
```
	
Except for grand, this ensures that all calls move forward the same amount through the randomness queue.
Some algos will operate like `ALIGN=64`, with their minimum internal size being `u64`.

Per Issue #10:
```
> ALIGN_RANDOM sets the minimum size of every call.
Meaning, that ALIGN_RANDOM=64 will ensure that if you
random_u32 random_u64 it will step forward in the random queue by 128
bits, ensuring the next call will inspect the 64 bits after that... etc.

An example:

A game uses a Photon, which is 32 bits wide per Random Next, but the
game almost exclusively uses u64 as its primary application of the
random data. Say that random data being consistent, say for map
generation, is very important. Thus, if someone were to add or replace a
random call of u64 to u32, misassuming that the next piece of random
data would be stable, would result in complete divergence and thus
annoying lame problems. (also if you added a u32 call in the right place
afterwards you could FIX the issue for later code, but code between the
first and second u32 would be BROKEN :[ LITERALLY UNDEBUGGABLE)
```

---

Here's a usage example:
```c
lcg2_t r[1] = {lcg2_init_raw(initial)};
u64 rand = random_u64(r);
```
This should applicate to all other generators.

## Headers

### [random.h](https://github.com/BasedProject/librandom/blob/master/source/random.h)

The top-level include. Pulls in all code.

Provides `_Generic` dispatch macros over all generator types:

    random_u32(T)    random_u64(T)    random_u128(T)
    random_s32(T)    random_s64(T)    random_s128(T)
    random_i32(T)    random_i64(T)    random_i128(T)    /* aliases for s-variants */
    random_f32(T)    random_f64(T)

Where `T` is a pointer to any generator instance. Selects the correct typed function at compile time.

### [lcg.h](https://github.com/BasedProject/librandom/blob/master/source/lcg.h)

A simple Linear Congruential Generator (LCG): extremely fast with low-quality randomness.

LCGs are among the oldest PRNGs.
This configuration is fast and compact but exhibits correlations and
shorter periods compared to modern generators.
Best suited for cosmetic randomness where quality is not critical.

Pros:
* Fast
* Trivial
* Tiny state

Cons:
* Poor statistical quality vs. xoroshiro
* Noticeable patterns in some uses

### [lcg2.h](https://github.com/BasedProject/librandom/blob/master/source/lcg2.h)

Similar to lcg.h, except employs the 64-bit LCG described [here](https://nuclear.llnl.gov/CNP/rng/rngman/node4.html).

Per Bret R. Beck and Eugene D. Brooks III.

### [pcg.h](https://github.com/BasedProject/librandom/blob/master/source/pcg.h)

A high-quality, small-state pseudo-random number generator based on the PCG family.

Implementation based off of the reference PCG Random, found here: https://www.pcg-random.org/index.html

PCG (Permuted Congruential Generator) offers excellent statistical quality with very small state
and extremely fast generation.
This implementation uses a 64-bit state with 32-bit outputs and
an increment (stream selector) to avoid overlapping sequences when constructing multiple instances.
Note: `pcg_init_raw` takes two arguments rather than one; `X_PROVES_RULES` is set and `pcg_init`
is defined manually.

Pros:
* Fast
* Suitable for gameplay hot paths

Great statistical quality for games and simulations; passes common PRNG test suites for 32-bit outputs.

> ![TIP]
> Use in general gameplay randomness, procedural content, Monte Carlo style sampling.

### [xorshift.h](https://github.com/BasedProject/librandom/blob/master/source/xorshift.h)

A classic, extremely fast XorShift PRNG with small state and modest quality.

XorShift generators are known for their simplicity and speed.
This variant operates on a 32-bit state and produces 32-bit outputs.
It is suitable for lightweight,
cosmetic randomness where maximum statistical rigor is not required.

Pros:
* Fast
* Tiny State

Cons:
* Poor statistical quality vs. newer generators
* Tiny State

### [xoroshiro.h](https://github.com/BasedProject/librandom/blob/master/source/xoroshiro.h)

A fast 128-bit state Xoroshiro-based PRNG with good quality and tiny footprint.

Xoroshiro family generators (here in a 64/64 configuration) offer an excellent balance between speed and quality for real-time applications.
This implementation maintains two 64-bit state variables and returns 64-bit outputs (`X_NEXT_WIDTH 64`).

Pros:
* Very fast
* suitable for gameplay and procedural generation
* good statistical properties
* long period (~2^128−1)

### [splitmix.h](https://github.com/BasedProject/librandom/blob/master/source/splitmix.h)

A fast 64-bit SplitMix generator often used as a high-quality seeding/mixing PRNG.

SplitMix64 is widely used to quickly generate well-distributed 64-bit values and as a seed source for
other generators.
Outputs 64-bit values (`X_NEXT_WIDTH 64`).

Pros: Very fast; great as a hash/mixer and for seed generation; simple.

> ![TIP]
> Use in producing seeds for other PRNGs, quick hash-like mixing, gameplay randomness.

### [wy.h](https://github.com/BasedProject/librandom/blob/master/source/wy.h)

A wyhash-inspired PRNG variant (WyRandom) leveraging multiply-mix operations for speed and good distribution.

Reference implementation: https://github.com/cocowalla/wyhash-dotnet/blob/master/src/WyHash/WyRng.cs

Designed around 64-bit multiply-and-mix steps, this generator is fast and suitable for general-purpose
randomness and hashing-like use cases. Outputs 64-bit values (`X_NEXT_WIDTH 64`).

Pros:
* Fast and simple
* good distribution for typical gameplay uses

Cons:
* Less widely standardized than PCG/Xoroshiro.

### [photon.h](https://github.com/BasedProject/librandom/blob/master/source/photon.h)

PhotonSpin32: a ring-buffer generator inspired by SHISHUA, tuned for high throughput and large period.

The reason this library exists.

This used to be hosted on
[Will Stafford Parsons github](https://github.com/wileylooper/photonspin),
which has since been deleted.
The only remaining references to it are this repo
and [Wallstop's Unity Helpers](https://github.com/wallstop/unity-helpers)
(`Runtime/Core/Random`).

Block size defaults to 20 words; override with `RANDOM_PHOTON_BLOCK_SIZE` before including.
Either induct `photon.c` into your program or recompile your library instance.
The spin increment defaults to `111111U`; override with `RANDOM_PHOTON_SPIN_INCREMENT`.

This generator produces batches of dynamically defined 32-bit values per round.
Offers a huge period (~2^512) and robust statistical performance.
It shines when large streams are required.

Pros:
* excellent distribution
* great for heavy simulation workloads

Cons:
* large state

> [!TIP]
> Best used in procedural workloads that benefit from bulk generation
> and long non-overlapping streams.

### [romuduo.h](https://github.com/BasedProject/librandom/blob/master/source/romuduo.h)
```c
romuduo_t  romuduo_init(const char * buffer, size_t length)
romuduo_t  romuduo_init_raw(u128 init)
u32        romuduo_next(romuduo_t * randomp)
/* + standard suite */
```

A member of the ROMU family (RomuDuo) emphasizing high speed and good statistical quality on modern CPUs.

RomuDuo maintains two 64-bit state variables (stored as a `u128`) and uses rotations and multiplies to evolve the state.
It is competitive with Xoroshiro-style generators in speed while exhibiting strong distribution for general use.

Pros:
* very fast
* excellent for real-time usage
* good statistical behavior for non-crypto applications
* deterministic and reproducible across platforms.

Cons:
* Relatively newer family
* choose proven options if organizational policy requires long-term validation.

> [!TIP]
> Use in gameplay RNG, procedural content generation, fast Monte Carlo sampling.

### [mt19937.h](https://github.com/BasedProject/librandom/blob/master/source/mt19937.h)
`BSD-3-clause -- Copyright 1997 - 2002, Makoto Matsumoto and Takuji Nishimura -- All rights reserved.`

See [mt19937.c](https://github.com/BasedProject/librandom/blob/master/source/mt19937.c) for full licensing.

See [here.](https://en.wikipedia.org/wiki/Mersenne_Twister)

Super large period of 2^19937-1.
Based on Mersenne primes.
State is 624 `u32` words plus an index.

Pros:
* generally robust
* good for simulations

Cons:
* Lots of state
* Nonoptimized implementation

---

### [grand.h](https://github.com/BasedProject/librandom/blob/master/source/grand.h)
```c
grand_t  grand_init(const char * buffer, size_t length)
grand_t  grand_init_raw(u128 init)

u32    grand_u32(void)
u64    grand_u64(void)
u128   grand_u128(void)
s32    grand_s32(void)      /* also grand_i32 */
s64    grand_s64(void)      /* also grand_i64 */
s128   grand_s128(void)     /* also grand_i128 */
f32    grand_f32(void)
f64    grand_f64(void)

u64    grand_range_u32(u32 low, u32 high)
u64    grand_range_u64(u64 low, u64 high)
i32    grand_range_i32(i32 low, i32 high)
i64    grand_range_i64(i64 low, i64 high)
f32    grand_range_f32(f32 low, f32 high)
f64    grand_range_f64(f64 low, f64 high)

grand_t grand_value[1]           /* the global instance */
grand() macro                    /* expands to grand_u32() or grand_u64() per GRAND_WIDTH */
```

A global interface backed by a selectable generator. Defaults to `photon` (this is literally the internal value of macro `GRAND`.)
Override by defining `GRAND` and `GRAND_WIDTH` before including `grand.h`:

```c
#define GRAND      pcg
#define GRAND_WIDTH 32
#include "grand.h"
```

Does not respect `ALIGN_RANDOM`, per that being technically cumbersome.

Example:

```c
u32 r    = grand_u32();
f32 f    = grand_range_f32(0.0f, 1.0f);
i32 i    = grand_range_i32(-10, 10);
u32 u    = grand_range_u32(1000, 10000);
```

> ![TIP]
> Uses dynamic dispatch via a function pointer (`grand_next`), initialized lazily on first call.

---

## Copyright?

All of these headers are marked as [`MIT License - Copyright 2025 wallstop`](https://mit-license.org/) unless otherwise specified or applicable to some larger domain, in which case the notice could be prunable. (i.e. very well known algorithms released to the public domain.)

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
