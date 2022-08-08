#include <uefi/uefi.h>
#include <stdbool.h>

#include "defs.h"
#include "types.h"
#include "std.h"
#include "pci.h"

// Keep track of the initial locations of the PCI header space
static uint8_t *base_address;
static uint8_t start_bus_no;

// Keep a list of all devices
static size_t device_list_size = 0;
static pci_header_t **all_devices = NULL;

pci_device_list_t init_pci(mcfg_t *mcfg) {
    uint32_t no_entries = (mcfg->length-44)/16; // Fixed header is 44 bytes, entry length 16
    mcfg_entry_t *entry_ptr = (mcfg_entry_t *) &mcfg->entry;
    // Each entry to the MCFG contains a PCI root
    for (size_t i = 0; i < no_entries; i++) {
        mcfg_entry_t *entry = &entry_ptr[i];

        if (BOOT_VERBOSE) {
            printf("MCFG Entry %d:\n", i);
            printf("- Base address: %lx\n", entry->base_address);
            printf("- PCI segment group number: %d\n", entry->pci_segment_group_number);
            printf("- Start bus number: %d\n", entry->start_bus_no);
            printf("- End bus number: %d\n", entry->end_bus_no);
            printf("\n");
        }

        base_address = (uint8_t *) entry->base_address;
        start_bus_no = entry->start_bus_no;

        check_all_buses();
    }

    if (BOOT_VERBOSE & PCI_VERBOSE) {
        printf("Loading PCI entries:\n");
    }

    // Iterate over all the devices found
    for (size_t i = 0; i < device_list_size; i++) {
        pci_header_t *pci_header = all_devices[i];

        if (BOOT_VERBOSE & PCI_VERBOSE) {
            printf("\nDevice found:\n");
            printf("Device ID: %x\n", pci_header->device_id);
            printf("Vendor ID: %x\n", pci_header->vendor_id);
            printf("Status: %x\n", pci_header->status);
            printf("Command: %x\n", pci_header->command);
            printf("Class Code: %x\n", pci_header->class_code);
            printf("Subclass: %x\n", pci_header->subclass);
            printf("Prog IF: %x\n", pci_header->prog_if);
            printf("Revision ID: %x\n", pci_header->revision_id);
            printf("BIST: %x\n", pci_header->bist);
            printf("Header Type: %x\n", swap_byte(pci_header->header_type));
            printf("Latency Timer: %x\n", pci_header->latency_timer);
            printf("Cache Line Size: %x\n", pci_header->cache_line_size);

            // if (false)
            switch (swap_byte(pci_header->header_type)) {
            case 0:
                pci_header_0_t *header_0 = (pci_header_0_t *) pci_header;
                printf("Base Address #0: %x\n", header_0->bar0);
                printf("Base Address #1: %x\n", header_0->bar1);
                printf("Base Address #2: %x\n", header_0->bar2);
                printf("Base Address #3: %x\n", header_0->bar3);
                printf("Base Address #4: %x\n", header_0->bar4);
                printf("Base Address #5: %x\n", header_0->bar5);
                printf("Cardbus CIS Pointer: %x\n", header_0->cardbus_cis_pointer);
                printf("Subsystem ID: %x\n", header_0->subsystem_id);
                printf("Subsystem Vendor ID: %x\n", header_0->subsystem_vendor_id);
                printf("Expansion ROM base address: %x\n", header_0->expansion_rom_base_address);
                printf("Capabilities Pointer: %x\n", header_0->capabilities_pointer);
                printf("Max Latency: %x\n", header_0->max_latency);
                printf("Min Grant: %x\n", header_0->min_grant);
                printf("Interrupt PIN: %x\n", header_0->interrupt_pin);
                printf("Interrupt Line: %x\n", header_0->interrupt_line);
                break;
            case 1:
                pci_header_1_t *header_1 = (pci_header_1_t *) pci_header;
                printf("Base Address #0: %x\n", header_1->bar0);
                printf("Base Address #1: %x\n", header_1->bar1);
                printf("Secondary Latency Timer: %x\n", header_1->secondary_latency_timer);
                printf("Subordinate Bus Number: %x\n", header_1->subordinate_bus_number);
                printf("Secondary Bus Number: %x\n", header_1->secondary_bus_number);
                printf("Primary Bus Number: %x\n", header_1->primary_bus_number);
                printf("Secondary Status: %x\n", header_1->secondary_status);
                printf("I/O Limit: %x\n", header_1->io_limit);
                printf("I/O Base: %x\n", header_1->io_base);
                printf("Memory Limit: %x\n", header_1->memory_limit);
                printf("Memory Base: %x\n", header_1->memory_base);
                printf("Prefetchable Memory Limit: %x\n", header_1->prefetchable_memory_limit);
                printf("Prefetchable Memory Base: %x\n", header_1->prefetchable_memory_base);
                printf("Prefetchable Base Upper 32 Bits: %x\n",
                    header_1->prefetchable_base_upper_32);
                printf("Prefetchable Limit Upper 32 Bits: %x\n",
                    header_1->prefetchable_limit_upper_32);
                printf("I/O Limit Upper 16 Bits: %x\n", header_1->io_limit_upper_16);   
                printf("I/O Base Upper 16 Bits: %x\n", header_1->io_base_upper_16);   
                printf("Capabilities Pointer: %x\n", header_1->capabilities_pointer); 
                printf("Expansion ROM Base Address: %x\n", header_1->expansion_rom_base_address);
                printf("Bridge Control: %x\n", header_1->bridge_control);
                printf("Interrupt PIN: %x\n", header_1->interrupt_pin);
                printf("Interrupt Line: %x\n", header_1->interrupt_line); 
                break;
            case 2:
                pci_header_2_t *header_2 = (pci_header_2_t *) pci_header;
                printf("CardBus Socket/ExCa base address: %x\n", header_2->cardbus_socket);
                printf("Secondary status: %x\n", header_2->secondary_status);
                printf("Offset of capabilities list: %x\n", header_2->offset_of_capabilities);
                printf("CardBus latency timer: %x\n", header_2->cardbus_latency_timer);
                printf("Subordinate bus number: %x\n", header_2->subordinate_bus_number);
                printf("CardBus bus number: %x\n", header_2->cardbus_bus_number);
                printf("PCI bus number: %x\n", header_2->pci_bus_number);
                printf("Memory Base Address 0: %x\n", header_2->memory_base_address_0);
                printf("Memory Limit 0: %x\n", header_2->memory_limit_0);
                printf("Memory Base Address 1: %x\n", header_2->memory_base_address_1);
                printf("Memory Limit 1: %x\n", header_2->memory_limit_1);
                printf("I/O Base Address 0: %x\n", header_2->io_base_address_0);
                printf("I/O Limit 0: %x\n", header_2->io_limit_0);
                printf("I/O Base Address 1: %x\n", header_2->io_base_address_1);
                printf("I/O Limit 1: %x\n", header_2->io_limit_1);
                printf("Bridge Control: %x\n", header_2->bridge_control);
                printf("Interrupt PIN: %x\n", header_2->interrupt_pin);
                printf("Interrupt Line: %x\n", header_2->interrupt_line); 
                printf("Subsystem Vendor ID: %x\n", header_2->subsystem_vendor_id); 
                printf("Subsystem Device ID: %x\n", header_2->subsystem_device_id); 
                printf("16-bit PC Card legacy mode base address: %x\n",
                    header_2->pc_card_legacy_mode_base_address); 
                break;
            default:
                char out[100];
                int err = sprintf(out, "Unknown header %d\n", pci_header->header_type);
                if (err > 0) {
                    out[99] = '\n';
                    handle_error(out);
                } else {
                    handle_error("Failed to write error\n");
                }
            }

            printf("Enter anything for next: ");
            char c = getchar();
            printf("\n");
        }
    }

    if (BOOT_VERBOSE & PCI_VERBOSE) {
        printf("PCI loading complete\n\n");
    }

    pci_device_list_t device_list = {all_devices, device_list_size};

    return device_list;
}

