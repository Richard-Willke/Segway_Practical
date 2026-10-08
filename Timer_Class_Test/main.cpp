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
#include "Timer.h"


/*
 * Template for ISR on timer x:
 * void ISRx()
 * {
 *     //Clear interrupt flag
 *     timerx.clearInterruptFlag();
 *
 *     // Your code
 * }
 */

void ISR1(), ISR2(), ISR3();
bool direction = 0, color = 0, controll = 1;
Timer timer1, timer2, timer3;
uint32_t periodA = 10000000;
uint32_t freqc=2;
System timerSystemObj;
GPIO red, blue;
int i = 1;

int main(void)
{
    timerSystemObj.init(40000000);
    red.init(&timerSystemObj, GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_DIR_MODE_OUT);
    blue.init(&timerSystemObj, GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_DIR_MODE_OUT);

    timer1.init(&timerSystemObj, TIMER1_BASE, ISR1);
    timer1.setPeriodUS(periodA);

    timer2.init(&timerSystemObj, TIMER2_BASE, ISR2);

    timer3.init(&timerSystemObj, TIMER3_BASE, ISR3);

    timer1.start();

    while (1)
    {

    }
}

void ISR1(){
        timer1.clearInterruptFlag();

        if(direction==1){
            i += 1;
            if(i>=5){
                i=5;
                direction = 0;
                timer3.start(); //red = 3
                timer2.stop(); //blue = 2
                blue.write(0);
            }
            timer2.setFreq(i * 2);
            timer3.setFreq(i * 2);
        }
        if(direction==0){
            i += -1;
            if(i<=0){
                i=1;
                direction = 1;
                timer2.start();
                timer3.stop();
                red.write(0);
            }
            timer2.setFreq(i * 2);
            timer3.setFreq(i * 2);
        }
}

void ISR2(){
        timer2.clearInterruptFlag();
        blue.write(!blue.read());
}

void ISR3(){
      timer3.clearInterruptFlag();
      red.write(!red.read());
}

