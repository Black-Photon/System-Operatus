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

            printf("Ports Supported: %d\n", (hba->capabilities & 0x1F) + 1);  // First 5 bits
            uint8_t no_supported_ports = 0;
            for (uint8_t port = 0; port < 32; port++) {
                if (hba->port_implemented & (1 << port)) {
                    no_supported_ports++;
                }
            }
            
            typedef struct port {
                uint8_t port_number;
                uint8_t status;
            } port_t;

            port_t *ports = malloc(no_supported_ports * sizeof(port_t));

            uint8_t supported_port_no = 0;
            for (uint8_t port = 0; port < 32; port++) {
                if (hba->port_implemented & (1 << port)) {
                    port_t current_port;
                    current_port.port_number = port;
                    current_port.status = hba->ports[port].sata_status & 0xF; // First 4 bits
                    ports[supported_port_no] = current_port;
                    supported_port_no++;
                }
            }
            
            for (uint8_t port = 0; port < no_supported_ports; port++) {
                port_t current_port = ports[port];
                printf("Port %d enabled\n", current_port.port_number);
                switch (current_port.status) {
                    case 0:
                        printf("No device detected at port\n");
                        break;
                    case 1:
                        printf("Device detected, but no Phy communication established at port\n");
                        break;
                    case 3:
                        printf("Device detected at port\n");
                        break;
                    case 4:
                        printf("Device detected, but Phy currently offline at port\n");
                        break;
                    
                    default:
                        printf("Unexpected status at port\n");
                        break;
                }
            }

            free(ports);

            printf("Enter anything for next: ");
            char c = getchar();
            printf("\n");
        }
    }
}