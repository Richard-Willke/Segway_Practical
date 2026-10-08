/*
 * PWM.cpp
 *
 *    Author: Richard Willke
 *    Email: willke.richard@gmail.com
 */


// Uncomment following #define to use the precompiled PWM library instead of
// the code in this file.
// #define USE_PWM_LIBRARY

#ifndef USE_PWM_LIBRARY


#include <PWM.h>


PWM::PWM()
{
    /*
     * Default empty constructor
     */
}

PWM::~PWM()
{
    /*
     * Default empty destructor
     */
}

void PWM::init(System* sys, uint32_t portBase, uint32_t pin1, uint32_t pin2, bool invert, uint32_t freq) {
    //Speichere die der Init Funktion �bergebenen Variablen im Objekt
    pwmSys = sys;
    pwmBase = portBase;
    pwmPin1 = pin1;
    pwmPin2 = pin2;
    pwmInvert = invert;
    pwmFreq = freq;

    //Berechne die Frequenz der PWM Clock
    pwmClockFreq = (pwmSys->getClockFreq()/pwmSys->getPWMClockDiv());
    //Schalte die FPU frei f�r Flie�kommazahlrechnungen
    pwmSys->enableFPU();

    //Schalte je nach g�ltiger Basis, Pin Kombination die jeweiligen Peripherals (PWM Port und Pin Port) frei. Schreibe ihnen einen Generator sowie die Addressen der jeweiligen Pin Bits, Outputs und Module zu
    switch (pwmBase | pwmPin1 | pwmPin2){
    case(GPIO_PORTB_BASE | GPIO_PIN_6 | GPIO_PIN_7):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        pwmGenerator = PWM_GEN_0;
        pwmModule = PWM0_BASE;
        pwmPin1_output = PWM_OUT_0;
        pwmPin2_output = PWM_OUT_1;
        pwmPin1bit = PWM_OUT_0_BIT;
        pwmPin2bit = PWM_OUT_1_BIT;
        pwmPin1module = GPIO_PB6_M0PWM0;
        pwmPin2module = GPIO_PB7_M0PWM1;
        break;
    case(GPIO_PORTB_BASE | GPIO_PIN_4 | GPIO_PIN_5):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        pwmGenerator = PWM_GEN_1;
        pwmModule = PWM0_BASE;
        pwmPin1_output = PWM_OUT_2;
        pwmPin2_output = PWM_OUT_3;
        pwmPin1bit = PWM_OUT_2_BIT;
        pwmPin2bit = PWM_OUT_3_BIT;
        pwmPin1module = GPIO_PB4_M0PWM2;
        pwmPin2module = GPIO_PB5_M0PWM3;
        break;
    case(GPIO_PORTE_BASE | GPIO_PIN_4 | GPIO_PIN_5):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        pwmGenerator = PWM_GEN_2;
        pwmModule = PWM0_BASE;
        pwmPin1_output = PWM_OUT_4;
        pwmPin2_output = PWM_OUT_5;
        pwmPin1bit = PWM_OUT_4_BIT;
        pwmPin2bit = PWM_OUT_5_BIT;
        pwmPin1module = GPIO_PE4_M0PWM4;
        pwmPin2module = GPIO_PE5_M0PWM5;
        break;
    case(GPIO_PORTC_BASE | GPIO_PIN_4 | GPIO_PIN_5):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
        pwmGenerator = PWM_GEN_3;
        pwmModule = PWM0_BASE;
        pwmPin1_output = PWM_OUT_6;
        pwmPin2_output = PWM_OUT_7;
        pwmPin1bit = PWM_OUT_6_BIT;
        pwmPin2bit = PWM_OUT_7_BIT;
        pwmPin1module = GPIO_PC4_M0PWM6;
        pwmPin2module = GPIO_PC5_M0PWM7;
        break;
    case(GPIO_PORTD_BASE | GPIO_PIN_0 | GPIO_PIN_1):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        pwmGenerator = PWM_GEN_0;
        pwmModule = PWM1_BASE;
        pwmPin1_output = PWM_OUT_0;
        pwmPin2_output = PWM_OUT_1;
        pwmPin1bit = PWM_OUT_0_BIT;
        pwmPin2bit = PWM_OUT_1_BIT;
        pwmPin1module = GPIO_PD0_M1PWM0;
        pwmPin2module = GPIO_PD1_M1PWM1;
        break;
    case(GPIO_PORTA_BASE | GPIO_PIN_6 | GPIO_PIN_7):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        pwmGenerator = PWM_GEN_1;
        pwmModule = PWM1_BASE;
        pwmPin1_output = PWM_OUT_2;
        pwmPin2_output = PWM_OUT_3;
        pwmPin1bit = PWM_OUT_2_BIT;
        pwmPin2bit = PWM_OUT_3_BIT;
        pwmPin1module = GPIO_PA6_M1PWM2;
        pwmPin2module = GPIO_PA7_M1PWM3;
        break;
    case(GPIO_PORTF_BASE | GPIO_PIN_0 | GPIO_PIN_1):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        pwmGenerator = PWM_GEN_2;
        pwmModule = PWM1_BASE;
        pwmPin1_output = PWM_OUT_4;
        pwmPin2_output = PWM_OUT_5;
        pwmPin1bit = PWM_OUT_4_BIT;
        pwmPin2bit = PWM_OUT_5_BIT;
        pwmPin1module = GPIO_PF0_M1PWM4;
        pwmPin2module = GPIO_PF1_M1PWM5;
        break;
    case(GPIO_PORTF_BASE | GPIO_PIN_2 | GPIO_PIN_3):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        pwmGenerator = PWM_GEN_3;
        pwmModule = PWM1_BASE;
        pwmPin1_output = PWM_OUT_6;
        pwmPin2_output = PWM_OUT_7;
        pwmPin1bit = PWM_OUT_6_BIT;
        pwmPin2bit = PWM_OUT_7_BIT;
        pwmPin1module = GPIO_PF2_M1PWM6;
        pwmPin2module = GPIO_PF3_M1PWM7;
        break;
    default:
        pwmSys->error(PWMWrongPins, &pwmBase, &pwmPin1, &pwmPin2);
        break;
    }
    //Warte bis die Peripherals freigeschaltet sind
    pwmSys->delayCycles(5);
    //Definiere die gew�hlten Pins als Typ PWM
    GPIOPinTypePWM(pwmBase, pwmPin1);
    GPIOPinTypePWM(pwmBase, pwmPin2);

    //Konfiguriere die Pins
    GPIOPinConfigure(pwmPin1module);
    GPIOPinConfigure(pwmPin2module);

    //Konfiguriere den Generator
    PWMGenConfigure(pwmModule, pwmGenerator, PWM_GEN_MODE_DOWN);

    //Setze beide Outputs auf False, damit der Segway nicht w�hrend dem initialisieren losf�hrt
    PWMOutputState(pwmModule, pwmPin1bit, false);
    PWMOutputState(pwmModule, pwmPin2bit, false);

    //Invertiere die Outputs (abh�ngig ob mit Segway oder mini Seg gearbeitet wird)
    PWMOutputInvert(pwmModule, pwmPin1bit, pwmInvert);
    PWMOutputInvert(pwmModule, pwmPin2bit, pwmInvert);

    //Rufe die setFreq() Methode auf
    setFreq(pwmFreq);
    PWMOutputUpdateMode(pwmModule, pwmPin1bit|pwmPin2bit, PWM_OUTPUT_MODE_SYNC_LOCAL);
    //Schalte den Generator frei
    PWMGenEnable(pwmModule, pwmGenerator);
}

