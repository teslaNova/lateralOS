/*
	lateralOS 					(c) 812 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - mp.c
*/
#include <lateralOS/mp/mp.h>

void
mp_parse(void) {
	struct mp_floating_pointer *mp_fp;
	uint tmp;

	uint i;
	uint loc[3][2] = {
		{0, 		0x400},
		{0x9FC00, 0xA0000},
		{0xE0000, 0xFFFFF}
	};

	char verify(byte *data, uint len) {
		uint i;
		byte sum;

		for(i=0, sum=0; i<len; i++) {
			sum += data[i];
		}

		return sum;
	}

	struct mp_floating_pointer *locate_floating_pointer(uint start, uint end) {
		struct mp_floating_pointer *mp_fp = (struct mp_floating_pointer *)start;
		

		while((uint)mp_fp < end) {
			if(mp_fp->sig == MP_SIG_FP) {
				if(verify((byte *)mp_fp, mp_fp->len * 16) == 0) {
					return mp_fp;
				}
			}

			mp_fp++;
		}

		return 0;
	}

	for(i=0; i<3; i++) {
		mp_fp = locate_floating_pointer(loc[i][0], loc[i][1]);

		if(mp_fp != 0) {
			break;
		}
	}

	if(mp_fp == 0) {
		k_cls();
		k_printf("Unable to find MP Configuration Tables\r\n");

		panic();
	}

	if(mp_fp->conf->sig != MP_SIG_CONF) {
		k_cls();
		k_printf("Unable to find MP Configuration Tables\r\n");

		panic();
	}


	byte *base_entry = ((byte *)mp_fp->conf) + sizeof(struct mp_config_table);

	for(i=0; i<mp_fp->conf->entries; i++) {
		switch(*base_entry) {
			case MP_ET_PROC: {
				base_entry += MP_ES_PROC;
			} break;

			case MP_ET_BUS: {
				base_entry += MP_ES_DEF;
			} break;

			case MP_ET_IOAPIC: {
				struct mp_entry_ioapic *ioapic = (struct mp_entry_ioapic *)base_entry; 
				
				apic_io_set(ioapic->address);

				base_entry += MP_ES_DEF;
			} break;

			case MP_ET_IOINT: {
				base_entry += MP_ES_DEF;
			} break;

			case MP_ET_LINT: {
				base_entry += MP_ES_DEF;
			} break;
		}
	}
}