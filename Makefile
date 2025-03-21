# Automatically generate lists of sources using wildcard.
C_SOURCES = $(wildcard src/*.c src/drivers/*.c)
HEADERS = $(wildcard include/*.h include/drivers/*.h)

# Todo: make sources depend on header files.

# Convert *.c filenames to *.o as a list of object files to build.
OBJ = ${C_SOURCES:.c=.o}

# Clean, build and run.
all: clean os-image run

# Removes all built files.
clean:
	rm -f src/boot/*.elf
	rm -f src/boot/*.bin
	rm -f src/drivers/*.o
	rm -f example/*.elf example/*.o
	rm -f src/*.elf src/*.o src/*.bin
	rm -f os-image

# --------------------------------------------------------------------------------------------------
# Examples and learning

# A hand-written binary of a trivially valid boot sector.
minimal_example:
	qemu-system-x86_64 -drive file=example/minimal,format=raw -net none

# An empty boot sector which loops forever.
boot_sector: example/boot_sector.bin
	qemu-system-x86_64 -drive file=$<,format=raw -net none

# A simple boot sector that demonstrates addressing.
find_the_byte: example/find_the_byte.bin
	qemu-system-x86_64 -drive file=$<,format=raw -net none

# Printing a message letter-by-letter using BIOS interrupts.
hello_simple: example/hello_simple.bin
	qemu-system-x86_64 -drive file=$<,format=raw -net none

# Printing a message by calling a `print_string` routine.
hello_advanced: example/hello_advanced.bin
	qemu-system-x86_64 -drive file=$<,format=raw -net none

# Other real mode examples:
# - stack_example: a simple boot sector that demonstrates the stack.
# - print_hex_example: runs a routine that prints out the hex value 0x1fb6.
# - segments_example: an example showing some aspects of segment offsetting.
# - disk_load_example: an example loading magic numbers written to disk.
%_example: example/%_example.bin
	qemu-system-x86_64 -drive file=$<,format=raw -net none

# A boot sector that switches from real to protected mode.
boot_to_pm: example/boot_to_pm.bin
	qemu-system-x86_64 -drive file=$<,format=raw -net none

# Compiler/disassembly examples:
# - basic_disassemble: prints out disassembly of a trivial function call.
# - local_var_disassemble: shows disassembly of allocating an integer.
# - calling_disassemble: disassembles the relationship between a function and its caller.
# - string_disassemble: shows storing a string as a pointer to null-terminated char array.
%_disassemble: example/%_disassemble.bin
	ndisasm -b 32 $<

example/%_disassemble.bin: example/%_disassemble.o
	x86_64-elf-ld -melf_i386 -o $@ -Ttext 0x0 --oformat binary $<

# Kernel code --------------------------------------------------------------------------------------

# Create kernel binary by linking kernel entry code to kernel main.
src/kernel.elf: src/enter_kernel.o $(OBJ)
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^

src/kernel.bin: src/kernel.elf
	objcopy -O binary $< $@

# Create executable image by prepending kernel binary with boot sector.
os-image: src/boot/load_kernel.bin src/kernel.bin
	cat $^ > $@

# Compile assembly sources into binary.
%.bin: %.asm
	nasm $< -f bin -o $@

# Assemble the kernel entry to an object file.
%.o: %.asm
	nasm $< -f elf -o $@

# Compile C sources into object files.
%.o: %.c
	gcc -m32 -ffreestanding -fno-pie -g -c $< -o $@

# --------------------------------------------------------------------------------------------------

run-debug: clean os-image run-server

debug: debug1 debug2

debug1: src/kernel.bin
	objcopy --only-keep-debug $< src/kernel.sym

debug2: src/kernel.sym
	objcopy --strip-debug src/kernel.elf

run-server: os-image
	qemu-system-i386 -drive file=$<,format=raw -net none -s -S

# Run operating system on emulated x86.
run: os-image
	qemu-system-i386 -drive file=$<,format=raw -net none
