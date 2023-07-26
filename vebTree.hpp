#pragma once
#include <cmath>
#include <openssl/rand.h>
#include <chrono>

using namespace std;
using namespace chrono;


static double elapsed(high_resolution_clock::time_point t1, high_resolution_clock::time_point t2) {
	return (duration_cast<duration<double>>(t2 - t1)).count();
}

static void safe_rand_bytes(unsigned char *v, uint32_t n) {
	while (n > 0) {
		size_t round_size = n >= INT_MAX ? INT_MAX - 1 : n;
		RAND_bytes(v, round_size);
		v += round_size;
		n -= round_size;
	}
}

class vebTree {
    public:
        vebTree(uint64_t n);
        uint32_t min; //Minimum stored val
        uint32_t max;
        uint32_t bottom; //Mask for lower order bits in computation
        uint32_t root; //Size of the node, the root of all nodes above
        uint32_t level; //Level of the tree as a decimeal number, used for masking

        vebTree* summary; 
        vebTree** children;
};

uint32_t succ(vebTree* v, uint32_t x);
bool find(vebTree* v, uint32_t x);
void insert(vebTree* v, uint32_t x);

