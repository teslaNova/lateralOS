/*
	lateralOS 					(c) 812 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - acpi.c
*/
#include <lateralOS/pm/acpi.h>

#include <lateralOS/ism/apic.h>

struct RSDP *rsdp;

void
acpi_init(void) {

}

void
acpi_parse(void) {
	uint tmp;

	uint i, n;
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

	struct RSDP *locate_rsdp(uint start, uint end) {
		struct RSDP *rsdp = 0;
		byte *o = (byte *)start;

		while((uint)o < end) {
			rsdp = (struct RSDP *)o;

			if(!memcmp(rsdp, ACPI_SIG_RSDP, 8)) {
				if(verify((byte *)rsdp, rsdp->len) == 0) {
					return rsdp;
				}
			}

			o++;
		}

		return 0;
	}

	for(i=0; i<3; i++) {
		rsdp = locate_rsdp(loc[i][0], loc[i][1]);

		if(rsdp != 0) {
			break;
		}
	}

	if(rsdp == 0 
	|| rsdp->rsdt->header.sig != ACPI_SIG_RSDT
	|| verify((byte *)&rsdp->rsdt->header, rsdp->rsdt->header.len) != 0) {
		k_cls();
		k_printf("Unable to find ACPI RSDP/RSDT\r\n");

		panic();
	}

	//k_printf("RSDP: 0x%x | RSDT: 0x%x | Entries: 0x%x\r\n", rsdp, rsdp->rsdt, rsdp->rsdt->entry[0]);

	n = (rsdp->rsdt->header.len - sizeof(struct SDH)) / sizeof(struct SDH *);
	//k_printf("SDHs: %d\r\n", n);

	for(i=0; i<n; i++) {
		struct SDH *e = (struct SDH *)rsdp->rsdt->entry[i];

		if(e->sig == ACPI_SIG_MADT) {
			struct MADT *madt = (struct MADT*)(e);
			byte *p = (byte *)madt + sizeof(struct MADT);
			uint offset = 0, limit = madt->header.len - sizeof(struct MADT);

			while(offset < limit) {
				byte type = p[offset];

				switch(type) {
					case ACPI_APIC_TYPE_LAPIC: {
						
					} break;

					case ACPI_APIC_TYPE_IOAPIC: {
						struct IOAPIC *ioapic = (struct IOAPIC *)(p + offset);

						apic_io_set(ioapic->ioapic_addr);
					} break;
				}

				offset += p[offset+1];
			}

		}
	}
}
