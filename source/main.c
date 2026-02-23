/* this is just for testing */
#include "random.h"

#include <stdio.h>
#include <stdlib.h>

#define _STRINGIFY(...) # __VA_ARGS__
#define STRINGIFY(...) _STRINGIFY(__VA_ARGS__)

#define test(initial, x, format) do {                                   \
    x##_t y[1] = {x##_init(initial)};                                   \
    printf("%16s %32s (%d)\t", "normal", STRINGIFY(x##_t), initial);    \
    for (int i = 0; i < 10; ++i) {                                      \
      printf(format " ", x(y));                                         \
    }                                                                   \
    printf("\n");                                                       \
  } while (0)

#define test_wrap(initial, x, wrap, format) do {                            \
    x##_t y[1] = {x##_init(initial)};                                       \
    printf("%16s %32s (%d)\t", STRINGIFY(wrap), STRINGIFY(x##_t), initial); \
    for (int i = 0; i < 10; ++i) {                                          \
      printf(format " ", wrap(x(y)));                                       \
    }                                                                       \
    printf("\n");                                                           \
  } while (0)

#define test_combined(initial, x, format) do {                          \
    x##_t y[1] = {x##_init(initial)};                                   \
    printf("%16s %32s (%d)\t", "combined", STRINGIFY(x##_t), initial);  \
    for (int i = 0; i < 10; ++i) {                                      \
      printf(format " ", random_combine(x(y), x(y)));                   \
    }                                                                   \
    printf("\n");                                                       \
  } while (0)


#define WIDTH "10"

#define fulltest32(x) do {                                              \
    test(initial, random_##x, "%" WIDTH "u");                           \
    test_combined(initial, random_##x, "%" WIDTH "lu");                 \
    test_wrap(initial, random_##x, random_float, "%" WIDTH "f");        \
  } while (0)

#define fulltest64(x) do {                                              \
    test(initial, random_##x, "%" WIDTH "lu");                          \
    test_wrap(initial, random_##x, random_float, "%" WIDTH "f");        \
  } while (0)

int main(int ac, char ** av) {
  uint32_t initial = --ac ? atoi(*(++av)) : 0;
  fulltest32(xorshift32);
  fulltest32(lcg32);
  return 0;
}
