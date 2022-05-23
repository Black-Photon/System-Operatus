#ifndef _PCI_H_
#define _PCI_H_

#include "types.h"

void init_pci(mcfg_t *mcfg);

pci_header_t *get_pci_header_at(uint8_t bus, uint8_t device, uint8_t function);

void check_device(uint8_t bus, uint8_t device);

void check_bus(uint8_t bus);

void check_all_buses();

#endif