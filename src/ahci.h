#ifndef _AHCI_H_
#define _AHCI_H_

#define	SATA_SIG_ATA	0x00000101	// SATA drive
#define	SATA_SIG_ATAPI	0xEB140101	// SATAPI drive
#define	SATA_SIG_SEMB	0xC33C0101	// Enclosure management bridge
#define	SATA_SIG_PM	    0x96690101	// Port multiplier
 
#define AHCI_DEV_NULL 0
#define AHCI_DEV_SATA 1
#define AHCI_DEV_SEMB 2
#define AHCI_DEV_PM 3
#define AHCI_DEV_SATAPI 4
 
#define HBA_PORT_IPM_NOT_PRESENT 0
#define HBA_PORT_IPM_ACTIVE 1
#define HBA_PORT_IPM_PARTIAL_POWER 2
#define HBA_PORT_IPM_SLUMBER 6
#define HBA_PORT_IPM_DEVSLEEP 8

#define HBA_PORT_DET_NOT_PRESENT 0
#define HBA_PORT_DET_NOT_ESTABLISHED 1
#define HBA_PORT_DET_PRESENT 3
#define HBA_PORT_DET_OFFLINE 4

#include <stdbool.h>

#include "types.h"

/**
 * @brief Initialises AHCI from a list of PCI devices
 * 
 * @param device_list List of PCI devices to search within for AHCI devices
 * @return True if AHCI successfully found and initialised
 */
bool init_ahci(pci_device_list_t device_list);

/**
 * @brief Finds if the given PCI entry is an AHCI interface
 */
static bool is_ahci(pci_header_t *pci_header);

/**
 * @brief Finds the first open port in the given HBA, returning that entry
 * 
 * @param hba Input HBA to search for open ports
 * @param port Output port pointer
 * @return True if an open port was found and returned
 */
static bool find_open_port(hba_t *hba, hba_port_t** port);

/**
 * @brief Finds and returns the type of the given port
 */
static uint8_t check_type(hba_port_t *port);

#endif