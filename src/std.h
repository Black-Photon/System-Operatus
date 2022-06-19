#ifndef _STD_H_
#define _STD_H_

#include <uefi/uefi.h>

/**
 * @brief Swaps all the bits of a byte (swaps endian-ness of byte)
 */
uint8_t swap_byte(uint8_t byte);

void handle_error(char *string);

#endif
