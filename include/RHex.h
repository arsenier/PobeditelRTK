#pragma once

#include <FixMath.h>

#define LOG(x) String(x) + " " + 

typedef float sfix;

const auto tc = (sfix(2*M_PI));
const auto ts (sfix(2*2/3.6*M_PI));
const auto phis (sfix(1.5));
const auto phi0 (sfix(-2));

sfix modc(sfix in, sfix modder)
{
    in = in + modder;
    while(in > modder*sfix(0.5))
    {
        in = in - modder;
    }
    return in;
}


inline sfix Fc(sfix t, sfix phists)
{
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
    t = modc(t, tc);
    sfix out;
    auto phists = phis/ts;
    if(t < -ts*sfix(0.5))
    {
        auto dydx = (sfix(2*M_PI) - phis) / (tc - ts);
        out = Fl(t, ts, phists, dydx);
    }
    else if(t < ts*sfix(0.5))
    {
        out = Fc(t, phists);
    }
    else
    {
        auto dydx = (sfix(2*M_PI) - phis) / (tc - ts);
        out = Fr(t, ts, phists, dydx);
    }
    return out + phi0;
}