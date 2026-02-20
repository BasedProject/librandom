# Makefile for Random Number Generator C implementations

CC = cc
CFLAGS = -O2 -Wall -Wextra -std=c99
LDFLAGS = -lm

TARGETS = photon_spin_random xorshift_random splitmix64 wy_random pcg_random xoroshiro_random

.PHONY: all clean test

all: $(TARGETS)

photon_spin_random: photon_spin_random.c photon_spin_random.h
	$(CC) $(CFLAGS) -DPHOTON_SPIN_TEST_MAIN -o $@ photon_spin_random.c $(LDFLAGS)

xorshift_random: xorshift_random.c xorshift_random.h
	$(CC) $(CFLAGS) -DXORSHIFT_TEST_MAIN -o $@ xorshift_random.c $(LDFLAGS)

splitmix64: splitmix64.c splitmix64.h
	$(CC) $(CFLAGS) -DSPLITMIX64_TEST_MAIN -o $@ splitmix64.c $(LDFLAGS)

wy_random: wy_random.c wy_random.h
	$(CC) $(CFLAGS) -DWY_TEST_MAIN -o $@ wy_random.c $(LDFLAGS)

pcg_random: pcg_random.c pcg_random.h
	$(CC) $(CFLAGS) -DPCG_TEST_MAIN -o $@ pcg_random.c $(LDFLAGS)

xoroshiro_random: xoroshiro_random.c xoroshiro_random.h
	$(CC) $(CFLAGS) -DXOROSHIRO_TEST_MAIN -o $@ xoroshiro_random.c $(LDFLAGS)

test: $(TARGETS)
	@echo "=== Testing PhotonSpinRandom ==="
	./photon_spin_random
	@echo "\n=== Testing XorShiftRandom ==="
	./xorshift_random
	@echo "\n=== Testing SplitMix64 ==="
	./splitmix64
	@echo "\n=== Testing WyRandom ==="
	./wy_random
	@echo "\n=== Testing PCG Random ==="
	./pcg_random
	@echo "\n=== Testing XoroShiro Random ==="
	./xoroshiro_random

clean:
	rm -f $(TARGETS)
