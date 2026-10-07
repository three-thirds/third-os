OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY(loader)

SECTIONS {
  . = 0x00100000; /* this code should only load at 1MB*/

  .text ALIGN(0x1000): /*align at 4kb */
  {
    *(.text)  /* all text from all files */
  }

  .rodata ALIGN(0x1000):
  {
    *(.rodata)
  }

  .data ALIGN(0x1000):
  {
    *(.data)
  }

  .bss ALIGN(0x1000):
  {
    *(.COMMON)
    *(.bss)
  }


}
