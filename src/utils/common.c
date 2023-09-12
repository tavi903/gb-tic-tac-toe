#ifndef COMMON_H
#include "./../../include/utils/common.h"
#endif

#include <rand.h>

void translateMapToCurrentOffset(unsigned char *map, uint8_t totalElements, uint8_t current_offset) {
    for (uint8_t i = 0; i < totalElements; i++) {
        map[i] += current_offset;  
    }
}

uint8_t generateRandomNumber(uint8_t min, uint8_t max) {
    /*
    unsigned char *ptr_div_reg = (unsigned char*) 0xFF04; // DIV Register
    return min+(*(ptr_div_reg) % (max-min));    // get value at memory address
    */
    return (rand() % (max + 1)) + min;
}