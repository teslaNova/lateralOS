/*
	lateralOS 					(c) 812 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - mp.h
*/
#ifndef K_CONF_MP_H_
#define K_CONF_MP_H_

#include <lateralOS/types.h>

#define MP_SIG_FP 		('_' | ('M' << 8) | ('P' << 16) | ('_' << 24))
#define MP_SIG_CONF		('P' | ('C' << 8) | ('M' << 16) | ('P' << 24))

struct mp_config_table;
struct mp_config_entry;

struct mp_floating_pointer {
	uint sig;
	
	struct mp_config_table *conf;
	
	byte len;
	byte rev;
	byte checksum;

	byte features[5];
};

struct mp_config_table {
	uint sig;

	ushort len;
	byte rev;
	byte checksum;

	byte oem[8];
	byte prod_id[12];

	uint oem_tbl_ptr;
	ushort oem_tbl_len;

	ushort entries;
	uint local_apic_base;

	uint reserved;
};

struct mp_entry_proc {
	byte type;

	byte local_apic_id;
	byte local_apic_version;

	byte usable 	: 1;
	byte is_bsp		: 1;

	byte _u0		: 6;

	byte sig[4];
	uint features;

	uint _r0[2];
};

struct mp_entry_bus {
	byte type;

	byte id;
	byte bus_type[6];
};

struct mp_entry_ioapic {
	byte type;

	byte id;
	byte version;
	
	byte usable : 1;
	byte _u0	: 7;

	uint address;
};

struct mp_entry_ioint {
	byte type;

	byte polarity 	: 2;
	byte mode		: 2;
	byte _u0		: 4;

	byte src_bus_id;
	byte src_bus_irq;

	byte dst_ioapic_id;
	byte dst_ioapic_intin;
};

struct mp_entry_lint {
	byte type;

	byte polarity 	: 2;
	byte mode		: 2;
	byte _u0		: 4;

	byte src_bus_id;
	byte src_bus_irq;

	byte dst_lapic_id;
	byte dst_lapic_intin;
};

enum {
	MP_ET_PROC		= 0,
	MP_ET_BUS,
	MP_ET_IOAPIC,
	MP_ET_IOINT,
	MP_ET_LINT,

	MP_ES_PROC		= 20,
	MP_ES_DEF		= 8,
};

void
mp_parse(void);

#endif /* K_CONF_MP_H_ */