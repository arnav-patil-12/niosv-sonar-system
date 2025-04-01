#ifndef KEYS_H
#define KEYS_H

#include "globals.h"

/* Function Declarations */
void act_on_key_edge(void); // function that handles KEY button edge detection
void key0_service(void); // initializes system
void key1_service(void); // mutes and unmutes audio
void key2_service(void); // stops sensor rotation
void key3_service(void); // pause VGA and sonar scan

/* Function Implementations */

/* pseudo interrupt handler, checks for each button press and calls the relevant key 
handling function. makes sure to reset the edge register before returning */
void act_on_key_edge(void) {
    int keyCode = *key_edge_ptr & 0xF;

    if (keyCode & 0x1) key0_service(); // KEY0 pressed, should initialize
    if (keyCode & 0x2) key1_service(); // KEY1 pressed, should mute/unmute
    if (keyCode & 0x4) key2_service(); // KEY2 pressed. should start/stop motor
    if (keyCode & 0x8) key3_service(); // KEY3 pressed, should turn on/off VGA

    // reset the edge register if not reset already;
    *key_edge_ptr = 0xF;

    return;
}

/* non-interrupt service routine for key0 */
void key0_service(void) {
    // toggle initialized global variable
    initialized = 1;

    return;
}

/* non-interrupt service routine for key1 */
void key1_service(void) {
    if (toggle_audio) toggle_audio = 0;
    else toggle_audio = 1;
    
    return;
}

/* non-interrupt service routine for key2 */
void key2_service(void) {
    if (toggle_motor) toggle_motor = 0;
    else toggle_motor = 1;
    
    return;
}

/* non-interrupt service routine for KEY3*/
void key3_service(void) {
    // assumes B0 in the edge register is high
    // turn off the screen
    *vga_cont_ptr = 0x0;

    // begin by driving 1 to reset the edge bit
    *key_edge_ptr = 0xF;

    // wait till the edge bit gets high again (button pressed again)
    while (!(*key_edge_ptr & 0x08));

    // drive 1 again then return;
    *key_edge_ptr = 0xF;

    // turn the screen back on
    *vga_cont_ptr = 0x4;

    return;
}

#endif