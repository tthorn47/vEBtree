#include <stdio.h>
#include <cmath>
#include <set>
#include <chrono>
#include <openssl/rand.h>
#include <bit>

#include "vebTree.hpp"

/**
 * van Emde Boas Tree
 * Author: Todd Thornley
 * 
 * PLEASE NOTE: in this implementation UINT32_MAX is a reserved value
 * representing an empty/invalid state. Attempting to insert that value will
 * result in undefined behavior.
 * 
 * 
*/


vebTree::vebTree(uint64_t n){
    
    this->min = UINT32_MAX;
    this->max = UINT32_MAX;
    if(n == 2) {
        this->root = 0;
        return;
    }

    this->root = sqrt(n);
    this->level =  bit_width(n-1U)/2;
    this->bottom = (n-1) >> this->level;
    this->summary = new vebTree(this->root);
    this->children = (vebTree**)malloc(sizeof(vebTree*) * (this->root));
};


uint32_t succ(vebTree* currentLvl, uint32_t givenVal){

    if(givenVal == UINT32_MAX) //Negative infinity
      return currentLvl->min;

    if(currentLvl->root == 0){ //base case for recursive calls
        if((givenVal == 0) && currentLvl->max == 1){
            return 1;
        } 
        else{
            return UINT32_MAX;
        }
    }

    if(currentLvl == NULL || givenVal >= currentLvl->max) //Empty or past max
        return UINT32_MAX;

    if(givenVal < currentLvl->min)
        return currentLvl->min;

    uint32_t highBits = givenVal >> currentLvl->level;
    uint32_t lowBits = givenVal & currentLvl->bottom;
    uint32_t temp;
    if (currentLvl->children != NULL && currentLvl->children[highBits] != NULL  
        && currentLvl->children[highBits]->max > lowBits && currentLvl->children[highBits]->max != UINT32_MAX){
        temp = succ(currentLvl->children[highBits], lowBits);
    } else {
        temp = UINT32_MAX;
    }

    if(temp != UINT32_MAX)
        return (highBits << currentLvl->level) | temp;

    temp = succ(currentLvl->summary, highBits);

    if(temp == UINT32_MAX)
        return UINT32_MAX;
    
    return (temp << currentLvl->level) | succ(currentLvl->children[temp], UINT32_MAX);
}

bool find(vebTree* vTree, uint32_t givenVal){
    if(givenVal == 0){
        return vTree->min == 0;
    } else if (givenVal == (UINT32_MAX-1)){
        return vTree->max == (UINT32_MAX-1);
    } else if (givenVal == (UINT32_MAX)){
        return false;
    } else {
        return succ(vTree, givenVal-1) == givenVal;
    }
}


void insert(vebTree* currentLvl, uint32_t ins){
    if(currentLvl->min == UINT32_MAX){ //Level is empty
        currentLvl->min = ins;
        currentLvl->max = ins;
        return;
    }

    uint32_t recur;
    if (ins < currentLvl->min){ //Value being inserted is replacing current min
        recur = currentLvl->min;  //Old min is inserted recursively in place of the given value
        currentLvl->min = ins;
    } else {
        recur = ins;
    }

    if(currentLvl->max < recur)
        currentLvl->max = recur;

    if(currentLvl->root == 0){ 
        return;
    }

    uint32_t highBits = recur >> currentLvl->level;
    uint32_t lowBits = recur & currentLvl->bottom;

    if(currentLvl->children[highBits] == NULL){
        currentLvl->children[highBits] = new vebTree(currentLvl->root);
        insert(currentLvl->summary, highBits);
    }
    insert(currentLvl->children[highBits], lowBits);
}

