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

#include "../JuceLibraryCode/JuceHeader.h"


class TimeSignature
{
public:
    TimeSignature(short numerator = 4, short denominator = 2)
    : numerator_(numerator),
    denominator_(denominator)
    {
    }
    
    ~TimeSignature()
    {}
    
    String getDisplayString()
    {
        return String(numerator_) + "/" + String(pow(denominator_,2));
    }
    
    short numerator() { return numerator_; }
    short denominator() { return denominator_; }
    
private:
    short numerator_;
    short denominator_;
};

class BarBeatTime
{
public:
    enum TimeResolution {
        Bars = 0,
        Beats,
        Sixteenths,
        Ticks
    };
    
    BarBeatTime(int bars = 0,
                int beats = 0,
                int sixteenths = 0,
                int ticks = 0,
                int division_ppq = 24,
                TimeSignature time_signature = TimeSignature()) :
    bars_(bars),
    beats_(beats),
    sixteenths_(sixteenths),
    ticks_(ticks),
    division_ppq_(division_ppq),
    time_signature_(time_signature)
    {
    }
    
    ~BarBeatTime()
    {
    }
    
    void advanceTime(int time_resolution)
    {
        switch (time_resolution)
        {
            case TimeResolution::Bars:
                bars_++;
                break;
            case TimeResolution::Beats:
                beats_++;
                if (beats_ == time_signature_.numerator())
                {
                    beats_ = 0;
                    bars_++;
                }
                break;
            case TimeResolution::Sixteenths:
                sixteenths_++;
                if (sixteenths_ == 4)
                {
                    sixteenths_ = 0;
                    beats_++;
                    
                    if (beats_ == time_signature_.numerator())
                    {
                        beats_ = 0;
                        bars_++;
                    }
                }
                break;
            case TimeResolution::Ticks:
                ticks_++;
                
                if (ticks_ == division_ppq_ / 4)
                {
                    ticks_ = 0;
                    sixteenths_++;
                    if (sixteenths_ == 4)
                    {
                        sixteenths_ = 0;
                        beats_++;
                        
                        if (beats_ == time_signature_.numerator())
                        {
                            beats_ = 0;
                            bars_++;
                        }
                    }
                }
                break;
        };
    }
    
    void setBarBeatTime(int bars, int beats, int sixteenths, int ticks)
    {
        bars_ = bars;
        beats_ = beats;
        sixteenths_ = sixteenths;
        ticks_ = ticks;
    }
    
    
    
    String getDisplayString(int time_resolution = TimeResolution::Ticks, bool truncateFirstCount = true)
    {
        String display_string("");
        
        switch (time_resolution) {
            case TimeResolution::Bars:
                display_string += String(bars_+1);
                break;
            case TimeResolution::Beats:
                display_string += String(bars_+1);
                
                if (beats_ || !truncateFirstCount)
                {
                    display_string += "." + String(beats_+1);
                }
                
                break;
            case TimeResolution::Sixteenths:
                display_string += String(bars_+1)
                + "." + String(beats_+1)
                + "." + String(sixteenths_+1);
                break;
            case TimeResolution::Ticks:
            default:
                display_string += String(bars_+1)
                + "." + String(beats_+1)
                + "." + String(sixteenths_+1)
                + "." + String(ticks_+1);
                break;
        };
        
        return display_string;
    }
    
private:
    int bars_;
    int beats_;
    int sixteenths_;
    int ticks_;
    int division_ppq_;
    
    TimeSignature time_signature_;
};



#endif /* MidiClockUtilities_hpp */
