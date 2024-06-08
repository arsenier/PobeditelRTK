#pragma once

#include "RHex.h"
#include "alfa4c.h"
#include "Defines.h"

extern sfix forward_vel;

ChannelSensor<sfix> forward_velocity_command(&forward_vel);
ChannelLast<sfix> leg_phase;
ChannelLast<sfix> leg_angles[6];

MODULE(update_phase,
    DRIVE(leg_phase, modc(leg_phase.get() + forward_velocity_command.get() * sfix(Ts_s), sfix(2*M_PI)))
)

MODULE(move_forward,
    DRIVE(leg_angles[0], modc(Ffull(leg_phase, tc, ts, phis, phi0), sfix(2*M_PI)))
    DRIVE(leg_angles[1], modc(Ffull(leg_phase.get() + sfix(M_PI), tc, ts, phis, phi0), sfix(2*M_PI)))
    DRIVE(leg_angles[2], modc(Ffull(leg_phase, tc, ts, phis, phi0), sfix(2*M_PI)))
    DRIVE(leg_angles[3], modc(Ffull(leg_phase.get() + sfix(M_PI), tc, ts, phis, phi0), sfix(2*M_PI)))
    DRIVE(leg_angles[4], modc(Ffull(leg_phase, tc, ts, phis, phi0), sfix(2*M_PI)))
    DRIVE(leg_angles[5], modc(Ffull(leg_phase.get() + sfix(M_PI), tc, ts, phis, phi0), sfix(2*M_PI)))
)



Updatable *links[] =
{
    &forward_velocity_command,
    &update_phase,
    &leg_phase,
    &move_forward,
    &leg_angles[0],
    &leg_angles[1],
    &leg_angles[2],
    &leg_angles[3],
    &leg_angles[4],
    &leg_angles[5],
};

void ALFA_update()
{
    for(auto l : links)
    {
        l->update();
    }
}