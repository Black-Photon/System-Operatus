#ifndef _TYPES_H
#define _TYPES_H

#include <uefi/uefi.h>

volatile struct rsdp {
    /**
     * @brief “RSD PTR ” (Notice that this signature must contain a trailing blank character.)
     */
    char_t signature[8];
    /**
     * @brief This is the checksum of the fields defined in the ACPI 1.0 specification. This
     * includes only the first 20 bytes of this table, bytes 0 to 19, including the checksum field.
     * These bytes must sum to zero.
     */
    uint8_t checksum;
    /**
     * @brief An OEM-supplied string that identifies the OEM.
     */
    char_t oemid[6];
    /**
     * @brief The revision of this structure. Larger revision numbers are back-ward compatible to
     * lower revision numbers. The ACPI version 1.0 revision number of this table is zero. The ACPI
     * version 1.0 RSDP Structure only includes the first 20 bytes of this table, bytes 0 to 19. It
     * does not include the Length field and beyond. The current value for this field is 2.
     */
    uint8_t revision;
    /**
     * @brief 32 bit physical address of the RSDT.
     */
    uint32_t rsdt_address;

    // Fields from here are only included in revision 2

    /**
     * @brief The length of the table, in bytes, including the header, starting from offset 0. This
     * field is used to record the size of the entire table. This field is not available in the
     * ACPI version 1.0 RSDP Structure.
     * 
     */
    uint32_t length;
    /**
     * @brief 64 bit physical address of the XSDT.
     */
    uint64_t xsdt_address;
    /**
     * @brief This is a checksum of the entire table, including both checksum fields.
     */
    uint8_t extended_checksum;
    /**
     * @brief Reserved field
     */
    uint8_t reserved[3];
} __attribute__((packed));
typedef struct rsdp rsdp_t;

volatile struct xsdt {
    /**
     * @brief ‘XSDT’. Signature for the Extended System Description Table.
     */
    char_t signature[4];
    /**
     * @brief Length, in bytes, of the entire table. The length implies the number of Entry fields
     * (n) at the end of the table.
     */
    uint32_t length;
    /**
     * @brief 1
     */
    uint8_t revision;
    /**
     * @brief Entire table must sum to zero.
     */
    uint8_t checksum;
    /**
     * @brief OEM ID
     */
    char_t oemid[6];
    /**
     * @brief For the XSDT, the table ID is the manufacture model ID. This field must match the
     * OEM Table ID in the FADT.
     */
    uint64_t oem_table_id;
    /**
     * @brief OEM revision of XSDT table for supplied OEM Table ID.
     */
    uint32_t oem_revision;
    /**
     * @brief Vendor ID of utility that created the table. For tables containing Definition Blocks,
     * this is the ID for the ASL Compiler.
     */
    uint32_t creator_id;
    /**
     * @brief Revision of utility that created the table. For tables containing Definition Blocks,
     * this is the revision for the ASL Compiler.
     */
    uint32_t creator_revision;
    /**
     * @brief An array of 64-bit physical addresses that point to other DESCRIPTION_HEADERs. OSPM
     * assumes at least the DESCRIPTION_HEADER is addressable, and then can further address the
     * table based upon its Length field.
     */
    char_t entry;
} __attribute__((packed));
typedef struct xsdt xsdt_t;

volatile struct mcfg {
    /**
     * @brief Table Signature ("MCFG") 
     */
    char_t signature[4];
    /**
     * @brief Length of table (in bytes) 
     */
    uint32_t length;
    /**
     * @brief Revision (1) 
     */
    uint8_t revision;
    /**
     * @brief Checksum (sum of all bytes in table & 0xFF = 0) 
     */
    uint8_t checksum;
    /**
     * @brief OEM ID (same meaning as other ACPI tables) 
     */
    char_t oemid[6];
    /**
     * @brief OEM table ID (manufacturer model ID) 
     */
    uint64_t oem_table_id;
    /**
     * @brief OEM Revision (same meaning as other ACPI tables) 
     */
    uint32_t oem_revision;
    /**
     * @brief Creator ID (same meaning as other ACPI tables) 
     */
    uint32_t creator_id;
    /**
     * @brief Creator Revision (same meaning as other ACPI tables) 
     */
    uint32_t creator_revision;
    /**
     * @brief Reserved
     */
    uint64_t reserved;
    /**
     * @brief Configuration space base address allocation structures. Each structure uses the
     * mcfg_entry format
     */
    char_t entry;
} __attribute__((packed));
typedef struct mcfg mcfg_t;

volatile struct mcfg_entry {
    /**
     * @brief Base address of enhanced configuration mechanism 
     */
    uint64_t base_address;
    /**
     * @brief PCI Segment Group Number
     */
    uint16_t pci_segment_group_number;
    /**
     * @brief Start PCI bus number decoded by this host bridge 
     */
    uint8_t start_bus_no;
    /**
     * @brief End PCI bus number decoded by this host bridge 
     */
    uint8_t end_bus_no;
    /**
     * @brief Reserved
     */
    uint32_t reserved;
} __attribute__((packed));
typedef struct mcfg_entry mcfg_entry_t;

/**
 * More detail in https://wiki.osdev.org/Pci
 */
