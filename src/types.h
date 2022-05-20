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

struct mcfg {
    char_t signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char_t oemid[6];
    uint64_t oem_table_id;
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
    uint64_t reserved;
    char_t entry;
} __attribute__((packed));

struct mcfg_entry {
    uint64_t base_address;
    uint16_t pci_segment_group_number;
    uint8_t start_bus_no;
    uint8_t end_bus_no;
    uint32_t reserved;
} __attribute__((packed));

struct pci_header {
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t revision_id;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class_code;
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
};

struct pci_header_0 {
    uint16_t device_id;
    uint16_t vendor_id;
    uint16_t status;
    uint16_t command;
    uint8_t class_code;
    uint8_t subclass;
    uint8_t prog_if;
    uint8_t revision_id;
    uint8_t bist;
    uint8_t header_type;
    uint8_t latency_timer;
    uint8_t cache_line_size;
    uint32_t bar0;
    uint32_t bar1;
    uint32_t bar2;
    uint32_t bar3;
    uint32_t bar4;
    uint32_t bar5;
    uint32_t cardbus_cis_pointer;
    uint16_t subsystem_id;
    uint16_t subsystem_vendor_id;
    uint32_t expansion_rom_base_address;
    uint8_t reserved[3];
    uint8_t capabilities_pointer;
    uint16_t reserved_2;
    uint8_t max_latency;
    uint8_t min_grant;
    uint8_t interrupt_pin;
    uint8_t interrupt_line;
};

struct pci_header_1 {
    uint16_t device_id;
    uint16_t vendor_id;
    uint16_t status;
    uint16_t command;
    uint8_t class_code;
    uint8_t subclass;
    uint8_t prog_if;
    uint8_t revision_id;
    uint8_t bist;
    uint8_t header_type;
    uint8_t latency_timer;
    uint8_t cache_line_size;
    uint32_t bar0;
    uint32_t bar1;
    uint8_t secondary_latency_timer;
    uint8_t subordinate_bus_number;
    uint8_t secondary_bus_number;
    uint8_t primary_bus_number;
    uint16_t secondary_status;
    uint8_t io_limit;
    uint8_t io_base;
    uint16_t memory_limit;
    uint16_t memory_base;
    uint16_t prefetchable_memory_limit;
    uint16_t prefetchable_memory_base;
    uint32_t prefetchable_base_upper_32;
    uint32_t prefetchable_limit_upper_32;
    uint16_t io_limit_upper_16;
    uint16_t io_base_upper_16;
    uint8_t reserved[3];
    uint8_t capabilities_pointer;
    uint32_t expansion_rom_base_address;
    uint16_t bridge_control;
    uint8_t interrupt_pin;
    uint8_t interrupt_line;
};

struct pci_header_2 {
    uint16_t device_id;
    uint16_t vendor_id;
    uint16_t status;
    uint16_t command;
    uint8_t class_code;
    uint8_t subclass;
    uint8_t prog_if;
    uint8_t revision_id;
    uint8_t bist;
    uint8_t header_type;
    uint8_t latency_timer;
    uint8_t cache_line_size;
    uint32_t cardbus_socket;
    uint16_t secondary_status;
    uint8_t reserved;
    uint8_t offset_of_capabilities;
    uint8_t cardbus_latency_timer;
    uint8_t subordinate_bus_number;
    uint8_t cardbus_bus_number;
    uint8_t pci_bus_number;
    uint32_t memory_base_address_0;
    uint32_t memory_limit_0;
    uint32_t memory_base_address_1;
    uint32_t memory_limit_1;
    uint32_t io_base_address_0;
    uint32_t io_limit_0;
    uint32_t io_base_address_1;
    uint32_t io_limit_1;
    uint16_t bridge_control;
    uint8_t interrupt_pin;
    uint8_t interrupt_line;
    uint16_t subsystem_vendor_id;
    uint16_t subsystem_device_id;
    uint32_t pc_card_legacy_mode_base_address;
};



typedef struct rsdp rsdp_t;
typedef struct xsdt xsdt_t;
typedef struct mcfg mcfg_t;
typedef struct mcfg_entry mcfg_entry_t;
typedef struct pci_header pci_header_t;
typedef struct pci_header_0 pci_header_0_t;
typedef struct pci_header_1 pci_header_1_t;
typedef struct pci_header_2 pci_header_2_t;

#endif