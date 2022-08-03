#ifndef _AHCI_H_
#define _AHCI_H_

#include <stdbool.h>

#include "types.h"

bool is_ahci(pci_header_t *pci_header);

void init_ahci(pci_device_list_t device_list);

#endif