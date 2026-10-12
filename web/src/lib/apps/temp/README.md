# Third OS (32 bit x86 kernel)

A freestanding, 32-bit x86 toy OS built for Third Space.

The kernel boots into Protected Mode via Multiboot mode. And sets up a hardware-like
VGA text console, it also provides an in-memory virtual filesyatem (RAMFS), and includes
some fun debugging utilities like BIOS Memory map walker and Hex dump

---

### Requirements

To compile and emulate this kernel on Linux:

```bash
# Fedora / RHEL
sudo dnf install gcc nasm qemu-system-i386 qemu-ui-gtk

# Ubuntu / Debian
sudo apt install build-essential nasm qemu-system-x86

# Arch Linux
sudo pacman -S base-devel nasm qemu-system-x86
```

### Quickstart

```
git clone https://github.com/three-thirds/third-os.git
cd kernel
make run
```

Then clean all the artifacts with `make clean`

---

### Basic Architecture

1. ENTRY POINT
   a. Multiboot specification: So the multiboot works from a simple assembly file which gives
   the OS it's magic number So that bootloader can verify the OS
   b. Physical Linking point: The binary had to be linked specifically at 0x0010000(1MB) boundary
   to avoid collisions with BIOS stuff
   c. ABI Stack Setup: Allocates a 16KB Stack in .bss, initializes `ESP`, pushes multiboot pointer and executes kmain()

2. VGA rendering stuff
   a. Directly writes VGA frame buffer at 0x000B8000, it like sends 2 bytes per cell, one for actual ASCII and another is
   maybe color info(idk Devaansh wrote it)
   b. CRTC Hardware cursor: Uses outb to connect to VGA I/O ports where:
   - port `0x3D4` - is for cursor
   - port `0x3D5` - 8-bit Data transfer port
   - And automatically handles backspace, newspace, etc.

3. Freestanding standard library
   Because kernel runs without libc, essential memory and string routines were implemented from scratch.

- String primitives like `strcmp`, `strlen`, `strncmp`, `memset` and `memcpy` are written by yours truly @Chish.
- Convert 32-bit signed base 10 integers to ASCII.
- kprintf() can print:

* %s - strings
* %c - single characters
* %d - signed decimal
* %x - unsigned lowercase decimal
* %% - literal percentage

---

I am too lazy to write anymore docs, I got my 10 hours logged :p