#include <Arduino.h>
#include "Motor.h"
#include "Defines.h"
#include "RHex.h"
#include "RHex_modules.h"

#include "crc8.h"
#include "Enotik.h"
#include "Enotik_master.h"
#include "Enotik_message_format.h"
#include "parser.h"
#include "Periph.h"



MotorConnectionParams mconnp0 =
{
    .IN1 =      M0_INA,
    .IN2 =      M0_INB,
    .PWM =      M0_PWM,
    .ENCA_PIN = M0_INTERRUPT_PIN,
    .ENCA_CH =  -1,
    .ENCB_PIN = M0_DIRECTION_PIN,
    .ENCB_CH =  -1,
    .ENC_PPR =  int(5.0/6*MOTORS_PPR),
    .i =        MOTORS_GEAR_RATIO,
    .ke =       MOTORS_KE,
    .ENC_DIR =  -1
};
MotorConnectionParams mconnp1 =
{
    .IN1 =      M1_INA,
    .IN2 =      M1_INB,
    .PWM =      M1_PWM,
    .ENCA_PIN = M1_INTERRUPT_PIN,
    .ENCA_CH =  -1,
    .ENCB_PIN = M1_DIRECTION_PIN,
    .ENCB_CH =  -1,
    .ENC_PPR =  MOTORS_PPR,
    .i =        MOTORS_GEAR_RATIO,
    .ke =       MOTORS_KE,
    .ENC_DIR =  -1
};
MotorConnectionParams mconnp2 =
{
    .IN1 =      M2_INA,
    .IN2 =      M2_INB,
    .PWM =      M2_PWM,
    .ENCA_PIN = M2_INTERRUPT_PIN,
    .ENCA_CH =  -1,
    .ENCB_PIN = M2_DIRECTION_PIN,
    .ENCB_CH =  -1,
    .ENC_PPR =  MOTORS_PPR,
    .i =        MOTORS_GEAR_RATIO,
    .ke =       MOTORS_KE,
    .ENC_DIR =  1
};
MotorConnectionParams mconnp3 =
{
    .IN1 =      M3_INA,
    .IN2 =      M3_INB,
    .PWM =      M3_PWM,
    .ENCA_PIN = M3_INTERRUPT_PIN,
    .ENCA_CH =  -1,
    .ENCB_PIN = M3_DIRECTION_PIN,
    .ENCB_CH =  -1,
    .ENC_PPR =  MOTORS_PPR,
    .i =        MOTORS_GEAR_RATIO,
    .ke =       MOTORS_KE,
    .ENC_DIR =  -1
};
MotorConnectionParams mconnp4 =
{
    .IN1 =      M4_INA,
    .IN2 =      M4_INB,
    .PWM =      M4_PWM,
    .ENCA_PIN = M4_INTERRUPT_PIN,
    .ENCA_CH =  -1,
    .ENCB_PIN = M4_DIRECTION_PIN,
    .ENCB_CH =  -1,
    .ENC_PPR =  MOTORS_PPR,
    .i =        MOTORS_GEAR_RATIO,
    .ke =       MOTORS_KE,
    .ENC_DIR =  1
};
MotorConnectionParams mconnp5 =
{
    .IN1 =      M5_INA,
    .IN2 =      M5_INB,
    .PWM =      M5_PWM,
    .ENCA_PIN = M5_INTERRUPT_PIN,
    .ENCA_CH =  -1,
    .ENCB_PIN = M5_DIRECTION_PIN,
    .ENCB_CH =  -1,
    .ENC_PPR =  MOTORS_PPR,
    .i =        MOTORS_GEAR_RATIO,
    .ke =       MOTORS_KE,
    .ENC_DIR =  -1
};

MotorControllerParams mctrlp = 
{
    .maxU = MOTORS_MAX_U,
    .maxUuse = MOTORS_MAX_U_USAGE,
    .moveU = MOTORS_MOVE_U,
    .maxSpeed = MOTORS_MAX_SPEED,
    .maxAccel = 9999,
    .Ts = Ts_s,
    .kp = MOTORS_PI_GAIN,
    .ki = MOTORS_PI_KI,
    .speedFilterT = 2*Ts_s,
    .maxUi = 9999
};

Motor motors[] = 
{
    Motor(&mconnp0, &mctrlp),
    Motor(&mconnp1, &mctrlp),
    Motor(&mconnp2, &mctrlp),
    Motor(&mconnp3, &mctrlp),
    Motor(&mconnp4, &mctrlp),
    Motor(&mconnp5, &mctrlp),
};

