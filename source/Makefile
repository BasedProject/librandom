# Makefile for PhotonSpinRandom C implementation

CC = cc
CFLAGS = -O2 -Wall -Wextra -std=c99
LDFLAGS = -lm

TARGET = photon_spin_random
SRC = photon_spin_random.c
HEADER = photon_spin_random.h

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SRC) $(HEADER)
	$(CC) $(CFLAGS) -DPHOTON_SPIN_TEST_MAIN -o $(TARGET) $(SRC) $(LDFLAGS)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
