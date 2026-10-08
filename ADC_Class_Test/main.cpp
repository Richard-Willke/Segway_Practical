/*
 * main.cpp
 *
 *    Author: Richard Willke
 *    Email: willke.richard@gmail.com
 */



/*
 * stdbool.h:           Boolean definitions for the C99 standard
 * stdint.h:            Variable definitions for the C99 standard
 * System.h:            Header file for the System class
 *                      (needed for example for clock settings)
 * GPIO.h:              Header file for the GPIO class
 * ADC.h:               Header file for the ADC class
 */
#include <stdbool.h>
#include <stdint.h>
#include "System.h"
#include "GPIO.h"
#include "ADC.h"


int main(void)
{
    System adcSystem;
    GPIO red, green;
    ADC pinTest, pinReferenz;

    adcSystem.init(40000000);
    red.init(&adcSystem, GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_DIR_MODE_OUT);
    green.init(&adcSystem, GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_DIR_MODE_OUT);
    pinTest.init(&adcSystem, ADC0_BASE, 1, ADC_CTL_CH1);
    pinReferenz.init(&adcSystem, ADC0_BASE, 2, ADC_CTL_CH2);
    pinTest.setHWAveraging(64);
    pinReferenz.setHWAveraging(64);
    while (1)
    {
        float testspannung = pinTest.read();
        float referenzspannung = pinReferenz.read();

        if(testspannung > referenzspannung){
            green.write(false);
            red.write(true);
        }
        else{
            green.write(true);
            red.write(false);
        }
    }
}
