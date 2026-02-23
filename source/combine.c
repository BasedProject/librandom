#include "combine.h"
uint64_t random_combine(uint32_t low, uint32_t high) {
  return low + (high << 32);
}
