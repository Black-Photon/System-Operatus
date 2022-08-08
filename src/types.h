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
    // See https://wiki.osdev.org/Pci for more parameter details
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
     * Bit 0     - I/O Space
     * Bit 1     - Memory Space
     * Bit 2     - Bus Master
     * Bit 3     - Special Cycles
     * Bit 4     - Memory Write and Invalidate Enable
     * Bit 5     - VGA Palette Snoop
     * Bit 6     - Parity Error Response
     * Bit 7     - Reserved (always 0)
     * Bit 8     - SERR# Enable
     * Bit 9     - Fast Back-Back Enable
     * Bit 10    - Interrupt Disable
     * Bit 11-15 - Reserved 
     */
    uint16_t command;
    /**
     * @brief Represents the status of the device
     * 
     * Bit 0-2   - Reserved
     * Bit 3     - Interrupt Status
     * Bit 4     - Capabilities List
     * Bit 5     - 66 MHz Capable 
     * Bit 6     - Reserved
     * Bit 7     - Fast Back-to-Back Capable
     * Bit 8     - Master Data Parity Error 
     * Bit 9-10  - DEVSEL Timing
     * Bit 11    - Signalled Target Abort
     * Bit 12    - Received Target Abort
     * Bit 13    - Received Master Abort 
     * Bit 14    - Signaled System Error
     * Bit 15    - Detected Parity Error
     */
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

typedef struct pci_msi_capabilities {
    uint8_t id;
    uint8_t next;
    uint16_t message_control;
    uint32_t message_address;
    uint32_t message_address_upper;
    uint16_t message_data;
    uint16_t reserved;
    uint32_t mask;
    uint32_t pending;
} pci_msi_capabilities_t;

typedef volatile struct hba_port {
	// 0x00
    uint32_t command_list_base;		    // Command list base address, 1K-byte aligned
	uint32_t command_list_upper;		// Command list base address upper 32 bits
	uint32_t fis_base;		            // FIS base address, 256-byte aligned
	uint32_t fis_upper;		            // FIS base address upper 32 bits
	// 0x10
	uint32_t interrupt_status;		    // Interrupt status
	uint32_t interrupt_enable;		    // Interrupt enable
	uint32_t command_and_status;		// Command and status
	uint32_t reserved;		            // Reserved
	// 0x20
	uint32_t task_file_data;		    // Task file data
	uint32_t signature;		            // Signature
    /**
     * Bit 0-3   - Device detection
     * Bit 4-7   - Current interface speed
     * Bit 8-11  - Interface power managment
     * Bit 12-31 - Reserved
     */
	uint32_t sata_status;		        // SATA status (SCR0:SStatus)
	uint32_t sata_control;		        // SATA control (SCR2:SControl)
	// 0x30
	uint32_t sata_error;		        // SATA error (SCR1:SError)
	uint32_t sata_active;		        // SATA active (SCR3:SActive)
	uint32_t command_issue;		        // Command issue
	uint32_t sata_notification;		    // SATA notification (SCR4:SNotification)
	// 0x40
	uint32_t fis_based_switch_control;  // FIS-based switch control
	// 0x44
	uint32_t reserved1[11];	            // Reserved
	// 0x70
	uint32_t vendor[4];	                // Vendor specific
	// 0x80
} hba_port_t;

typedef volatile struct hba {
	// 0x00
    /**
     * Bit 0-4   - Number of Ports (supported in hardward)
     * Bit 5     - Supports external SATA
     * Bit 6     - Enclosure management supported
     * Bit 7     - Command completion coalescing supported
     * Bit 8-12  - Number of command slots
     * Bit 13    - Partial state capable
     * Bit 14    - Slumber state capable
     * Bit 15    - PIO multiple DRQ block
     * Bit 16    - FIS-based switching supported
     * Bit 17    - Supports port multiplier
     * Bit 18    - Supports AHCI mode only
     * Bit 19    - Reserved
     * Bit 20-23 - Interface speed support
     * Bit 24    - Supports command list override
     * Bit 25    - Supports activity LED
     * Bit 26    - Supports aggressive link power management
     * Bit 27    - Supports staggered spin-up
     * Bit 28    - Supports mechanical presence switch
     * Bit 29    - Supports SNotification register
     * Bit 30    - Supports native command queuing
     * Bit 31    - Supports 64-bit addressing
     */
	uint32_t capabilities;	        	// Host capability
    /**
     * Bit 0     - HBA reset
     * Bit 1     - Interrupt enable
     * Bit 2     - MSI revert to single message
     * Bit 3-30  - Reserved
     * Bit 31    - AHCI Enable
     */
	uint32_t global_host_control;		// Global host control
	uint32_t interrupt_status;		    // Interrupt status
	uint32_t port_implemented;		    // Port implemented
	// 0x10
	uint32_t version;		            // Version
	uint32_t ccc_control;	            // Command completion coalescing control
	uint32_t ccc_ports;	                // Command completion coalescing ports
	uint32_t em_location;	            // Enclosure management location
	// 0x20
	uint32_t em_control;	            // Enclosure management control
	uint32_t capabilities_extended;		// Host capabilities extended
	uint32_t bios_os_handoff;		    // BIOS/OS handoff control and status
	// 0x2C
	uint8_t reserved[0xA0-0x2C];        // Reserved
	// 0xA0
	uint8_t vendor[0x100-0xA0];	        // Vendor specific registers
	// 0x100
	hba_port_t ports[32];	            // Port control registers
} hba_t;

