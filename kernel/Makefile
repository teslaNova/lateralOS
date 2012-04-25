# TLMK
include ../Makefile.rules

CFLAGS += -I./include -I../include
LDFLAGS += -T kernel.ld

SRC=$(shell find init -name '*.[c|S]')\
	$(shell find kernel -name '*.[c|S]')\
	$(shell find mm -name '*.[c|S]')\
	$(shell find ism -name '*.[c|S]')\
	$(shell find pm -name '*.[c|S]')\
	$(shell find mp -name '*.[c|S]')

OBJ=$(addsuffix .o, $(subst %, %, $(basename $(SRC))))

.PHONY: all

all: $(OBJ)
	$(LD) $(LDFLAGS) -Map ./bin/$(KRNL_TARGET).map -o ./bin/$(KRNL_TARGET) $(OBJ)

clean:
	$(RM) -rf $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.S
	$(CC) $(CFLAGS) $^ -o $@
