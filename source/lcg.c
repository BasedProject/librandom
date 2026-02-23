#include "lcg.h"

random_lcg32_t random_lcg32_init(uint32_t init) {
  return (random_lcg32_t) init;
}

uint32_t random_lcg32(random_lcg32_t * randomp) {
  random_lcg32_t random = *randomp;
  random   = random * 1664525 + 1013904223;
  *randomp = random;
  return random;
}
