#ifndef VIDEO_H
#define VIDEO_H

#include "clib.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void clear();
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void update_cursor(int x, int y);
uint32_t get_cursor_position(void);
void garboize();
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

#endif