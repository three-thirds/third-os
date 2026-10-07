CC := gcc
AS := NASM
LD := ld
QEMU := qemy-system-i386

CFLAGS  := -m32 \
           -ffreestanding \
           -fno-builtin \
           -fno-stack-protector \
           -nostdlib \
           -Wall \
           -Wextra \
           -Iinclude

ASFLAGS := -f elf32
LDFLAGS := -m elf_i386 -T link.ld -nostdlib

C_SRCS := $(wildcard *.c) $(wildcard src/*.c)
C_OBJS := $(C_SRCS:.c=.o)

ASM_SRCS := $(wildcard *.s) $(wildcard src/*.s)
ASM_OBJS := $(ASM_SRCS:.s=.o)

KERNEL := kernel.elf
ISO := os.iso

.PHONY := all run run-iso iso bochs clean

all: $(KERNEL)

$(KERNEL): $(ASM_OBJS) $(C_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Assemble NASM files
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

# Compile C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

iso: $(ISO)

$(ISO): $(KERNEL)
	cp $(KERNEL) iso/boot/kernel.elf
	genisoimage -R \
		-b boot/grub/stage2_eltorito \
		-no-emul-boot \
		-boot-load-size 4 \
		-A os \
		-input-charset utf8 \
		-quiet \
		-boot-info-table \
		-o $(ISO) \
		iso

run: $(KERNEL)
	$(QEMU) -kernel $(KERNEL) -display gtk

run-iso: $(ISO)
	$(QEMU) -cdrom $(ISO) -display gtk

bochs: $(ISO)
	bochs -f bochsrc.txt -q
clean:
	rm -f $(KERNEL) $(ISO) *.o src/*.o arch/x86/*.o iso/boot/kernel.elf