typedef enum
{
	fis_reg_h2d_e	    = 0x27,	// Register FIS - host to device
	fis_reg_d2h_e       = 0x34,	// Register FIS - device to host
	fis_dma_act_e       = 0x39,	// DMA activate FIS - device to host
	fis_dma_setup_e	    = 0x41,	// DMA setup FIS - bidirectional
	fis_data_e		    = 0x46,	// Data FIS - bidirectional
	fis_bist_e		    = 0x58,	// BIST activate FIS - bidirectional
	fis_pio_setup_e	    = 0x5F,	// PIO setup FIS - device to host
	fis_dev_bits_e	    = 0xA1,	// Set device bits FIS - device to host
} fis_t;

typedef struct fis_reg_h2d
{
	uint8_t fis_type;	    // fis_reg_h2d_e
    /**
     * Bit 0-3  - Port Mulitplier
     * Bit 4-6  - Reserved
     * Bit 7    - 0: Control, 1: Command
     */
    uint8_t options;
	uint8_t command;	    // Command register
	uint8_t feature_lower;	// Feature register lower 8 bits
	uint8_t lba0;		    // LBA low register, 7:0
	uint8_t lba1;		    // LBA mid register, 15:8
	uint8_t lba2;		    // LBA high register, 23:16
	uint8_t device;		    // Device register
	uint8_t lba3;		    // LBA register, 31:24
	uint8_t lba4;		    // LBA register, 39:32
	uint8_t lba5;		    // LBA register, 47:40
	uint8_t feature_upper;	// Feature register upper 8 bits
	uint8_t count_lower;	// Count register lower 8 bits
	uint8_t count_upper;	// Count register upper 8 bits
	uint8_t icc;		    // Isochronous command completion
	uint8_t control;	    // Control register
	uint8_t reserved[4];	// Reserved
} fis_reg_h2d_t;

typedef struct fis_reg_d2h
{
	uint8_t fis_type;	    // fis_reg_d2h_e
    /**
     * Bit 0-3  - Port Mulitplier
     * Bit 4-5  - Reserved
     * Bit 6    - Interrupt Bit
     * Bit 7    - Reserved
     */
    uint8_t options;
	uint8_t status;	        // Status register
	uint8_t error;	        // Error register
	uint8_t lba0;		    // LBA low register, 7:0
	uint8_t lba1;		    // LBA mid register, 15:8
	uint8_t lba2;		    // LBA high register, 23:16
	uint8_t device;		    // Device register
	uint8_t lba3;		    // LBA register, 31:24
	uint8_t lba4;		    // LBA register, 39:32
	uint8_t lba5;		    // LBA register, 47:40
	uint8_t reserved;	    // Reserved
	uint8_t count_lower;	// Count register lower 8 bits
	uint8_t count_upper;	// Count register upper 8 bits
	uint8_t reserved1[6];	// Reserved
} fis_reg_d2h_t;

typedef struct fis_data
{
    uint8_t fis_type;	    // fis_data_t
    /**
     * Bit 0-3  - Port Mulitplier
     * Bit 4-7  - Reserved
     */
    uint8_t options;
	uint8_t reserved[2];	// Reserved
	uint32_t data;	        // Payload
} fis_data_t;

