#include <Arduino.h>
#include "Motor.h"
#include "Defines.h"


MotorConnectionParams mconnp0 =
{
    .IN1 =      M0_INA,
    .IN2 =      M0_INB,
    .ENCA_PIN = M0_INTERRUPT_PIN,
    .ENCA_CH =  -1,
    .ENCB_PIN = M0_DIRECTION_PIN,
    .ENCB_CH =  -1,
    .ENC_PPR =  MOTORS_PPR,
    .i =        MOTORS_GEAR_RATIO,
    .ke =       MOTORS_KE,
};

MotorControllerParams mctrlp = 
{
    .maxU = MOTORS_MAX_U,
    .moveU = MOTORS_MOVE_U,
    .maxSpeed = MOTORS_MAX_SPEED,
    .maxAccel = 9999,
    .Ts = Ts_s,
    .kp = MOTORS_PI_GAIN,
    .ki = MOTORS_PI_KI,
    .speedFilterT = 2*Ts_s,
    .maxUi = 9999
};

Motor motor0(&mconnp0, &mctrlp);

void setup()
{
    attachInterrupt(
        digitalPinToInterrupt(M0_INTERRUPT_PIN), []()
        { motor0.interruptHandler(); },
        CHANGE);
}

void loop()
{

}
