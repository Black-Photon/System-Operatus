#ifndef _TYPES_H
#define _TYPES_H

struct rsdp {
    /**
     * @brief “RSD PTR ” (Notice that this signature must contain a
     * trailing blank character.)
     */
    char_t signature[8];
    /**
     * @brief This is the checksum of the fields defined in the ACPI 1.0
     * specification. This includes only the first 20 bytes of this
     * table, bytes 0 to 19, including the checksum field. These bytes
     * must sum to zero.
     */
    uint8_t checksum;
    /**
     * @brief An OEM-supplied string that identifies the OEM.
     */
    char_t oemid[6];
    /**
     * @brief The revision of this structure. Larger revision numbers are
     * back-ward compatible to lower revision numbers. The ACPI version
     * 1.0 revision number of this table is zero. The ACPI version 1.0
     * RSDP Structure only includes the first 20 bytes of this table,
     * bytes 0 to 19. It does not include the Length field and beyond.
     * The current value for this field is 2.
     */
    uint8_t revision;
    /**
     * @brief 32 bit physical address of the RSDT.
     */
    uint32_t rsdt_address;

    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __attribute__((packed));

struct xsdt {
    char_t signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char_t oemid[6];
    uint64_t oem_table_id;
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
    char_t entry;
} __attribute__((packed));

typedef struct rsdp rsdp_t;
typedef struct xsdt xsdt_t;


#endif