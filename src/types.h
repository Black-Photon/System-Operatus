#ifndef _TYPES_H
#define _TYPES_H

struct RSDP {
    /**
     * @brief “RSD PTR ” (Notice that this signature must contain a
     * trailing blank character.)
     */
    CHAR8 Signature[8];
    /**
     * @brief This is the checksum of the fields defined in the ACPI 1.0
     * specification. This includes only the first 20 bytes of this
     * table, bytes 0 to 19, including the checksum field. These bytes
     * must sum to zero.
     */
    UINT8 Checksum;
    /**
     * @brief An OEM-supplied string that identifies the OEM.
     */
    CHAR8 OEMID[6];
    /**
     * @brief The revision of this structure. Larger revision numbers are
     * back-ward compatible to lower revision numbers. The ACPI version
     * 1.0 revision number of this table is zero. The ACPI version 1.0
     * RSDP Structure only includes the first 20 bytes of this table,
     * bytes 0 to 19. It does not include the Length field and beyond.
     * The current value for this field is 2.
     */
    UINT8 Revision;
    /**
     * @brief 32 bit physical address of the RSDT.
     */
    UINT32 RsdtAddress;

    UINT32 Length;
    UINT64 XsdtAddress;
    UINT8 ExtendedChecksum;
    UINT8 Reserved[3];
};

#endif