typedef volatile struct pci_header {
    /**
     * @brief The ID of the vendor for the device
     */
    uint16_t vendor_id;
    /**
     * @brief The ID of the device
     */
    uint16_t device_id;
    /**
     * @brief Represents available commands
     * 
     * Bit 0     - I/O Space                          - If set, can respond to I/O space access
     * Bit 1     - Memory Space                       - If set, can respond to memory space access
     * Bit 2     - Bus Master                         - If set, can act as a PCI bus holder
     * Bit 3     - Special Cycles                     - If set, can monitor special cycles
     * Bit 4     - Memory Write and Invalidate Enable - If set, can use the 'Memory Write AND
     *                                                  Invalidate' command (as well as'Memory
     *                                                  Write')
     * Bit 5     - VGA Palette Snoop                  - If set, doesn't respond to VGA palette
     *                                                  writes and instead snoops
     * Bit 6     - Parity Error Response              - If set, will assert the PERR# (Parity Error)
     *                                                  pin on a parity error (instead of just
     *                                                  setting the status bit)
     * Bit 7     - Reserved (always 0)
     * Bit 8     - SERR# Enable                       - If set, SERR# driver is enable
     * Bit 9     - Fast Back-Back Enable              - If set, can generate fast back-to-back
     *                                                  transactions to other agent
     * Bit 10    - Interrupt Disable                  - If set, the INTx# signal (interrupt) is
     *                                                  disabled
     * Bit 11-16 - Reserved 
     */
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
} pci_header_t;

typedef volatile struct pci_header_0 {
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
    uint32_t bar0;
    uint32_t bar1;
    uint32_t bar2;
    uint32_t bar3;
    uint32_t bar4;
    uint32_t bar5;
    uint32_t cardbus_cis_pointer;
    uint16_t subsystem_vendor_id;
    uint16_t subsystem_id;
    uint32_t expansion_rom_base_address;
    uint8_t capabilities_pointer;
    uint8_t reserved[7];
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t min_grant;
    uint8_t max_latency;
} pci_header_0_t;

typedef volatile struct pci_header_1 {
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
    uint32_t bar0;
    uint32_t bar1;
    uint8_t primary_bus_number;
    uint8_t secondary_bus_number;
    uint8_t subordinate_bus_number;
    uint8_t secondary_latency_timer;
    uint8_t io_base;
    uint8_t io_limit;
    uint16_t secondary_status;
    uint16_t memory_base;
    uint16_t memory_limit;
    uint16_t prefetchable_memory_base;
    uint16_t prefetchable_memory_limit;
    uint32_t prefetchable_base_upper_32;
    uint32_t prefetchable_limit_upper_32;
    uint16_t io_base_upper_16;
    uint16_t io_limit_upper_16;
    uint8_t capabilities_pointer;
    uint8_t reserved[3];
    uint32_t expansion_rom_base_address;
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint16_t bridge_control;
} pci_header_1_t;

typedef volatile struct pci_header_2 {
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
    uint32_t cardbus_socket;
    uint8_t offset_of_capabilities;
    uint8_t reserved;
    uint16_t secondary_status;
    uint8_t pci_bus_number;
    uint8_t cardbus_bus_number;
    uint8_t subordinate_bus_number;
    uint8_t cardbus_latency_timer;
    uint32_t memory_base_address_0;
    uint32_t memory_limit_0;
    uint32_t memory_base_address_1;
    uint32_t memory_limit_1;
    uint32_t io_base_address_0;
    uint32_t io_limit_0;
    uint32_t io_base_address_1;
    uint32_t io_limit_1;
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint16_t bridge_control;
    uint16_t subsystem_device_id;
    uint16_t subsystem_vendor_id;
    uint32_t pc_card_legacy_mode_base_address;
} pci_header_2_t;

typedef volatile struct pci_device_list {
    struct pci_header **all_devices;
    size_t device_list_size;
} pci_device_list_t;

typedef volatile struct hba_port {
    uint32_t command_list_base;		    // Command list base address, 1K-byte aligned
	uint32_t command_list_upper;		// Command list base address upper 32 bits
	uint32_t fis_base;		            // FIS base address, 256-byte aligned
	uint32_t fis_upper;		            // FIS base address upper 32 bits
	uint32_t interrupt_status;		    // Interrupt status
	uint32_t interrupt_enable;		    // Interrupt enable
	uint32_t command_and_status;		// Command and status
	uint32_t reserved;		            // Reserved
	uint32_t task_file_data;		    // Task file data
	uint32_t signature;		            // Signature
	uint32_t sata_status;		        // SATA status (SCR0:SStatus)
	uint32_t sata_control;		        // SATA control (SCR2:SControl)
	uint32_t sata_error;		        // SATA error (SCR1:SError)
	uint32_t sata_active;		        // SATA active (SCR3:SActive)
	uint32_t command_issue;		        // Command issue
	uint32_t sata_notification;		    // SATA notification (SCR4:SNotification)
	uint32_t fis_based_switch_control;  // FIS-based switch control
	uint32_t reserved1[11];	            // Reserved
	uint32_t vendor[4];	                // Vendor specific
} hba_port_t;

typedef volatile struct hba {
	uint32_t capabilities;	        	// Host capability
	uint32_t global_host_control;		// Global host control
	uint32_t interrupt_status;		    // Interrupt status
	uint32_t port_implemented;		    // Port implemented
	uint32_t version;		            // Version
	uint32_t ccc_control;	            // Command completion coalescing control
	uint32_t ccc_ports;	                // Command completion coalescing ports
	uint32_t em_location;	            // Enclosure management location
	uint32_t em_control;	            // Enclosure management control
	uint32_t capabilities_extended;		// Host capabilities extended
	uint32_t bios_os_handoff;		    // BIOS/OS handoff control and status
	uint8_t  reserved[0xA0-0x2C];       // Reserved
	uint8_t  vendor[0x100-0xA0];	    // Vendor specific registers
	struct hba_port ports[32];	        // Port control registers
} hba_t;

#endif
