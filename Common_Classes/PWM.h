/*
 * PWM.h
 *
 *    Author: Richard Willke
 *    Email: willke.richard@gmail.com
 */

#ifndef PWM_H_
#define PWM_H_


/*
 * stdbool.h:               Boolean definitions for the C99 standard
 * stdint.h:                Variable definitions for the C99 standard
 * System.h:                Access to current CPU clock and other functions.
 */
#include <stdbool.h>
#include <stdint.h>
#include "System.h"
// Informationen �ber die Typen an Pins, die System Controll Funktionen, die GPIO & PWM Driver Bibliothek sowie die Klasse Config.h
// math.h wird ben�tigt f�r zugriff auf Funktionen wie abs()
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "Config.h"
#include "GPIO.h"
#include <math.h>


class PWM
{
public:
    PWM();
    virtual ~PWM();
    void init(System *sys,uint32_t portBase, uint32_t pin1, uint32_t pin2,
              bool invert = false, uint32_t freq = 5000);
    void setFreq(uint32_t freq);
    void setDuty(float duty);

private:
    System* pwmSys;
    uint32_t pwmBase, pwmPin1, pwmPin2, pwmPin1_output, pwmPin2_output, pwmGenerator, pwmPin1bit, pwmPin2bit, pwmPin1module, pwmPin2module, pwmClockFreq, pwmModule;
    bool pwmInvert;
    uint32_t pwmFreq, pwmLoad, pwmClockFrequency;
    float duty;

    /*
     * The following array is needed to let the compiler know that the
     * precompiled class library needs space for its private variables.
     * Or in shorter terms: simply ignore it.
     */
    uint32_t spaceForLib[92];

};

#endif /* PWM_H_ */
