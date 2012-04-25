# TLOS
include Makefile.rules

DEPLOY=./image
OUTPUT=./bin

.PHONY: all

all: build_kernel build_image

build_image:
	$(MKISOFS)  	-R -b boot/grub/$(BOOT_TARGET) -no-emul-boot		\
					-boot-load-size 4 -boot-info-table 			\
					-o $(OUTPUT)/$(IMAGE_TARGET) $(DEPLOY)

build_kernel:
	$(MAKE) -wC kernel

	$(CP) $(KRNL_OUTPUT)/$(KRNL_TARGET) image/boot/$(KRNL_TARGET)

clean:
	$(MAKE) -wC kernel clean

	$(RM) -rf $(DEPLOY)/kernel/*
	$(RM) -rf $(DEPLOY)/ext/*
