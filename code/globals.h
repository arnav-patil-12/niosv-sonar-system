#ifndef GLOBALS_H
#define GLOBALS_H

#define SW_BASE 0xFF200040
#define LEDR_BASE 0xFF200000
#define KEY_EDGE 0xFF20005C
#define VGA_CON_FB 0xFF203020
#define VGA_CON_STAT 0xFF20302C
#define MTIME_LOW 0xFF202100
#define PP_BASE 0xFF200070
#define TIMER2_BASE 0xFF202020
#define GREEN 0x07E0
#define BLACK 0x0
#define RED 0xF800
#define NUM_SAMPLES 1754
#define DEG_0 1000 // 0 degrees = 1 ms
#define DEG_180 4000 // 180 degrees = 4 ms
#define AUDIO_BASE 0xFF203040

typedef unsigned int uint;

extern int initialized = 0; // stays zero until KEY3 pressed, then goes to 1, controlled by key3_service();
extern int toggle_audio = 1; // audio plays if 1, not if 0, controlled by key1_service()
extern int toggle_motor = 1; // motor spins if 1, not if 0, controlled by key2_service()
extern int pixel_buffer; // variable that we store the start of the pixel buffer into at the start of the drawing functions
extern int pulse_increment = 5; // how much we want to increment the servo motor by, not sure if we're even using this in main.c
extern volatile int *ledr_ptr = (int *)(LEDR_BASE); // pointer to LEDs
extern volatile int *pp_ptr = (int *)(PP_BASE); // pointer to GPIO pins
extern volatile int *pp_dir_ptr = (int *)(PP_BASE + 4); // pointer to GPIO direction register
extern volatile int *mtimer_base = (int *)(MTIME_LOW); // pointer to machine timer low count
extern volatile int *sw_ptr = (int *)(SW_BASE); // pointer to SW base
extern volatile int *key_edge_ptr = (int *)(KEY_EDGE); // pointer to the KEY PIT edge detect register
extern volatile int *vga_ptr = (int *)(VGA_CON_FB); // pointer to the front buffer of the VGA controller
extern volatile int *vga_cont_ptr = (int *)(VGA_CON_STAT); // pointer to control register of the VGA controller
extern volatile struct audio_t *const audio_ptr = (struct audio_t *)(AUDIO_BASE); // pointer to the audio controller
extern volatile struct timer_t *const timer2_ptr = (struct timer_t *)(TIMER2_BASE); // pointer to the second interval timer

/* Custom Structs */
struct audio_t {
    volatile unsigned int control;
    volatile unsigned char rarc;
    volatile unsigned char ralc;
    volatile unsigned char wsrc;
    volatile unsigned char wslc;
    volatile unsigned int ldata;    
    volatile unsigned int rdata;
};

struct timer_t {
    volatile unsigned int status;
    volatile unsigned int control;
    volatile unsigned int startValLow;
    volatile unsigned int startValHigh;
    volatile unsigned int snapshotLow;
    volatile unsigned int snapshotHigh;
};

#endif