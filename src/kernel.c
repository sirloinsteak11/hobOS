#include <cpuid.h>

#include "../include/clib.h"
#include "../include/string.h"
#include "../include/video.h"
/* 
 * Some very basic kernel code in C.
 */

void main() {
    // The first position of video memory (row 0, col 0) is at memory address 0xb8000.
    // We store a pointer to this byte. `(char*) ` is necessary before 0xb8000 to typecast
    // it from a 32-bit int to a pointer-to-char.
    //long long int* kernel_memory = (long long int*) 0xc0000;
    volatile char* video_memory = (char*) 0xb8000;

    // We assign the value 'X' to the byte in memory pointed to by `video_memory`.
    // Note: char, byte and uint8 all contain 8 bits of memory, they just mean different things.
    //*video_memory = 'X';

    garboize();
    clear();
    enable_cursor(0, 80);
    update_cursor(0, 0);

    //uint32_t cursor_position = get_cursor_position();

    /*
    char stuff[6] = "krill";
    
    volatile char* tmp = video_memory;
    
    for (int i = 0; stuff[i] != 0; i++) {
        *tmp = stuff[i];
        tmp += 2;
    }*/

    //volatile char* ptr = video_memory;
    //*video_memory = get_cursor_position();

    //*video_memory = stuff[0];

    //clear();
}