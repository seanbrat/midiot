//
//  MidiClockUtilities.hpp
//  Midiot
//
//  Created by Sean Bratnober on 10/14/17.
//
//

#ifndef MidiClockUtilities_hpp
#define MidiClockUtilities_hpp

#include <stdio.h>


class BarBeatTime
{
public:
    BarBeatTime(int bars = 1,
                int beats = 0,
                int sixteenths = 0,
                int clock_ticks = 0);
    
    ~BarBeatTime();
    
    int bars_;
    int beats_;
    int sixteenths_;
    int clock_ticks_;
};



#endif /* MidiClockUtilities_hpp */
