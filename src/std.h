#ifndef _STD_H_
#define _STD_H_

#include <uefi/uefi.h>

/**
 * @brief Swaps all the bits of a byte (swaps endian-ness of byte)
 */
uint8_t swap_byte(uint8_t byte);

/**
 * @brief Swaps all the bits of a 16-bit int (swaps endian-ness of int)
 */
uint16_t swap_int16(uint16_t int16);

void handle_error(char *string);

#endif
