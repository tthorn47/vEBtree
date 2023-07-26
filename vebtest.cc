#include <stdio.h>
#include <set>
#include "vebTree.hpp"

/**
 * van Emde Boas Tree Test
 * Author: Todd Thornley
 * A simple correctness test for the data structure. Checking both that all items given
 * are inserted and all successor queries match STL set, which is assumed to be correct.
 */

int main(int argc, char** argv) {

    if(argc < 2){
        printf("Specify number of items\n");
        exit(1);
    }

    uint64_t N = atoi(argv[1]);

    uint32_t* ins = (uint32_t*)malloc(sizeof(uint32_t)*N);
    uint32_t* suc = (uint32_t*)malloc(sizeof(uint32_t)*N);

    if(ins == NULL || suc == NULL){
        printf("malloc failed\n");
        exit(1);
    }

    safe_rand_bytes((uint8_t*)ins, N*sizeof(*ins));
    safe_rand_bytes((uint8_t*)suc, N*sizeof(*suc));

    std::set<uint32_t> result_check;
    vebTree* v = new vebTree((uint64_t)(UINT32_MAX)+1);
    
    high_resolution_clock::time_point t1,t2;
    t1 = high_resolution_clock::now();
    for(size_t i = 0; i < N; i++){
        insert(v, ins[i]);
    }
    t2 = high_resolution_clock::now();
    printf("Time to insert %lu items: %f secs\n", N, elapsed(t1,t2));

    for(size_t i = 0; i < N; i++){
        result_check.insert(ins[i]);
    }

    t1 = high_resolution_clock::now();
    for(size_t i = 0; i < N; i++){
        find(v, ins[i]);
    }
    t2 = high_resolution_clock::now();
    printf("Time to query contains for %lu items: %f secs\n", N, elapsed(t1,t2));

    t1 = high_resolution_clock::now();
    for(size_t i = 0; i < N; i++){
        succ(v, suc[i]);
    }
    t2 = high_resolution_clock::now();
    printf("Time to query successor for %lu items: %f secs\n", N, elapsed(t1,t2));

    for(size_t i = 0; i < N; i++){
        bool found = find(v,ins[i]);
        if(!found){
            printf("item %u not found!\n", ins[i]);
            exit(1);
        }
        uint32_t res = succ(v, suc[i]);
        auto res_check = result_check.upper_bound(suc[i]);

        //Check if the successor is greater than the given value
        if(res < suc[i] 
        //Check if the successor is in the inserted values or properly returned invalid
        || (result_check.count(res) == 0 && (res == UINT32_MAX && result_check.upper_bound(res) != result_check.end()))
        || res != *res_check && res_check != result_check.end()){

            printf("succ failed res:%u res check:%u val:%u \n", res, *res_check, suc[i]);
            exit(1);
        }

        //double check that find isn't returning bogus values
        if(find(v,suc[i]) == (result_check.find(suc[i]) == result_check.end())){
            printf("item %u find disparity! v find = %u bst find = %u\n", suc[i],(uint32_t)find(v,suc[i]),(result_check.find(suc[i]) == result_check.end()));
            exit(1);
        }
    }
    printf("results verified!\n");
    free(ins);
    free(suc);
}