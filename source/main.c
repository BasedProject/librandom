/* https://creativecommons.org/publicdomain/zero/1.0/ <- CC0 1.0 Universal */
/* this is just for testing */
#include "random.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define fulltest(n, x, ...) do {                                        \
    test(random_##x, "%" WIDTH "lu", n, __VA_ARGS__);                   \
    if (n == 32)                                                        \
    {test_combined(random_##x, "%" WIDTH "lu", n, __VA_ARGS__); }       \
    test_wrap(random_##x, random_float, "%" WIDTH "f", n, __VA_ARGS__); \
  } while (0)

#define fulltest32(x, ...) fulltest(32, x, __VA_ARGS__)
#define fulltest64(x, ...) do {                                                 \
    fulltest(32, x, __VA_ARGS__);                                               \
    fulltest(64, x, __VA_ARGS__);                                               \
  } while (0)


#define selectable(x, bit, ...)                                 \
  if (generator == enum_##x) {                                  \
    random_##x##_t r[1] = {random_##x##_init(__VA_ARGS__)};     \
    while (1) {                                                 \
      uint##bit##_t random = random_##x##bit(r);                \
      fwrite(&random, sizeof(uint##bit##_t), 1, stdout);        \
    }                                                           \
  }

int main(int ac, char ** av) {
  enum {
    #define X(x,y,...) enum_##x,
    #include "librandom_x.h"
    LAST,
  };
  char * list[] = {
    #define X(x,y,...) STRINGIFY(x),
    #include "librandom_x.h"
  };
  #if 0
# generates the initial signature for each algo.
for i in $(seq 0 8); do
  ./random.out select $i | dd if=/dev/stdin bs=4 count=4 of=/dev/stdout 2> /dev/null | hexdump -e '16/1 "%.2x" "\n"'
done
# Full Battery.
for i in $(seq 0 9); do
 echo "Proceeding with test $i..."
 time ./random.out select $i | dieharder -a -Y 1 -g 200
done
  #endif
  if (ac-1 && strstr("select", av[1])) {
    --ac, ++av;
    uint32_t generator = --ac > 0 ? atoi(*(++av)) : 0;
    uint64_t a = --ac > 0 ? atoll(*(++av)) : 0;
    uint64_t b = --ac > 0 ? atoll(*(++av)) : 0;
    #define XA a
    #define XB b
    #define X(x,y,...) selectable(x, y, __VA_ARGS__);
    #include "librandom_x.h"
  } else if (ac-1 && strstr("list", av[1])) {
    for (int i = 0; i < LAST; ++i) {
      printf("%3d | %-16s |\n", i, list[i]);
    }
  } else {
    uint64_t initial = --ac ? atoll(*(++av)) : 0;
    #define XA initial
    #define XB initial
    #define X(x,y,...) fulltest##y(x, __VA_ARGS__);
    #include "librandom_x.h"
    return 0;
  }
}
