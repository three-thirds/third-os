OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(loader)

SECTIONS {
  . = 0x00100000;

  .text ALIGN(4K) : {
    KEEP(*(.multiboot))
    *(.text .text.*)
  }

  .rodata ALIGN(4K) : {
    *(.rodata .rodata.*)
  }

  .data ALIGN(4K) : {
    *(.data .data.*)
  }

  .bss ALIGN(4K) : {
    *(COMMON)
    *(.bss .bss.*)
  }

  /* First byte after the kernel image (used by the PMM). */
  kernel_end = .;

  /DISCARD/ : {
    *(.comment)
    *(.note*)
  }
}
