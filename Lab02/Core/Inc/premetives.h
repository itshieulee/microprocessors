/*
 * premetives.h
 *
 *  Created on: Oct 1, 2024
 *      Author: nguye
 */

#ifndef INC_PREMETIVES_H_
#define INC_PREMETIVES_H_

const int MAX_LED_MATRIX = 8;

/* Row bitmap: bit 7 is the leftmost pixel. Shared by Ex9 and Ex10. */
const uint8_t A_pattern[8] = {
    0b00011000,
    0b00100100,
    0b01000010,
    0b01000010,
    0b01111110,
    0b01000010,
    0b01000010,
    0b00000000
};

uint8_t matrix_buffer[8] = {
    0b00011000,
    0b00100100,
    0b01000010,
    0b01000010,
    0b01111110,
    0b01000010,
    0b01000010,
    0b00000000
};


#endif /* INC_PREMETIVES_H_ */
