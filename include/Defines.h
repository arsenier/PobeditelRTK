#pragma once
#include"Arduino.h"
//motor0
#define M0_INA 33
#define M0_INB 35
#define M0_PWM 5
#define M0_CS A2
#define M0_INTERRUPT_PIN 20 // PD1
#define M0_DIRECTION_PIN 22 // PA0

//motor1
#define M1_INA 39
#define M1_INB 37
#define M1_PWM 7
#define M1_CS A3
#define M1_INTERRUPT_PIN 21 // PD0
#define M1_DIRECTION_PIN 23 // PA1
//motor2
#define M2_INA 32
#define M2_INB 34
#define M2_PWM 4
#define M2_CS A4
#define M2_INTERRUPT_PIN 19 // PD2
#define M2_DIRECTION_PIN 24 // PA2
//motor3
#define M3_INA 43
#define M3_INB 41
#define M3_PWM 9
#define M3_CS A5
#define M3_INTERRUPT_PIN 18 // PD3
#define M3_DIRECTION_PIN 25 // PA3
//motor4
#define M4_INA 36
#define M4_INB 38
#define M4_PWM 6
#define M4_CS A0
#define M4_INTERRUPT_PIN 3  // PE5
#define M4_DIRECTION_PIN 26 // PA4
//motor5
#define M5_INA 42
#define M5_INB 40
#define M5_PWM 8
#define M5_CS A1
#define M5_INTERRUPT_PIN 2  // PE4
#define M5_DIRECTION_PIN 27 // PA5

//////////////////////////////////////////////

#define Ts_us                                   6000 /*!< sample time [microseconds]*/
#define Ts_s                                    ((float)Ts_us * 0.000001)

#define MOTORS_MAX_U                            12.0 /*!< [V] */
#define MOTORS_MOVE_U                           2  /*!< [V] */
#define MOTORS_MAX_SPEED                        30.0     //rad/second
#define MOTORS_KE                               0.185   /*!< [V/rad/s] */

#define MOTORS_PPR                              450     //Encoder pulses per rotation
#define MOTORS_GEAR_RATIO                       1
#define MOTORS_TIME_CONSTANT                    0.030
#define MOTORS_Tu                               Ts_s
#define MOTORS_ABS_OPTIMUM_SETTING              2.0
#define MOTORS_PI_KI                            (MOTORS_KE/(MOTORS_ABS_OPTIMUM_SETTING*MOTORS_Tu))
#define MOTORS_PI_GAIN                          (MOTORS_PI_KI*MOTORS_TIME_CONSTANT)

///////////////////////////////////////////////
