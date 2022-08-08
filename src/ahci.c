#include "ahci.h"
#include "std.h"
#include "defs.h"
#include "pci.h"

bool init_ahci(pci_device_list_t device_list) {
    if (BOOT_VERBOSE) {
        printf("Starting initialisation for AHCI\n");
    }

    size_t device_list_size = device_list.device_list_size;
    pci_header_t **all_devices = device_list.all_devices;

    pci_header_0_t *ahci_entry = NULL;

    // Iterate over all PCI devices to find ACPI
    for (size_t i = 0; i < device_list_size; i++) {
        pci_header_t *pci_header = all_devices[i];

        if (is_ahci(pci_header)) {
            ahci_entry = pci_header;
            break;
        }
    }

    if (ahci_entry == NULL) {
        handle_error("Could not find AHCI entry in PCI device list.\n");
        return false;
    }

    enable_msi(ahci_entry);
    
    hba_t *hba = ahci_entry->bar5; // Get the HBA table from ABAR (BAR5)
    hba_port_t *port;

    bool success = find_open_port(hba, &port);
    if (!success) {
        return false;
    }

    if (BOOT_VERBOSE) {
        uint8_t type = check_type(port);
        switch (type) {
            case AHCI_DEV_SATA:
                printf("Chosen device is a SATA device\n");
                break;
            case AHCI_DEV_SATAPI:
                printf("Chosen device is a SATAPI device\n");
                break;
            case AHCI_DEV_SEMB:
                printf("Chosen device is a SEMB device\n");
                break;
            case AHCI_DEV_PM:
                printf("Chosen device is a PM device\n");
                break;
            
            default:
                printf("Unknown device type\n");
                break;
        }

        printf("Enter anything for next: ");
        char c = getchar();
        printf("\n");
    }

    return true;
}

static bool is_ahci(pci_header_t *pci_header) {
    return
        swap_byte(pci_header->header_type) == 0x0 &&
        pci_header->class_code == 0x01 &&
        pci_header->subclass == 0x06;
}

static bool find_open_port(hba_t *hba, hba_port_t** port) {
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
        uint8_t power;
    } port_t;

    port_t *ports = malloc(no_supported_ports * sizeof(port_t));

    uint8_t supported_port_no = 0;
    for (uint8_t port = 0; port < 32; port++) {
        if (hba->port_implemented & (1 << port)) {
            port_t current_port;
            current_port.port_number = port;
            current_port.status = hba->ports[port].sata_status & 0xF; // 0x0-0x3
            current_port.power = (hba->ports[port].sata_status >> 8) & 0xF; // 0x8-0x11
            ports[supported_port_no] = current_port;
            supported_port_no++;
        }
    }
    
    if (BOOT_VERBOSE) {
        printf("\n");
        for (uint8_t port = 0; port < no_supported_ports; port++) {
            port_t current_port = ports[port];
            printf("Port %d enabled\n", current_port.port_number);
            switch (current_port.status) {
                case HBA_PORT_DET_NOT_PRESENT:
                    printf("No device detected at port\n");
                    break;
                case HBA_PORT_DET_NOT_ESTABLISHED:
                    printf("Device detected, but no Phy communication established at port\n");
                    break;
                case HBA_PORT_DET_PRESENT:
                    printf("Device detected at port\n");
                    break;
                case HBA_PORT_DET_OFFLINE:
                    printf("Device detected, but Phy currently offline at port\n");
                    break;
                
                default:
                    printf("Unexpected status at port\n");
                    break;
            }
            switch (current_port.power) {
                case HBA_PORT_IPM_NOT_PRESENT:
                    printf("No power settings detected at port\n");
                    break;
                case HBA_PORT_IPM_ACTIVE:
                    printf("Device is active\n");
                    break;
                case HBA_PORT_IPM_PARTIAL_POWER:
                    printf("Device is on partial power mode\n");
                    break;
                case HBA_PORT_IPM_SLUMBER:
                    printf("Device is in slumber mode\n");
                    break;
                case HBA_PORT_IPM_DEVSLEEP:
                    printf("Device is in devsleep mode\n");
                    break;
                
                default:
                    printf("Unexpected power settings at port\n");
                    break;
            }
            printf("\n");
        }
    }

    bool is_open_port = false;
    hba_port_t *connected_port;
    for (uint8_t port = 0; port < no_supported_ports; port++) {
        port_t current_port = ports[port];
        if (current_port.status == HBA_PORT_DET_PRESENT &&
            current_port.power == HBA_PORT_IPM_ACTIVE) {
            is_open_port = true;
            connected_port = &hba->ports[current_port.port_number];
        }
    }

    free(ports);

    if (!is_open_port) {
        handle_error("Could not find any SATA port with an active connection.\n");
        return false;
    }

    *port = connected_port;
    return true;
}

static uint8_t check_type(hba_port_t *port) {
	switch (port->signature) {
        case SATA_SIG_ATAPI:
            return AHCI_DEV_SATAPI;
        case SATA_SIG_SEMB:
            return AHCI_DEV_SEMB;
        case SATA_SIG_PM:
            return AHCI_DEV_PM;
        default:
            return AHCI_DEV_SATA;
	}
}