void PWM::setFreq(uint32_t freq) {
    //Berechne den Load value von dem aus der Generator runter z�hlt
    pwmLoad = pwmClockFreq / freq;
    //Setze die Periode des Generators
    PWMGenPeriodSet(pwmModule, pwmGenerator, pwmLoad);
}

void PWM::setDuty(float duty) {
    //die minimale Aufl�sung unserer Pulsweite. Unterschreitet unsere Duty diese, sollten motoren stehen beliben
    float minimum_resolution = (1.00f / pwmLoad);

    //beide Motoren bleiben stehen
    if (abs(duty) < minimum_resolution) {
        //PWMPulseWidthSet(pwmBase, pwmPin1_output, PWMGenPeriodGet(pwmBase, pwmGenerator));
       // PWMPulseWidthSet(pwmBase, pwmPin2_output, PWMGenPeriodGet(pwmBase, pwmGenerator));
        PWMOutputState(pwmModule, pwmPin1bit, false);
        PWMOutputState(pwmModule, pwmPin2bit, false);
    }
    //Duty ist oberhalb der m�glichen Grenze. Gib eine Errormeldung zur�ck
    else if (abs(duty) > 1) {
        pwmSys-> error(PWMDutyOutOfRange, &duty);
    }
    //Segway f�hrt nach vorne, da duty innerhalb der Grenzen positiv ist
    else if((duty > 0) && (minimum_resolution <= abs(duty))){
        PWMPulseWidthSet(pwmModule, pwmPin1_output, static_cast<int>(abs(duty) * pwmLoad));
        PWMPulseWidthSet(pwmModule, pwmPin2_output, static_cast<int>(abs(duty) * pwmLoad));
        PWMOutputState(pwmModule, pwmPin1bit, true);
        PWMOutputState(pwmModule, pwmPin2bit, false);
    }
    //duty ist innerhalb der Grenzen negativ. Segway f�hrt nach hinten
    else if((duty < 0) && (minimum_resolution <= abs(duty))) {
        PWMPulseWidthSet(pwmModule, pwmPin1_output, static_cast<int>(abs(duty) * pwmLoad));
        PWMPulseWidthSet(pwmModule, pwmPin2_output, static_cast<int>(abs(duty) * pwmLoad));
        PWMOutputState(pwmModule, pwmPin1bit, false);
        PWMOutputState(pwmModule, pwmPin2bit, true);
    }
}

#endif
