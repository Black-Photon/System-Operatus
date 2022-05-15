#include <uefi/uefi.h>
#include <stdbool.h>

#include "types.h"

bool are_guids_eq(efi_guid_t guid1, efi_guid_t guid2) {
    size_t size = 16;
    char *array1 = (char *) &guid1;
    char *array2 = (char *) &guid2;

    for (size_t i = 0; i < size; i++) {
        if (array1[i] != array2[i]) return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    for (size_t i = 0; i < ST->NumberOfTableEntries; i++) {
        efi_configuration_table_t table = ST->ConfigurationTable[i];
        efi_guid_t acpi_2_table = ACPI_20_TABLE_GUID;

        if(are_guids_eq(table.VendorGuid, acpi_2_table)) {
            rsdp_t *rsdp = (rsdp_t *) table.VendorTable;
            printf("%s\n", rsdp->Signature);
        }
    }

    printf("Hello World!\n");
    printf("Enter anything to continue: ");
    char c = getchar();
    return 0;
}
