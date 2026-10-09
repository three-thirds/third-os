# Third OS: Kernel Technical Specification & Guide

This directory contains the 32-bit x86 bare-metal kernel subsystem. The kernel executes in a freestanding environment without access to standard C library runtimes (`libc`) or underlying host OS services.

---

## 1. System Specifications

| Parameter           | Specification                                                   |
| :------------------ | :-------------------------------------------------------------- |
| **Architecture**    | x86 (IA-32 / 32-bit Protected Mode)                             |
| **Boot Protocol**   | Multiboot 1 Specification (v0.6.96)                             |
| **Link Address**    | `0x00100000` (1 MiB boundary)                                   |
| **Binary Format**   | ELF32 (`elf_i386`)                                              |
| **Execution State** | Flat memory model, interrupts disabled on boot, paging disabled |
| **ABI**             | System V i386 ABI                                               |

---

## 2. Toolchain & Dependencies

The build pipeline requires a 32-bit compatible C compiler, NASM, the GNU linker, and QEMU with graphical display support.

### Required Packages:

- **Fedora / RHEL:**
  ```bash
  sudo dnf install gcc nasm qemu-system-i386 qemu-ui-gtk genisoimage
  ```
- **Debian / Ubuntu:**
  ```bash
  sudo apt install build-essential nasm qemu-system-x86 genisoimage
  ```
- **Arch Linux:**
  ```bash
  sudo pacman -S base-devel nasm qemu-system-x86 cdrtools
  ```

_Note on Fedora/Wayland:_ If QEMU launches with `VNC server running on ::1:5900` instead of a display window, verify that `qemu-ui-gtk` is installed and supply `-display gtk` to the emulator command.

---

## 3. Physical Memory Map

The kernel uses a 1 MiB physical load address to prevent collisions with legacy BIOS structures, memory-mapped device registers, and real-mode vectors below 1 MiB.

```text
+-----------------------------------+ 0xFFFFFFFF (4 GiB)
|                                   |
|      Unmapped / Free RAM          |
|                                   |
+-----------------------------------+
|      Kernel Stack (BSS)           | (Grows downwards)
+-----------------------------------+
|      Kernel Image (.text, .data)  | 0x00100000 (1 MiB Link Point)
+-----------------------------------+
|      BIOS Reserved / Extended     | 0x000F0000
+-----------------------------------+
|      VGA Text Buffer (80x25)      | 0x000B8000 - 0x000BFFFF
+-----------------------------------+
|      Conventional Memory (BDA/IVT)| 0x00000000 - 0x00007FFF
+-----------------------------------+
```

### Video Display Hardware (`0x000B8000`):

- Memory-mapped text matrix: 80 columns × 25 rows.
- Each character cell occupies 2 consecutive bytes (16 bits):
  - **Byte 0 (Bits 0–7):** ASCII character code.
  - **Byte 1 (Bits 8–15):** Attribute byte (Foreground color: bits 0–3, Background color: bits 4–7).

---

## 4. Bootstrapping Sequence

1. **Bootloader Handoff:**
   - GRUB (or QEMU direct loader) scans the first 8 KiB of `kernel.elf` for the Multiboot header:
     - `MAGIC`: `0x1BADB002` (4-byte aligned).
     - `FLAGS`: Flags bitfield.
     - `CHECKSUM`: `-(MAGIC + FLAGS)`.
   - The bootloader verifies `MAGIC + FLAGS + CHECKSUM == 0`, enters 32-bit Protected Mode, sets `CS` and `DS` descriptors to flat 4 GiB segments, sets `EAX = 0x2BADB002`, and jumps to the entry point symbol `loader`.

2. **Assembly Setup (`loader.s`):**
   - Allocates stack memory in the `.bss` section (typically 4–16 KiB).
   - Points the stack pointer (`ESP`) to the top of the allocated stack block.
   - Clears `EFLAGS`.
   - Calls the C entry symbol (`call kmain`).

3. **Kernel Main Execution (`kmain`):**
   - Initializes core hardware subsystems (VGA buffer, GDT, IDT).
   - Enters an infinite terminal execution loop (`for(;;)` / `hlt`).

---

## 5. Compiler Flags & Constraints

All C source code compiled for the kernel must use freestanding compiler flags:

- **`-m32`**: Targets 32-bit x86 architecture on 64-bit host machines.
- **`-ffreestanding`**: Instructs GCC that standard library runtime routines do not exist.
- **`-fno-builtin`**: Prevents the compiler from implicitly replacing loops with calls to library symbols like `memset`, `memcpy`, or `printf`.
- **`-fno-stack-protector`**: Disables stack-canary checks, which require runtime functions (`__stack_chk_fail`) not yet implemented in the kernel.
- **`-nostdlib`**: Excludes host runtime initialization objects (`crt0.o`) and standard C libraries (`libc`, `libm`).

### ABI Calling Conventions:

- Arguments are pushed onto the stack in reverse order (right-to-left).
- Functions preserve `EBX`, `ESI`, `EDI`, and `EBP`.
- Functions may modify `EAX`, `ECX`, and `EDX`.
- 32-bit return values are placed in register `EAX`.

---

## 6. Build Targets

The `Makefile` in this directory provides the following interfaces:

```bash
make          # Compiles all .s and .c files, links kernel.elf
make run      # Boots kernel.elf directly in QEMU (bypasses ISO generation)
make iso      # Copies kernel.elf into iso/boot/ and builds os.iso via genisoimage
make run-iso  # Boots the complete bootable CD-ROM image via GRUB 0.97
make clean    # Removes all compiled object files, kernel binaries, and ISO images
```

---

## 7. Directory Layout & Module Structure

```text
kernel/
├── Makefile            # Build orchestration & dependency graph
├── link.ld             # Linker script specifying physical memory sections
├── loader.s            # Multiboot header and assembly stack initialization
├── include/            # Kernel header declarations (.h)
│   ├── vga.h           # VGA colors, screen dimensions, driver signatures
│   └── io.h            # In/Out assembly port primitives
├── src/                # Kernel C source files (.c)
│   ├── kmain.c         # Primary kernel entry point
│   └── vga.c           # Memory-mapped console driver
└── iso/                # El Torito ISO staging directory
    └── boot/
        ├── grub/
        │   ├── menu.lst        # GRUB runtime configuration
        │   └── stage2_eltorito # El Torito bootloader binary (~103 KB)
        └── kernel.elf          # Output kernel binary
```

---

## 8. Contributor Rules & Practices

1. **No Implicit Library Calls:** Always implement or explicitly declare your helper functions. Do not include `<stdio.h>`, `<stdlib.h>`, or `<string.h>`.
2. **Hardware I/O Isolation:** Hardware port communication (`inb`, `outb`) must use inline assembly routines wrapped inside `include/io.h`.
3. **Linker Script Awareness:** Any new initialized global data belongs in `.data`; uninitialized buffers must be placed in `.bss`. Do not assume RAM is zeroed unless placed in `.bss`.
4. **Bootloader File Integrity:** Do not replace `iso/boot/grub/stage2_eltorito` with arbitrary GRUB binaries. The binary must strictly be the 103 KB El Torito stage 2 binary to ensure correct BIOS `INT 13h` relocation parsing.
