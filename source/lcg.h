#ifndef RANDOM_LCG_H_
#define RANDOM_LCG_H_
#include <stdint.h>

typedef uint32_t random_lcg32_t;

random_lcg32_t random_lcg32_init(uint32_t init);
uint32_t random_lcg32(random_lcg32_t * randomp);

#endif /* RANDOM_LCG_H_ */
