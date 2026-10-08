/*
 * Segway.cpp
 *
 *    Author: Henri Hornburg
 *     Email: henri.hornburg@student.kit.edu
 * Co-author: Max Zuidberg
 *     Email: m.zuidberg@icloud.com
 *
 * The segway class contains all the code needed drive a segway.
 */

#include <Segway.h>

Segway::Segway()
{
    /*
     * Default empty constructor
     */
}

Segway::~Segway()
{
    /*
     * Default empty destructor
     */
}

void Segway::init(System *sys)
{
    /*  Initializes all the objects used in the segway class.
     *  Needs to be called before any components can be used
     *
     * sys: Pointer to the current System instance. Needed to get CPU clock
     *      frequency.
     */

    // Create private reference to the given System object.
    this->sys = sys;

    // Initialize all objects with the given parameters and the parameters from
    // the Config header file.
    leftMotor.init(sys,
                   CFG_LM_PORT,
                   CFG_LM_PIN1,
                   CFG_LM_PIN2,
                   CFG_PWM_INVERT,
                   CFG_LM_FREQ);
    rightMotor.init(sys,
                    CFG_RM_PORT,
                    CFG_RM_PIN1,
                    CFG_RM_PIN2,
                    CFG_PWM_INVERT,
                    CFG_RM_FREQ);
    enableMotors.init(sys,
                      CFG_EM_PORT,
                      CFG_EM_PIN,
                      CFG_EM_DIR);
    footSwitch.init(sys,
                    CFG_FS_PORT,
                    CFG_FS_PIN,
                    CFG_FS_DIR,
                    CFG_FS_PULLUP);
    steering.init(sys,
                  CFG_STEERING_BASE,
                  CFG_STEERING_SSEQ,
                  CFG_STEERING_AIN);
    controller.init(sys,
                    CFG_CTLR_MAX_SPEED);
    batteryVoltage.init(sys,
                        CFG_BATT_BASE,
                        CFG_BATT_SSEQ,
                        CFG_BATT_AIN);
    sensor.init(sys,
                CFG_SENSOR_I2C_MODULE,
                CFG_SENSOR_ADRESSBIT);

    // Configure sensor orientation
    sensor.setWheelAxis(CFG_SENSOR_WHEEL_AXIS);
    sensor.setHorAxis(CFG_SENSOR_HOR_AXIS);
    sensor.accelHorInvertSign(CFG_SENSOR_INVERT_HOR);
    sensor.accelVerInvertSign(CFG_SENSOR_INVERT_VER);
    sensor.angleRateInvertSign(CFG_SENSOR_INVERT_ANGLE_RATE);

    // This Enable Motors Pin is only needed for compatibility with the TivSeg
    // Hardware. It is not used at any other place in the code.
    enableMotors.write(CFG_EM_ACTIVE_STATE);

    // We use floats, therefore we want to profit from the FPU.
    sys->enableFPU();

    // Initializing done, segway is ready but not active yet.
    standby = true;
}

void Segway::update(){
        //Überprüfe ob der Fahrer den Foot Switch gedrückt hat
        footSwitchPressed = (footSwitch.read() == CFG_FS_ACTIVE_STATE);

        //Ist der Foot Switch gedrückt, so wird das Segway aus dem Standby geweckt. Die Duty wird berechnet und den Motoren übergeben
        if (footSwitchPressed){
                standby = false;
                //Hole mit dem Steering Objekt den Value, bzw Duty für die Motoren
                steeringValue = steering.getValue();
                //Mit dem Sensor Objekt werden die Neigung des Lenkers, sowie Beschleunigung in horizontale und vertikale Richtung berechnet
                angleRateRad = sensor.getAngleRate() * 3.14159265358979f / 180.0f;
                accelHor = sensor.getAccelHor();
                accelVer = sensor.getAccelVer();
                //Das Controller Objekt übernimmt die Messerte des Sensors und Updatet damit dessen Values
                controller.updateValuesRad(steeringValue, angleRateRad, accelHor, accelVer);
                //Hole die Values für die Duty aus dem Controller Objekt
                motorDutyLeft = controller.getLeftSpeed();
                motorDutyRight = controller.getRightSpeed();
                //Übergebe den Motoren die Werte für die Duty, um dessen Geschwindigkeiten zu regeln
                leftMotor.setDuty(motorDutyLeft);
                rightMotor.setDuty(motorDutyRight);

                sys->setDebugVal("Steering_Value_[%]", steeringValue * 100);
                sys->setDebugVal("Left_Speed_[%]" , motorDutyLeft * 100);
                sys->setDebugVal("Right_Speed_[%]" , motorDutyRight * 100);

            }
        else{
                //Ist die Person nicht auf dem Segway, geht dieser in den Standbymodus. Die Motoren werden auf eine Duty von Null gesetzt.
                //Der Segway wird zum Stehen gebracht
                controller.resetSpeeds();
                leftMotor.setDuty(0);
                rightMotor.setDuty(0);
                standby = true;
            }
        //Setze das Update Flag auf true. Es werden regelmäßig Updates durchgeführt um die Geschwindigkeiten der Motoren zu rekalibieren.
        updateFlag = true;
}

void Segway::backgroundTasks()
{
    voltage = 0;
    //Ist das Update Flag auf true, überprüft die Spannungsüberwachung regelmäßig die Spannung der Batterie
        if (updateFlag){
            if (batteryFlag){
                grenzeTimeout--;
            }
            else{
                grenzeTimeout = (CFG_BATT_TIMEOUT * CFG_CTLR_UPDATE_FREQ);
            }
            //Setze nach Ausführen einer der cases beide Flags wieder auf false
            updateFlag = false;
            batteryFlag = false;
        }
        //Ist das Battery Flag auf false, so wird die Spannung der Batterie vom ADC Objekt gemessen. Ist diese kleiner als die minimale Spannung, so setze das Battery Flag auf true
        if (!batteryFlag){
            voltage = batteryVoltage.readVolt();
            sys->delayCycles(5);
            if (voltage < (CFG_BATT_MIN/10)){
                batteryFlag = true;
            }
        }
        //Ist der Timeout gleich 0, so Stoppe die Motoren und gebe die Errormeldung zurück, dass die Spannung zu niedrig ist
        if (grenzeTimeout == 0){
            controller.setMaxSpeed(0);
            sys->error(BatteryLow, &voltage);
        }
}
