#ifndef I0RANDOM_H
#define I0RANDOM_H

#include "math.h"

#define RANDOM_M 0x80000000
#define RANDOM_A 1103515245
#define RANDOM_C 12345

#define random_t long

//init the generator with the given __seed__;
random_t create_random(long seed);

//use the worker id as a seed;
random_t create_random_no_seed();

// return a random value between __from__ and __to__
long rand(random_t* random_generator, long from, long to);


#endif
