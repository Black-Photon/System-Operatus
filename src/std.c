#include "std.h"

uint8_t swap_byte(uint8_t byte) {
    uint8_t new_byte;
    new_byte += (byte << 7) & 0b10000000;
    new_byte += (byte << 5) & 0b01000000;
    new_byte += (byte << 3) & 0b00100000;
    new_byte += (byte << 1) & 0b00010000;
    new_byte += (byte >> 1) & 0b00001000;
    new_byte += (byte >> 3) & 0b00000100;
    new_byte += (byte >> 5) & 0b00000010;
    new_byte += (byte >> 7) & 0b00000001;
    return new_byte;
}

uint16_t swap_int16(uint16_t int16) {
    uint16_t new_int16;
    new_int16 += (swap_byte((uint8_t) (int16 >> 8) & 0xFF));
    new_int16 += (swap_byte((uint8_t) int16 & 0xFF)) << 8;
    return new_int16;
}

void handle_error(char *string) {
    printf(string);
    printf("Enter anything to continue: ");
    char c = getchar();
}
