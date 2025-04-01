#ifndef DEFINE_H
#define DEFINE_H

#include "globals.h"
#include "background.h"
#include "bootscreen.h"

/* Function Declarations */
void clear_cursor(int x); // clears cursor column to black
void draw_background(void); // draws the background at the start of the program
void draw_init_screen(void); // draws the initial screen upon boot
void draw_cursor(int x); // draws the green cursor column
void draw_red(int x, int val); // draws the red cursor column
void fade_cursor(int x); // creates the fading tail effect
void vsync(void); // function that syncs the program with the VGA controller

/* Function Implementations */

/* double for loop to iterate through each pixel and write from the bmp_data
struct to the front buffer directly. Uses code partially implemented in Lab 7 */
void draw_background(void) {
    volatile int *vga_ptr = (int *)(VGA_CON_FB);
    volatile short int *pixel;
    pixel_buffer = *vga_ptr;

    for (int x = 0; x < 320; x++) {
        for (int y = 0; y < 240; y++) {
            pixel = (short int *)(pixel_buffer + (y<<10) + (x<<1));
            *pixel = BMP_DATA[y][x];
        }
    }

    return;
}

/* double for loop to iterate through each pixel and write from the initscreen struct
to the front buffer directly. uses code parially implemented in lab 7 */
void draw_init_screen(void) {
    volatile int *vga_ptr = (int *)(VGA_CON_FB);
    volatile short int *pixel;
    pixel_buffer = *vga_ptr;

    for (int x = 0; x < 320; x++) {
        for (int y = 0; y < 240; y++) {
            pixel = (short int *)(pixel_buffer + (y<<10) + (x<<1));
            *pixel = INITSCREEN[y][x];
        }
    }

    return;
}

/* draws the green cursor between y=60 and y=199 at the specified x coordinates */
void draw_cursor(int x) {
    volatile short int  *pixel;

    for (int y = 60; y < 200; y++) {
        pixel = (short int *)(pixel_buffer + (y<<10) + (x<<1));
        *pixel = GREEN;
    }

    return;
}

/* clears the cursor artifacts at the specified x coordinates.
restrictions are placed on x so as not to clear the screen edges, 
must remain green. */
void clear_cursor(int x) {
    if (x > 318 || x < 1) return;

    volatile short int  *pixel;

    for (int y = 60; y < 200; y++) {
        pixel = (short int *)(pixel_buffer + (y<<10) + (x<<1));
        *pixel = BLACK;
    }

    return;
}

/* draws the red data over top of the green cursor. */
void draw_red(int x, int val) {
    int y_high = 60 + (val * 0.7);
    if (val < 0) y_high = 60;
    else if (val > 200) y_high = 200;
    volatile short int *pixel;

    
    for (int y = y_high; y < 200; y++) {
        pixel = (short int *)(pixel_buffer + (y<<10) + (x<<1));
        *pixel = RED;
    }
}

/* writes a 1 to the first register of the VGA controller (the front buffer),
which sets the S status bit in the control register to 1. It is set 0 when 
the controller has finished drawing the current frame and is about to start
drawing the next frame. */
void vsync(void) {
    volatile int *vga_ptr = (int *)(VGA_CON_FB);
    volatile int *status = (int *)(VGA_CON_STAT);

    *vga_ptr = 0x1;

    // wait till S bit goes back to 0
    while ((*status & 0x1) != 0);

    return;
}

/* test function to fade away the line 
!!!!! this function isn't working on CPUlator for whatever reason but I really hope 
this works on the actual board. all the logic and arithmetic seems correct. */
void fade_cursor(int x) {
    if (x > 318 || x < 1) return;

    volatile short int *pixel;

    for (int y = 60; y < 200; y++) {
        pixel = (short int *)(pixel_buffer + (y << 10) + (x << 1));
        short int red = (*pixel) & 0xF800;
        short int green = (*pixel) & 0x07E0;

        red = (red >> 11);    // 0â31
        green = (green >> 5); // 0â63

        red = (red * 7) / 10;
        green = (green * 7) / 10;

        *pixel = ((green << 5) & 0x07E0) | ((red << 11) & 0xF800);
    }

    return;
}

#endif