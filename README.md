# NIOS-V Sonar System

Built by: Arnav Patil & Jaidyn Duan for our ECE243 (Computer Organization) final course project.

## Project Description

Our project is a sonar system built by connecting an ultrasonic sensor and a servo motor to the NIOS-V processor (downloaded on the DE1-SoC board). Both devices are powered by the Vcc5 and GND pins of the JP1 parallel port. The GPIO pins used to transmit and receive data drive 3.3V, whereas the sensor and motor require 5V; the pull-up and pull-down networks are described in the following section. 

In its main loop, the program asserts the ```TRIG``` signal for at least 10 microseconds to probe the sensor. Then, the program measures the duration of the returned ```ECHO``` pulse using the machine timer. The distance between the sensor and a detected object is encoded in the width of the pulse, so we extract that distance value to be output. There must be at least 60 microseconds between ```TRIG``` probes, though this is not a concern as the VGA controller’s vertical synchronization ensures an adequate pause between scans. The servo motor is controlled by the width of a pulse sent through its ```PWM``` signal; a 1 millisecond pulse turns the motor to 0 degrees, and a 4 millisecond pulse turns the motor to 180 degrees.

The VGA monitor display consists of a dark background with some textual elements and a scan bar with a fading cursor. The green cursor “scans” from left to right then back. A red section is drawn on top, with the height representing the distance measured. A taller red cursor indicates the sensor is detecting an object that is closer, and vice versa. 
The project also has an audio component; a “beep” is played for each scan. The frequency of the “beep” represents distance; a faster “beep” represents the object is closer and vice versa. 

We used the KEY push button ports to introduce a level of control. The KEYs are used to turn on/off the display and scan, mute the audio, and turn off the motor. A key design choice was made to not use the interrupts since the program heavily depends on the machine timer and time-sensitive signals for the motor and sensor. We cannot risk the program entering the interrupt handler while waiting for a signal or measuring time, as that could cause the program to get stuck, or miscalculate a time measurement. 

## Hardware Components

The sensor utilized for the project is a first-generation HC SR-04 ultrasonic sensor, and the motor utilized is the SG-90 servo motor. As mentioned in the project description, the sensor requires 5V, whereas the data pins for the JP1 port drive 3.3V. To address this, we implemented a pull-up and pull-down network using a BJT logic circuit and a voltage division circuit. The servo motor sees 3.3V as a “high” signal meaning no additional circuitry is required to use it. 

## Operation Instructions

Once the program has been compiled and downloaded onto the NIOS-V system, the KEY push buttons are the only human inputs to the program. The operations are given on the boot screen, as they are given below:

- KEY0 – initializes the system (VGA switches to scan mode and sensor/motor begin running),
- KEY1 – mutes and unmutes the audio without pausing the scan,
- KEY2 – pauses and unpauses the motor spin without pausing the scan,
- KEY3 – pauses and unpauses the entire operation of the system and turns off the VGA display.