Updatable *input[] =
{

};

void INPUT_update()
{
    for(auto i : input)
    {
        i->update();
    }
}

Updatable *output[] =
{
    &motors[0],
    &motors[1],
    &motors[2],
    &motors[3],
    &motors[4],
    &motors[5],
};

void OUTPUT_update()
{
    for(auto q : output)
    {
        q->update();
    }
}

sfix rx_forward_vel;
sfix rx_ang_vel;

sfix forward_vel;
sfix ang_vel;

void drive_callback(byte v1, byte v2);

Timer<uint32_t> timer;
Assertion<uint32_t> assertion;

void setup()
{
    attachInterrupt(
        digitalPinToInterrupt(M0_INTERRUPT_PIN), []()
        { motors[0].interruptHandler(); },
        CHANGE);
    attachInterrupt(
        digitalPinToInterrupt(M1_INTERRUPT_PIN), []()
        { motors[1].interruptHandler(); },
        CHANGE);
    attachInterrupt(
        digitalPinToInterrupt(M2_INTERRUPT_PIN), []()
        { motors[2].interruptHandler(); },
        CHANGE);
    attachInterrupt(
        digitalPinToInterrupt(M3_INTERRUPT_PIN), []()
        { motors[3].interruptHandler(); },
        CHANGE);
    attachInterrupt(
        digitalPinToInterrupt(M4_INTERRUPT_PIN), []()
        { motors[4].interruptHandler(); },
        CHANGE);
    attachInterrupt(
        digitalPinToInterrupt(M5_INTERRUPT_PIN), []()
        { motors[5].interruptHandler(); },
        CHANGE);

    Serial.begin(9600);
    Serial3.begin(9600);

    Enotik.init_slave(0x01);
    Enotik.bind(2, drive_callback);

    timer.trigger(4000);
}

enum State
{
    sleep,
    forw,
    rotate,
};

State state = sleep;

void drive_callback(byte v1, byte v2)
{
    Serial.println("1111111");
    rx_forward_vel = int8_t(v1);
    rx_ang_vel = int8_t(v2);
}

uint32_t time_keeper = 0;
uint32_t time_delta;

void loop()
{
    while(micros() < time_keeper + Ts_us);
    time_delta = micros() - time_keeper;
    time_keeper = micros();

    // uint32_t last = millis();

    // Enotik.work();
    // // APC READ
    rx_forward_vel = min(millis()/4000, 4);
    // Serial.println(state);
    // Serial.println(rx_ang_vel);


    switch (state)
    {
    case sleep:
        select_module(&stand_up);
        forward_vel = 0;
        ang_vel = 0;


        if(rx_forward_vel)
        {
            state = forw;
        }
        else if(rx_ang_vel)
        {
            state = rotate;
            ang_vel = rx_ang_vel;
        }
        break;
    case forw:
        select_module(&move_forward);
        forward_vel = rx_forward_vel;
        ang_vel = 0;
        // if(fabs(leg_phase) < 0.01)
        // {
        //     rx_forward_vel = 0;
        //     forward_vel = 0;
        //     state = sleep;
        // }
        break;
    case rotate:
        select_module(&rotate_in_place);
        forward_vel = 0;
        if (fabs(leg_phase_delta) < 0.01)
        {
            state = sleep;
            ang_vel = 0;
            rx_ang_vel = 0;
            // timer.trigger(4000);
        }
    default:
        break;
    }

    // forward_vel = 3;
    // move_action = &move_forward;
    // select_module(&move_forward);
    // ang_vel = 3;

    ALFA_update();
    for(size_t i = 0; i < 6; i++)
    {
        // motors[i].usePID(false);
        // motors[i].setSpeed(0);
        motors[i].setSpeed(10*modc(leg_angles_smoothed[i] - motors[i].getAngle(), 2*M_PI));
    }


    OUTPUT_update();
    // uint32_t now = millis();
    // Serial.println(last);
    // Serial.println(now);
    // Serial.println((uintptr_t)move_action);
    // Serial.println(leg_phase);
    // Serial.println(leg_phase_delta);
    // Serial.print(" ");
    // Serial.print(motors[0].getAngle());
    // Serial.print(" ");
    // Serial.print(motors[1].getAngle());
    // Serial.print(" ");
    // Serial.print(motors[2].getAngle());
    // Serial.print(" ");
    // Serial.print(motors[3].getAngle());
    // Serial.print(" ");
    // Serial.print(motors[4].getAngle());
    // Serial.print(" ");
    // Serial.println(motors[5].getAngle());
}
