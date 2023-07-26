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
 * In addition, all values passed to the vebTree constructor are assumed to be powers of 2.
 * This would be necessary via rounding even if the assumption weren't made due to the nature
 * of the bitwise operatons.
 *
 * For an overview of the algorithm itself please see wikipedia: https://en.wikipedia.org/wiki/Van_Emde_Boas_tree
 * 
*/


vebTree::vebTree(uint64_t n){
    
    this->min = UINT32_MAX; //min and max values for this node over all of its descendants
    this->max = UINT32_MAX;

    if(n == 2) {
        this->root = 0;
        return;
    }

    this->root = sqrt(n);                                                   //How many nodes are directly beneath this node
    this->level =  bit_width(n-1U)/2;                                       //What is the depth of this node
    this->bottom = (n-1U) >> this->level;                                   //Mask used to slice off lower order bits for value insertion
    this->summary = new vebTree(this->root);                                //Summary node showing which children are populated
    this->children = (vebTree**)malloc(sizeof(vebTree*) * (this->root));    //Pre-allocated space for child structures
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

    if(givenVal < currentLvl->min) //successor is in bounds of current node, but below the min
        return currentLvl->min;

    uint32_t highBits = givenVal >> currentLvl->level;
    uint32_t lowBits = givenVal & currentLvl->bottom;

    uint32_t temp;
    if (currentLvl->children != NULL && currentLvl->children[highBits] != NULL  //Check for successor in current node
        && currentLvl->children[highBits]->max > lowBits 
        && currentLvl->children[highBits]->max != UINT32_MAX){

        temp = succ(currentLvl->children[highBits], lowBits);
    } else {
        temp = UINT32_MAX;
    }

    if(temp != UINT32_MAX) //Return successor if found
        return (highBits << currentLvl->level) | temp;

    temp = succ(currentLvl->summary, highBits); //check the summary for the appropriate high bits

    //No successor found, also functions as a null check for the final return
    if(temp == UINT32_MAX)
        return UINT32_MAX;
    
    return (temp << currentLvl->level) | succ(currentLvl->children[temp], UINT32_MAX); //retrieve the low bits directly from the min of the next node down
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
    if(currentLvl->min == UINT32_MAX){ //Level is empty, no further recursion needed
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

    if(currentLvl->root == 0){ //Final base case, tree has bottomed out to two bit nodes
        return;
    }

    uint32_t highBits = recur >> currentLvl->level;
    uint32_t lowBits = recur & currentLvl->bottom;

    if(currentLvl->children[highBits] == NULL){ // New subnode needed
        currentLvl->children[highBits] = new vebTree(currentLvl->root);
        insert(currentLvl->summary, highBits);
    }
    insert(currentLvl->children[highBits], lowBits);
}

