# Third OS (32-bit x86 Kernel)

A freestanding, 32-bit x86 toy operating system built from bare metal for Hack Club's **Third Space** sprint. 

The kernel boots into Protected Mode via the Multiboot 1 standard, sets up a hardware-interfaced VGA text console, provides an in-memory virtual filesystem (RAMFS), and includes low-level debugging utilities (raw physical memory hex dumper and BIOS E820 memory map walker).

## Quick Start

Want to try it without cloning or building anything? You only need QEMU.

1. Install QEMU:
   - **Fedora:** `sudo dnf install qemu-system-i386 qemu-ui-gtk`
   - **Debian / Ubuntu:** `sudo apt install qemu-system-x86`
   - **Arch:** `sudo pacman -S qemu-system-x86`
   - **Windows / macOS:** install from [qemu.org/download](https://www.qemu.org/download/)

2. Download `os.iso` from the [Releases page](https://github.com/three-thirds/third-os/releases/latest).

3. Boot it:
   ```bash
   qemu-system-i386 -cdrom os.iso -display gtk
   ```

   On Windows/macOS, if `-display gtk` isn't available in your build, drop that flag and QEMU will pick its default display.

To exit, close the QEMU window (or press `Ctrl+Alt+G` to release the mouse first if it's grabbed).

Want to build it yourself? See [Build Targets](#6-build-targets).

---
## Toolchain & Requirements

To compile and emulate the kernel on Linux:

```bash
# Fedora / RHEL
sudo dnf install gcc nasm qemu-system-i386 qemu-ui-gtk

# Ubuntu / Debian
sudo apt install build-essential nasm qemu-system-x86

# Arch Linux
sudo pacman -S base-devel nasm qemu-system-x86
```

## Quickstart

Compile and direct-boot into QEMU:

```bash
cd kernel
make run
```

Clean build artifacts:
```bash
make clean
```

Build Final ISO:

```bash
make          # Compiles all .s and .c files, links kernel.elf
make run      # Boots kernel.elf directly in QEMU (bypasses ISO generation)
make iso      # Copies kernel.elf into iso/boot/ and builds os.iso via genisoimage
make run-iso  # Boots the complete bootable CD-ROM image via GRUB 0.97
make clean    # Removes all compiled object files, kernel binaries, and ISO images
```

## Some Features I guess.

1. Bootstrapping & Entry (`arch/x86/boot.s`, `linker.ld`)

2. Video Console & Hardware Cursor (`src/vga.c`, `include/io.h`)

3. Freestanding Standard Library & Logger (`src/string.c`, `src/kprintf.c`)

4. In-Memory Virtual Filesystem (RAMFS) (`src/fs.c`)

5. Debugging & Inspection Tools (`src/memdump.c`)

## Available Shell Commands

Type these directly into the kernel console in QEMU:

- `help` - Lists all available kernel commands
- `clear` - Clears the 80x25 screen and resets cursor to top-left
- `ls` - Lists all files and sizes currently stored in the RAMFS
- `cat <file>` - Displays the contents of a file
- `write <file> <text>` - Creates a new file in RAMFS or overwrites existing contents
- `dump <addr> [len]` - Inspects raw physical memory at `<addr>` in canonical hex/ASCII
- `mmap` - Displays the physical BIOS E820 RAM layout
- `time` - Reads real-world wall clock time from the CMOS Real-Time Clock
- `uptime` - Displays system uptime calculated from PIT timer hardware ticks
