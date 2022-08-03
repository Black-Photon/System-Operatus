#include "ahci.h"

bool is_ahci(pci_header_t *pci_header) {
    return
        swap_byte(pci_header->header_type) == 0x0 &&
        pci_header->class_code == 0x01 &&
        pci_header->subclass == 0x06;
}

void init_ahci(pci_device_list_t device_list) {
    size_t device_list_size = device_list.device_list_size;
    pci_header_t **all_devices = device_list.all_devices;

    pci_header_0_t *ahci_entry = NULL;

    // Iterate over all PCI devices to find ACPI
    for (size_t i = 0; i < device_list_size; i++) {
        pci_header_t *pci_header = all_devices[i];

        if (is_ahci(pci_header)) {
            pci_header_0_t *pci_header_0 = pci_header;
            hba_t *hba = pci_header_0->bar5; // Get the HBA table from ABAR (BAR5)
            
        }
    }
}