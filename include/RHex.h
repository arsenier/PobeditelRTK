#pragma once

#include <FixMath.h>

#define LOG(x) String(x) + " " + 

typedef SFix<12,12> sfix;

const auto tc = (sfix(2*M_PI));
const auto ts (sfix(1*M_PI));
const auto phis (sfix(1));
const auto phi0 (sfix(1));

inline sfix Fc(sfix t, sfix phists)
{
    // Serial.println("\n" + String(t.asFloat()) + " " + String(phists.asFloat()));
    return t*phists;
}

inline sfix Fr0(sfix t, sfix dydx)
{
    return t*dydx;
}

inline sfix Fcomp(sfix ts, sfix phists, sfix dydx)
{
    return Fc(ts*sfix(0.5), phists) - Fr0(ts*sfix(0.5), dydx);
}

inline sfix Fr(sfix t, sfix ts, sfix phists, sfix dydx)
{
    return Fr0(t, dydx) + Fcomp(ts, phists, dydx);
}

inline sfix Fl(sfix t, sfix ts, sfix phists, sfix dydx)
{
    return -Fr(-t, ts, phists, dydx);
}

inline sfix Ffull(sfix t, sfix tc, sfix ts, sfix phis, sfix phi0)
{
    // Serial.println(LOG(t.asFloat()) LOG(tc.asFloat()) LOG(ts.asFloat()) LOG(phis.asFloat()) LOG(phi0.asFloat()) "");
    // Serial.println(LOG(t < -ts*sfix(0.5)) LOG(t < ts*sfix(0.5)) "");
    // Serial.println(LOG(t.asFloat()) LOG((ts*sfix(0.5)).asFloat()) "");
    sfix out;
    auto phists = phis*ts.invFast();
    if(t < -ts*sfix(0.5))
    {
        auto dydx = (sfix(2*M_PI) - phis) * (tc - ts).invFast();
        out = Fl(t, ts, phists, dydx);
    }
    else if(t < ts*sfix(0.5))
    {
        out = Fc(t, phists);
    }
    else
    {
        auto dydx = (sfix(2*M_PI) - phis) * (tc - ts).invFast();
        out = Fr(t, ts, phists, dydx);
    }
    return out + phi0;
}

sfix modc(sfix in, sfix modder)
{
    in = in + modder;
    while(in > modder*sfix(0.5))
    {
        in = in - modder;
    }
    return in;
}
