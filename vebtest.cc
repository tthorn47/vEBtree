#include <stdio.h>
#include <set>
#include <bit>

#include "vebTree.hpp"

//#define DEBUG

int main(int argc, char** argv) {

    if(argc < 2){
        printf("Specify number of items\n");
        exit(1);
    }

    uint64_t N = atoi(argv[1]);

    uint32_t* ins = (uint32_t*)malloc(sizeof(uint32_t)*N);
    uint32_t* suc = (uint32_t*)malloc(sizeof(uint32_t)*N);

    safe_rand_bytes((uint8_t*)ins, N*sizeof(*ins));
    safe_rand_bytes((uint8_t*)suc, N*sizeof(*suc));
    if(ins == NULL || suc == NULL){
        printf("malloc failed\n");
        exit(1);
    }

    std::set<uint32_t> result_check;
    vebTree* v = new vebTree((uint64_t)(UINT32_MAX)+1);
    
    //printf("size: %lu\n", sizeof(vebTree));
    high_resolution_clock::time_point t1,t2;
    t1 = high_resolution_clock::now();
    for(size_t i = 0; i < N; i++){
        //printf("ins %u\n", ins[i]);
        insert(v, ins[i]);
    }
    t2 = high_resolution_clock::now();
    printf("Time to insert %lu items: %f secs\n", N, elapsed(t1,t2));

    for(size_t i = 0; i < N; i++){
        //printf("ins %u\n", ins[i]);
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

    #ifdef DEBUG
    uint32_t check = 0;
    #endif

    for(size_t i = 0; i < N; i++){
        bool found = find(v,ins[i]);
        if(!found){
            printf("item %u not found!\n", ins[i]);
            #ifdef DEBUG
            check = ins[i];
            break;
            #endif
            exit(1);
        }
        uint32_t res = succ(v, suc[i]);
        auto res_check = result_check.upper_bound(suc[i]);
        //Check if the successor is greater than the given value
        if(res < suc[i] 
        //Check if the successor is in the inserted values or properly returned invalid
        || (result_check.count(res) == 0 && (res == UINT32_MAX && result_check.upper_bound(res) != result_check.end()))
        || res != *res_check && res_check != result_check.end()){

            #ifdef DEBUG
            if(check == 0)
                check = suc[i];
            #endif

            printf("succ failed res:%u res check:%u val:%u \n", res, *res_check, suc[i]);

            #ifndef DEBUG
            exit(1);
            #endif
        }

        //double check find against values that may not be present
        if(find(v,suc[i]) == (result_check.find(suc[i]) == result_check.end())){
            printf("item %u find disparity! v find = %u bst find = %u\n", suc[i],(uint32_t)find(v,suc[i]),(result_check.find(suc[i]) == result_check.end()));
            exit(1);
        }
    }
    printf("results verified!\n");
    free(ins);
    free(suc);
    //free(result_check);

    #ifdef DEBUG
    if(check > 0){
        succ(v, check);
    }
    #endif
}