bool enable_msi(pci_header_0_t *header) {
    // TODO handle MSI-X
    if (!(header->status & 0x10)) {
        handle_error("Capability reading for this device is disabled\n");
        return false;
    }
    void *capabilities_pointer = header->capabilities_pointer & 0xFC; // All but last 2 bits
    pci_msi_capabilities_t *capabilities = (uint32_t) header + (uint32_t) capabilities_pointer;
    uint8_t first_id = capabilities->id;
    bool msi_found = false;

    if (first_id == PCI_CAP_MSI) {
        msi_found = true;
    } else {
        capabilities_pointer = capabilities->next & 0xFC;
        capabilities = (uint32_t) header + (uint32_t) capabilities_pointer;

        while (capabilities->id != first_id) {
            if (capabilities->id == PCI_CAP_MSI) {
                msi_found = true;
                break;
            }

            capabilities_pointer = capabilities->next & 0xFC;
            capabilities = (uint32_t) header + (uint32_t) capabilities_pointer;
        }
    }

    if (!msi_found) {
        handle_error("First capability entry is not MSI\n");
        return false;
    }


}

static pci_header_t *get_pci_header_at(uint8_t bus, uint8_t device, uint8_t function) {
    // Each bus contains 32 devices of up to 8 functions
    return (pci_header_t *)
        (base_address + ((bus - start_bus_no) << 20 | device << 15 | function << 12));
}

static void check_device(uint8_t bus, uint8_t device) {
    pci_header_t *header = get_pci_header_at(bus, device, 0);

    uint16_t vendor_id = header->vendor_id;
    if (vendor_id == 0xFFFF) return; // Device doesn't exist

    // Add the device to the list
    void *new_pointer = realloc(all_devices, (device_list_size+1) * sizeof(pci_header_t *));
    if (new_pointer == NULL) {
        handle_error("Could not allocate array\n");
        exit(EXIT_FAILURE);
    }
    all_devices = new_pointer;
    all_devices[device_list_size] = header;
    device_list_size++;
    
    // If PCI-to-PCI, secondary bus will be another valid PCI bus
    if ((header->class_code == 0x6)
     && (header->subclass == 0x4)
     && (swap_byte(header->header_type) == 1)) {
        pci_header_1_t *header_1 = (pci_header_1_t *) header;
        uint8_t secondary_bus = header_1->secondary_bus_number;
        check_bus(secondary_bus);
    }

    // Check all functions for PCI-to-CardBus bridges
    if (swap_byte(header->header_type) == 2) {
        for (uint8_t function = 1; function < 8; function++) {
            header = get_pci_header_at(0, 0, function);
            if (header->vendor_id == 0xFFFF) break;
            check_bus(function);
        }
    }
}

static void check_bus(uint8_t bus) {
    for (uint8_t device = 0; device < 32; device++) {
        check_device(bus, device);
    }
}

static void check_all_buses() {
    pci_header_t *root_header = get_pci_header_at(0, 0, 0);
    if (swap_byte(root_header->header_type) != 2) {
        check_bus(0);
    } else {
        // If root header has multiple functions, check those too
        for (uint8_t function = 0; function < 8; function++) {
            root_header = get_pci_header_at(0, 0, function);
            if (root_header->vendor_id == 0xFFFF) break; // Device doesn't exist
            check_bus(function);
        }
    }
}