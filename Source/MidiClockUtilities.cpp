//
//  MidiClockUtilities.cpp
//  Midiot
//
//  Created by Sean Bratnober on 10/14/17.
//
//

#include "MidiClockUtilities.hpp"


BarBeatTime::BarBeatTime(int bars,
            int beats,
            int sixteenths,
            int clock_ticks) :
bars_(bars),
beats_(beats),
sixteenths_(sixteenths),
clock_ticks_(clock_ticks)
{
}

BarBeatTime::~BarBeatTime()
{
}
