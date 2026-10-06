global loader

MAGIC_NUMBER equ 0x1BADB002 ;magic number constant
FLAGS equ 0x0
CHECKSUM equ -MAGIC_NUMBER

section .text:
align 4 ; need to make sure boot is on multiples of 4, so that memory bus reads it properly
  dd MAGIC_NUMBER ;dd means define double
  dd FLAGS
  dd CHECKSUM

loader:
  mov eax, 0xCAFEBABE
.loop:
  jmp .loop
