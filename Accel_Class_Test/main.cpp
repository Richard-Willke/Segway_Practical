/*
 * main.cpp
 *
 *    Author: Richard Willke
 *     Email: willke.richard@gmail.com
 *
 *
 * Test program for the MPU6050. It simply reads the sensor values. These can
 * be visualized with the Arduino Serial Plotter.
 * Push SW1 on the LaunchPad to initialize the MPU6050 and start sampling. Push
 * it again to stop and power off the sensor. When stopped (blue LED on the
 * LaunchPad is off) the sensor can be disconnected and replaced by another
 * MPU6050 without resetting the Tiva microcontroller.
 */



/*
 * stdbool.h:           Boolean definitions for the C99 standard
 * stdint.h:            Variable definitions for the C99 standard
 * System.h:            Header file for the System class
 *                      (needed for example for clock settings)
 * GPIO.h:              Header file for the GPIO class
 * MPU6050.h:           Header file for the MPU6050 class
 */
#include <stdint.h>
#include <stdbool.h>
#include "System.h"
#include "GPIO.h"
#include "MPU6050.h"


int main(void)

{
    // Define all variables and objects
    System system;
    GPIO sensorEnable, sensorSupply;
    MPU6050 sensor;

    // Start system and run at 40 MHz.
    system.init(40000000);

    // Temporarily store all readings in a variable for easier debugging
    // with CCS (easier to read variables than to read return values).
    float angleRate, hor, ver;

    // Use the on-board switch SW1 connected to PF4 to enable or disable the
    // sensor. We need a pull-up, as the switch is connected to GND.
    sensorEnable.init(&system, GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN, true);

    // Use PF2 to supply the sensor.
	sensorSupply.init(&system, GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_DIR_MODE_OUT);

	// Configure as supply pin (max. current) and make sure it's off.
    sensorSupply.write(0);
	sensorSupply.setCurrent(12);

	while (42)
	{
        // wait for user to (re-)enable sampling
        while (sensorEnable.read());

        // Power up sensor
        sensorSupply.write(1);

        // Delay for debounce and sensor power up
        system.delayUS(50000);

        // Wait for button release
        while (!sensorEnable.read());

        // Initialize possibly new sensor.
        sensor.init(&system, I2C1_BASE, 0, 'y', 'z');

	    // If the button SW1 (PF4) is pressed, stop sampling.
        // Otherwise read its data and send it to the computer.
	    while (sensorEnable.read())
	    {
	        // Read all data
	        angleRate = sensor.getAngleRate();
	        hor       = sensor.getAccelHor();
	        ver       = sensor.getAccelVer();

	        // Update System class with the new values
            system.setDebugVal("Anglerate_[deg/s]", angleRate);
	        system.setDebugVal("Horizontal_Accel_[0.1g]", 10*hor);
	        system.setDebugVal("Vertical_Accel_[0.1g]", 10*ver);

	        // Send them to the Arduino plotter
	        system.sendDebugVals();

	        // Reduce update frequency to around 100Hz.
	        system.delayUS(10000);
	    }

	    // Power down sensor
	    sensorSupply.write(0);

	    // Delay for debounce
        system.delayUS(50000);

        // Wait for button release
        while (!sensorEnable.read());
	}
}
