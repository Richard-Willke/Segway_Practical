/*
 * Steering.h
 *
 *    Author: Richard Willke
 *    Email: willke.richard@gmail.com
 */

#ifndef STEERING_H_
#define STEERING_H_

#include <stdint.h>
#include "ADC.h"
#include "System.h"

class Steering
{
public:
    Steering();
    ~Steering();
    void init(System* sys, uint32_t base, uint32_t sampleSeq, uint32_t analogInput);
    float scaling(float scale);
    float getValue();

private:
    System* steeringSys;
    uint32_t steeringBase, steeringSequencer, steeringAnalogIn;
    ADC steeringInput;
    float voltage_offset, motor_duty, voltage_input, amplitude_voltage, steeringValue, voltage_max, voltage_min;
};

#endif /* STEERING_H_ */
