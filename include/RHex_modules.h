#pragma once

#include "RHex.h"
#include "alfa4c.h"
#include "Defines.h"

extern sfix forward_vel;
extern sfix ang_vel;

ChannelSensor<sfix> forward_velocity_command_raw(&forward_vel), angular_velocity_command(&ang_vel);
ChannelComputed<sfix> forward_velocity_command([](){ return -forward_velocity_command_raw; });
ChannelLast<sfix> leg_phase, leg_phase_delta;
ChannelLast<sfix> leg_angles[6], leg_angles_smoothed[6];

MODULE(update_forw_phase,
    DRIVE(leg_phase, modc(leg_phase.get() + forward_velocity_command.get() * sfix(Ts_s), sfix(2*M_PI)))
)

MODULE(update_delta_phase,
    DRIVE(leg_phase_delta, modc(leg_phase_delta.get() + angular_velocity_command.get() * sfix(Ts_s), sfix(2*M_PI)))
)

MODULE(stand_up,
    for(size_t i = 0; i < 6; i++)
    {
        // DRIVE(leg_angles[i], phi0 + M_PI * (i%2))
        DRIVE(leg_angles[i], leg_angles[i] + constrain((phi0+ M_PI * (i%2) - leg_angles[i]), -2, 2)*Ts_s)
    }
    Serial.println("Standup");
    // DRIVE(leg_angles[1], )
    // DRIVE(leg_angles[2], )
    // DRIVE(leg_angles[3], )
    // DRIVE(leg_angles[4], )
    // DRIVE(leg_angles[5], )
)

MODULE(move_forward,
    DRIVE(leg_angles[0], modc(Ffull(leg_phase, tc, ts, phis, phi0), sfix(2*M_PI)))
    DRIVE(leg_angles[1], modc(Ffull(leg_phase.get() + sfix(M_PI), tc, ts, phis, phi0), sfix(2*M_PI)))
    DRIVE(leg_angles[2], modc(Ffull(leg_phase, tc, ts, phis, phi0), sfix(2*M_PI)))
    DRIVE(leg_angles[3], modc(Ffull(leg_phase.get() + sfix(M_PI), tc, ts, phis, phi0), sfix(2*M_PI)))
    DRIVE(leg_angles[4], modc(Ffull(leg_phase, tc, ts, phis, phi0), sfix(2*M_PI)))
    DRIVE(leg_angles[5], modc(Ffull(leg_phase.get() + sfix(M_PI), tc, ts, phis, phi0), sfix(2*M_PI)))
    Serial.println("Forward");
)

MODULE(rotate_in_place,
    DRIVE(leg_angles[0], modc(Ffull(leg_phase + leg_phase_delta, tc, ts, phis, phi0), sfix(2*M_PI)))
    DRIVE(leg_angles[1], modc(Ffull(leg_phase + sfix(M_PI) + leg_phase_delta, tc, ts, phis, phi0), sfix(2*M_PI)))
    DRIVE(leg_angles[2], modc(Ffull(leg_phase + leg_phase_delta, tc, ts, phis, phi0), sfix(2*M_PI)))
    DRIVE(leg_angles[3], modc(Ffull(leg_phase + sfix(M_PI) - leg_phase_delta, tc, ts, phis, phi0), sfix(2*M_PI)))
    DRIVE(leg_angles[4], modc(Ffull(leg_phase - leg_phase_delta, tc, ts, phis, phi0), sfix(2*M_PI)))
    DRIVE(leg_angles[5], modc(Ffull(leg_phase + sfix(M_PI) - leg_phase_delta, tc, ts, phis, phi0), sfix(2*M_PI)))
)

MODULE(move_soft,
    for(size_t i = 0; i < 6; i++)
    {
        // DRIVE(leg_angles_smoothed[i], leg_angles_smoothed[i] + constrain((leg_angles[i] - leg_angles_smoothed[i]), -2, 2)*Ts_s)
        DRIVE(leg_angles_smoothed[i], leg_angles[i])
    }
)

Updatable *links[] =
{
    &current_real_time_ms,
    &forward_velocity_command_raw,
    &forward_velocity_command,
    &angular_velocity_command,
    &update_forw_phase,
    &update_delta_phase,
    &leg_phase,
    &leg_phase_delta,
    // &move_forward,
    // &rotate_in_place,
    &stand_up,
    &leg_angles[0],
    &leg_angles[1],
    &leg_angles[2],
    &leg_angles[3],
    &leg_angles[4],
    &leg_angles[5],
    &move_soft,
    &leg_angles_smoothed[0],
    &leg_angles_smoothed[1],
    &leg_angles_smoothed[2],
    &leg_angles_smoothed[3],
    &leg_angles_smoothed[4],
    &leg_angles_smoothed[5],
};

#define LINK_SIZE (sizeof(links)/sizeof(links[0]))

void select_module(Module *module)
{
    links[8] = module;
}

void ALFA_update()
{
    for(size_t i = 0; i < LINK_SIZE; i++)
    {
        links[i]->update();
    }
}