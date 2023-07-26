#include <stdio.h>
#include <set>
#include "vebTree.hpp"

int main(int argc, char** argv){
    if(argc < 2){
        printf("Specify number of items\n");
        exit(1);
    }

    FILE* out = fopen("res.csv", "a");
    uint64_t N = atoi(argv[1]);

    uint32_t* ins = (uint32_t*)malloc(sizeof(uint32_t)*N);
    uint32_t* suc = (uint32_t*)malloc(sizeof(uint32_t)*N);
    uint32_t* toFind = (uint32_t*)malloc(sizeof(uint32_t)*N);
    if(ins == NULL || suc == NULL || toFind == NULL){
        printf("malloc failed\n");
        exit(1);
    }

    safe_rand_bytes((uint8_t*)ins, N*sizeof(uint32_t));
    safe_rand_bytes((uint8_t*)suc, N*sizeof(uint32_t));
    for (size_t i = 0; i < N; i++)
    {
        if(i % 2 == 0){
            toFind[i] = ins[i];
        } else {
            toFind[i] = suc[i];
        }
    }
    

    std::set<uint32_t>* bt = new std::set<uint32_t>();
    vebTree* v = new vebTree((uint64_t)(UINT32_MAX)+1);

    //INSERT

    high_resolution_clock::time_point t1,t2;
    t1 = high_resolution_clock::now();
    for(size_t i = 0; i < N; i++){
        insert(v, ins[i]);
    }
    t2 = high_resolution_clock::now();
    fprintf(out, "%f, ",elapsed(t1,t2));
    printf("vebTree Time to insert %lu items: %f secs\n", N, elapsed(t1,t2));

    t1 = high_resolution_clock::now();
    for(size_t i = 0; i < N; i++){
        bt->insert(ins[i]);
    }
    t2 = high_resolution_clock::now();
    fprintf(out, "%f\n",elapsed(t1,t2));
    printf("std::Set Time to insert %lu items: %f secs\n", N, elapsed(t1,t2));

    //FIND

    t1 = high_resolution_clock::now();
    for(size_t i = 0; i < N; i++){
        find(v, toFind[i]);
    }
    t2 = high_resolution_clock::now();
    fprintf(out, "%f, ",elapsed(t1,t2));
    printf("vebTree Time to find %lu items: %f secs\n", N, elapsed(t1,t2));
    
    t1 = high_resolution_clock::now();
    for(size_t i = 0; i < N; i++){
        bt->find(toFind[i]);
    }
    t2 = high_resolution_clock::now();
    fprintf(out, "%f\n",elapsed(t1,t2));
    printf("std::Set Time to find %lu items: %f secs\n", N, elapsed(t1,t2));

    //SUCCESSOR

    t1 = high_resolution_clock::now();
    for(size_t i = 0; i < N; i++){
        succ(v, suc[i]);
    }
    t2 = high_resolution_clock::now();
    fprintf(out, "%f, ",elapsed(t1,t2));
    printf("vebTree Time to find successor for %lu items: %f secs\n", N, elapsed(t1,t2));

    t1 = high_resolution_clock::now();
    for(size_t i = 0; i < N; i++){
        bt->upper_bound(suc[i]);
    }
    t2 = high_resolution_clock::now();
    fprintf(out, "%f\n\n",elapsed(t1,t2));
    printf("std::Set Time to find successor for %lu items: %f secs\n", N, elapsed(t1,t2));
}