typedef struct fis_pio_setup
{
	uint8_t fis_type;	    // fis_pio_setup_e
    /**
     * Bit 0-3  - Port Mulitplier
     * Bit 4    - Reserved
     * Bit 5    - Data transfer direction - 0: Host to Device, 1: Device to Host
     * Bit 6    - Interrupt bit
     * Bit 7    - Reserved
     */
    uint8_t options;
	uint8_t status;	            // Status register
	uint8_t error;	            // Error register
	uint8_t lba0;		        // LBA low register, 7:0
	uint8_t lba1;	    	    // LBA mid register, 15:8
	uint8_t lba2;		        // LBA high register, 23:16
	uint8_t device;		        // Device register
	uint8_t lba3;		        // LBA register, 31:24
	uint8_t lba4;		        // LBA register, 39:32
	uint8_t lba5;		        // LBA register, 47:40
	uint8_t reserved;	        // Reserved
	uint8_t count_lower;	    // Count register lower 8 bits
	uint8_t count_upper;	    // Count register upper 8 bits
	uint8_t reserved1;	        // Reserved
	uint8_t e_status;	        // New value of status register
	uint16_t transfer_count;    // Transfer count
	uint8_t reserved2[2];	    // Reserved
} fis_pio_setup_t;

typedef struct fis_dma_setup
{
	uint8_t fis_type;	    // fis_dma_setup_e
    /**
     * Bit 0-3  - Port Mulitplier
     * Bit 4    - Reserved
     * Bit 5    - Data transfer direction - 0: Host to Device, 1: Device to Host
     * Bit 6    - Interrupt bit
     * Bit 7    - Auto-activate. Specifies if DMA Activate FIS is needed
     */
    uint8_t options;
	uint8_t reserved[2];	    // Reserved
	uint64_t dma_buffer_id;		// DMA Buffer Identifier. Used to Identify DMA buffer in host memory.
                                // SATA Spec says host specific and not in Spec. Trying AHCI spec might work.
	uint8_t reserved1[4];	    // Reserved
	uint32_t dma_buffer_offset; // Byte offset into buffer. First 2 bits must be 0
	uint32_t transfer_count;	// Number of bytes to transfer. Bit 0 must be 0
	uint8_t reserved2[4];		// Reserved
} fis_dma_setup_t;

typedef volatile struct hba_fis
{
	// 0x00
	fis_dma_setup_t	dma_setup_fis;		// DMA Setup FIS
	uint8_t pad0[4];
 
	// 0x20
	fis_pio_setup_t	pio_setup_fis;		// PIO Setup FIS
	uint8_t pad1[12];
 
	// 0x40
	fis_reg_d2h_t d2h_register_fis;	    // Register – Device to Host FIS
	uint8_t pad2[4];
 
	// 0x58
	uint8_t set_device_bits_fis[2];	    // Set Device Bit FIS
 
	// 0x60
	uint8_t ufis[64];
 
	// 0xA0
	uint8_t reserved[0x100-0xA0];
} hba_fis_t;

typedef struct hba_cmd_header
{
    /**
     * Bit 0-4  - Command FIS length in DWORDS, 2 ~ 16 (DWORD = 4 bytes)
     * Bit 5    - ATAPI
     * Bit 6    - Write, 0: Host to Device, 1: Device to Host
     * Bit 7    - Prefetchable
     */
    uint8_t options;
    /**
     * Bit 0    - Reset
     * Bit 1    - BIST
     * Bit 2    - Clear busy upon R_OK
     * Bit 3    - Reserved
     * Bit 4-7  - Port multiplier port
     */
    uint8_t options1;
	uint16_t prd_table_length;		    // Physical region descriptor table length in entries
	volatile uint32_t prd_byte_count;	// Physical region descriptor byte count transferred
	uint32_t ctd_base_address;		    // Command table descriptor base address
	uint32_t ctd_bass_address_upper;	// Command table descriptor base address upper 32 bits
	uint32_t reserved[4];	// Reserved
} hba_cmd_header_t;
 
typedef struct hba_prdt_entry
{
	uint32_t data_base_address;		    // Data base address
	uint32_t data_base_address_upper;	// Data base address upper 32 bits
	uint32_t reserved;		            // Reserved
    /**
     * Bit 0-21  - Byte count, 4M max
     * Bit 22-30 - Reserved
     * Bit 31    - Interrupt on completion
     */
    uint32_t options;
} hba_prdt_entry_t;

typedef struct hba_cmd_tbl
{
	uint8_t command_fis[64];	    // Command FIS
	uint8_t atapi_command[16];	    // ATAPI command, 12 or 16 bytes
	uint8_t reserved[48];	        // Reserved
	hba_prdt_entry_t prdt_entry;	// Physical region descriptor table entries, 0 ~ 65535
} hba_cmd_tbl_t;

#endif
