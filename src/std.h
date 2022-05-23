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

void handle_error(char *string) {
    printf(string);
    printf("Enter anything to continue: ");
    char c = getchar();
}