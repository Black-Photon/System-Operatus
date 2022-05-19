#include <uefi/uefi.h>
#include <stdbool.h>

#include "types.h"

bool are_guids_eq(efi_guid_t guid1, efi_guid_t guid2) {
    size_t size = 16;
    char_t *array1 = (char_t *) &guid1;
    char_t *array2 = (char_t *) &guid2;

    for (size_t i = 0; i < size; i++) {
        if (array1[i] != array2[i]) return false;
    }
    return true;
}

bool verify_checksum(char_t data[], size_t length) {
    char_t checksum;
    for (size_t i = 0; i < length; i++) {
        checksum += data[i];
    }
    return checksum == 0;
}

rsdp_t *get_rsdp_table() {
    for (size_t i = 0; i < ST->NumberOfTableEntries; i++) {
        efi_configuration_table_t table = ST->ConfigurationTable[i];
        efi_guid_t acpi_2_table = ACPI_20_TABLE_GUID;

        if(are_guids_eq(table.VendorGuid, acpi_2_table)) {
            return (rsdp_t *) table.VendorTable;
        }
    }
    return NULL;
}

mcfg_t *get_mcfg_table(xsdt_t *xsdt) {
    uint32_t no_entries = (xsdt->length-36)/8;
    uint64_t *entry_ptr = (uint64_t *) &xsdt->entry;
    for (int i = 0; i < no_entries; i++) {
        void *entry = (void *) entry_ptr[i];

        char_t entry_sig[4];
        memcpy(entry_sig, entry, 4);

        if(strncmp(entry_sig, "MCFG", 4) == 0) {
            return (mcfg_t *) entry;
        }
    }
    return NULL;
}

void handle_error(char *string) {
    printf(string);
    printf("Enter anything to continue: ");
    char c = getchar();
}

void *get_physical_address(uint8_t bus, uint8_t device, uint8_t function, uint8_t *starting_address, uint8_t starting_bus) {
    return (void *) (starting_address + ((bus - starting_bus) << 20 | device << 15 | function << 12));
}

uint16_t get_vendor_id(uint16_t *physical_address) {   
    return physical_address[1];
}

uint8_t get_header_type(uint8_t *physical_address) {
    return physical_address[13];
}

uint8_t get_base_class(uint8_t *physical_address) {

}

uint8_t get_sub_class(uint8_t *physical_address) {

}

uint8_t get_secondary_bus(uint8_t *physical_address) {

}

// void check_function(uint8_t bus, uint8_t device, uint8_t function) {
//     uint8_t base_class;
//     uint8_t sub_class;
//     uint8_t secondary_bus;

//     base_class = get_base_class(bus, device, function);
//     sub_class = get_sub_class(bus, device, function);
//     if ((base_class == 0x6) && (sub_class == 0x4)) {
//         secondary_bus = get_secondary_bus(bus, device, function);
//         check_bus(secondary_bus);
//     }
// }

void check_device(uint8_t bus, uint8_t device, uint8_t *starting_address, uint8_t starting_bus) {
    uint8_t function = 0;
 
    void *physical_address = get_physical_address(bus, device, function, starting_address, starting_bus);

    uint16_t vendor_id = get_vendor_id((uint16_t *) physical_address);
    if (vendor_id == 0xFFFF) return; // Device doesn't exist
    printf("ID = %x\n", vendor_id);
    // check_function(bus, device, function);
    // uint8_t header_type = get_header_type((uint8_t *) physical_address);
    // if ((header_type & 0x80) != 0) {
    //     // It's a multi-function device, so check remaining functions
    //     for (function = 1; function < 8; function++) {
    //         physical_address = get_physical_address(bus, device, function, starting_address, starting_address);
    //         if (get_vendor_id(physical_address) != 0xFFFF) {
    //             check_function(bus, device, function);
    //         }
    //     }
    // }
}

void check_all_buses(uint8_t *starting_address, uint8_t starting_bus) {
    uint16_t bus;
    uint8_t device;

    for (bus = 0; bus < 256; bus++) {
        for (device = 0; device < 32; device++) {
            check_device(bus, device, starting_address, starting_bus);
        }
    }
}

int main(int argc, char **argv)
{
    rsdp_t *rsdp = get_rsdp_table();
    if (rsdp == NULL) {
        handle_error("Could not find RSDP table v2\n");
        return 1;
    }

    char rsdp_signature[9];
    memcpy(rsdp_signature, rsdp->signature, 8);
    rsdp_signature[8] = '\0';
    char rsdp_oemid[7];
    memcpy(rsdp_oemid, rsdp->oemid, 6);
    rsdp_oemid[6] = '\0';

    printf("RSDP Signature: %s\n", rsdp_signature);
    printf("RSDP Checksum Verifies: %s\n", verify_checksum((char *) rsdp, 20) ? "true" : "false");
    printf("RSDP OEMID: %s\n", rsdp_oemid);
    printf("RSDP Length: %d\n", rsdp->length);
    printf("RSDP Extended Checksum Verifies: %s\n", verify_checksum((char *) rsdp, 36) ? "true" : "false");
    
    if (verify_checksum((char *) rsdp, 20) == false) {
        handle_error("Invalid RSDP table\n");
        return 1;
    }
    
    if (rsdp->revision == 1) {
        handle_error("RSDP table loaded is v1, when v2 is required\n");
        return 1;
    } else { // Assume future revisions have same structure up to 36 bytes
        if (verify_checksum((char *) rsdp, 36) == false) {
            handle_error("Invalid RSDP table\n");
            return 1;
        }
    }

    xsdt_t *xsdt = (xsdt_t *) rsdp->xsdt_address;

    char xsdt_sig[5];
    memcpy(xsdt_sig, xsdt->signature, 4);
    xsdt_sig[4] = '\0';

    printf("XSDT Signature: %s\n", xsdt_sig);
    printf("XSDT Length: %d\n", xsdt->length);

    
    mcfg_t *mcfg = get_mcfg_table(xsdt);

    if (mcfg == NULL) {
        handle_error("Could not find MCFG table\nCheck your device has PCIe support enabled\n");
        return 1;
    }
    
    char_t entry_sig[5];
    memcpy(entry_sig, mcfg->signature, 4);
    entry_sig[4] = '\0';
    
    printf("MCFG Signature: %s\n", entry_sig);
    printf("MCFG Length: %d\n", mcfg->length);
    printf("MCFG Checksum Verifies: %s\n", verify_checksum((char *) mcfg, mcfg->length) ? "true" : "false");

    if (verify_checksum((char *) mcfg, mcfg->length) == false) {
        handle_error("Invalid MCFG table\n");
        return 1;
    }


    uint32_t no_entries = (mcfg->length-44)/16;
    mcfg_entry_t *entry_ptr = (mcfg_entry_t *) &mcfg->entry;
    for (size_t i = 0; i < no_entries; i++) {
        mcfg_entry_t *entry = &entry_ptr[i];

        printf("MCFG Entry %d:\n", i);
        printf("- Base address: %lx\n", entry->base_address);
        printf("- PCI segment group number: %d\n", entry->pci_segment_group_number);
        printf("- Start bus number: %d\n", entry->start_bus_no);
        printf("- End bus number: %d\n", entry->end_bus_no);

        // printf("Contents = %d\n", *((uint32_t *) entry->base_address));
        check_all_buses((uint8_t *) entry->base_address, entry->start_bus_no);
    }


    printf("Enter anything to continue: ");
    char c = getchar();
    return 0;
}
