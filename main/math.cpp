#include <stdio.h>
#include <stdarg.h>
#include "function.h"

int Xor(uint8_t data[], size_t data_len) {
    if (data_len <= 0) {
        return 0;
    }
    int result = data[0];
    for (int i = 1; i < data_len - 1; i++) {
        result ^= data[i];
    }
    if (result == data[data_len - 1]) {
        return 1;
    } else {
        return 0;
    }
}
// MSB -> LSB
int combineBytes(int bytes, ...) {
    va_list args;
    va_start(args, bytes);
    int result = 0;
    for (int i = 0; i < bytes; i++) {
        int byte = va_arg(args, int);
        result = (result * 256) + byte;
    }

    va_end(args);
    return result;
}

void byte_to_Bits(uint8_t byte, uint8_t bits[8]) {
    int i;
    for (i = 7; i >= 0; i--) {
        bits[i] = (byte >> i) & 0x01;
    }
}

void value_to_bin(int digit, uint8_t bit[]) {
    if (digit < 0) {
        for (int i = 0; i < 8; ++i) {
            bit[i] = 0;
        }
        return;
    }
    else if (digit > 9) {
        for (int i = 0; i < 8; ++i) {
            bit[i] = 1;
        }
        return;
    }
    uint8_t binary_digits[10][8] = {
        {1,1,1,1,1,1,0,0}, // 0
        {0,1,1,0,0,0,0,0}, // 1
        {1,1,0,1,1,0,1,0}, // 2
        {1,1,1,1,0,0,1,0}, // 3
        {0,1,1,0,0,1,1,0}, // 4
        {1,0,1,1,0,1,1,0}, // 5
        {0,0,1,1,1,1,1,0}, // 6
        {1,1,1,0,0,0,0,0}, // 7
        {1,1,1,1,1,1,1,0}, // 8
        {1,1,1,0,0,1,1,0}  // 9
    };

    for (int i = 0; i < 8; ++i) {
        bit[i] = binary_digits[digit][i];
    }
}
uint8_t* set_led_bytes(int cmd, uint8_t data1[], uint8_t data2[], uint8_t data3[]) {

    uint8_t binaryNumber1 = 0;
    uint8_t binaryNumber2 = 0;
    uint8_t binaryNumber3 = 0;

   for( int i = 0; i < 8 ; i++ ) {
        data1[i] = data1[i] << i ;
        data2[i] = data2[i] << i;
        data3[i] = data3[i] << i;
        binaryNumber1 = binaryNumber1 | data1[i];
        binaryNumber2 = binaryNumber2 | data2[i];
        binaryNumber3 = binaryNumber3 | data3[i];
    }


    uint8_t* led_data = (uint8_t*)malloc(6);
    led_data[0] = 0xF1;
    led_data[1] = cmd;
    led_data[2] = binaryNumber1;
    led_data[3] = binaryNumber2;
    led_data[4] = binaryNumber3;
    led_data[5] = 0x1F;

    return led_data;
}

int count_digits(int number) {
    if (number == 0) {
        return 1; // 0 的位数为 1
    }
    
    int digits = 0;
    while (number != 0) {
        number /= 10;
        digits++;
    }
    return digits;
}

bool compareArrays(const uint8_t array1[], const uint8_t array2[], size_t length) {
    for (size_t i = 0; i < length; ++i) {
        if (array1[i] != array2[i]) {
            return false;
        }
    }
    return true;
}
void clearArray(uint8_t array[], size_t length) {
    for (size_t i = 0; i < length; ++i) {
        array[i] = 0;
    }
}