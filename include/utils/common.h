#define COMMON_H

#include <gb/gb.h>

void translateMapToCurrentOffset(unsigned char *map, uint8_t totalElements, uint8_t current_offset);
uint8_t generateRandomNumber(uint8_t min, uint8_t max);