//
//  MidiInstrumentControl.hpp
//  Midiot
//
//  Created by Sean Bratnober on 12/5/17.
//
//

#ifndef MidiInstrumentControl_hpp
#define MidiInstrumentControl_hpp

#include <stdio.h>


class MidiInstrumentControl
{
public:
    class ContinuousControl
    {
        
    };
    
    class SysexControl
    {
        
    };
    
    
private:
    ContinuousControl cc_def_;
    SysexControl sysex_def_;
};

#endif /* MidiInstrumentControl_hpp */
