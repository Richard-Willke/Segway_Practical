/*
 * ADC.h
 *
 *    Author: Richard Willke
 *    Email: willke.richard@gmail.com
 */

#ifndef ADC_H_
#define ADC_H_


/*
 * stdbool.h:               Boolean definitions for the C99 standard
 * stdint.h:                Variable definitions for the C99 standard
 * System.h:                Access to current CPU clock and other functions.
 */
#include <stdbool.h>
#include <stdint.h>
#include "System.h"
// Informationen �ber die Typen an Pins, die System Controll Funktionen, die GPIO & ADC Driver Bibliothek sowie die Klasse Config.h
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "Config.h"


class ADC
{
public:
    ADC();
    virtual ~ADC();
    void init(System *sys, uint32_t base, uint32_t sampleSeq, uint32_t analogInput);
    void setHWAveraging(uint32_t averaging);
    uint32_t read();
    float readVolt();

private:
    System* ADCsys;
    uint32_t ADCbase, ADCsampleSeq, ADCanalogInput, ADCread[1], ADCaveraging;
    float ADCreadVolt;
    /*
     * The following array is needed to let the compiler know that the
     * precompiled class library needs space for its private variables.
     * Or in shorter terms: simply ignore it.
     */
    uint32_t spaceForLib[41];

};


#endif /* ADC_H_ */
