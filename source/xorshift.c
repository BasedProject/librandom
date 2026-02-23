#include "xorshift.h"

static inline random_xorshift32_t random_xorshift32_normalize(random_xorshift32_t random) {
  const random_xorshift32_t random_xorshift32_default = 2463534242;
  return random == 0 ? random_xorshift32_default : random;
}

random_xorshift32_t random_xorshift32_init(uint32_t init) {
  return random_xorshift32_normalize((random_xorshift32_t) init);
}

uint32_t random_xorshift32(random_xorshift32_t * randomp) {
  random_xorshift32_t random = *randomp;
  random    = random_xorshift32_normalize(random);
  random   ^= random << 13;
  random   ^= random >> 17;
  random   ^= random << 5;
  *randomp  = random;
  return random;
}
