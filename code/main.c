#include "audio.h"
#include "background.h"
#include "bootscreen.h"
#include "display.h"
#include "globals.h"
#include "keys.h"
#include "motor.h"

int main(void) {
    // allow all keys to give interrupts
    *(key_edge_ptr - 1) = 0xF;

    // draw the boot screen then keep scanning until KEY3 pressed
    draw_init_screen();
    while (!initialized) {
        for (int i = 0; i < 100; i++);
        if (*key_edge_ptr) act_on_key_edge();
    }

    // draw the radar screen background
    draw_background();

    // set GP0 to input and GP1 to output
    *pp_dir_ptr = 0x6;

    // enter infinite scrolling loop
    while (1) {
        for (int x = 2; x < 319; x++) {
            // first check if any KEYs pressed or not
            if (*key_edge_ptr) act_on_key_edge();

            // sending TRIG signal to the sensor
            *pp_ptr = 0x2; // set trigger pin to high
            for (int i = 1000; i >= 0; i--); // wait a bit
            *pp_ptr = 0x0; // set trigger pin low
            
            // wait till ECHO goes high then snapshot
            while (!(*pp_ptr & 0x00000001));
            uint start = *mtimer_base;

            // wait till ECHO goes low then snapshot
            while (*pp_ptr & 0x00000001);
            uint stop = *mtimer_base;

            // calculate distance
            uint duration = stop - start;
            int duration_us = (int)(duration * 0.01);
            int distance_cm = (int)((duration_us * 0.0343) / 2);

            // fill up audio FIFOs then do weird wait
            if (toggle_audio) {
                beep();
                for (int i = 0; i < distance_cm * 10000; i++);  
            }

            // move the servo motor
            if (toggle_motor) {
                int pulse = (int)(5 * x + 981);
                servo_pulse(pulse);
            }

            vsync();

            // VGA controller commands
            draw_cursor(x);
            draw_red(x, distance_cm);
            fade_cursor(x-7);
            fade_cursor(x-12);
            fade_cursor(x-20);
            fade_cursor(x-31);
            fade_cursor(x-45);
            fade_cursor(x-62);
            clear_cursor(x-82);
            vsync();
        }
        for (int x = 317; x > 0; x--) {
            // first check if any KEYs pressed or not
            if (*key_edge_ptr) act_on_key_edge();

            // sending TRIG signal to the sensor
            *pp_ptr = 0x2; // set trigger pin to high
            for (int i = 1000; i >= 0; i--); // wait a bit
            *pp_ptr = 0x0; // set trigger pin low
            
            // wait till ECHO goes high then snapshot
            while (!(*pp_ptr & 0x00000001));
            uint start = *mtimer_base;

            // wait till ECHO goes low then snapshot
            while (*pp_ptr & 0x00000001);
            uint stop = *mtimer_base;

            // calculate distance
            uint duration = stop - start;
            int duration_us = (int)(duration * 0.01);
            int distance_cm = (int)((duration_us * 0.0343) / 2);

            // fill up audio FIFOs then do weird wait
            if (toggle_audio) {
                beep();
                for (int i = 0; i < distance_cm * 10000; i++);
            }

            // move the servo motor
            if (toggle_motor) {
                int pulse = (int)(5 * x + 981);
                servo_pulse(pulse);
            }

            vsync();

            // VGA controller commands
            draw_cursor(x);
            draw_red(x, distance_cm);
            fade_cursor(x+7);
            fade_cursor(x+12);
            fade_cursor(x+20);
            fade_cursor(x+31);
            fade_cursor(x+45);
            fade_cursor(x+62);
            clear_cursor(x+82);
            vsync();
        }
    }

    return 0;
}
