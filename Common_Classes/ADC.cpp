/*
 * ADC.cpp
 *
 *    Author: Richard Willke
 *    Email: willke.richard@gmail.com
 */


// Uncomment following #define to use the precompiled ADC library instead of
// the code in this file.
//#define USE_ADC_LIBRARY

#ifndef USE_ADC_LIBRARY


#include <ADC.h>


ADC::ADC()
{
    /*
     * Default empty constructor
     */
}

ADC::~ADC()
{
    /*
     * Default empty destructor
     */
}
void ADC::init(System* sys, uint32_t base, uint32_t sampleSeq, uint32_t analogInput) {
    //Speichere alle der init Funktion �bergebenen Variablen in der ADC Klasse
    ADCsys = sys;
    ADCbase = base;
    ADCsampleSeq = sampleSeq;
    ADCanalogInput = analogInput;

    //Schalte die FPU f�r floating point Arithmetik frei
    ADCsys->enableFPU();

    //Je nach Basis Modul; Schalte das entsprechende Modul frei
    if(ADCbase == ADC0_BASE){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    }
    else{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    }
    //Warte bis die Basis freigeschaltet ist
    ADCsys->delayCycles(5);

    //Je nach gew�hlten Chanel, schalte den dem Chanel zugerh�rigen Pin frei und klassifiziere ihn als ADC Pin
    switch (ADCanalogInput) {
    case(ADC_CTL_CH0):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
        break;
    case(ADC_CTL_CH1):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);
        break;
    case(ADC_CTL_CH2):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);
        break;
    case(ADC_CTL_CH3):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);
        break;
    case(ADC_CTL_CH4):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3);
        break;
    case(ADC_CTL_CH5):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_2);
        break;
    case(ADC_CTL_CH6):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);
        break;
    case(ADC_CTL_CH7):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);
        break;
    case(ADC_CTL_CH8):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_5);
        break;
    case(ADC_CTL_CH9):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_4);
        break;
    case(ADC_CTL_CH10):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_4);
        break;
    case(ADC_CTL_CH11):
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_5);
        break;
    default:
        ADCsys->error(ADCWrongConfig, &ADCanalogInput);
        break;
    }
    //Warte bis die Peripherals freigeschaltet und die Pins konfiguriert sind
    ADCsys->delayCycles(5);

    //Wir Konfigurieren den ADC Sample Sequencer und seine Steps
    ADCSequenceConfigure(ADCbase, ADCsampleSeq, ADC_TRIGGER_PROCESSOR, ADCsampleSeq);

    ADCSequenceStepConfigure(ADCbase, ADCsampleSeq, 0, ADCanalogInput);

    //Der Sequencer wird ab dem Punkt freigeschaltet. Die Sequence l�uft ab dem Punkt ab.
    ADCSequenceEnable(ADCbase, ADCsampleSeq);
}

void ADC::setHWAveraging(uint32_t averaging) {
    //Mit dieser Methode berechnen wir den Mittelwert unserer gemessenen Werte

    //F�r eine Anzahl an Messwerten, welche zwischen 64 und 2 liegt, sowie ein vielfaches von 2 ist, berechnen wir den Mittlewert.
    //Ansonsten geben wir eine Errormeldung heraus
    if ((averaging % 2 != 0) | (averaging < 1) | (averaging > 64)) {
        ADCsys->error(ADCWrongConfig, &averaging);
    }
    else if ((1 <= averaging <= 64) & (averaging % 2 == 0)) {
        ADCHardwareOversampleConfigure(ADCbase, ADCaveraging);
    }
    else {
        ADCsys->error(ADCWrongConfig, &averaging);
    }
}

uint32_t ADC::read() {
    //in dieser Methode wollen wir die Werte an unserem ADC Pin auslesen und in der Klasse speichern

    //Disable die Interrups beim lesen
    IntMasterDisable();
    //Cleare die vorherigen gespeicherten Werte
    ADCIntClear(ADCbase, ADCsampleSeq);
    //Sammel die Daten
    ADCProcessorTrigger(ADCbase, ADCsampleSeq);
    ADCsys->delayCycles(5);
    ADCSequenceDataGet(ADCbase, ADCsampleSeq, ADCread);
    //Erm�gliche dem System wieder Interrups zu machen
    IntMasterEnable();
    //Gebe den gelesenen Wert zur�ck
    return ADCread[0];
}

float ADC::readVolt() {
    //Diese Methode berechnet mit Hilfe der read() Methode die Spannung in Volt. Diese reicht von 0V bis 3.3V

    //Rufe die Read Funktion auf und rechne diese in Volt um
    ADCreadVolt = (3.3f / 4095.0f) * read();
    //Gebe den Wert der Spannung zur�ck
    return ADCreadVolt;
}

#endif
