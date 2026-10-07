/*
 * functions.h
 *
 *  Created on: Sep 30, 2024
 *      Author: nguye
 */

#ifndef INC_FUNCTIONS_H_
#define INC_FUNCTIONS_H_

#include "main.h"
#include "premetives.h"

int clock_buffer[] = {0,0,0,0};
/* Ex3's buffer is reused by Ex4 and the digital clock in Ex5..10. */
volatile int led_buffer[4] = {1, 2, 3, 4};

const uint8_t segmentMap[10] = {
    0b11000000, // 0
    0b11111001, // 1
    0b10100100, // 2
    0b10110000, // 3
    0b10011001, // 4
    0b10010010, // 5
    0b10000010, // 6
    0b11111000, // 7
    0b10000000, // 8
    0b10010000  // 9
};
void display7seg(int nth, int value) {
	if (nth < 0 || nth > 3 || value < 0 || value > 9) return;

	/* Disable every digit before changing segment data to avoid ghosting. */
	HAL_GPIO_WritePin(GPIOA, en0_Pin | en1_Pin | en2_Pin | en3_Pin, GPIO_PIN_SET);

    uint8_t segments = segmentMap[value];  // Get segment pattern for the value

    // Assuming 7-segment pins are connected to GPIOB Pins 0-6
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, (segments & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (segments & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, (segments & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (segments & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (segments & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (segments & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (segments & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    /* PNP digit drivers are active low. Enable only after data is ready. */
    const uint16_t enablePins[4] = {en0_Pin, en1_Pin, en2_Pin, en3_Pin};
    HAL_GPIO_WritePin(GPIOA, enablePins[nth], GPIO_PIN_RESET);
}

void clearLED() {
    // Turn off all segments (assuming GPIOB pins 0-6 control the segments)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                      GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6, GPIO_PIN_SET);
}

void clearEnableVsLED()
{
	HAL_GPIO_WritePin(GPIOA, en0_Pin | en1_Pin | en2_Pin | en3_Pin, GPIO_PIN_SET);
	clearLED();
}

void update7SEG(int index)
{
    switch (index) {
    case 0: display7seg(0, led_buffer[0]); break;
    case 1: display7seg(1, led_buffer[1]); break;
    case 2: display7seg(2, led_buffer[2]); break;
    case 3: display7seg(3, led_buffer[3]); break;
    default: break;
    }
}



void updateClockBuffer(volatile int *clock_buffer, int hour, int min, int second) {
	clock_buffer[0] = hour / 10;
	clock_buffer[1] = hour % 10;
	clock_buffer[2] = min / 10;
	clock_buffer[3] = min % 10;
}

void clockDigit()
{
	int hour = 15, minute = 8, second = 50;

	while (1) {
	    second++;

	    if (second >= 60) {
	        second = 0;
	        minute++;
	    }

	    if (minute >= 60) {
	        minute = 0;
	        hour++;
	    }

	    if (hour >= 24) {
	        hour = 0;
	    }

	    updateClockBuffer(clock_buffer, hour, minute, second);
	    HAL_Delay(1000);
	}
}

void dis7seg(int value)
{
	uint8_t segments = segmentMap[value];  // Get segment pattern for the value

	// Assuming 7-segment pins are connected to GPIOB Pins 0-6
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, (segments & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (segments & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, (segments & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (segments & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (segments & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (segments & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (segments & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


void setRow(int row)
{
    HAL_GPIO_WritePin(GPIOB, row0_Pin, (row == 0)? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, row1_Pin, (row == 1)? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, row2_Pin, (row == 2)? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, row3_Pin, (row == 3)? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, row4_Pin, (row == 4)? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, row5_Pin, (row == 5)? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, row6_Pin, (row == 6)? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, row7_Pin, (row == 7)? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void setColumn(int value)
{
    HAL_GPIO_WritePin(GPIOA, em0_Pin, (value & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, em1_Pin, (value & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, em2_Pin, (value & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, em3_Pin, (value & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, em4_Pin, (value & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, em5_Pin, (value & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, em6_Pin, (value & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, em7_Pin, (value & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* Circular rotation, available separately if another animation needs it. */
void rotateLeft(uint8_t matrix_buffer[8])
{
	for (int i = 0; i < 8; i++) {
		uint8_t leftBit = (matrix_buffer[i] & 0x80) >> 7;
		matrix_buffer[i] = (matrix_buffer[i] << 1) | leftBit;
	}
}

/* Shift left with zero fill: pixels leaving the left edge disappear. */
void shiftLeft(uint8_t matrix_buffer[8])
{
    for (int i = 0; i < 8; i++) {
        matrix_buffer[i] <<= 1;
    }
}

void displayMatrixColumn(int index)
{
    uint8_t rows = 0;
    for (int row = 0; row < 8; row++) {
        if (matrix_buffer[row] & (0x80 >> index)) rows |= (1 << row);
    }
    setColumn(0);  /* disable columns before changing row data */
    HAL_GPIO_WritePin(GPIOB, row0_Pin, (rows & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, row1_Pin, (rows & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, row2_Pin, (rows & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, row3_Pin, (rows & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, row4_Pin, (rows & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, row5_Pin, (rows & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, row6_Pin, (rows & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, row7_Pin, (rows & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    setColumn(1 << index);
}

void updateLEDMatrix(int index)
{
    switch (index) {
    case 0: displayMatrixColumn(0); break;
    case 1: displayMatrixColumn(1); break;
    case 2: displayMatrixColumn(2); break;
    case 3: displayMatrixColumn(3); break;
    case 4: displayMatrixColumn(4); break;
    case 5: displayMatrixColumn(5); break;
    case 6: displayMatrixColumn(6); break;
    case 7: displayMatrixColumn(7); break;
    default: break;
    }
}

#endif /* INC_FUNCTIONS_H_ */
