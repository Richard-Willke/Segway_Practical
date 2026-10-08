/*
 * Steering.cpp
 *
 *    Author: Richard Willke
 *    Email: willke.richard@gmail.com
 */

#include "Steering.h"


Steering::Steering()
{
    /*
     * Default empty constructor
     */
}

Steering::~Steering()
{
    /*
     * Default empty destructor
     */
}

void Steering::init(System* sys, uint32_t base, uint32_t sampleSeq, uint32_t analogInput){
    //Speichere alle der Init Funktion �bergebenen Variablen in der Steering Klasse
    steeringSys = sys;
    steeringBase = base;
    steeringSequencer = sampleSeq;
    steeringAnalogIn = analogInput;

    //Schalte die FPU frei um mit Flie�kommazahlen rechnen zu k�nnen
    steeringSys->enableFPU();
    //Initialisiere das ADC objekt innerhalb der Steering Klasse
    steeringInput.init(steeringSys, steeringBase, steeringSequencer, steeringAnalogIn);
    //Gebe dem ADC Objekt bescheid, dass man aus 64 Werten den Mittelwert berechnen will
    steeringInput.setHWAveraging(64);
    //Lese die Spannung am Pin des ADC Objektes aus. Diese ist in unserem Fall der Offset unserer Spannung.
    //Diese ben�tigen wir sp�ter f�r die Berechnung der Duty. Mehr dazu in der Scaling Methode und der PWM Klasse.
    voltage_offset = steeringInput.read();
}

float Steering::scaling(float scale)
{
    //Definiere die obere und untere Grenze der Spannung. Diese steht im Bezug zum Offset
    voltage_max = 0.2f;
    voltage_min = -0.2f;
    amplitude_voltage = scale;

    //Ist die Amplitude unserer Spannung gr��er null und kleiner der maximalen Spannung, wird die Duty aus
    //dem Quotienten der Amplitude zur Maximalen Spanung berechnet
    if ((amplitude_voltage >= 0) && (amplitude_voltage <= voltage_max))
    {
        motor_duty = amplitude_voltage / voltage_max;
        return motor_duty;
    }
    //�hnlich zum vorherigen Fall, wird die Duty aus dem Quotienten von Aplitude zu minimalen Spannung berechnet.
    //Hier ist die Amplitude jedoch innerhalb der Grenzen 0 und voltage_min
    else if ((amplitude_voltage < 0) && (amplitude_voltage >= voltage_min)) {
        motor_duty = -1 * abs(amplitude_voltage / voltage_min);
        return motor_duty;
    }
    //�berschreitet die Amplitude das Maximum, bzw. unterschreitet das Minimum, so wird die Duty zu 1, bzw. -1, gesetzt
    //Zudem wird der Maximum Wert der Amplitude �berschrieben mit dem neuen Wert der Amplitude
    else if ((amplitude_voltage > voltage_max) | (amplitude_voltage < voltage_min)) {
        if ((amplitude_voltage >= 0) && (amplitude_voltage > voltage_max)) {
            voltage_max = amplitude_voltage;
            motor_duty = 1;
            return motor_duty;
        }
        else if ((amplitude_voltage < 0) && (amplitude_voltage < voltage_min)) {
            voltage_min = amplitude_voltage;
            motor_duty = -1;
            return motor_duty;
        }
    }
   return motor_duty;
}

float Steering::getValue()
{
        //Lese den ADC Pin. Speichere den Messwert
        voltage_input = steeringInput.read();
        //Subtrahiere den Offset und berechne die Amplitude
        amplitude_voltage = (voltage_input - voltage_offset);
        //Berechne die Duty des Motors mit der Amplitude der Spannung als Eingabewert.
        steeringValue=scaling(amplitude_voltage);
        //Plotte die Ergebnisse in der Arduino IDE
        steeringSys->setDebugVal("Motor_Duty_[%]", steeringValue * 100);
        steeringSys->setDebugVal("Amplitude_Voltage_[%]", amplitude_voltage * 100);
        //Gebe den Value, bzw. Duty, zur�ck
    return steeringValue;
}
