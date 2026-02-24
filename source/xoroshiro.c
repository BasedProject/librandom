#include "xoroshiro.h"

#include "ro.h"

static inline
void random_xoroshiro_normalize(random_xoroshiro_t * randomp) {
  if ((randomp->a | randomp->b) == 0) {
    randomp->a = 0x9E3779B97F4A7C15ULL;
    randomp->b = 0xD1B54A32D192ED03ULL;
  }
}
random_xoroshiro_t random_xoroshiro_init(uint64_t init_a, uint64_t init_b) {
  random_xoroshiro_t xoroshiro;
  xoroshiro.a = init_a;
  xoroshiro.b = init_b;
  random_xoroshiro_normalize(&xoroshiro);
  return xoroshiro;
}

uint32_t random_xoroshiro32(random_xoroshiro_t * randomp) {
  return random_xoroshiro64(randomp);
}

uint64_t random_xoroshiro64(random_xoroshiro_t * randomp) {
  uint64_t a, b, sum;
  random_xoroshiro_normalize(randomp);
  a             = randomp->a;
  b             = randomp->b;
  sum           = a + b;
  b            ^= a;
  randomp->a    = random_rol64(a, 24) ^ b ^ (b << 16);
  randomp->b    = random_rol64(b, 37);
  return sum;
}
