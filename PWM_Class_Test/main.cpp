/*
 * main.cpp
 *
 *    Author: Richard Willke
 *     Email: willke.richard@gmail.com
 */

/*
 * stdbool.h:           Boolean definitions for the C99 standard
 * stdint.h:            Variable definitions for the C99 standard
 * System.h:            Header file for the System class
 *                      (needed for example for clock settings)
 * GPIO.h:              Header file for the GPIO class
 * Timer.h:             Header file for the Timer class
 */
#include <stdbool.h>
#include <stdint.h>
#include "System.h"
#include "GPIO.h"
#include "PWM.h"
#include "Timer.h"


int main(void)
{
    System systemObj;
    PWM motorLeft, motorRight;
    GPIO enablePin;
    enablePin.init(&systemObj, GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_DIR_MODE_OUT);
    systemObj.init(40000000);
    motorLeft.init(&systemObj, GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_3, false, 5000);
    motorRight.init(&systemObj, GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_5, false, 5000);
    enablePin.write(true);

    float duty = 0.00f;
    //float *dutyPointer = &duty;

    bool countUp = true;
    //bool *countUpPointer = &countUp;

    while (1)
    {
       while(countUp){
           duty = duty + 0.1f;
           systemObj.delayUS(1000000);
           if(duty >= 1.00f){
               duty = 1.00f;
               countUp = false;
           }
           motorLeft.setDuty(duty);
           motorRight.setDuty(duty);
       }
       while(!countUp){
           duty = duty - 0.1f;
           systemObj.delayUS(1000000);
           if(duty <= -1.00f){
               duty = -1.00f;
              countUp = true;
           }
           motorLeft.setDuty(duty);
           motorRight.setDuty(duty);
       }
    }
}
