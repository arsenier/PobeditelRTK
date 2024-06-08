#include <Arduino.h>
#include "Motor.h"
#include "Defines.h"
#include "RHex.h"
#include "RHex_modules.h"


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

sfix forward_vel;

void setup()
{
    attachInterrupt(
        digitalPinToInterrupt(M0_INTERRUPT_PIN), []()
        { motor0.interruptHandler(); },
        CHANGE);

    Serial.begin(9600);

    for(auto i = sfix(-M_PI); i < sfix(M_PI); i = i + sfix(0.1))
    {
        // Serial.print(i.asFloat());
        // Serial.print(" ");
        // Serial.println(Ffull(modc(i + sfix(M_PI), sfix(2*M_PI)), sfix(2*M_PI), sfix(1*M_PI), sfix(1), sfix(1)).asFloat());
    }
}

void loop()
{
    forward_vel = sfix(10);

    uint32_t last = millis();
    ALFA_update();
    uint32_t now = millis();
    Serial.println(last);
    Serial.println(now);
    Serial.println(leg_angles[0].get().asFloat());
}
