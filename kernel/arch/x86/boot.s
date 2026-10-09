global loader
extern kmain

MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x1          ; request mem_lower / mem_upper
CHECKSUM     equ -(MAGIC_NUMBER + FLAGS)

KERNEL_STACK_SIZE equ 16384

section .multiboot
align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
kernel_stack:
    resb KERNEL_STACK_SIZE

section .text
loader:
    mov esp, kernel_stack + KERNEL_STACK_SIZE
    ; SysV i386: push args right-to-left → kmain(magic, mbi)
    push ebx
    push eax
    call kmain
    add esp, 8

.hang:
    cli
    hlt
    jmp .hang
