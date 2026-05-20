/* https://creativecommons.org/publicdomain/zero/1.0/ <- CC0 1.0 Universal */
/* this is just for testing */
#include "random.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_VALUES 6

#define _STRINGIFY(...) # __VA_ARGS__
#define STRINGIFY(...) _STRINGIFY(__VA_ARGS__)

int main(int ac, char ** av) {
# define zeros(al, format, type) do {                                         \
    al##_t al[1] = {al##_init(NULL, 0)};                                \
    printf("%-10s: ", STRINGIFY(al));                                   \
    for (size_t i = 0; i < NUMBER_OF_VALUES; ++i)                       \
    {printf(format "%s", al##_##type(al), i+1==NUMBER_OF_VALUES ? "\n" : " ");} \
  } while (0);
  /* s32 */
  zeros(lcg,       "%12d", s32);
  zeros(lcg2,      "%12d", s32);
  zeros(mt19937,   "%12d", s32);
  zeros(pcg,       "%12d", s32);
  zeros(photon,    "%12d", s32);
  zeros(romuduo,   "%12d", s32);
  zeros(splitmix,  "%12d", s32);
  zeros(wy,        "%12d", s32);
  zeros(xoroshiro, "%12d", s32);
  zeros(xorshift,  "%12d", s32);
  /* u64 */
  zeros(lcg,       "%20ld", u64);
  zeros(lcg2,      "%20ld", u64);
  zeros(mt19937,   "%20ld", u64);
  zeros(pcg,       "%20ld", u64);
  zeros(photon,    "%20ld", u64);
  zeros(romuduo,   "%20ld", u64);
  zeros(splitmix,  "%20ld", u64);
  zeros(wy,        "%20ld", u64);
  zeros(xoroshiro, "%20ld", u64);
  zeros(xorshift,  "%20ld", u64);
  /* floats */
  zeros(lcg,       "%f", f32);
  zeros(lcg2,      "%f", f32);
  zeros(mt19937,   "%f", f32);
  zeros(pcg,       "%f", f32);
  zeros(photon,    "%f", f32);
  zeros(romuduo,   "%f", f32);
  zeros(splitmix,  "%f", f32);
  zeros(wy,        "%f", f32);
  zeros(xoroshiro, "%f", f32);
  zeros(xorshift,  "%f", f32);

  printf("grand %f\n", grand_f32());
  printf("grand %f\n", grand_f32());
  printf("grand %f\n", grand_range_f32(2,1000));
}
