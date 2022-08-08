#ifndef _PCI_H_
#define _PCI_H_

#define PCI_CAP_MSI 0x5

#include "types.h"

/**
 * @brief Searches for all PCI entries in the MCFG, returning a list of pointers  
 */
pci_device_list_t init_pci(mcfg_t *mcfg);

/**
 * @brief Enabled message signaled interrupts for the given PCI entry
 * 
 * @return True if interrupts successfully enabled
 */
bool enable_msi(pci_header_0_t *header);

/**
 * @brief Finds the PCI header located at the given bus for the given device using the given
 * function
 */
static pci_header_t *get_pci_header_at(uint8_t bus, uint8_t device, uint8_t function);

/**
 * @brief Checks the given bus of the given device and adds it to the device list
 */
static void check_device(uint8_t bus, uint8_t device);

/**
 * @brief Checks the given bus, adding devices found to the device list, and checks any buses that
 * are attached/at different functions of the device
 */
static void check_bus(uint8_t bus);

/**
 * @brief Recursively checks for any entries in every bus in the PCI address space, adding devices
 * found to the device list
 */
static void check_all_buses();

#endif