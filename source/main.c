/* this is just for testing */
#include "random.h"

#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_VALUES 6

#define _STRINGIFY(...) # __VA_ARGS__
#define STRINGIFY(...) _STRINGIFY(__VA_ARGS__)

#define GENERAL_FORMAT "%16s %32s %2d (%d)\t"

#define test(x, format, n, ...) do {                                            \
    x##_t y[1] = {x##_init(__VA_ARGS__)};                                       \
    printf(GENERAL_FORMAT, "normal", STRINGIFY(x##_t), n, __VA_ARGS__);         \
    for (int i = 0; i < NUMBER_OF_VALUES; ++i) {                                \
      printf(format " ", x##n(y));                                              \
    }                                                                           \
    printf("\n");                                                               \
  } while (0)

#define test_wrap(x, wrap, format, n, ...) do {                                 \
    x##_t y[1] = {x##_init(__VA_ARGS__)};                                       \
    printf(GENERAL_FORMAT, STRINGIFY(wrap), STRINGIFY(x##_t), n, __VA_ARGS__);  \
    for (int i = 0; i < NUMBER_OF_VALUES; ++i) {                                \
      printf(format " ", wrap(x##n(y)));                                        \
    }                                                                           \
    printf("\n");                                                               \
  } while (0)

#define test_combined(x, format, n, ...) do {                                   \
    x##_t y[1] = {x##_init(__VA_ARGS__)};                                       \
    printf(GENERAL_FORMAT, "combined", STRINGIFY(x##_t), n, __VA_ARGS__);       \
    for (int i = 0; i < NUMBER_OF_VALUES; ++i) {                                \
      printf(format " ", random_combine(x##n(y), x##n(y)));                     \
    }                                                                           \
    printf("\n");                                                               \
  } while (0)


#define WIDTH "20"

#define fulltest(n, x, ...) do {                                                \
    test(random_##x, "%" WIDTH "u", n, __VA_ARGS__);                            \
    test_combined(random_##x, "%" WIDTH "lu", n, __VA_ARGS__);                  \
    test_wrap(random_##x, random_float, "%" WIDTH "f", n, __VA_ARGS__);         \
  } while (0)

#define fulltest32(x, ...) fulltest(32, x, __VA_ARGS__)
#define fulltest64(x, ...) do {                                                 \
    fulltest(32, x, __VA_ARGS__);                                               \
    fulltest(64, x, __VA_ARGS__);                                               \
  } while (0)

int main(int ac, char ** av) {
  uint32_t initial = --ac ? atoi(*(++av)) : 0;
  fulltest32(xorshift, initial);
  fulltest32(lcg, initial);
  fulltest32(pcg, initial, RANDOM_PCG_PRIME);
  fulltest32(photon, initial, initial);
  fulltest32(romuduo, initial, initial);
  fulltest64(xoroshiro, initial, initial);
  fulltest64(splitmix, initial);
  fulltest64(wy, initial);
  return 0;
}
