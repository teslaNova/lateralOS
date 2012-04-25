/*
	lateralOS 					(c) 2012 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - acpi.h
*/
#ifndef K_ACPI_H_
#define K_ACPI_H_

#include <lateralOS/types.h>

#define ACPI_SIG_RSDP "RSD PTR "
#define ACPI_SIG_RSDT ('R' | ('S' << 8) | ('D' << 16) | ('T' << 24))	
#define ACPI_SIG_XSDT ('X' | ('S' << 8) | ('D' << 16) | ('T' << 24))
#define ACPI_SIG_FACP ('F' | ('A' << 8) | ('C' << 16) | ('P' << 24))
#define ACPI_SIG_FACS ('F' | ('A' << 8) | ('C' << 16) | ('S' << 24))
#define ACPI_SIG_DSDT ('D' | ('S' << 8) | ('D' << 16) | ('T' << 24))
#define ACPI_SIG_SSDT ('S' | ('S' << 8) | ('D' << 16) | ('T' << 24))
#define ACPI_SIG_MADT ('A' | ('P' << 8) | ('I' << 16) | ('C' << 24))

struct RSDT;
struct XSDT;

struct RSDP {
	byte sig[8];
	byte checksum;
	byte oem[6];
	byte rev;
	struct RSDT *rsdt;

	/* v2.0 */
	uint len;
	uint64 xsdt;
	byte ext_checksum;
	uint reserved : 24;
};

struct SDH {
	uint sig;
	uint len;
	byte rev;
	byte checksum;

	byte oem[6];
	byte oem_tbl[8];
	uint oem_rev;

	uint creator_id;
	uint creator_rev;
};

struct RSDT {
	struct SDH header;
	uint *entry;
};

struct XSDT {
	struct SDH header;
	uint64 *entry;
};

struct DSDT;
struct FADT {
	struct SDH header;
	
	uint firmware_ctrl;
	struct DSDT *dsdt;
	
	byte _r0;

	byte pref_pm_profile;
	ushort sci_int;
	uint smi_cmd;

	byte acpi_enable;
	byte acpi_disable;

	byte s4bios_req;

	byte pstate_cnt;
	uint pm1a_evt_blk;
	uint pm1b_evt_blk;
	uint pm1a_cnt_blk;
	uint pm1b_cnt_blk;
	uint pm2_cnt_blk;
	uint pm_tmr_blk;

	uint gpe0_blk;
	uint gpe1_blk;

	byte pm1_evt_len;
	byte pm1_cnt_len;
	byte pm2_cnt_len;
	byte pm_tmr_len;
	byte gpe0_blk_len;
	byte gpe1_blk_len;
	byte gpe1_base;

	byte cst_cnt;

	ushort p_lvl2_lat;
	ushort p_lvl3_lat;

	ushort flush_size;
	ushort flush_stride;

	byte duty_offset;
	byte duty_with;

	byte day_alrm;
	byte mon_alrm;
	byte century;

	ushort iapc_boot_arch;

	byte _r1;

	uint flags;

	byte reset_reg[12];
	byte reset_value;

	byte _r2[3];

	uint64 x_firmware_ctrl; // FACS
	uint64 x_dsdt;			// DSDT

	byte x_pm1a_evt_blk[12];
	byte x_pm1b_evt_blk[12];
	byte x_pm1a_cnt_blk[12];
	byte x_pm1b_cnt_blk[12];
	byte x_pm2_cnt_blk[12];
	byte x_pm_tmr_blk[12];
	byte x_gpe0_blk[12];
	byte x_gpe1_blk[12];
};

struct FACS {
	uint sig;
	uint len;
	uint hw_sig;
	uint firmware_waking_vector;
	uint global_lock;
	uint flags;
	uint64 x_firmware_waking_vector;
	byte vers;
	byte _r0[31];
};

struct DSDT {
	struct SDH header;
	byte *aml_code;
};

struct SSDT {
	struct SDH header;
	byte *aml_code;
};

struct LAPIC;
struct IOAPIC;
struct ISO;
struct NMI;
struct LAPIC_NMI;
struct LAPIC_AO;
struct IOSAPIC;
struct LSAPIC;
struct PIS;

struct MADT {
	struct SDH header;
	uint lapic_addr;
	uint flags;
	// apic structure[n]
};

enum {
	ACPI_APIC_TYPE_LAPIC = 0,
	ACPI_APIC_TYPE_IOAPIC,
	ACPI_APIC_TYPE_ISO,
	ACPI_APIC_TYPE_NMI,
	ACPI_APIC_TYPE_LAPIC_NMI,
	ACPI_APIC_TYPE_LAPIC_AO,
	ACPI_APIC_TYPE_IOSAPIC,
	ACPI_APIC_TYPE_LSAPIC,
	ACPI_APIC_TYPE_PIS,
	ACPI_APIC_TYPE_RESERVED,
};

struct LAPIC {
	byte type;
	byte len;
	byte acpi_proc_id;
	byte apic_id;
	uint flags;
};

enum {
	ACPI_LAPIC_ENABLED = 1,
};

struct IOAPIC {
	byte type;
	byte len;
	byte ioapic_id;
	byte _r0;
	uint ioapic_addr;
	uint gs_int_base;
};

struct ISO {
	byte type;
	byte len;
	byte bus;
	byte source;
	uint gs_int;
	ushort flags;
};

struct NMI {
	byte type;
	byte len;
	ushort flags;
	uint gs_int;
};

struct LAPIC_NMI {
	byte type;
	byte len;
	byte acpi_proc_id;
	ushort flags;
	byte lapic_lint_no;
};

struct LAPIC_AO {
	byte type;
	byte len;
	ushort _r0;
	uint64 lapic_address;
};

struct IOSAPIC {
	byte type;
	byte len;
	byte ioapic_id;
	byte _r0;
	uint gs_int_base;
	uint64 iosapic_address;
};

struct LSAPIC {
	byte type;
	byte len;
	byte acpi_proc_id;
	byte lsapic_id;
	byte lsapic_eid;
	byte _r0[3];
	uint flags;
	uint acpi_proc_uid;
	byte *acpi_proc_uid_str;
};

struct PIS {
	byte type;
	byte len;
	ushort flags;
	byte int_Type;
	byte proc_id;
	byte proc_eid;
	byte iosapic_vec;
	uint gs_int_base;
	uint platform_int_src_flags;
};

void
acpi_init(void);

void
acpi_parse(void);

#endif /* K_ACPI_H_ */