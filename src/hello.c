#include <efi.h>
#include <efilib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

EFI_STATUS print(CHAR16 *string) {
    return ST->ConOut->OutputString(ST->ConOut, string);
}

bool are_guids_eq(EFI_GUID guid1, EFI_GUID guid2) {
    size_t size = 16;
    char *array1 = (char *) &guid1;
    char *array2 = (char *) &guid2;

    for (size_t i = 0; i < size; i++) {
        if (array1[i] != array2[i]) return false;
    }
    return true;
}
 
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS Status;
    EFI_INPUT_KEY Key;
 
    /* Store the system table for future use in other functions */
    ST = SystemTable;

    for (int I = 0; I < ST->NumberOfTableEntries; I++) {
        EFI_CONFIGURATION_TABLE Table = ST->ConfigurationTable[I];
        EFI_GUID ACPI_2_Table = ACPI_20_TABLE_GUID;

        if(are_guids_eq(Table.VendorGuid, ACPI_2_Table)) {
            #define SignatureSize 8
            char * Signature8 = (char *) Table.VendorTable;
            CHAR16 Signature16[SignatureSize+3];
            for (size_t J = 0; J < SignatureSize; J++) {
                Signature16[J] = Signature8[J];
            }
            Signature16[SignatureSize] = '\r';
            Signature16[SignatureSize+1] = '\n';
            Signature16[SignatureSize+2] = '\0';

            // Status = print(L"Success!\r\n");
            Status = print(Signature16);
            if (EFI_ERROR(Status)) return Status;
        }
    }
 
    /* Say hi */
    Status = print(L"Hello World\r\n"); // EFI Applications use Unicode and CRLF, a la Windows
    if (EFI_ERROR(Status)) return Status;
 
    /* Now wait for a keystroke before continuing, otherwise your
       message will flash off the screen before you see it.
 
       First, we need to empty the console input buffer to flush
       out any keystrokes entered before this point */
    Status = ST->ConIn->Reset(ST->ConIn, FALSE);
    if (EFI_ERROR(Status))
        return Status;
 
    /* Now wait until a key becomes available.  This is a simple
       polling implementation.  You could try and use the WaitForKey
       event instead if you like */
    while ((Status = ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) == EFI_NOT_READY) ;
 
    return Status;
}