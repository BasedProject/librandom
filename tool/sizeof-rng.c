// @BAKE gcc -I../include/ -I../source/ -o $*.out $@ ../object/librandom.out.a -std=c23
#include <stdio.h>
#include <random.h>

#define sizeof_rng(RNG) do {\
    auto rng = RNG ## _init_raw(0);\
    printf("  sizeof(" # RNG "): %*ld\n", 20 - (int)sizeof(# RNG), sizeof(rng));\
} while (0)

signed main(void) {
    sizeof_rng(lcg);
    sizeof_rng(lcg2);
    //sizeof_rng(pcg); // XXX
    sizeof_rng(xorshift);
    sizeof_rng(xoroshiro);
    sizeof_rng(splitmix);
    sizeof_rng(wy);
    sizeof_rng(photon);
    sizeof_rng(romuduo);
    sizeof_rng(mt19937);

    return 0;
}
