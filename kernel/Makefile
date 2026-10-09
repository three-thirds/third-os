# Linux hosts typically use gcc + binutils ld.
# On Windows/MSYS2, the native gcc/ld emit PE, so use clang + lld for ELF32.
ifeq ($(OS),Windows_NT)
  CC := clang
  LD := ld.lld
  CFLAGS_TARGET := --target=i386-elf
else
  CC := gcc
  LD := ld
  CFLAGS_TARGET :=
endif

AS := nasm
QEMU := qemu-system-i386

CFLAGS  := $(CFLAGS_TARGET) \
           -m32 \
           -ffreestanding \
           -fno-builtin \
           -fno-stack-protector \
           -nostdlib \
           -Wall \
           -Wextra \
           -Iinclude

ASFLAGS := -f elf32
LDFLAGS := -m elf_i386 -T linker.ld -nostdlib --entry=loader

C_SRCS := $(wildcard src/*.c)
C_OBJS := $(C_SRCS:.c=.o)

ASM_SRCS := $(wildcard arch/x86/*.s)
ASM_OBJS := $(ASM_SRCS:.s=.o)

KERNEL := kernel.elf
ISO := os.iso

.PHONY: all run run-iso iso clean

all: $(KERNEL)

$(KERNEL): $(ASM_OBJS) $(C_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

arch/x86/%.o: arch/x86/%.s
	$(AS) $(ASFLAGS) $< -o $@

src/%.o: src/%.c
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

clean:
	rm -f $(KERNEL) $(ISO) $(C_OBJS) $(ASM_OBJS) iso/boot/kernel.elf
