#ifndef _TYPES_H
#define _TYPES_H

struct rsdp {
    /**
     * @brief “RSD PTR ” (Notice that this signature must contain a
     * trailing blank character.)
     */
    char_t Signature[8];
    /**
     * @brief This is the checksum of the fields defined in the ACPI 1.0
     * specification. This includes only the first 20 bytes of this
     * table, bytes 0 to 19, including the checksum field. These bytes
     * must sum to zero.
     */
    int8_t Checksum;
    /**
     * @brief An OEM-supplied string that identifies the OEM.
     */
    char_t OEMID[6];
    /**
     * @brief The revision of this structure. Larger revision numbers are
     * back-ward compatible to lower revision numbers. The ACPI version
     * 1.0 revision number of this table is zero. The ACPI version 1.0
     * RSDP Structure only includes the first 20 bytes of this table,
     * bytes 0 to 19. It does not include the Length field and beyond.
     * The current value for this field is 2.
     */
    uint8_t Revision;
    /**
     * @brief 32 bit physical address of the RSDT.
     */
    uint32_t RsdtAddress;

    uint32_t Length;
    uint64_t XsdtAddress;
    uint8_t ExtendedChecksum;
    uint8_t Reserved[3];
};

typedef struct rsdp rsdp_t;


#endif