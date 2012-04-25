/*
	lateralOS 					(c) 812 terraNova
	==============================================
	terraNova's leightweight Microkernel

	lateralOS - acpi.c
*/
#include <lateralOS/pm/acpi.h>

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
		k_printf("Unable to find ACPI RSDP\r\n");

		panic();
	}

	k_printf("ACPI Rev. %x\r\n", rsdp->rev);

	n = (rsdp->rsdt->header.len - sizeof(struct SDH)) / sizeof(struct SDH *);
	k_printf("SDHs: %d\r\n", n);

	for(i=0; i<n; i++) {
		struct SDH *e = (struct SDH *)rsdp->rsdt->entry[i];

		k_printf("entry at 0x%x -- ",rsdp->rsdt->entry[i]);

		if(e->sig == ACPI_SIG_MADT) {
			k_printf("APIC\r\n");
		} else if(e->sig == ACPI_SIG_SSDT) {
			k_printf("SSDT\r\n");
		} else if(e->sig == ACPI_SIG_SSDT) {
				k_printf("SSDT\r\n");
		} else {
			k_printf("\r\n");
		}
	}
}
