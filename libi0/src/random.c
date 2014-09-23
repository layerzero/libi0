#include "./random.h"

random_t create_random(long seed) {
	return seed;
}

random_t create_random_no_seed() {
	return  *(long*)0x100000120;
}

long rand(random_t* random_generator, long from, long to) {
	long temp;
	if(from > to){
		temp = to;
		to = from;
		from = temp;
	}
	*random_generator = RANDOM_A* (*random_generator) + RANDOM_C;
	*random_generator = remainder(*random_generator, RANDOM_M);

	return remainder(*random_generator, to-from+1) + from;

}

