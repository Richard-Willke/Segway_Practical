/*
 * Timer.cpp
 *
 *    Author: Richard Willke
 *    Email: willke.richard@gmail.com
 */


// Uncomment following #define to use the precompiled Timer library instead of
// the code in this file.
// #define USE_TIMER_LIBRARY

#ifndef USE_TIMER_LIBRARY


#include <Timer.h>


Timer::Timer()
{
    /*
     * Default empty constructor
     */
}

Timer::~Timer()
{
    /*
     * Default empty destructor
     */
}

void Timer::init(System* sys, uint32_t base, void (*ISR)(void), uint32_t freq) {
    //Speichere die der init Funktion �bergebenen Vabriablen in der Timer Klasse
    timerSys = sys;
    timerBase = base;
    timerFreq = freq;
    timerISR = ISR;
    timerClock = timerSys->getClockFreq();

    //Je nach Basis Modul wird ein Basis Timer Modul von 0 bis 5 freigeschaltet
    switch (timerBase) {
    case TIMER0_BASE:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
        break;
    case TIMER1_BASE:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
        break;
    case TIMER2_BASE:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
        break;
    case TIMER3_BASE:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
        break;
    case TIMER4_BASE:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);
        break;
    case TIMER5_BASE:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
        break;
    default:
        timerSys -> error(TimerWrongConfig, &timerBase, &timerFreq, &timerISR);
    }
    //Warte bis das Basismodul freigeschaltet ist
    timerSys->delayCycles(5);
    //Die Timer clock bezieht ihre Ressourcen aus der System clock
    //TimerClockSourceSet(timerBase, TIMER_CLOCK_SYSTEM);
    //Konfiguriere den Timer, dass er periodisch z�hlt
    TimerConfigure(timerBase, TIMER_CFG_PERIODIC);
    //stoppe den Timer. Er soll noch nicht los laufen
    stop();
    //setze Frequenz mittels der setFreq() Methode
    setFreq(timerFreq);
    //Gebe dem System bescheid, dass es Interrups machen kann
    TimerIntEnable(timerBase, TIMER_TIMA_TIMEOUT);
    //
    TimerIntRegister(timerBase, TIMER_A, timerISR);
}

void Timer::start() {
    //Starte den Timer indem man ihn enabled
    TimerIntEnable(timerBase, TIMER_TIMA_TIMEOUT);
    TimerEnable(timerBase, TIMER_A);
}

void Timer::stop() {
    //Stoppe den Timer indem man ihn disabled
    TimerIntDisable(timerBase, TIMER_TIMA_TIMEOUT);
    TimerDisable(timerBase, TIMER_A);
}

void Timer::clearInterruptFlag(){
    //Cleare alle Interrupts unseres Timer Moduls
    TimerIntClear(timerBase, TIMER_TIMA_TIMEOUT);
}

uint32_t Timer::getFreq() {
    //Geb die Frequenz, welche in unserer Klasse gespeichert ist, zur�ck
    return timerFreq;
}

void Timer::setFreq(uint32_t freq) {
    //speichere die eingegebene Frequenz als die neue Frequenz unseres Timers
    timerFreq = freq;
    //Ist die Frequenz ungleich null, berechnen wir den Load Value unseres Timers bis zu dem dieser hoch z�hlt bzw. von diesem herunterz�hlt
    if (timerFreq != 0){
        timerLoadValue = timerClock/timerFreq -1;
        TimerLoadSet(timerBase, TIMER_A, timerLoadValue);
    }
    //Ist die eingegebene Frequenz gleich null, so wird die Perdiodendauer auch auf null gesetzt und der Timer wird gestoppt
    else {
        timerPeriodUS = 0;
        timerFreq = 0;
        stop();
    }
}

uint32_t Timer::getPeriodUS() {
    //geb die in der Klasse gespeicherte Periodendauer in mykrosekunden zur�ck
    return timerPeriodUS;
}

void Timer::setPeriodUS(uint32_t periodUS) {
    //speichere die gew�nsche Periodendauer in mykrosekunden in unserer Timer Klasse
    timerPeriodUS = periodUS;
    //Ist die Periodendauer gleich null, so wird der Timer gestoppt und die Frequenz des Timers auf null Hertz gesetzt
    if (timerPeriodUS == 0) {
        timerFreq = 0;
        stop();
    }
    //Ist die Periodendauer ungleich null, so berechnen wir zu erst die Frequenz und den Load Value .
    //Letzteres ben�tigen wir um den Load Value der Periodendauer anzupassen
    else {
        timerFreq = (10 ^ 6) / (timerPeriodUS);
        timerLoadValue = timerPeriodUS*(timerClock/(10^6)) -1;
        TimerLoadSet(timerBase, TIMER_A, timerLoadValue);
    }
}

#endif
