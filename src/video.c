#include "../include/video.h"
#include "../include/clib.h"

void clear() {
    volatile char* vidmem = (char*)0xb8000;
    volatile char* tmp = vidmem;
    //volatile char* tmp2 = vidmem + 1;

    for (int i = 0; i < (VGA_HEIGHT * VGA_WIDTH); i++) {
        *tmp = 32;
        *(tmp + 1) = 0x09;
        tmp+=2;
    }
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

/*
uint32_t get_cursor_position()
{
    uint32_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint32_t)inb(0x3D5)) << 8;
    return pos;
}
*/

void garboize() {
    char* kernel_memory = (char*) 0xc0000;
    volatile char* video_memory = (char*) 0xb8000;

    for (int i = 0; i < (VGA_HEIGHT * VGA_WIDTH); i++) {
        *(video_memory) = *kernel_memory;
        video_memory += 2;
        kernel_memory++;
    }
}

inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
    /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers that fit in 8b. (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "in %w1, %b0" : "=a"(ret) : "Nd"(port): "memory");
    return ret;
}