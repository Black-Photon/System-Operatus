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

int main(int argc, char **argv)
{
    rsdp_t *rsdp = get_rsdp_table();
    if (rsdp == NULL) {
        printf("Could not find RSDP table\n");
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

    xsdt_t *xsdt = (xsdt_t *) rsdp->xsdt_address;

    char xsdt_sig[5];
    memcpy(xsdt_sig, xsdt->signature, 4);
    xsdt_sig[4] = '\0';

    printf("XSDT Signature: %s\n", xsdt_sig);
    printf("XSDT Length: %d\n", xsdt->length);

    uint32_t no_entries = (xsdt->length-36)/8;
    uint64_t *entry_ptr = (uint64_t *) &xsdt->entry;
    for (int i = 0; i < no_entries; i++) {
        void *entry = (void *) entry_ptr[i];
        char_t entry_sig[5];
        memcpy(entry_sig, entry, 4);
        entry_sig[4] = '\0';
        printf("XSDT Entry %d: %s\n", i, entry_sig);
    }

    printf("Enter anything to continue: ");
    char c = getchar();
    return 0;
}
